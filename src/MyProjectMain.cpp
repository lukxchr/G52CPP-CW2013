#include "header.h"
#include "BaseEngine.h"
#include "Ghost.h"
#include "Packman.h"
#include "MyProjectMain.h"
#include "JPGImage.h"
#include "TileManager.h"

#include <fstream>
#include <iostream>

using namespace std;

MyProjectMain::MyProjectMain(void)
: BaseEngine( 50 )
, m_state(stateInit)

{
}

MyProjectMain::~MyProjectMain(void)
{
}

void MyProjectMain::SetupBackgroundBuffer()
{
	switch( m_state )
	{
	case stateInit: 
		FillBackground( 0 );

		{
		
			// do this in  Init because it doesn't change for different levels/mazes
			m_oTiles.SetSize( 16, 16 ); 
			m_oTiles.SetBaseTilesPositionOnScreen( 50, 50 );

			//load graphics for later use
			m_oBackgroundImg.LoadImage("img/background.jpg");
			m_oLogoImg.LoadImage("img/logo.png");
			
			m_oLogoImg.RenderImageWithMask( this->GetBackground(), 0, 0, 100, 100,
				m_oLogoImg.GetWidth(), m_oLogoImg.GetHeight() );

		}
		return;


	case stateMenu:
		FillBackground( 0xff0000 );
		m_oBackgroundImg.RenderImageWithMask( this->GetBackground(), 0, 0, 0, 0,
				m_oBackgroundImg.GetWidth(), m_oBackgroundImg.GetHeight() );
		return;

	case stateChooseLevel:
		FillBackground( 0x00ff00);
		m_oBackgroundImg.RenderImageWithMask( this->GetBackground(), 0, 0, 0, 0,
				m_oBackgroundImg.GetWidth(), m_oBackgroundImg.GetHeight() );
		return;

	case stateMain:

		FillBackground( 0 );

		//load maze from file only if not already loaded
		/*the resoning behind loading the maze here and
		not in Init is to avoid having to keep all mazes in memory*/
		
		if (m_vsMaze.empty()) 
			SetupNewGame();

		m_oTiles.DrawAllTiles( this, 
			this->GetBackground(), 
			0, 0, 15, 15 );
		break; 

	case statePaused:
		FillBackground( 0 );
		m_oTiles.DrawAllTiles( this, 
			this->GetBackground(), 
			0, 0, 15, 15 );
		break;

	case stateGameOver:
		FillBackground( 0x0000ff );
		m_oBackgroundImg.RenderImageWithMask( this->GetBackground(), 0, 0, 0, 0,
				m_oBackgroundImg.GetWidth(), m_oBackgroundImg.GetHeight() );
		Redraw(true);
		break;

	case stateMazeEditor:
		FillBackground(0);
		m_oTiles.DrawAllTiles( this, 
			this->GetBackground(), 
			0, 0, 15, 15 );
		break;
	} // switch ends
}

int MyProjectMain::InitialiseObjects()
{
	DrawableObjectsChanged();
	DestroyOldObjects();

	m_ppDisplayableObjects = new DisplayableObject*[5]; 

	m_ppDisplayableObjects[0] = new Packman(this,8,14);
	m_ppDisplayableObjects[1] = new Ghost(this,10,10);	
	m_ppDisplayableObjects[2] = new Ghost(this,5,5);
	m_ppDisplayableObjects[3] = new Ghost(this,7,7);
	m_ppDisplayableObjects[4] = NULL;
	


	return 0;
}







