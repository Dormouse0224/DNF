#pragma once
#include "assert.h"



struct Vec2D
{
public:
	float x;
	float y;

public:
	Vec2D()
		: x(0.f)
		, y(0.f)
	{}

	template<typename T>
	Vec2D(T _x, T _y)
		: x((float)_x)
		, y((float)_y)
	{}

	Vec2D(const Vec2D& _other)
		: x(_other.x)
		, y(_other.y)
	{}

	~Vec2D()
	{}

public:
	Vec2D operator +(const Vec2D& _Other)
	{
		return Vec2D(x + _Other.x, y + _Other.y);
	}

	Vec2D operator -(const Vec2D& _Other)
	{
		return Vec2D(x - _Other.x, y - _Other.y);
	}

	template<typename T>
	Vec2D operator *(const T& _Other)
	{
		return Vec2D(x * _Other, y * _Other);
	}

	template<typename T>
	Vec2D operator /(const T& _Other)
	{
		assert(_Other);
		return Vec2D(x / _Other, y / _Other);
	}
};


struct StTask
{
	TaskType m_TaskType;
	DWORD_PTR m_param0;
	DWORD_PTR m_param1;
};

struct DbgRenderInfo
{
	DbgRenderShape m_Shape;
	Vec2D m_Coord0;
	Vec2D m_Coord1;

	float m_Time;
	float m_Duration;

	DbgRenderInfo(DbgRenderShape _Shape, Vec2D _Coord0, Vec2D _Coord1, float _Duration)
		: m_Shape(_Shape)
		, m_Coord0(_Coord0)
		, m_Coord1(_Coord1)
		, m_Time(0)
		, m_Duration(_Duration)
	{}
};

struct TextureInfo
{
	class CTexture* Texture;
	Vec2D LeftTop;
	Vec2D RightBottom;
	int Top;
	int Unknown;
};

#pragma pack(push, 1)	// ����ü �޸� �е��� ���ֱ� ���� 1����Ʈ ������ ����.
struct DDSHeader {
	uint32_t dwMagic;			// "DDS " (0x20534444)
	uint32_t dwSize;			// Header ũ�� (���� 124)
	uint32_t dwFlags;			// DDSD_CAPS, DDSD_HEIGHT, DDSD_WIDTH, DDSD_PIXELFORMAT ��
	uint32_t dwHeight;			// �̹����� ����
	uint32_t dwWidth;			// �̹����� �ʺ�
	uint32_t dwPitchOrLinearSize;
	uint32_t dwDepth;			// ���� �ؽ�ó�� ��� (�Ϲ� 2D �ؽ�ó�� 0)
	uint32_t dwMipMapCount;		// mipmap ��
	uint32_t dwReserved1[11];	// ����� ����
	struct {
		uint32_t dwSize;		// �ȼ� ���� ����ü ũ�� (���� 32)
		uint32_t dwFlags;		// DDPF_ALPHAPIXELS, DDPF_FOURCC ��
		uint32_t dwFourCC;		// ���� (��: "DXT1", "DXT5")
		uint32_t dwRGBBitCount;	// ��Ʈ �� (����� �ؽ�ó��)
		uint32_t dwRBitMask;	// Red ����ũ (����� �ؽ�ó��)
		uint32_t dwGBitMask;	// Green ����ũ (����� �ؽ�ó��)
		uint32_t dwBBitMask;	// Blue ����ũ (����� �ؽ�ó��)
		uint32_t dwABitMask;	// Alpha ����ũ (����� �ؽ�ó��)
	} ddspf;
	struct {
		uint32_t dwCaps1;		// DDSCAPS_COMPLEX, DDSCAPS_TEXTURE ��
		uint32_t dwCaps2;		// DDSCAPS2_CUBEMAP, DDSCAPS2_VOLUME ��
		uint32_t dwCaps3;
		uint32_t dwCaps4;		// ����� ����
	} ddsCaps;
	uint32_t dwReserved2;		// ����� ����
};
#pragma pack(pop)

#pragma pack(push, 1)
struct ProgramInfo
{
public:
	Vec2D Resolution;
	Vec2D WindowPos;
	float Scale;
};
#pragma pack(pop)