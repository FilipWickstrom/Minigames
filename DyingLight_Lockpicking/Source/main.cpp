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

// Blur versions
#define BLUR_VERSION 1
// 0. Broteforce blur (Done)
// 1. Horizontal and vertical blur (Done)
// 2. Shared memory to reduce texelFetches

void LogCallback(int logLevel, const char *text, va_list args)
{
    const char *color;

    switch (logLevel)
    {
        case LOG_INFO:    color = "\033[32m"; break; // green
        case LOG_WARNING: color = "\033[33m"; break; // yellow
        case LOG_ERROR:   color = "\033[31m"; break; // red
        case LOG_DEBUG:   color = "\033[36m"; break; // cyan
        default:          color = "\033[0m";  break;
    }

    printf("%s", color);
    vprintf(text, args);
    printf("\033[0m\n"); // reset
}

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

    // Custom colors for the log 
    SetTraceLogCallback(LogCallback);
    
    // Initialize Raylib and ImGui
    const Vector2 resolution(1920, 1080);
    InitWindow(resolution.x, resolution.y, "Lockpicking");
    SetWindowState(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    rlImGuiSetup(true);
    rlSetFramebufferWidth(resolution.x);
    rlSetFramebufferHeight(resolution.y);
    //SetTargetFPS(60);
    
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

    // Loading textures
    Image img = LoadImage("Assets/Textures/DyingLight2_Roof.png");
    ImageFormat(&img, RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    Texture2D input = LoadTextureFromImage(img);
    UnloadImage(img);
    Texture2D lockTexture = LoadTexture("Assets/Textures/Lock.png");
    Image img2 = LoadImage("Assets/Textures/Tool1.png");
    // Resize to fit better with the scene
    ImageResize(&img2, img2.width * 2, img2.height * 2);
    Texture2D tool1Texture = LoadTextureFromImage(img2);
    UnloadImage(img2);


#if BLUR_VERSION == 0
    // Create empty output texture
    RenderTexture2D output = LoadRenderTexture(resolution.x, resolution.y);

    char* shaderCode = LoadFileText("Assets/Shaders/Blur.cs");
    unsigned int shaderId = rlLoadShader(shaderCode, RL_COMPUTE_SHADER);
    unsigned int shaderProgram = rlLoadShaderProgramCompute(shaderId);
    rlUnloadShader(shaderId);
    UnloadFileText(shaderCode);

#elif BLUR_VERSION == 1
    // Create empty output texture
    RenderTexture2D output1 = LoadRenderTexture(resolution.x, resolution.y);
    RenderTexture2D output2 = LoadRenderTexture(resolution.x, resolution.y);

    char* horizontalCode = LoadFileText("Assets/Shaders/BlurHorizontal.cs");
    unsigned int horizontalShaderId = rlLoadShader(horizontalCode, RL_COMPUTE_SHADER);
    unsigned int horizontalShaderProgram = rlLoadShaderProgramCompute(horizontalShaderId);
    rlUnloadShader(horizontalShaderId);
    UnloadFileText(horizontalCode);

    char* verticalCode = LoadFileText("Assets/Shaders/BlurVertical.cs");
    unsigned int verticalShaderId = rlLoadShader(verticalCode, RL_COMPUTE_SHADER);
    unsigned int verticalShaderProgram = rlLoadShaderProgramCompute(verticalShaderId);
    rlUnloadShader(verticalShaderId);
    UnloadFileText(verticalCode);
#endif

    // Game loop
    while (!WindowShouldClose())
    {
        BeginDrawing();
        rlImGuiBegin();	
        ClearBackground(BLACK);
    
    #if BLUR_VERSION == 0
        rlBindImageTexture(input.id, 0, RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, true);
        rlBindImageTexture(output.texture.id, 1, RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, false);
        rlEnableShader(shaderProgram);
        rlComputeShaderDispatch(resolution.x / 8, resolution.y / 8, 1);
        rlDisableShader();
        
        DrawTexture(output.texture, 0, 0, WHITE);
    #elif BLUR_VERSION == 1
        rlBindImageTexture(input.id, 0, RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, true);
        rlBindImageTexture(output1.texture.id, 1, RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, false);
        rlEnableShader(horizontalShaderProgram);
        rlComputeShaderDispatch(resolution.x / 8, resolution.y / 8, 1);
        rlDisableShader();

        rlBindImageTexture(output1.texture.id, 0, RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, true);
        rlBindImageTexture(output2.texture.id, 1, RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, false);
        rlEnableShader(verticalShaderProgram);
        rlComputeShaderDispatch(resolution.x / 8, resolution.y / 8, 1);
        rlDisableShader();

        DrawTexture(output2.texture, 0, 0, WHITE);
    #endif

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
        //DrawCircleSector(centerPos, lockSize, 0.f, -180.f, lockSize, RED);
        Vector2 lockPos = Vector2(centerPos.x - lockTexture.width, centerPos.y - lockTexture.height);
        DrawTextureEx(lockTexture, lockPos, 0.0, 2.0, WHITE);

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
        Vector2 lockpick = Vector2(centerPos.x + lockSize * cos(deg2rad(lockpickAngle - 180)), centerPos.y + lockSize * sin(deg2rad(lockpickAngle - 180)));
        DrawLineV(centerPos, lockpick, WHITE);

        if (IsKeyDown(KEY_A)) 
        {
            unlockTime += GetFrameTime();
            unlockTime = std::min(unlockTime, maxTimeToUnlock);
        }
        else 
        {
            unlockTime -= GetFrameTime();
            unlockTime = std::max(unlockTime, 0.0f);
        }
        
        DrawTexturePro(tool1Texture, 
                       Rectangle(0, 0, tool1Texture.width, tool1Texture.height),
                       Rectangle(centerPos.x, centerPos.y - 100, tool1Texture.width, tool1Texture.height),
                       Vector2(tool1Texture.width / 2.0f, tool1Texture.height / 2.0f), 
                       std::lerp(0, -90, unlockTime / maxTimeToUnlock), WHITE);
        
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

    // Cleanup duty
#if BLUR_VERSION == 0
    rlUnloadShaderProgram(shaderProgram);
    UnloadTexture(output.texture);
#elif BLUR_VERSION == 1
    rlUnloadShaderProgram(verticalShaderProgram);
    rlUnloadShaderProgram(horizontalShaderProgram);
    UnloadTexture(output1.texture);
    UnloadTexture(output2.texture);

#endif

    rlImGuiShutdown();	
    CloseWindow();

    return 0;
}