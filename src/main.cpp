#include <iostream>
#include "raylib.h"
#include "GameManager.h"
#include "LogManager.h"

int main() {
    // Start up Game Manager.
    if (GM.startUp()) {
        LM.writeLog(”Error starting game manager!”);
        GM.shutDown();
        return 3;
    }
    // Set flush of logfile during development (when done , make false ).
    LM.setFlush(true);
    // Show splash screen .
    df::splash();
    // Shut everything down.
    GM.shutDown();
    return 0;
    InitWindow(800, 600, "test");

    while(!WindowShouldClose()) {
        BeginDrawing();
        DrawRectangle(0,0, 100, 100, RED);
        EndDrawing();
    }
    return 0;
}