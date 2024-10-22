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



// �ٹ� ��� ���̾�α� ���ν��� �Լ�
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
            // ����� ȭ�� �ʱ�ȭ
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
            // NPK ���� �ҷ����� ��ư�� Ŭ���� ���
            // ���� Ž���� �ʱ�ȭ
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
            Desc.lpstrTitle = L"NPK ���� �ҷ�����";
            Desc.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

            if (GetOpenFileName(&Desc))
            {
                // ���� Ž���⿡�� NPK ������ ������ �� NPK ���� ���� �ٹ��� �ٹ� ����Ʈ�ڽ��� ����
                HWND hAlbumLBX = GetDlgItem(hDlg, LBX_AlbumList);
                HWND hNPKDir = GetDlgItem(hDlg, STATIC_NPKDir);
                HWND hTexLBX = GetDlgItem(hDlg, LBX_TextureList);
                SendMessage(hAlbumLBX, LB_RESETCONTENT, 0, 0);  // �ٹ� ����Ʈ�ڽ� �ʱ�ȭ
                SendMessage(hTexLBX, LB_RESETCONTENT, 0, 0);    // �ؽ�ó ����Ʈ�ڽ� �ʱ�ȭ
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
            // �ٹ� ����Ʈ�ڽ� �׸��� ����Ŭ������ ���
            HWND hAlbumLBX = GetDlgItem(hDlg, LBX_AlbumList);
            HWND hTexLBX = GetDlgItem(hDlg, LBX_TextureList);
            HWND hNPKDir = GetDlgItem(hDlg, STATIC_NPKDir);
            HWND hOwnALbum = GetDlgItem(hDlg, STATIC_OwnerAlbum);

            WCHAR npkdir[255] = {};
            GetWindowText(hNPKDir, npkdir, 255);

            SendMessage(hTexLBX, LB_RESETCONTENT, 0, 0);    // �ؽ�ó ����Ʈ�ڽ� �ʱ�ȭ

            LRESULT index = SendMessage(hAlbumLBX, LB_GETCURSEL, 0, 0);
            TCHAR buffer[255] = {};
            SendMessage(hAlbumLBX, LB_GETTEXT, index, (LPARAM)buffer);
            wstring filename = buffer;

            CAlbum* pAlbum = CTextureMgr::GetInst()->LoadAlbum(string(filename.begin(), filename.end()), npkdir);
            SetWindowText(hOwnALbum, buffer);
            for (int i = 0; i < pAlbum->GetCount(); ++i)
            {
                CTexture* pTex = pAlbum->GetScene(i);
                pTex->Load();
                SendMessage(hTexLBX, LB_ADDSTRING, 0, (LPARAM)pTex->GetName().c_str());
            }

        }
        else if (HIWORD(_wParam) == LBN_SELCHANGE && LOWORD(_wParam) == LBX_TextureList)
        {
            // �ؽ�ó ����Ʈ�ڽ� �׸��� Ŭ����
            HWND hTexLBX = GetDlgItem(hDlg, LBX_TextureList);
            HWND hOwnALbum = GetDlgItem(hDlg, STATIC_OwnerAlbum);
            HWND hNPKDir = GetDlgItem(hDlg, STATIC_NPKDir);

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

            CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
            CLevel_Edit* pEditLv = dynamic_cast<CLevel_Edit*>(pLevel);
            if (pEditLv && pTex)
            {
                pEditLv->SetPreviewTexture(pTex);
            }
        }
        else if (LOWORD(_wParam) == BTN_CreateAni)
        {
            HWND hNPKDir = GetDlgItem(hDlg, STATIC_NPKDir);
            HWND hOwnerAlbum = GetDlgItem(hDlg, STATIC_OwnerAlbum);
            HWND hSelectedNPK = GetDlgItem(hDlg, STATIC_SelectedNpk);
            HWND hSelectedAlbum = GetDlgItem(hDlg, STATIC_SelectedAlbum);

            WCHAR buffer1[255] = {};
            WCHAR buffer2[255] = {};
            GetWindowText(hNPKDir, buffer1, 255);
            GetWindowText(hOwnerAlbum, buffer2, 255);
            SendMessage(hSelectedNPK, WM_SETTEXT, 0, (LPARAM)buffer1);
            SendMessage(hSelectedAlbum, WM_SETTEXT, 0, (LPARAM)buffer2);

            DialogBox(CEngine::GetInst()->GetProgramInst(), MAKEINTRESOURCE(DLG_CreateAni), hDlg, &CreateAniProc);
        }
        break;
        case WM_DESTROY:
        {
            // ����� ȭ�� �ʱ�ȭ
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

// �ִϸ��̼� ���� ���� �ۼ� ���ν���
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
            Desc.lpstrTitle = L"�ִϸ��̼� ���� ����";
            Desc.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
            Desc.lpstrDefExt = L"animation";

            if (GetSaveFileName(&Desc))
            {
                // ���̾�α� �������� ����Ʈ ��Ʈ�� ���� NPK, Album ������ ������� �ִϸ��̼� ���� ���� �ۼ�
                char cNPKPath[255] = {};
                char cOwnerAlbum[255] = {};
                GetDlgItemTextA(hDlg, STATIC_SelectedNpk, cNPKPath, 255);
                GetDlgItemTextA(hDlg, STATIC_SelectedAlbum, cOwnerAlbum, 255);
                string NPKDir = cNPKPath;
                string OwnerAlbum = cOwnerAlbum;
                NPKDir = NPKDir.substr(CEngine::GetInst()->GetResourcePathA().size());

                // �Էµ� ������ ����ü�� ����
                AnimationInfo animationInfo;
                animationInfo.NPKDirLen = (int)NPKDir.size();
                animationInfo.AlbumPathLen = (int)OwnerAlbum.size();
                WCHAR buffer[255] = {};
                WCHAR buffer1[255] = {};
                GetDlgItemText(hDlg, EDIT_IndexBegin, buffer, 255);
                animationInfo.IndexBegin = stoi(wstring(buffer));
                GetDlgItemText(hDlg, EDIT_IndexEnd, buffer, 255);
                animationInfo.IndexEnd = stoi(wstring(buffer));
                GetDlgItemText(hDlg, EDIT_FPS, buffer, 255);
                animationInfo.FPS = stoi(wstring(buffer));
                GetDlgItemText(hDlg, EDIT_OffsetX, buffer, 255);
                GetDlgItemText(hDlg, EDIT_OffsetY, buffer1, 255);
                animationInfo.Offset = Vec2D(stof(wstring(buffer)), stof(wstring(buffer1)));
                GetDlgItemText(hDlg, EDIT_Angle, buffer, 255);
                animationInfo.angle = stof(wstring(buffer));

                HWND hLoop = GetDlgItem(hDlg, CHECK_LOOP);
                if (SendMessage(hLoop, BM_GETCHECK, 0, 0) == BST_CHECKED)
                {
                    animationInfo.bLoop = true;
                }
                else
                {
                    animationInfo.bLoop = false;
                }
                
                // ����ü�� ����� ������ ���Ϸ� ����
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

// �ٹ� �ۼ� ���ν���
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
            // ����� ȭ�� �ʱ�ȭ
            CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
            CLevel_Edit* pEditLv = dynamic_cast<CLevel_Edit*>(pLevel);
            if (pEditLv)
            {
                pEditLv->SetPreviewTexture(nullptr);
            }

            // TempAlbum �ʱ�ȭ
            CTextureMgr::GetInst()->ClearTempAlbum();

            EndDialog(hDlg, LOWORD(_wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(_wParam) == BTN_AddImg)
        {
            // �̹��� �߰� ��ư�� Ŭ���� ���

            // ���� Ž���� �ʱ�ȭ
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
            OpenNPK.lpstrTitle = L"�̹��� ���� �ҷ�����";
            OpenNPK.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

            if (GetOpenFileName(&OpenNPK))
            {
                // ���� Ž���⿡�� �̹��� ������ ������ �� �ٹ� ����Ʈ�ڽ��� ����
                HWND hAlbumLBX = GetDlgItem(hDlg, LBX_CustomAlbum);
                wstring ImageDir = filepath;
                CTexture* pTex = CTextureMgr::GetInst()->LoadFromFile(filepath);
                LRESULT index = SendMessage(hAlbumLBX, LB_ADDSTRING, 0, (LPARAM)pTex->GetName().c_str());
            }
        }
        else if (HIWORD(_wParam) == LBN_SELCHANGE && LOWORD(_wParam) == LBX_CustomAlbum)
        {
            // �ٹ� ����Ʈ�ڽ� �׸��� Ŭ������ ���
            HWND hAlbumLBX = GetDlgItem(hDlg, LBX_CustomAlbum);

            LRESULT index = SendMessage(hAlbumLBX, LB_GETCURSEL, 0, 0);
            WCHAR buffer[255] = {};
            SendMessage(hAlbumLBX, LB_GETTEXT, index, (LPARAM)buffer);
            CTexture* pTex = CTextureMgr::GetInst()->GetTempAlbum()->GetScene(buffer);

            // ���� ������ ������ �������� Ȯ�� �� �´ٸ� �̸����� ����
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
            // NPK�� �����ϱ� ��ư�� ���� ���

            // ���� Ž���� �ʱ�ȭ
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
            Desc.lpstrTitle = L"�ٹ��� NPK�� ����";
            Desc.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
            Desc.lpstrDefExt = L"NPK";

            if (GetSaveFileName(&Desc))
            {
                HWND hAlbumLBX = GetDlgItem(hDlg, LBX_CustomAlbum);

                // ������ο� ������ �̸����� NPK ���Ϸ� �ٹ� ����
                wstring Filename = filename;
                Filename = Filename.substr(0, Filename.find(L".NPK"));
                wstring Filepath = filepath;
                CTextureMgr::GetInst()->SaveAlbum(string().assign(Filename.begin(), Filename.end()), string().assign(Filepath.begin(), Filepath.end()));

                // �ٹ� ����Ʈ�ڽ� �ʱ�ȭ
                SendMessage(hAlbumLBX, LB_RESETCONTENT, 0, 0);

                // TempAlbum �ʱ�ȭ
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
        // ����� ȭ�� �ʱ�ȭ
        CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
        CLevel_Edit* pEditLv = dynamic_cast<CLevel_Edit*>(pLevel);
        if (pEditLv)
        {
            pEditLv->SetPreviewTexture(nullptr);
        }

        // TempAlbum �ʱ�ȭ
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
            // ���� Ž���� �ʱ�ȭ
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
            desc.lpstrTitle = L"�ִϸ��̼� ���� �ҷ�����";
            desc.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
            desc.lpstrInitialDir = initpath.c_str();

            if (GetOpenFileName(&desc))
            {
                // ���Ϸκ��� ������ �о ǥ��
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

                // ���� ���� ������ ���� �ִϸ��̼� ������
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

            // ������ ������ ������
            WCHAR wFPS[255] = {};
            WCHAR wOffsetX[255] = {};
            WCHAR wOffsetY[255] = {};
            WCHAR filepath[255] = {};
            bool bCheck = false;
            WCHAR wAngle[255] = {};
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

            // �Է°��� ���°�� �ߴ�
            if (wstring(filepath) == L"Static")
                break;

            int FPS = std::stoi(wFPS);
            Vec2D Offset(std::stoi(wOffsetX), std::stoi(wOffsetY));
            float Angle = std::stof(wAngle);

            // ������ ������ ������� ���� ������ ����
            fstream infofile;
            infofile.open(filepath, std::ios::in | std::ios::out | std::ios::binary);
            infofile.seekp(16);
            infofile.write((char*)&bCheck, sizeof(bCheck));
            infofile.write((char*)&FPS, sizeof(FPS));
            infofile.write((char*)&Offset, sizeof(Offset));
            infofile.write((char*)&Angle, sizeof(Angle));
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
            // ���� �Էµ� ������ ������
            WCHAR wIndexBegin[255] = {};
            WCHAR wIndexEnd[255] = {};
            WCHAR wFPS[255] = {};
            WCHAR wOffsetX[255] = {};
            WCHAR wOffsetY[255] = {};
            bool bCheck = false;
            WCHAR wAngle[255] = {};
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

            // �Է°��� ���°�� �ߴ�
            if (wstring(wIndexBegin) == L"Static")
                break;

            // ���� ������ ������ �������� Ȯ�� �� ������ ����
            CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
            CLevel_Edit* pEditLv = dynamic_cast<CLevel_Edit*>(pLevel);
            if (pEditLv)
            {
                pEditLv->GetPreviewPlayer()->SetPlayInfo(std::stoi(wIndexBegin), std::stoi(wIndexEnd), bCheck
                    , std::stoi(wFPS), Vec2D(std::stof(wOffsetX), std::stof(wOffsetY)), std::stof(wAngle));
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
            desc.lpstrTitle = L"BGM �߰�";
            desc.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
            desc.lpstrInitialDir = Initpath.c_str();

            if (GetOpenFileName(&desc))
            {
                wstring wstr(filepath);
                wstr = wstr.substr(wstr.rfind('.'));
                if (wstr == L".wav" || wstr == L".ogg")
                {
                    HWND hBGMPath = GetDlgItem(hDlg, STATIC_BGMPath);
                    SetWindowText(hBGMPath, filepath);
                }
                else
                {
                    // ���� ������ wav, ogg �� �ƴ� ��� ���â ���
                    MessageBox(hDlg, L"�������� �ʴ� �����Դϴ�.", L"���� ���� ����", MB_ICONWARNING | MB_OK);
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
            desc.lpstrTitle = L"BGA �߰�";
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
                    // ���� ������ animation �� �ƴ� ��� ���â ���
                    MessageBox(hDlg, L"�ִϸ��̼� ������ �ƴմϴ�.", L"���� ���� ����", MB_ICONWARNING | MB_OK);
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

            // �Է°� ����ó��
            if (wstring(wStageName).empty() || wstring(wHorizPixel).empty() || wstring(wVertPixel).empty() || !SendMessage(hBGAList, LB_GETCOUNT, 0, 0))
            {
                // �ʼ� �Է°��� ���� �Էµ��� �ʾ���
                MessageBox(hDlg, L"�Է°��� �ʿ��մϴ�.", L"�Է� ����", MB_ICONWARNING | MB_OK);
                break;
            }

            // �������� ���� �ۼ�
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


            // ��������Ŀ ������ �������� map�� �������� �߰�
            pDungeonMakerLv->AddStageInfo(pDungeonMakerLv->GetSelectedTile(), pStage);

            EndDialog(hDlg, LOWORD(_wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    }
    return (INT_PTR)FALSE;
}
