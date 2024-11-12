#pragma once
#include "core/blx_defines.h"

//THIS HEADER FILE IS SUBJECT TO BEING NAME CHANGED WHEN I FIND MORE USEFUL STUFF TO DO HERE.

typedef enum {
    BLX_FILE_MODE_READ = 0x1,
    BLX_FILE_MODE_WRITE = 0x2,
}blxFileMode;

typedef void blxFile;

//TODO: Move to platform folder, and make platform folder not INTERNAL!

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

BLXAPI uint64 blxFileGetSize(blxFile* handle);

BLXAPI blxBool blxFileExists(const char* path);

BLXAPI blxBool blxOpenFile(const char* path, blxFileMode fileMode, blxFile** outHandle);

BLXAPI blxBool blxFileReadLine(blxFile* handle, uint64 maxLength, char** lineBuffer, uint64* outLineLength);



