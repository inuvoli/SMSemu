#include "SegaEmu.h"

SegaEmu::SegaEmu()
{
    m_bRunning = false;
    
    m_pWindow = NULL;
    m_pScreenSurface = NULL;
    m_pOverlay = NULL;
    m_pFrameBuffer = NULL;
        
    m_pControllerA = NULL;
    m_pControllerB = NULL;
}

SegaEmu::~SegaEmu()
{
}

bool SegaEmu::InitSystem(int scrWidth, int scrHeight)
{
    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    else
    {
        printf("SDL Initialized...\n");

        //Create Controller
        printf("Scanning Controllers....detected [%d]\n", SDL_NumJoysticks());

        for (int i = 0; i < SDL_NumJoysticks(); i++)
        {
            if (SDL_IsGameController(i))
            {
                printf("Configuring Controller [%d]\n", i+1);
                m_pControllerA = SDL_GameControllerOpen(i);
            }
                
        }

        //Create Window
        m_pWindow = SDL_CreateWindow("SegaEmu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, scrWidth, scrHeight, SDL_WINDOW_SHOWN);
        if (m_pWindow == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            return false;
        }
        else
        {
            printf("Window created...\n");
            //Get window surface
            m_pScreenSurface = SDL_GetWindowSurface(m_pWindow);
        }
    }

    //Init Audio Device
    int nDeviceCount = SDL_GetNumAudioDevices(0);
    printf("Scanning Audio Devices....detected [%d]\n", nDeviceCount);
    for (int i = 0; i < nDeviceCount; i++)
    {
        printf("Audio Device [%d]: %s\n", i, SDL_GetAudioDeviceName(i, 0));
    }

    SDL_AudioSpec want, have;
    SDL_memset(&want, 0, sizeof(want));
    want.freq = 44100;
    want.format = AUDIO_F32;
    want.channels = 1;
    want.samples = 1024;
    want.callback = NULL;

    m_AudioDevice = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(0, 0), 0, &want, &have, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
    if (m_AudioDevice == 0)
    {
        printf("Failed to initialize Audio Device! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        printf("Audio Device ID [%d] Initialized [Buffer Size: %d, Freq: %d]\n", m_AudioDevice, have.size, have.freq);
    }
    SDL_PauseAudioDevice(m_AudioDevice, 0);

    //Init Timer for Frame Rate Capping
    m_tp1 = std::chrono::system_clock::now();
    m_tp2 = std::chrono::system_clock::now();

    m_bRunning = true;

    return true;
}

bool SegaEmu::InitEmulator(int nPlatform, const std::string& sFileName, int nRegion)
{
    switch (nPlatform)
    {
    case SEGAMASTERSYSTEM:
        m_nActivePlatform = SEGAMASTERSYSTEM;
        sms = new SMS(sFileName, SEGA, nRegion);
        m_pFrameBuffer = SDL_CreateRGBSurface(0, 256, 192, 32, 0, 0, 0, 0);
        if (nRegion == JP || nRegion == US)
        {
            SetFPS(60);
        }
        else
        {
            SetFPS(50);
        }

        SDL_ClearQueuedAudio(m_AudioDevice);
        break;
    
    case SEGAMEGADRIVE:
        m_nActivePlatform = SEGAMEGADRIVE;

        break;
    }

    return true;
}

bool SegaEmu::SetFPS(int nFPS)
{
    m_fFrameDuration = 1.0f / (float)nFPS;

    return true;
}

bool SegaEmu::HandleEvents()
{
    //Handle events on queue
    while (SDL_PollEvent(&m_Event) != 0)
    {
        //User requests quit
        if (m_Event.type == SDL_QUIT)
        {
            m_bRunning = false;
        }
 
        //Scan Keyboard Pressed
        else if (m_Event.type == SDL_KEYDOWN)
        {
            //Select surfaces based on key press
            switch (m_Event.key.keysym.sym)
            {
            case SDLK_UP:
                sms->cnt.portA &= 0xfe;
                break;
            case SDLK_DOWN:
                sms->cnt.portA &= 0xfd;
                break;
            case SDLK_LEFT:
                sms->cnt.portA &= 0xfb;
                break;
            case SDLK_RIGHT:
                sms->cnt.portA &= 0xf7;
                break;
            case SDLK_z:
                sms->cnt.portA &= 0xef;
                break;
            case SDLK_x:
                sms->cnt.portA &= 0xdf;
                break;
            }
        }

        //Scan Keyboard Released
        else if (m_Event.type == SDL_KEYUP)
        {
            //Select surfaces based on key press
            switch (m_Event.key.keysym.sym)
            {
            case SDLK_UP:
                sms->cnt.portA |= 0x01;
                break;
            case SDLK_DOWN:
                sms->cnt.portA |= 0x02;
                break;
            case SDLK_LEFT:
                sms->cnt.portA |= 0x04;
                break;
            case SDLK_RIGHT:
                sms->cnt.portA |= 0x08;
                break;
            case SDLK_z:
                sms->cnt.portA |= 0x10;
                break;
            case SDLK_x:
                sms->cnt.portA |= 0x20;
                break;
            }
        }

        //Scan Controller Pressed
        else if (m_Event.type == SDL_CONTROLLERBUTTONDOWN)
        {
            //Check Wich Button
            switch (m_Event.cbutton.button)
            {
            case SDL_CONTROLLER_BUTTON_A:
                sms->cnt.portA &= 0xef;
                break;
            case SDL_CONTROLLER_BUTTON_B:
                sms->cnt.portA &= 0xdf;
                break;
            case SDL_CONTROLLER_BUTTON_X:
                break;
            case SDL_CONTROLLER_BUTTON_Y:
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_UP:
                sms->cnt.portA &= 0xfe;
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                sms->cnt.portA &= 0xfd;
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                sms->cnt.portA &= 0xfb;
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                sms->cnt.portA &= 0xf7;
                break;
            }
        }

        //Scan Controller Released
        else if (m_Event.type == SDL_CONTROLLERBUTTONUP)
        {
            //Check Wich Button
            switch (m_Event.cbutton.button)
            {
            case SDL_CONTROLLER_BUTTON_A:
                sms->cnt.portA |= 0x10;
                break;
            case SDL_CONTROLLER_BUTTON_B:
                sms->cnt.portA |= 0x20;
                break;
            case SDL_CONTROLLER_BUTTON_X:
                break;
            case SDL_CONTROLLER_BUTTON_Y:
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_UP:
                sms->cnt.portA |= 0x01;
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                sms->cnt.portA |= 0x02;
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                sms->cnt.portA |= 0x04;
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                sms->cnt.portA |= 0x08;
                break;
            }
        }
    }

    return true;
}

bool SegaEmu::NewFrame()
{
    static float fElapsedTime;

    m_tp2 = std::chrono::system_clock::now();
    std::chrono::duration<float> diff = m_tp2 - m_tp1;
    m_tp1 = m_tp2;

    fElapsedTime += diff.count();

    if (fElapsedTime >= m_fFrameDuration)
    {
        sms->NewFrame();
        fElapsedTime = 0.0f;

        //Get Audio Samples per Frame
        m_nSamplePerFrame = sms->psg.GetSamplePerFrame();
        //printf("Samples per Frame: %d\n", m_nSamplePerFrame);

        for (int i = 0; i < m_nSamplePerFrame; i++)
            m_fAudioBuffer[i] = sms->psg.GetSample();

        //printf("Audio Samples Still Queued in Bytes: %d\n", SDL_GetQueuedAudioSize(m_AudioDevice));
    
        if (SDL_QueueAudio(m_AudioDevice, m_fAudioBuffer, m_nSamplePerFrame * 4) < 0)
        {
            printf("Error Queueing Audio Samples: %s\n", SDL_GetError());
        }
        return true;
    }
    
    return false;
}

bool SegaEmu::RenderFrame()
{
    //Copy FrameBuffer to ScreenSurface
    if (SDL_LockSurface(m_pFrameBuffer) != 0)
        printf("Error while locking ScreeSurface: %s\n", SDL_GetError());

    SDL_memcpy4(m_pFrameBuffer->pixels, sms->vdp.GetScreen(), sms->vdp.GetScreenWidth() * sms->vdp.GetScreenHeight());
    
    SDL_UnlockSurface(m_pFrameBuffer);
    
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
 
    SDL_GetWindowSize(m_pWindow, &rect.w, &rect.h);
    
    SDL_BlitScaled(m_pFrameBuffer, NULL, m_pScreenSurface, &rect);

    //Update the Windows
    SDL_UpdateWindowSurface(m_pWindow);

    return false;
}

void SegaEmu::Close()
{
    //Close Controllers

    SDL_GameControllerClose(m_pControllerA);
    m_pControllerA = NULL;

    //Free Additional Surface;
    SDL_FreeSurface(m_pOverlay);
    m_pOverlay = NULL;

    //Destroy window
    SDL_DestroyWindow(m_pWindow);

    //Close Audio
    SDL_CloseAudioDevice(m_AudioDevice);

    //Quit SDL subsystems
    SDL_Quit();
}

//void SegaEmu::MyAudioCallback(void* userdata, Uint8* stream, int len)
//{
//    int _len = len / 4; //float is for byte
//    float* _stream = (float*)stream;
//    SegaEmu* pInst = (SegaEmu*)userdata;
//
//    printf("CallBack Audio: stream %d, samples %d", _stream, _len);
//
//    //Get Samples
//    for (int i = 0; i < _len; i++)
//    {
//        _stream[i] = pInst->sms->psg.GetSample();
//    }
//}
