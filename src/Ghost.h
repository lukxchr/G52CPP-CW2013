#pragma once
#include "displayableobject.h"
#include "MovementPosition.h"
#include "JPGImage.h"

class MyProjectMain;


class Ghost :
	public DisplayableObject
{
public:
	Ghost(MyProjectMain* pEngine, int iMapX, int iMapY);
	~Ghost(void);
	void Draw(void);
	void DoUpdate(int iCurrentTime);
	int getCurrentTileX();
	int getCurrentTileY();
	void ChangePosition(); //this is used after collision, ghost is relocated to a random position in the upper part of the maze 
	void KillGhost();
	

private:
	MyProjectMain* m_pMainEngine;
	int m_iMapX, m_iMapY;
	int m_Dir; // { 'N', 'S', 'E', 'W' }, N stands for North etc.
	int m_iSpeed; // in ticks per tile
	MovementPosition m_oMover;
	ImageData m_oImg;
protected:
	void runAI(int tileX, int tileY);  
	bool isAlreadyOccupied(int iTileX, int iTileY); //checks if the region is occupied by another ghost 

};
