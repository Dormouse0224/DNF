#include "pch.h"
#include "CTextureMgr.h"
#include "CTexture.h"
#include "CAlbum.h"
#include "CNpkMgr.h"
#include "CEngine.h"
#include "CCameraMgr.h"
#include "CLevelMgr.h"
#include "CStage.h"

#include "DDSTextureLoader11.h"
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

CTextureMgr::CTextureMgr()
	: m_TempAlbum(nullptr)
	, m_SysReservedAlbum(nullptr)
	, m_pd3dDevice(nullptr)
	, m_pd3dContext(nullptr)
{

}

CTextureMgr::~CTextureMgr()
{
	for (map<string, CAlbum*>::iterator iter = m_Albums.begin(); iter != m_Albums.end(); ++iter)
	{
		if (iter->second != nullptr)
		{
			delete iter->second;
			iter->second = nullptr;
		}
	}
	m_Albums.clear();

	m_pd3dDevice->Release();
	m_pd3dContext->Release();
}

void CTextureMgr::Init()
{
	CAlbum* pAlbum1 = new CAlbum(L"__TempNPK__");
	pAlbum1->Path = "__TempAlbum__";
	m_TempAlbum = pAlbum1;
	m_Albums.insert(make_pair("__TempAlbum__", pAlbum1));


	CAlbum* pAlbum2 = new CAlbum(L"__SysReservedNPK__");
	pAlbum2->Path = "__SysReservedAlbum__";
	m_SysReservedAlbum = pAlbum2;
	m_Albums.insert(make_pair("__SysReservedAlbum__", pAlbum2));

	UINT Flag = 0;
#ifdef _DEBUG
	Flag = D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL level = D3D_FEATURE_LEVEL_11_0;

	// 디바이스 초기화
	D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, Flag
		, 0, 0, D3D11_SDK_VERSION, &m_pd3dDevice, &level, &m_pd3dContext);
}

void CTextureMgr::ClearTempAlbum()
{
	for (CTexture* pTex : m_TempAlbum->m_SceneVector)
	{
		delete pTex;
		pTex = nullptr;
	}
	m_TempAlbum->m_SceneVector.clear();
	m_TempAlbum->Count = 0;
}

void CTextureMgr::CreateFromNpk(ifstream& _file, CAlbum* _Album)
{
	switch (_Album->GetVersion())
	{
	case ImgVersion::Ver2:
	{
		CreateFromNpkV2(_file, _Album);
	}
		break;
	case ImgVersion::Ver4:
	{
		CreateFromNpkV4(_file, _Album);
	}
		break;
	case ImgVersion::Ver5:
	{
		CreateFromNpkV5(_file, _Album);
	}
		break;
	case ImgVersion::Ver6:
	{
		CreateFromNpkV6(_file, _Album);
	}
		break;
	}
}

void CTextureMgr::CreateFromNpkV2(ifstream& _file, CAlbum* _Album)
{
	map<CTexture*, int> dic;
	LONGLONG num = _file.tellg() + _Album->GetIndexLength();
	for (int index = 0; index < _Album->GetCount(); ++index)
	{
		CTexture* pTexture = new CTexture(std::to_wstring(index), _Album);
		pTexture->Index = _Album->GetSceneCount();
		pTexture->Type = (ColorBits)CNpkMgr::Read<int>(_file);
		_Album->AddScene(pTexture);
		if (pTexture->Type == ColorBits::LINK)
		{
			dic.insert(make_pair(pTexture, CNpkMgr::Read<int>(_file)));
		}
		else
		{
			int a = (int)_file.tellg();
			pTexture->CompressMode = (CompressMode)CNpkMgr::Read<int>(_file);
			pTexture->m_Size.x = (float)CNpkMgr::Read<int>(_file);
			pTexture->m_Size.y = (float)CNpkMgr::Read<int>(_file);
			pTexture->Length = CNpkMgr::Read<int>(_file);
			pTexture->m_Offset.x = (float)CNpkMgr::Read<int>(_file);
			pTexture->m_Offset.y = (float)CNpkMgr::Read<int>(_file);
			pTexture->m_CanvasSize.x = (float)CNpkMgr::Read<int>(_file);
			pTexture->m_CanvasSize.y = (float)CNpkMgr::Read<int>(_file);
		}
	}
	for (int index = 0; index < _Album->GetSceneCount(); ++index)
	{
		CTexture* pTexture = _Album->GetScene(index);
		if (pTexture->Type == ColorBits::LINK)
		{
			if (dic.find(pTexture) != dic.end() && dic[pTexture] < _Album->GetSceneCount() && dic[pTexture] > -1 && dic[pTexture] != pTexture->Index)
			{
				pTexture->Target = _Album->GetScene(dic[pTexture]);
				pTexture->m_Size = pTexture->Target->m_Size;
				pTexture->m_CanvasSize = pTexture->Target->m_CanvasSize;
				pTexture->m_Offset = pTexture->Target->m_Offset;
			}
		}
		else
		{
			if (pTexture->CompressMode == CompressMode::NONE)
				pTexture->Length = (int)pTexture->m_Size.x * (int)pTexture->m_Size.y * (pTexture->Type == ColorBits::ARGB_8888 ? 4 : 2);
			char* buffer = new char[pTexture->Length];
			_file.read(buffer, pTexture->Length);
			pTexture->Data = buffer;
		}
	}
}

void CTextureMgr::CreateFromNpkV4(ifstream& _file, CAlbum* _Album)
{
	int count = CNpkMgr::Read<int>(_file);
	vector<Color> Palette;
	vector<vector<Color>> paletteVec;
	Palette = ReadPalette(_file, count);
	paletteVec.push_back(Palette);
	_Album->SetPaletteVector(vector<vector<Color>>(paletteVec));
	CreateFromNpkV2(_file, _Album);
}

void CTextureMgr::CreateFromNpkV5(ifstream& _file, CAlbum* _Album)
{
	int num1 = CNpkMgr::Read<int>(_file);
	_Album->Length = CNpkMgr::Read<int>(_file);
	int count = CNpkMgr::Read<int>(_file);
	vector<vector<Color>> PaletteVec;
	PaletteVec.push_back(ReadPalette(_file, count));
	_Album->m_PaletteVector = PaletteVec;
	vector<CTexture*> TextureVec;
	for (int index = 0; index < num1; ++index)
	{
		CTexture* texture = new CTexture(std::to_wstring(index), _Album);
		texture->Version = (TextureVersion)CNpkMgr::Read<int>(_file);
		texture->Type = (ColorBits)CNpkMgr::Read<int>(_file);
		texture->Index = CNpkMgr::Read<int>(_file);
		texture->Length = CNpkMgr::Read<int>(_file);
		texture->FullLength = CNpkMgr::Read<int>(_file);
		texture->m_Size.x = (float)CNpkMgr::Read<int>(_file);
		texture->m_Size.y = (float)CNpkMgr::Read<int>(_file);
		TextureVec.push_back(texture);
	}
	map<CTexture*, int> dictionary;
	vector<CTexture*> spriteVec;
	for (int index1 = 0; index1 < _Album->Count; ++index1)
	{
		CTexture* pTexture = new CTexture(std::to_wstring(index1), _Album);
		pTexture->Index = _Album->GetSceneCount();
		pTexture->Type = (ColorBits)CNpkMgr::Read<int>(_file);
		_Album->AddScene(pTexture);
		if (pTexture->Type == ColorBits::LINK)
		{
			dictionary.insert(make_pair(pTexture, CNpkMgr::Read<int>(_file)));
		}
		else
		{
			pTexture->CompressMode = (CompressMode)CNpkMgr::Read<int>(_file);
			pTexture->m_Size.x = (float)CNpkMgr::Read<int>(_file);
			pTexture->m_Size.y = (float)CNpkMgr::Read<int>(_file);
			pTexture->Length = CNpkMgr::Read<int>(_file);
			pTexture->m_Offset.x = (float)CNpkMgr::Read<int>(_file);
			pTexture->m_Offset.y = (float)CNpkMgr::Read<int>(_file);
			pTexture->m_CanvasSize.x = (float)CNpkMgr::Read<int>(_file);
			pTexture->m_CanvasSize.y = (float)CNpkMgr::Read<int>(_file);
			if (pTexture->Type < ColorBits::LINK && pTexture->Length != 0)
			{
				spriteVec.push_back(pTexture);
			}
			else
			{
				int num2 = CNpkMgr::Read<int>(_file);
				int index2 = CNpkMgr::Read<int>(_file);
				CTexture* texture = TextureVec[index2];
				float x1 = (float)CNpkMgr::Read<int>(_file);
				float y1 = (float)CNpkMgr::Read<int>(_file);
				Vec2D point1 = Vec2D(x1, y1);
				float x2 = (float)CNpkMgr::Read<int>(_file);
				float y2 = (float)CNpkMgr::Read<int>(_file);
				Vec2D point2 = Vec2D(x2, y2);
				int num3 = CNpkMgr::Read<int>(_file);
				TextureInfo textureInfo;
				textureInfo.Unknown = num2;
				textureInfo.Texture = texture;
				textureInfo.LeftTop = point1;
				textureInfo.RightBottom = point2;
				textureInfo.Top = num3;
				int index = pTexture->Index;
				_Album->m_Map.insert(make_pair(index, textureInfo));
			}
		}
	}

	_Album->AtlasVec = TextureVec;
	for (map<CTexture*, int>::iterator iter = dictionary.begin(); iter != dictionary.end(); ++iter)
	{
		iter->first->Target = _Album->GetScene(iter->second);
	}
	for (CTexture* texture : TextureVec)
	{
		char* buf = new char[texture->Length];
		_file.read(buf, texture->Length);
		texture->Data = buf;
	}
	for (CTexture* sprite : spriteVec)
	{
		char* buf = new char[sprite->Length];
		_file.read(buf, sprite->Length);
		sprite->Data = buf;
	}
}

