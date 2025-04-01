#include "raylib.h"
#include "resource_dir.h" // utility header for SearchAndSetResourceDir
#include "system_params.h"
#include "window_util.h"
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <unistd.h>

// Funny trick to get the paramter/structure that contains a circle.
#define CONTAINER_OF(ptr, type, member) ((type *)((char *)ptr - offsetof(type, member)))

pSystemInfo allInfo;
pthread_mutex_t lock;

void HandleMouseEvent(pSystemInfo sInfo)
{
    Vector2 point;
    point.x = GetMouseX();
    point.y = GetMouseY();
    assert(sInfo->circlePtrArr != NULL);
    pcircle *circles = sInfo->circlePtrArr;
    for (size_t i = 0; i < sInfo->countOfCircles; i++)
    {
        pcircle circ = circles[i];
        Vector2 circCenter = {.x = circ->xloc, .y = circ->yloc};
        if (CheckCollisionPointCircle(point, circCenter, circ->r))
        {
            parameter *p = CONTAINER_OF(circ, parameter, circ);
            p->drawable = p->drawable == YES_DRAW ? NO_DRAW : YES_DRAW;
        }
    }
}

void *DataGatheringThread(void *args)
{
    while (1)
    {
        GetData(allInfo);
        usleep(300000);
    }
    return NULL;
}

int main()
{
    // Tell the window to use vsync and work on high DPI displays
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    // Create the window and OpenGL context
    InitWindow(1280, 800, "Processor count");
    // Utility function from resource_dir.h to find the resources folder and set
    // it as the current working directory so we can load from it
    SearchAndSetResourceDir("resources");

    // Load a texture from the resources directory
    Texture wabbit = LoadTexture("wabbit_alpha.png");
    allInfo = SetUpSystemInfo();
    pthread_mutex_init(&lock, NULL);
    pthread_t myThread;
    pthread_create(&myThread, NULL, DataGatheringThread, NULL);
    pthread_detach(myThread);
    // game loop
    while (!WindowShouldClose()) // run the loop untill the user presses ESCAPE
                                 // or presses the Close button on the window
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            HandleMouseEvent(allInfo);
        }
        BeginDrawing();
        ClearBackground(BLACK);
        DrawFPS(XLOC_FPS, YLOC_FPS);
        // DrawTexture(wabbit, 400, 200, WHITE);
        WriteData(allInfo);
        // end the frame and get ready for the next one  (display frame, poll
        // input, etc...)
        EndDrawing();
    }

    // cleanup
    // unload our texture so it can be cleaned up
    UnloadTexture(wabbit);
    // destroy the window and cleanup the OpenGL context
    CloseWindow();
    return 0;
}
