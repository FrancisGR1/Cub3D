#include "cub3d.h"

int main(void)
{
    logger_initConsoleLogger(stderr);
    logger_setLevel(LOG_LEVEL);
    LOG_INFO("console logging");
    int i = 324;
    LOG_WARN("TEST: %d", i);
    LOG_FATAL("TEST: %d", i);
    return 0;
}
