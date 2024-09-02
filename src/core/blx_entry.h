#include "blx_application.h"
#include "blx_logger.h"
#include "blx_gameType.h"
#include "blx_memory.h"

extern blxBool blxCreateGame(blxGameInstance* outGame);

int main(void)
{
    _blxInitMemory();
    blxGameInstance gameInstance;
    if (!blxCreateGame(&gameInstance)) {
        BLXERROR("Creating Game Failed!");
        return -1;
    }

    if (!gameInstance.Init || !gameInstance.Update || !gameInstance.Render) {
        BLXERROR("A function pointer has not been assinged yet.");
        return -2;
    }

    if (!blxCreateApplication(&gameInstance)) {
        BLXINFO("Application failed to create!");
        return 1;
    }

    if (!blxRunApplication()) {
        BLXINFO("Application did not shut down correctly.");
        return 2;
    }

    _blxShutDownMemory();
    return 0;
}