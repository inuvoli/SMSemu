#pragma once

#include <cstdint>
#include <cstdio>
#include <SDL.h>

#include "Emuconst.h"
#include "SMS.h"

//Emulator Class Definition
class SegaEmu
{
public:
	SegaEmu();
	~SegaEmu();

	SMS *sms;

	bool InitSystem(int scrWidth, int scrHeight);
	bool InitEmulator(int nPlatform, const std::string& sFileName, int nRegion);
	bool SetFPS(int nFPS);
	bool HandleEvents();
	bool NewFrame();
	bool RenderFrame();
	void Close();
		
//	static void MyAudioCallback(void* userdata, Uint8* stream, int len);

private:
	int m_nActivePlatform;

	SDL_Event	m_Event;
	SDL_Window* m_pWindow;
	SDL_AudioDeviceID	m_AudioDevice;

	SDL_Surface* m_pScreenSurface;
	SDL_Surface* m_pFrameBuffer;
	SDL_Surface* m_pOverlay;

	//Controllers
	SDL_GameController* m_pControllerA;
	SDL_GameController* m_pControllerB;

	//Audio & Video Frame Timing
	float m_fFrameDuration;
	std::chrono::time_point<std::chrono::system_clock> m_tp1, m_tp2;
	
	int m_nSamplePerFrame;
	float m_fAudioBuffer[1024];

public:
	bool m_bRunning;
};

