#ifndef SYSTEM_PARAMS_H
#define SYSTEM_PARAMS_H

#include "assert.h"
#include "errno.h"
#include "raylib.h"
#include "stdio.h"
#include "stdlib.h"
#include "sys/sysinfo.h"
#include <stdlib.h>
#include <string.h>
#include <sys/statvfs.h>

#define ulong unsigned long
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define TX_FILE "/sys/class/net/eth0/statistics/tx_bytes"
#define RX_FILE "/sys/class/net/eth0/statistics/rx_bytes"
#define HERE "."
#define MAX_MSG_LENGTH 256

#define MB (1024 * 1024)
#define GB (1024 * 1024) * 1024
#define DIVISOR_1024 1024

#define PVPARAM(param) param->genParam.paramVlong
#define VPARAM(param) param.genParam.paramVlong
#define DPARAM(param) param.genParam.paramDouble
#define IPARAM(param) param.genParam.paramInt
#define LPARAM(param) param.genParam.paramLong

#define CHECK_ERR(res)                                                                                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        if (res < 0)                                                                                                   \
        {                                                                                                              \
            fprintf(stderr, "Failed getting system info: %d", errno);                                                  \
        }                                                                                                              \
    } while (0)

/*---------------- STRUCTS ---------------*/

typedef struct
{
    int xloc;
    int yloc;
    int r;
} circle, *pcircle;

typedef struct
{
    char *msg;
    union
    {
        int paramInt;
        ulong long paramVlong;
        long paramLong;
        double paramDouble;
    } genParam;

    circle circ;
    int yMenuLoc;
    bool drawable;
} parameter, *pparameter;

typedef struct
{
    parameter numProc;
    parameter availPhysPages;
    parameter usedRam;
    parameter totalRam;
    parameter freeRam;
    parameter sharedRam;
    parameter numProcesses;
} info, *pinfo;

typedef struct
{
    // stored in GBs cause why not???
    parameter totalSpace;
    parameter freeSpace;
    parameter usedSpace;
} diskInfo, *pDiskInfo;

typedef struct
{
    parameter txBytes;
    parameter rxBytes;
} networkInfo, *pNetworkInfo;

typedef struct
{
    pDiskInfo diskInfo;
    pinfo generalInfo;
    pNetworkInfo network;
    circle **circlePtrArr;
    size_t countOfCircles;
} systemInfo, *pSystemInfo;

/*-------- METHOD DEFINITIONS ------------*/
pSystemInfo SetUpSystemInfo();

ulong GetNetWorkParamFromFile(char *fileToRead);

void GetNetworkInformation(pNetworkInfo n);

void GetSystemInformation(pinfo p);

void GetDiskSpaceInfo(pDiskInfo diskData);

// Retrieves and populates all system information.
void GetData(pSystemInfo allInfo);

#endif // SYSTEM_PARAMS_H