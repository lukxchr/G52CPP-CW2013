#include "header.h"

#include "GameTileManager.h"

GameTileManager::GameTileManager(void)
{
}

GameTileManager::~GameTileManager(void)
{
}

// Get the size of a tile - width
int GameTileManager::GetTileWidth()
{
	return 32;
}

// Get the size of a tile - height
int GameTileManager::GetTileHeight()
{
	return 32;
}


void GameTileManager::DrawTileAt( 
	BaseEngine* pEngine, 
	SDL_Surface* pSurface, 
	int iMapX, int iMapY,
	int iStartPositionScreenX, int iStartPositionScreenY )
{


	switch( GetValue(iMapX,iMapY) )
	{
	//empty
	case 0:
		
		pEngine->DrawRectangle( 
			iStartPositionScreenX,
			iStartPositionScreenY, 
			iStartPositionScreenX + GetTileWidth() - 1,
			iStartPositionScreenY + GetTileHeight() - 1,
			 0 ,
			pSurface );
			
		break;
		//wall
	case 1:
		pEngine->DrawRectangle( 
			iStartPositionScreenX,
			iStartPositionScreenY, 
			iStartPositionScreenX + GetTileWidth() - 1,
			iStartPositionScreenY + GetTileHeight() - 1,
			 0xff5500,
			pSurface );
		
		break;

	//dot
	case 3:
		
		pEngine->DrawOval( 
			iStartPositionScreenX+GetValue(iMapX,iMapY)*2+5,
			iStartPositionScreenY+GetValue(iMapX,iMapY)*2+5, 
			iStartPositionScreenX + GetTileWidth() - GetValue(iMapX,iMapY)*2 -6,
			iStartPositionScreenY + GetTileHeight() - GetValue(iMapX,iMapY)*2 -6,
			0xffffff, pSurface );
		break;
	//boost dot
	case 4:
		
		pEngine->DrawOval( 
			iStartPositionScreenX+GetValue(iMapX,iMapY)*2,
			iStartPositionScreenY+GetValue(iMapX,iMapY)*2, 
			iStartPositionScreenX + GetTileWidth() - GetValue(iMapX,iMapY)*2 -1,
			iStartPositionScreenY + GetTileHeight() - GetValue(iMapX,iMapY)*2 -1,
			0x00ffff, pSurface );
		break;
	//power dot
	case 5:
		
		pEngine->DrawOval( 
			iStartPositionScreenX+GetValue(iMapX,iMapY)*2,
			iStartPositionScreenY+GetValue(iMapX,iMapY)*2, 
			iStartPositionScreenX + GetTileWidth() - GetValue(iMapX,iMapY)*2 -1,
			iStartPositionScreenY + GetTileHeight() - GetValue(iMapX,iMapY)*2 -1,
			0xff00ff, pSurface );
		break;
	//big dot
	case 6:
	
		pEngine->DrawOval( 
			iStartPositionScreenX+GetValue(iMapX,iMapY),
			iStartPositionScreenY+GetValue(iMapX,iMapY), 
			iStartPositionScreenX + GetTileWidth() - GetValue(iMapX,iMapY),
			iStartPositionScreenY + GetTileHeight() - GetValue(iMapX,iMapY),
			0xffffff, pSurface );
		break;


	}
}
