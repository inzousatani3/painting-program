#include "raylib.h"

#define MAX_COLORS_COUNT 23

int main(void)
{

    const int screenWidth = 800;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "paint");

    // Colours to choose from
    Color colors[MAX_COLORS_COUNT] = {
        RAYWHITE, YELLOW, GOLD, ORANGE, PINK, RED, MAROON, GREEN, LIME, DARKGREEN,
        SKYBLUE, BLUE, DARKBLUE, PURPLE, VIOLET, DARKPURPLE, BEIGE, BROWN, DARKBROWN,
        LIGHTGRAY, GRAY, DARKGRAY, BLACK };

    // Define colorsRecs data (for every rectangle)
    Rectangle colorsRecs[MAX_COLORS_COUNT] = { 0 };

    for (int i = 0; i < MAX_COLORS_COUNT; i++)
    {
        colorsRecs[i].x = 10;
        colorsRecs[i].y = 60 + 30 * i + 2 * i;
        colorsRecs[i].width = 25;
        colorsRecs[i].height = 25;
    }

    int colorSelected = 0;
    int colorSelectedPrev = 0;
    int colorMouseHover = 0;
    float brushSize = 20.0;
    bool mouseWasPressed = false;

    // Create a RenderTexture2D to use as a canvas
    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);

    // Clear render texture before entering the game loop
    BeginTextureMode(target);
    ClearBackground(colors[0]);
    EndTextureMode();

    SetTargetFPS(120);              // Set our game to run at 120 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        Vector2 mousePos = GetMousePosition();

        if (colorSelected >= MAX_COLORS_COUNT) colorSelected = MAX_COLORS_COUNT - 1;
        else if (colorSelected < 0) colorSelected = 0;

        for (int i = 0; i < MAX_COLORS_COUNT; i++)
        {
            if (CheckCollisionPointRec(mousePos, colorsRecs[i]))
            {
                colorMouseHover = i;
                break;
            }
            else colorMouseHover = -1;
        }

        if ((colorMouseHover >= 0) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            colorSelected = colorMouseHover;
            colorSelectedPrev = colorSelected;
        }

        brushSize += GetMouseWheelMove()*5;
        if (brushSize < 5) brushSize = 5;
        if (brushSize > 50) brushSize = 50;

        if (IsKeyPressed(KEY_R))
        {

            BeginTextureMode(target);
            ClearBackground(colors[0]);
            EndTextureMode();
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) || (GetGestureDetected() == GESTURE_DRAG))
        {

            BeginTextureMode(target);
            if (mousePos.x > 50) DrawCircle((int)mousePos.x, (int)mousePos.y, brushSize, colors[colorSelected]);
            EndTextureMode();
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            if (!mouseWasPressed)
            {
                colorSelectedPrev = colorSelected;
                colorSelected = 0;
            }

            mouseWasPressed = true;

            BeginTextureMode(target);
            if (mousePos.y > 50) DrawCircle((int)mousePos.x, (int)mousePos.y, brushSize, colors[0]);
            EndTextureMode();
        }
        else if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && mouseWasPressed)
        {
            colorSelected = colorSelectedPrev;
            mouseWasPressed = false;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTextureRec(target.texture, (Rectangle) { 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2) { 0, 0 }, WHITE);

        if (mousePos.y > 50)
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) DrawCircleLines((int)mousePos.x, (int)mousePos.y, brushSize, GRAY);
            else DrawCircle(GetMouseX(), GetMouseY(), brushSize, colors[colorSelected]);
        }
        DrawFPS(60, 750);

        DrawRectangle(0, 0, 50, GetScreenHeight(), RAYWHITE);
        DrawLine(50, 0, 50,GetScreenHeight(), LIGHTGRAY);
        DrawRectangle(0, 0, GetScreenWidth(), 50, RAYWHITE);
        DrawLine(50, 50, GetScreenWidth(), 50, LIGHTGRAY);

        DrawText("colors", 10, 10, 20, BLACK);
        DrawText("press R to reset", 100, 10, 20, BLACK);
        DrawText("use the mouse to select a color", 300, 10, 20, BLACK);
        DrawText("Esc to exit", 680, 10, 20, BLACK);

        DrawLine(80, 0, 80, 50, LIGHTGRAY);
        DrawLine(290, 0, 290, 50, LIGHTGRAY);
        DrawLine(650, 0, 650, 50, LIGHTGRAY);
        DrawLine(1, 1, GetScreenWidth(), 1, LIGHTGRAY);
        
        for (int i = 0; i < MAX_COLORS_COUNT; i++) DrawRectangleRec(colorsRecs[i], colors[i]);
        DrawRectangleLines(10, 60, 25, 25, LIGHTGRAY);

        if (colorMouseHover >= 0) DrawRectangleRec(colorsRecs[colorMouseHover], Fade(WHITE, 0.6f));

        DrawRectangleLinesEx((Rectangle){ colorsRecs[colorSelected].x - 2, colorsRecs[colorSelected].y - 2,
                             colorsRecs[colorSelected].width + 4, colorsRecs[colorSelected].height + 4 }, 2, BLACK);


        EndDrawing();
    }
    UnloadRenderTexture(target);   
    CloseWindow();                  

    return 0;
}