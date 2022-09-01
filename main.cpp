#include <raylib.h>
#include <math.h>
#include <iostream>
#include <vector>

const int WIDTH = 860;
const int HEIGHT = 428;

float Clamp(float value, float min, float max)
{
    float result = (value < min) ? min : value;

    if (result > max)
        result = max;

    return result;
}

struct Player
{
    Vector2 pos;
    Vector2 vel;
    Vector2 acc;
    float radius = 10;
    float rot = 0;

    Player(float x, float y)
    {
        pos.x = x;
        pos.y = y;
    }

    void Seek(Vector2 &target, int &e, int &length)
    {
        float dx = target.x - this->pos.x;
        float dy = target.y - this->pos.y;

        float angle = atan2f(dy, dx);

        if (rot <= angle)
        {
            rot += 0.1;
        }
        else if (rot >= angle)
        {
            rot -= 0.1;
        }
        else
        {
            rot = angle;
        }

        rot = Clamp(rot, rot, angle);

        float fx = cosf(rot) * 3;
        float fy = sinf(rot) * 3;

        std::cout << "ROTASI = " << rot << " "
                  << "ANGLE = " << angle << std::endl;

        float dist = sqrtf((dx * dx) + (dy * dy));

        if (dist > 30)
        {
            this->vel.x += fx;
            this->vel.y += fy;
        }
        else
        {
            rot = angle;
            e = (e + 1) % length;
        }
    }
};

struct Path
{
    Vector2 pos;

    Path() = default;
    Path(float x, float y, float endX, float endY)
    {
        pos.x = x;
        pos.y = y;
    }

    void Draw()
    {
        DrawCircleV(pos, 5.0, RED);
    }
};

int main()
{
    InitWindow(WIDTH, HEIGHT, "Car Path following");
    SetTargetFPS(60);

    Texture2D track = LoadTexture("texture/nascar.png");
    Texture2D carImg = LoadTexture("texture/car.png");

    Player player = Player(300, 90);

    std::vector<Path> paths;
    Vector2 mousePos;
    Vector2 target;
    int e = 0;
    int pathsLength = 0;
    while (!WindowShouldClose())
    {

        ClearBackground(BLACK);
        BeginDrawing();
        DrawTextureEx(track, Vector2(0, 0), 0.0, 1.0, WHITE);

        mousePos = GetMousePosition();

        pathsLength = paths.size();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Path p;
            p.pos = GetMousePosition();
            paths.push_back(p);
        }

        player.acc = -player.vel * 0.5;
        player.vel += player.acc;
        player.pos += player.vel;

        if (player.pos.x > WIDTH)
        {
            player.pos.x = 0;
        }
        else if (player.pos.y > HEIGHT)
            player.pos.y = 0;

        for (int i = 0; i < paths.size(); i++)
            paths[i].Draw();

        if (paths.size() > 0)
        {
            player.Seek(paths[e].pos, e, pathsLength);
        }
        DrawTexturePro(carImg, Rectangle{0.0, 0.0, static_cast<float>(carImg.width), static_cast<float>(carImg.height)}, Rectangle{player.pos.x, player.pos.y, carImg.width * 0.3f, carImg.height * 0.3f}, Vector2(carImg.width * 0.3 / 2, carImg.height * 0.3 / 2), player.rot * RAD2DEG, WHITE);

        EndDrawing();
    }
    UnloadTexture(track);
    CloseWindow();

    return 0;
}