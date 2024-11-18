#include "pch.h"
#include "CNpkMgr.h"
#include "CTextureMgr.h"
#include "CAlbum.h"
#include "CTexture.h"

CNpkMgr::CNpkMgr()
{
	m_encryptionKey.append("puchikon@neople dungeon and fighter ");
	while (m_encryptionKey.size() < 256)
	{
		switch (m_encryptionKey.size() % 3)
		{
		case 0:
			m_encryptionKey.append("D");
			break;
		case 1:
			m_encryptionKey.append("N");
			break;
		case 2:
			m_encryptionKey.append("F");
			break;
		}
	}
}

CNpkMgr::~CNpkMgr()
{

}


vector<CAlbum*> CNpkMgr::ReadNpk(ifstream& _file, wstring _NpkPath)
{
	vector<CAlbum*> AlbumVector;
	_file.seekg(0, ios::end);
	LONGLONG FileLength = _file.tellg();
	_file.seekg(0, ios::beg);
	char buff[255] = {};
	_file.read(buff, 16);
	string str = buff;
	if (str == "NeoplePack_Bill")
	{
		_file.seekg(0, ios::beg);
		AlbumVector = ReadInfo(_file, _NpkPath);
		if (AlbumVector.size() > 0)
			_file.seekg(32, ios::cur);
	}

	for (int i = 0; i < AlbumVector.size(); ++i)
	{
		LONGLONG length = i < AlbumVector.size() - 1 ? (LONGLONG)AlbumVector[i + 1]->Offset : FileLength;
		ReadImg(_file, AlbumVector[i], length);
	}

	return AlbumVector;
}

void CNpkMgr::WriteNpk(ofstream& _file, string _AlbumName)
{
	// __TempAlbum__ 을 _AlbumName 으로 저장
	CAlbum* TempAlbum = CTextureMgr::GetInst()->GetTempAlbum();
	_file.write("NeoplePack_Bill", 16);		// NPK 기본 헤더
	int AlbumCount = 1;						// 앨범 개수는 1개로 고정
	int AlbumOffset = 316;					// 앨범 개수가 하나밖에 없으므로 오프셋 또한 상수
	int ImgVer = 2;
	_file.write((char*)&AlbumCount, sizeof(AlbumCount));

	// 앨범 내 텍스처를 바이너리 데이터로 변경
	LONGLONG IndexLen = 36 * TempAlbum->GetCount();
	int AlbumLength = (int)(32 + IndexLen);
	for (int i = 0; i < TempAlbum->GetCount(); ++i)
	{
		CTextureMgr::GetInst()->BitmapToArray(TempAlbum->GetScene(i));
		AlbumLength += TempAlbum->GetScene(i)->GetLength();
	}

	// Info 작성
	_file.write((char*)&AlbumOffset, sizeof(AlbumOffset));
	_file.write((char*)&AlbumLength, sizeof(AlbumLength));
	WritePath(_file, _AlbumName);
	_file.write("Dummy_Hash_Data_Dummy_Hash_Data", 32);

	// Album 데이터 작성
	_file.write("Neople Img File", 16);
	_file.write((char*)&IndexLen, sizeof(IndexLen));
	_file.write((char*)&ImgVer, sizeof(ImgVer));
	_file.write((char*)&(TempAlbum->Count), sizeof(TempAlbum->Count));

	// 앨범 내 텍스처 정보 작성
	for (int i = 0; i < TempAlbum->GetCount(); ++i)
	{
		CTexture* pTex = TempAlbum->GetScene(i);
		int type = (int)pTex->Type;
		int CompressMode = (int)pTex->CompressMode;
		int size_x = (int)pTex->m_Size.x;
		int size_y = (int)pTex->m_Size.y;
		int length = (int)pTex->Length;
		int offset_x = (int)pTex->m_Offset.x;
		int offset_y = (int)pTex->m_Offset.y;
		int canvassize_x = (int)pTex->m_CanvasSize.x;
		int canvassize_y = (int)pTex->m_CanvasSize.y;
		_file.write((char*)&type, sizeof(type));
		_file.write((char*)&CompressMode, sizeof(CompressMode));
		_file.write((char*)&size_x, sizeof(size_x));
		_file.write((char*)&size_y, sizeof(size_y));
		_file.write((char*)&length, sizeof(length));
		_file.write((char*)&offset_x, sizeof(offset_x));
		_file.write((char*)&offset_y, sizeof(offset_y));
		_file.write((char*)&canvassize_x, sizeof(canvassize_x));
		_file.write((char*)&canvassize_y, sizeof(canvassize_y));
	}
	for (int index = 0; index < TempAlbum->GetSceneCount(); ++index)
	{
		CTexture* pTex = TempAlbum->GetScene(index);
		_file.write(pTex->Data, pTex->Length);
	}
}


void CNpkMgr::ReadImg(ifstream& _file, CAlbum* _Album, LONGLONG _Length)
{
	_file.seekg(_Album->Offset, ios::beg);
	char buffer[16] = {};
	_file.read(buffer, 16);
	string str = buffer;
	if (str == "Neople Img File")
	{
		_Album->IndexLength = Read<LONGLONG>(_file);
		_Album->Version = (ImgVersion)Read<int>(_file);
		_Album->Count = Read<int>(_file);
		CTextureMgr::GetInst()->CreateFromNpk(_file, _Album);
	}
}

vector<CAlbum*> CNpkMgr::ReadInfo(ifstream& _file, wstring _NpkPath)
{
	vector<CAlbum*> AlbumVector;
	char buff[255] = {};
	_file.read(buff, 16);
	string str = buff;
	if (str != "NeoplePack_Bill")
		return AlbumVector;
	int num = Read<int>(_file);
	for (int i = 0; i < num; ++i)
	{
		CAlbum* album = new CAlbum(_NpkPath);
		album->Offset = Read<int>(_file);
		album->Length = Read<int>(_file);
		album->Path = ReadPath(_file);
		album->m_OwnerNpk = _NpkPath;
		AlbumVector.push_back(album);
	}

	return AlbumVector;
}

template<typename T>
T CNpkMgr::Read(ifstream& _file)
{
	char* byte = new char[sizeof(T)];
	_file.read(byte, sizeof(T));
	T data = *(T*)byte;
	delete[] byte;
	return data;
}


string CNpkMgr::ReadPath(ifstream& _file)
{
	char buffer[256] = {};
	int index = 0;
	for (; index < 256; ++index)
	{
		char ch = Read<char>(_file);
		buffer[index] = (char)((UINT)ch ^ (UINT)m_encryptionKey[index]);
		if (buffer[index] == 0)
			break;
	}
	_file.seekg(255 - index, ios::cur);
	return string(buffer);
}

void CNpkMgr::WritePath(ofstream& _file, string _AlbumName)
{
	char buffer1[256] = {};
	memcpy(buffer1, _AlbumName.c_str(), _AlbumName.size());
	char buffer2[256] = {};
	for (int index = 0; index < 256; ++index)
	{
		buffer2[index] = (char)((UINT)buffer1[index] ^ (UINT)m_encryptionKey[index]);
	}
	_file.write(buffer2, 256);
}