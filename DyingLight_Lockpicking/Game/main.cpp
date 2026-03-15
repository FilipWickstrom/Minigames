#include <raylib.h>
#include <rlgl.h>
#include <rlImGui.h>    // remove in release
#include <imgui.h>      // remove in release
#include <cmath>
#include <algorithm>
#include <string>

/* #### Dying Light lockpicking clone ####
The goal is to unluck the lock by picking the correct angle

Input:
Pick angle: A / left stick
Rotate lock: Mouse / right stick
*/


/* #### TODO ####
* Durability on the lockpicks. 3 tries then broken.
* Add textures and models
*/


float deg2rad(float deg)
{
    return deg * (PI / 180);
}

float rad2deg(float rad) 
{
    return rad * (180 / PI);
}

enum class DifficultyType
{
    EASY,
    MEDIUM,
    HARD,
    VERYHARD
};
    
constexpr std::string DifficultyToString(const DifficultyType& type) 
{
    switch (type)
    {    
        case DifficultyType::EASY:     return "Easy";
        case DifficultyType::MEDIUM:   return "Medium";
        case DifficultyType::HARD:     return "Hard";
        case DifficultyType::VERYHARD: return "Very hard";
        default: return "Error...";
    }
};

constexpr float DifficultyToAngle(const DifficultyType& type) 
{
    switch (type)
    {    
        case DifficultyType::EASY:     return 16.0f; 
        case DifficultyType::MEDIUM:   return 8.0f;
        case DifficultyType::HARD:     return 4.0f;
        case DifficultyType::VERYHARD: return 2.0f;
        default:                       return 16.0f;
    }
};

enum class LockStatus 
{
    UNLOCKING,
    UNLOCKED_SUCCESS,
    UNLOCKED_FAILED
};


// Remove if not needed
float RandomizeAngle()
{
    // Check which difficulty
    // Easy:    0, 90 or 180 deg
    // Medium:  0, 45, 90, 135, 180 deg
    // Hard:    0, 25, 50, 75, ... 180
    // Very hard: 0, 10, ...         180 deg
    return 0.0f;
}

