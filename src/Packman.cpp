#include "header.h"
#include "Packman.h"
#include "MyProjectMain.h"
//this is needed to check for collisions 
#include "Ghost.h"


Packman::Packman(MyProjectMain* pEngine, int iMapX, int iMapY)
: DisplayableObject(pEngine)
, m_pMainEngine( pEngine )
, m_iMapX(iMapX)
, m_iMapY(iMapY)
, m_iSpeed(300)

{
	m_iCurrentScreenX = m_iPreviousScreenX = 50+32*iMapX+16;
	m_iCurrentScreenY = m_iPreviousScreenY = 50+32*iMapY+16;

	m_iStartDrawPosX = -16;
	m_iStartDrawPosY = -16;

	m_iDrawWidth = 32;
	m_iDrawHeight = 32;

	SetVisible(true);
}


Packman::~Packman(void)
{
}


void Packman::Draw(void)
{
	if(!IsVisible())
		return;

	int iSize = 16;
	int iTick = m_pMainEngine->GetTime()/20;
	int iFrame = iTick % 20;
	m_pMainEngine->DrawScreenOval(
			m_iCurrentScreenX - iSize,
			m_iCurrentScreenY - iSize,
			m_iCurrentScreenX + iSize-1,
			m_iCurrentScreenY + iSize-1,
			0xffff00 );
	
	//animation only if the packman is moving 
	if ( !m_oMover.HasMovementFinished(m_pMainEngine->GetTime()) && iFrame < 10 ) {

		if (m_Dir == 'N')
			m_pMainEngine->DrawScreenTriangle(m_iCurrentScreenX, m_iCurrentScreenY,
				m_iCurrentScreenX+iSize-1, m_iCurrentScreenY-iSize+1,
				m_iCurrentScreenX-iSize+1, m_iCurrentScreenY-iSize+1,
				0x0);

		if (m_Dir == 'S')
			m_pMainEngine->DrawScreenTriangle(m_iCurrentScreenX, m_iCurrentScreenY,
				m_iCurrentScreenX+iSize-1, m_iCurrentScreenY+iSize-1,
				m_iCurrentScreenX-iSize+1, m_iCurrentScreenY+iSize-1,
				0x0);

		if (m_Dir == 'E')
			m_pMainEngine->DrawScreenTriangle(m_iCurrentScreenX, m_iCurrentScreenY,
				m_iCurrentScreenX+iSize-1, m_iCurrentScreenY+iSize-1,
				m_iCurrentScreenX+iSize-1, m_iCurrentScreenY-iSize-1,
				0x0);

		if (m_Dir == 'W')
			m_pMainEngine->DrawScreenTriangle(m_iCurrentScreenX, m_iCurrentScreenY,
				m_iCurrentScreenX-iSize+1, m_iCurrentScreenY+iSize-1,
				m_iCurrentScreenX-iSize+1, m_iCurrentScreenY-iSize-1,
				0x0);
		
			
	
	} //if ends

	StoreLastScreenPositionAndUpdateRect();

}

