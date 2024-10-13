#include "pch.h"
#include "CButton_LoadNPK.h"
#include "CEngine.h"
#include "resource.h"
#include "CAlbum.h"
#include "CTextureMgr.h"

LRESULT CALLBACK AlbumViewerProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

CButton_LoadNPK::CButton_LoadNPK(wstring _name)
	: CButton(_name)
    , m_hAlbumViewerWnd(nullptr)
{
}

CButton_LoadNPK::~CButton_LoadNPK()
{
}

void CButton_LoadNPK::Click()
{
    if (m_hAlbumViewerWnd == nullptr)
	    m_hAlbumViewerWnd = CreateDialog(CEngine::GetInst()->GetProgramInst(), MAKEINTRESOURCE(DLG_AlbumViewer), CEngine::GetInst()->GetMainWnd(), &AlbumViewerProc);
    ShowWindow(m_hAlbumViewerWnd, SW_SHOW);
}


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
                wstring npkdir = filepath;
                vector<CAlbum*> AlbumVec = CTextureMgr::GetInst()->LoadNPK(npkdir);
                for (CAlbum* pAlbum : AlbumVec)
                {
                    string lpar = pAlbum->GetPath();
                    lpar = lpar.substr(lpar.rfind('/') + 1);
                    wstring wstr(lpar.begin(), lpar.end());
                    HWND hAlbumLBX = GetDlgItem(hDlg, LBX_AlbumList);
                    SendMessage(hAlbumLBX, LB_ADDSTRING, 0, (LPARAM)wstr.c_str());
                }
            }

            
        }
        else if (LOWORD(_wParam) == BTN_LoadImg)
        {

        }
        break;
    }
    return (INT_PTR)FALSE;
}