void CTextureMgr::CreateFromNpkV6(ifstream& _file, CAlbum* _Album)
{
	int num = CNpkMgr::Read<int>(_file);
	vector<vector<Color>> paletteVec;
	for (int index = 0; index < num; ++index)
	{
		vector<Color> Palette;
		int count = CNpkMgr::Read<int>(_file);
		Palette = ReadPalette(_file, count);
		paletteVec.push_back(Palette);
	}
	_Album->SetPaletteVector(vector<vector<Color>>(paletteVec));
	CreateFromNpkV2(_file, _Album);
}

Bitmap* CTextureMgr::ConvertToBitmap(CTexture* _pTexture)
{
	switch (_pTexture->m_Owner->GetVersion())
	{
	case ImgVersion::Ver2:
	{
		return ConvertToBitmapV2(_pTexture);
	}
	case ImgVersion::Ver4:
	{
		return ConvertToBitmapV4(_pTexture);
	}
	case ImgVersion::Ver5:
	{
		return ConvertToBitmapV5(_pTexture);
	}
	case ImgVersion::Ver6:
	{
		return ConvertToBitmapV6(_pTexture);
	}
	}
	return nullptr;
}

Bitmap* CTextureMgr::ConvertToBitmapV2(CTexture* _pTexture)
{
	bool bUncomped = false;
	char* data = _pTexture->Data;
	ColorBits type = _pTexture->Type;
	if (type == ColorBits::LINK)
	{
		return _pTexture->Target->m_Bitmap;
	}
	int size = (int)_pTexture->m_Size.x * (int)_pTexture->m_Size.y * (type == ColorBits::ARGB_8888 ? 4 : 2);
	if (_pTexture->CompressMode == CompressMode::ZLIB)
	{
		data = (char*)UncompressZlib(_pTexture->Data, size, _pTexture->Length);
		bUncomped = true;
	}
	Bitmap* pBitmap = BitmapFromArray(data, _pTexture->m_Size, type);
	if (bUncomped)
		delete[] data;
	return pBitmap;
}

Bitmap* CTextureMgr::ConvertToBitmapV4(CTexture* _pTexture)
{
	char* data = _pTexture->Data;
	int size = (int)(_pTexture->m_Size.x * _pTexture->m_Size.y);
	char* data2 = new char[size * 4];
	if (_pTexture->Type == ColorBits::ARGB_1555 && _pTexture->CompressMode == CompressMode::ZLIB)
	{
		if (size == 1 && _pTexture->Type != ColorBits::LINK)
		{
			//delete[] data;
			delete[] data2;
			return nullptr;
		}

		data = (char*)UncompressZlib(_pTexture->Data, size, _pTexture->Length);
		vector<Color> ColorTable = _pTexture->m_Owner->m_PaletteVector[0];
		if (ColorTable.size() > 0)
		{
			for (int index = 0; index < size; ++index)
			{
				char argb[4] =
				{
					(char)ColorTable[data[index]].GetA(),
					(char)ColorTable[data[index]].GetR(),
					(char)ColorTable[data[index]].GetG(),
					(char)ColorTable[data[index]].GetB()
				};
				WriteColor(data2 + (index * 4), argb, ColorBits::ARGB_8888);
			}
			delete[] data;
			Bitmap* bitmap = BitmapFromArray(data2, _pTexture->m_Size, ColorBits::ARGB_8888);
			delete[] data2;
			return bitmap;
		}
	}
	delete[] data2;
	return ConvertToBitmapV2(_pTexture);
}

Bitmap* CTextureMgr::ConvertToBitmapV5(CTexture* _pTexture)
{
	if (_pTexture->Type < ColorBits::LINK && _pTexture->Length > 0)
		return ConvertToBitmapV2(_pTexture);
	if (_pTexture->m_Owner->m_Map.find(_pTexture->Index) == _pTexture->m_Owner->m_Map.end())
		return nullptr /*new Bitmap(1, 1)*/;
	TextureInfo textureInfo = _pTexture->m_Owner->m_Map[_pTexture->Index];
	char* data = nullptr;
	data = (char*)UncompressZlib(textureInfo.Texture->Data, textureInfo.Texture->FullLength, textureInfo.Texture->Length);
	Bitmap* bitmap = ReadDDSFromArray(data, textureInfo.Texture->FullLength);
	if (textureInfo.Top != 0)
		bitmap->RotateFlip(RotateFlipType::Rotate270FlipNone);
	delete[] data;
	return bitmap;
}

Bitmap* CTextureMgr::ConvertToBitmapV6(CTexture* _pTexture)
{
	char* data1 = _pTexture->Data;
	int size = (int)(_pTexture->m_Size.x * _pTexture->m_Size.y);
	if (_pTexture->Type == ColorBits::ARGB_1555 && _pTexture->CompressMode == CompressMode::ZLIB)
	{
		char* data2 = (char*)UncompressZlib(data1, size, _pTexture->Length);
		vector<Color> ColorTable = _pTexture->m_Owner->m_PaletteVector[0];
		if (ColorTable.size() > 0)
		{
			for (int index = 0; index < size; ++index)
			{
				char argb[4] =
				{
					(char)ColorTable[data2[index]].GetA(),
					(char)ColorTable[data2[index]].GetR(),
					(char)ColorTable[data2[index]].GetG(),
					(char)ColorTable[data2[index]].GetB()
				};
				WriteColor(data2 + (index * 4), argb, ColorBits::ARGB_8888);
			}
			return BitmapFromArray(data2, _pTexture->m_Size, ColorBits::ARGB_8888);
		}
	}
	return ConvertToBitmapV2(_pTexture);
}


Bytef* CTextureMgr::UncompressZlib(const char* _data, int size, int _dataSize)
{
	Bytef* dest = new Bytef[size];
	uLongf destlen = size;
	int err = uncompress(dest, &destlen, (Bytef*)_data, _dataSize);
	assert(err == Z_OK);
	assert(destlen == size);
	return dest;
}

Bitmap* CTextureMgr::BitmapFromArray(const char* _data, Vec2D _size, ColorBits _bits)
{
	char* temp = new char[(size_t)(_size.x * _size.y * 4)];
	for (int index = 0; index < _size.x * _size.y; ++index)
	{
		char* src = nullptr;
		if (_bits == ColorBits::ARGB_8888)
			src = (char*)ReadColor(_data + (index * 4), _bits);
		else
			src = (char*)ReadColor(_data + (index * 2), _bits);
		memcpy(temp + (index * 4), src, 4);
		delete[] src;
	}
	Bitmap* bitmap = new Bitmap((INT)_size.x, (INT)_size.y, PixelFormat32bppARGB);
	BitmapData bitmapdata;
	Rect rect = Rect((INT)0, (INT)0, (INT)_size.x, (INT)_size.y);
	bitmap->LockBits(&rect, ImageLockModeWrite, PixelFormat32bppARGB, &bitmapdata);
	memcpy(bitmapdata.Scan0, temp, (size_t)(_size.x * _size.y * 4));
	bitmap->UnlockBits(&bitmapdata);
	delete[] temp;

	return bitmap;
}