int main()
{
    // Set working directory
    ChangeDirectory(PROJECT_DIR);
    
    const Vector2 resolution(1920, 1080);
    
    // Initialize Raylib and ImGui
    InitWindow(resolution.x, resolution.y, "Lockpicking");
    SetWindowState(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    rlImGuiSetup(true);
    rlSetFramebufferWidth(resolution.x);
    rlSetFramebufferHeight(resolution.y);
    
    const auto& defaultFont = GetFontDefault();
    DifficultyType difficulty = DifficultyType::EASY;
    float difficultyAngle = DifficultyToAngle(difficulty);

    const Vector2 centerPos(resolution.x * 0.5f, resolution.y * 0.5f);  
    const float lockSize = 0.3f * resolution.y;

    //SetRandomSeed(time)
    float lockAngleDeg = float(GetRandomValue(0, 180));
    float lockpickAngle = 0.f;

    float unlockTime = 0.0f;
    float maxTimeToUnlock = 2.0f;
    LockStatus lockStatus = LockStatus::UNLOCKING;

    // Progress bar
    const Vector2 progressBarPos(resolution.x * 0.5f, resolution.y * 0.55f);
    const Vector2 progressBarSize(lockSize*2.0, resolution.y * 0.06);
    const Vector2 progressBarPosTopLeft(progressBarPos.x - progressBarSize.x/2.0, progressBarPos.y - progressBarSize.y/2.0);
    const char*   progressBarText = "Unlocking!";
    Vector2       progressBarTextSize(0, 0);
    float         progressBarFontSize = 10.0f;
    float         progressBarTextSpacing = 10.0f;

    // TODO:
    // Change the text inside of the progressBar?
    // 1. Unlocking!, 2. Unlocked! 3. Failed...
    // Yellow, Green, Red

    // Loop to check which fontsize is enough for the box
    while (progressBarTextSize.y < progressBarSize.y * 0.7) 
    {
        progressBarTextSize = MeasureTextEx(defaultFont, progressBarText, progressBarFontSize, progressBarTextSpacing);
        progressBarFontSize += 1;
    }

    // TESTING AREA
    // NEED TO BE RGBA
    Image img = LoadImage("Assets/Textures/DyingLight2_Roof2.jpg");
    ImageFormat(&img, RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    Texture2D input = LoadTextureFromImage(img);
    UnloadImage(img);
    //const Texture2D texture = LoadTexture("Assets/Textures/DyingLight2_Roof2.jpg");
    // if (IsTextureValid(texture)) {
    //     std::printf("WE LOADED THE FUCKING IMAGE\n");
    // }
    // Create empty output texture
    RenderTexture2D output = LoadRenderTexture(resolution.x, resolution.y);

    char* shaderCode = LoadFileText("Assets/Shaders/Blur.cs");
    unsigned int shaderId = rlCompileShader(shaderCode, RL_COMPUTE_SHADER);
    unsigned int shaderProgram = rlLoadComputeShaderProgram(shaderId);
    UnloadFileText(shaderCode);

    // Game loop
    while (!WindowShouldClose())
    {
        rlBindImageTexture(input.id, 0, RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, true);
        rlBindImageTexture(output.texture.id, 1, RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, false);
        rlEnableShader(shaderProgram);
        rlComputeShaderDispatch(resolution.x / 8, resolution.y / 8, 1);
        rlDisableShader();

        BeginDrawing();
        rlImGuiBegin();	
        ClearBackground(BLACK);

        DrawTexture(output.texture, 0, 0, WHITE);
        

        // ### DEBUG ### Random new lock
        if (IsKeyReleased(KEY_R)) 
        {
            lockAngleDeg = float(GetRandomValue(0, 180));
            lockStatus = LockStatus::UNLOCKING;
        }

        /*
        #####################
        ####### INPUT #######
        #####################
        */    
        if (IsKeyReleased(KEY_ONE))
        {
            difficulty = DifficultyType::EASY;
        }
        else if (IsKeyReleased(KEY_TWO))
        {
            difficulty = DifficultyType::MEDIUM;
        }
        if (IsKeyReleased(KEY_THREE))
        {
            difficulty = DifficultyType::HARD;
        }
        if (IsKeyReleased(KEY_FOUR))
        {
            difficulty = DifficultyType::VERYHARD;
        }

        difficultyAngle = DifficultyToAngle(difficulty);


        /*
        #########################
        ####### RENDERING #######
        #########################
        */

        std::string difficultyText = "Difficulty: " + DifficultyToString(difficulty);
        DrawText(difficultyText.c_str(), 0, 0, 50, WHITE);

        // TODO: Draw a texture here
        DrawCircleSector(centerPos, lockSize, 0.f, -180.f, lockSize, RED);

        Vector2 mouseDelta = GetMouseDelta();
        if (mouseDelta.x != 0.f && mouseDelta.y != 0.f) 
        {
            // Calc angle between mousepoint and center
            Vector2 mousePos = GetMousePosition();
            float deltaX = centerPos.x - mousePos.x;
            float deltaY = std::max(centerPos.y - mousePos.y, 0.0f);
            lockpickAngle = rad2deg(atan2(deltaY, deltaX));
        }
        lockpickAngle = std::clamp(lockpickAngle, 0.f, 180.f);

        // Draw the correct angle
        float lockpickX = centerPos.x + lockSize * cos(deg2rad(lockpickAngle - 180));
        float lockpickY = centerPos.y + lockSize * sin(deg2rad(lockpickAngle - 180));
        DrawLine(centerPos.x, centerPos.y, lockpickX, lockpickY, WHITE);

        if (IsKeyDown(KEY_A)) 
        {
            unlockTime = std::min(unlockTime + GetFrameTime(), maxTimeToUnlock);
        }
        else if (IsKeyReleased(KEY_A)) 
        {
            unlockTime = 0.0f;
        }
        
        // Progress bar
        DrawRectangleLines(progressBarPosTopLeft.x, progressBarPosTopLeft.y, progressBarSize.x, progressBarSize.y, GRAY);
        DrawRectangle(progressBarPosTopLeft.x, progressBarPosTopLeft.y, (int)(progressBarSize.x * (unlockTime / maxTimeToUnlock)), progressBarSize.y, YELLOW);
        DrawTextPro(defaultFont, progressBarText, progressBarPos, Vector2(progressBarTextSize.x/2, progressBarTextSize.y/2), 0, progressBarFontSize, progressBarTextSpacing, BLACK);


        // Do once
        if (unlockTime >= maxTimeToUnlock) 
        {
            float angleDiff = std::abs(lockpickAngle - lockAngleDeg);
            if (angleDiff < difficultyAngle) 
                lockStatus = LockStatus::UNLOCKED_SUCCESS;
            else
                lockStatus = LockStatus::UNLOCKED_FAILED;
        }

        if (lockStatus == LockStatus::UNLOCKED_SUCCESS) 
        {
            int textSize = MeasureText("Lock unlocked!", 50.f);
            DrawTextPro(defaultFont,   "Lock unlocked!", centerPos, Vector2(textSize/2,0), 0.0f, 50.f, 10.0f, YELLOW);
        }
        else if (lockStatus == LockStatus::UNLOCKED_FAILED)
        {
            int textSize = MeasureText("Epic fail...", 50.f);
            DrawTextPro(defaultFont,   "Epic fail...", centerPos, Vector2(textSize/2,0), 0.0f, 50.f, 10.0f, RED);
        }

        /*
        #####################
        ####### DEBUG #######
        #####################
        */
        // DEBUG: Draw the angle the lock
        float x = centerPos.x + lockSize * cos(deg2rad(lockAngleDeg - 180));
        float y = centerPos.y + lockSize * sin(deg2rad(lockAngleDeg - 180));
        DrawLine(centerPos.x, centerPos.y, x, y, GREEN);
        // Debug stuff
        ImGui::Text("Angle of the lock: %f", lockAngleDeg);
        ImGui::Text("Lockpick angle: %f", lockpickAngle);
        
        rlImGuiEnd();
        DrawFPS(resolution.x - 100, 10);
        EndDrawing();
    }

    rlUnloadShaderProgram(shaderProgram);

    rlImGuiShutdown();	
    CloseWindow();

    return 0;
}