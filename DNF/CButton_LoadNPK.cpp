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

vector<WPARAM> wpar;
INT_PTR CALLBACK AlbumViewerProc(HWND hDlg, UINT message, WPARAM _wParam, LPARAM _lParam)
{
    UNREFERENCED_PARAMETER(_lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        wpar.push_back(_wParam);
        if (LOWORD(_wParam) == IDOK || LOWORD(_wParam) == IDCANCEL)
        {
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
            memcpy(InitDir, CEngine::GetInst()->GetResourcePath().c_str(), CEngine::GetInst()->GetResourcePath().size() * 2);
            OPENFILENAME OpenNPK = {};
            OpenNPK.lStructSize = sizeof(OPENFILENAME);
            OpenNPK.hwndOwner = hDlg;
            OpenNPK.lpstrFilter = L"NPK\0*.NPK\0ALL\0*.*\0";
            OpenNPK.lpstrFile = filepath;
            OpenNPK.nMaxFile = 255;
            OpenNPK.lpstrFileTitle = filename;
            OpenNPK.nMaxFileTitle = 255;
            OpenNPK.lpstrInitialDir = InitDir;
            OpenNPK.lpstrTitle = L"NPK 파일 불러오기";
            OpenNPK.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

            if (GetOpenFileName(&OpenNPK))
            {
                // 파일 탐색기에서 NPK 파일을 가져온 뒤 NPK 파일 안의 앨범을 앨범 리스트박스에 넣음
                HWND hAlbumLBX = GetDlgItem(hDlg, LBX_AlbumList);
                HWND hNPKDir = GetDlgItem(hDlg, STATIC_NPKDir);
                SendMessage(hAlbumLBX, LB_RESETCONTENT, 0, 0);  // 앨범 리스트박스 초기화
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
            //if (LOWORD(_wParam) == LBX_AlbumList)
            // 앨범 리스트박스 항목을 더블클릭했을 경우
            HWND hAlbumLBX = GetDlgItem(hDlg, LBX_AlbumList);
            HWND hTexLBX = GetDlgItem(hDlg, LBX_TextureList);
            HWND hNPKDir = GetDlgItem(hDlg, STATIC_NPKDir);
            HWND hOwnALbum = GetDlgItem(hDlg, STATIC_OwnerAlbum);

            WCHAR npkdir[255] = {};
            GetWindowText(hNPKDir, npkdir, 255);

            SendMessage(hTexLBX, LB_RESETCONTENT, 0, 0);    // 텍스처 리스트박스 초기화

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
            // 텍스처 리스트박스 항목이 더블클릭됨
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
        break;
    }
    return (INT_PTR)FALSE;
}