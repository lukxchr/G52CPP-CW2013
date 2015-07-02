#pragma once

#include "tilemanager.h"

class GameTileManager :
	public TileManager
{
public:
	GameTileManager(void);
	~GameTileManager(void);

	// Get the size of a tile - width
	virtual int GetTileWidth();

	// Get the size of a tile - height
	virtual int GetTileHeight();

	
	virtual void DrawTileAt( 
		BaseEngine* pEngine, 
		SDL_Surface* pSurface, 
		int iMapX, int iMapY,
		int iStartPositionScreenX, int iStartPositionScreenY );

};
