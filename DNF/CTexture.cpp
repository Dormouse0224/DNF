#include "pch.h"
#include "CTexture.h"
#include "CEngine.h"
#include "CObj.h"
#include "CTextureMgr.h"

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
	//// GDI+ 초기화
	//
	//GdiplusStartup(&GdiplusToken, &GdiplusStartupInput, nullptr);

	//// 로딩할 텍스쳐 리소스 경로 설정
	//GetCurrentPath(path)
	//path = path + L"\\resource\\texture\\" + CBase::GetName();

	//// 이미지 로드
	//m_Image = new Image(path.c_str());

	m_Bitmap = CTextureMgr::GetInst()->ConvertToBitmap(this);


	return S_OK;
}

void CTexture::Render()
{

	// 로드된 이미지를 렌더링
	m_DC = CEngine::GetInst()->GetSubDC();
	Graphics graphics(m_DC);
	Status st = graphics.DrawImage(m_Bitmap
								, (int)(m_Owner->GetOwner()->GetLocation().x + m_Offset.x)
								, (int)(m_Owner->GetOwner()->GetLocation().y + m_Offset.y)
								, (int)(m_Size.x * CEngine::GetInst()->GetScreenScale())
								, (int)(m_Size.y * CEngine::GetInst()->GetScreenScale()));
}