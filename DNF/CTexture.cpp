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
	if (Type == ColorBits::LINK)
		return;

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
	if (!m_Bitmap)
		m_Bitmap = CTextureMgr::GetInst()->ConvertToBitmap(this);
	return S_OK;
}

// ������Ʈ�� ���� �ؽ�ó�� ������
// ������Ʈ �����̼� + �ؽ��� ��ü ������ + �߰� ������ ������(�⺻�� 0)
void CTexture::Render(Vec2D _RenderOffset, float _angle, bool bCameraFallow, bool bLinearDodge, bool bFlipHorizontal, float _renderPercent)
{
	// �⺻ �ػ� �������� �ؽ�ó�� ���� ��ġ�� ���
	Vec2D Resolution = CEngine::GetInst()->GetResolution();
	Vec2D FinalPos(m_Owner->GetOwner()->GetLocation() + m_Offset + _RenderOffset);
	Vec2D CameraPos = CCameraMgr::GetInst()->GetCameraPos();
	if (bCameraFallow)
	{
		CameraPos = Vec2D(0, 0);
	}
	Vec2D ObjCenter = m_Owner->GetOwner()->GetLocation() + (m_Owner->GetOwner()->GetScale() / 2) - CameraPos;		// ������Ʈ �߽��� ���
	// �⺻ �ػ󵵸� �������� ī�޶��� ���� �ȿ� �ؽ�ó�� ���ԵǾ� �ִ��� Ȯ��
	if (CameraIntersectCheck(CameraPos, Resolution, 0.f, FinalPos, m_Size, _angle, Vec2D(0, 0), ObjCenter + CameraPos) || bCameraFallow)
	{
		// �ε�� �̹����� �ػ� ������ ���� ������
		if (bLinearDodge)
		{
			// ���� ��Ʈ�� �̹����� ȸ�� �� ������Ų �� ��Ʈ�� ����
			int newWidth = (int)(abs(m_Size.x * cos(_angle / 180 * PI)) + abs(m_Size.y * sin(_angle / 180 * PI)));
			int newHeight = (int)(abs(m_Size.x * sin(_angle / 180 * PI)) + abs(m_Size.y * cos(_angle / 180 * PI)));
			Bitmap RotatedBitmap(newWidth, newHeight);
			Graphics graphics(&RotatedBitmap);
			graphics.SetInterpolationMode(InterpolationModeLowQuality);
			graphics.TranslateTransform(newWidth / 2.0f, newHeight / 2.0f);
			graphics.ScaleTransform(bFlipHorizontal ? -1.0f : 1.0f, 1.0f);
			graphics.RotateTransform(_angle);
			graphics.TranslateTransform(-m_Size.x / 2.0f, -m_Size.y / 2.0f);
			graphics.DrawImage(m_Bitmap, Point(0, 0));

			// �� ��Ʈ���� ���� ���� ��ǥ ���
			Vec2D rotatedCenterDiff = ((FinalPos + m_Size / 2) - (ObjCenter + CameraPos)).Clockwise(_angle);
			Vec2D RFinalPosLT = ObjCenter + Vec2D(bFlipHorizontal ? -rotatedCenterDiff.x : rotatedCenterDiff.x, rotatedCenterDiff.y) - Vec2D(newWidth / 2, newHeight / 2);
			Vec2D RFinalPosRB = RFinalPosLT + Vec2D(newWidth, newHeight);

			Bitmap* backbuffer = CEngine::GetInst()->GetBackbuffer()->GetBitmap();
			BitmapData BufferBitmapdata;
			BitmapData RotatedBitmapdata;
			Rect BufferRect(max(RFinalPosLT.x, 0)
				, max(RFinalPosLT.y, 0)
				, min(RFinalPosRB.x, CEngine::GetInst()->GetResolution().x) - max(RFinalPosLT.x, 0)
				, min(RFinalPosRB.y, CEngine::GetInst()->GetResolution().y) - max(RFinalPosLT.y, 0));
			Rect RotatedRect(max(-RFinalPosLT.x, 0)
				, max(-RFinalPosLT.y, 0)
				, min(RFinalPosRB.x, CEngine::GetInst()->GetResolution().x) - max(RFinalPosLT.x, 0)
				, min(RFinalPosRB.y, CEngine::GetInst()->GetResolution().y) - max(RFinalPosLT.y, 0));
			backbuffer->LockBits(&BufferRect, ImageLockModeWrite, PixelFormat32bppARGB, &BufferBitmapdata);
			RotatedBitmap.LockBits(&RotatedRect, ImageLockModeRead, PixelFormat32bppARGB, &RotatedBitmapdata);
			LinearDodge(&BufferBitmapdata, &RotatedBitmapdata
				, min(RFinalPosRB.x, CEngine::GetInst()->GetResolution().x) - max(RFinalPosLT.x, 0)
				, min(RFinalPosRB.y, CEngine::GetInst()->GetResolution().y) - max(RFinalPosLT.y, 0));
			RotatedBitmap.UnlockBits(&RotatedBitmapdata);
			backbuffer->UnlockBits(&BufferBitmapdata);
		}
		else
		{
			Graphics graphics(CEngine::GetInst()->GetBackbuffer()->GetBitmap());
			TextureBrush textureBrush(m_Bitmap);

			// ������ �� ���� �׷��Ƚ� ������ ����
			graphics.SetCompositingMode(CompositingModeSourceOver);			// ���ĺ��� ����
			graphics.SetCompositingQuality(CompositingQualityHighSpeed);	// ���� ���� ������
			graphics.SetPixelOffsetMode(PixelOffsetModeNone);				// �ȼ� ��ġ ���� ������
			graphics.SetSmoothingMode(SmoothingModeNone);					// ��Ƽ�ٸ���� ������
			graphics.SetInterpolationMode(InterpolationModeLowQuality);		// �̹��� ���� ������

			graphics.TranslateTransform(ObjCenter.x, ObjCenter.y);		// �Ҽ� ������Ʈ �߽������� ȸ���߽� �̵�
			graphics.ScaleTransform(bFlipHorizontal ? -1.0f : 1.0f, 1.0f);	// bool ������ �¿� ����
			graphics.RotateTransform(_angle);							// ȸ�� ����
			graphics.TranslateTransform(-ObjCenter.x, -ObjCenter.y);	// ���� ��ġ�� �̵�

			if (_renderPercent == 1.f)
				graphics.DrawImage(m_Bitmap, (int)(FinalPos.x - CameraPos.x), (int)(FinalPos.y - CameraPos.y), (int)m_Size.x, (int)m_Size.y);
			else
			{
				Rect destRect((int)(FinalPos.x - CameraPos.x), (int)(FinalPos.y - CameraPos.y) + (int)(m_Size.y * (1.f - _renderPercent)), (int)m_Size.x , (int)m_Size.y * _renderPercent);
				graphics.DrawImage(m_Bitmap, destRect, 0, (int)(m_Size.y * (1.f - _renderPercent)), (int)m_Size.x, (int)(m_Size.y * _renderPercent), UnitPixel);
			}
			graphics.ResetTransform();


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
	if (bCameraFallow)
	{
		CameraPos = Vec2D(0, 0);
	}
	Vec2D AbsCenter = CameraPos * (-1);		// ��ǥ�� ���� ���
	// �⺻ �ػ󵵸� �������� ī�޶��� ���� �ȿ� �ؽ�ó�� ���ԵǾ� �ִ��� Ȯ��
	if (CameraIntersectCheck(CameraPos, Resolution, 0.f, FinalPos, m_Size, _angle, Vec2D(0, 0), Vec2D(0, 0)) || bCameraFallow)
	{
		// �ε�� �̹����� �ػ� ������ ���� ������
		//m_DC = CEngine::GetInst()->GetSubDC();
		if (bLinearDodge)
		{
			// ���� ��Ʈ�� �̹����� ȸ����Ų �� ��Ʈ�� ����
			int newWidth = (int)(abs(m_Size.x * cos(_angle / 180 * PI)) + abs(m_Size.y * sin(_angle / 180 * PI)));
			int newHeight = (int)(abs(m_Size.x * sin(_angle / 180 * PI)) + abs(m_Size.y * cos(_angle / 180 * PI)));
			Bitmap RotatedBitmap(newWidth, newHeight);
			Graphics graphics(&RotatedBitmap);
			graphics.SetInterpolationMode(InterpolationModeLowQuality);
			graphics.TranslateTransform(newWidth / 2.0f, newHeight / 2.0f);
			graphics.RotateTransform(_angle);
			graphics.TranslateTransform(-m_Size.x / 2.0f, -m_Size.y / 2.0f);
			graphics.DrawImage(m_Bitmap, Point(0, 0));

			// ȸ���� ��Ʈ���� ���� ���� ��ǥ ���
			Vec2D RFinalPosLT = AbsCenter + FinalPos.Clockwise(_angle) + (m_Size.Clockwise(_angle) / 2) - Vec2D(newWidth / 2, newHeight / 2);
			Vec2D RFinalPosRB = RFinalPosLT + Vec2D(newWidth, newHeight);

			Bitmap* backbuffer = CEngine::GetInst()->GetBackbuffer()->GetBitmap();
			BitmapData BufferBitmapdata;
			BitmapData RotatedBitmapdata;
			Rect BufferRect(max(RFinalPosLT.x, 0)
				, max(RFinalPosLT.y, 0)
				, min(RFinalPosRB.x, CEngine::GetInst()->GetResolution().x) - max(RFinalPosLT.x, 0)
				, min(RFinalPosRB.y, CEngine::GetInst()->GetResolution().y) - max(RFinalPosLT.y, 0));
			Rect RotatedRect(max(-RFinalPosLT.x, 0)
				, max(-RFinalPosLT.y, 0)
				, min(RFinalPosRB.x, CEngine::GetInst()->GetResolution().x) - max(RFinalPosLT.x, 0)
				, min(RFinalPosRB.y, CEngine::GetInst()->GetResolution().y) - max(RFinalPosLT.y, 0));
			backbuffer->LockBits(&BufferRect, ImageLockModeWrite, PixelFormat32bppARGB, &BufferBitmapdata);
			RotatedBitmap.LockBits(&RotatedRect, ImageLockModeRead, PixelFormat32bppARGB, &RotatedBitmapdata);
			LinearDodge(&BufferBitmapdata, &RotatedBitmapdata
				, min(RFinalPosRB.x, CEngine::GetInst()->GetResolution().x) - max(RFinalPosLT.x, 0)
				, min(RFinalPosRB.y, CEngine::GetInst()->GetResolution().y) - max(RFinalPosLT.y, 0));
			RotatedBitmap.UnlockBits(&RotatedBitmapdata);
			backbuffer->UnlockBits(&BufferBitmapdata);
		}
		else
		{
			Graphics graphics(CEngine::GetInst()->GetBackbuffer()->GetBitmap());

			// ������ �� ���� �׷��Ƚ� ������ ����
			graphics.SetCompositingMode(CompositingModeSourceOver);			// ���ĺ��� ����
			graphics.SetCompositingQuality(CompositingQualityHighSpeed);	// ���� ���� ������
			graphics.SetPixelOffsetMode(PixelOffsetModeNone);				// �ȼ� ��ġ ���� ������
			graphics.SetSmoothingMode(SmoothingModeNone);					// ��Ƽ�ٸ���� ������
			graphics.SetInterpolationMode(InterpolationModeLowQuality);		// �̹��� ���� ������

			graphics.TranslateTransform(AbsCenter.x, AbsCenter.y);		// ��ǥ�� �������� ȸ���߽� �̵�
			graphics.RotateTransform(_angle);							// ȸ�� ����
			graphics.TranslateTransform(-AbsCenter.x, -AbsCenter.y);	// ���� ��ġ�� �̵�

			Status st = graphics.DrawImage(m_Bitmap, (int)(FinalPos.x - CameraPos.x), (int)(FinalPos.y - CameraPos.y), (int)m_Size.x, (int)m_Size.y);
		}
	}
}

bool CTexture::CameraIntersectCheck(Vec2D& _locationA, Vec2D& _SizeA, float _AngleA , Vec2D& _locationB, Vec2D& _SizeB, float _AngleB, Vec2D _rotationPivotA, Vec2D _rotationPivotB)
{
	Vec2D RectA[4] =
	{
		(_locationA - _rotationPivotA).Clockwise(_AngleA) + _rotationPivotA,
		((_locationA + Vec2D(_SizeA.x, 0.f)) - _rotationPivotA).Clockwise(_AngleA) + _rotationPivotA,
		((_locationA + _SizeA) - _rotationPivotA).Clockwise(_AngleA) + _rotationPivotA,
		((_locationA + Vec2D(0.f, _SizeA.y)) - _rotationPivotA).Clockwise(_AngleA) + _rotationPivotA
	};
	Vec2D RectB[4] =
	{
		(_locationB - _rotationPivotB).Clockwise(_AngleB) + _rotationPivotB,
		((_locationB + Vec2D(_SizeB.x, 0.f)) - _rotationPivotB).Clockwise(_AngleB) + _rotationPivotB,
		((_locationB + _SizeB) - _rotationPivotB).Clockwise(_AngleB) + _rotationPivotB,
		((_locationB + Vec2D(0.f, _SizeB.y)) - _rotationPivotB).Clockwise(_AngleB) + _rotationPivotB
	};

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (LineCrossCheck(RectA[i], RectA[(i + 1) % 4], RectB[j], RectB[(j + 1) % 4]))
				return true;
		}
	}

	if (min(RectB[0].x, RectB[2].x) <= RectA[0].x && max(RectB[0].x, RectB[2].x) >= RectA[0].x 
		&& min(RectB[0].y, RectB[2].y) <= RectA[0].y && max(RectB[0].y, RectB[2].y) >= RectA[0].y)
		return true;
	if (min(RectA[0].x, RectA[2].x) <= RectB[0].x && max(RectA[0].x, RectA[2].x) >= RectB[0].x
		&& min(RectA[0].y, RectA[2].y) <= RectB[0].y && max(RectA[0].y, RectA[2].y) >= RectB[0].y)
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

void CTexture::LinearDodge(BitmapData* _dest, BitmapData* _src, int _width, int _height)
{
	byte* Dest = (byte*)_dest->Scan0;
	byte* Src = (byte*)_src->Scan0;
	for (int y = 0; y < _height; ++y)
	{
		for (int x = 0; x < _width; ++x)
		{
			int destIndex = _dest->Stride * y + x * 4;
			int srcIndex = _src->Stride * y + x * 4;
			Dest[destIndex + 0] = min(Dest[destIndex + 0] + Src[srcIndex + 0], 255);	// B
			Dest[destIndex + 1] = min(Dest[destIndex + 1] + Src[srcIndex + 1], 255);	// G
			Dest[destIndex + 2] = min(Dest[destIndex + 2] + Src[srcIndex + 2], 255);	// R
			Dest[destIndex + 3] = min(Dest[destIndex + 3] + Src[srcIndex + 3], 255);	// A
		}
	}
}