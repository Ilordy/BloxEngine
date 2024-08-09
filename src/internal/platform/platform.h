#pragma once
#include "Defines.h"
#include "blx_rendering.h"

typedef struct {
    void* internalState;
}platformState;


blxBool PlatformInit(platformState* platform, const char* appName, unsigned short width, unsigned short height, GraphicsAPI graphicsAPI);
void PlatformShutDown(platformState* platform);
blxBool PlatformPumpMessages(platformState* platform);
void PlatformPollEvents();
void PlatformWriteToConsole(const char* msg, uint8_t color);
void PlatformWriteToConsoleError(const char* msg);
double PlatformGetTime();
void* PlatformAllocate(unsigned long long size);
void* PlatformMemSetZero(void* block, unsigned long long size);
void PlatformFreeMemory(void* block);
void* PlatformMemCpy(void* dest, const void* src, unsigned long long size);



