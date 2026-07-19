#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <optional>

#include <raylib.h>
#include <switch.h>


#include <curl/curl.h>

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
bool running = true;

int request_website(std::string url, std::string* htmlResponse);

int main(void)
{
    InitWindow(screenWidth, screenHeight, "NX Raylib Template");

    // Texture loading
    //Texture2D exampleTex = LoadTexture("romfs:/resources/example.png");

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

    std::string websiteHTML = "";
    int requestRes = request_website("https://example.org", &websiteHTML);

    while (!WindowShouldClose() && running)
    {
        BeginDrawing();
        
            if(IsGamepadAvailable(gamepad))
            {

                // Plus button to exit
                if (IsGamepadButtonPressed(gamepad, GAMEPAD_BUTTON_MIDDLE_RIGHT))
                {
                    running = false;
                }

                ClearBackground(RAYWHITE);

                //DrawText(TextFormat("GP%d: %s", gamepad, GetGamepadName(gamepad)), 10, 10, 20, BLACK);

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


                DrawText("Puszta Browser NX", 20, 40, 20, BLACK);
                DrawText("By Laky2k8", 20, 80, 20, RED);

                DrawText(TextFormat("Result code of request: %d", requestRes), 1020, 50, 20, BLACK);
                DrawText(websiteHTML.c_str(), 1020, 80, 20, BLACK);

                DrawText(TextFormat("Left Stick: (%.2f, %.2f)", leftStickX, leftStickY), 1020, 590, 20, DARKGRAY);
                DrawText(TextFormat("Right Stick: (%.2f, %.2f)", rightStickX, rightStickY), 102, 620, 20, DARKGRAY);
                DrawText(TextFormat("Left Trigger: %.2f", leftTrigger), 1020, 650, 20, DARKGRAY);
                DrawText(TextFormat("Right Trigger: %.2f", rightTrigger), 1020, 680, 20, DARKGRAY);

                //DrawTexture(exampleTex, screenWidth/2 - exampleTex.width/2, screenHeight/2 - exampleTex.height/2, WHITE);
                //DrawTexture(exampleTex, screenWidth/2 - exampleTex.width/2, 300, WHITE);
            }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

size_t write_to_string(void * a_buffer, size_t a_size, size_t a_nmemb, void * a_userp)
{
    size_t resulting_size = a_size * a_nmemb;

    std::string * result_string = static_cast<std::string *>(a_userp);
    char * buffer_char_ptr = static_cast<char *>(a_buffer);

    // Check for overflow
    if(result_string->max_size() - result_string->size() < resulting_size)
    {
        return 0;
    }

    result_string->append(buffer_char_ptr, resulting_size);

    return resulting_size;
}

int request_website(std::string url, std::string* htmlResponse)
{
    CURL *curl;
    CURLcode resCode;
    char curl_errbuf[CURL_ERROR_SIZE];
    std::string response;

    BeginDrawing();
    ClearBackground(RAYWHITE);


    DrawText("Initalizing curl...", 10, 90, 20, BLUE);
    resCode = curl_global_init(CURL_GLOBAL_ALL);

    if(resCode != CURLE_OK)
    {
        DrawText(TextFormat("curl init failed: %d", (int)resCode), 10, 120, 20, RED);
        return (int)resCode;
    }

    curl = curl_easy_init();

    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curl_errbuf);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/124.0.0.0 Safari/537.36");

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        DrawText("Getting webpage", 10, 90, 20, BLUE);
        resCode = curl_easy_perform(curl);

        if(resCode == CURLE_OK)
        {
            *htmlResponse = response;
        }
        else
        {
            DrawText(TextFormat("Getting webpage failed: %s", curl_errbuf), 10, 150, 20, RED);
        }

        curl_easy_cleanup(curl);

    }
    else
    {
       DrawText(TextFormat("curl init failed: %s", curl_errbuf), 10, 120, 20, RED);
    }

    curl_global_cleanup();
    EndDrawing();

    return (int)resCode;
}