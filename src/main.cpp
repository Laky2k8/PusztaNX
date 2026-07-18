#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <raylib.h>
#include <switch.h>

const int screenWidth = 1280;
const int screenHeight = 720;

// Set axis deadzones
const float leftStickDeadzoneX = 0.1f;
const float leftStickDeadzoneY = 0.1f;
const float rightStickDeadzoneX = 0.1f;
const float rightStickDeadzoneY = 0.1f;
const float leftTriggerDeadzone = -0.9f;
const float rightTriggerDeadzone = -0.9f;

int gamepad = 0; // gamepad index

int main(void)
{
    InitWindow(screenWidth, screenHeight, "NX Raylib Template");

    // Texture loading
    Texture2D exampleTex = LoadTexture("romfs:/resources/example.png");

    if(IsGamepadAvailable(gamepad))
    {
        SetExitKey(GAMEPAD_BUTTON_MIDDLE_RIGHT);
    }
    else
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Controller is not available! Something is wrong!", 10, 10, 20, RED);
        EndDrawing();
    }

    while (!WindowShouldClose())
    {
        BeginDrawing();
        
            if(IsGamepadAvailable(gamepad))
            {
                ClearBackground(RAYWHITE);

                DrawText(TextFormat("GP%d: %s", gamepad, GetGamepadName(gamepad)), 10, 10, 20, BLACK);

                // Get axis values
                float leftStickX = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_X);
                float leftStickY = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_Y);
                float rightStickX = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_X);
                float rightStickY = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_Y);
                float leftTrigger = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_TRIGGER);
                float rightTrigger = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_TRIGGER);

                // Calculate deadzones
                if (leftStickX > -leftStickDeadzoneX && leftStickX < leftStickDeadzoneX) leftStickX = 0.0f;
                if (leftStickY > -leftStickDeadzoneY && leftStickY < leftStickDeadzoneY) leftStickY = 0.0f;
                if (rightStickX > -rightStickDeadzoneX && rightStickX < rightStickDeadzoneX) rightStickX = 0.0f;
                if (rightStickY > -rightStickDeadzoneY && rightStickY < rightStickDeadzoneY) rightStickY = 0.0f;
                if (leftTrigger < leftTriggerDeadzone) leftTrigger = -1.0f;
                if (rightTrigger < rightTriggerDeadzone) rightTrigger = -1.0f;


                DrawText("NX Raylib Template", 190, 200, 20, BLACK);
                DrawText("By Laky2k8", 190, 230, 20, RED);

                DrawText(TextFormat("Left Stick: (%.2f, %.2f)", leftStickX, leftStickY), 10, 50, 20, DARKGRAY);
                DrawText(TextFormat("Right Stick: (%.2f, %.2f)", rightStickX, rightStickY), 10, 80, 20, DARKGRAY);
                DrawText(TextFormat("Left Trigger: %.2f", leftTrigger), 10, 110, 20, DARKGRAY);
                DrawText(TextFormat("Right Trigger: %.2f", rightTrigger), 10, 140, 20, DARKGRAY);

                //DrawTexture(exampleTex, screenWidth/2 - exampleTex.width/2, screenHeight/2 - exampleTex.height/2, WHITE);
                DrawTexture(exampleTex, screenWidth/2 - exampleTex.width/2, 300, WHITE);
            }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}