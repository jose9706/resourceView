#include "raylib.h"
#include "resource_dir.h" // utility header for SearchAndSetResourceDir
#include "stdio.h"
#include "stdlib.h"
#include "system_params.h"
#include "window_util.h"

#define FREE(p)                                                                                                        \
    do                                                                                                                 \
    {                                                                                                                  \
        free(p);                                                                                                       \
        p = NULL;                                                                                                      \
    } while (0)

void HandleMouseEvent()
{
    int mLocX = GetMouseX();
    int mLocY = GetMouseY();
    // RLAPI bool CheckCollisionPointCircle(Vector2 point, Vector2 center, float radius);
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
    pSystemInfo allInfo = SetUpSystemInfo();
    // game loop
    while (!WindowShouldClose()) // run the loop untill the user presses ESCAPE
                                 // or presses the Close button on the window
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            // Check where tf we are.
            // RLAPI int GetMouseX(void); // Get mouse position X
            // RLAPI int GetMouseY(void); // Get mouse position Y
        }
        GetData(allInfo);
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
    FREE(allInfo);
    // destroy the window and cleanup the OpenGL context
    CloseWindow();
    return 0;
}