void MyProjectMain::DrawStrings()
{
	switch( m_state )
	{
	case stateInit:
		DrawScreenString( 200, 500, "Press SPACE to continue", 0xffffff, NULL );
		break;
	case stateChooseLevel:
		DrawScreenString( 100, 300, "Press 1, 2 or 3 to select level", 0xffffff, NULL );
		break;
	case stateMain:
		CopyBackgroundPixels( 500, 40, GetScreenWidth(), 100 );
		itoa(m_iScore, m_ScoreBuffer, 10);
		itoa(m_iLivesLeft, m_iLivesLeftBuffer, 10);
		DrawScreenString( 600, 100, "Score" , 0xffffff, NULL );
		DrawScreenString( 600, 50, "Lives" , 0xffffff, NULL );
		DrawScreenString( 700, 100, m_ScoreBuffer , 0xffffff, NULL ); 
		DrawScreenString( 700, 50, m_iLivesLeftBuffer , 0xffffff, NULL ); 


		//draw in red if boost/power dot active, white otherwise
		if ( dynamic_cast<Packman*>(GetDisplayableObject(0))->BoostActive() )
			DrawScreenString( 600, 250, "BOOST" , 0xfF0000, NULL );
		else {
			DrawScreenString( 600, 250, "BOOST" , 0xfFffff, NULL );
		}

		if (m_bPowerDotActive) {
			DrawScreenString( 600, 200, "POWER DOT" , 0xff0000, NULL );
			//disactivate if time's up
			if ( m_iTimeToDisactivatePowerDot - this->GetTime() < 0)
				m_bPowerDotActive = false;
		} else
			DrawScreenString( 600, 200, "POWER DOT" , 0xffffff, NULL );

		DrawScreenString( 580, 400, "Press:" , 0xffffff, NULL );
		DrawScreenString( 580, 450, "(M)enu" , 0xffffff, NULL );
		DrawScreenString( 580, 500, "Space to pause" , 0xffffff, NULL );

		SetNextUpdateRect( 500, 40, GetScreenWidth(), 400 );
		break;

	case statePaused:
		DrawScreenString( 200, 300, "Paused. Press SPACE to continue", 0xffffff, NULL );
		DrawScreenString( 600, 100, "Score" , 0xffffff, NULL );
		DrawScreenString( 600, 50, "Lives" , 0xffffff, NULL );
		DrawScreenString( 700, 100, m_ScoreBuffer , 0xffffff, NULL ); 
		DrawScreenString( 700, 50, m_iLivesLeftBuffer , 0xffffff, NULL ); 


		//draw in red if boost/power dot active, white otherwise
		if ( dynamic_cast<Packman*>(GetDisplayableObject(0))->BoostActive() )
			DrawScreenString( 600, 250, "BOOST" , 0xfF0000, NULL );
		else {
			DrawScreenString( 600, 250, "BOOST" , 0xfFffff, NULL );
		}

		if (m_bPowerDotActive) {
			DrawScreenString( 600, 200, "POWER DOT" , 0xff0000, NULL );
			//disactivate if time's up
			if ( m_iTimeToDisactivatePowerDot - this->GetTime() < 0)
				m_bPowerDotActive = false;
		} else
			DrawScreenString( 600, 200, "POWER DOT" , 0xffffff, NULL );
		break;

	case stateMenu:
		DrawScreenString( 200, 350, "NEW GAME: PRESS 1", 0xBBFF00, NULL );
		DrawScreenString( 200, 400, "MAZE EDITOR: PRESS 2", 0xBBff00, NULL );
	break;

	case stateGameOver:
		DrawScreenString( 200, 300, "Your Score:", 0xffffff, NULL );
		DrawScreenString( 375, 300, m_ScoreBuffer, 0xffffff, NULL );
		DrawScreenString( 200, 400, "Press (M)enu", 0xffffff, NULL );
		break;

	case stateMazeEditor:
		DrawScreenString( 600, 50, "Press:" , 0xffffff, NULL );
		DrawScreenString( 600, 100, "(S)ave" , 0xffffff, NULL );
		DrawScreenString( 600, 150, "(M)enu" , 0xffffff, NULL );
		DrawScreenString( 600, 200, "1, 2 or 3" , 0xffffff, NULL );
		DrawScreenString( 600, 220, "to switch" , 0xffffff, NULL );
		DrawScreenString( 600, 240, "between" , 0xffffff, NULL );
		DrawScreenString( 600, 260, "mazes" , 0xffffff, NULL );
		DrawScreenString( 50, 565, "LEFT CLICK to change between wall/empty" , 0xffffff, NULL );
		break;
	}
}


