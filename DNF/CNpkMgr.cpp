#include "pch.h"
#include "CNpkMgr.h"
#include "CTextureMgr.h"

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