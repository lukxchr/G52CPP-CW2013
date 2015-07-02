#pragma once
#include "displayableobject.h"
#include "MovementPosition.h"

class MyProjectMain;

class Packman :
	public DisplayableObject
{


public:
	Packman(MyProjectMain* pEngine, int iMapX, int iMapY);
	~Packman(void);
	void Draw(void);
	void DoUpdate(int iCurrentTime);
	int getCurrentTileX();
	int getCurrentTileY();
	void ChangePosition(int iMapX=8, int iMapY=14); //this is used after collision 
	void Boost(int iCurrentTime); // boost and record time to switch boost off
	void UpdateBoost(int iCurrentTime); // check if it's time to switch boost off
	bool BoostActive() { return !(m_iSpeed == 300); }

private:
	MyProjectMain* m_pMainEngine;
	int m_iMapX, m_iMapY;
	int m_Dir; // { 'N', 'S', 'E', 'W' }, N stands for North etc.
	int m_iSpeed; // in ticks per tile i.e. lower m_iSpeed means higer actual speed
	MovementPosition m_oMover;
	int m_iTimeToSwitchBoostOff;

};

