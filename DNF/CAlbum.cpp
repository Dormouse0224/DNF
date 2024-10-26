#include "pch.h"
#include "CAlbum.h"
#include "CTexture.h"
#include "CTimeMgr.h"

CAlbum::CAlbum(wstring _NpkPath)
	: CResource(_NpkPath, ResourceType::Album)
	, m_Owner(nullptr)
	, m_SceneVector{}
	, Offset(0)
	, Length(0)
	, Path{}
	, IndexLength(0)
	, Version(ImgVersion::Other)
	, Count(0)
	, m_OwnerNpk{}
	, m_PaletteVector{}
	, AtlasVec{}
	, m_Map{}
{
}

CAlbum::~CAlbum()
{
	Delete_Vector(m_SceneVector);
	Delete_Vector(AtlasVec);

}

int CAlbum::Load()
{
	return S_OK;
}

CTexture* CAlbum::GetScene(wstring _name)
{
	if (!m_SceneVector.empty())
	{
		for (vector<CTexture*>::iterator iter = m_SceneVector.begin(); iter != m_SceneVector.end(); ++iter)
		{
			if ((*iter)->GetName() == _name)
			{
				return *iter;
			}
		}
	}
	return nullptr;
}


