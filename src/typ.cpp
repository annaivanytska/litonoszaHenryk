#include "typ.hpp"

int frameWidth = 110;
int frameHeight = 180;

int currentFrame = 0;
float frameTime = 0.0f;
float frameSpeed = 0.1f; // im mniejsza, tym szybciej

int framesCounter = 0;

float scale = 0.35f;

enum Direction { DOWN = 0, UP = 1, LEFT = 2, RIGHT = 3 };
Direction currentDirection = DOWN;


Typ::Typ()
{
    image = LoadTexture("Graphics/1/typAni2.png");
    position.x = 50;
    position.y = 50;
    speed = 5;
}

Typ::~Typ()
{
    UnloadTexture(image);
}

void Typ::Draw()
{
    Rectangle sourceRec = {
        (float)currentFrame * frameWidth,         // X - numer klatki
        (float)currentDirection * frameHeight,    // Y - kierunek (rząd w sprite sheet)
        (float)frameWidth,                        // szerokość klatki
        (float)frameHeight                        // wysokość klatki
    };

    Rectangle destRec = {
        position.x,
        position.y,
        frameWidth * scale, // skalowanie ×0.5
        frameHeight * scale
    };

    Vector2 origin = { 0.0f, 0.0f };

    DrawTexturePro(image, sourceRec, destRec, origin, 0.0f, WHITE);
}


void Typ::Update()
{
    bool moving = false;

    if(IsKeyDown(KEY_RIGHT)) {
        position.x += speed;
        currentDirection = RIGHT;
        moving = true;
    }
    if(IsKeyDown(KEY_LEFT)) {
        position.x -= speed;
        currentDirection = LEFT;
        moving = true;
    }
    if(IsKeyDown(KEY_UP)) {
        position.y -= speed;
        currentDirection = UP;
        moving = true;
    }
    if(IsKeyDown(KEY_DOWN)) {
        position.y += speed;
        currentDirection = DOWN;
        moving = true;
    }

    if(moving) {
        frameTime += GetFrameTime();
        if(frameTime >= frameSpeed) {
            frameTime = 0.0f;
            currentFrame++;
            if(currentFrame > 2) currentFrame = 0; // 3 klatki: 0,1,2
        }
    } else {
        currentFrame = 0; // środkowa klatka jako "idle"
        currentDirection = DOWN;
    }
}


Rectangle Typ::GetRect()
{
    //return Rectangle{position.x, position.y, float(image.width/2.0f), float(image.height/2.0f)};
    return Rectangle {
        position.x,
        position.y,
        (float)frameWidth * scale,
        (float)frameHeight * scale
    };
}

void Typ::DrawHitbox(bool isColliding)
{
    if(isColliding)
        DrawRectangleLinesEx(GetRect(), 3, RED);
}

Vector2 Typ::GetPosition()
{
    return (Vector2){position.x, position.y};
}

void Typ::SetPosition (Vector2 pos){
    position.x = pos.x;
    position.y = pos.y;
}

void Typ::SetPositionX (int posX){
    position.x = posX;
}

void Typ::SetPositionY (int posY){
    position.y = posY;
}