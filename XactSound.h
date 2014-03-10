#pragma once
#include "resource.h"
#include "stdafx.h"
#include <xact3.h>
class Game;
HRESULT PrepareXACT();
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
VOID CleanupXACT();
HRESULT FindMediaFileCch( WCHAR* strDestPath, int cchDest, LPCWSTR strFilename );
bool DoesCommandLineContainAuditionSwitch();
class XactSound 
{
	IXACT3Engine *pEngine;
	Game *game;
	private:
		HRESULT hr;
		IXACT3WaveBank* pWaveBank;
		IXACT3SoundBank* pSoundBank;
		
		
public:
	string str;
	XactSound(Game *initGame);
	~XactSound();
	void playSound(PCSTR cue);
	void stopSound(PCSTR cue);
	void doWork();

	HRESULT loadSoundProject(Game *game);
	HRESULT intiSound();
	//bool DoesCommandLineContainAuditionSwitch();
	//HRESULT FindMediaFileCch( WCHAR* strDestPath, int cchDest, LPCWSTR strFilename );
};