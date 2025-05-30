#pragma once
#include "CAlbum.h"

class CTextureMgr
{
	SINGLETON(CTextureMgr);

private:
	map<string, CAlbum*> m_Albums;
	map<wstring, vector<CAlbum*>> m_NPKs;
	CAlbum* m_TempAlbum;			// 기본 임시 앨범
	CAlbum* m_SysReservedAlbum;		// 프로그램에서 사용되는 텍스처를 저장해두는 앨범

	ID3D11Device* m_pd3dDevice;
	ID3D11DeviceContext* m_pd3dContext;

public:
	void Init();

	CAlbum* GetTempAlbum() { return m_TempAlbum; }
	void ClearTempAlbum();

	void CreateFromNpk(ifstream& _file, CAlbum* _Album);
	Bitmap* ConvertToBitmap(CTexture* _pTexture);
	Bytef* UncompressZlib(const char* _data, int size, int _dataSize);
	Bitmap* BitmapFromArray(const char* data, Vec2D size, ColorBits bits);
	void BitmapToArray(CTexture* _Texture);
	byte* ReadColor(const char* _data, ColorBits _bits);
	void WriteColor(char* _dest, const char* _src, ColorBits _bits);
	vector<CAlbum*> LoadNPK(wstring _NpkPath);
	CAlbum* LoadAlbum(string _AlbumPath, wstring _NpkPath);
	void LoadAll(wstring _NpkPath);
	void SaveAlbum(string _AlbumName, string _Directory);
	CTexture* CreateRectTexture(wstring _Name, Vec2D _size, Vec2D _offset, Color _color, bool _IsAddedBySys);
	CTexture* LoadFromFile(wstring _filepath);
	CAlbum* GetAlbum(string _AlbumPath);

	void DrawLine(Color _color, int _width, Vec2D _begin, Vec2D _end, bool bCameraFallow = false);
	void DrawLine(Color _color, int _width, int _Len, Vec2D _LinePos, float _LineAngle, bool bCameraFallow = false);
	void DrawRect(Color _color, int _width, Vec2D _LeftTop, Vec2D _size, bool bCameraFallow = false);
	void FillRect(Color _color, Vec2D _LeftTop, Vec2D _size, bool bCameraFallow = false);
	void DrawEllipse(Color _color, int _width, Vec2D _LeftTop, Vec2D _size, bool bCameraFallow = false);
	void FillEllipse(Color _color, Vec2D _LeftTop, Vec2D _size, bool bCameraFallow = false);
	void FillDonut(Color _color, Vec2D _OuterLeftTop, Vec2D _OuterSize, Vec2D _InnerLeftTop, Vec2D _InnerSize, bool bCameraFallow = false);
	void FillDividedLine(Color _color, Vec2D _LinePos, float _LineAngle, bool bCameraFallow = false);


	static void PreloadFromFile(wstring _filepath);
	static void PreloadFromFileT(wstring _fileName);
	static void PreloadAvatarT(wstring _fileName);
	static void PreloadAvatar(wstring _fileName);

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

	bool LineInRectCheck(Vec2D& _p1, Vec2D& _p2, Vec2D& _q1, Vec2D& _q2);
};

