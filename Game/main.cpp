#include <raylib.h>
#include <rlImGui.h>
#include <imgui.h>

/*##################### Getting started ################################
* Some useful links
* https://www.raylib.com/examples.html (check shaders tab for some cool stuff)
* https://www.raylib.com/cheatsheet/cheatsheet.html
######################################################################*/

int main()
{
    // Set working directory
    ChangeDirectory(PROJECT_DIR);
    
    // Initialize Raylib and ImGui
    InitWindow(1920, 1080, "Game Jam Template");
    SetWindowState(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    rlImGuiSetup(true);
    
    // Unlimited FPS? 
    //ClearWindowState(FLAG_VSYNC_HINT);
    // Limit FPS?
    //SetTargetFPS(120);

    Texture2D bussenTex = LoadTexture("Assets/BussenStudio.png");
    
    // Game loop
    while (!WindowShouldClose())
    {
        BeginDrawing();
        rlImGuiBegin();	
        ClearBackground(BLACK);
        
        DrawTexture(bussenTex, (GetScreenWidth() - bussenTex.width) / 2, (GetScreenHeight() - bussenTex.height) / 2, RAYWHITE);
        ImGui::ShowDemoWindow();

        rlImGuiImage(&bussenTex);
        
        rlImGuiEnd();
        DrawFPS(10, 10);
        EndDrawing();
    }

    rlImGuiShutdown();	
    CloseWindow();

    return 0;
}