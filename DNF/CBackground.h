#pragma once
#include "CObj.h"

class CTexture;
class CAlbumPlayer;

class CBackground
	: public CObj
{
public:
	CBackground(wstring _Name);
	~CBackground();

private:
	vector<CAlbumPlayer*> m_AlbumPlayerVector;

public:
	virtual void Begin() override;       // ó�� ������ �� �� ��
	virtual void Tick() override;    // �ݺ������� �� ��
	virtual void Render() override;      // ������Ʈ�� �׸���

	virtual void AddComponent(CComponent* _Component) override;

};