void MyProjectMain::GameAction()
{
	if ( !TimeToAct() )
		return;

	SetTimeToAct( 1 );

	switch( m_state )
	{
	case stateInit:
	case stateMenu:
	case statePaused:
	case stateChooseLevel:
	case stateGameOver:
	case stateMazeEditor:
		break;
	case stateMain:
		UpdateAllObjects( GetTime() );
		break;
	}
}


void MyProjectMain::MouseDown( int iButton, int iX, int iY )
{
	//mouse can be used only in stateMazeEditor 
	if ( m_state != stateMazeEditor ) 
		return;


	if ( iButton == SDL_BUTTON_LEFT ) {
		int tileX, tileY;
		tileX = m_oTiles.GetTileXForPositionOnScreen(iX);
		tileY = m_oTiles.GetTileYForPositionOnScreen(iY);
		//if outside maze return
		if ( tileX < 1 || tileX > 14 || tileY < 1 || tileY > 14)
			return;
		//switch between 0/1 nothing/wall
		int newVal = (m_oTiles.GetValue(tileX, tileY)) ? 0 : 1; // 0 == false
		m_oTiles.UpdateTile(this,tileX, tileY, newVal);
		//SetupBackgroundBuffer();
		Redraw(true);
				
	}
		


}


void MyProjectMain::KeyDown(int iKeyCode)
{
	switch ( iKeyCode )
	{
	
	case SDLK_ESCAPE: 
		SetExitWithCode( 0 );
		break;

	//back to menu when m pressed
	case SDLK_m:
		//this is needed so that another level will be loaded if the player 
		//starts a new game without restarting the whole program
		m_vsMaze.clear();
		m_state = stateMenu;
		SetupBackgroundBuffer();
		Redraw(true);
		break;

	//pause
	case SDLK_SPACE: 
		switch( m_state )
		{
		case stateInit:
			m_state = stateMenu;
			SetupBackgroundBuffer();
			Redraw(true);
			break;

		case stateMain:
			m_state = statePaused;
			SetupBackgroundBuffer();
			Redraw(true);
			break;
		case statePaused:
			m_state = stateMain;
			SetupBackgroundBuffer();
			Redraw(true);
			break;
		} // state switch ends
		break; 

	case SDLK_1:
		switch ( m_state ) {
		case stateMenu:
			m_state = stateChooseLevel;
			SetupBackgroundBuffer();
			Redraw(true);
			break;

		case stateChooseLevel:
			m_sMazeFile = "mazes/01.txt";
			m_state = stateMain;
			SetupBackgroundBuffer();
			Redraw(true);
			break;

		case stateMazeEditor:
			SetUpEditor("mazes/01.txt");
			SetupBackgroundBuffer();
			Redraw(true);
		}//state switch ends
		break;

case SDLK_2:
		switch ( m_state ) {
		
		case stateChooseLevel:
			m_sMazeFile = "mazes/02.txt";
			m_state = stateMain;
			SetupBackgroundBuffer();
			Redraw(true);
			break;

		case stateMenu:
			SetUpEditor();
			m_state = stateMazeEditor;
			SetupBackgroundBuffer();
			Redraw(true);
			break;

		case stateMazeEditor:
			SetUpEditor("mazes/02.txt");
			SetupBackgroundBuffer();
			Redraw(true);
			break;
		}//state switch ends
		break;

case SDLK_3:
		switch ( m_state ) {
		
		case stateChooseLevel:
			m_sMazeFile = "mazes/03.txt";
			m_state = stateMain;
			SetupBackgroundBuffer();
			Redraw(true);
			break;

		case stateMazeEditor:
			SetUpEditor("mazes/03.txt");
			SetupBackgroundBuffer();
			Redraw(true);
		
		}//state switch ends
		break;
		
case SDLK_s:
	//used only in stateMazeEditor to save changes
	if (m_state == stateMazeEditor)
		SaveMazeToFile();
	} 

}


void MyProjectMain::DrawChanges()
{
	
	if ( m_state == stateInit || m_state == stateMenu
		|| m_state == stateChooseLevel || m_state == stateMazeEditor )
		return; 

	UndrawChangingObjects();
	DrawStrings();
	DrawChangingObjects();
}

