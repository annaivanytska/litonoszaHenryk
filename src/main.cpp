#include <raylib.h>
#include <vector>
#include <cmath>
#include <string>
#include <cstdlib>
#include <iostream>

//#include "dino.hpp"
#include "typ.hpp"
#include "build.hpp"

// Screen definitions
typedef enum GameScreen {Main = 0, Info, Game, Win} GameScreen;

const int screenWidth = 1200;
const int screenHeight = 800;

struct Man {
    Vector2 position;
    Texture2D texture;

    void Draw() const {
        //DrawTexture(texture, (int)position.x, (int)position.y, WHITE);
        DrawTextureEx(texture, position, 0.0f, 0.25f, WHITE);
    }
};

int main()
{
    InitWindow(screenWidth, screenHeight, "Listonosza Henryk");
    InitAudioDevice();

    Music bgMusic = LoadMusicStream("Graphics/music/wonder.wav");
    PlayMusicStream(bgMusic);

    GameScreen currentScreen = Main;
    Typ typ;
    Texture2D back = LoadTexture("Graphics/road8.png");

    std::vector<Building> buildings;
    InitBuildings(buildings);
    int envpGet = 0;
    int envpLast = 3;

    Rectangle playButton = Rectangle{120, 350, 400, 200};
    Rectangle exitButton = Rectangle{100, 600, 200, 100};
    Man man;
    man.texture = LoadTexture("Graphics/kk.png");
    man.position = {800, 300};

    Camera2D camera = {0};
    camera.target = typ.GetPosition();
    camera.offset = (Vector2){screenWidth/3.0f, screenHeight/3.0f};
    camera.rotation = 0.0f;
    camera.zoom = 2.0f;

    SetTargetFPS(60);

    while(WindowShouldClose() == false)
    {
    // UPDATE ---------------------------------------------
    UpdateMusicStream(bgMusic);
        switch (currentScreen) {
            case Main: {
                if (CheckCollisionPointRec(GetMousePosition(), playButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                    currentScreen = Info;
                }
                if (CheckCollisionPointRec(GetMousePosition(), exitButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                    exit(0);
                }
            } break;
            case Info: {
                if (IsKeyPressed(KEY_SPACE)) currentScreen = Game;
            } break;
            case Game: {
                Vector2 prevPos = {typ.GetPosition().x, typ.GetPosition().y};
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) std::cout << GetMousePosition().x << ' ' << GetMousePosition().y << std::endl;
                typ.Update();
                if (typ.GetPosition().x + typ.GetRect().width > screenWidth) typ.SetPositionX(screenWidth - typ.GetRect().width);
                    else if (typ.GetPosition().x < 0) typ.SetPositionX(0);
                if (typ.GetPosition().y + typ.GetRect().height > screenHeight)
                    typ.SetPositionY(screenHeight - typ.GetRect().height);
                else if (typ.GetPosition().y < 0)
                    typ.SetPositionY(0);

                // Sprawdzenie kolizji z budynkami
                for (const Building& b : buildings) {
                    if (CheckCollisionRecs(typ.GetRect(), b.GetRect())) {
                        // Odbicie: powrót do poprzedniej pozycji
                        typ.SetPosition(prevPos);
                        b.doTask();
                        break;
                    }
                }
                camera.target = typ.GetPosition();
            } break;
            case Win: {
                if (IsKeyPressed(KEY_SPACE)) exit(0);
            } break;
            default: break;
        }

    // DRAW ---------------------------------------------
        BeginDrawing();
        //ClearBackground(WHITE);
        DrawTexture(back, 0, 0, WHITE);

        switch (currentScreen){
            case Main:{
                DrawRectangleRec({80, 90, 700, 250}, Color{255, 255, 255, 150});  // RGBA, A=150 oznacza przezroczystość
                DrawRectangleLinesEx({80, 90, 700, 250}, 5.0f, BLACK);  // grubość 2px, kolor czarny
                man.Draw();
                DrawText("Listonosza", 100, 100, 120, RED);
                DrawText("Henryk", 300, 200, 120, RED);
                DrawRectangleRec(playButton, Color{255, 255, 255, 150});  // RGBA, A=150 oznacza przezroczystość
                DrawRectangleLinesEx(playButton, 5, BLACK);
                DrawText("PLay", 150, 400, 100, RED);
                DrawRectangleRec(exitButton, Color{255, 255, 255, 150});  // RGBA, A=150 oznacza przezroczystość
                DrawRectangleLinesEx(exitButton, 5, BLACK);
                DrawText("Exit", 150, 625, 50, RED);
            } break;
            case Info: {
                DrawText("How to Play:", 150, 150, 70, RED);
                DrawRectangleRec({15, 225, 1000, 350}, Color{255, 255, 255, 150});  // RGBA, A=150 oznacza przezroczystość
                DrawRectangleLinesEx({15, 225, 1000, 350}, 5.0f, BLACK);  // grubość 2px, kolor czarny
                DrawText("Jako nowy listonosza masz dzisiaj prace", 25, 250, 40, BLACK);
                DrawText("na rozgrzewke tylko kilka kopert", 75, 300, 40, BLACK);
                DrawText("do zebrania i oddania w miescie.", 25, 350, 40, BLACK);
                //DrawText("licznik z iloscia kopert, ktor zostalo znalesc.", 75, 400, 40, BLACK);
                DrawText("If you want to Play Press Space", 150, 650, 60, RED);
            } break;
            case Game:{
                BeginMode2D(camera);
                    for (const Building& b : buildings) {
                        b.Draw();
                        b.Animate(envpGet, envpLast);
                        //DrawRectangleRec(b.GetRect(), Fade(RED, 0.3f));
                    }
                    typ.Draw();
                    //typ.DrawHitbox(true);
                EndMode2D();
                DrawRectangleRec({1, 1, 400, 90}, Color{255, 255, 255, 150});
                DrawRectangleLinesEx({1, 1, 400, 90}, 5.0f, BLACK);  // grubość 2px, kolor czarny
                DrawText(TextFormat("Uzbierane koperty: %i/3", envpGet), 10, 10, 30, BLUE);
                DrawText(TextFormat("Oddane koperty: %i/3", envpLast), 10, 50, 30, BLUE);
                if (envpGet >= 3 && envpLast == 0) currentScreen = Win;
            } break;
            case Win: {
                DrawText("Nice Job!!!:", 150, 150, 70, RED);
                DrawRectangleRec({15, 225, 900, 350}, Color{255, 255, 255, 150});  // RGBA, A=150 oznacza przezroczystość
                DrawRectangleLinesEx({15, 225, 900, 350}, 5.0f, BLACK);  // grubość 2px, kolor czarny
                DrawText("Na dzisiaj wykonalesc cala", 25, 250, 40, BLACK);
                DrawText("prace jaka byla zaplanowana w miescie.", 75, 300, 40, BLACK);
                DrawText("Ale nie martw sie w przyszlosci bedzie", 25, 350, 40, BLACK);
                DrawText("jej wiecej.", 75, 400, 40, BLACK);
                DrawText("If you want to End Game Press Space", 150, 650, 50, RED);
            } break;
            
            default: break;
        }

        EndDrawing();
    }
    UnloadMusicStream(bgMusic);
    CloseAudioDevice();
    CloseWindow();

    // // Pamiętaj, aby na końcu zwolnić zasoby
    // for (auto& building : buildings) {
    //     UnloadTexture(building.texture); // Zwolnij teksturę
    // }
    // UnloadTexture(man.texture); // Zwolnij teksturę man
};
