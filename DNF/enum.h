#pragma once

enum class PEN_TYPE
{
	RED,
	GREEN,
	BLUE,
	END,
};

enum class BRUSH_TYPE
{
	RED,
	GREEN,
	BLUE,
	GRAY,
	HOLLOW,
	END,
};


enum class TaskType
{
	ObjectCreate,
	ObjectDelete,
	ChangeLevel,

};

enum class LayerType
{
	Background,
	Far,
	Object,
	Near,

	UI = 63,
	END = 64
};


enum class DbgRenderShape
{
	Rectangle,
	Circle,
	Line,

};

enum class ResourceType
{
	Texture,
	Sound,
	Album,

};

enum class ImgVersion
{
	Other = 0,
	Ver1 = 1,
	Ver2 = 2,

	Ver4 = 4,
	Ver5 = 5,
	Ver6 = 6,
	Ver7 = 7,
	Ver8 = 8,
	Ver9 = 9,
};

enum class ColorBits
{
	UNKNOWN = 0,
	ARGB_1555 = 14, // 0x0000000E
	ARGB_4444 = 15, // 0x0000000F
	ARGB_8888 = 16, // 0x00000010
	LINK = 17, // 0x00000011
	DXT_1 = 18, // 0x00000012
	DXT_3 = 19, // 0x00000013
	DXT_5 = 20, // 0x00000014
};

enum class CompressMode
{
	UNKNOWN = 1,
	NONE = 5,
	ZLIB = 6,
	DDS_ZLIB = 7,
};

enum class TextureVersion
{
	Dxt1 = 1,
	Dxt3 = 3,
	Dxt5 = 5,
};

enum class StageType
{
	START,
	MIDDLE,
	FINAL,

};

enum class PortalDirection
{
	TOP,
	LEFT,
	RIGHT,
	BOTTOM,

	END
};

enum class MonsterTemplate
{
	bloatedfat,
	dominatedunnaturals,

	NONE
};

enum class NPCTemplate
{
	seria,
	chestbox,

	NONE
};

enum class MonsterState
{
	Idle,
	Move,
	Attack,
	Hurt,
	JumpUp,
	JumpDown,
	

	END
};

enum class PlayerState
{
	Idle,
	Walk,
	Run,
	Jump,
	Attack,


	END
};

enum class LargoState
{
	Income,
	Attack,
	Pattern,
	Death,

	END
};

enum class CameraEffect
{
	FadeIn,
	FadeOut,
	Flash,

	END
};

enum class AvatarParts
{
	Hair,
	Cap,
	Pants,
	Coat,
	Shoes,
	LBow,

	END
};