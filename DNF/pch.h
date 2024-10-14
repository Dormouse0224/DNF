#pragma once

#define _AFXDLL

#include "framework.h"




#include <vector>
#include <list>
#include <map>
#include <string>
#include <iostream>
#include <fstream>


//#include <ole2.h>
#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib")

#include "zlib.h"
#pragma comment (lib, "zlibwapi.lib")

#include "DirectXTex.h"
#ifdef _DEBUG
#pragma comment (lib, "DirectXTex.lib")
#else
#pragma comment (lib, "DirectXTexRelease.lib")
#endif

#include "vld.h"
#pragma comment (lib, "vld.lib")

using namespace Gdiplus;

using std::wstring;
using std::string;
using std::vector;
using std::list;
using std::map;
using std::make_pair;
using std::ifstream;
using std::ofstream;
using std::ios;


#include "global.h"

#include <assert.h>