void MyProjectMain::DrawScreen()
{
	
	CopyAllBackgroundBuffer();
	DrawStrings();

	if ( m_state == stateInit || m_state == stateMenu
		|| m_state == stateChooseLevel || m_state == stateGameOver
		|| m_state == stateMazeEditor)
		return; 

	DrawChangingObjects();
}



void MyProjectMain::loadMaze(string mazeFile)
{
	ifstream maze(mazeFile);

	if (!m_vsMaze.empty())
			m_vsMaze.clear();

	while (true) {
		string m;
		getline(maze,m);
		m_vsMaze.push_back(m);

		if (maze.fail()) break;
	}

}



void MyProjectMain::SetupNewGame()
{
	
	//next 2 lines reinitilise objects when new game starts
	m_ppDisplayableObjects[0] = NULL;
	InitialiseObjects();

	m_iDotsRemaining = 0;
	loadMaze(m_sMazeFile);
	int val;
	for ( int x = 0 ; x < 16 ; x++ )
		for ( int y = 0 ; y < 16 ; y++ ) {
			val = m_vsMaze[y][x]-48;
			 // fill with dots if not wall
			if (val != 1) {
				val = 3;
				m_iDotsRemaining++;
			}
			m_oTiles.SetValue( x, y, val );
			
			
		}

	//add power dot
	int x, y;
	x=y=1;
	while ( true ) {
		x = rand() % 15 + 1;
		y = rand() % 15 + 1;
		if ( m_oTiles.GetValue(x, y) == 3 ){ 
			m_oTiles.SetValue(x, y, 5);
			break;
		}
	}

	//add boost dot
	x=y=1;
	while ( true ) {
		x = rand() % 15 + 1;
		y = rand() % 15 + 1;
		if ( m_oTiles.GetValue(x, y) == 3 ){
			m_oTiles.SetValue(x, y, 4);
			break;
		}
	}

	//add big dot
	x=y=1;
	while ( true ) {
		x = rand() % 15 + 1;
		y = rand() % 15 + 1;
		if ( m_oTiles.GetValue(x, y) == 3 ){
			m_oTiles.SetValue(x, y, 6);
			m_iDotsRemaining += 7;
			break;
		}
	}

	m_iScore = 0;
	m_iLivesLeft = 3;
	m_bPowerDotActive = false; 

}


void MyProjectMain::KillPackman ()
{

	dynamic_cast<Packman*>(GetDisplayableObject(0))->ChangePosition();
	
	int i =1;
	while (true) {
		Ghost* o =  dynamic_cast<Ghost*>(this->GetDisplayableObject(i));
		if (o == NULL) break;
		o->ChangePosition();
		++i;	
	}
	UpdateAllObjects(GetTime());
	m_state = statePaused;
	m_iLivesLeft--;
	if (m_iLivesLeft < 1) {
		m_state = stateGameOver;
		SetupBackgroundBuffer();
	}
	Redraw(true);
}


void MyProjectMain::ActivatePowerDot()
{
	m_bPowerDotActive = true;
	m_iTimeToDisactivatePowerDot = this->GetTime() + 10e3/2;
}

void MyProjectMain::DecrementDotsRemaining(int d)
{
	m_iDotsRemaining -= d;
	if (m_iDotsRemaining == 0) {
		m_iScore += 10*m_iLivesLeft; // a bonus 
		m_state = stateGameOver;
		SetupBackgroundBuffer();
	}
}

void MyProjectMain::SetUpEditor(string fn)
{
	m_sMazeFile = fn; //this is needed for SaveMazeToFile
	loadMaze(m_sMazeFile);
	for ( int x = 0 ; x < 16 ; x++ )
		for ( int y = 0 ; y < 16 ; y++ ) 
			m_oTiles.SetValue( x, y, m_vsMaze[y][x]-48 );
}


void MyProjectMain::SaveMazeToFile()
{
	ofstream outf;
	outf.open(m_sMazeFile);

	for ( int i = 0; i != 16; ++i) {
		for (int j = 0; j != 16; ++j)
			outf << m_oTiles.GetValue(j,i);
		outf << endl;
	}
	outf.close();
}