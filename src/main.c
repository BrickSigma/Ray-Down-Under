#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <globals.h>
#include <utilities.h>
#include <sprites.h>
#include <levels.h>

#ifdef PLATFORM_WEB
    #include <emscripten/emscripten.h>
#endif

// Update the screen
void Update(void);

// Draw the tiles
void DrawTiles(void);

void GameLoop(void);

int main(void)
{
    InitWindow(SCREEN_WIDTH*SCALE, SCREEN_HEIGHT*SCALE, "Ray Down Under");

    InitAudioDevice();
    SetMasterVolume(75);

    SetTargetFPS(FPS);
    jump = LoadSound("resources/jump.mp3");
    LeverFlipped = LoadSound("resources/lever flipped.mp3");
    hurt = LoadSound("resources/hurt.mp3");
    kill = LoadSound("resources/kill.mp3");

    screen = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    HomeBackground = LoadTexture("resources/home screen.png");
    EndBackground = LoadTexture("resources/end screen.png");
    start = LoadTexture("resources/start.png");
    TileSheet = LoadTexture("resources/tiles.png");
    BugSheet = LoadTexture("resources/bug.png");

    player.alive = 1;
    player.img = LoadTexture("resources/player sprites.png");
    player.NumFrames = 3;
    player.frame = 0;
    player.framepos = 1;
    player.frameadd = 1;
    //player.pos = (Vector3){.x=10, .y=1, .z=1};  // Determined in ReadLevel function
    player.vel = (Vector3){.x=0, .y=0, .z=-0.6};
    player.jump = 0;
    player.face = (Vector2){1, 0};


    #ifdef PLATFORM_WEB
        emscripten_set_main_loop(GameLoop, 0, 1);
    #else
        while (!WindowShouldClose()) {
            GameLoop();
        }
    #endif

    free(TileSet);
    if (NumBugs) {
        free(BugSet);
    }
    UnloadTexture(HomeBackground);
    UnloadTexture(start);
    UnloadTexture(player.img);
    UnloadLevers();
    UnloadTexture(TileSheet);
    UnloadTexture(BugSheet);
    UnloadRenderTexture(screen);
    UnloadSound(jump);
    UnloadSound(LeverFlipped);
    UnloadSound(hurt);
    UnloadSound(kill);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}


void Update(void)
{
    BeginTextureMode(screen);
        ClearBackground(DARKGRAY);
        delay += add;
        if (delay >= timer) {
            delay = timer;
            show++;
        }
        if (show == 90) {
            add *= -1;
            show = 91;
        }
        if (delay < 0) {
            delay = 0;
        }
        DrawTiles();
        ShowScore();
        DrawText(msg1, msg1v.x, msg1v.y, 10, (Color){.a=(unsigned char)(delay*255/timer), .r=255, .g=255, .b=255});
        DrawText(msg2, msg2v.x, msg2v.y, 10, (Color){.a=(unsigned char)(delay*255/timer), .r=255, .g=255, .b=255});
    EndTextureMode();

    BeginDrawing();
        DrawTexturePro(screen.texture, (Rectangle){.x=0, .y=0, .width=SCREEN_WIDTH, .height=-SCREEN_HEIGHT}, (Rectangle){.x=0, .y=0, .width=SCREEN_WIDTH*SCALE, .height=SCREEN_HEIGHT*SCALE}, (Vector2){0, 0}, 0, WHITE);
    EndDrawing();
}

