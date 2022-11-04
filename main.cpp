#include "raylib.h"
#include "rlgl.h" //needed for skybox
#include "raymath.h" //needed for Pitch/Yaw Calculation & application


struct EnemyShip
{
  Vector3 pos;
  float EnemyVelocity;
  Model EModel;
  Texture2D Texture;
  BoundingBox eBounds;
};

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    float characterVelocity = 0.1f;
    float yaw = 0.0f;
    float pitch = 0.0f;
    float roll = 0.0f;
    bool STcolliding = false;
    //bool SEcolliding = false;
    bool SE1colliding = false;
    bool SE2colliding = false;
    bool SE3colliding = false;
    bool SGcolliding = false;
    float finalTime = 0.0f;
    bool GetFinalTime = false;
    bool GameOver = false;
    bool Alive = true;
    
    //Enemy Position Vector
    Vector3 EPosc[3] = {(Vector3){0.0f, 0.0f, -20.0f}, (Vector3){-5.0f,0.0f,-40.0f}, (Vector3){0.0f,0.0f,-60.0f}};
   
    
    
    
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


    // Adds Model for skybox & texture (Is just a cube)
    Model skybox = LoadModel("resources/cube.obj");
    skybox.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTexture("resources/Space.png");


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
    
    /*
    //Enemy Ship
    Model eShip = LoadModel("Resources/Ship.obj");                   // Loads Ship model from resource folder
    Texture2D eshipTexture = LoadTexture("resources/ShipTextures.png"); // Load model texture
    eShip.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = eshipTexture;            // Set model diffuse texture
    //Postion is deteminded by array of vectors EPosc
    Vector3 eshipPosition = { 0.0f, 0.0f, -20.0f };
    */

    //Adding Eneny Ships from Sturct
    EnemyShip EnemyShip1;
   
    EnemyShip1.EModel = LoadModel("Resources/Ship.obj");   
    EnemyShip1.EModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTexture("resources/ShipTextures.png");
    EnemyShip1.pos = {EPosc[0].x,EPosc[0].y,EPosc[0].z};

    EnemyShip EnemyShip2;
   
    EnemyShip2.EModel = LoadModel("Resources/Ship.obj");   
    EnemyShip2.EModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTexture("resources/ShipTextures.png");
    EnemyShip2.pos = {EPosc[1].x,EPosc[1].y,EPosc[1].z};

    EnemyShip EnemyShip3;
   
    EnemyShip3.EModel = LoadModel("Resources/Ship.obj");   
    EnemyShip3.EModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTexture("resources/ShipTextures.png");
    EnemyShip3.pos = {EPosc[2].x,EPosc[2].y,EPosc[2].z};

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
            Add Player Velocity and Velocity Control *
            Add Collision (Floor & Goal post) *
            Add Gravity *
            Add Textures (Examples on raylib, Check Cheatsheet) * - See Notes for Important info
            Add Background Music * - See Notes for Important info
            Add Sounds (Examples on Raylib, Check Cheatsheet) * (Was missing updateMusicStream() - Needs to be there as music is checked every frame)
            Add Win/loss Condition (Position based) (Needs Collision) *
            Add Pitch & Yaw for turning (Extra) (Example on raylib) *
            Draw on Screen(Game objective, Player Velocity,elapsed time, Etc) *
            Add Enemies *
            Refeactor Code ~ Tidy up code based on feedback 
            Enemy Struct - Position, Model, Speed, Texture, BoundingBox *
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
                 if(characterVelocity<0.5f)
                 {
                    characterVelocity += 0.01f;
                 }
                 //temporary movement code to debug 
                 //shipPosition.z -=0.3f;
                 
            }
            //Decreases Character Speed. Bottoms out at 1. Character will always be moving.
            if(IsKeyDown(KEY_S))
            {
                 if(characterVelocity>0.1f)
                 {
                    characterVelocity -= 0.01f;
                 }
                 //temporary movement code to debug 
                 //shipPosition.z +=0.3f;
                 
            }
            if(IsKeyDown(KEY_L))
            {
                GetFinalTime = true;
            }

        }
            //applies transformation to yaw of ship.
        Ship.transform = MatrixRotateXYZ((Vector3){ DEG2RAD*pitch, DEG2RAD*yaw, DEG2RAD*roll });

        //Moves Enemy Ship
        //eshipPosition.z-=0.1f;
        EnemyShip1.pos.z-=0.1f;
        EnemyShip2.pos.z-=0.15f;
        EnemyShip3.pos.z-=0.2f;


        //Moves Player Ship
        shipPosition.z-=characterVelocity;
       

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
                
                //Drawing Skybox
                rlDisableBackfaceCulling();
                //rlDisableDepthMask();
                    DrawModel(skybox, (Vector3){0, 0, 0}, 500.0f, WHITE);
                rlEnableBackfaceCulling();
                //rlEnableDepthMask();
                
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

                /*
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
                */

                //Draws GoalModel
                DrawModel(Goal, GoalPosition, 2.4f, YELLOW);
                BoundingBox GoalBounds = (BoundingBox){(Vector3){ GoalPosition.x - 10.0f,
                                     GoalPosition.y,
                                     GoalPosition.z + 5.0f },
                          (Vector3){ GoalPosition.x + 10.0f,
                                     GoalPosition.y +10.0f,
                                     GoalPosition.z + 10.0f }};
                DrawBoundingBox(GoalBounds, YELLOW);
                
                //Draw Enemy Ship /Now Using data from struct
                 DrawModel(EnemyShip1.EModel,EnemyShip1.pos, 1.0f, WHITE);
                BoundingBox eship1Bounds = (BoundingBox){(Vector3){ EnemyShip1.pos.x - 2.5f,
                                     EnemyShip1.pos.y,
                                     EnemyShip1.pos.z - 2.5f },
                          (Vector3){ EnemyShip1.pos.x + 2.5f,
                                     EnemyShip1.pos.y +2.5f,
                                     EnemyShip1.pos.z + 2.5f }};
                DrawBoundingBox(eship1Bounds, BLUE);

                DrawModel(EnemyShip2.EModel,EnemyShip2.pos, 1.0f, WHITE);
                BoundingBox eship2Bounds = (BoundingBox){(Vector3){ EnemyShip2.pos.x - 2.5f,
                                     EnemyShip2.pos.y,
                                     EnemyShip2.pos.z - 2.5f },
                          (Vector3){ EnemyShip2.pos.x + 2.5f,
                                     EnemyShip2.pos.y +2.5f,
                                     EnemyShip2.pos.z + 2.5f }};
                DrawBoundingBox(eship2Bounds, RED);

                 DrawModel(EnemyShip3.EModel,EnemyShip3.pos, 1.0f, WHITE);
                BoundingBox eship3Bounds = (BoundingBox){(Vector3){ EnemyShip3.pos.x - 2.5f,
                                     EnemyShip3.pos.y,
                                     EnemyShip3.pos.z - 2.5f },
                          (Vector3){ EnemyShip3.pos.x + 2.5f,
                                     EnemyShip3.pos.y +2.5f,
                                     EnemyShip3.pos.z + 2.5f }};
                DrawBoundingBox(eship3Bounds, PURPLE);

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

            if((SE1colliding == true || SE2colliding == true|| SE3colliding == true) && Alive == true && GameOver == false)
            {
                DrawText("Colliding /w Enemy", 10, 90, 20, ORANGE);
                PlaySoundMulti(EnemyHit);
                Alive = false;
                characterVelocity=0.0f;
                
                
            }
            if(SGcolliding == true && Alive == true && GameOver == false)
            {
                GetFinalTime = true;
                PlaySoundMulti(Victory);
                characterVelocity=0.0f;
                
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
            //SEcolliding = CheckCollisionBoxes(shipBounds, eshipBounds);  
            SE1colliding = CheckCollisionBoxes(shipBounds, eship1Bounds);
            SE2colliding = CheckCollisionBoxes(shipBounds, eship2Bounds);  
            SE3colliding = CheckCollisionBoxes(shipBounds, eship3Bounds);    
            SGcolliding = CheckCollisionBoxes(shipBounds, GoalBounds);                     

            
        

        EndDrawing();

         
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadMusicStream(backgroundMusic);   // Unload music stream buffers from RAM

    CloseAudioDevice();         // Close audio device (music streaming is automatically stopped)
    UnloadTexture(trackTexture); // unloads Texture of Track
    UnloadTexture(shipTexture); // unloads Texture of Ship
    //UnloadTexture(eshipTexture); // unloads Texture of Enemy Ship
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

    Skybox Image (Space.png)
    https://blenderartists.org/t/space-background/660743

    
    *Notes*
    Remove materials in Blender before exporting models in .obj, Keeping the material info messes up the models in raylib*
    Music is implemented and should be working correctly. - Music is playing according to IsMusicStreamPlaying() but nothing can be heard. Sounds work tho.



 */