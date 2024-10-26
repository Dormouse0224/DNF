// 프로시저 함수 선언 헤더

LRESULT CALLBACK AlbumViewerProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK CreateAniProc(HWND hDlg, UINT message, WPARAM _wParam, LPARAM _lParam);

INT_PTR CALLBACK CreateAlbumProc(HWND hDlg, UINT message, WPARAM _wParam, LPARAM _lParam);

INT_PTR CALLBACK EditAlimationProc(HWND hDlg, UINT message, WPARAM _wParam, LPARAM _lParam);

INT_PTR CALLBACK AddStageProc(HWND hDlg, UINT message, WPARAM _wParam, LPARAM _lParam);

INT_PTR CALLBACK StageMakerPanelProc(HWND hDlg, UINT message, WPARAM _wParam, LPARAM _lParam);
INT_PTR CALLBACK AddWallProc(HWND hDlg, UINT message, WPARAM _wParam, LPARAM _lParam);
INT_PTR CALLBACK AddMonsterProc(HWND hDlg, UINT message, WPARAM _wParam, LPARAM _lParam);
INT_PTR CALLBACK AddNPCProc(HWND hDlg, UINT message, WPARAM _wParam, LPARAM _lParam);