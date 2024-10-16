#include "pch.h"
#include "CButton_CreateAlbum.h"
#include "CEngine.h"
#include "resource.h"
#include "CTextureMgr.h"
#include "CTexture.h"
#include "CLevelMgr.h"
#include "CLevel_Edit.h"

INT_PTR CALLBACK CreateAlbumProc(HWND hDlg, UINT message, WPARAM _wParam, LPARAM _lParam);

CButton_CreateAlbum::CButton_CreateAlbum(wstring _name)
	: CButton(_name)
	, m_hCreateAlbum(nullptr)
{
}

CButton_CreateAlbum::~CButton_CreateAlbum()
{
}

void CButton_CreateAlbum::MouseLBtnClikced()
{
    if (m_hCreateAlbum == nullptr)
    {
        m_hCreateAlbum = CreateDialog(CEngine::GetInst()->GetProgramInst(), MAKEINTRESOURCE(DLG_AlbumMaker), CEngine::GetInst()->GetMainWnd(), &CreateAlbumProc);
    }
    else
    {
        DestroyWindow(m_hCreateAlbum);
        m_hCreateAlbum = CreateDialog(CEngine::GetInst()->GetProgramInst(), MAKEINTRESOURCE(DLG_AlbumMaker), CEngine::GetInst()->GetMainWnd(), &CreateAlbumProc);
    }
	ShowWindow(m_hCreateAlbum, SW_SHOW);
}


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
            WCHAR InitDir[255] = {};
            memcpy(InitDir, CEngine::GetInst()->GetResourcePath().c_str(), CEngine::GetInst()->GetResourcePath().size() * 2);
            OPENFILENAME OpenNPK = {};
            OpenNPK.lStructSize = sizeof(OPENFILENAME);
            OpenNPK.hwndOwner = hDlg;
            OpenNPK.lpstrFilter = L"png\0*.png\0ALL\0*.*\0";
            OpenNPK.lpstrFile = filepath;
            OpenNPK.nMaxFile = 255;
            OpenNPK.lpstrFileTitle = filename;
            OpenNPK.nMaxFileTitle = 255;
            //OpenNPK.lpstrInitialDir = InitDir;
            OpenNPK.lpstrTitle = L"�̹��� ���� �ҷ�����";
            OpenNPK.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

            if (GetOpenFileName(&OpenNPK))
            {
                // ���� Ž���⿡�� �̹��� ������ ������ �� �ٹ� ����Ʈ�ڽ��� ����
                HWND hAlbumLBX = GetDlgItem(hDlg, LBX_CustomAlbum);
                wstring ImageDir = filepath;
                CTexture* pTex = CTextureMgr::GetInst()->LoadFromFile(filepath);
                int index = SendMessage(hAlbumLBX, LB_ADDSTRING, 0, (LPARAM)pTex->GetName().c_str());
            }
        }
        else if (HIWORD(_wParam) == LBN_SELCHANGE && LOWORD(_wParam) == LBX_CustomAlbum)
        {
            // �ٹ� ����Ʈ�ڽ� �׸��� Ŭ������ ���
            HWND hAlbumLBX = GetDlgItem(hDlg, LBX_CustomAlbum);
            
            int index = SendMessage(hAlbumLBX, LB_GETCURSEL, 0, 0);
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

            int index = SendMessage(hLBX, LB_GETCURSEL, 0, 0);
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
