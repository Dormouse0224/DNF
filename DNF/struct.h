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

#pragma pack(push, 1)	// 구조체 메모리 패딩을 없애기 위해 1바이트 단위로 맞춤.
struct DDSHeader {
	uint32_t dwMagic;			// "DDS " (0x20534444)
	uint32_t dwSize;			// Header 크기 (보통 124)
	uint32_t dwFlags;			// DDSD_CAPS, DDSD_HEIGHT, DDSD_WIDTH, DDSD_PIXELFORMAT 등
	uint32_t dwHeight;			// 이미지의 높이
	uint32_t dwWidth;			// 이미지의 너비
	uint32_t dwPitchOrLinearSize;
	uint32_t dwDepth;			// 볼륨 텍스처의 경우 (일반 2D 텍스처는 0)
	uint32_t dwMipMapCount;		// mipmap 수
	uint32_t dwReserved1[11];	// 예약된 공간
	struct {
		uint32_t dwSize;		// 픽셀 포맷 구조체 크기 (보통 32)
		uint32_t dwFlags;		// DDPF_ALPHAPIXELS, DDPF_FOURCC 등
		uint32_t dwFourCC;		// 포맷 (예: "DXT1", "DXT5")
		uint32_t dwRGBBitCount;	// 비트 수 (비압축 텍스처용)
		uint32_t dwRBitMask;	// Red 마스크 (비압축 텍스처용)
		uint32_t dwGBitMask;	// Green 마스크 (비압축 텍스처용)
		uint32_t dwBBitMask;	// Blue 마스크 (비압축 텍스처용)
		uint32_t dwABitMask;	// Alpha 마스크 (비압축 텍스처용)
	} ddspf;
	struct {
		uint32_t dwCaps1;		// DDSCAPS_COMPLEX, DDSCAPS_TEXTURE 등
		uint32_t dwCaps2;		// DDSCAPS2_CUBEMAP, DDSCAPS2_VOLUME 등
		uint32_t dwCaps3;
		uint32_t dwCaps4;		// 예약된 공간
	} ddsCaps;
	uint32_t dwReserved2;		// 예약된 공간
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