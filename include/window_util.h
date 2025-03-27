#ifndef WINDOW_UTIL_H
#define WINDOW_UTIL_H
#include "raylib.h"
#include "system_params.h"

/*--------- DEFINES ------------*/
#define MB_STR "MBS"
#define GB_STR "GBS"
#define XLOC_FPS 100
#define YLOC_FPS 100
#define X_LOCATION_LIST 250
#define MENU_MAX_LEN 256
#define FONT_SIZE 20

#define TO_MEGA_BT(bytes) ((bytes) / DIVISOR_1024 / DIVISOR_1024)

/*------------ METHODS -----------*/

void WriteParam(parameter param, char *subfix);

void WriteData(pSystemInfo allInfo);

#endif // WINDOW_UTIL_H