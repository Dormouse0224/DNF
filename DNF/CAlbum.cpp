#include "pch.h"
#include "CAlbum.h"
#include "CTexture.h"
#include "CTimeMgr.h"

CAlbum::CAlbum(wstring _NpkPath)
	: CResource(_NpkPath, ResourceType::Animation)
{
}

CAlbum::~CAlbum()
{
	Delete_Vector(m_SceneVector);
	Delete_Vector(AtlasVec);
}

int CAlbum::Load()
{
	// 로딩할 텍스쳐 리소스 경로 설정
	//wstring path = CBase::GetName() + L"\\";

	//for (int i = 0; i < m_SceneCount; ++i)
	//{
	//	//wstring filepath = path + std::to_wstring(i) + L".png";
	//	//CTexture* ptexture = new CTexture(filepath, (CObj*)this);
	//	//ptexture->Load();
	//	//m_SceneVector.push_back(ptexture);
	//}

	return S_OK;
}


