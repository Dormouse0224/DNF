// DNF.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "DNF.h"
#include "CEngine.h"
#include "CDbgRender.h"
#include "CTextureMgr.h"
#include "CTexture.h"

// 전역 변수:
HINSTANCE hInst;                // 현재 인스턴스입니다.
bool IsProgressFin = false;     // 프로그레스 종료 여부

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void MainProgress(HACCEL hAccelTable);
void BackgroundLoad();
void BackgroundReadFile();


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    // 메모리 누수 체크
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    hInst = hInstance;

    // 윈도우 클래스 생성 및 정보 입력
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInst;
    wcex.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_DNF));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr/*MAKEINTRESOURCEW(IDC_DNF)*/;
    wcex.lpszClassName = L"main_window";
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    RegisterClassExW(&wcex);

    // 단축키 테이블
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DNF));

    // 백그라운드 로딩 스레드와 프로그레스 표시 스레드 시작
    std::thread readfileThread(BackgroundReadFile);
    std::thread loadingThread(BackgroundLoad);
    std::thread progressThread(MainProgress, hAccelTable);

    // 스레드 종료 대기
    if (readfileThread.joinable()) {
        readfileThread.join();
    }
    if (loadingThread.joinable()) {
        loadingThread.join();
    }
    if (progressThread.joinable()) {
        progressThread.join();
    }
   
    return 0;
}


void MainProgress(HACCEL hAccelTable)
{
    // Engine 초기화
    if (FAILED(CEngine::GetInst()->Init(hInst)))
        return;


    //메세지 루프
    MSG msg;


    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                IsProgressFin = true;
                loadCV.notify_all();
                break;
            }

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        else
        {
            CEngine::GetInst()->Progress();
        }
    }
}

void CALLBACK LoadSceneThread(PTP_CALLBACK_INSTANCE instance, PVOID context, PTP_WORK work)
{
    std::pair<CAlbum*, int>* data = static_cast<std::pair<CAlbum*, int>*>(context);
    data->first->GetScene(data->second)->Load();
    delete data; // 할당된 메모리 해제
}

int GetPhysicalCoreCount() {
    DWORD bufferSize = 0;
    GetLogicalProcessorInformationEx(RelationProcessorCore, nullptr, &bufferSize);

    std::vector<uint8_t> buffer(bufferSize);
    if (!GetLogicalProcessorInformationEx(RelationProcessorCore,
        reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(buffer.data()),
        &bufferSize)) {
        return -1; // 오류 발생
    }

    int coreCount = 0;
    auto ptr = buffer.data();
    while (ptr < buffer.data() + bufferSize) {
        PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX info =
            reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(ptr);
        if (info->Relationship == RelationProcessorCore) {
            coreCount++;
        }
        ptr += info->Size;
    }
    return coreCount;
}

std::mutex loadMutex;
void BackgroundLoad()
{
    // 스레드 풀 생성
    PTP_POOL pool = CreateThreadpool(nullptr);
    if (!pool)
        return;

    // 스레드 풀 환경 설정
    TP_CALLBACK_ENVIRON callbackEnv;
    InitializeThreadpoolEnvironment(&callbackEnv);
    SetThreadpoolThreadMaximum(pool, GetPhysicalCoreCount() + 1);
    SetThreadpoolCallbackPool(&callbackEnv, pool);

    while (!IsProgressFin)
    {
        {
            // 작업이 없는 경우 대기상태로 전환
            std::unique_lock<std::mutex> lock(loadMutex);
            loadCV.wait(lock, [] {return !LoadQueue.empty() || IsProgressFin; });
        }

        std::unique_lock<std::mutex> lock(queueMutex);
        if (!LoadQueue.empty())
        {
            CAlbum* Data = LoadQueue.front();
            lock.unlock();

            // 각 씬에 대해 작업을 큐에 추가
            std::vector<PTP_WORK> workItems;
            for (int i = 0; i < Data->GetSceneCount(); ++i) {
                auto* data = new std::pair<CAlbum*, int>(Data, i);
                PTP_WORK work = CreateThreadpoolWork(LoadSceneThread, data, &callbackEnv);
                if (work) {
                    workItems.push_back(work);
                    SubmitThreadpoolWork(work);
                }
                else {
                    delete data;
                }
            }

            // 모든 작업이 완료될 때까지 대기
            for (PTP_WORK work : workItems) {
                WaitForThreadpoolWorkCallbacks(work, FALSE);
                CloseThreadpoolWork(work);
            }

            

            std::unique_lock<std::mutex> lock1(queueMutex);
            LoadQueue.pop_front();
            lock1.unlock();
        }
    }

    // 스레드 풀 및 환경 정리
    DestroyThreadpoolEnvironment(&callbackEnv);
    CloseThreadpool(pool);
}

void BackgroundReadFile()
{
    while (true)
    {
        if (IsProgressFin)
            break;

        std::unique_lock<std::mutex> lock(queueMutex1);
        if (!ReadQueue.empty())
        {
            wstring Data = ReadQueue.front();

            CTextureMgr::PreloadAvatar(Data);

            ReadQueue.pop_front();
            lock.unlock();
        }
    }
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
//bool EditorMenu(HINSTANCE _hInst, HWND _hWnd, int _wmID);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);

            //if (EditorMenu(hInst, hWnd, wmId))
            //    break;

            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case DBGRENDER_TOGGLE:
                CDbgRender::GetInst()->ToggleDbgRender();
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_DESTROY:
    {
        PostQuitMessage(0);

        // 종료 전 창 크기와 위치 기억
        char buff[255] = {};
        GetModuleFileNameA(nullptr, buff, 255);
        ofstream DataFile;
        string str = string(buff).substr(0, string(buff).rfind('\\')) + "\\DNF_program.data";
        DataFile.open(str, ios::binary);
        assert(DataFile.is_open());
        ProgramInfo info;
        info.Resolution = Vec2D(CEngine::GetInst()->GetResolution().x, CEngine::GetInst()->GetResolution().y);
        RECT rect;
        GetWindowRect(CEngine::GetInst()->GetMainWnd(), &rect);
        info.WindowPos = Vec2D(rect.left, rect.top);
        info.Scale = CEngine::GetInst()->GetScreenScale();
        DataFile.write((char*)&info, sizeof(info));
        DataFile.close();
    }
        break;
    case WM_GETMINMAXINFO:
    {
        MINMAXINFO* minMaxInfo = (MINMAXINFO*)lParam;

        if (CEngine::GetInst()->GetWindowSize() != Vec2D(0, 0))
        // 윈도우의 최소 및 최대 크기를 설정
        {
            minMaxInfo->ptMinTrackSize.x = (LONG)CEngine::GetInst()->GetWindowSize().x;
            minMaxInfo->ptMinTrackSize.y = (LONG)CEngine::GetInst()->GetWindowSize().y;
            minMaxInfo->ptMaxTrackSize.x = (LONG)CEngine::GetInst()->GetWindowSize().x;
            minMaxInfo->ptMaxTrackSize.y = (LONG)CEngine::GetInst()->GetWindowSize().y;
        }
    }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
