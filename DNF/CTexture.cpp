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

// ������Ʈ�� ���� �ؽ�ó�� ������
// ������Ʈ �����̼� + �ؽ��� ��ü ������ + �߰� ������ ������(�⺻�� 0)
void CTexture::Render(Vec2D _RenderOffset, float _angle, bool bCameraFallow, bool bLinearDodge)
{
	// �⺻ �ػ� �������� �ؽ�ó�� ���� ��ġ�� ���
	Vec2D FinalPos(m_Owner->GetOwner()->GetLocation() + m_Offset + _RenderOffset);
	Vec2D CameraPos = CCameraMgr::GetInst()->GetCameraPos();
	Vec2D ObjCenter = m_Owner->GetOwner()->GetLocation() + (m_Owner->GetOwner()->GetScale() / 2) - CameraPos;		// ������Ʈ �߽��� ���
	// �⺻ �ػ󵵸� �������� ī�޶��� ���� �ȿ� �ؽ�ó�� ���ԵǾ� �ִ��� Ȯ��
	if (((abs((CameraPos.x + 1066 / 2) - (FinalPos.x + m_Size.x / 2)) < ((CEngine::GetInst()->GetResolution().x + m_Size.x) / 2)
		&& abs((CameraPos.y + 600 / 2) - (FinalPos.y + m_Size.y / 2)) < ((CEngine::GetInst()->GetResolution().y + m_Size.y) / 2))) || bCameraFallow)
	{
		if (bCameraFallow)
		{
			CameraPos = Vec2D(0, 0);
		}
		// �ε�� �̹����� �ػ� ������ ���� ������
		m_DC = CEngine::GetInst()->GetSubDC();
		if (bLinearDodge)
		{
		}
		else
		{
			Graphics graphics(m_DC);
			graphics.TranslateTransform(ObjCenter.x, ObjCenter.y);		// �Ҽ� ������Ʈ �߽������� ȸ���߽� �̵�
			graphics.RotateTransform(_angle);							// ȸ�� ����
			graphics.TranslateTransform(-ObjCenter.x, -ObjCenter.y);	// ���� ��ġ�� �̵�
			graphics.DrawImage(m_Bitmap, (int)(FinalPos.x - CameraPos.x), (int)(FinalPos.y - CameraPos.y), (int)m_Size.x, (int)m_Size.y);
		}
	}
}


// ������Ʈ�� ������ ���� �ؽ�ó�� ������
// �ؽ��� ��ü ������ + �߰� ������ ������(�⺻�� 0)
void CTexture::DirectRender(Vec2D _RenderOffset, float _angle, bool bCameraFallow, bool bLinearDodge)
{
	// �⺻ �ػ� �������� �ؽ�ó�� ���� ��ġ�� ���
	Vec2D Resolution = CEngine::GetInst()->GetResolution();
	Vec2D FinalPos(m_Offset + _RenderOffset);
	Vec2D CameraPos = CCameraMgr::GetInst()->GetCameraPos();
	Vec2D AbsCenter = CameraPos * (-1);		// ��ǥ�� ���� ���
	// �⺻ �ػ󵵸� �������� ī�޶��� ���� �ȿ� �ؽ�ó�� ���ԵǾ� �ִ��� Ȯ��
	bool tf = CameraIntersectCheck(CameraPos, Resolution, 0.f, FinalPos, m_Size, _angle);
	if (CameraIntersectCheck(CameraPos, Resolution, 0.f, FinalPos, m_Size, _angle) || bCameraFallow)
	{
		if (bCameraFallow)
		{
			CameraPos = Vec2D(0, 0);
		}
		// �ε�� �̹����� �ػ� ������ ���� ������
		m_DC = CEngine::GetInst()->GetSubDC();
		Graphics graphics(m_DC);
		graphics.TranslateTransform(AbsCenter.x, AbsCenter.y);		// ��ǥ�� �������� ȸ���߽� �̵�
		graphics.RotateTransform(_angle);							// ȸ�� ����
		graphics.TranslateTransform(-AbsCenter.x, -AbsCenter.y);	// ���� ��ġ�� �̵�
		Status st = graphics.DrawImage(m_Bitmap, (int)(FinalPos.x - CameraPos.x), (int)(FinalPos.y - CameraPos.y), (int)m_Size.x, (int)m_Size.y);
	}
}

