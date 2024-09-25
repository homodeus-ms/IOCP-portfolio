#pragma once

enum class SceneType
{
	None,
	DevScene,
	EditScene
};

enum LayerType
{
	LAYER_BACKGROUND,
	LAYER_OBJECT,
	LAYER_FOREGROUND,
	LAYER_UI,
	LAYER_TILEMAP,
	LAYER_EFFECT,
	LAYER_MAXCOUNT,
};

enum class ColliderType
{
	Box,
	Sphere,

};

/*
enum class ObjectState
{
	Idle,
	Move,
	Attack,
};

enum Dir
{
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT,
	DIR_END,
};
*/
