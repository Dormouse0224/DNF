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

public:
	virtual void Begin() override;       // 처음 시작할 때 할 일
	virtual void Tick() override;    // 반복적으로 할 일
	virtual void Render() override;      // 오브젝트를 그리기

};