void CTextureMgr::BitmapToArray(CTexture* _Texture)
{
	int pixelCount = (int)_Texture->m_Size.x * (int)_Texture->m_Size.y;
	char* temp = new char[pixelCount * 4];
	BitmapData bitmapdata;
	Rect rect = Rect((INT)0, (INT)0, (INT)_Texture->m_Size.x, (INT)_Texture->m_Size.y);
	_Texture->m_Bitmap->LockBits(&rect, ImageLockModeWrite, PixelFormat32bppARGB, &bitmapdata);
	memcpy(temp, bitmapdata.Scan0, (size_t)(pixelCount * 4));
	_Texture->m_Bitmap->UnlockBits(&bitmapdata);

	uLongf compLen = pixelCount * 4;
	char* comp = new char[compLen];
	compress((Bytef*)comp, &compLen, (Bytef*)temp, (uLong)(pixelCount * 4));
	delete[] temp;

	_Texture->Type = ColorBits::ARGB_8888;
	_Texture->CompressMode = CompressMode::ZLIB;
	_Texture->Length = compLen;
	_Texture->m_CanvasSize = _Texture->m_Size;
	_Texture->Data = comp;
}

byte* CTextureMgr::ReadColor(const char* _data, ColorBits _bits)
{
	int bits = (int)_bits;
	// ARGB_8888 인 경우
	if (bits == 16)
	{
		byte* buffer = new byte[4];
		memcpy(buffer, _data, 4);
		return buffer;
	}
	byte num1 = 0;
	byte num2 = 0;
	byte num3 = 0;
	byte num4 = 0;
	byte* buffer = new byte[2];
	memcpy(buffer, _data, 2);
	if (bits != 14)
	{
		// ARGB_4444 인 경우
		if (bits == 15)
		{
			num1 = (byte)((UINT)buffer[1] & 240U);
			num2 = (byte)(((int)buffer[1] & 15) << 4);
			num3 = (byte)((UINT)buffer[0] & 240U);
			num4 = (byte)(((int)buffer[0] & 15) << 4);
		}
	}
	// ARGB_1555 인 경우
	else
	{
		if (buffer[1])
			int a = 0;
		byte num5 = (byte)((UINT)buffer[1] >> 7);
		byte num6 = (byte)((int)buffer[1] >> 2 & 31);
		byte num7 = (byte)((int)buffer[0] >> 5 | ((int)buffer[1] & 3) << 3);
		byte num8 = (byte)((UINT)buffer[0] & 31U);
		num1 = (byte)((UINT)num5 * (UINT)255);
		num2 = (byte)((int)num6 << 3 | (int)num6 >> 2);
		num3 = (byte)((int)num7 << 3 | (int)num7 >> 2);
		num4 = (byte)((int)num8 << 3 | (int)num8 >> 2);
	}
	delete[] buffer;
	return new byte[4]{ num4, num3, num2, num1 };
}

void CTextureMgr::WriteColor(char* _dest, const char* _src, ColorBits _bits)
{
	if (_bits == ColorBits::ARGB_8888)
	{
		memcpy(_dest, _src, 4);
	}
	else
	{
		char num1 = _src[0];	// A
		char num2 = _src[1];	// R
		char num3 = _src[2];	// G
		char num4 = _src[3];	// B
		int num5 = 0;
		int num6 = 0;
		if (_bits != ColorBits::ARGB_1555)
		{
			if (_bits == ColorBits::ARGB_4444)
			{
				num5 = (int)num3 | (int)num4 >> 4;
				num6 = (int)num1 | (int)num2 >> 4;
			}
		}
		else
		{
			char num7 = (char)((UINT)num1 >> 7);
			char num8 = (char)((UINT)num2 >> 3);
			char num9 = (char)((UINT)num3 >> 3);
			char num10 = (char)((UINT)num4 >> 3);
			num5 = (int)(char)((UINT)(((int)num9 & 7) << 5) | (UINT)num10);
			num6 = (int)(char)((int)num7 << 7 | (int)num8 << 2 | (int)num9 >> 3);
		}
		_dest[0] = (char)num5;
		_dest[1] = (char)num6;
	}
}

vector<CAlbum*> CTextureMgr::LoadNPK(wstring _NpkPath)
{
	vector<CAlbum*> AlbumList;
	map<wstring, vector<CAlbum*>>::iterator iter = m_NPKs.find(_NpkPath);
	if (iter == m_NPKs.end())
	{
		ifstream readfile;
		readfile.open(_NpkPath, ios::binary);
		assert(readfile.is_open());	// 파일 읽기 오류
		// NPK 파일 읽어서 앨범 저장
		AlbumList = CNpkMgr::GetInst()->ReadNpk(readfile, _NpkPath);
		readfile.close();
		for (CAlbum* _album : AlbumList)
		{
			m_Albums.insert(make_pair(_album->GetPath(), _album));
		}
		m_NPKs.insert(make_pair(_NpkPath, AlbumList));
	}
	else
	{
		AlbumList = iter->second;
	}

	return AlbumList;
}

CAlbum* CTextureMgr::LoadAlbum(string _AlbumPath, wstring _NpkPath)
{
	CAlbum* result = nullptr;
	// 앨범 데이터 가져왔는지 검사
	map<string, CAlbum*>::iterator iter = m_Albums.find(_AlbumPath);
	if (_NpkPath != L"__TempNPK__" && _NpkPath != L"__SysReservedNPK__")
	{
		// 앨범을 가져온 적 없는 경우 - 앨범이 속한 NPK 파일 로드 후 앨범 데이터 저장, 요청된 앨범은 텍스처를 메모리에 로드
		if (iter == m_Albums.end())
		{
			// 앨범을 찾을 수 없는데 해당 앨범이 속한 npk는 등록되어 있는 경우, 없는 앨범이므로 중단
			map<wstring, vector<CAlbum*>>::iterator iter1 = m_NPKs.find(_NpkPath);
			if (iter1 != m_NPKs.end())
				return nullptr;

			ifstream readfile;
			readfile.open(_NpkPath, ios::binary);
			assert(readfile.is_open());	// 파일 읽기 오류
			// NPK 파일 읽어서 앨범 저장
			vector<CAlbum*> AlbumList = CNpkMgr::GetInst()->ReadNpk(readfile, _NpkPath);
			readfile.close();
			for (CAlbum* _album : AlbumList)
			{
				m_Albums.insert(make_pair(_album->GetPath(), _album));
				if (_album->GetPath() == _AlbumPath)
				{
					if (m_MultithreadLoad)
						AddLoadQueue(_album);
					else
					{
						for (int i = 0; i < _album->GetSceneCount(); ++i)
						{
							_album->GetScene(i)->Load();
						}
					}


					result = _album;
				}
			}
			m_NPKs.insert(make_pair(_NpkPath, AlbumList));
		}
		// 앨범을 가져온 적이 있으면 엘범 정보를 바탕으로 메모리에 로드 (Texture 로드 호출 시 이미 메모리에 로드된 경우 다시 로드하지 않고 무시됨)
		else
		{
			if (m_MultithreadLoad)
				AddLoadQueue(iter->second);
			else
			{
				for (int i = 0; i < iter->second->GetSceneCount(); ++i)
				{
					iter->second->GetScene(i)->Load();
				}
			}

			result = iter->second;
		}
	}
	return result;
}

