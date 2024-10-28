#include "pch.h"
#include "DialogProc.h"
#include "CEngine.h"
#include "resource.h"
#include "CAlbum.h"
#include "CTextureMgr.h"
#include "CTexture.h"
#include "CLevelMgr.h"
#include "CLevel_Edit.h"
#include "CAlbumPlayer.h"
#include "CDungeonMaker.h"
#include "CWall.h"
#include "CNPC.h"
#include "CMonster.h"
#include "CSticker.h"
#include "CMonster.h"
#include "CStageMaker.h"

#include <Commctrl.h>


// 앨범 뷰어 다이얼로그 프로시저 함수
INT_PTR CALLBACK AlbumViewerProc(HWND hDlg, UINT message, WPARAM _wParam, LPARAM _lParam)
{
    UNREFERENCED_PARAMETER(_lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(_wParam) == IDOK || LOWORD(_wParam) == IDCANCEL)
        {
            // 종료시 화면 초기화
            CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
            CLevel_Edit* pEditLv = dynamic_cast<CLevel_Edit*>(pLevel);
            if (pEditLv)
            {
                pEditLv->SetPreviewTexture(nullptr);
            }
            EndDialog(hDlg, LOWORD(_wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(_wParam) == BTN_LoadNPK)
        {
            // NPK 파일 불러오기 버튼이 클릭된 경우
            // 파일 탐색기 초기화
            WCHAR filepath[255] = {};
            WCHAR filename[255] = {};
            WCHAR InitDir[255] = {};
            wstring wsInitDir = CEngine::GetInst()->GetResourcePathW() + L"\\texture";
            //const wchar_t wcInitDir = wsInitDir.c_str();
           
            OPENFILENAME Desc = {};
            Desc.lStructSize = sizeof(OPENFILENAME);
            Desc.hwndOwner = hDlg;
            Desc.lpstrFilter = L"NPK\0*.NPK\0ALL\0*.*\0";
            Desc.lpstrFile = filepath;
            Desc.nMaxFile = 255;
            Desc.lpstrFileTitle = filename;
            Desc.nMaxFileTitle = 255;
            Desc.lpstrInitialDir = wsInitDir.c_str();
            Desc.lpstrTitle = L"NPK 파일 불러오기";
            Desc.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

            if (GetOpenFileName(&Desc))
            {
                // 파일 탐색기에서 NPK 파일을 가져온 뒤 NPK 파일 안의 앨범을 앨범 리스트박스에 넣음
                HWND hAlbumLBX = GetDlgItem(hDlg, LBX_AlbumList);
                HWND hNPKDir = GetDlgItem(hDlg, STATIC_NPKDir);
                HWND hTexLBX = GetDlgItem(hDlg, LBX_TextureList);
                SendMessage(hAlbumLBX, LB_RESETCONTENT, 0, 0);  // 앨범 리스트박스 초기화
                SendMessage(hTexLBX, LB_RESETCONTENT, 0, 0);    // 텍스처 리스트박스 초기화
                SetWindowText(hNPKDir, filepath);
                wstring npkdir = filepath;
                vector<CAlbum*> AlbumVec = CTextureMgr::GetInst()->LoadNPK(npkdir);
                for (CAlbum* pAlbum : AlbumVec)
                {
                    string lpar = pAlbum->GetPath();
                    wstring wstr(lpar.begin(), lpar.end());
                    SendMessage(hAlbumLBX, LB_ADDSTRING, 0, (LPARAM)wstr.c_str());
                }
            }
        }
        else if (HIWORD(_wParam) == LBN_DBLCLK && LOWORD(_wParam) == LBX_AlbumList)
        {
            // 앨범 리스트박스 항목을 더블클릭했을 경우
            HWND hAlbumLBX = GetDlgItem(hDlg, LBX_AlbumList);
            HWND hTexLBX = GetDlgItem(hDlg, LBX_TextureList);
            HWND hNPKDir = GetDlgItem(hDlg, STATIC_NPKDir);
            HWND hOwnALbum = GetDlgItem(hDlg, STATIC_OwnerAlbum);

            WCHAR npkdir[255] = {};
            GetWindowText(hNPKDir, npkdir, 255);

            SendMessage(hTexLBX, LB_RESETCONTENT, 0, 0);    // 텍스처 리스트박스 초기화

            // 선택한 항목 파일명 가져오기
            LRESULT index = SendMessage(hAlbumLBX, LB_GETCURSEL, 0, 0);
            TCHAR buffer[255] = {};
            SendMessage(hAlbumLBX, LB_GETTEXT, index, (LPARAM)buffer);
            wstring filename = buffer;

            CAlbum* pAlbum = CTextureMgr::GetInst()->LoadAlbum(string(filename.begin(), filename.end()), npkdir);
            SetWindowText(hOwnALbum, buffer);
            for (int i = 0; i < pAlbum->GetCount(); ++i)
            {
                CTexture* pTex = pAlbum->GetScene(i);
                SendMessage(hTexLBX, LB_ADDSTRING, 0, (LPARAM)pTex->GetName().c_str());
            }

        }
        else if (HIWORD(_wParam) == LBN_SELCHANGE && LOWORD(_wParam) == LBX_TextureList)
        {
            // 텍스처 리스트박스 항목이 클릭됨
            HWND hTexLBX = GetDlgItem(hDlg, LBX_TextureList);
            HWND hOwnALbum = GetDlgItem(hDlg, STATIC_OwnerAlbum);
            HWND hNPKDir = GetDlgItem(hDlg, STATIC_NPKDir);
            HWND hWidth = GetDlgItem(hDlg, STATIC_Width);
            HWND hHeight = GetDlgItem(hDlg, STATIC_Height);

            // album path
            WCHAR ownalb[255] = {};
            GetWindowText(hOwnALbum, ownalb, 255);
            wstring wstr = ownalb;

            // npk directory
            WCHAR npkdir[255] = {};
            GetWindowText(hNPKDir, npkdir, 255);

            LRESULT index = SendMessage(hTexLBX, LB_GETCURSEL, 0, 0);

            CAlbum* pAlbum = CTextureMgr::GetInst()->GetAlbum(string(wstr.begin(), wstr.end()));
            WCHAR buffer[255] = {};
            SendMessage(hTexLBX, LB_GETTEXT, index, (LPARAM)buffer);
            CTexture* pTex = pAlbum->GetScene(buffer);

            // 텍스처 크기정보 표시
            SetWindowText(hWidth, std::to_wstring(pTex->GetSize().x).c_str());
            SetWindowText(hHeight, std::to_wstring(pTex->GetSize().y).c_str());

            CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
            CLevel_Edit* pEditLv = dynamic_cast<CLevel_Edit*>(pLevel);
            if (pEditLv && pTex)
            {
                pEditLv->SetPreviewTexture(pTex);
            }
        }
        else if (LOWORD(_wParam) == BTN_CreateAni)
        {
            // 애니메이션 생성 버튼
            HWND hNPKDir = GetDlgItem(hDlg, STATIC_NPKDir);
            HWND hOwnerAlbum = GetDlgItem(hDlg, STATIC_OwnerAlbum);
            HWND hSelectedNPK = GetDlgItem(hDlg, STATIC_SelectedNpk);
            HWND hSelectedAlbum = GetDlgItem(hDlg, STATIC_SelectedAlbum);

            WCHAR buffer1[255] = {};
            WCHAR buffer2[255] = {};
            GetWindowText(hNPKDir, buffer1, 255);
            GetWindowText(hOwnerAlbum, buffer2, 255);

            // 입력값 예외처리
            if (wstring(buffer1).empty() || wstring(buffer2).empty())
            {
                // 필수 입력값이 전부 입력되지 않았음
                MessageBox(hDlg, L"입력값이 필요합니다.", L"입력 오류", MB_ICONWARNING | MB_OK);
                break;
            }

            //SendMessage(hSelectedNPK, WM_SETTEXT, 0, (LPARAM)buffer1);
            //SendMessage(hSelectedAlbum, WM_SETTEXT, 0, (LPARAM)buffer2);

            DialogBox(CEngine::GetInst()->GetProgramInst(), MAKEINTRESOURCE(DLG_CreateAni), hDlg, &CreateAniProc);
        }
        break;
        case WM_DESTROY:
        {
            // 종료시 화면 초기화
            CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
            CLevel_Edit* pEditLv = dynamic_cast<CLevel_Edit*>(pLevel);
            if (pEditLv)
            {
                pEditLv->SetPreviewTexture(nullptr);
            }
            break;
        }
    }
    return (INT_PTR)FALSE;
}

// 애니메이션 인포 파일 작성 프로시저
INT_PTR CALLBACK CreateAniProc(HWND hDlg, UINT message, WPARAM _wParam, LPARAM _lParam)
{
    switch (message)
    {
    case(WM_INITDIALOG):
    {
        HWND hNPKDir = GetDlgItem(GetParent(hDlg), STATIC_NPKDir);
        HWND hOwnerAlbum = GetDlgItem(GetParent(hDlg), STATIC_OwnerAlbum);
        HWND hSelectedNPK = GetDlgItem(hDlg, STATIC_SelectedNpk);
        HWND hSelectedAlbum = GetDlgItem(hDlg, STATIC_SelectedAlbum);

        WCHAR buffer1[255] = {};
        WCHAR buffer2[255] = {};
        GetWindowText(hNPKDir, buffer1, 255);
        GetWindowText(hOwnerAlbum, buffer2, 255);
        SendMessage(hSelectedNPK, WM_SETTEXT, 0, (LPARAM)buffer1);
        SendMessage(hSelectedAlbum, WM_SETTEXT, 0, (LPARAM)buffer2);
    }
    case(WM_COMMAND):
        if (LOWORD(_wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(_wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(_wParam) == IDOK)
        {
            char cNPKPath[255] = {};
            char cOwnerAlbum[255] = {};
            GetDlgItemTextA(hDlg, STATIC_SelectedNpk, cNPKPath, 255);
            GetDlgItemTextA(hDlg, STATIC_SelectedAlbum, cOwnerAlbum, 255);

            WCHAR IndexBegin[255] = {};
            WCHAR IndexEnd[255] = {};
            WCHAR FPS[255] = {};
            WCHAR OffsetX[255] = {};
            WCHAR OffsetY[255] = {};
            WCHAR angle[255] = {};
            GetDlgItemText(hDlg, EDIT_IndexBegin, IndexBegin, 255);
            GetDlgItemText(hDlg, EDIT_IndexEnd, IndexEnd, 255);
            GetDlgItemText(hDlg, EDIT_FPS, FPS, 255);
            GetDlgItemText(hDlg, EDIT_OffsetX, OffsetX, 255);
            GetDlgItemText(hDlg, EDIT_OffsetY, OffsetY, 255);
            GetDlgItemText(hDlg, EDIT_Angle, angle, 255);

            // 입력값 예외처리
            if (string(cNPKPath).empty() || string(cOwnerAlbum).empty() || wstring(IndexBegin).empty()
                || wstring(IndexEnd).empty() || wstring(FPS).empty() || wstring(OffsetX).empty()
                || wstring(OffsetY).empty() || wstring(angle).empty())
            {
                // 필수 입력값이 전부 입력되지 않았음
                MessageBox(hDlg, L"입력값이 필요합니다.", L"입력 오류", MB_ICONWARNING | MB_OK);
                break;
            }

            WCHAR filepath[255] = {};
            WCHAR filename[255] = {};
            WCHAR buff[255] = {};
            GetModuleFileName(nullptr, buff, 255);
            wstring wstr = wstring(buff).substr(0, wstring(buff).rfind('\\')) + L"\\resource\\animation";
            OPENFILENAME Desc = {};
            Desc.lStructSize = sizeof(OPENFILENAME);
            Desc.hwndOwner = hDlg;
            Desc.lpstrFilter = L"animation\0*.animation\0ALL\0*.*\0";
            Desc.lpstrFile = filepath;
            Desc.nMaxFile = 255;
            Desc.lpstrFileTitle = filename;
            Desc.nMaxFileTitle = 255;
            Desc.lpstrInitialDir = wstr.c_str();
            Desc.lpstrTitle = L"애니메이션 파일 저장";
            Desc.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
            Desc.lpstrDefExt = L"animation";

            if (GetSaveFileName(&Desc))
            {
                // 다이얼로그 윈도우의 에디트 컨트롤 값과 NPK, Album 정보를 기반으로 애니메이션 인포 파일 작성
                
                string NPKDir = cNPKPath;
                string OwnerAlbum = cOwnerAlbum;
                NPKDir = NPKDir.substr(CEngine::GetInst()->GetResourcePathA().size());

                // 입력된 정보를 구조체로 저장
                AnimationInfo animationInfo;
                animationInfo.NPKDirLen = (int)NPKDir.size();
                animationInfo.AlbumPathLen = (int)OwnerAlbum.size();
                animationInfo.IndexBegin = stoi(wstring(IndexBegin));
                animationInfo.IndexEnd = stoi(wstring(IndexEnd));
                animationInfo.FPS = stoi(wstring(FPS));
                animationInfo.Offset = Vec2D(stof(wstring(OffsetX)), stof(wstring(OffsetY)));
                animationInfo.angle = stof(wstring(angle));

                HWND hLoop = GetDlgItem(hDlg, CHECK_LOOP);
                if (SendMessage(hLoop, BM_GETCHECK, 0, 0) == BST_CHECKED)
                {
                    animationInfo.bLoop = true;
                }
                else
                {
                    animationInfo.bLoop = false;
                }
                HWND hDodge = GetDlgItem(hDlg, CHECK_Dodge);
                if (SendMessage(hDodge, BM_GETCHECK, 0, 0) == BST_CHECKED)
                {
                    animationInfo.bDodge = true;
                }
                else
                {
                    animationInfo.bDodge = false;
                }
                
                // 구조체로 저장된 정보를 파일로 쓰기
                ofstream writeFile;
                writeFile.open(filepath, ios::binary);

                writeFile.write((char*)&animationInfo, sizeof(animationInfo));
                writeFile.write(NPKDir.c_str(), NPKDir.size());
                writeFile.write(OwnerAlbum.c_str(), OwnerAlbum.size());

                writeFile.close();
            }

            EndDialog(hDlg, LOWORD(_wParam));
            return (INT_PTR)TRUE;
        }
    }
    return (INT_PTR)FALSE;
}

// 앨범 작성 프로시저
INT_PTR CALLBACK CreateAlbumProc(HWND hDlg, UINT message, WPARAM _wParam, LPARAM _lParam)
{
    UNREFERENCED_PARAMETER(_lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(_wParam) == IDOK || LOWORD(_wParam) == IDCANCEL)
        {
            // 종료시 화면 초기화
            CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
            CLevel_Edit* pEditLv = dynamic_cast<CLevel_Edit*>(pLevel);
            if (pEditLv)
            {
                pEditLv->SetPreviewTexture(nullptr);
            }

            // TempAlbum 초기화
            CTextureMgr::GetInst()->ClearTempAlbum();

            EndDialog(hDlg, LOWORD(_wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(_wParam) == BTN_AddImg)
        {
            // 이미지 추가 버튼이 클릭된 경우

            // 파일 탐색기 초기화
            WCHAR filepath[255] = {};
            WCHAR filename[255] = {};
            OPENFILENAME OpenNPK = {};
            OpenNPK.lStructSize = sizeof(OPENFILENAME);
            OpenNPK.hwndOwner = hDlg;
            OpenNPK.lpstrFilter = L"png\0*.png\0ALL\0*.*\0";
            OpenNPK.lpstrFile = filepath;
            OpenNPK.nMaxFile = 255;
            OpenNPK.lpstrFileTitle = filename;
            OpenNPK.nMaxFileTitle = 255;
            OpenNPK.lpstrTitle = L"이미지 파일 불러오기";
            OpenNPK.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

            if (GetOpenFileName(&OpenNPK))
            {
                // 파일 탐색기에서 이미지 파일을 가져온 뒤 앨범 리스트박스에 넣음
                HWND hAlbumLBX = GetDlgItem(hDlg, LBX_CustomAlbum);
                wstring ImageDir = filepath;
                CTexture* pTex = CTextureMgr::GetInst()->LoadFromFile(filepath);
                LRESULT index = SendMessage(hAlbumLBX, LB_ADDSTRING, 0, (LPARAM)pTex->GetName().c_str());
            }
        }
        else if (HIWORD(_wParam) == LBN_SELCHANGE && LOWORD(_wParam) == LBX_CustomAlbum)
        {
            // 앨범 리스트박스 항목을 클릭했을 경우
            HWND hAlbumLBX = GetDlgItem(hDlg, LBX_CustomAlbum);

            LRESULT index = SendMessage(hAlbumLBX, LB_GETCURSEL, 0, 0);
            WCHAR buffer[255] = {};
            SendMessage(hAlbumLBX, LB_GETTEXT, index, (LPARAM)buffer);
            CTexture* pTex = CTextureMgr::GetInst()->GetTempAlbum()->GetScene(buffer);

            // 현재 레벨이 에디터 레벨인지 확인 후 맞다면 미리보기 실행
            CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
            CLevel_Edit* pEditLv = dynamic_cast<CLevel_Edit*>(pLevel);
            if (pEditLv && pTex)
            {
                pEditLv->SetPreviewTexture(pTex);
            }
            SetDlgItemText(hDlg, EDIT_OffsetX, std::to_wstring(pTex->GetOffset().x).c_str());
            SetDlgItemText(hDlg, EDIT_OffsetY, std::to_wstring(pTex->GetOffset().y).c_str());
        }
        else if (LOWORD(_wParam) == BTN_SaveAlbum)
        {
            // NPK로 저장하기 버튼이 눌린 경우

            // 파일 탐색기 초기화
            WCHAR filepath[255] = {};
            WCHAR filename[255] = {};
            OPENFILENAME Desc = {};
            Desc.lStructSize = sizeof(OPENFILENAME);
            Desc.hwndOwner = hDlg;
            Desc.lpstrFilter = L"NPK\0*.NPK\0ALL\0*.*\0";
            Desc.lpstrFile = filepath;
            Desc.nMaxFile = 255;
            Desc.lpstrFileTitle = filename;
            Desc.nMaxFileTitle = 255;
            Desc.lpstrTitle = L"앨범을 NPK로 저장";
            Desc.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
            Desc.lpstrDefExt = L"NPK";

            if (GetSaveFileName(&Desc))
            {
                HWND hAlbumLBX = GetDlgItem(hDlg, LBX_CustomAlbum);

                // 지정경로에 지정된 이름으로 NPK 파일로 앨범 저장
                wstring Filename = filename;
                Filename = Filename.substr(0, Filename.find(L".NPK"));
                wstring Filepath = filepath;
                CTextureMgr::GetInst()->SaveAlbum(string().assign(Filename.begin(), Filename.end()), string().assign(Filepath.begin(), Filepath.end()));

                // 앨범 리스트박스 초기화
                SendMessage(hAlbumLBX, LB_RESETCONTENT, 0, 0);

                // TempAlbum 초기화
                CTextureMgr::GetInst()->ClearTempAlbum();
            }
        }
        else if (LOWORD(_wParam) == BTN_ChangeOffset)
        {
            HWND hLBX = GetDlgItem(hDlg, LBX_CustomAlbum);

            LRESULT index = SendMessage(hLBX, LB_GETCURSEL, 0, 0);
            WCHAR buffer[255] = {};
            SendMessage(hLBX, LB_GETTEXT, index, (LPARAM)buffer);
            CTexture* pTex = CTextureMgr::GetInst()->GetTempAlbum()->GetScene(buffer);
            char buffer1[255] = {};
            char buffer2[255] = {};
            GetDlgItemTextA(hDlg, EDIT_OffsetY, buffer2, 255);
            GetDlgItemTextA(hDlg, EDIT_OffsetX, buffer1, 255);
            pTex->SetOffset(stof(string(buffer1)), stof(string(buffer2)));
        }
        break;
    case WM_DESTROY:
    {
        // 종료시 화면 초기화
        CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
        CLevel_Edit* pEditLv = dynamic_cast<CLevel_Edit*>(pLevel);
        if (pEditLv)
        {
            pEditLv->SetPreviewTexture(nullptr);
        }

        // TempAlbum 초기화
        CTextureMgr::GetInst()->ClearTempAlbum();
        break;
    }
    }
    return (INT_PTR)FALSE;
}

INT_PTR EditAlimationProc(HWND hDlg, UINT message, WPARAM _wParam, LPARAM _lParam)
{
    switch (message)
    {
    case(WM_COMMAND):
    {
        if (LOWORD(_wParam) == BTN_LoadAni)
        {
            // 파일 탐색기 초기화
            WCHAR filepath[255] = {};
            WCHAR filename[255] = {};
            wstring initpath = CEngine::GetInst()->GetResourcePathW() + L"\\animation";
            OPENFILENAME desc = {};
            desc.lStructSize = sizeof(OPENFILENAME);
            desc.hwndOwner = hDlg;
            desc.lpstrFilter = L"animation\0*.animation\0ALL\0*.*\0";
            desc.lpstrFile = filepath;
            desc.nMaxFile = 255;
            desc.lpstrFileTitle = filename;
            desc.nMaxFileTitle = 255;
            desc.lpstrTitle = L"애니메이션 파일 불러오기";
            desc.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
            desc.lpstrInitialDir = initpath.c_str();

            if (GetOpenFileName(&desc))
            {
                // 파일로부터 정보를 읽어서 표시
                AnimationInfo info;
                char NPKDir[255] = {};
                char AlbumPath[255] = {};
                ifstream infofile;
                infofile.open(filepath, ios::binary);
                infofile.read((char*)&info, sizeof(info));
                infofile.read(NPKDir, info.NPKDirLen);
                infofile.read(AlbumPath, info.AlbumPathLen);
                infofile.close();
                WCHAR wNPKDir[255] = {};
                WCHAR wAlbumPath[255] = {};
                MultiByteToWideChar(CP_ACP, 0, NPKDir, -1, wNPKDir, 255);
                MultiByteToWideChar(CP_ACP, 0, AlbumPath, -1, wAlbumPath, 255);


                SetWindowText(GetDlgItem(hDlg, STATIC_TargetNpk), wNPKDir);
                SetWindowText(GetDlgItem(hDlg, STATIC_TargetAlbum), wAlbumPath);
                SetWindowText(GetDlgItem(hDlg, STATIC_TargetFile), filepath);
                SetWindowText(GetDlgItem(hDlg, STATIC_IndexBegin), std::to_wstring(info.IndexBegin).c_str());
                SetWindowText(GetDlgItem(hDlg, STATIC_IndexEnd), std::to_wstring(info.IndexEnd).c_str());
                SetWindowText(GetDlgItem(hDlg, EDIT_EditFPS), std::to_wstring(info.FPS).c_str());
                SetWindowText(GetDlgItem(hDlg, EDIT_EditOffsetX), std::to_wstring(info.Offset.x).c_str());
                SetWindowText(GetDlgItem(hDlg, EDIT_EditOffsetY), std::to_wstring(info.Offset.y).c_str());
                SetWindowText(GetDlgItem(hDlg, EDIT_EditAngle), std::to_wstring(info.angle).c_str());
                if (info.bLoop)
                {
                    SendMessage(GetDlgItem(hDlg, CHECK_EditLOOP), BM_SETCHECK, BST_CHECKED, 0);
                }
                else
                {
                    SendMessage(GetDlgItem(hDlg, CHECK_EditLOOP), BM_SETCHECK, BST_UNCHECKED, 0);
                }
                if (info.bDodge)
                {
                    SendMessage(GetDlgItem(hDlg, CHECK_Dodge), BM_SETCHECK, BST_CHECKED, 0);
                }
                else
                {
                    SendMessage(GetDlgItem(hDlg, CHECK_Dodge), BM_SETCHECK, BST_UNCHECKED, 0);
                }

                // 읽은 파일 정보를 토대로 애니메이션 프리뷰
                CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
                CLevel_Edit* pEditLv = dynamic_cast<CLevel_Edit*>(pLevel);
                if (pEditLv)
                {
                    CAlbumPlayer* pAlbPlayer = CAlbumPlayer::CreatePlayerFromFile(L"PreviewAnimation", filepath);
                    pEditLv->SetPreviewPlayer(pAlbPlayer);
                }
            }
        }
        else if (LOWORD(_wParam) == IDCANCEL)
        {
            CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
            CLevel_Edit* pEditLv = dynamic_cast<CLevel_Edit*>(pLevel);
            if (pEditLv)
            {
                pEditLv->SetPreviewPlayer(nullptr);
            }
            EndDialog(hDlg, LOWORD(_wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(_wParam) == IDOK)
        {

            // 수정된 정보를 가져옴
            WCHAR wFPS[255] = {};
            WCHAR wOffsetX[255] = {};
            WCHAR wOffsetY[255] = {};
            WCHAR filepath[255] = {};
            bool bCheck = false;
            WCHAR wAngle[255] = {};
            bool bDodge = false;
            GetWindowText(GetDlgItem(hDlg, EDIT_EditFPS), wFPS, 255);
            GetWindowText(GetDlgItem(hDlg, EDIT_EditOffsetX), wOffsetX, 255);
            GetWindowText(GetDlgItem(hDlg, EDIT_EditOffsetY), wOffsetY, 255);
            GetWindowText(GetDlgItem(hDlg, STATIC_TargetFile), filepath, 255);
            GetWindowText(GetDlgItem(hDlg, EDIT_EditAngle), wAngle, 255);
            if (SendMessage(GetDlgItem(hDlg, CHECK_EditLOOP), BM_GETCHECK, 0, 0) == BST_CHECKED)
            {
                bCheck = true;
            }
            else
            {
                bCheck = false;
            }
            if (SendMessage(GetDlgItem(hDlg, CHECK_Dodge), BM_GETCHECK, 0, 0) == BST_CHECKED)
            {
                bDodge = true;
            }
            else
            {
                bDodge = false;
            }

            // 입력값이 없는경우 중단
            if (wstring(filepath) == L"Static")
                break;

            int FPS = std::stoi(wFPS);
            Vec2D Offset(std::stoi(wOffsetX), std::stoi(wOffsetY));
            float Angle = std::stof(wAngle);

            // 수정된 정보를 기반으로 파일 데이터 편집
            fstream infofile;
            infofile.open(filepath, std::ios::in | std::ios::out | std::ios::binary);
            infofile.seekp(16);
            infofile.write((char*)&bCheck, sizeof(bCheck));
            infofile.write((char*)&FPS, sizeof(FPS));
            infofile.write((char*)&Offset, sizeof(Offset));
            infofile.write((char*)&Angle, sizeof(Angle));
            infofile.write((char*)&bDodge, sizeof(bDodge));
            infofile.close();

            CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
            CLevel_Edit* pEditLv = dynamic_cast<CLevel_Edit*>(pLevel);
            if (pEditLv)
            {
                pEditLv->SetPreviewPlayer(nullptr);
            }
            EndDialog(hDlg, LOWORD(_wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(_wParam) == BTN_Preview)
        {
            // 현재 입력된 정보를 가져옴
            WCHAR wIndexBegin[255] = {};
            WCHAR wIndexEnd[255] = {};
            WCHAR wFPS[255] = {};
            WCHAR wOffsetX[255] = {};
            WCHAR wOffsetY[255] = {};
            bool bCheck = false;
            WCHAR wAngle[255] = {};
            bool bDodge = false;
            GetWindowText(GetDlgItem(hDlg, STATIC_IndexBegin), wIndexBegin, 255);
            GetWindowText(GetDlgItem(hDlg, STATIC_IndexEnd), wIndexEnd, 255);
            GetWindowText(GetDlgItem(hDlg, EDIT_EditFPS), wFPS, 255);
            GetWindowText(GetDlgItem(hDlg, EDIT_EditOffsetX), wOffsetX, 255);
            GetWindowText(GetDlgItem(hDlg, EDIT_EditOffsetY), wOffsetY, 255);
            GetWindowText(GetDlgItem(hDlg, EDIT_EditAngle), wAngle, 255);
            if (SendMessage(GetDlgItem(hDlg, CHECK_EditLOOP), BM_GETCHECK, 0, 0) == BST_CHECKED)
            {
                bCheck = true;
            }
            else
            {
                bCheck = false;
            }
            if (SendMessage(GetDlgItem(hDlg, CHECK_Dodge), BM_GETCHECK, 0, 0) == BST_CHECKED)
            {
                bDodge = true;
            }
            else
            {
                bDodge = false;
            }

            // 입력값이 없는경우 중단
            if (wstring(wIndexBegin) == L"Static")
                break;

            // 현재 레벨이 에디터 레벨인지 확인 후 프리뷰 변경
            CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
            CLevel_Edit* pEditLv = dynamic_cast<CLevel_Edit*>(pLevel);
            if (pEditLv)
            {
                pEditLv->GetPreviewPlayer()->SetPlayInfo(std::stoi(wIndexBegin), std::stoi(wIndexEnd), bCheck
                    , std::stoi(wFPS), Vec2D(std::stof(wOffsetX), std::stof(wOffsetY)), std::stof(wAngle), bDodge);
            }
        }
        break;
    }
    }
    return (INT_PTR)FALSE;
}

INT_PTR AddStageProc(HWND hDlg, UINT message, WPARAM _wParam, LPARAM _lParam)
{
    switch (message)
    {
    case(WM_COMMAND):
    {
        if (LOWORD(_wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(_wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(_wParam) == BTN_AddBGM)
        {
            WCHAR filepath[255] = {};
            WCHAR filename[255] = {};
            wstring Initpath = CEngine::GetInst()->GetResourcePathW();

            OPENFILENAME desc = {};
            desc.lStructSize = sizeof(OPENFILENAME);
            desc.hwndOwner = hDlg;
            desc.lpstrFilter = L"ogg\0*.ogg\0wav\0*.wav\0ALL\0*.*\0";
            desc.lpstrFile = filepath;
            desc.nMaxFile = 255;
            desc.lpstrFileTitle = filename;
            desc.nMaxFileTitle = 255;
            desc.lpstrTitle = L"BGM 추가";
            desc.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
            desc.lpstrInitialDir = Initpath.c_str();

            if (GetOpenFileName(&desc))
            {
                wstring wstr(filepath);
                wstr = wstr.substr(wstr.rfind('.'));
                if (wstr == L".wav" || wstr == L".ogg")
                {
                    HWND hBGMName = GetDlgItem(hDlg, STATIC_BGMPath);
                    SetWindowText(hBGMName, filename);
                }
                else
                {
                    // 파일 형식이 wav, ogg 가 아닐 경우 경고창 출력
                    MessageBox(hDlg, L"지원되지 않는 형식입니다.", L"파일 형식 오류", MB_ICONWARNING | MB_OK);
                }
            }
        }
        else if (LOWORD(_wParam) == BTN_AddBGA)
        {
            WCHAR filepath[255] = {};
            WCHAR filename[255] = {};
            wstring Initpath = CEngine::GetInst()->GetResourcePathW();

            OPENFILENAME desc = {};
            desc.lStructSize = sizeof(OPENFILENAME);
            desc.hwndOwner = hDlg;
            desc.lpstrFilter = L"animation\0*.animation\0ALL\0*.*\0";
            desc.lpstrFile = filepath;
            desc.nMaxFile = 255;
            desc.lpstrFileTitle = filename;
            desc.nMaxFileTitle = 255;
            desc.lpstrTitle = L"BGA 추가";
            desc.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
            desc.lpstrInitialDir = Initpath.c_str();

            if (GetOpenFileName(&desc))
            {
                wstring wstr(filepath);
                wstr = wstr.substr(wstr.rfind('.'));
                if (wstr == L".animation")
                {
                    HWND hBGAList = GetDlgItem(hDlg, LBX_BGAList);
                    SendMessage(hBGAList, LB_ADDFILE, 0, (LPARAM)filepath);
                }
                else
                {
                    // 파일 형식이 animation 가 아닐 경우 경고창 출력
                    MessageBox(hDlg, L"애니메이션 파일이 아닙니다.", L"파일 형식 오류", MB_ICONWARNING | MB_OK);
                }
            }
        }
        else if (LOWORD(_wParam) == IDOK)
        {
            WCHAR wStageName[255] = {};
            WCHAR wHorizPixel[255] = {};
            WCHAR wVertPixel[255] = {};
            WCHAR wBGMPath[255] = {};
            HWND hStageName = GetDlgItem(hDlg, EDIT_StageName);
            HWND hHorizPixel = GetDlgItem(hDlg, EDIT_HorizPixel);
            HWND hVertPixel = GetDlgItem(hDlg, EDIT_VertPixel);
            HWND hBGMPath = GetDlgItem(hDlg, STATIC_BGMPath);
            HWND hBGAList = GetDlgItem(hDlg, LBX_BGAList);
            GetWindowText(hStageName, wStageName, 255);
            GetWindowText(hHorizPixel, wHorizPixel, 255);
            GetWindowText(hVertPixel, wVertPixel, 255);
            GetWindowText(hBGMPath, wBGMPath, 255);

            CDungeonMaker* pDungeonMakerLv = dynamic_cast<CDungeonMaker*>(CLevelMgr::GetInst()->GetCurrentLevel());
            assert(pDungeonMakerLv);

            // 입력값 예외처리
            if (wstring(wStageName).empty() || wstring(wHorizPixel).empty() || wstring(wVertPixel).empty() || !SendMessage(hBGAList, LB_GETCOUNT, 0, 0))
            {
                // 필수 입력값이 전부 입력되지 않았음
                MessageBox(hDlg, L"입력값이 필요합니다.", L"입력 오류", MB_ICONWARNING | MB_OK);
                break;
            }

            // 스테이지 정보 작성
            StageInfo* pStage = new StageInfo;
            pStage->StageName = wStageName;
            pStage->StageSize = Vec2D(std::stoi(wHorizPixel), std::stoi(wVertPixel));
            pStage->BGMPath = wBGMPath;
            for (int i = 0; i < SendMessage(hBGAList, LB_GETCOUNT, 0, 0); ++i)
            {
                WCHAR buffer[255] = {};
                SendMessage(hBGAList, LB_GETTEXT, i, (LPARAM)buffer);
                pStage->vecBGA.push_back(buffer);
            }


            // 던전메이커 레벨의 스테이지 map에 스테이지 추가
            pDungeonMakerLv->AddStageInfo(pDungeonMakerLv->GetSelectedTile(), pStage);

            EndDialog(hDlg, LOWORD(_wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    }
    return (INT_PTR)FALSE;
}

INT_PTR StageMakerPanelProc(HWND hDlg, UINT message, WPARAM _wParam, LPARAM _lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
    {
        return (INT_PTR)TRUE;
    }

    case WM_COMMAND:
        if (LOWORD(_wParam) == BTN_AddMonster)
        {
            DialogBox(CEngine::GetInst()->GetProgramInst(), MAKEINTRESOURCE(DLG_AddMonster), hDlg, &AddMonsterProc);
        }
        else if (LOWORD(_wParam) == BTN_AddNPC)
        {
            DialogBox(CEngine::GetInst()->GetProgramInst(), MAKEINTRESOURCE(DLG_AddNPC), hDlg, &AddNPCProc);
        }
        else if (LOWORD(_wParam) == BTN_AddWall)
        {
            DialogBox(CEngine::GetInst()->GetProgramInst(), MAKEINTRESOURCE(DLG_Wall), hDlg, &AddWallProc);
        }
        else if (LOWORD(_wParam) == BTN_EditSelObj)
        {
            DialogBox(CEngine::GetInst()->GetProgramInst(), MAKEINTRESOURCE(DLG_EditObj), hDlg, &EditObjProc);
        }
        else if (LOWORD(_wParam) == BTN_DelSelObj)
        {
            int ID = CLevelMgr::GetInst()->GetCurrentLevel()->GetSelectedObj()->GetID();
            CStageMaker* pStageMaker = dynamic_cast<CStageMaker*>(CLevelMgr::GetInst()->GetCurrentLevel());
            if (pStageMaker)
                pStageMaker->EraseObjFromMap(ID);
            CLevelMgr::GetInst()->GetCurrentLevel()->DeleteSelectedObj();
        }
        else if (LOWORD(_wParam) == BTN_EditStageSize)
        {
            DialogBox(CEngine::GetInst()->GetProgramInst(), MAKEINTRESOURCE(DLG_EditStageSize), hDlg, &EditStageSizeProc);
        }
    }
    return (INT_PTR)FALSE;
}

INT_PTR AddWallProc(HWND hDlg, UINT message, WPARAM _wParam, LPARAM _lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(_wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(_wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(_wParam) == IDOK)
        {
            // 입력값 가져오기
            HWND hName  = GetDlgItem(hDlg, EDIT_Name);
            HWND hPosX  = GetDlgItem(hDlg, EDIT_PosX);
            HWND hPosY  = GetDlgItem(hDlg, EDIT_PosY);
            HWND hSizeX = GetDlgItem(hDlg, EDIT_SizeX);
            HWND hSizeY = GetDlgItem(hDlg, EDIT_SizeY);
            WCHAR wName [255] = {};
            WCHAR wPosX [255] = {};
            WCHAR wPosY [255] = {};
            WCHAR wSizeX[255] = {};
            WCHAR wSizeY[255] = {};
            GetWindowText(hName , wName , 255);
            GetWindowText(hPosX , wPosX , 255);
            GetWindowText(hPosY , wPosY , 255);
            GetWindowText(hSizeX, wSizeX, 255);
            GetWindowText(hSizeY, wSizeY, 255);

            // 입력값 예외처리
            if (wstring(wName).empty() || wstring(wPosX).empty() || wstring(wPosY).empty() || wstring(wSizeX).empty() || wstring(wSizeY).empty())
            {
                // 필수 입력값이 전부 입력되지 않았음
                MessageBox(hDlg, L"입력값이 필요합니다.", L"입력 오류", MB_ICONWARNING | MB_OK);
                break;
            }

            WCHAR* stopstr;
            CWall* pWall = new CWall(wName);
            pWall->SetLocation(Vec2D(wcstof(wPosX, &stopstr), wcstof(wPosY, &stopstr)));
            pWall->SetScale(Vec2D(wcstof(wSizeX, &stopstr), wcstof(wSizeY, &stopstr)));
            CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pWall, LayerType::Object);
            pWall->AddComponent(new CSticker(wName));

            CStageMaker* pStageMaker = dynamic_cast<CStageMaker*>(CLevelMgr::GetInst()->GetCurrentLevel());
            pStageMaker->GetWallMap().insert(make_pair(pWall->GetID(), pWall));

            EndDialog(hDlg, LOWORD(_wParam));
            return (INT_PTR)TRUE;
        }
    }
    return (INT_PTR)FALSE;
}

INT_PTR AddMonsterProc(HWND hDlg, UINT message, WPARAM _wParam, LPARAM _lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(_wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(_wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(_wParam) == IDOK)
        {
            // 입력값 가져오기
            HWND hName = GetDlgItem(hDlg, EDIT_Name);
            HWND hPosX = GetDlgItem(hDlg, EDIT_PosX);
            HWND hPosY = GetDlgItem(hDlg, EDIT_PosY);
            WCHAR wName[255] = {};
            WCHAR wPosX[255] = {};
            WCHAR wPosY[255] = {};
            GetWindowText(hName, wName, 255);
            GetWindowText(hPosX, wPosX, 255);
            GetWindowText(hPosY, wPosY, 255);

            // 입력값 예외처리
            if (wstring(wName).empty() || wstring(wPosX).empty() || wstring(wPosY).empty())
            {
                // 필수 입력값이 전부 입력되지 않았음
                MessageBox(hDlg, L"입력값이 필요합니다.", L"입력 오류", MB_ICONWARNING | MB_OK);
                break;
            }

            WCHAR* stopstr;
            CMonster* pMonster = new CMonster(wName);
            CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pMonster, LayerType::Object);
            pMonster->AddComponent(new CSticker(L"dominatedunnaturals_Stk"));
            pMonster->SetLocation(Vec2D(wcstof(wPosX, &stopstr), wcstof(wPosY, &stopstr)));
            pMonster->SetScale(Vec2D(100, 140));
            pMonster->SetMonsterTemplate(MonsterTemplate::dominatedunnaturals);
            pMonster->AddAnimation(MonsterState::Idle, CAlbumPlayer::CreatePlayerFromFile(L"dominatedunnaturals_Idle"
                , CEngine::GetInst()->GetResourcePathW() + L"\\animation\\monster_dominatedunnaturals_Idle.animation"));
            pMonster->AddAnimation(MonsterState::Move, CAlbumPlayer::CreatePlayerFromFile(L"dominatedunnaturals_Move"
                , CEngine::GetInst()->GetResourcePathW() + L"\\animation\\monster_dominatedunnaturals_Move.animation"));
            pMonster->AddAnimation(MonsterState::Attack, CAlbumPlayer::CreatePlayerFromFile(L"dominatedunnaturals_Attack0"
                , CEngine::GetInst()->GetResourcePathW() + L"\\animation\\monster_dominatedunnaturals_Attack0.animation"));
            pMonster->AddAnimation(MonsterState::Attack, CAlbumPlayer::CreatePlayerFromFile(L"dominatedunnaturals_Attack1"
                , CEngine::GetInst()->GetResourcePathW() + L"\\animation\\monster_dominatedunnaturals_Attack1.animation"));
            pMonster->AddAnimation(MonsterState::Hurt, CAlbumPlayer::CreatePlayerFromFile(L"dominatedunnaturals_Hurt"
                , CEngine::GetInst()->GetResourcePathW() + L"\\animation\\monster_dominatedunnaturals_Hurt.animation"));
            pMonster->SetDetectRange(250);
            pMonster->SetAttackRange(100);
            pMonster->SetAttackFrame(make_pair(2, 3));

            CStageMaker* pStageMaker = dynamic_cast<CStageMaker*>(CLevelMgr::GetInst()->GetCurrentLevel());
            pStageMaker->GetMonsterMap().insert(make_pair(pMonster->GetID(), pMonster));

            EndDialog(hDlg, LOWORD(_wParam));
            return (INT_PTR)TRUE;
        }
    }
    return (INT_PTR)FALSE;
}

INT_PTR AddNPCProc(HWND hDlg, UINT message, WPARAM _wParam, LPARAM _lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(_wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(_wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(_wParam) == IDOK)
        {
            // 입력값 가져오기
            HWND hName = GetDlgItem(hDlg, EDIT_Name);
            HWND hPosX = GetDlgItem(hDlg, EDIT_PosX);
            HWND hPosY = GetDlgItem(hDlg, EDIT_PosY);
            HWND hNPCSizeX = GetDlgItem(hDlg, EDIT_NPCSizeX);
            HWND hNPCSizeY = GetDlgItem(hDlg, EDIT_NPCSizeY);
            HWND hIdleAni = GetDlgItem(hDlg, STATIC_IdleAni);
            WCHAR wName[255] = {};
            WCHAR wPosX[255] = {};
            WCHAR wPosY[255] = {};
            WCHAR wNPCSizeX[255] = {};
            WCHAR wNPCSizeY[255] = {};
            WCHAR wIdleAni[255] = {};
            GetWindowText(hName, wName, 255);
            GetWindowText(hPosX, wPosX, 255);
            GetWindowText(hPosY, wPosY, 255);
            GetWindowText(hNPCSizeX, wNPCSizeX, 255);
            GetWindowText(hNPCSizeY, wNPCSizeY, 255);
            GetWindowText(hIdleAni, wIdleAni, 255);

            // 입력값 예외처리
            if (wstring(wName).empty() || wstring(wPosX).empty() || wstring(wPosY).empty() 
                || wstring(wNPCSizeX).empty() || wstring(wNPCSizeY).empty() || wstring(wIdleAni).empty())
            {
                // 필수 입력값이 전부 입력되지 않았음
                MessageBox(hDlg, L"입력값이 필요합니다.", L"입력 오류", MB_ICONWARNING | MB_OK);
                break;
            }

            WCHAR* stopstr;
            CNPC* pNPC = new CNPC(wName);
            pNPC->SetLocation(Vec2D(wcstof(wPosX, &stopstr), wcstof(wPosY, &stopstr)));
            pNPC->SetScale(Vec2D(wcstof(wNPCSizeX, &stopstr), wcstof(wNPCSizeY, &stopstr)));
            CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pNPC, LayerType::Object);
            pNPC->AddComponent(new CSticker(wName));
            pNPC->AddComponent(CAlbumPlayer::CreatePlayerFromFile(wstring(wName) + L"_IdleAni"
                , CEngine::GetInst()->GetResourcePathW() + L"\\animation\\" + wIdleAni));
            pNPC->SetIdleAni(wIdleAni);

            CStageMaker* pStageMaker = dynamic_cast<CStageMaker*>(CLevelMgr::GetInst()->GetCurrentLevel());
            pStageMaker->GetNPCMap().insert(make_pair(pNPC->GetID(), pNPC));

            EndDialog(hDlg, LOWORD(_wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(_wParam) == BTN_SetIdle)
        {

            // 파일 탐색기 초기화
            WCHAR filepath[255] = {};
            WCHAR filename[255] = {};
            wstring initpath = CEngine::GetInst()->GetResourcePathW() + L"\\animation";
            OPENFILENAME desc = {};
            desc.lStructSize = sizeof(OPENFILENAME);
            desc.hwndOwner = hDlg;
            desc.lpstrFilter = L"animation\0*.animation\0ALL\0*.*\0";
            desc.lpstrFile = filepath;
            desc.nMaxFile = 255;
            desc.lpstrFileTitle = filename;
            desc.nMaxFileTitle = 255;
            desc.lpstrTitle = L"애니메이션 파일 불러오기";
            desc.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
            desc.lpstrInitialDir = initpath.c_str();

            if (GetOpenFileName(&desc))
            {
                HWND hIdleAni = GetDlgItem(hDlg, STATIC_IdleAni);
                SetWindowText(hIdleAni, filename);
            }
        }
    }
    return (INT_PTR)FALSE;
}

INT_PTR EditObjProc(HWND hDlg, UINT message, WPARAM _wParam, LPARAM _lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
    {
        // 개체값 세팅
        CObj* pObj = CLevelMgr::GetInst()->GetCurrentLevel()->GetSelectedObj();
        if (pObj)
        {
            HWND hName = GetDlgItem(hDlg, STATIC_EditObjName);
            HWND hPosX = GetDlgItem(hDlg, EDIT_PosX);
            HWND hPosY = GetDlgItem(hDlg, EDIT_PosY);
            HWND hSizeX = GetDlgItem(hDlg, EDIT_SizeX);
            HWND hSizeY = GetDlgItem(hDlg, EDIT_SizeY);
            SetWindowText(hName, pObj->GetName().c_str());
            SetWindowText(hPosX, std::to_wstring(pObj->GetLocation().x).c_str());
            SetWindowText(hPosY, std::to_wstring(pObj->GetLocation().y).c_str());
            SetWindowText(hSizeX, std::to_wstring(pObj->GetScale().x).c_str());
            SetWindowText(hSizeY, std::to_wstring(pObj->GetScale().y).c_str());
        }
        else
        {
            EndDialog(hDlg, LOWORD(_wParam));
            return (INT_PTR)TRUE;
        }
    }
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(_wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(_wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(_wParam) == IDOK)
        {
            // 입력값 가져오기
            HWND hName = GetDlgItem(hDlg, STATIC_EditObjName);
            HWND hPosX = GetDlgItem(hDlg, EDIT_PosX);
            HWND hPosY = GetDlgItem(hDlg, EDIT_PosY);
            HWND hSizeX = GetDlgItem(hDlg, EDIT_SizeX);
            HWND hSizeY = GetDlgItem(hDlg, EDIT_SizeY);
            WCHAR wName[255] = {};
            WCHAR wPosX[255] = {};
            WCHAR wPosY[255] = {};
            WCHAR wSizeX[255] = {};
            WCHAR wSizeY[255] = {};
            GetWindowText(hName, wName, 255);
            GetWindowText(hPosX, wPosX, 255);
            GetWindowText(hPosY, wPosY, 255);
            GetWindowText(hSizeX, wSizeX, 255);
            GetWindowText(hSizeY, wSizeY, 255);

            // 입력값 예외처리
            if (wstring(wName).empty() || wstring(wPosX).empty() || wstring(wPosY).empty() || wstring(wSizeX).empty() || wstring(wSizeY).empty())
            {
                // 필수 입력값이 전부 입력되지 않았음
                MessageBox(hDlg, L"입력값이 필요합니다.", L"입력 오류", MB_ICONWARNING | MB_OK);
                break;
            }

            // 개체값 세팅
            WCHAR* stopstr;
            CObj* pObj = CLevelMgr::GetInst()->GetCurrentLevel()->GetSelectedObj();
            pObj->SetLocation(Vec2D(wcstof(wPosX, &stopstr), wcstof(wPosY, &stopstr)));
            pObj->SetScale(Vec2D(wcstof(wSizeX, &stopstr), wcstof(wSizeY, &stopstr)));


            EndDialog(hDlg, LOWORD(_wParam));
            return (INT_PTR)TRUE;
        }
    }
    return (INT_PTR)FALSE;
}

INT_PTR EditStageSizeProc(HWND hDlg, UINT message, WPARAM _wParam, LPARAM _lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
    {
        HWND hWidth = GetDlgItem(hDlg, EDIT_Width);
        HWND hHeight = GetDlgItem(hDlg, EDIT_Height);
        Vec2D Size;
        CStageMaker* pStageMaker = dynamic_cast<CStageMaker*>(CLevelMgr::GetInst()->GetCurrentLevel());
        if (pStageMaker)
            Size = pStageMaker->GetStageInfo()->StageSize;
        SetWindowText(hWidth, std::to_wstring(Size.x).c_str());
        SetWindowText(hHeight, std::to_wstring(Size.y).c_str());
        return (INT_PTR)TRUE;
    }

    case WM_COMMAND:
        if (LOWORD(_wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(_wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(_wParam) == IDOK)
        {
            // 입력값 가져오기
            HWND hWidth = GetDlgItem(hDlg, EDIT_Width);
            HWND hHeight = GetDlgItem(hDlg, EDIT_Height);
            WCHAR wWidth[255] = {};
            WCHAR wHeight[255] = {};
            GetWindowText(hWidth, wWidth, 255);
            GetWindowText(hHeight, wHeight, 255);

            // 입력값 예외처리
            if (wstring(wWidth).empty() || wstring(wHeight).empty())
            {
                // 필수 입력값이 전부 입력되지 않았음
                MessageBox(hDlg, L"입력값이 필요합니다.", L"입력 오류", MB_ICONWARNING | MB_OK);
                break;
            }


            WCHAR* end = nullptr;
            CStageMaker* pStageMaker = dynamic_cast<CStageMaker*>(CLevelMgr::GetInst()->GetCurrentLevel());
            if (pStageMaker)
            {
                pStageMaker->GetStageInfo()->StageSize = Vec2D(wcstof(wWidth, &end), wcstof(wHeight, &end));
            }
            
            HWND upperbound = GetDlgItem(GetParent(hDlg), SLIDE_UpperBound);
            SendMessage(upperbound, TBM_SETRANGE, TRUE, MAKELONG(0, wcstof(wHeight, &end)));
            SendMessage(upperbound, TBM_SETPOS, TRUE, 0);


            EndDialog(hDlg, LOWORD(_wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(_wParam) == BTN_SetIdle)
        {

            // 파일 탐색기 초기화
            WCHAR filepath[255] = {};
            WCHAR filename[255] = {};
            wstring initpath = CEngine::GetInst()->GetResourcePathW() + L"\\animation";
            OPENFILENAME desc = {};
            desc.lStructSize = sizeof(OPENFILENAME);
            desc.hwndOwner = hDlg;
            desc.lpstrFilter = L"animation\0*.animation\0ALL\0*.*\0";
            desc.lpstrFile = filepath;
            desc.nMaxFile = 255;
            desc.lpstrFileTitle = filename;
            desc.nMaxFileTitle = 255;
            desc.lpstrTitle = L"애니메이션 파일 불러오기";
            desc.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
            desc.lpstrInitialDir = initpath.c_str();

            if (GetOpenFileName(&desc))
            {
                HWND hIdleAni = GetDlgItem(hDlg, STATIC_IdleAni);
                SetWindowText(hIdleAni, filename);
            }
        }
    }
    return (INT_PTR)FALSE;
}
