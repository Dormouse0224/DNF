#include "pch.h"
#include "CTexture.h"
#include "CEngine.h"
#include "CObj.h"
#include "CTextureMgr.h"
#include "CCameraMgr.h"


CTexture::CTexture(wstring _RelativPath, CAlbum* _Owner)
	: CResource(_RelativPath, ResourceType::Texture)
	, m_Owner(_Owner)
	, m_Bitmap(nullptr)
	, m_DC(nullptr)
	, m_Size(Vec2D(0.f, 0.f))
	, m_Offset(Vec2D(0.f, 0.f))
	, Index(0)
	, Type(ColorBits::UNKNOWN)
	, CompressMode(CompressMode::NONE)
	, Length(0)
	, m_CanvasSize(Vec2D(0.f, 0.f))
	, Target(nullptr)
	, Data(nullptr)
	, Version(TextureVersion::Dxt1)
	, FullLength(0)
{
}

CTexture::~CTexture()
{
	if (m_Bitmap != nullptr)
	{
		delete m_Bitmap;
		m_Bitmap = nullptr;
	}
	if (Data != nullptr)
	{
		delete[] Data;
		Data = nullptr;
	}
}

int CTexture::Load()
{
	m_Bitmap = CTextureMgr::GetInst()->ConvertToBitmap(this);
	Graphics graphics(m_Bitmap);
	m_DC = graphics.GetHDC();
	return S_OK;
}

void CTexture::Render(Vec2D _PlayerOffset)
{
	// �⺻ �ػ� �������� �ؽ�ó�� ���� ��ġ�� ���
	Vec2D FinalPos = Vec2D(m_Owner->GetOwner()->GetLocation().x + m_Offset.x + _PlayerOffset.x
		, m_Owner->GetOwner()->GetLocation().y + m_Offset.y + _PlayerOffset.y);
	// �⺻ �ػ󵵸� �������� ī�޶��� ���� �ȿ� �ؽ�ó�� ���ԵǾ� �ִ��� Ȯ��
	if (abs((CCameraMgr::GetInst()->GetCameraPos().x + 1066 / 2) - (FinalPos.x + m_Size.x / 2)) < ((CEngine::GetInst()->GetResolution().x + m_Size.x) / 2)
		&& abs((CCameraMgr::GetInst()->GetCameraPos().y + 600 / 2) - (FinalPos.y + m_Size.y / 2)) < ((CEngine::GetInst()->GetResolution().y + m_Size.y) / 2))
	{
		// �ε�� �̹����� �ػ� ������ ���� ������
		m_DC = CEngine::GetInst()->GetSubDC();
		Graphics graphics(m_DC);
		Status st = graphics.DrawImage(m_Bitmap
			, (int)((FinalPos.x - CCameraMgr::GetInst()->GetCameraPos().x) * CEngine::GetInst()->GetScreenScale())
			, (int)((FinalPos.y - CCameraMgr::GetInst()->GetCameraPos().y) * CEngine::GetInst()->GetScreenScale())
			, (int)(m_Size.x * CEngine::GetInst()->GetScreenScale())
			, (int)(m_Size.y * CEngine::GetInst()->GetScreenScale()));
	}


}