void CTextureMgr::LoadAll(wstring _NpkPath)
{
	bool bLoaded = false;
	if (_NpkPath != L"__TempNPK__" && _NpkPath != L"__SysReservedNPK__")
	{
		
		
		// npk는 등록되어 있는 경우, 속한 앨범 중 하나 이상이 로드된 적이 있음
		map<wstring, vector<CAlbum*>>::iterator iter1 = m_NPKs.find(_NpkPath);
		if (iter1 != m_NPKs.end())
			bLoaded = true;

		ifstream readfile;
		readfile.open(_NpkPath, ios::binary);
		assert(readfile.is_open());	// 파일 읽기 오류
		// NPK 파일 읽어서 앨범 저장
		vector<CAlbum*> AlbumList = CNpkMgr::GetInst()->ReadNpk(readfile, _NpkPath);
		readfile.close();
		for (CAlbum* _album : AlbumList)
		{
			if (!bLoaded)
				m_Albums.insert(make_pair(_album->GetPath(), _album));

			//AddLoadQueue(_album);
			for (int i = 0; i < _album->GetSceneCount(); ++i)
			{
				_album->GetScene(i)->Load();
			}

		}
		if (!bLoaded)
			m_NPKs.insert(make_pair(_NpkPath, AlbumList));
	}
}


void CTextureMgr::SaveAlbum(string _AlbumName, string _Directory)
{
	ofstream writeAlbum;
	writeAlbum.open(_Directory, ios::binary);
	CNpkMgr::GetInst()->WriteNpk(writeAlbum, _AlbumName);
	writeAlbum.close();
}


vector<Color> CTextureMgr::ReadPalette(ifstream& _file, int count)
{
	vector<Color> Palette;
	for (int i = 0; i < count; ++i)
	{
		char buf[4] = {};
		_file.read(buf, 4);
		Palette.push_back(Color(Color::MakeARGB(buf[2], buf[1], buf[0], buf[3])));
	}
	return Palette;
}

std::mutex textureMutex;
Bitmap* CTextureMgr::ReadDDSFromArray(const char* _DDSdata, int _DDSdataSize)
{
	std::lock_guard<std::mutex> lock(textureMutex);

	HRESULT err;
	if (_DDSdataSize < sizeof(DDSHeader))
		return nullptr;

	// DDS 헤더 읽기
	DDSHeader header;
	memcpy(&header, _DDSdata, sizeof(DDSHeader));

	// 텍스처 데이터 위치 및 크기 계산 후 복사
	int textureDataOffset = sizeof(DDSHeader);
	int textureDataSize = _DDSdataSize - textureDataOffset;
	if (textureDataSize <= 0)
		return nullptr;
	char* textureData = new char[textureDataSize];
	memcpy(textureData, _DDSdata + textureDataOffset, textureDataSize);

	// 텍스처 설명 설정
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = header.dwWidth;
	desc.Height = header.dwHeight;
	desc.MipLevels = header.dwMipMapCount ? header.dwMipMapCount : 1;
	desc.ArraySize = 1;
	if (header.ddspf.dwFourCC == MAKEFOURCC('D', 'X', 'T', '1'))
	{
		desc.Format = DXGI_FORMAT_BC1_UNORM;
	}
	else if (header.ddspf.dwFourCC == MAKEFOURCC('D', 'X', 'T', '3'))
	{
		desc.Format = DXGI_FORMAT_BC2_UNORM;
	}
	else if (header.ddspf.dwFourCC == MAKEFOURCC('D', 'X', 'T', '5'))
	{
		desc.Format = DXGI_FORMAT_BC3_UNORM;
	}
	else
	{
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	}
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	// 서픽스 서술자 작성
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = textureData;
	initData.SysMemPitch = header.dwPitchOrLinearSize;
	initData.SysMemSlicePitch = 0;


	
	ID3D11Texture2D* texture = nullptr;
	ID3D11ShaderResourceView* textureView = nullptr;
	
	
	
	// DDS 메모리로부터 텍스처 로드
	err = DirectX::CreateDDSTextureFromMemory(m_pd3dDevice, (const uint8_t*)_DDSdata, (size_t)_DDSdataSize, (ID3D11Resource**)&texture, &textureView);
	assert(err == S_OK);


	D3D11_TEXTURE2D_DESC textureDesc;
	texture->GetDesc(&textureDesc);

	// 텍스처를 CPU로 복사할 수 있는 형식으로 새 텍스처 생성
	D3D11_TEXTURE2D_DESC cpuTextureDesc = textureDesc;
	cpuTextureDesc.Usage = D3D11_USAGE_STAGING;
	cpuTextureDesc.BindFlags = 0;
	cpuTextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	cpuTextureDesc.MiscFlags = 0;


	ID3D11Texture2D* pStagingTexture = nullptr;
	err = m_pd3dDevice->CreateTexture2D(&cpuTextureDesc, nullptr, &pStagingTexture);
	if (FAILED(err))
	{
		// 할당된 메모리 해제
		delete[] textureData;
		return nullptr;
	}

	// 원본 텍스처를 스테이징 텍스처로 복사
	if (pStagingTexture == nullptr)
		return nullptr;
	m_pd3dContext->CopyResource(pStagingTexture, texture);


	// 텍스처 맵핑
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	err = m_pd3dContext->Map(pStagingTexture, 0, D3D11_MAP_READ, 0, &mappedResource);
	assert(err == S_OK);


	// DXT 포맷으로 압축된 텍스처를 압축해제
	DirectX::Image BCtex;
	BCtex.width = textureDesc.Width;
	BCtex.height = textureDesc.Height;
	BCtex.format = textureDesc.Format;
	BCtex.rowPitch = mappedResource.RowPitch;
	BCtex.slicePitch = mappedResource.DepthPitch;
	BCtex.pixels = (uint8_t*)mappedResource.pData;

	//DirectX::ScratchImage DecompressedScratchImage;
	DirectX::ScratchImage* DecompressedScratchImage = new DirectX::ScratchImage;

	err = DirectX::Decompress(BCtex, DXGI_FORMAT_R8G8B8A8_UNORM, *DecompressedScratchImage);

	const DirectX::Image* DecompImg;
	DecompImg = DecompressedScratchImage->GetImage(0, 0, 0);



	// 텍스처 데이터를 GDI+ Bitmap으로 변환
	UINT width = (UINT)DecompImg->width;
	UINT height = (UINT)DecompImg->height;
	Bitmap* pBitmap = new Bitmap(width, height, PixelFormat32bppARGB);
	Gdiplus::BitmapData bmpData;
	Gdiplus::Rect rect(0, 0, width, height);
	if (pBitmap->LockBits(&rect, Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &bmpData) == Gdiplus::Ok)
	{
		for (UINT y = 0; y < height; ++y)
		{
			char* srcRow = (char*)DecompImg->pixels + y * DecompImg->rowPitch;
			char* destRow = (char*)bmpData.Scan0 + y * bmpData.Stride;
			for (UINT x = 0; x < width; ++x)
			{
				// RGBA 픽셀 데이터 (4바이트씩)
				char* pixel = srcRow + (x * 4);
				// RGBA -> ARGB로 변환
				destRow[x * 4 + 0] = pixel[2];  // A (Alpha)
				destRow[x * 4 + 1] = pixel[1];  // R (Red)
				destRow[x * 4 + 2] = pixel[0];  // G (Green)
				destRow[x * 4 + 3] = pixel[3];  // B (Blue)
			}
		}
	}
	pBitmap->UnlockBits(&bmpData);

	// D3D11 객체 해제
	textureView->Release();
	texture->Release();
	pStagingTexture->Release();
	//DecompressedScratchImage->Release();
	delete DecompressedScratchImage;


	// 할당된 메모리 해제
	delete[] textureData;

	return pBitmap;
}

CTexture* CTextureMgr::CreateRectTexture(wstring _Name, Vec2D _size, Vec2D _offset, Color _color, bool _IsAddedBySys)
{
	// 같은 이름의 텍스쳐가 시스템 예약 앨범 또는 임시 앨범에 없어야함
	assert(m_SysReservedAlbum->GetScene(_Name) == nullptr);
	assert(m_TempAlbum->GetScene(_Name) == nullptr);

	CTexture* pTex = new CTexture(_Name, m_SysReservedAlbum);
	pTex->m_Size = _size;
	pTex->m_Offset = _offset;
	pTex->m_Bitmap = new Bitmap((INT)(_size.x), (INT)(_size.y), PixelFormat32bppARGB);

	Graphics graphics(pTex->m_Bitmap);
	graphics.Clear(Color(0, 0, 0, 0));
	Pen pen(_color);
	graphics.DrawRectangle(&pen, _offset.x, _offset.y, _size.x, _size.y);

	if (_IsAddedBySys)
	{
		m_SysReservedAlbum->AddScene(pTex);
	}
	else
	{
		m_TempAlbum->AddScene(pTex);
	}

	return pTex;
}

