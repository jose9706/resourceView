#include "system_params.h"
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

void SetMsg(char *msgToSet, char **placeToStore)
{
    *placeToStore = (char *)malloc(MAX_MSG_LENGTH * sizeof(char));
    assert(placeToStore != NULL);
    strcpy(*placeToStore, msgToSet);
}

pSystemInfo SetUpSystemInfo()
{
    pSystemInfo sysInfo = calloc(1, sizeof(systemInfo));
    assert(sysInfo);
    sysInfo->diskInfo = calloc(1, sizeof(diskInfo));
    assert(sysInfo->diskInfo);
    SetMsg("Free space", &sysInfo->diskInfo->freeSpace.msg);
    arrput(sysInfo->circlePtrArr, &sysInfo->diskInfo->freeSpace.circ);
    sysInfo->diskInfo->freeSpace.yMenuLoc = 200;
    SetMsg("Total space", &sysInfo->diskInfo->totalSpace.msg);
    sysInfo->diskInfo->totalSpace.yMenuLoc = 220;
    arrput(sysInfo->circlePtrArr, &sysInfo->diskInfo->totalSpace.circ);
    SetMsg("Used space", &sysInfo->diskInfo->usedSpace.msg);
    sysInfo->diskInfo->usedSpace.yMenuLoc = 240;
    arrput(sysInfo->circlePtrArr, &sysInfo->diskInfo->usedSpace.circ);
    sysInfo->generalInfo = calloc(1, sizeof(info));
    assert(sysInfo->generalInfo);
    SetMsg("Number of Processes", &sysInfo->generalInfo->numProcesses.msg);
    sysInfo->generalInfo->numProcesses.yMenuLoc = 260;
    arrput(sysInfo->circlePtrArr, &sysInfo->generalInfo->numProcesses.circ);
    SetMsg("Number of Processors", &sysInfo->generalInfo->numProc.msg);
    sysInfo->generalInfo->numProc.yMenuLoc = 280;
    arrput(sysInfo->circlePtrArr, &sysInfo->generalInfo->numProc.circ);
    SetMsg("Used RAM", &sysInfo->generalInfo->usedRam.msg);
    sysInfo->generalInfo->usedRam.yMenuLoc = 300;
    arrput(sysInfo->circlePtrArr, &sysInfo->generalInfo->usedRam.circ);
    SetMsg("Free RAM", &sysInfo->generalInfo->freeRam.msg);
    sysInfo->generalInfo->freeRam.yMenuLoc = 360;
    arrput(sysInfo->circlePtrArr, &sysInfo->generalInfo->freeRam.circ);
    SetMsg("Total RAM", &sysInfo->generalInfo->totalRam.msg);
    sysInfo->generalInfo->totalRam.yMenuLoc = 380;
    arrput(sysInfo->circlePtrArr, &sysInfo->generalInfo->totalRam.circ);
    SetMsg("Shared RAM", &sysInfo->generalInfo->sharedRam.msg);
    sysInfo->generalInfo->sharedRam.yMenuLoc = 400;
    arrput(sysInfo->circlePtrArr, &sysInfo->generalInfo->sharedRam.circ);
    SetMsg("Physical pages available", &sysInfo->generalInfo->availPhysPages.msg);
    sysInfo->generalInfo->availPhysPages.yMenuLoc = 420;
    arrput(sysInfo->circlePtrArr, &sysInfo->generalInfo->availPhysPages.circ);
    sysInfo->network = calloc(1, sizeof(networkInfo));
    assert(sysInfo->network);
    SetMsg("Megabytes out", &sysInfo->network->rxBytes.msg);
    arrput(sysInfo->circlePtrArr, &sysInfo->network->rxBytes.circ);
    SetMsg("Megabytes in", &sysInfo->network->txBytes.msg);
    sysInfo->network->rxBytes.yMenuLoc = 440;
    arrput(sysInfo->circlePtrArr, &sysInfo->network->txBytes.circ);
    sysInfo->network->txBytes.yMenuLoc = 460;
    return sysInfo;
}

ulong GetNetWorkParamFromFile(char *fileToRead)
{
    FILE *f = fopen(fileToRead, "r");
    assert(f);
    char buffer[50];
    size_t ret = fread(buffer, sizeof(*buffer), ARRAY_SIZE(buffer), f);
    if (ret > ARRAY_SIZE(buffer))
    {
        fprintf(stderr, "We read too much, might buffer overflow and thats bad!! Buffer size=[%lu], read bytes=[%lu]",
                ARRAY_SIZE(buffer), ret);
    }

    char *endptr;
    ulong bytes = strtoll(buffer, &endptr, 10);
    if (*endptr != '\0' && *endptr != '\n')
    {
        // means invalid.
        fprintf(stderr, "Failed to format bytes into a double!! Failing part: %s", endptr);
        bytes = 0;
    }

    fclose(f);
    return bytes;
}

void GetNetworkInformation(pNetworkInfo n)
{
    n->rxBytes.genParam.paramVlong = GetNetWorkParamFromFile(RX_FILE) / MB;
    n->txBytes.genParam.paramVlong = GetNetWorkParamFromFile(TX_FILE) / MB;
}

void GetSystemInformation(pinfo p)
{
    assert(p);
    struct sysinfo s;
    int res = sysinfo(&s);
    CHECK_ERR(res);
    IPARAM(p->numProc) = get_nprocs();
    LPARAM(p->availPhysPages) = get_avphys_pages();
    VPARAM(p->usedRam) = (s.totalram - s.freeram) / MB;
    VPARAM(p->freeRam) = s.freeram / MB;
    VPARAM(p->totalRam) = s.totalram / MB;
    VPARAM(p->sharedRam) = s.sharedram / MB;
}

void GetDiskSpaceInfo(pDiskInfo diskData)
{
    struct statvfs apiDiskData;
    // we want diskspace here so.
    int res = statvfs(HERE, &apiDiskData);
    CHECK_ERR(res);
    VPARAM(diskData->totalSpace) = (apiDiskData.f_blocks * apiDiskData.f_frsize) / (GB);
    VPARAM(diskData->freeSpace) = (apiDiskData.f_bfree * apiDiskData.f_frsize) / (GB);
    VPARAM(diskData->usedSpace) = VPARAM(diskData->totalSpace) - VPARAM(diskData->freeSpace);
}

void GetData(pSystemInfo allInfo)
{
    pDiskInfo diskData = allInfo->diskInfo;
    pinfo generalInfo = allInfo->generalInfo;
    pNetworkInfo network = allInfo->network;
    GetSystemInformation(generalInfo);
    GetDiskSpaceInfo(diskData);
    GetNetworkInformation(network);
}