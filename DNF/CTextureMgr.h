#pragma once
#include "CAlbum.h"

class CTextureMgr
{
	SINGLETON(CTextureMgr);

private:
	map<string, CAlbum*> m_Albums;

public:
	void CreateFromNpk(ifstream& _file, CAlbum* _Album);
	Bitmap* ConvertToBitmap(CTexture* _pTexture);
	Bytef* UncompressZlib(const char* _data, int size, int _dataSize);
	Bitmap* BitmapFromArray(const char* data, Vec2D size, ColorBits bits);
	byte* ReadColor(const char* _data, ColorBits _bits);
	void WriteColor(char* _dest, const char* _src, ColorBits _bits);
	CAlbum* LoadAlbum(string _AlbumPath, wstring _NpkPath);

private:

	Bitmap* ConvertToBitmapV2(CTexture* _pTexture);
	Bitmap* ConvertToBitmapV4(CTexture* _pTexture);
	Bitmap* ConvertToBitmapV5(CTexture* _pTexture);
	Bitmap* ConvertToBitmapV6(CTexture* _pTexture);

	void CreateFromNpkV2(ifstream& _file, CAlbum* _Album);
	void CreateFromNpkV4(ifstream& _file, CAlbum* _Album);
	void CreateFromNpkV5(ifstream& _file, CAlbum* _Album);
	void CreateFromNpkV6(ifstream& _file, CAlbum* _Album);

	vector<Color> ReadPalette(ifstream& _file, int count);
	Bitmap* ReadDDSFromArray(const char* _DDSdata, int _DDSdataSize);
};

