#include "pch.h"
#include "CTextureMgr.h"
#include "CTexture.h"
#include "CAlbum.h"
#include "CNpkMgr.h"

#include "DDSTextureLoader11.h"
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

CTextureMgr::CTextureMgr()
	: m_TempAlbum(nullptr)
	, m_SysReservedAlbum(nullptr)
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
				_Album->m_Map.insert(make_pair(pTexture->Index, textureInfo));
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
}

Bitmap* CTextureMgr::ConvertToBitmapV2(CTexture* _pTexture)
{
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
	}
	Bitmap* pBitmap = BitmapFromArray(data, _pTexture->m_Size, type);
	return pBitmap;
}

Bitmap* CTextureMgr::ConvertToBitmapV4(CTexture* _pTexture)
{
	char* data = _pTexture->Data;
	int size = (int)(_pTexture->m_Size.x * _pTexture->m_Size.y);
	char* data2 = new char[size * 4];
	if (_pTexture->Type == ColorBits::ARGB_1555 && _pTexture->CompressMode == CompressMode::ZLIB)
	{
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
			return BitmapFromArray(data2, _pTexture->m_Size, ColorBits::ARGB_8888);
		}
	}
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
	int size = _pTexture->m_Size.x * _pTexture->m_Size.y;
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
	char* temp = new char[_size.x * _size.y * 4];
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

	// 픽셀 데이터 배열의 메모리 해제
	delete[] _data;
	_data = nullptr;

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

CAlbum* CTextureMgr::LoadAlbum(string _AlbumPath, wstring _NpkPath)
{
	// 앨범 데이터 가져왔는지 검사
	map<string, CAlbum*>::iterator iter = m_Albums.find(_AlbumPath);
	if (_NpkPath != L"__TempNPK__" && _NpkPath != L"__SysReservedNPK__")
	{
		// 앨범을 가져온 적 없는 경우 - 앨범이 속한 NPK 파일 로드 후 앨범 데이터 저장
		if (iter == m_Albums.end())
		{
			ifstream readfile;
			readfile.open(_NpkPath, ios::binary);
			assert(readfile.is_open());	// 파일 읽기 오류
			// NPK 파일 읽어서 앨범 저장
			vector<CAlbum*> AlbumList = CNpkMgr::GetInst()->ReadNpk(readfile, _NpkPath);
			readfile.close();
			for (CAlbum* _album : AlbumList)
			{
				m_Albums.insert(make_pair(_album->GetPath(), _album));
			}
			// 재검색
			iter = m_Albums.find(_AlbumPath);
		}
		// 메모리에 앨범 이미지 로드
		for (int i = 0; i < iter->second->GetSceneCount(); ++i)
		{
			iter->second->GetScene(i)->Load();
		}
	}
	return iter->second;
}

void CTextureMgr::SaveAlbum(string _AlbumName, string _Directory)
{
	ofstream writeAlbum;
	writeAlbum.open(_Directory + "\\" + _AlbumName + ".NPK", ios::binary);
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

Bitmap* CTextureMgr::ReadDDSFromArray(const char* _DDSdata, int _DDSdataSize)
{
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


	ID3D11Device* pd3dDevice = nullptr;
	ID3D11DeviceContext* pd3dContext = nullptr;
	ID3D11Texture2D* texture = nullptr;
	ID3D11ShaderResourceView* textureView = nullptr;
	
	// 디바이스 초기화
	err = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, (D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_SINGLETHREADED)
		, nullptr, 0, D3D11_SDK_VERSION, &pd3dDevice, nullptr, &pd3dContext);
	assert(err == S_OK);
	
	// DDS 메모리로부터 텍스처 로드
	err = DirectX::CreateDDSTextureFromMemory(pd3dDevice, (const uint8_t*)_DDSdata, (size_t)_DDSdataSize, (ID3D11Resource**)&texture, &textureView);
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
	pd3dDevice->CreateTexture2D(&cpuTextureDesc, nullptr, &pStagingTexture);

	// 원본 텍스처를 스테이징 텍스처로 복사
	if (pStagingTexture == nullptr)
		return nullptr;
	pd3dContext->CopyResource(pStagingTexture, texture);

	// 텍스처 맵핑
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	err = pd3dContext->Map(pStagingTexture, 0, D3D11_MAP_READ, 0, &mappedResource);
	assert(err == S_OK);

	// DXT 포맷으로 압축된 텍스처를 압축해제
	DirectX::Image BCtex;
	BCtex.width = textureDesc.Width;
	BCtex.height = textureDesc.Height;
	BCtex.format = textureDesc.Format;
	BCtex.rowPitch = mappedResource.RowPitch;
	BCtex.slicePitch = mappedResource.DepthPitch;
	BCtex.pixels = (uint8_t*)mappedResource.pData;

	DirectX::ScratchImage DecompressedScratchImage;

	err = DirectX::Decompress(BCtex, DXGI_FORMAT_R8G8B8A8_UNORM, DecompressedScratchImage);

	const DirectX::Image* DecompImg;
	DecompImg = DecompressedScratchImage.GetImage(0, 0, 0);

	// 텍스처 데이터를 GDI+ Bitmap으로 변환
	UINT width = DecompImg->width;
	UINT height = DecompImg->height;
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
	pd3dDevice->Release();
	pd3dContext->Release();
	texture->Release();
	textureView->Release();
	pStagingTexture->Release();
	DecompressedScratchImage.Release();

	// 할당된 메모리 해제
	delete[] textureData;

	return pBitmap;
}

HDC CTextureMgr::CreateRectTexture(wstring _Name, Vec2D _size, Vec2D _offset, Color _color, bool _IsAddedBySys)
{
	// 같은 이름의 텍스쳐가 시스템 예약 앨범 또는 임시 앨범에 없어야함
	assert(m_SysReservedAlbum->GetScene(_Name) == nullptr);
	assert(m_TempAlbum->GetScene(_Name) == nullptr);

	CTexture* pTex = new CTexture(_Name, m_SysReservedAlbum);
	pTex->m_Size = _size;
	pTex->m_Offset = _offset;
	pTex->m_Bitmap = new Bitmap(_size.x + _offset.x, _size.y + _offset.y, PixelFormat32bppARGB);

	Graphics graphics(pTex->m_Bitmap);
	graphics.Clear(Color(0, 0, 0, 0));
	Pen pen(_color);
	graphics.DrawRectangle(&pen, _offset.x, _offset.y, _size.x, _size.y);

	pTex->m_DC = graphics.GetHDC();

	if (_IsAddedBySys)
	{
		m_SysReservedAlbum->AddScene(pTex);
	}
	else
	{
		m_TempAlbum->AddScene(pTex);
	}

	return pTex->m_DC;
}

void CTextureMgr::LoadFromFile(wstring _filepath)
{
	wstring filename = _filepath;
	filename = filename.substr(filename.rfind('\\') + 1, filename.rfind('.') - filename.rfind('\\'));
	CTexture* newTex = new CTexture(filename, m_TempAlbum);
	m_TempAlbum->AddScene(newTex);
	++m_TempAlbum->Count;

	newTex->m_Bitmap = Bitmap::FromFile(_filepath.c_str());
	newTex->m_Size = Vec2D(newTex->m_Bitmap->GetWidth(), newTex->m_Bitmap->GetHeight());
	newTex->m_Offset = Vec2D(0, 0);
	Graphics graphics(newTex->m_Bitmap);
	newTex->m_DC = graphics.GetHDC();
}
