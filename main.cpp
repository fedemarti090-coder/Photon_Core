#include <iostream>

#include "src/windowGPU.hpp"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    WinINIT app;

    if(!app.Run(hInstance, nCmdShow)){
        std::cout<<"Errore runtime vulkan";
        return -1;
    }

    return 0;
}