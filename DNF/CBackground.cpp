#include "pch.h"
#include "CBackground.h"
#include "CTexture.h"

#include "CAlbumPlayer.h"

CBackground::CBackground(wstring _Name)
	: CObj(_Name)
{
}

CBackground::~CBackground()
{
}

void CBackground::Begin()
{
	/*for (int i = 0; i < m_AlbumPlayerVector.size(); ++i)
	{
		m_AlbumPlayerVector[i]->Load();
	}*/
}

void CBackground::Tick()
{
}

void CBackground::Render()
{
	for (int i = 0; i < m_AlbumPlayerVector.size(); ++i)
	{
		m_AlbumPlayerVector[i]->Render(this);
	}
}

void CBackground::AddComponent(CComponent* _Component)
{
	CObj::AddComponent(_Component);
	if(dynamic_cast<CAlbumPlayer*>(_Component))
		m_AlbumPlayerVector.push_back((CAlbumPlayer*)_Component);
}