CTexture* CTextureMgr::LoadFromFile(wstring _filepath)
{
	wstring filename = _filepath;
	filename = filename.substr(filename.rfind('\\') + 1, filename.rfind('.') - filename.rfind('\\'));
	CTexture* newTex = new CTexture(filename, m_TempAlbum);
	m_TempAlbum->AddScene(newTex);
	++m_TempAlbum->Count;

	newTex->m_Bitmap = Bitmap::FromFile(_filepath.c_str());
	newTex->m_Size = Vec2D(newTex->m_Bitmap->GetWidth(), newTex->m_Bitmap->GetHeight());
	newTex->m_Offset = Vec2D(0, 0);
	return newTex;
}

CAlbum* CTextureMgr::GetAlbum(string _AlbumPath)
{
	map<string, CAlbum*>::iterator iter = m_Albums.find(_AlbumPath);
	if (iter != m_Albums.end())
	{
		return iter->second;
	}
	return nullptr;
}

void CTextureMgr::DrawLine(Color _color, int _width, Vec2D _begin, Vec2D _end, bool bCameraFallow)
{
	Pen pen(_color, (REAL)_width);
	Graphics graphics(CEngine::GetInst()->GetBackbuffer()->GetBitmap());

	// 프레임 방어를 위한 그래픽스 렌더링 세팅
	graphics.SetCompositingMode(CompositingModeSourceOver);			// 알파블렌딩 적용
	graphics.SetCompositingQuality(CompositingQualityHighSpeed);	// 감마 보정 미적용
	graphics.SetPixelOffsetMode(PixelOffsetModeNone);				// 픽섹 위치 조정 미적용
	graphics.SetSmoothingMode(SmoothingModeNone);					// 안티앨리어싱 미적용
	graphics.SetInterpolationMode(InterpolationModeLowQuality);		// 이미지 보간 미적용

	Vec2D CameraPos = CCameraMgr::GetInst()->GetCameraPos();
	if (bCameraFallow)
	{
		CameraPos = Vec2D(0, 0);
	}
	Vec2D pt1 = CameraPos;
	Vec2D pt2 = CameraPos + Vec2D(1066, 600);
	if (LineInRectCheck(_begin, _end, pt1, pt2))
	{
		graphics.DrawLine(&pen, _begin.x - CameraPos.x, _begin.y - CameraPos.y, _end.x - CameraPos.x, _end.y - CameraPos.y);
	}
}

void CTextureMgr::DrawLine(Color _color, int _width, int _Len, Vec2D _LinePos, float _LineAngle, bool bCameraFallow)
{
	float rad = _LineAngle * PI / 180.f;
	Vec2D DirVec(cosf(rad), sinf(rad));
	Vec2D _begin = _LinePos + (DirVec * _Len / 2);
	Vec2D _end = _LinePos - (DirVec * _Len / 2);

	Pen pen(_color, (REAL)_width);
	Graphics graphics(CEngine::GetInst()->GetBackbuffer()->GetBitmap());

	// 프레임 방어를 위한 그래픽스 렌더링 세팅
	graphics.SetCompositingMode(CompositingModeSourceOver);			// 알파블렌딩 적용
	graphics.SetCompositingQuality(CompositingQualityHighSpeed);	// 감마 보정 미적용
	graphics.SetPixelOffsetMode(PixelOffsetModeNone);				// 픽섹 위치 조정 미적용
	graphics.SetSmoothingMode(SmoothingModeNone);					// 안티앨리어싱 미적용
	graphics.SetInterpolationMode(InterpolationModeLowQuality);		// 이미지 보간 미적용

	Vec2D CameraPos = CCameraMgr::GetInst()->GetCameraPos();
	if (bCameraFallow)
	{
		CameraPos = Vec2D(0, 0);
	}
	Vec2D pt1 = CameraPos;
	Vec2D pt2 = CameraPos + Vec2D(1066, 600);
	if (LineInRectCheck(_begin, _end, pt1, pt2))
	{
		graphics.DrawLine(&pen, _begin.x - CameraPos.x, _begin.y - CameraPos.y, _end.x - CameraPos.x, _end.y - CameraPos.y);
	}
}

void CTextureMgr::DrawRect(Color _color, int _width, Vec2D _LeftTop, Vec2D _size, bool bCameraFallow)
{
	Pen pen(_color, (REAL)_width);
	Graphics graphics(CEngine::GetInst()->GetBackbuffer()->GetBitmap());

	// 프레임 방어를 위한 그래픽스 렌더링 세팅
	graphics.SetCompositingMode(CompositingModeSourceOver);			// 알파블렌딩 적용
	graphics.SetCompositingQuality(CompositingQualityHighSpeed);	// 감마 보정 미적용
	graphics.SetPixelOffsetMode(PixelOffsetModeNone);				// 픽섹 위치 조정 미적용
	graphics.SetSmoothingMode(SmoothingModeNone);					// 안티앨리어싱 미적용
	graphics.SetInterpolationMode(InterpolationModeLowQuality);		// 이미지 보간 미적용

	Vec2D CameraPos = CCameraMgr::GetInst()->GetCameraPos();
	if (bCameraFallow)
	{
		CameraPos = Vec2D(0, 0);
	}
	Vec2D RectCenter = _LeftTop + (_size / 2);
	if ((RectCenter.x - (CameraPos.x + CEngine::GetInst()->GetResolution().x / 2)) < ((CEngine::GetInst()->GetResolution().x / 2) + _size.x)
		|| (RectCenter.y - (CameraPos.y + CEngine::GetInst()->GetResolution().y / 2)) < ((CEngine::GetInst()->GetResolution().y / 2) + _size.y))
	{
		graphics.DrawRectangle(&pen, _LeftTop.x - CameraPos.x, _LeftTop.y - CameraPos.y, _size.x, _size.y);
	}
	
}

void CTextureMgr::FillRect(Color _color, Vec2D _LeftTop, Vec2D _size, bool bCameraFallow)
{
	SolidBrush brush(_color);
	Graphics graphics(CEngine::GetInst()->GetBackbuffer()->GetBitmap());

	// 프레임 방어를 위한 그래픽스 렌더링 세팅
	graphics.SetCompositingMode(CompositingModeSourceOver);			// 알파블렌딩 적용
	graphics.SetCompositingQuality(CompositingQualityHighSpeed);	// 감마 보정 미적용
	graphics.SetPixelOffsetMode(PixelOffsetModeNone);				// 픽섹 위치 조정 미적용
	graphics.SetSmoothingMode(SmoothingModeNone);					// 안티앨리어싱 미적용
	graphics.SetInterpolationMode(InterpolationModeLowQuality);		// 이미지 보간 미적용

	Vec2D CameraPos = CCameraMgr::GetInst()->GetCameraPos();
	if (bCameraFallow)
	{
		CameraPos = Vec2D(0, 0);
	}
	Vec2D RectCenter = _LeftTop + (_size / 2);
	if ((RectCenter.x - (CameraPos.x + CEngine::GetInst()->GetResolution().x / 2)) < ((CEngine::GetInst()->GetResolution().x / 2) + _size.x)
		|| (RectCenter.y - (CameraPos.y + CEngine::GetInst()->GetResolution().y / 2)) < ((CEngine::GetInst()->GetResolution().y / 2) + _size.y))
	{
		graphics.FillRectangle(&brush, _LeftTop.x - CameraPos.x, _LeftTop.y - CameraPos.y, _size.x, _size.y);
	}
}

