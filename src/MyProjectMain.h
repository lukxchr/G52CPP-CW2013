#ifndef MYPROJECTMAIN_H
#define MYPROJECTMAIN_H

#include "BaseEngine.h"
#include "TileManager.h"
#include "GameTileManager.h"
#include "JPGImage.h"

#include <vector>
#include <string>
#include <stdlib.h>


class MyProjectMain :
	public BaseEngine
{
public:
	MyProjectMain(void);
	~MyProjectMain(void);

	void SetupBackgroundBuffer();
	int InitialiseObjects();
	void DrawStrings();
	void GameAction();
	void MouseDown( int iButton, int iX, int iY );
	void KeyDown(int iKeyCode);
	GameTileManager& GetTileManager() { return m_oTiles; }

private:
	GameTileManager m_oTiles;

public:
	enum State { stateInit, stateMenu, stateChooseLevel, stateMain, statePaused,
				 stateGameOver, stateMazeEditor };
	void DrawScreen();
	void DrawChanges();
	void loadMaze(std::string mazeFile);
	void SetupNewGame(); //performs some initial setup e.g. loads maze from file
	void SetUpEditor(std::string fn="mazes/01.txt"); //used in stateMazeEditor
	void IncrementScore(int d=1) { m_iScore += d; } //+1 by default 
	void KillPackman();
	void ActivatePowerDot();
	bool isPowerDotActive() { return m_bPowerDotActive; }
	void DecrementDotsRemaining(int d=1); //in addition checks if it's time to end the game (i.e. if m_iDotsRemaining == 0)

private:
	State m_state;
	std::vector<std::string> m_vsMaze;
	std::string m_sMazeFile;
	int m_iScore;
	char m_ScoreBuffer[20];  //used to store m_iScore converted to cstring
	int m_iLivesLeft;
	char m_iLivesLeftBuffer[3];  //used to store m_iLivesLeft converted to cstring
	bool m_bPowerDotActive;
	int m_iTimeToDisactivatePowerDot;
	int m_iDotsRemaining; //used to end game if the player's got all dots, it's needed because m_iScore depends not only on # of dots collected but also on # of ghosts killed
	ImageData m_oBackgroundImg;
	ImageData m_oLogoImg;
	void SaveMazeToFile();



};




#endif
