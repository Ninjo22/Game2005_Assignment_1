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

Vector2 launchposition; //Start position of the parcel
float launchSpeed = 120.0f; //Starting launch speed, vi in the assignment
float launchAngle = 0.0f; //Starting launch angle, theta in the assignment
float spawnRadius = 10.0f; //Here just for display, irrelevent to assignment

const unsigned int TARGET_FPS = 50;

class PhysicsBody {
public:
    Vector2 position = Vector2{ 0,0 };
    Vector2 velocity = Vector2{ 0,0 };
    float mass = 1.0f;
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
            bodies[i].velocity *= 1.0f - FIXED_DELTA_TIME;

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
        DrawRectangle(0, 0, 400, 200, Color{ 100,255,255,50});
        
        
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
