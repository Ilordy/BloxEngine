#pragma once
#include "core/blx_defines.h"
#include "rendering/blx_rendering.h"

typedef struct {
    void* internalState;
}platformState;

/// @brief Initializes the platform.
/// @param platform The platform state to initialize.
/// @param appName Name of the application.
/// @param width Initial width of the window.
/// @param height Initial height of the window.
/// @param graphicsAPI Initial graphics API to use.
/// @return True if the platform was initialized successfully, false otherwise.
blxBool blxPlatform_Init(platformState* platform, const char* appName, unsigned short width, unsigned short height, GraphicsAPI graphicsAPI);

/// @brief Shuts down the platform and releases any allocated resources.
/// @param platform The platform state to shut down.
void blxPlatform_ShutDown(platformState* platform);

/// @brief Processes platform-specific messages (e.g., window events).
/// @param platform The platform state.
/// @return True if the application should continue running, false if it should exit.
blxBool blxPlatform_PumpMessages(platformState* platform);

/// @brief Writes a message to the console with the specified color.
/// @param msg The message to write.
/// @param color The color to use for the message.
void blxPlatform_WriteToConsole(const char* msg, uint8_t color);

/// @brief Writes an error message to the console.
/// @param msg The error message to write.
void blxPlatform_WriteToConsoleError(const char* msg);

/// @brief Retrieves the current time in seconds since the application started.
/// @return The current time in seconds.
double blxPlatform_GetTime();

/// @brief Allocates a block of memory of the specified size.
/// @param size The size of the memory block to allocate.
/// @return A pointer to the allocated memory block, or NULL if allocation fails.
void* blxPlatform_Allocate(unsigned long long size);

/// @brief Sets a block of memory to zero.
/// @param block The memory block to set to zero.
/// @param size The size of the memory block.
/// @return A pointer to the memory block.
void* blxPlatform_MemSetZero(void* block, unsigned long long size);

/// @brief Frees a previously allocated block of memory.
/// @param block The memory block to free.
void blxPlatform_FreeMemory(void* block);

/// @brief Copies memory from one location to another.
/// @param dest The destination memory block.
/// @param src The source memory block.
/// @param size The number of bytes to copy.
/// @return A pointer to the destination memory block.
void* blxPlatform_MemCpy(void* dest, const void* src, unsigned long long size);

/// @brief Suspends the execution of the current thread for the specified duration.
/// @param ms The duration to sleep, in milliseconds.
void blxPlatform_Sleep(uint64 ms);

/// @brief Opens a file selection dialog.
/// @param title The title of the dialog.
/// @param defDirectory The default directory to open.
/// @param extension The file extension filter.
/// @param buffer A buffer to store the selected file path.
/// @return True if a file was selected, false otherwise.
blxBool blxPlatform_OpenFilePanel(const char* title, const char* defDirectory, const char* extension, char* buffer);

/// @brief Swaps the front and back buffers for rendering.
/// This is typically used to present the rendered frame to the screen.
void blxPlatform_SwapBuffers();



