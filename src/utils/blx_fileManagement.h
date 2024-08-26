#pragma once
#include "../Defines.h"

//THIS HEADER FILE IS SUBJECT TO BEING NAME CHANGED WHEN I FIND MORE USEFUL STUFF TO DO HERE.

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