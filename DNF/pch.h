#pragma once

#define _AFXDLL

#include "framework.h"


#include <vector>
#include <list>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

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
#define DIRECTINPUT_VERSION 0x0800 // DirectInput 8.0 버전 사용
#include <mmsystem.h>
#include <dsound.h>
#include <dinput.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dsound.lib")

// ogg 사운드용 라이브러리
#include <ogg.h>
#pragma comment(lib, "libogg.lib")
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include <vorbis/vorbisenc.h>
#pragma comment(lib, "libvorbis_static.lib")
#pragma comment(lib, "libvorbisfile_static.lib")



using std::wstring;
using std::string;
using std::vector;
using std::list;
using std::map;
using std::pair;
using std::make_pair;
using std::ifstream;
using std::ofstream;
using std::fstream;
using std::ios;


#include "global.h"

#include <assert.h>
