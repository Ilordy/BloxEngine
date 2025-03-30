#include "blx_fileManagement.h"
#include "internal/platform/platform.h"
#include "core/blx_logger.h"
#include <sys/stat.h>
#include "core/blx_string.h"
#include "utils/blx_assertions.h"
#include "core/blx_memory.h"


blxBool blxOpenFilePanel(const char* title, const char* defDirectory, const char* extension, char* buffer)
{
    return blxPlatform_OpenFilePanel(title, defDirectory, extension, buffer);
}

blxBool blxFileExists(const char* path)
{
#ifdef _MSC_VER
    struct _stat buffer;
    return _stat(path, &buffer) == 0;
#else
    struct stat buffer;
    return stat(path, &buffer) == 0;
#endif
}

blxBool blxOpenFile(const char* path, blxFileMode fileMode, blxFile** outHandle)
{
    const char* fileModeStr;
    if ((fileMode & BLX_FILE_MODE_READ) != 0 && (fileMode & BLX_FILE_MODE_WRITE) != 0) {
        fileModeStr = "w+";
    }
    else if ((fileMode & BLX_FILE_MODE_READ) != 0 && (fileMode & BLX_FILE_MODE_WRITE) == 0) {
        fileModeStr = "r";
    }
    else if ((fileMode & BLX_FILE_MODE_WRITE) != 0 && (fileMode & BLX_FILE_MODE_READ) == 0) {
        fileModeStr = "w";
    }
    else {
        BLXERROR("Incorrect blxFileMode passed while trying to open file: %s", path);
        return BLX_FALSE;
    }

    FILE* file = fopen(path, fileModeStr);
    if (!file) {
        BLXERROR("Error opening file: %s\n Error Reason: %s", path, strerror(errno));
        return BLX_FALSE;
    }

    *outHandle = file;
    return BLX_TRUE;
}

void blxCloseFile(blxFile* file)
{
    if (file) {
        fclose((FILE*)file);
        file = NULL;
    }
}


uint64 blxFileGetSize(blxFile* handle)
{
    BLXASSERT(handle != NULL);

    uint64 size;
    fseek((FILE*)handle, 0, SEEK_END);
    size = ftell((FILE*)handle);
    rewind((FILE*)handle);
    return size;
}

char* blxFileReadAllText(blxFile* handle, uint64* outBufSize)
{
    BLXASSERT(handle != NULL);

    uint64 size = blxFileGetSize(handle);
    char* buffer = blxAllocate(size, BLXMEMORY_TAG_STRING);
    *outBufSize = fread(buffer, sizeof(char), size, (FILE*)handle);
    return buffer;
}

blxBool blxFileReadLine(blxFile* handle, uint64 maxLength, char** lineBuffer, uint64* outLineLength)
{
    BLXASSERT(handle != NULL);
    BLXASSERT(lineBuffer != NULL);
    BLXASSERT(maxLength > 0);

    char* buf = *lineBuffer;
    if (fgets(buf, maxLength, (FILE*)handle)) {
        *outLineLength = blxStrLen(*lineBuffer);
        return BLX_TRUE;
    }
    return BLX_FALSE;
}