void CTextureMgr::DrawEllipse(Color _color, int _width, Vec2D _LeftTop, Vec2D _size, bool bCameraFallow)
{
	Pen pen(_color, (REAL)_width);
	Graphics graphics(CEngine::GetInst()->GetBackbuffer()->GetBitmap());

	// 프레임 방어를 위한 그래픽스 렌더링 세팅
	graphics.SetCompositingMode(CompositingModeSourceOver);			// 알파블렌딩 적용
	graphics.SetCompositingQuality(CompositingQualityHighSpeed);	// 감마 보정 미적용
	graphics.SetPixelOffsetMode(PixelOffsetModeNone);				// 픽섹 위치 조정 미적용
	graphics.SetSmoothingMode(SmoothingModeNone);					// 안티앨리어싱 미적용
	graphics.SetInterpolationMode(InterpolationModeLowQuality);		// 이미지 보간 미적용

	Vec2D CameraPos = CCameraMgr::GetInst()->GetCameraPos();
	if (bCameraFallow)
	{
		CameraPos = Vec2D(0, 0);
	}
	Vec2D RectCenter = _LeftTop + (_size / 2);
	if ((RectCenter.x - (CameraPos.x + CEngine::GetInst()->GetResolution().x / 2)) < ((CEngine::GetInst()->GetResolution().x / 2) + _size.x)
		|| (RectCenter.y - (CameraPos.y + CEngine::GetInst()->GetResolution().y / 2)) < ((CEngine::GetInst()->GetResolution().y / 2) + _size.y))
	{
		graphics.DrawEllipse(&pen, (int)(_LeftTop.x - CameraPos.x), (int)(_LeftTop.y - CameraPos.y), (int)(_size.x), (int)(_size.y));
	}
}

void CTextureMgr::FillEllipse(Color _color, Vec2D _LeftTop, Vec2D _size, bool bCameraFallow)
{
	SolidBrush brush(_color);
	Graphics graphics(CEngine::GetInst()->GetBackbuffer()->GetBitmap());

	// 프레임 방어를 위한 그래픽스 렌더링 세팅
	graphics.SetCompositingMode(CompositingModeSourceOver);			// 알파블렌딩 적용
	graphics.SetCompositingQuality(CompositingQualityHighSpeed);	// 감마 보정 미적용
	graphics.SetPixelOffsetMode(PixelOffsetModeNone);				// 픽섹 위치 조정 미적용
	graphics.SetSmoothingMode(SmoothingModeNone);					// 안티앨리어싱 미적용
	graphics.SetInterpolationMode(InterpolationModeLowQuality);		// 이미지 보간 미적용

	Vec2D CameraPos = CCameraMgr::GetInst()->GetCameraPos();
	if (bCameraFallow)
	{
		CameraPos = Vec2D(0, 0);
	}
	Vec2D RectCenter = _LeftTop + (_size / 2);
	if ((RectCenter.x - (CameraPos.x + CEngine::GetInst()->GetResolution().x / 2)) < ((CEngine::GetInst()->GetResolution().x / 2) + _size.x)
		|| (RectCenter.y - (CameraPos.y + CEngine::GetInst()->GetResolution().y / 2)) < ((CEngine::GetInst()->GetResolution().y / 2) + _size.y))
	{
		graphics.FillEllipse(&brush, (int)(_LeftTop.x - CameraPos.x), (int)(_LeftTop.y - CameraPos.y), (int)(_size.x), (int)(_size.y));
	}
}

void CTextureMgr::FillDonut(Color _color, Vec2D _OuterLeftTop, Vec2D _OuterSize, Vec2D _InnerLeftTop, Vec2D _InnerSize, bool bCameraFallow)
{
	SolidBrush brush(_color);
	Graphics graphics(CEngine::GetInst()->GetBackbuffer()->GetBitmap());
	Gdiplus::GraphicsPath path;

	// 프레임 방어를 위한 그래픽스 렌더링 세팅
	graphics.SetCompositingMode(CompositingModeSourceOver);			// 알파블렌딩 적용
	graphics.SetCompositingQuality(CompositingQualityHighSpeed);	// 감마 보정 미적용
	graphics.SetPixelOffsetMode(PixelOffsetModeNone);				// 픽섹 위치 조정 미적용
	graphics.SetSmoothingMode(SmoothingModeNone);					// 안티앨리어싱 미적용
	graphics.SetInterpolationMode(InterpolationModeLowQuality);		// 이미지 보간 미적용

	Vec2D CameraPos = CCameraMgr::GetInst()->GetCameraPos();
	if (bCameraFallow)
	{
		CameraPos = Vec2D(0, 0);
	}

	// 큰 원 추가
	path.AddEllipse(_OuterLeftTop.x - CameraPos.x, _OuterLeftTop.y - CameraPos.y, _OuterSize.x, _OuterSize.y);
	// 작은 원을 뺌
	path.AddEllipse(_InnerLeftTop.x - CameraPos.x, _InnerLeftTop.y - CameraPos.y, _InnerSize.x, _InnerSize.y);

	Vec2D RectCenter = _OuterLeftTop + (_OuterSize / 2);
	if ((RectCenter.x - (CameraPos.x + CEngine::GetInst()->GetResolution().x / 2)) < ((CEngine::GetInst()->GetResolution().x / 2) + _OuterSize.x)
		|| (RectCenter.y - (CameraPos.y + CEngine::GetInst()->GetResolution().y / 2)) < ((CEngine::GetInst()->GetResolution().y / 2) + _OuterSize.y))
	{
		graphics.FillPath(&brush, &path);
	}
}

