/*
This project uses the Raylib framework to provide us functionality for math, graphics, GUI, input etc.
See documentation here: https://www.raylib.com/, and examples here: https://www.raylib.com/examples.html
*/


//This is Eliott Mathias' Copy
//Student number 101631449
//Proof that I wrote the code and not an AI.

#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <vector>

int screenWidth = 1200;
int screenHeight = 800;

const unsigned int TARGET_FPS = 50;

class PhysicsBody {
public:
    Vector2 position = Vector2{ 0,0 };
    Vector2 velocity = Vector2{ 0,0 };
    float mass = 1.0f;
    float drag = 0.0f;
    float gravityScale = 1.0f;
    Color color = YELLOW;
    float radius = 10.0f;
};

bool IsOverlapping(PhysicsBody a, PhysicsBody b)
{
    Vector2 disp = b.position - a.position;
    float dist = Vector2Length(disp);
    float sumRadius = a.radius + b.radius;
    return dist < sumRadius;
}

//Class that containts global properties of the physics simulation
class PhysicsSimulation
{
public:
    std::vector<PhysicsBody> bodies;
    Vector2 gravity = Vector2{ 0, 200 };
    const float FIXED_DELTA_TIME = 1.0f / TARGET_FPS;
    void checkCollisions()
    {
        for (int i = 0; i < bodies.size(); i++)
        {
            bodies[i].color = GREEN;
        }
        for (int i = 0; i < bodies.size(); i++)
        {
            for (int j = i + 1; j < bodies.size(); j++)
            {
                if (IsOverlapping(bodies[i], bodies[j]))
                {
                    bodies[i].color = RED;
                    bodies[j].color = RED;
                }
            }
        }
    }
    void Update()
    {
        //Update Positions
        for (int i = 0; i < bodies.size(); i++)
        {
            //velocity
            bodies[i].position += bodies[i].velocity * FIXED_DELTA_TIME;
            //acceleration
            bodies[i].velocity += gravity * bodies[i].gravityScale * FIXED_DELTA_TIME;

            //Apply drag to counteract velocity 
            bodies[i].velocity *= 1.0f - (bodies[i].drag * FIXED_DELTA_TIME);

        }

        checkCollisions();
    }

    void Draw()
    {
        for (int i = 0; i < bodies.size(); i++)
        {
            DrawCircleV(bodies[i].position, bodies[i].radius, bodies[i].color);
        }
    }

    void AddBody(PhysicsBody body)
    {
        bodies.push_back(body);
    }

    //private:
        //std::vector<PhysicsBody> bodies;
};

//PhysicsBody bird;
PhysicsSimulation sim;

Vector2 launchposition; //where the bird is launched from
float launchPositionAdjusmentSpeed = 100.0f;
float launchSpeed = 0.0f; //Speed of the bird when launched
float launchAngle = 0.0f; //Angle of the bird when launched
float spawnDrag = 0.0f;
float spawnRadius = 10.0f;

int main()
{
    InitWindow(screenWidth, screenHeight, "Physics-1");
    SetTargetFPS(TARGET_FPS);


    PhysicsBody target;
    target.radius = 50;
    target.position = { 900, (float)(screenHeight - 200) };
    target.gravityScale = 0.0f;
    sim.AddBody(target);

    launchposition = { 30, (float)(screenHeight - 50) };

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(Color{ 0,100,166,255 });
        //GUI
        DrawRectangle(0, 0, 400, 200, Color{ 100,255,255,50 });
        GuiSlider(Rectangle{ 100,0,100,20 }, "Launch Speed", TextFormat("%.2f", launchSpeed), &launchSpeed, 1, 1000);
        GuiSlider(Rectangle{ 100,30,100,20 }, "Launch Angle", TextFormat("%.2f", launchAngle), &launchAngle, -90, 0);
        GuiSlider(Rectangle{ 100,60,100,20 }, "Gravity", TextFormat("%.2f", sim.gravity.y), &sim.gravity.y, 500, 1000);
        GuiSlider(Rectangle{ 100,90,100,20 }, "Drag", TextFormat("%.2f", spawnDrag), &spawnDrag, 0, 10);
        GuiSlider(Rectangle{ 100,120,100,20 }, "Radius", TextFormat("%.2f", spawnRadius), &spawnRadius, 2, 50);

        if (IsKeyDown(KEY_UP)) {
            launchposition.y -= launchPositionAdjusmentSpeed * GetFrameTime();
        }
        if (IsKeyDown(KEY_DOWN)) {
            launchposition.y += launchPositionAdjusmentSpeed * GetFrameTime();
        }

        Vector2 velocityPreview = { cosf(launchAngle * DEG2RAD) * launchSpeed, sinf(launchAngle * DEG2RAD) * launchSpeed }; // uses speed and angle

        DrawCircleV(launchposition, 5, BROWN);
        DrawLineEx(launchposition, launchposition + velocityPreview, 1, RED);

        if (IsKeyPressed(KEY_SPACE))
        {
            PhysicsBody birdToLaunch;
            birdToLaunch.color = RED;
            birdToLaunch.radius = 10;
            birdToLaunch.position = launchposition;
            birdToLaunch.velocity = velocityPreview;
            birdToLaunch.drag = spawnDrag;
            birdToLaunch.radius = spawnRadius;

            sim.AddBody(birdToLaunch);
        }

        sim.Update();

        //Draw the bird
        //DrawCircleV(bird.position, 10, RED);
        sim.Draw();

        //GuiSliderBar(Rectangle{ 60, 5, 1000, 10 }, "Time", TextFormat("%.2f", time), &time, 0, 240);


        EndDrawing();
    }

    CloseWindow();
    return 0;
}
