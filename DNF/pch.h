#pragma once

#define _AFXDLL

#include "framework.h"


#include <vector>
#include <list>
#include <map>
#include <string>
#include <iostream>
#include <fstream>

// GDI+
//#include <ole2.h>
#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib")
using namespace Gdiplus;

// ZLIB 압축 알고리즘 라이브러리
#include "zlib.h"
#pragma comment (lib, "zlibwapi.lib")

// DDS 및 DXT 이미지 처리용 라이브러리
#include "DirectXTex.h"
#ifdef _DEBUG
#pragma comment (lib, "DirectXTex.lib")
#else
#pragma comment (lib, "DirectXTexRelease.lib")
#endif

// Visual Leak Detector (디버그에서만 적용)
#ifdef _DEBUG
#include "vld.h"
#pragma comment (lib, "vld.lib")
#endif

// 사운드
#include <mmsystem.h>
#include <dsound.h>
#include <dinput.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dsound.lib")


using std::wstring;
using std::string;
using std::vector;
using std::list;
using std::map;
using std::make_pair;
using std::ifstream;
using std::ofstream;
using std::fstream;
using std::ios;


#include "global.h"

#include <assert.h>
