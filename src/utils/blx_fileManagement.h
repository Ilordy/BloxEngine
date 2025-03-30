#pragma once
#include "core/blx_defines.h"

/// @brief File modes for opening files (flags).
typedef enum {
    BLX_FILE_MODE_READ = 0x1,
    BLX_FILE_MODE_WRITE = 0x2,
}blxFileMode;

typedef void blxFile;

// TODO: Add macros for other platforms.
#define blxMaxFilePath 0
#ifdef BLXWIN32
#define blxMaxFilePath 260
#endif

/// @brief Displays the Platform specific dialog box for opening a file.
/// @param title The title of the dialog box.
/// @param defDirectory The default directory we want out dialog box to start in (can be NULL)
/// @param extension Extension of the file type we are looking for.
/// @param buffer If a valid file was seletected this buffer will store the path to the file.
/// @return True if a valid file was opened, false other wise.
BLXAPI blxBool blxOpenFilePanel(const char* title, const char* defDirectory, const char* extension, char* buffer);

/// @brief Reads all Text of a given file.
/// @param handle Handle to the file to read. 
/// @param outBufSize Out Buffer Size of the file (Use this to index through the returned buffer).
/// @return A buffer allocated on the heap MUST BE FREED BY USER! (use: blxFree(buffer, BLXMEMORY_TAG_STRING))
BLXAPI char* blxFileReadAllText(blxFile* handle, uint64* outBufSize);

/// @brief Gets the size of a file (only works when reading file in binary mode.)
/// @param handle The file handle to get the size of.
/// @return The size of the file in bytes.
BLXAPI uint64 blxFileGetSize(blxFile* handle);

/// @brief Checks if a file exists.
/// @param path The path to the file.
/// @return True if the file exists, false otherwise.
BLXAPI blxBool blxFileExists(const char* path);

/// @brief Opens a file.
/// @param path Path to the file.
/// @param fileMode Mode to open the file in.
/// @param outHandle A pointer to the file handle.
/// @return True if the file was opened successfully, false otherwise.
BLXAPI blxBool blxOpenFile(const char* path, blxFileMode fileMode, blxFile** outHandle);

/// @brief Reads a line from a file (depending if reading in bytes or text different outcome can happen).
/// @param handle A handle to the file to read from.
/// @param maxLength Max length of the line to read or till a newline character is reached if reading text.
/// @param lineBuffer A pointer to the buffer to store the line in.
/// @param outLineLength Length of the line read.
/// @return True if a line was read, false otherwise.
BLXAPI blxBool blxFileReadLine(blxFile* handle, uint64 maxLength, char** lineBuffer, uint64* outLineLength);

/// @brief Closes a file (File will not be able to be interacted with anymore after).
/// @param file The file to close.
BLXAPI void blxCloseFile(blxFile* file);





