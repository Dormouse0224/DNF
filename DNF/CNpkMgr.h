#pragma once

#include "CAlbum.h"

class CNpkMgr
{
	SINGLETON(CNpkMgr);

private:
	string m_encryptionKey;


public:
	vector<CAlbum*> ReadNpk(ifstream& _file, wstring _NpkPath);

	void WriteNpk(ofstream& _file, string _AlbumName);

	template<typename T>
	static T Read(ifstream& _file);

private:
	void ReadImg(ifstream& _file, CAlbum* _Album, LONGLONG _Length);

	vector<CAlbum*> ReadInfo(ifstream& _file, wstring _NpkPath);


	string ReadPath(ifstream& _file);

	void WritePath(ofstream& _file, string _AlbumName);

};