void CTextureMgr::FillDividedLine(Color _color, Vec2D _LinePos, float _LineAngle, bool bCameraFallow)
{
	SolidBrush brush(_color);
	Graphics graphics(CEngine::GetInst()->GetBackbuffer()->GetBitmap());
	Gdiplus::GraphicsPath path;

	// 프레임 방어를 위한 그래픽스 렌더링 세팅
	graphics.SetCompositingMode(CompositingModeSourceOver);			// 알파블렌딩 적용
	graphics.SetCompositingQuality(CompositingQualityHighSpeed);	// 감마 보정 미적용
	graphics.SetPixelOffsetMode(PixelOffsetModeNone);				// 픽섹 위치 조정 미적용
	graphics.SetSmoothingMode(SmoothingModeNone);					// 안티앨리어싱 미적용
	graphics.SetInterpolationMode(InterpolationModeLowQuality);		// 이미지 보간 미적용

	Vec2D CameraPos = CCameraMgr::GetInst()->GetCameraPos();
	if (bCameraFallow)
	{
		CameraPos = Vec2D(0, 0);
	}

	CStage* pStage = (CStage*)CLevelMgr::GetInst()->GetCurrentLevel();
	// 직선 위치점 기준 레벨의 각 꼭짓점 위치벡터
	Vec2D RectA[4] =
	{
		Vec2D(0, 0) - _LinePos,
		Vec2D(0, 0) + Vec2D(pStage->GetStageInfo()->StageSize.x, 0.f) - _LinePos,
		Vec2D(0, 0) + pStage->GetStageInfo()->StageSize - _LinePos,
		Vec2D(0, 0) + Vec2D(0.f, pStage->GetStageInfo()->StageSize.y) - _LinePos,
	};
	Vec2D RectB[4] =
	{
		Vec2D(0, 0),
		Vec2D(0, 0) + Vec2D(pStage->GetStageInfo()->StageSize.x, 0.f),
		Vec2D(0, 0) + pStage->GetStageInfo()->StageSize,
		Vec2D(0, 0) + Vec2D(0.f, pStage->GetStageInfo()->StageSize.y),
	};
	_LineAngle = std::fmod(_LineAngle, 360.0);
	if (_LineAngle < 0) 
	{
		_LineAngle += 360.0;
	}
	float rad = _LineAngle * PI / 180.f;
	Vec2D DirVec(cosf(rad), sinf(rad));

	if ((_LineAngle >= 0 && _LineAngle < 90) || (_LineAngle >= 180 && _LineAngle < 270))
	{
		int i = _LineAngle / 90;
		Vec2D temp;
		if (DirVec.Cross(RectA[(i + 2) % 4]) < 0)
		{
			path.AddLine(RectB[(i + 1) % 4].x - CameraPos.x, RectB[(i + 1) % 4].y - CameraPos.y, RectB[(i + 2) % 4].x - CameraPos.x, RectB[(i + 2) % 4].y - CameraPos.y);
			path.AddLine(RectB[(i + 2) % 4].x - CameraPos.x, RectB[(i + 2) % 4].y - CameraPos.y, (RectB[(i + 2) % 4].y - _LinePos.y) / tanf(rad) + _LinePos.x - CameraPos.x, RectB[(i + 2) % 4].y - CameraPos.y);
			temp = Vec2D((RectB[(i + 2) % 4].y - _LinePos.y) / tanf(rad) + _LinePos.x, RectB[(i + 2) % 4].y);
		}
		else
		{
			path.AddLine(RectB[(i + 1) % 4].x - CameraPos.x, RectB[(i + 1) % 4].y - CameraPos.y, RectB[(i + 2) % 4].x - CameraPos.x, (RectB[(i + 2) % 4].x - _LinePos.x) * tanf(rad) + _LinePos.y - CameraPos.y);
			temp = Vec2D(RectB[(i + 2) % 4].x, (RectB[(i + 2) % 4].x - _LinePos.x) * tanf(rad) + _LinePos.y);
		}

		if (DirVec.Cross(RectA[i % 4]) < 0)
		{
			path.AddLine(temp.x - CameraPos.x, temp.y - CameraPos.y, RectB[i % 4].x - CameraPos.x, (RectB[i % 4].x - _LinePos.x) * tanf(rad) + _LinePos.y - CameraPos.y);
			path.AddLine(RectB[i % 4].x - CameraPos.x, (RectB[i % 4].x - _LinePos.x) * tanf(rad) + _LinePos.y - CameraPos.y, RectB[i % 4].x - CameraPos.x, RectB[i % 4].y - CameraPos.y);
			path.AddLine(RectB[i % 4].x - CameraPos.x, RectB[i % 4].y - CameraPos.y, RectB[(i + 1) % 4].x - CameraPos.x, RectB[(i + 1) % 4].y - CameraPos.y);
		}
		else
		{
			path.AddLine(temp.x - CameraPos.x, temp.y - CameraPos.y, (RectB[i % 4].y - _LinePos.y) / tanf(rad) + _LinePos.x - CameraPos.x, RectB[i % 4].y - CameraPos.y);
			path.AddLine((RectB[i % 4].y - _LinePos.y) / tanf(rad) + _LinePos.x - CameraPos.x, RectB[i % 4].y - CameraPos.y, RectB[(i + 1) % 4].x - CameraPos.x, RectB[(i + 1) % 4].y - CameraPos.y);
		}
	}
	else
	{
		int i = _LineAngle / 90;
		Vec2D temp;
		if (DirVec.Cross(RectA[(i + 2) % 4]) < 0)
		{
			path.AddLine(RectB[(i + 1) % 4].x - CameraPos.x, RectB[(i + 1) % 4].y - CameraPos.y, RectB[(i + 2) % 4].x - CameraPos.x, RectB[(i + 2) % 4].y - CameraPos.y);
			path.AddLine(RectB[(i + 2) % 4].x - CameraPos.x, RectB[(i + 2) % 4].y - CameraPos.y, RectB[(i + 2) % 4].x - CameraPos.x, (RectB[(i + 2) % 4].x - _LinePos.x) * tanf(rad) + _LinePos.y - CameraPos.y);
			temp = Vec2D(RectB[(i + 2) % 4].x, (RectB[(i + 2) % 4].x - _LinePos.x) * tanf(rad) + _LinePos.y);
		}
		else
		{
			path.AddLine(RectB[(i + 1) % 4].x - CameraPos.x, RectB[(i + 1) % 4].y - CameraPos.y, (RectB[(i + 1) % 4].y - _LinePos.y) / tanf(rad) + _LinePos.x - CameraPos.x, RectB[(i + 1) % 4].y - CameraPos.y);
			temp = Vec2D((RectB[(i + 1) % 4].y - _LinePos.y) / tanf(rad) + _LinePos.x, RectB[(i + 1) % 4].y);
		}

		if (DirVec.Cross(RectA[i % 4]) < 0)
		{
			path.AddLine(temp.x - CameraPos.x, temp.y - CameraPos.y, (RectB[i % 4].y - _LinePos.y) / tanf(rad) + _LinePos.x - CameraPos.x, RectB[i % 4].y - CameraPos.y);
			path.AddLine((RectB[i % 4].y - _LinePos.y) / tanf(rad) + _LinePos.x - CameraPos.x, RectB[i % 4].y - CameraPos.y, RectB[i % 4].x - CameraPos.x, RectB[i % 4].y - CameraPos.y);
			path.AddLine(RectB[i % 4].x - CameraPos.x, RectB[i % 4].y - CameraPos.y, RectB[(i + 1) % 4].x - CameraPos.x, RectB[(i + 1) % 4].y - CameraPos.y);
		}
		else
		{
			path.AddLine(temp.x - CameraPos.x, temp.y - CameraPos.y, RectB[i % 4].x - CameraPos.x, (RectB[i % 4].x - _LinePos.x) * tanf(rad) + _LinePos.y - CameraPos.y);
			path.AddLine(RectB[i % 4].x - CameraPos.x, (RectB[i % 4].x - _LinePos.x) * tanf(rad) + _LinePos.y - CameraPos.y, RectB[(i + 1) % 4].x - CameraPos.x, RectB[(i + 1) % 4].y - CameraPos.y);
		}
	}
	


	graphics.FillPath(&brush, &path);

}

bool CTextureMgr::LineInRectCheck(Vec2D& _p1, Vec2D& _p2, Vec2D& _LT, Vec2D& _RB)
{
	Vec2D rect[4] = { _LT, Vec2D(_RB.x, _LT.y), _RB, Vec2D(_LT.x, _RB.y)};
	for (int i = 0; i < 4; ++i)
	{
		if (CTexture::LineCrossCheck(_p1, _p2, rect[i], rect[(i + 1) % 4]))
			return true;
	}

	if (_LT << _p1 && _RB >> _p1)
		return true;
	if (_LT << _p2 && _RB >> _p2)
		return true;

	return false;
}


void CALLBACK PreloadFileThread(PTP_CALLBACK_INSTANCE instance, PVOID context, PTP_WORK work)
{
	wstring* line = (wstring*)context;
	wstring _filepath = CEngine::GetInst()->GetResourcePathW() + L"\\animation\\" + *line + L".animation";
	ifstream animation;
	AnimationInfo desc;
	animation.open(_filepath.c_str(), ios::binary);
	if (animation.is_open())
	{
		char NPKDir[255] = {};
		char AlbumPath[255] = {};
		animation.read((char*)&desc, sizeof(desc));
		animation.read(NPKDir, desc.NPKDirLen);
		animation.read(AlbumPath, desc.AlbumPathLen);

		animation.close();


		string strAlbumPath = AlbumPath;
		WCHAR wNPKDir[255] = {};
		MultiByteToWideChar(CP_ACP, 0, NPKDir, -1, wNPKDir, 255);
		wstring wstrNPKDir = wNPKDir;
		wstrNPKDir = CEngine::GetInst()->GetResourcePathW() + wstrNPKDir;

		CTextureMgr::GetInst()->LoadAlbum(strAlbumPath, wstrNPKDir);
	}
	else
	{

		animation.close();
	}
}

void CTextureMgr::PreloadFromFileT(wstring _fileName)
{
	std::wifstream PreloadList(CEngine::GetInst()->GetResourcePathW() + L"\\animation\\Preload\\" + _fileName);
	wstring line;
	
	// 스레드 풀 생성
	PTP_POOL pool = CreateThreadpool(nullptr);
	if (!pool)
		return;

	// 스레드 풀 환경 설정
	TP_CALLBACK_ENVIRON callbackEnv;
	InitializeThreadpoolEnvironment(&callbackEnv);
	SetThreadpoolCallbackPool(&callbackEnv, pool);

	// 각 씬에 대해 작업을 큐에 추가
	std::vector<PTP_WORK> workItems;
	while (std::getline(PreloadList, line)) {
		wstring* data = new wstring(line);
		PTP_WORK work = CreateThreadpoolWork(PreloadFileThread, data, &callbackEnv);
		if (work) {
			workItems.push_back(work);
			SubmitThreadpoolWork(work);
		}
		else {
			delete data;
		}
	}

	// 모든 작업이 완료될 때까지 대기
	for (PTP_WORK work : workItems) {
		WaitForThreadpoolWorkCallbacks(work, FALSE);
		CloseThreadpoolWork(work);
	}

	// 스레드 풀 및 환경 정리
	DestroyThreadpoolEnvironment(&callbackEnv);
	CloseThreadpool(pool);

	PreloadList.close();
}

