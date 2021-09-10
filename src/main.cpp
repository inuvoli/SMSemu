#include "SegaEmu.h"

constexpr auto SCREEN_WIDTH = 1024;
constexpr auto SCREEN_HEIGHT = 768;

int main(int argc, char* args[])
{
    SegaEmu emu;

    //Init SDL Platform
    if (!emu.InitSystem(SCREEN_WIDTH, SCREEN_HEIGHT))
    {
        printf("Failed to Initialize SDL!\n");
        return 0;
    }

    //Init Emulator
    if (!emu.InitEmulator(SEGAMASTERSYSTEM, "./ROM/Wonder Boy 3 - The Dragon's Trap (UE) [T+Swe0.7b_Metalhead].sms", UE))
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