bool CTexture::CameraIntersectCheck(Vec2D& _locationA, Vec2D& _SizeA, float _AngleA, Vec2D& _locationB, Vec2D& _SizeB, float _AngleB)
{
	Vec2D RectA[4] =
	{
		_locationA.Clockwise(_AngleA),
		(_locationA + Vec2D(_SizeA.x, 0.f)).Clockwise(_AngleA),
		(_locationA + _SizeA).Clockwise(_AngleA),
		(_locationA + Vec2D(0.f, _SizeA.y)).Clockwise(_AngleA)
	};
	Vec2D RectB[4] =
	{
		_locationB.Clockwise(_AngleB),
		(_locationB + Vec2D(_SizeB.x, 0.f)).Clockwise(_AngleB),
		(_locationB + _SizeB).Clockwise(_AngleB),
		(_locationB + Vec2D(0.f, _SizeB.y)).Clockwise(_AngleB)
	};

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (LineCrossCheck(RectA[i], RectA[(i + 1) % 4], RectB[j], RectB[(j + 1) % 4]))
				return true;
		}
	}

	if (min(RectB[0].x, RectB[2].x) <= RectA[0].x && max(RectB[0].x, RectB[2].x) >= RectA[0].x && min(RectB[0].y, RectB[2].y) <= RectA[0].y && max(RectB[0].y, RectB[2].y) >= RectA[0].y)
		return true;
	if (min(RectA[0].x, RectA[2].x) <= RectB[0].x && max(RectA[0].x, RectA[2].x) >= RectB[0].x && min(RectA[0].y, RectA[2].y) <= RectB[0].y && max(RectA[0].y, RectA[2].y) >= RectB[0].y)
		return true;

	return false;
}

bool CTexture::LineCrossCheck(Vec2D& _p1, Vec2D& _p2, Vec2D& _q1, Vec2D& _q2)
{
	int dir1 = PointDirectionCheck(_p1, _p2, _q1);
	int dir2 = PointDirectionCheck(_p1, _p2, _q2);
	int dir3 = PointDirectionCheck(_q1, _q2, _p1);
	int dir4 = PointDirectionCheck(_q1, _q2, _p2);

	// �� ������ X �Ǵ� T ���·� ������
	if (dir1 != dir2 && dir3 != dir4)
		return true;

	// ������ ��ħ
	if (dir1 == 0 && LineOverlapCheck(_p1, _p2, _q1, _q2))
		return true;

	return false;
}

int CTexture::PointDirectionCheck(Vec2D& _p, Vec2D& _q, Vec2D& _r)
{
	Vec2D pq= (_q - _p);
	Vec2D qr= (_r - _q);
	int result = pq.Cross(qr);
	if (result == 0)
		return 0;
	else if (result < 0)
		return 1;
	else
		return 2;
}

bool CTexture::LineOverlapCheck(Vec2D& _p1, Vec2D& _p2, Vec2D& _q1, Vec2D& _q2)
{
	if (min(_p1.x, _p2.x) <= _q1.x && max(_p1.x, _p2.x) >= _q1.x && min(_p1.y, _p2.y) <= _q1.y && max(_p1.y, _p2.y) >= _q1.y)
		return true;
	if (min(_p1.x, _p2.x) <= _q2.x && max(_p1.x, _p2.x) >= _q2.x && min(_p1.y, _p2.y) <= _q2.y && max(_p1.y, _p2.y) >= _q2.y)
		return true;
	return false;
}
