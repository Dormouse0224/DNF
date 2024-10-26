#pragma once

class CSticker;
class CObj;

class CStickerMgr
{
	SINGLETON(CStickerMgr);

private:
	vector<CSticker*> m_StickerVec;
	Vec2D m_PrevCursorPos;
	CSticker* m_Check;

public:
	void Tick();

	bool CheckObjCursorOn(CObj* _object);
};