void CTextureMgr::PreloadFromFile(wstring _fileName)
{
	std::wifstream PreloadList(CEngine::GetInst()->GetResourcePathW() + L"\\animation\\Preload\\" + _fileName);
	wstring line;
	while (std::getline(PreloadList, line))
	{
		wstring _filepath = CEngine::GetInst()->GetResourcePathW() + L"\\animation\\" + line + L".animation";
		ifstream animation;
		AnimationInfo desc;
		animation.open(_filepath.c_str(), ios::binary);
		if (animation.is_open())
		{
			char NPKDir[255] = {};
			char AlbumPath[255] = {};
			animation.read((char*)&desc, sizeof(desc));
			animation.read(NPKDir, desc.NPKDirLen);
			animation.read(AlbumPath, desc.AlbumPathLen);

			animation.close();


			string strAlbumPath = AlbumPath;
			WCHAR wNPKDir[255] = {};
			MultiByteToWideChar(CP_ACP, 0, NPKDir, -1, wNPKDir, 255);
			wstring wstrNPKDir = wNPKDir;
			wstrNPKDir = CEngine::GetInst()->GetResourcePathW() + wstrNPKDir;

			CTextureMgr::GetInst()->LoadAlbum(strAlbumPath, wstrNPKDir);
		}
		else
		{
			animation.close();
		}
	}
	PreloadList.close();
}

void CALLBACK PreloadAvatarThread(PTP_CALLBACK_INSTANCE instance, PVOID context, PTP_WORK work)
{
	pair<string, int>* data = (pair<string, int>*)context;
	int i = data->second;
	string line = data->first;
	AvatarParts _parts = (AvatarParts)i;
	wstring wstrNPKDir = CEngine::GetInst()->GetResourcePathW() + L"\\texture\\player\\sprite_character_archer_equipment_avatar_skin.NPK";
	string strAlbumPath = "sprite/character/archer/equipment/avatar/skin/ac_body0000.img";

	string parts[6] = { "hair", "cap", "pants", "coat", "shoes", "lbow" };
	wstring wparts[6] = { L"hair", L"cap", L"pants", L"coat", L"shoes", L"lbow" };
	string layer[4] = { "d", "c", "b", "a" };

	// 아바타가 속한 NPK 파일명
	int pos0 = wstrNPKDir.find(L"skin");
	if (pos0 != wstring::npos)
		wstrNPKDir.replace(pos0, 4, wparts[(int)_parts]);
	if (_parts == AvatarParts::LBow)
	{
		int pos = wstrNPKDir.find(L"avatar");
		if (pos != wstring::npos)
			wstrNPKDir.replace(pos, 6, L"weapon");
	}

	// Coat 레이어에만 존재하는 x 레이어는 따로 처리
	if (_parts == AvatarParts::Coat)
	{
		string temp = strAlbumPath;
		int pos1 = temp.find("skin");
		if (pos1 != string::npos)
			temp.replace(pos1, 4, parts[(int)_parts]);
		int pos2 = temp.find("body");
		if (pos2 != string::npos)
			temp.replace(pos2, 8, parts[(int)_parts] + line + "x");

		CTextureMgr::GetInst()->LoadAlbum(temp, wstrNPKDir);
	}

	// 아바타가 속한 앨범명 계산 후 배열에 저장
	for (int layerIdx = 0; layerIdx < 4; ++layerIdx)
	{
		string temp = strAlbumPath;
		int pos1 = temp.find("skin");
		if (pos1 != string::npos)
			temp.replace(pos1, 4, parts[(int)_parts]);
		if (_parts == AvatarParts::LBow)
		{
			int pos = temp.find("avatar");
			if (pos != string::npos)
				temp.replace(pos, 6, "weapon");
		}
		int pos2 = temp.find("body");
		if (pos2 != string::npos)
			temp.replace(pos2, 8, parts[(int)_parts] + line + layer[layerIdx]);

		CTextureMgr::GetInst()->LoadAlbum(temp, wstrNPKDir);
	}
}

void CTextureMgr::PreloadAvatarT(wstring _fileName)
{
	std::ifstream PreloadList(CEngine::GetInst()->GetResourcePathW() + L"\\animation\\Preload\\" + _fileName);
	string line;
	while (std::getline(PreloadList, line))
	{
		for (int i = 0; i < (int)AvatarParts::END; ++i)
		{
			// 스레드 풀 생성
			PTP_POOL pool = CreateThreadpool(nullptr);
			if (!pool)
				return;

			// 스레드 풀 환경 설정
			TP_CALLBACK_ENVIRON callbackEnv;
			InitializeThreadpoolEnvironment(&callbackEnv);
			SetThreadpoolCallbackPool(&callbackEnv, pool);

			// 각 씬에 대해 작업을 큐에 추가
			std::vector<PTP_WORK> workItems;
			while (std::getline(PreloadList, line)) {
				pair<string, int>* data = new pair<string, int>(line, i);
				PTP_WORK work = CreateThreadpoolWork(PreloadAvatarThread, data, &callbackEnv);
				if (work) {
					workItems.push_back(work);
					SubmitThreadpoolWork(work);
				}
				else {
					delete data;
				}
			}

			// 모든 작업이 완료될 때까지 대기
			for (PTP_WORK work : workItems) {
				WaitForThreadpoolWorkCallbacks(work, FALSE);
				CloseThreadpoolWork(work);
			}

			// 스레드 풀 및 환경 정리
			DestroyThreadpoolEnvironment(&callbackEnv);
			CloseThreadpool(pool);
		}
	}
	PreloadList.close();
}

void CTextureMgr::PreloadAvatar(wstring _fileName)
{
	std::ifstream PreloadList(CEngine::GetInst()->GetResourcePathW() + L"\\animation\\Preload\\" + _fileName);
	string line;
	while (std::getline(PreloadList, line))
	{
		int idx = -1;
		if (line.find('_') != string::npos)
		{
			idx = std::stoi(line.substr(line.find('_') + 1));
			line = line.substr(0, line.find('_'));
		}
		
		for (int i = 0; i < (int)AvatarParts::END; ++i)
		{
			if (idx != -1 && idx != i)
				continue;

			AvatarParts _parts = (AvatarParts)i;
			wstring wstrNPKDir = CEngine::GetInst()->GetResourcePathW() + L"\\texture\\player\\sprite_character_archer_equipment_avatar_skin.NPK";
			string strAlbumPath = "sprite/character/archer/equipment/avatar/skin/ac_body0000.img";

			string parts[6] = { "hair", "cap", "pants", "coat", "shoes", "lbow" };
			wstring wparts[6] = { L"hair", L"cap", L"pants", L"coat", L"shoes", L"lbow" };
			string layer[4] = { "d", "c", "b", "a" };

			// 아바타가 속한 NPK 파일명
			int pos0 = wstrNPKDir.find(L"skin");
			if (pos0 != wstring::npos)
				wstrNPKDir.replace(pos0, 4, wparts[(int)_parts]);
			if (_parts == AvatarParts::LBow)
			{
				int pos = wstrNPKDir.find(L"avatar");
				if (pos != wstring::npos)
					wstrNPKDir.replace(pos, 6, L"weapon");
			}

			// Coat 레이어에만 존재하는 x 레이어는 따로 처리
			if (_parts == AvatarParts::Coat)
			{
				string temp = strAlbumPath;
				int pos1 = temp.find("skin");
				if (pos1 != string::npos)
					temp.replace(pos1, 4, parts[(int)_parts]);
				int pos2 = temp.find("body");
				if (pos2 != string::npos)
					temp.replace(pos2, 8, parts[(int)_parts] + line + "x");

				CTextureMgr::GetInst()->LoadAlbum(temp, wstrNPKDir);
			}

			// 아바타가 속한 앨범명 계산 후 배열에 저장
			for (int layerIdx = 0; layerIdx < 4; ++layerIdx)
			{
				string temp = strAlbumPath;
				int pos1 = temp.find("skin");
				if (pos1 != string::npos)
					temp.replace(pos1, 4, parts[(int)_parts]);
				if (_parts == AvatarParts::LBow)
				{
					int pos = temp.find("avatar");
					if (pos != string::npos)
						temp.replace(pos, 6, "weapon");
				}
				int pos2 = temp.find("body");
				if (pos2 != string::npos)
					temp.replace(pos2, 8, parts[(int)_parts] + line + layer[layerIdx]);

				CTextureMgr::GetInst()->LoadAlbum(temp, wstrNPKDir);
			}
		}
	}
	PreloadList.close();
}