#include "header.h"
#include "Ghost.h"
#include "MyProjectMain.h"


//this is needed for updateDir
// "class Packman;" won't do because of dynamic_cast 
#include "Packman.h"

Ghost::Ghost(MyProjectMain* pEngine, int iMapX, int iMapY)
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

	m_oImg.LoadImage("img/gGhost1.png");

	SetVisible(true);
}




Ghost::~Ghost(void)
{
}


void Ghost::Draw(void)
{
	if(!IsVisible()) return;
	int iSize = 16;
	m_oImg.RenderImageWithMask(GetEngine()->GetForeground(),
		0, 0, m_iCurrentScreenX-16, m_iCurrentScreenY-16,
		m_oImg.GetWidth(), m_oImg.GetHeight());
	
	StoreLastScreenPositionAndUpdateRect();
}

void Ghost::DoUpdate(int iCurrentTime)
{
	//see KillGhost() 
	if (!IsVisible()) return;

	m_iPreviousScreenX = m_iCurrentScreenX;
	m_iPreviousScreenY = m_iCurrentScreenY;

	//get reference to  tile manager and then some values that are used in a few places below
	GameTileManager& tm = m_pMainEngine->GetTileManager();

	int tileX, tileY, tileVal;

	tileX = tm.GetTileXForPositionOnScreen(m_iCurrentScreenX);
	tileY = tm.GetTileYForPositionOnScreen(m_iCurrentScreenY);
	tileVal = tm.GetValue(tileX, tileY);



	if ( m_oMover.HasMovementFinished(iCurrentTime) ) {

		//call ghost AIevery x ticks
		if (m_pMainEngine->GetTime()%20 == 1)
			runAI(tileX, tileY);

		switch (m_Dir) {
		case 'N':
			if (tm.GetValue(tileX, tileY-1) == 1)  
				break; 
			m_oMover.Setup(50+32*tileX+16, 50+32*tileY+16,
				50+32*tileX+16, 50+32*(tileY-1)+16,
				iCurrentTime, iCurrentTime+m_iSpeed);
			break;
		case 'S':
			if (tm.GetValue(tileX, tileY+1) == 1) 
				break; 
			m_oMover.Setup(50+32*tileX+16, 50+32*tileY+16,
				50+32*tileX+16, 50+32*(tileY+1)+16,
				iCurrentTime, iCurrentTime+m_iSpeed);
			break;
		case 'E':
			if (tm.GetValue(tileX+1, tileY) == 1) 
				break; 
			m_oMover.Setup(50+32*tileX+16, 50+32*tileY+16,
				50+32*(tileX+1)+16, 50+32*tileY+16,
				iCurrentTime, iCurrentTime+m_iSpeed);
			break;
		case 'W':
			if (tm.GetValue(tileX-1, tileY) == 1) 
				break; 
			m_oMover.Setup(50+32*tileX+16, 50+32*tileY+16,
				50+32*(tileX-1)+16, 50+32*tileY+16,
				iCurrentTime, iCurrentTime+m_iSpeed);
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

	
int Ghost::getCurrentTileX()
{
	GameTileManager& tm = m_pMainEngine->GetTileManager();


	return tm.GetTileXForPositionOnScreen(m_iCurrentScreenX);
}

int Ghost::getCurrentTileY()
{
	GameTileManager& tm = m_pMainEngine->GetTileManager();


	return tm.GetTileYForPositionOnScreen(m_iCurrentScreenY);
}


void Ghost::runAI(int tileX, int tileY)
{

	


	char newDir;
	int newDistance;
	int minDistance = 100;
	Packman* packman = dynamic_cast<Packman*>(m_pMainEngine->GetDisplayableObject(0));
	GameTileManager& tm = m_pMainEngine->GetTileManager();

	int packmanTileX, packmanTileY;

	packmanTileX = packman->getCurrentTileX();
	packmanTileY = packman->getCurrentTileY();
	if ( m_pMainEngine->isPowerDotActive() ) {
		// reflaction around the middle of the maze
		//as a result the ghost will aim to choose the direction that takes it as far from the packman as possible 
		packmanTileX = abs(packmanTileX - 16);
		packmanTileY = abs(packmanTileY - 16);
	}

	int selfTileX, selfTileY;
	selfTileX = getCurrentTileX();
	selfTileY = getCurrentTileY();

	//this prevents the ghosts from stucking underneath each other forever
	//it can still happen for a period of time
	if (isAlreadyOccupied(selfTileX, selfTileY)) {
			m_Dir = "NSEW"[rand()%4];
			return;
	}

	//distance is measured using manhattan distance
	
	//try north
	if (tm.GetValue(selfTileX,selfTileY-1) != 1 
		&& (abs(packmanTileX-selfTileX)+abs(packmanTileY-(selfTileY-1))) < minDistance) {
		newDir = 'N';
		minDistance = abs(packmanTileX-selfTileX)+abs(packmanTileY-(selfTileY-1));
	}
	//try south
	if (tm.GetValue(selfTileX,selfTileY+1) != 1 
		&& (abs(packmanTileX-selfTileX)+abs(packmanTileY-(selfTileY+1))) < minDistance) {
		newDir = 'S';
		minDistance = abs(packmanTileX-selfTileX)+abs(packmanTileY-(selfTileY+1));
	}
	//try east
	if (tm.GetValue(selfTileX+1,selfTileY) != 1 
		&& (abs(packmanTileX-(selfTileX+1))+abs(packmanTileY-selfTileY)) < minDistance) {
		newDir = 'E';
		minDistance = abs((packmanTileX+1)-selfTileX)+abs(packmanTileY-selfTileY);
	}
	//try west
	if (tm.GetValue(selfTileX-1,selfTileY) != 1 
		&& (abs(packmanTileX-(selfTileX-1))+abs(packmanTileY-selfTileY)) < minDistance) {
		newDir = 'W';
		minDistance = abs((packmanTileX-1)-selfTileX)+abs(packmanTileY-selfTileY);
	}
	
	m_Dir = newDir;

}



void Ghost::ChangePosition()
{

	//see KillGhost() 
	if (!IsVisible()) 
		return;


	//some random position in the upper half of the maze
	int iMapX, iMapY;
	iMapX = rand() % 13 + 2;
	iMapY = rand() % 6 + 2;

	//this is needed if colliosion happens when !m_oMover.HasMovmentFinished()
	//there's probably a better way of doing this...
	m_oMover.Setup(m_iCurrentScreenX,m_iCurrentScreenY,
		m_iCurrentScreenX, m_iCurrentScreenY, 
		m_pMainEngine->GetTime(), 1);



	m_iCurrentScreenX = 50+32*iMapX+16;
	m_iCurrentScreenY = 50+32*iMapY+16;
	return;
}

void Ghost::KillGhost()
{
	//rather crude way of doing this but it works well
	//also it isn't particulary inefficient since DoUpdate returns immediately if !IsVisible()
	m_iCurrentScreenX = 600;
	m_iCurrentScreenY = 400;
	if (IsVisible())
		m_pMainEngine->IncrementScore(10); //10 bonus points
	SetVisible(false);
	
	//a ghost being killed is a rare event so this shouldn't be a problem
	RedrawWholeScreen();
}



bool Ghost::isAlreadyOccupied(int iTileX, int iTileY)
{
	Ghost* g;
	int i = 1;

	//using while loop rather than for loop makes it easy to add more ghosts 
	while ( true ) {

		g = dynamic_cast<Ghost*>( m_pEngine->GetDisplayableObject(i) );
		//printf("got");
		if (g == NULL)
			break;
		//need to increment i in here, otherwise infinite loop because of next if
		++i;

		if ( g == this ) 
			continue;

		if ( abs( g-> getCurrentTileX() - iTileX ) <= 1
			&& abs( g->getCurrentTileX() - iTileY ) <=1 ) {
			return true;
		}
	}
	return false;

}