void DrawTiles(void)
{    
    int PlayerDrawn = 0, LeversDrawn = 0;
    int Pz = (int)floor(player.pos.z);
    int Px1 = (int)floor(player.pos.x), Py1 = (int)floor(player.pos.y);
    int Px2 = Px1+2, Py2 = Py1+2;

    for (int z = 0; z < Layers; z++) {
        int zindex = z*TileSheetWidth*TileSheetHeight;
        if (Pz <= z && Pz >= z-1) {
            // Draw behind the top of the player
            for (int y = 0; y < Py1; y++) {
                for (int x = 0; x < TileSheetWidth; x++) {
                    if (TileSet[zindex + y*TileSheetWidth + x] != 0 && InRange(x, 0, TileSheetWidth) && InRange(y, 0, TileSheetHeight)) {
                        DrawAnyTile(x, y, z, TileSet[zindex + y*TileSheetWidth + x], &LeversDrawn);
                    }
                }
            }

            // Draw behind the left of the player
            for (int y = Py1; y < Py2; y++) {
                for (int x = 0; x < Px1; x++) {
                    if (TileSet[zindex + y*TileSheetWidth + x] != 0 && InRange(x, 0, TileSheetWidth) && InRange(y, 0, TileSheetHeight)) {
                        DrawAnyTile(x, y, z, TileSet[zindex + y*TileSheetWidth + x], &LeversDrawn);
                    }
                }
            }

            // Draw the player
            if (!PlayerDrawn) {
                if ((int)floor(player.pos.z) == 1) {
                    // Draw Bugs in order
                    DrawBugsOrdered1();
                }
                DrawPlayer();
                PlayerDrawn = 1;
            }
            for (int y = Py1; y < Py2; y++) {
                for (int x = Px1; x < Px2; x++) {
                    if (TileSet[zindex + y*TileSheetWidth + x] != 0 && InRange(x, 0, TileSheetWidth) && InRange(y, 0, TileSheetHeight) && player.pos.x < TileSheetWidth) {
                        DrawAnyTile(x, y, z, TileSet[zindex + y*TileSheetWidth + x], &LeversDrawn);
                    }
                }
            }

            // Draw in front of the right of the player
            for (int y = Py1; y < Py2; y++) {
                for (int x = Px2; x < TileSheetWidth; x++) {
                    if (TileSet[zindex + y*TileSheetWidth + x] != 0 && InRange(x, 0, TileSheetWidth) && InRange(y, 0, TileSheetHeight)) {
                        DrawAnyTile(x, y, z, TileSet[zindex + y*TileSheetWidth + x], &LeversDrawn);
                    }
                }
            }

            // Draw in front of the bottom of the player
            for (int y = Py2; y < TileSheetHeight; y++) {
                for (int x = 0; x < TileSheetWidth; x++) {
                    if (TileSet[zindex + y*TileSheetWidth + x] != 0 && InRange(x, 0, TileSheetWidth) && InRange(y, 0, TileSheetHeight)) {
                        DrawAnyTile(x, y, z, TileSet[zindex + y*TileSheetWidth + x], &LeversDrawn);
                    }
                }
            }

            if ((int)floor(player.pos.z) == 1) {
                // Finish drawing all the bugs
                DrawBugsOrdered2();
            }
        } else {
            for (int y = 0; y < TileSheetHeight; y++) {
                for (int x = 0; x < TileSheetWidth; x++) {
                    if (TileSet[zindex + y*TileSheetWidth + x] != 0) {
                        DrawAnyTile(x, y, z, TileSet[zindex + y*TileSheetWidth + x], &LeversDrawn);
                    }
                }
            }
            if (z == 1) {
                DrawBugs();
            }
        }
    }

    if (!PlayerDrawn) {
        DrawPlayer();
    }
}

void GameLoop(void)
{
    switch (state) {
        case 0:
            // Show home screen
            HomeScreen();
            break;
        
        case 1:
            // Main Game
            if (!LevelLoaded) {
                switch (LevelNum) {
                    case 1:
                        Level1();
                        break;
                    
                    case 3:
                        Level2();
                        break;

                    case 5:
                        Level3();
                        break;

                    case 7:
                        Level4();
                        break;

                    default:
                        timer = 120;
                        delay++;
                        break;

                }
                LevelLoaded = 1;
            }

            if (LevelNum%2) {
                MoveBugs();
                MovePlayer();
                ControlLever();

                if (LeversFlipped == NumLevers && BugsSquashed == NumBugs) {
                    LevelNum++;
                    LevelLoaded = 0;
                    delay = 0;
                }

                Update();
            } else {
                DrawDelay();
                if (delay == timer) {
                    delay = 0;
                    LevelNum++;
                }
                if (LevelNum == 9) {
                    state = 2;
                }
                LevelLoaded = 0;
            }

            break;

        case 2:
            // End screen
            EndScreen();
            break;

    }
}