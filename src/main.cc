#include "raylib.h"


#define GAME_OVER CLITERAL (Color){204, 79, 65,255}

// Window dimensions:
inline const int windowHeight{ 380 };
inline const int windowWidth{ 512 };
// Acceleration due to gravity (pixels/sec) / sec
inline const int gravity{ 1000 };
// Jump velocity (pixels / sec)
inline const int jumpVel{ -600 };

inline const int nebVel{ -300 };

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
    int vel;
};

// Check if is on the ground:
bool isOnGround(AnimData data, int windowHeight);
// Update animation data:
AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame);

// Check for collision
bool collision(AnimData& robot, AnimData nebulae[], int numNebulae);

int main()
{
    // Initialize window:
    InitWindow(windowWidth, windowHeight, "AYARI Adel");

    // Nebula variables:
    Texture2D nebula = LoadTexture("resources/Nebula.png");

    float nebulaeDistance{};
    AnimData nebulae[10]{};
    for (int i = 0; i < 10; i++)
    {
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width / 8;
        nebulae[i].rec.height = nebula.height / 8;
        nebulae[i].pos.x = windowWidth + nebulaeDistance;
        nebulae[i].pos.y = windowHeight - nebulae[i].rec.height;
        nebulae[i].frame = 0;
        nebulae[i].updateTime = 1.0 / 12.0;
        nebulae[i].runningTime = 0.0;
        nebulaeDistance += 600.0;
        nebulae[i].vel = -250;
    }

    // Finish line:
    float finishLine{ nebulae[9].pos.x + 20};

    // Robot variables:
    Texture2D robot = LoadTexture("resources/Robot.png");

    AnimData robotData;
    robotData.rec.height = robot.height;
    robotData.rec.width = robot.width / 6;
    robotData.rec.x = 0;
    robotData.rec.y = 0;
    robotData.pos.x = windowWidth / 2 - robotData.rec.width / 2;
    robotData.pos.y = windowHeight - robotData.rec.height;
    robotData.frame = 0;
    robotData.updateTime = 1.0 / 12.0;
    robotData.runningTime = 0.0;
    robotData.vel = 0;

    // Initialize velocity:
    int velocity{ 0 };

    // Check if is in the air:
    bool isInAir{};

    Texture2D background = LoadTexture("resources/background.png");
    int bgX{ 0 };
    Texture2D foreground = LoadTexture("resources/foreground.png");
    int fgX{ 0 };
    Texture2D midground = LoadTexture("resources/midground.png");
    int mgX{ 0 };

    bool collision_{};
    bool gameOver = false; // Initialize the game over state

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        // Time since the last frame:
        const float dT{ GetFrameTime() };

        // Start drawing:
        BeginDrawing();
        ClearBackground(WHITE);

        // Scroll midground:
        mgX -= 80 * dT;
        if (mgX <= -midground.width * 2) mgX = 0;
        // Scroll foreground:
        fgX -= 150 * dT;
        if (fgX <= -foreground.width * 2) fgX = 0;

        Vector2 bg1Pos{ 0, -50 };
        DrawTextureEx(background, bg1Pos, 0.0, 0.4, WHITE);
        Vector2 bg2Pos{ background.width * 0.4, 0 };
        DrawTextureEx(background, bg2Pos, 0.0, 0.4, WHITE);

        Vector2 mg1Pos{ mgX, 0 };
        DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE);
        Vector2 mg2Pos{ mgX + midground.width * 2.0, 0 };
        DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);

        Vector2 fg1Pos{ fgX, 0 };
        DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);
        Vector2 fg2Pos{ fgX + foreground.width * 2.0, 0 };
        DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);

        // Ground check:
        if (isOnGround(robotData, windowHeight))
        {
            // Rectangle is on the ground:
            velocity = 0;
            isInAir = false;
        }
        else
        {
            // Rectangle is in the air:
            // Apply gravity:
            velocity += gravity * dT;
            isInAir = true;
        }

        // Check for jumping:
        if (IsKeyPressed(KEY_UP) && !isInAir) velocity = jumpVel;

        for (int i = 0; i < 10; i++)
        {
            // Update nebula position:
            nebulae[i].pos.x += nebVel * dT;
            // Update nebula animation frame:
            nebulae[i] = updateAnimData(nebulae[i], dT, 7);
        }
        // Update finishLine x pos
        finishLine += nebulae[9].vel * dT;

        if (!gameOver)
        {
            // Update robot position:
            robotData.pos.y += velocity * dT;

            // Update robot animation frame:
            if (!isInAir)
            {
                robotData = updateAnimData(robotData, dT, 5);
            }

            collision_ = collision(robotData, nebulae, sizeof(nebulae) / sizeof(nebulae[0]));

            if (robotData.pos.x >= finishLine)
            {
                // Robot reached the finish line, set game over state to true
                gameOver = true;
            }
            else if (collision_)
            {
                // Collision detected, set game over state to true
                gameOver = true;
            }
        }

        if (gameOver)
        {
            // Display the "Game Over" message and restart button
            if (robotData.pos.x >= finishLine)
            {
                DrawText("You Win!", windowWidth / 2 - MeasureText("You Win!", 40) / 2, windowHeight / 2 - 40, 40, GREEN);
                // © AYARI Adel
                DrawText("© AYARI Adel",windowWidth / 2 - MeasureText("© AYARI Adel", 10) / 2, windowHeight / 2 + 60,10, GREEN);
            }
            else
            {
                DrawText("Game Over", windowWidth / 2 - MeasureText("Game Over", 40) / 2, windowHeight / 2 - 40, 40, GAME_OVER);
                // © AYARI Adel
                DrawText("© AYARI Adel",windowWidth / 2 - MeasureText("© AYARI Adel", 10) / 2, windowHeight / 2 + 60,10, GAME_OVER);
            }
            DrawText("Press R to Restart", windowWidth / 2 - MeasureText("Press R to Restart", 20) / 2, windowHeight / 2 + 20, 20, GRAY);
            
            // Check for restart input
            if (IsKeyPressed(KEY_R))
            {
                // Reset the game state to restart the game
                gameOver = false;
                robotData.pos.x = windowWidth / 2 - robotData.rec.width / 2;
                robotData.pos.y = windowHeight - robotData.rec.height;
                // Reset other variables as needed
                // ...
            }
        }

        // Draw Nebula:
        for (int i = 0; i < 10; i++)
        {
            // Draw nebula:
            DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
        }

        // Draw robot only when there is no collision and it hasn't won yet
        if (!gameOver && !collision_ && robotData.pos.x < finishLine)
        {
            DrawTextureRec(robot, robotData.rec, robotData.pos, WHITE);
        }

        // Finish drawing:
        EndDrawing();
    }
    UnloadTexture(robot);
    UnloadTexture(nebula);
    UnloadTexture(foreground);
    UnloadTexture(background);
    UnloadTexture(midground);
    CloseWindow();
}

bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0;
        // Update the animation:
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if (data.frame > maxFrame) data.frame = 0;
    }
    return data;
}

bool collision(AnimData& robot, AnimData nebulae[], int numNebulae)
{
    bool collision{ false };
    for (int i = 0; i < numNebulae; i++)
    {
        Rectangle nebRec
        {
            nebulae[i].pos.x + 60,
            nebulae[i].pos.y + 20,
            nebulae[i].rec.width - 80,
            nebulae[i].rec.height - 80
        };
        Rectangle robotRec
        {
            robot.pos.x,
            robot.pos.y,
            robot.rec.width,
            robot.rec.height
        };
        if (CheckCollisionRecs(nebRec, robotRec)) { collision = true; break; }
    }
    return collision;
}
