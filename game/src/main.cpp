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

//Class that containts global properties of the physics simulation
class PhysicsSimulation
{
public:
    std::vector<PhysicsBody> bodies;
    Vector2 gravity = Vector2{ 0, 200 };
    const float FIXED_DELTA_TIME = 1.0f / TARGET_FPS;
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

};
//PhysicsBody;
PhysicsSimulation sim;

//Define the struct to hold all the recored values per Part A requirements
struct TestRecord
{
    double angle;
    double Range;
    double time;

};

int main()
{
	InitWindow(screenWidth, screenHeight, "Physics-Assignment-1");
    SetTargetFPS(TARGET_FPS);


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

        //Math for Part A
        const double vi = 120.0; //Initial velocity
        const double g = 20.0; //Gravity  
        double ranges[10] = { 0.0, 80.0, 160.0, 240.0, 320.0, 400.0, 480.0, 560.0, 640.0, 720.0 };
        //All provided ranges for each testrun


        std::vector<TestRecord> inputRecords(10);
        std::vector<TestRecord> outputRecords(10);

        for (int i = 0; i < 10; i++)
        {
            // Calculate launch angle from range: theta = 0.5 * arcsin((R * g) / (vi^2))
            double ratio = (ranges[i] * g) / (vi * vi);
            if (ratio > 1.0) ratio = 1.0; // safety clamp
            double angleRad = 0.5 * asin(ratio);

            inputRecords[i].angle = angleRad;

            // Calculate theoretical input range and flight-time using formulas
            inputRecords[i].time = (2.0 * vi * sin(angleRad)) / g;
            inputRecords[i].Range = (vi * vi * sin(2.0 * angleRad)) / g;

            //Setup output records with angles since they are the only one that will be the same across both in and output
            outputRecords[i].angle = inputRecords[i].angle;
            outputRecords[i].Range = 0.0;
            outputRecords[i].time = 0.0;
        }

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

        sim.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
