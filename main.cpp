#include "raylib.h"
#include "raymath.h" //needed for Pitch/Yaw Calculation & application
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    float characterVelocity = 2.0f;
    float yaw = 0.0f;
    float pitch = 0.0f;
    float roll = 0.0f;
    bool STcolliding = false;
    bool SEcolliding = false;
    bool SGcolliding = false;
    float finalTime = 0.0f;
    bool GetFinalTime = false;
    bool GameOver = false;
    bool Alive = true;
    bool Working = false;
    //Test
    //check if music is being played
    bool musicPlaying = false;

     // Music
    //--------------------------------------------------------------------------------------
    InitAudioDevice(); // Initialises Audio Device
    Music backgroundMusic = LoadMusicStream("resources/BackgroundMusic.mp3");
    backgroundMusic.looping = true;
    PlayMusicStream(backgroundMusic);
    backgroundMusic.looping = true;
    
    //Adds Sound(s)
    Sound EnemyHit = LoadSound("resources/crash.wav");
    Sound ScreamAndExplosion = LoadSound("resources/ScreamAndExplosion.wav");
    Sound Victory = LoadSound("resources/Victory.wav");

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera mode");
    // Define the camera to look into our 3d world
    // Camera position & target added to Main game loop so it constantly updates its position as it needs to follow the player
    Camera3D camera = { 0 };
    camera.up = (Vector3){ 0.0f, 0.5f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 60.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

    Model Track = LoadModel("Resources/Track.obj");                 // Loads track model from resource folder
    Texture2D trackTexture = LoadTexture("Resources/Track_Texture.png"); // Load model texture
    Track.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = trackTexture;            // Set model diffuse texture
    Vector3 trackPosition = { 0.0f, -1.0f, 0.0f };
    

    //Bounding box for track - based on tracks initial size. may need to scale depending on scale needed for game.
    
    //Player Ship
    Model Ship = LoadModel("Resources/Ship.obj");                   // Loads Ship model from resource folder
    Texture2D shipTexture = LoadTexture("Resources/ShipTextures.png"); // Load model texture
    Ship.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = shipTexture;            // Set model diffuse texture
    Vector3 shipPosition = { 0.0f, 0.0f, 0.0f };
    //Bounding box for ship - based on ships initial size. may need to scale depending on scale needed for game.
    
    //Enemy Ship
    Model eShip = LoadModel("Resources/Ship.obj");                   // Loads Ship model from resource folder
    Texture2D eshipTexture = LoadTexture("resources/ShipTextures.png"); // Load model texture
    eShip.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = eshipTexture;            // Set model diffuse texture
    Vector3 eshipPosition = { 0.0f, 0.0f, -20.0f };

    Model Goal = LoadModel("Resources/Goal.obj");                   // Loads Ship model from resource folder
    //Texture2D GoalTexture = LoadTexture("resources/GoalTextures.png"); // Load model texture
    //Goal.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = GoalTexture;            // Set model diffuse texture
    Vector3 GoalPosition = { 1.0f, 0.0f, -250.0f };


    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    


    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO:
        /*
            * = Finished
            ~ = Partially implemented
            - = Issue

            Add Controls (Player input) *
            Add 3D Models (player character & Track) *
            Add Player Velocity and Velocity Control ~ Partially implemented, Will add last as its easy to debug.
            Add Collision (Floor & Goal post) *
            Add Gravity *
            Add Textures (Examples on raylib, Check Cheatsheet) * - See Notes for Important info
            Add Background Music ~ Added but you can't hear it.
            Add Sounds (Examples on Raylib, Check Cheatsheet) * (Was missing updateMusicStream() - Needs to be there as music is checked every frame)
            Add Win/loss Condition (Position based) (Needs Collision) *
            Add Pitch & Yaw for turning (Extra) (Example on raylib) *
            Draw on Screen(Game objective, Player Velocity,elapsed time, Etc) *
            Add Enemies ~ 1 enemy added. need to stream line adding enemies.
            Refeactor Code ~ Tidy up code based on feedback 
        */
        //----------------------------------------------------------------------------------
            UpdateMusicStream(backgroundMusic);
            
        // MOVEMENT 

            //Moves character Right
        if(GameOver== false && Alive == true)
        {
                if(IsKeyDown(KEY_D))
                {
                    shipPosition.x += 0.3f;
                    //yaw will only change if above a certain point. this avoids the ship constantly spinning if 'D' is held down.
                    if(yaw > -25.0f)
                    {
                        yaw -= 1.2f;
                    }
                
                }
                    //resets yaw to natural position after button has been released
                else
                {
                    if (yaw > 0.3f) yaw -= 0.5f;
                    else if (yaw < -0.3f) yaw += 0.5f;
                }

            //Moves character Left
            if(IsKeyDown(KEY_A))
            {
                shipPosition.x -= 0.3f;
                //yaw will only change if below a certain point. this avoids the ship constantly spinning if 'A' is held down.
                 if(yaw < 25.0f)
                {
                    yaw += 1.2f;
                }
            }
            //resets yaw to natural position after button has been released
            else
            {
                if (yaw > 0.3f) yaw -= 0.5f;
                else if (yaw < -0.3f) yaw += 0.5f;
            }
            
            //Increases characters Speed. Maxes out at 5. Character will always be moving.
            if(IsKeyDown(KEY_W))
            {
                 if(characterVelocity<5.0f)
                 {
                    characterVelocity += 0.1f;
                 }
                 //temporary movement code to debug 
                 shipPosition.z -=0.3f;
                 
            }
            //Decreases Character Speed. Bottoms out at 1. Character will always be moving.
            if(IsKeyDown(KEY_S))
            {
                 if(characterVelocity>1.0f)
                 {
                    characterVelocity -= 0.1f;
                 }
                 //temporary movement code to debug 
                 shipPosition.z +=0.3f;
                 
            }
            if(IsKeyDown(KEY_L))
            {
                GetFinalTime = true;
            }

        }
            //applies transformation to yaw of ship.
        Ship.transform = MatrixRotateXYZ((Vector3){ DEG2RAD*pitch, DEG2RAD*yaw, DEG2RAD*roll });

        eshipPosition.z-=0.1f;
       

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            //Cameras position and target are based of the characters position so it always follows the character
            camera.position = (Vector3){ shipPosition.x, shipPosition.y+10.0f, shipPosition.z+10.0f };  // Camera position
            camera.target = (Vector3){ shipPosition.x, shipPosition.y, shipPosition.z};      // Camera looking at point
            


            BeginMode3D(camera);
                //Vector3 cubePosition = {characterX, characterY, characterZ};
                //DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
                //DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);

                //DrawGrid(10, 1.0f);

                
                //Draws Ship Model
                DrawModel(Ship, shipPosition, 1.0f, WHITE);
                //Creates and draws to screen bounding box around model
                BoundingBox shipBounds = (BoundingBox){(Vector3){ shipPosition.x - 2.5f,
                                     shipPosition.y,
                                     shipPosition.z - 2.5f },
                          (Vector3){ shipPosition.x + 2.5f,
                                     shipPosition.y +2.5f,
                                     shipPosition.z + 2.5f }};
                DrawBoundingBox(shipBounds, YELLOW);
                                                           
               
                //Draws Track model
                DrawModel(Track, trackPosition, 1.0f, BLUE);
                 //Creates and draws to screen bounding box around model
                BoundingBox trackBounds = (BoundingBox){(Vector3){ trackPosition.x - 10.0f,
                                     trackPosition.y,
                                     trackPosition.z - 600.0f },
                          (Vector3){ trackPosition.x + 10.0f,
                                     trackPosition.y +1.0f,
                                     trackPosition.z + 10.0f }};
                DrawBoundingBox(trackBounds, YELLOW); 

                //Draws Enemy Ship
                DrawModel(eShip, eshipPosition, 1.0f, WHITE);
                //Creates and draws to screen bounding box around model
                BoundingBox eshipBounds = (BoundingBox){(Vector3){ eshipPosition.x - 2.5f,
                                     eshipPosition.y,
                                     eshipPosition.z - 2.5f },
                          (Vector3){ eshipPosition.x + 2.5f,
                                     eshipPosition.y +2.5f,
                                     eshipPosition.z + 2.5f }};
                DrawBoundingBox(eshipBounds, YELLOW);

                //Draws GoalModel
                DrawModel(Goal, GoalPosition, 2.4f, YELLOW);
                BoundingBox GoalBounds = (BoundingBox){(Vector3){ GoalPosition.x - 10.0f,
                                     GoalPosition.y,
                                     GoalPosition.z + 5.0f },
                          (Vector3){ GoalPosition.x + 10.0f,
                                     GoalPosition.y +10.0f,
                                     GoalPosition.z + 10.0f }};
                DrawBoundingBox(GoalBounds, YELLOW);

            EndMode3D();

            //Draws text on screen of objective in top left of the screen
            DrawText("Objective: Race To the Finish", 10, 30, 20, RED);

            //Draws text of Characters current speed on screen. Velocity is stores as a float.Text format and %f is used to display the float as text
            DrawText(TextFormat("Current Speed %f",characterVelocity), 10, 400, 25, RED);

            //Draws Games FPS in top left of screen
            DrawFPS(10, 10);

            if(STcolliding == true)
            {
                DrawText("Colliding /w Track", 10, 70, 20, ORANGE);
                
            }

            //if you not colliding with the track you will fall
            if(STcolliding == false)
            {
                shipPosition.y-=0.4f;
            }

            if(SEcolliding == true && Alive == true)
            {
                DrawText("Colliding /w Enemy", 10, 90, 20, ORANGE);
                PlaySoundMulti(EnemyHit);
                Alive = false;
                
                
            }
            if(SGcolliding == true && Alive == true && GameOver == false)
            {
                GetFinalTime = true;
                PlaySoundMulti(Victory);
                
            }

            if(Alive == false)
            {
                DrawText("You Lose", 270, 70, 50, RED);
            }

            //kill trigger if you fall off track
            if(shipPosition.y<-10.0f && Alive == true)
            {
                Alive = false;
                PlaySoundMulti(ScreamAndExplosion);
                
            }
            
            if(GetFinalTime == false && GameOver == false)
            {
             DrawText(TextFormat("Elapsed Time: %02.02f", GetTime()), 10, 370, 30, ORANGE);
            }

            if(GetFinalTime == true)
            {
                finalTime = GetTime();
                
                GetFinalTime = false;
                GameOver = true;
                
            }
            
            if(GetFinalTime == false && GameOver == true && Alive == true)
            {
                //Draws Final Time On Screen
                DrawText("You Win!", 300, 150, 40, GREEN);
                DrawText(TextFormat("Final Time: %02.02f", finalTime), 250, 200, 40, GREEN);
            }
            
            //check if music is being played
            musicPlaying = IsMusicStreamPlaying(backgroundMusic);
            DrawText(TextFormat("Music Playing: %i", musicPlaying), 10, 330, 30, ORANGE);

             // Collision
            //----------------------------------------------------------------------------------
            //Checks Collision between bounding boxes track and ship
            STcolliding = CheckCollisionBoxes(shipBounds, trackBounds);    
            SEcolliding = CheckCollisionBoxes(shipBounds, eshipBounds);  
            SGcolliding = CheckCollisionBoxes(shipBounds, GoalBounds);                     

        
        

        EndDrawing();

         
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadMusicStream(backgroundMusic);   // Unload music stream buffers from RAM

    CloseAudioDevice();         // Close audio device (music streaming is automatically stopped)
    UnloadTexture(trackTexture); // unloads Texture of track
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//References
/*

    Initial start point - Raylibs 3D Camera mode Example - Used to quickly set up a start point for the project. - Referenced for how to set up a 3d Camera and world in raylib.
    https://www.raylib.com/examples.html

    Displaying text in raylib using textformat (Uses %i for intergers while i use %f for floats) - Used for drawing the characters speed on screen.
    https://stackoverflow.com/questions/53500874/how-to-show-variables-in-text-in-raylib

    Raylib Cheat sheet was referenced for how certain functions in Raylib Work
    https://www.raylib.com/cheatsheet/cheatsheet.html

    Raylib pitch/Yaw Example was referenced to add rotation to ship - refactored for my own use
    https://www.raylib.com/examples.html

    Raylib Model loading example was referenced for adding models and their textures
    https://www.raylib.com/examples.html

    Crash Sound Effect
    https://freesound.org/people/FxKid2/sounds/367624/

    Scream And Explosion - 2 seperate sounds mixed together by me
    https://freesound.org/people/vmgraw/sounds/257691/
    https://freesound.org/people/Iwiploppenisse/sounds/156031/

    Victory Sound Effect
    https://freesound.org/people/SilverIllusionist/sounds/462250/

    Background Music - Megaman X4 Jet Stingray Stage  -Copyright Capcom
    https://www.youtube.com/watch?v=l4qUsOC8xUI

    
    *Notes*
    Remove materials in Blender before exporting models in .obj, Keeping the material info messes up the models in raylib*
    Music is implemented and should be working correctly. - Music is playing according to IsMusicStreamPlaying() but nothing can be heard. Sounds work tho.



 */