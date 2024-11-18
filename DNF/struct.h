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
	Vec2D operator +(const Vec2D& _Other) const
	{
		return Vec2D(x + _Other.x, y + _Other.y);
	}

	Vec2D operator -(const Vec2D& _Other) const
	{
		return Vec2D(x - _Other.x, y - _Other.y);
	}

	bool operator >(const Vec2D& _Other) const
	{
		if (x != _Other.x)
		{
			return x > _Other.x;
		}
		return y > _Other.y;
	}

	bool operator >>(const Vec2D& _Other) const
	{
		if (x > _Other.x && y > _Other.y)
		{
			return true;
		}
		return false;
	}

	bool operator <(const Vec2D& _Other) const
	{
		if (x != _Other.x)
		{
			return x < _Other.x;
		} 
		return y < _Other.y;
	}

	bool operator <<(const Vec2D& _Other) const
	{
		if (x < _Other.x && y < _Other.y)
		{
			return true;
		}
		return false;
	}

	bool operator ==(const Vec2D& _Other) const
	{
		if (x == _Other.x && y == _Other.y)
			return true;
		else
			return false;
	}

	bool operator !=(const Vec2D& _Other) const
	{
		return !(*this == _Other);
	}

	template<typename T>
	Vec2D operator *(const T& _Other) const
	{
		return Vec2D(x * _Other, y * _Other);
	}

	template<typename T>
	Vec2D operator /(const T& _Other) const
	{
		assert(_Other);
		return Vec2D(x / _Other, y / _Other);
	}

	float Dot(const Vec2D& _other) const
	{
		return (x * _other.x) + (y * _other.y);
	}

	float Cross(const Vec2D& _other) const
	{
		return (x * _other.y) - (y * _other.x);
	}

	Vec2D Clockwise(float _angle)
	{
		return Vec2D(x * cosf(_angle / 180.f * PI) - y * sinf(_angle / 180.f * PI), y * cosf(_angle / 180.f * PI) + x * sinf(_angle / 180.f * PI));
	}

	float Length() const
	{
		return sqrt((x * x) + (y * y));
	}

	Vec2D Normalize() const
	{
		return *this / sqrtf(x * x + y * y);
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

	Color m_Color;

	DbgRenderInfo(DbgRenderShape _Shape, Vec2D _Coord0, Vec2D _Coord1, float _Duration, Color _color)
		: m_Shape(_Shape)
		, m_Coord0(_Coord0)
		, m_Coord1(_Coord1)
		, m_Time(0)
		, m_Duration(_Duration)
		, m_Color(_color)
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
	float Scale = 1.f;
};
#pragma pack(pop)

struct TextInfo
{
	UINT x;
	UINT y;
	wstring Text;
	COLORREF Color;

	TextInfo()
		: Color(RGB(0, 0, 0))
		, x(0)
		, y(0)
	{}
	TextInfo(UINT _x, UINT _y, wstring _Text, COLORREF _Color)
	{
		x = _x;
		y = _y;
		Text = _Text;
		Color = _Color;
	}
};

#pragma pack(push, 1)
struct AnimationInfo
{
	int AlbumPathLen;
	int NPKDirLen;
	int IndexBegin;
	int IndexEnd;
	bool bLoop;
	int FPS;
	Vec2D Offset;
	float angle;
	bool bDodge;
};
#pragma pack(pop)

struct NPCInfo
{
	wstring Name;
	Vec2D Pos;
	Vec2D Size;
	wstring IdleAnimation;
};

struct WallInfo
{
	wstring Name;
	Vec2D Pos;
	Vec2D Size;
};

struct MonsterInfo
{
	wstring Name;
	MonsterTemplate MonsterTemplate;
	Vec2D pos;
};



struct PortalInfo
{
	Vec2D Location = Vec2D(-1, -1);
	wstring PointStageName = L"";
};

struct StageInfo
{
	wstring StageName;
	StageType StageType;
	Vec2D StageSize;
	Vec2D GridLoc;
	wstring BGMPath;
	vector<wstring> vecBGA;
	int UpperBound = 0;
	PortalInfo arrPortalInfo[(int)PortalDirection::END];
	vector<MonsterInfo*> vecMonsterInfo = {};
	vector<WallInfo*> vecWallInfo = {};
	vector<NPCInfo*> vecNPCInfo = {};

	~StageInfo()
	{
		auto deletevec = [](auto& vec)
			{
				for (int i = 0; i < vec.size(); ++i)
				{
					if (vec[i])
						delete vec[i];
				}
				vec.clear();
			};
		deletevec(vecMonsterInfo);
		deletevec(vecWallInfo);
		deletevec(vecNPCInfo);
	}
};

struct DungeonInfo
{
	wstring DungeonName;
	int StageCount;
};

#pragma pack(push, 1)
struct StageInfoHeader
{
	StageType StageType;
	Vec2D StageSize;
	Vec2D GridLoc;
	int UpperBound;
	int vecBGACount;
	int vecMonsterInfoCount;
	int vecWallInfoCount;
	int vecNPCInfoCount;
};
#pragma pack(pop)

struct ItemInfo
{
	ItemType Item_type;
	int Item_secondtype;
	wstring Texture_ani;
	string Name;
};
