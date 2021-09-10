#include "SegaEmu.h"

constexpr auto SCREEN_WIDTH = 1024;
constexpr auto SCREEN_HEIGHT = 768;

int main(int argc, char* argv[])
{
    SegaEmu emu;

    //Init SDL Platform
    if (!emu.InitSystem(SCREEN_WIDTH, SCREEN_HEIGHT))
    {
        printf("Failed to Initialize SDL!\n");
        return 0;
    }

    //Init Emulator
    if (!emu.InitEmulator(SEGAMASTERSYSTEM, argv[1], atoi(argv[2])))
    {
        printf("Failed to Initialize Emulator!\n");
        return 0;
    }
    
    while (emu.m_bRunning)
    {
        //Manage SDL Events
        emu.HandleEvents();

        //Render NewFrame at given Frame Rate
        if (emu.NewFrame())
            emu.RenderFrame();
    }

    emu.Close();
    
    return 0;
}