#include "pch.h"
#include "CButton_LoadNPK.h"
#include "CEngine.h"
#include "resource.h"
#include "CAlbum.h"
#include "CTextureMgr.h"
#include "CTexture.h"
#include "CLevelMgr.h"
#include "CLevel_Edit.h"
#include "CAlbumPlayer.h"

LRESULT CALLBACK AlbumViewerProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK CreateAniProc(HWND hDlg, UINT message, WPARAM _wParam, LPARAM _lParam);

CButton_LoadNPK::CButton_LoadNPK(wstring _name)
	: CButton(_name)
    , m_hAlbumViewerWnd(nullptr)
{
}

CButton_LoadNPK::~CButton_LoadNPK()
{
}

void CButton_LoadNPK::MouseLBtnClikced()
{
    if (m_hAlbumViewerWnd == nullptr)
	    m_hAlbumViewerWnd = CreateDialog(CEngine::GetInst()->GetProgramInst(), MAKEINTRESOURCE(DLG_AlbumViewer), CEngine::GetInst()->GetMainWnd(), &AlbumViewerProc);
    ShowWindow(m_hAlbumViewerWnd, SW_SHOW);
}

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
            memcpy(InitDir, CEngine::GetInst()->GetResourcePath().c_str(), CEngine::GetInst()->GetResourcePath().size() * 2);
           
            OPENFILENAME Desc = {};
            Desc.lStructSize = sizeof(OPENFILENAME);
            Desc.hwndOwner = hDlg;
            Desc.lpstrFilter = L"NPK\0*.NPK\0ALL\0*.*\0";
            Desc.lpstrFile = filepath;
            Desc.nMaxFile = 255;
            Desc.lpstrFileTitle = filename;
            Desc.nMaxFileTitle = 255;
            Desc.lpstrInitialDir = InitDir;
            Desc.lpstrTitle = L"NPK ���� �ҷ�����";
            Desc.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

            if (GetOpenFileName(&Desc))
            {
                // ���� Ž���⿡�� NPK ������ ������ �� NPK ���� ���� �ٹ��� �ٹ� ����Ʈ�ڽ��� ����
                HWND hAlbumLBX = GetDlgItem(hDlg, LBX_AlbumList);
                HWND hNPKDir = GetDlgItem(hDlg, STATIC_NPKDir);
                SendMessage(hAlbumLBX, LB_RESETCONTENT, 0, 0);  // �ٹ� ����Ʈ�ڽ� �ʱ�ȭ
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

            int index = SendMessage(hAlbumLBX, LB_GETCURSEL, 0, 0);
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

            int index = SendMessage(hTexLBX, LB_GETCURSEL, 0, 0);

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
    }
    return (INT_PTR)FALSE;
}


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
            wstring wstr = wstring(buff).substr(0, wstring(buff).rfind('\\'));
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

            if (GetSaveFileName(&Desc))
            {
                // ���̾�α� �������� ����Ʈ ��Ʈ�� ���� NPK, Album ������ ������� �ִϸ��̼� ���� ���� �ۼ�
                char cNPKPath[255] = {};
                char cOwnerAlbum[255] = {};
                GetDlgItemTextA(hDlg, STATIC_SelectedNpk, cNPKPath, 255);
                GetDlgItemTextA(hDlg, STATIC_SelectedAlbum, cOwnerAlbum, 255);
                string NPKDir = cNPKPath;
                string OwnerAlbum = cOwnerAlbum;

                AnimationInfo animationInfo;
                animationInfo.NPKDirLen = NPKDir.size();
                animationInfo.AlbumPathLen = OwnerAlbum.size();
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

                HWND hLoop = GetDlgItem(hDlg, CHECK_LOOP);
                
                ofstream writeFile;
                writeFile.open(filepath, ios::binary);

                writeFile.write((char*)&animationInfo, sizeof(animationInfo));
                writeFile.write(NPKDir.c_str(), NPKDir.size());
                writeFile.write(OwnerAlbum.c_str(), OwnerAlbum.size());

                writeFile.close();
            }
        }
    }
    return (INT_PTR)FALSE;
}