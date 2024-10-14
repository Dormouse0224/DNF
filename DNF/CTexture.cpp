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

// 오브젝트에 속한 텍스처의 렌더링
// 오브젝트 로케이션 + 텍스쳐 자체 오프셋 + 추가 렌더링 오프셋(기본값 0)
void CTexture::Render(Vec2D _RenderOffset, bool bCameraDependent)
{
	// 기본 해상도 기준으로 텍스처의 최종 위치를 계산
	Vec2D FinalPos = Vec2D(m_Owner->GetOwner()->GetLocation().x + m_Offset.x + _RenderOffset.x
		, m_Owner->GetOwner()->GetLocation().y + m_Offset.y + _RenderOffset.y);
	Vec2D CameraPos = CCameraMgr::GetInst()->GetCameraPos();
	// 기본 해상도를 기준으로 카메라의 범위 안에 텍스처가 포함되어 있는지 확인
	if (((abs((CameraPos.x + 1066 / 2) - (FinalPos.x + m_Size.x / 2)) < ((CEngine::GetInst()->GetResolution().x + m_Size.x) / 2)
		&& abs((CameraPos.y + 600 / 2) - (FinalPos.y + m_Size.y / 2)) < ((CEngine::GetInst()->GetResolution().y + m_Size.y) / 2))) || !bCameraDependent)
	{
		if (!bCameraDependent)
		{
			CameraPos = Vec2D(0, 0);
		}
		// 로드된 이미지를 해상도 비율에 맞춰 렌더링
		m_DC = CEngine::GetInst()->GetSubDC();
		Graphics graphics(m_DC);
		Status st = graphics.DrawImage(m_Bitmap
			, (int)((FinalPos.x - CameraPos.x) * CEngine::GetInst()->GetScreenScale())
			, (int)((FinalPos.y - CameraPos.y) * CEngine::GetInst()->GetScreenScale())
			, (int)(m_Size.x * CEngine::GetInst()->GetScreenScale())
			, (int)(m_Size.y * CEngine::GetInst()->GetScreenScale()));
	}
}


// 오브젝트에 속하지 않은 텍스처의 렌더링
// 텍스쳐 자체 오프셋 + 추가 렌더링 오프셋(기본값 0)
void CTexture::DirectRender(Vec2D _RenderOffset, bool bCameraDependent)
{
	// 기본 해상도 기준으로 텍스처의 최종 위치를 계산
	Vec2D FinalPos = Vec2D(m_Offset.x + _RenderOffset.x, m_Offset.y + _RenderOffset.y);
	Vec2D CameraPos = CCameraMgr::GetInst()->GetCameraPos();
	// 기본 해상도를 기준으로 카메라의 범위 안에 텍스처가 포함되어 있는지 확인
	if (((abs((CameraPos.x + 1066 / 2) - (FinalPos.x + m_Size.x / 2)) < ((CEngine::GetInst()->GetResolution().x + m_Size.x) / 2)
		&& abs((CameraPos.y + 600 / 2) - (FinalPos.y + m_Size.y / 2)) < ((CEngine::GetInst()->GetResolution().y + m_Size.y) / 2))) || !bCameraDependent)
	{
		if (!bCameraDependent)
		{
			CameraPos = Vec2D(0, 0);
		}
		// 로드된 이미지를 해상도 비율에 맞춰 렌더링
		m_DC = CEngine::GetInst()->GetSubDC();
		Graphics graphics(m_DC);
		Status st = graphics.DrawImage(m_Bitmap
			, (int)((FinalPos.x - CameraPos.x) * CEngine::GetInst()->GetScreenScale())
			, (int)((FinalPos.y - CameraPos.y) * CEngine::GetInst()->GetScreenScale())
			, (int)(m_Size.x * CEngine::GetInst()->GetScreenScale())
			, (int)(m_Size.y * CEngine::GetInst()->GetScreenScale()));
	}
}