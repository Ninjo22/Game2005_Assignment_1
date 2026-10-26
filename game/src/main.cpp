/*
This project uses the Raylib framework to provide us functionality for math, graphics, GUI, input etc.
See documentation here: https://www.raylib.com/, and examples here: https://www.raylib.com/examples.html
*/


//This is Eliott Mathias' Copy
//Student number 101631449
//Proof that I wrote the code and not an AI.
//Why is the output and input different?
//The input is the theoretical values calculated,
//While the output is the measured values from the simulation. 
//The discreptency we see is due to the simulation not being perfect and rounding certain values.
//The framerate also causes object to keep moving even after hitting the ground, 
//Which is why we see a range of 2.4 for the angle 0 even thought it should be 0.

#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <vector>
#include <fstream>
#include <iostream>

int screenWidth = 1200;
int screenHeight = 800;

Vector2 launchposition; //Start position of the parcel
float launchSpeed = 120.0f; //Starting launch speed, vi in the assignment
float launchAngle = 0.0f; //Starting launch angle, theta in the assignment
float spawnRadius = 10.0f; //Here just for display, irrelevent to assignment

const unsigned int TARGET_FPS = 50;

//Define the struct to hold all the recored values per Part A requirements
struct TestRecord
{
    double angle;
    double Range;
    double time;

};

class PhysicsBody {
public:
    Vector2 position = Vector2{ 0,0 };
    Vector2 velocity = Vector2{ 0,0 };
    float mass = 1.0f;
    float gravityScale = 1.0f;
    Color color = YELLOW;
    float radius = 10.0f;
    bool active = true;
    float flightTime = 0.0f;
    int id = 0;
};

//Class that containts global properties of the physics simulation
class PhysicsSimulation
{
public:
    std::vector<PhysicsBody> bodies;
    Vector2 gravity = Vector2{ 0, 20.0f};
    const float FIXED_DELTA_TIME = 1.0f / TARGET_FPS;
    void Update(std::vector<TestRecord>& outputRecords, float groundY)
    {
        for (size_t i = 0; i < bodies.size(); i++)
        {
            if (!bodies[i].active) continue;

            // Update position and velocity
            bodies[i].position += bodies[i].velocity * FIXED_DELTA_TIME;
            bodies[i].velocity += gravity * bodies[i].gravityScale * FIXED_DELTA_TIME;

            bodies[i].flightTime += FIXED_DELTA_TIME;

            // Check if projectile hits the ground (groundY)
            if (bodies[i].position.y >= groundY)
            {
                bodies[i].position.y = groundY; // Snap to ground
                bodies[i].active = false;

                // Calculate range as horizontal distance from launch position
                double measuredRange = (double)(bodies[i].position.x - launchposition.x);

                // Store in output record
                if (bodies[i].id >= 0 && bodies[i].id < 10)
                {
                    outputRecords[bodies[i].id].Range = measuredRange;
                    outputRecords[bodies[i].id].time = (double)bodies[i].flightTime;
                }
            }
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

//Function to write the CSV file of both input and output records
void WriteCSV(const std::string& filename, const std::vector<TestRecord>& records)
{
    std::ofstream file(filename);
    file << "Angle,Range,Time\n";
    for (const auto& r : records)
    {
        file << r.angle << "," << r.Range << "," << r.time << "\n";
    }
    file.close();
}

int main()
{
	InitWindow(screenWidth, screenHeight, "Physics-Assignment-1");
    SetTargetFPS(TARGET_FPS);
    launchposition = { 100, (float)(screenHeight - 100) };
    float groundY = launchposition.y;

    //Math for Part A
    const double vi = 120.0; //Initial velocity
    const double g = 20.0; //Gravity  
    double ranges[10] = { 0.0, 80.0, 160.0, 240.0, 320.0, 400.0, 480.0, 560.0, 640.0, 720.0 };
    //All provided ranges for each testrun


    std::vector<TestRecord> inputRecords(10);
    std::vector<TestRecord> outputRecords(10);

    for (int i = 0; i < 10; i++)
    {
        // Calculate launch angle from range
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
    WriteCSV("input.csv", inputRecords);

    bool simulationTriggered = false;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(Color{ 0,100,166,255 });
        DrawLine(0, (int)groundY, screenWidth, (int)groundY, GREEN);

        DrawCircleV(launchposition, 5, BROWN);

        sim.Draw();

        if (!simulationTriggered) 
        {
            DrawText("Press space to begin", 50, 50, 20, WHITE);
        }
        if (IsKeyPressed(KEY_SPACE) && !simulationTriggered)
        {
            simulationTriggered = true;
            for (int i = 0; i < 10; i++)
            {
                PhysicsBody parcel;
                parcel.id = i;
                parcel.color = RED;
                parcel.radius = spawnRadius;
                parcel.position = launchposition;

                // Calculate velocity components using input record angle
                float angle = (float)inputRecords[i].angle;
                parcel.velocity = {
                    cosf(angle) * launchSpeed,
                    -sinf(angle) * launchSpeed // Negative because screen Y goes down
                };

                sim.AddBody(parcel);
            }
        }

        if (simulationTriggered)
        {
            sim.Update(outputRecords, groundY);

            // Once all bodies are done, write output.csv automatically
            static bool csvWritten = false;
            if (!csvWritten) {
                bool allFinished = true;
                for (const auto& b : sim.bodies) {
                    if (b.active) allFinished = false;
                }
                if (allFinished && !sim.bodies.empty()) {
                    WriteCSV("output.csv", outputRecords);
                    csvWritten = true;
                }
            }
        }
        EndDrawing();

    }

    CloseWindow();
    return 0;
}
