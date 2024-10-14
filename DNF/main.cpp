// DNF.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "DNF.h"
#include "CEngine.h"
#include "CDbgRender.h"

// 전역 변수:
HINSTANCE hInst;    // 현재 인스턴스입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_DNF);
    wcex.lpszClassName = L"main_window";
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    RegisterClassExW(&wcex);


    // Engine 초기화
    if (FAILED(CEngine::GetInst()->Init(hInst)))
        return FALSE;

    // 단축키 테이블
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DNF));

    //메세지 루프
    MSG msg;


    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;
            
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

    return (int) msg.wParam;
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
bool EditorMenu(HINSTANCE _hInst, HWND _hWnd, int _wmID);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);

            if (EditorMenu(hInst, hWnd, wmId))
                break;

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

        // 윈도우의 최소 및 최대 크기를 설정
        minMaxInfo->ptMinTrackSize.x = CEngine::GetInst()->GetResolution().x;
        minMaxInfo->ptMinTrackSize.y = CEngine::GetInst()->GetResolution().y;
        minMaxInfo->ptMaxTrackSize.x = CEngine::GetInst()->GetResolution().x;
        minMaxInfo->ptMaxTrackSize.y = CEngine::GetInst()->GetResolution().y;
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
