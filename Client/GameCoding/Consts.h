#pragma once

const int32 GWinSizeX = 800;
const int32 GWinSizeY = 600;

const int32 GWinHalfX = GWinSizeX / 2;
const int32 GWinHalfY = GWinSizeY / 2;

const int32 GMinimapSizeX = 200;
const int32 GMinimapSizeY = 128;

extern int32 GMapSizeX;
extern int32 GMapSizeY;

const float PI = 3.1415926f;

enum
{
	STAGE01_TILEX = 63,
	STAGE01_TILEY = 43,
	STAGE01_TILE_SIZEX = 48,
	STAGE01_TILE_SIZEY = 48,
	CITYSTAGE_TILEX = 80,
	CITYSTAGE_TILEY = 60,
	CITYSTAGE_TILE_SIZE = 48,
};

const VectorInt DELTA_XY[4] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };