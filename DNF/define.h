#pragma once

#define SINGLETON(Type)	public:\
							static Type* GetInst()\
							{\
								static Type single;\
								return &single;\
							}\
						private:\
							Type();\
							Type(const Type& _other) = delete;\
							void operator =(const Type& _other) = delete;\
							~Type();

#define DT CTimeMgr::GetInst()->GetDeltaTime()

#define SELECT_PEN(Pen) CSelectGDI SelectPen(CEngine::GetInst()->GetSubDC(), CEngine::GetInst()->GetPen(Pen))
#define SELECT_BRUSH(Brush) CSelectGDI SelectBrush(CEngine::GetInst()->GetSubDC(), CEngine::GetInst()->GetBrush(Brush))



#define GetCurrentPath(path)	wchar_t CurrentPath[255] = {};\
								GetCurrentDirectory(255, CurrentPath);\
								wstring path = CurrentPath;

#define GDIPlusStart	GdiplusStartupInput gdiplusStartupInput;\
						ULONG_PTR gdiplusToken;\
						GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

#define GDIPlusEnd		GdiplusShutdown(gdiplusToken);

#define _TempNPK		L"__TempNPK__"
#define _TempAlbum		"__TempAlbum__"
#define _SysNPK			L"__SysReservedNPK__"
#define _SysAlbum		"__SysReservedAlbum__"