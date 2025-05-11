#pragma once
#include <raylib.h>

class Typ{
private:
    Vector2 position;
    Texture2D image;
    int speed;
public:
    Typ();
    ~Typ();

    void Draw();
    void Update();
    Rectangle GetRect();
    void DrawHitbox(bool isColliding);

    Vector2 GetPosition();
    void SetPosition (Vector2 pos);
    void SetPositionX (int posX);
    void SetPositionY (int posY);
};