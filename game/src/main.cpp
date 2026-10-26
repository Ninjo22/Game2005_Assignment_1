/*
This project uses the Raylib framework to provide us functionality for math, graphics, GUI, input etc.
See documentation here: https://www.raylib.com/, and examples here: https://www.raylib.com/examples.html
*/


//This is Eliott Mathias' Copy
//Student number 101631449
//Proof that I wrote the code and not an AI.
//No I didnt just copy this disclamer from my lab code
#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <vector>


int screenWidth = 1200;
int screenHeight = 800;

const unsigned int TARGET_FPS = 50;
//Line 22 to 100 i am using from my Lab assigment 3 submission as the physics sim logic is the same
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
};// Comment here in case the possision in the code changes and this is no longer line 100

int main()
{
    InitWindow(screenWidth, screenHeight, "Physics-1");
    SetTargetFPS(TARGET_FPS);
    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(WHITE);
            DrawText("Hello world!", 10, 10, 20, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
