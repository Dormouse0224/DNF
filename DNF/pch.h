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

// ZLIB ���� �˰��� ���̺귯��
#include "zlib.h"
#pragma comment (lib, "zlibwapi.lib")

// DDS �� DXT �̹��� ó���� ���̺귯��
#include "DirectXTex.h"
#ifdef _DEBUG
#pragma comment (lib, "DirectXTex.lib")
#else
#pragma comment (lib, "DirectXTexRelease.lib")
#endif

// Visual Leak Detector (����׿����� ����)
#ifdef _DEBUG
#include "vld.h"
#pragma comment (lib, "vld.lib")
#endif

// ����
#define DIRECTINPUT_VERSION 0x0800 // DirectInput 8.0 ���� ���
#include <mmsystem.h>
#include <dsound.h>
#include <dinput.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dsound.lib")

// ogg ����� ���̺귯��
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
