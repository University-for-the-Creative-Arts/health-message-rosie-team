#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <algorithm>
#include <vector>

const int screenWidth = 960;
const int screenHeight = 600;

Vector2 guyPosition = (Vector2){ screenWidth * 0.5f, screenHeight * 0.5f };
float guyRadius = 50.0f;

int health = 100;

class Projectile {
    private:
    Vector2 position; Vector2 direction;
    float speed = 10.0f, radius = 15.0f;

    public:
    void Init() {
        direction = Vector2Normalize((Vector2){ (float)GetRandomValue(-500, 500), (float)GetRandomValue(-500, 500) });
        position = guyPosition - (direction * GetScreenWidth() * 0.5f);
    }

    bool Update() {
        position += direction * speed;

        if (health > 0 && CheckCollisionCircles(guyPosition, guyRadius, position, radius)) {
            health--;
            return true;
        }
        return false;
    }
        
    void Draw() {
        DrawCircle(position.x, position.y, radius, ORANGE);
    }
};
std::vector<Projectile> allProjectiles;

void DrawTextScreenScaled(Font font, const char *text, float posPercentX, float posPercentY, float sizePercent, float spacingPercent, float posAlignmentPercent, Color color) {
    Vector2 textScale = MeasureTextEx(font, text, GetScreenHeight() * sizePercent, GetScreenHeight() * spacingPercent);
    DrawTextEx(font, text, (Vector2){ GetScreenWidth() * posPercentX, GetScreenHeight() * posPercentY } - (Vector2){ textScale.x * posAlignmentPercent, textScale.y * 0.5f }, GetScreenHeight() * sizePercent, GetScreenHeight() * spacingPercent, color);
}

int main() {
    InitWindow(screenWidth, screenHeight, "Fireballs!!!!");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        if (health > 0 && IsKeyPressed(KEY_SPACE)) {
            allProjectiles.push_back(*new Projectile);
            allProjectiles.back().Init();
        }

        for (auto it = allProjectiles.begin(); it != allProjectiles.end(); ) {
            if (it->Update())
            {
                it = allProjectiles.erase(it);

            }
                else
            {
                ++it;
            }
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);
            if (health > 0) {
                DrawCircle(guyPosition.x, guyPosition.y, guyRadius, BLUE);
                DrawTextScreenScaled(GetFontDefault(), TextFormat("Health: %i", health), 0.5f, 0.3f, 0.1f, 0.01f, 0.5f, RED);
            }
            else {
                DrawTextScreenScaled(GetFontDefault(), "DEAD!! NOOOO :(", 0.5f, 0.5f, 0.2f, 0.01f, 0.5f, RED);
            }

            for (Projectile& i : allProjectiles) {
                i.Draw();
            }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}