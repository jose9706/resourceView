#include "window_util.h"
#include "raylib.h"
#include "system_params.h"

void DrawMenuCircle(int xloc, int yloc, circle *circ)
{
    circ->xloc = xloc + 20;
    circ->r = FONT_SIZE / 2;
    circ->yloc = yloc + 10;
    DrawCircleLines(circ->xloc, circ->yloc, (float)circ->r, WHITE);
}

void WriteParam(parameter param, char *subfix)
{
    char formattedMsg[MENU_MAX_LEN];
    assert(param.msg != NULL);
    if (subfix != NULL)
    {
        snprintf(formattedMsg, MENU_MAX_LEN, "%s : %llu %s", param.msg, VPARAM(param), subfix);
    }
    else
    {
        snprintf(formattedMsg, MENU_MAX_LEN, "%s : %llu", param.msg, VPARAM(param));
    }
    DrawText(formattedMsg, X_LOCATION_LIST, param.yMenuLoc, FONT_SIZE, WHITE);
    int xPixelOffset = MeasureText(formattedMsg, FONT_SIZE);
    DrawMenuCircle(X_LOCATION_LIST + xPixelOffset, param.yMenuLoc, &param.circ);
}

void WriteProgressBar(parameter usedParam, parameter totalValue, char *msg, int yloc, Color barColor)
{
    char formattedMsg[300];
    int progressBarWidth = 200;
    int progressBarHeight = 20;
    double progress = ((double)(usedParam.genParam.paramVlong) * progressBarWidth) / totalValue.genParam.paramVlong;
    DrawRectangle(X_LOCATION_LIST, yloc, progressBarWidth, progressBarHeight, GRAY);
    DrawRectangle(X_LOCATION_LIST, yloc, progress, progressBarHeight, barColor);
    snprintf(formattedMsg, sizeof(formattedMsg), "%s: %.2f%%", msg, (progress * 100) / progressBarWidth);
    DrawText(formattedMsg, X_LOCATION_LIST, yloc + FONT_SIZE, FONT_SIZE, GOLD);
}

void WriteData(pSystemInfo allInfo)
{

    pDiskInfo diskData = allInfo->diskInfo;
    pinfo generalInfo = allInfo->generalInfo;
    pNetworkInfo network = allInfo->network;
    WriteParam(generalInfo->totalRam, MB_STR);
    WriteParam(generalInfo->freeRam, MB_STR);
    WriteParam(generalInfo->sharedRam, MB_STR);
    WriteParam(generalInfo->usedRam, MB_STR);
    WriteParam(generalInfo->numProc, NULL);
    WriteParam(generalInfo->numProcesses, NULL);
    WriteParam(generalInfo->availPhysPages, NULL);
    WriteProgressBar(generalInfo->usedRam, generalInfo->totalRam, "Memory usage", generalInfo->usedRam.yMenuLoc + 20,
                     GREEN);
    WriteParam(diskData->totalSpace, GB_STR);
    WriteParam(diskData->freeSpace, GB_STR);
    WriteParam(diskData->usedSpace, GB_STR);
    // WriteProgressBar(diskData->usedSpace, diskData->totalSpace, "Used space", X_LOCATION_LIST, 460, PINK);
    WriteParam(network->txBytes, MB_STR);
    WriteParam(network->rxBytes, MB_STR);
}
