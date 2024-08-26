#include "blx_fileManagement.h"
#include "internal/platform/platform.h"

blxBool blxOpenFilePanel(const char* title, const char* defDirectory, const char* extension, char* buffer)
{
    return PlatformOpenFilePanel(title, defDirectory, extension, buffer);
}