void Packman::DoUpdate(int iCurrentTime)
{
	m_iPreviousScreenX = m_iCurrentScreenX;
	m_iPreviousScreenY = m_iCurrentScreenY;

	//get reference to  tile manager and then some values that are used in a few places below
	GameTileManager& tm = m_pMainEngine->GetTileManager();

	int tileX, tileY, tileVal;

	tileX = tm.GetTileXForPositionOnScreen(m_iCurrentScreenX);
	tileY = tm.GetTileYForPositionOnScreen(m_iCurrentScreenY);
	tileVal = tm.GetValue(tileX, tileY);

	if (BoostActive()) 
		UpdateBoost(m_pMainEngine->GetTime());
	



	if ( m_oMover.HasMovementFinished(iCurrentTime)) {
		//handle key presses
		if (m_pMainEngine->IsKeyPressed(SDLK_UP))
			m_Dir = 'N';
		if (m_pMainEngine->IsKeyPressed(SDLK_DOWN))
			m_Dir = 'S';
		if (m_pMainEngine->IsKeyPressed(SDLK_RIGHT))
			m_Dir = 'E';
		if (m_pMainEngine->IsKeyPressed(SDLK_LEFT))
			m_Dir = 'W';
	}

	if ( m_oMover.HasMovementFinished(iCurrentTime) ) {

	int i =1;
	while (true) {

		Ghost* o =  dynamic_cast<Ghost*>(m_pMainEngine->GetDisplayableObject(i));
		
		
		if (o == NULL) break;
		if (abs(o->getCurrentTileX() - tileX) <= 1 &&
			abs(o->getCurrentTileY() - tileY) <= 1) {
			if ( m_pMainEngine->isPowerDotActive() ) 
				o->KillGhost();
			else
				m_pMainEngine->KillPackman();
			return;
		} ++i;
			
	}

		switch (m_Dir) {
		case 'N':
			if (tm.GetValue(tileX, tileY-1) == 1) break; //if wall break, otherwise setup movement
			m_oMover.Setup(50+32*tileX+16, 50+32*tileY+16,
				50+32*tileX+16, 50+32*(tileY-1)+16,
				iCurrentTime, iCurrentTime+m_iSpeed);
			break;
		case 'S':
			if (tm.GetValue(tileX, tileY+1) == 1) break; //if wall break, otherwise setup movement
			m_oMover.Setup(50+32*tileX+16, 50+32*tileY+16,
				50+32*tileX+16, 50+32*(tileY+1)+16,
				iCurrentTime, iCurrentTime+m_iSpeed);
			break;
		case 'E':
			if (tm.GetValue(tileX+1, tileY) == 1) break; //if wall break, otherwise setup movement
			m_oMover.Setup(50+32*tileX+16, 50+32*tileY+16,
				50+32*(tileX+1)+16, 50+32*tileY+16,
				iCurrentTime, iCurrentTime+m_iSpeed);
			break;
		case 'W':
			if (tm.GetValue(tileX-1, tileY) == 1) break; //if wall break, otherwise setup movement
			m_oMover.Setup(50+32*tileX+16, 50+32*tileY+16,
				50+32*(tileX-1)+16, 50+32*tileY+16,
				iCurrentTime, iCurrentTime+m_iSpeed);
			break;

		} //switch ends

	switch (tileVal) {

	case 3: 
		tm.UpdateTile(m_pMainEngine,tileX,tileY,0);
		m_pMainEngine->IncrementScore();
		m_pMainEngine->DecrementDotsRemaining();
		break;
	case 4:
		tm.UpdateTile(m_pMainEngine,tileX,tileY,0);
		Boost(m_pMainEngine->GetTime());
		break;
	case 5:
		tm.UpdateTile(m_pMainEngine,tileX,tileY,0);
		m_pMainEngine->ActivatePowerDot();
		break;
	case 6:
		tm.UpdateTile(m_pMainEngine,tileX,tileY,0);
		m_pMainEngine->IncrementScore(10); //big dot gives 10 points
		m_pMainEngine->DecrementDotsRemaining(10);
		break;

	}

	}

	
	if ( !m_oMover.HasMovementFinished(iCurrentTime) ) {
		m_oMover.Calculate(iCurrentTime);
		m_iCurrentScreenX = m_oMover.GetX();
		m_iCurrentScreenY = m_oMover.GetY();
	}
		
	

	
	RedrawObjects();
	return;
}


int Packman::getCurrentTileX()
{
	GameTileManager& tm = m_pMainEngine->GetTileManager();
	return tm.GetTileXForPositionOnScreen(m_iCurrentScreenX);
}

int Packman::getCurrentTileY()
{
	GameTileManager& tm = m_pMainEngine->GetTileManager();
	return tm.GetTileYForPositionOnScreen(m_iCurrentScreenY);
}


void Packman::ChangePosition(int iMapX, int iMapY)
{
	m_iCurrentScreenX = 50+32*iMapX+16;
	m_iCurrentScreenY = 50+32*iMapY+16;
	return;
}


void Packman::Boost(int iCurrentTime)
{
	m_iTimeToSwitchBoostOff = iCurrentTime + 10e3;
	m_iSpeed -= 150;
}


void Packman::UpdateBoost(int iCurrentTime)
{
	if ((m_iTimeToSwitchBoostOff - iCurrentTime) < 0 )
		m_iSpeed = 300;

}