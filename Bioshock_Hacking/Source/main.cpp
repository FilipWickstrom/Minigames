#include <raylib.h>

int main()
{
    InitWindow(800, 450, "raylib [core] example - basic window");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }
    
    CloseWindow(); //ERROR with this on windows... Collision with name of the function...

	return 0;
}