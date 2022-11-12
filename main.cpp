#include "raylib.h"
#include "rlgl.h" //needed for skybox
#include "raymath.h" //needed for Pitch/Yaw Calculation & application

//Enemy Ship Struct
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
    bool SE1colliding = false;
    bool SE2colliding = false;
    bool SE3colliding = false;
    bool SGcolliding = false;
    float finalTime = 0.0f;
    bool GetFinalTime = false;
    bool GameOver = false;
    bool Alive = true;
    
    //Enemy Position Vector Array
    Vector3 EPosc[3] = {(Vector3){0.0f, 0.0f, -40.0f}, (Vector3){-5.0f,0.0f,-80.0f}, (Vector3){0.0f,0.0f,-160.0f}};
   
    
    
    
    //check if music is being played
    bool musicPlaying = false;

     // Music
    //--------------------------------------------------------------------------------------
    InitAudioDevice(); // Initialises Audio Device
    Music backgroundMusic = LoadMusicStream("resources/BackgroundMusic.mp3"); // Imports Background Music File
    backgroundMusic.looping = true; //Loops background music
    PlayMusicStream(backgroundMusic); //Plays Background Music
    
    //Import Sound(s)
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
    
    
    //Player Ship
    Model Ship = LoadModel("Resources/Ship.obj");                   // Loads Ship model from resource folder
    Texture2D shipTexture = LoadTexture("Resources/ShipTextures.png"); // Load model texture
    Ship.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = shipTexture;            // Set model diffuse texture
    Vector3 shipPosition = { 0.0f, 0.0f, 0.0f }; // Sets Ship Inital Position
    
    

    //Adding Eneny Ships from Sturct, Adding texure and giving a position to each ship from the EnemyPosition Array
    EnemyShip EnemyShip1;
   
    EnemyShip1.EModel = LoadModel("Resources/Ship.obj");   //Loads Model
    EnemyShip1.EModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTexture("resources/ShipTextures.png"); //Loads and applys texture to model
    EnemyShip1.pos = {EPosc[0].x,EPosc[0].y,EPosc[0].z}; //Gets and sets position from EnemyPosition Array

    EnemyShip EnemyShip2;
   
    EnemyShip2.EModel = LoadModel("Resources/Ship.obj");   
    EnemyShip2.EModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTexture("resources/ShipTextures.png");
    EnemyShip2.pos = {EPosc[1].x,EPosc[1].y,EPosc[1].z};

    EnemyShip EnemyShip3;
   
    EnemyShip3.EModel = LoadModel("Resources/Ship.obj");   
    EnemyShip3.EModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTexture("resources/ShipTextures.png");
    EnemyShip3.pos = {EPosc[2].x,EPosc[2].y,EPosc[2].z};

    Model Goal = LoadModel("Resources/Goal.obj");                   // Loads Goal model from resource folder
    Vector3 GoalPosition = { 1.0f, 0.0f, -450.0f }; //Position vector to set goals Position in game


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
            UpdateMusicStream(backgroundMusic); //Updates music, Needed for music to be audible for more thant 1 frame.
            
        // MOVEMENT 

        //Character Can only move if the game is NOT over
        if(GameOver== false && Alive == true)
        {
                //Moves character Right. Checks for D key Press.
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

            //Moves character Left. Checks for A key Press.
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
            
            //Checks for W key Press. Increases characters Speed. Maxes out at 0.5. Character will always be moving.
            if(IsKeyDown(KEY_W))
            {
                 if(characterVelocity<0.5f)
                 {
                    characterVelocity += 0.01f;
                 }
                 //temporary movement code to debug 
                 //shipPosition.z -=0.3f;
                 
            }
            //checks for S key press. Decreases Character Speed. Bottoms out at 0.1. Character will always be moving.
            if(IsKeyDown(KEY_S))
            {
                 if(characterVelocity>0.1f)
                 {
                    characterVelocity -= 0.01f;
                 }
                 //temporary movement code to debug 
                 //shipPosition.z +=0.3f;
                 
            }
            
            

        }
        //applies transformation to yaw of ship. Makes it look life the ship is drifting slightly ingame.
        Ship.transform = MatrixRotateXYZ((Vector3){ DEG2RAD*pitch, DEG2RAD*yaw, DEG2RAD*roll });

        //Moves Enemy Ship
        EnemyShip1.pos.z-=0.1f;
        EnemyShip2.pos.z-=0.15f;
        EnemyShip3.pos.z-=0.2f;


        //Moves Player Ship foward constantly. 
        shipPosition.z-=characterVelocity;
       

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing(); //Begins Drawing the Game Scene

            ClearBackground(RAYWHITE); //Clears background. Draw a clear white background 
            
            //Cameras position and target are based of the characters position so it always follows the character
            camera.position = (Vector3){ shipPosition.x, shipPosition.y+10.0f, shipPosition.z+10.0f };  // Camera position
            camera.target = (Vector3){ shipPosition.x, shipPosition.y, shipPosition.z};      // Camera looking at point
            


            BeginMode3D(camera); //Sets the camera object i created to be the camera for the game.
               
                
                //Drawing Skybox
                rlDisableBackfaceCulling(); //Disables back face culling so texture can be displayed on the inside of the object.
                    DrawModel(skybox, (Vector3){0, 0, 0}, 500.0f, WHITE);
                rlEnableBackfaceCulling(); //Re-Enables backface culling so only the skybox model has backface culling disables.

                
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
                 //Creates and draws to screen bounding box around model.
                BoundingBox trackBounds = (BoundingBox){(Vector3){ trackPosition.x - 10.0f,
                                     trackPosition.y,
                                     trackPosition.z - 800.0f },
                          (Vector3){ trackPosition.x + 10.0f,
                                     trackPosition.y +1.0f,
                                     trackPosition.z + 10.0f }};
                DrawBoundingBox(trackBounds, YELLOW); 


                //Draws GoalModel
                DrawModel(Goal, GoalPosition, 2.4f, YELLOW);
                BoundingBox GoalBounds = (BoundingBox){(Vector3){ GoalPosition.x - 12.0f,
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

            //Checks if player is colliding with track, Draws text if they are. Debug Code.
            if(STcolliding == true)
            {
                DrawText("Colliding /w Track", 10, 70, 20, ORANGE);
                
            }

            //if you not colliding with the track you will fall
            if(STcolliding == false)
            {
                shipPosition.y-=0.4f;
            }
            //Checks if you are colliding with the enemy while the game is not over. (Loss Condition)
            if((SE1colliding == true || SE2colliding == true|| SE3colliding == true) && Alive == true && GameOver == false)
            {
                DrawText("Colliding /w Enemy", 10, 90, 20, ORANGE);
                PlaySoundMulti(EnemyHit);
                Alive = false;
                characterVelocity=0.0f;
                
                
            }
            //Checks if you are colliding with the Gate and the game is not over (Win Condition). Stops the characters velocity once they hit the win condition.
            if(SGcolliding == true && Alive == true && GameOver == false)
            {
                GetFinalTime = true;
                PlaySoundMulti(Victory); //Plays Win Condition Sound
                characterVelocity=0.0f; // Sets character velocity to 0. Stops movement once game is over.
                
            }
            //Checks if character is alive (Loss Condition) Draw text on screen to let player know they lost
            if(Alive == false)
            {
                DrawText("You Lose", 270, 70, 50, RED); //Draws loss condition text
            }

            //kill trigger if you fall off track (Loss Condition)
            if(shipPosition.y<-10.0f && Alive == true)
            {
                Alive = false;
                PlaySoundMulti(ScreamAndExplosion); // Plays sound.
                
            }
            //Draws elaped time on screen durign gameplay. Stops drawing once the game is over.
            if(GetFinalTime == false && GameOver == false)
            {
             DrawText(TextFormat("Elapsed Time: %02.02f", GetTime()), 10, 370, 30, ORANGE);
            }
            //When get final time is true this code gets the elpased time of the game once and then stores it in a variable while putting the game in it gameover state
            if(GetFinalTime == true)
            {
                finalTime = GetTime(); //Function used to get elapsed time in application. Stored to be displayed on victory screen.
                
                GetFinalTime = false;
                GameOver = true;
                
            }
            //Win Condition. Triggers when player makes it to goal post and is stil alive. Draws Win con text and final time of the player on screen.
            if(GetFinalTime == false && GameOver == true && Alive == true)
            {
                //Draws Final Time On Screen
                DrawText("You Win!", 300, 150, 40, GREEN);
                DrawText(TextFormat("Final Time: %02.02f", finalTime), 250, 200, 40, GREEN);
            }
            
            //check if music is being played
            musicPlaying = IsMusicStreamPlaying(backgroundMusic);
            //Draws text to screen. 1 = Music is playing, 0 = No Music playing. Used to Debug Background Music
            DrawText(TextFormat("Music Playing: %i", musicPlaying), 10, 330, 30, ORANGE);

             // Collision
            //----------------------------------------------------------------------------------
            //Checks Collision between bounding boxes track and ship
            STcolliding = CheckCollisionBoxes(shipBounds, trackBounds);    
            //Checks Collision between bounding boxes ship and enemy ship(s)
            SE1colliding = CheckCollisionBoxes(shipBounds, eship1Bounds);
            SE2colliding = CheckCollisionBoxes(shipBounds, eship2Bounds);  
            SE3colliding = CheckCollisionBoxes(shipBounds, eship3Bounds);    
            //Checks Collision between bounding boxes ship and goal
            SGcolliding = CheckCollisionBoxes(shipBounds, GoalBounds);                     

            
        

        EndDrawing(); //Ends Drawing to Scene

         
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadMusicStream(backgroundMusic);   // Unload music stream buffers from RAM

    CloseAudioDevice();         // Close audio device (music streaming is automatically stopped)
    UnloadTexture(trackTexture); // unloads Texture of Track
    UnloadTexture(shipTexture); // unloads Texture of Ship
    UnloadTexture(EnemyShip1.EModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture); //Unloads Texture of Enemy Ship 1
    UnloadTexture(EnemyShip2.EModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture); //Unloads Texture of Enemy Ship 2
    UnloadTexture(EnemyShip3.EModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture); //Unloads Texture of Enemy Ship 3
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0; //End of Program
}

//References
/*

    Initial start point - Raylibs 3D Camera mode Example - Used to quickly set up a start point for the project. - Referenced for how to set up a 3d Camera and world in raylib.
    https://www.raylib.com/examples.html

    Displaying text in raylib using textformat (Uses %i for intergers while i use %f for floats) - Used for drawing the characters speed on screen.
    https://stackoverflow.com/questions/53500874/how-to-show-variables-in-text-in-raylib

    Raylib Cheat sheet was referenced for what functions were available in Raylib and how they Work
    https://www.raylib.com/cheatsheet/cheatsheet.html

    Raylib Example page was referenced for how certain functions worked (Sounds, Background Music, Adding Models / Textures, Pitch/Yaw, Skybox)
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
    Music REQUIRES the updateMusicStream() function. this was why i could not hear music as it was only playing music on the first frame. 
    

    Game Controls:
    W - Speed up
    S - Slow down
    A - Turn Left
    D - Turn Right

    Objective: Race to the Finish
    Win Con: Make it to the Goal
    Loss Con: Fall off track / Collide with Other Ships



 */