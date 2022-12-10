#include <levels.h>
#include <globals.h>
#include <utilities.h>
#include <raylib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int LeversFlipped = 0;
int BugsSquashed = 0;

int StartButton = 0;

void HomeScreen(void)
{
    Vector2 mouse = GetMousePosition();
    mouse.x /= SCALE;
    mouse.y /= SCALE;
    if (StartButton != 2) {
        if (CheckCollisionPointRec(mouse, (Rectangle){.x=70, .y=90, .width=128, .height=48})) {
            StartButton = 1;
            if (IsMouseButtonPressed(0)) {
                StartButton = 2;
                state++;
            }
        } else {
            StartButton = 0;
        }
    }

    BeginTextureMode(screen);
        DrawTexture(HomeBackground, 0, 0, WHITE);
        DrawTexturePro(start, (Rectangle){.x=0, .y=48*StartButton, .width=128, .height=48}, (Rectangle){.x=70, .y=90, .width=128, .height=48}, (Vector2){0, 0}, 0, WHITE);
    EndTextureMode();

    BeginDrawing();
        DrawTexturePro(screen.texture, (Rectangle){.x=0, .y=0, .width=SCREEN_WIDTH, .height=-SCREEN_HEIGHT}, (Rectangle){.x=0, .y=0, .width=SCREEN_WIDTH*SCALE, .height=SCREEN_HEIGHT*SCALE}, (Vector2){0, 0}, 0, WHITE);
    EndDrawing();
}

void EndScreen(void)
{
    static int c = 0;
    if (c < 120) {
        c++;
        BeginTextureMode(screen);
            DrawTexture(EndBackground, 0, 0, WHITE);
        EndTextureMode();
    } else {
        BeginTextureMode(screen);
            ClearBackground(BLACK);
            DrawText("segmentation fault!", 30, 110, 20, WHITE);
        EndTextureMode();
    }
    BeginDrawing();
        DrawTexturePro(screen.texture, (Rectangle){.x=0, .y=0, .width=SCREEN_WIDTH, .height=-SCREEN_HEIGHT}, (Rectangle){.x=0, .y=0, .width=SCREEN_WIDTH*SCALE, .height=SCREEN_HEIGHT*SCALE}, (Vector2){0, 0}, 0, WHITE);
    EndDrawing();
}

void DrawDelay(void) {
    BeginDrawing();
        DrawTexturePro(screen.texture, (Rectangle){.x=0, .y=0, .width=SCREEN_WIDTH, .height=-SCREEN_HEIGHT}, (Rectangle){.x=0, .y=0, .width=SCREEN_WIDTH*SCALE, .height=SCREEN_HEIGHT*SCALE}, (Vector2){0, 0}, 0, WHITE);
        DrawRectangle(0, 0, 256*SCALE, 256*SCALE, (Color){.a=(unsigned char)(delay*255/timer), .r=0, .g=0, .b=0});
    EndDrawing();
}

void InitLevers(void)
{
    for (int i = 0; i < NumLevers; i++) {
        LeverSet[i].flipped = 0;
        LeverSet[i].frame = 0;
    }
    LeversFlipped = 0;
}

void UnloadLevers(void)
{
    free(LeverSet);
}

void ControlLever(void)
{
    int switched = IsKeyPressed(KEY_C) || IsKeyPressed(KEY_N);
    int xpoints[4] = {-1, 0, 1, 0};
    int ypoints[4] = {0, -1, 0, 1};
    Vector2 faces[4] = {(Vector2){1, 0}, (Vector2){0, 1}, (Vector2){-1, 0}, (Vector2){0, -1}};
    for (int i = 0; i < NumLevers; i++) {
        if ((int)floor(player.pos.z) == (int)LeverSet[i].pos.z && !LeverSet[i].flipped) {
            for (int j = 0; j < 4; j++) {
                if (CheckCollisionRecs((Rectangle){.x=LeverSet[i].pos.x+xpoints[j], .y=LeverSet[i].pos.y+ypoints[j], .width=1, .height=1}, (Rectangle){.x=player.pos.x, .y=player.pos.y, .width=1, .height=1}) && player.face.x == faces[j].x && player.face.y == faces[j].y) {
                    LeverSet[i].frame = 1;
                    if (switched) {
                        PlaySound(LeverFlipped);
                        LeverSet[i].flipped = 1;
                        LeverSet[i].frame = 2;
                        LeversFlipped++;
                    }
                    break;
                } else {
                    LeverSet[i].frame = 0;
                }
            }
        } else  if (!LeverSet[i].flipped){
            LeverSet[i].frame = 0;
        }
    }
}

void ShowScore(void)
{
    char buff[15];
    snprintf(buff, 15, "%d/%d", LeversFlipped, NumLevers);
    DrawText(buff, 24, 6, 16, WHITE);
    DrawTexturePro(TileSheet, (Rectangle){.x=0, .y=32, 16, 16}, (Rectangle){.x=4, .y=4, 16, 16}, (Vector2){0, 0}, 0, WHITE);

    if (NumBugs) {
        snprintf(buff, 15, "%d/%d", BugsSquashed, NumBugs);
    DrawText(buff, 24, 28, 16, WHITE);
    DrawTexturePro(TileSheet, (Rectangle){.x=16, .y=32, 16, 16}, (Rectangle){.x=4, .y=26, 16, 16}, (Vector2){0, 0}, 0, WHITE);
    }

}


void Level1(void)
{
    ReadLevel("resources/level 1.txt");
    strncpy(msg1, "It seems the graphics are broken", 255);
    strncpy(msg2, "Flip the lever to fix it", 255);
    msg1v = (Vector2){.x=52, .y=42};
    msg2v = (Vector2){.x=28, .y=180};
    delay = 0;
    timer = 120;
    show = 0;
    add = 1;
    offset = (Vector2){5.5, 0};
    SpawnOffset = offset;
    NumBugs = 0;
    BugsSquashed = 0;
}

void Level2(void)
{
    ReadLevel("resources/level 2.txt");
    strncpy(msg1, "That's better. Now switch on the", 255);
    strncpy(msg2, "debugger to bring in the audio", 255);
    msg1v = (Vector2){.x=52, .y=40};
    msg2v = (Vector2){.x=52, .y=50};
    delay = 0;
    timer = 120;
    show = 0;
    add = 1;
    offset = (Vector2){7, 0};
    SpawnOffset = offset;
    NumBugs = 0;
    BugsSquashed = 0;
}

void Level3(void)
{
    ReadLevel("resources/level 3.txt");
    strncpy(msg1, "Now we can see these bugs", 255);
    strncpy(msg2, "Jump onto them to destroy these pests!", 255);
    msg1v = (Vector2){.x=48, .y=48};
    msg2v = (Vector2){.x=48, .y=58};
    delay = 0;
    timer = 120;
    show = 0;
    add = 1;
    offset = (Vector2){7, 0};
    SpawnOffset = offset;
    NumBugs = 5;
    BugsSquashed = 0;
    BugSet = (Bug *)calloc(NumBugs, sizeof(Bug));

    // Initialise each bug manually since I'm too lazy to make it quick
    BugSet[0].spawn = (Vector3){.x=5, .y=1, .z=1};
    BugSet[0].EndPos = (Vector3){.x=5, .y=3, .z=1};
    BugSet[0].vel = (Vector2){.x=0, .y=BugVel};

    BugSet[1].spawn = (Vector3){.x=14, .y=6, .z=1};
    BugSet[1].EndPos = (Vector3){.x=14, .y=2, .z=1};
    BugSet[1].vel = (Vector2){.x=0, .y=-BugVel};

    BugSet[2].spawn = (Vector3){.x=7, .y=8, .z=1};
    BugSet[2].EndPos = (Vector3){.x=7, .y=13, .z=1};
    BugSet[2].vel = (Vector2){.x=0, .y=BugVel*2};

    BugSet[3].spawn = (Vector3){.x=14, .y=12, .z=1};
    BugSet[3].EndPos = (Vector3){.x=14, .y=10, .z=1};
    BugSet[3].vel = (Vector2){.x=0, .y=-BugVel};

    BugSet[4].spawn = (Vector3){.x=1, .y=17, .z=1};
    BugSet[4].EndPos = (Vector3){.x=1, .y=9, .z=1};
    BugSet[4].vel = (Vector2){.x=0, .y=-BugVel*2};

    for (int i = 0; i < NumBugs; i++) {
        BugSet[i].pos = BugSet[i].spawn;
        BugSet[i].alive = 1;
    }
}

void Level4(void)
{
    ReadLevel("resources/level 4.txt");
    strncpy(msg1, "Woah, that's a lot of bugs!", 255);
    strncpy(msg2, "", 255);
    msg1v = (Vector2){.x=48, .y=48};
    msg2v = (Vector2){.x=48, .y=58};
    delay = 0;
    timer = 120;
    show = 0;
    add = 1;
    offset = (Vector2){7, 0};
    SpawnOffset = offset;
    if (BugsInit) {
        free(BugSet);
        BugsInit = 0;
    }

    NumBugs = 10;
    BugsSquashed = 0;
    BugSet = (Bug *)calloc(NumBugs, sizeof(Bug));

    BugSet[0].spawn = (Vector3){.x=6, .y=1, .z=1};
    BugSet[0].EndPos = (Vector3){.x=6, .y=3, .z=1};
    BugSet[0].vel = (Vector2){.x=0, .y=BugVel};

    BugSet[1].spawn = (Vector3){.x=9, .y=3, .z=1};
    BugSet[1].EndPos = (Vector3){.x=9, .y=1, .z=1};
    BugSet[1].vel = (Vector2){.x=0, .y=-BugVel};

    BugSet[2].spawn = (Vector3){.x=12, .y=1, .z=1};
    BugSet[2].EndPos = (Vector3){.x=12, .y=3, .z=1};
    BugSet[2].vel = (Vector2){.x=0, .y=BugVel};

    BugSet[3].spawn = (Vector3){.x=15, .y=3, .z=1};
    BugSet[3].EndPos = (Vector3){.x=15, .y=1, .z=1};
    BugSet[3].vel = (Vector2){.x=0, .y=-BugVel};

    BugSet[4].spawn = (Vector3){.x=18, .y=7, .z=1};
    BugSet[4].EndPos = (Vector3){.x=14, .y=7, .z=1};
    BugSet[4].vel = (Vector2){.x=-BugVel, .y=0};

    BugSet[5].spawn = (Vector3){.x=7, .y=9, .z=1};
    BugSet[5].EndPos = (Vector3){.x=12, .y=9, .z=1};
    BugSet[5].vel = (Vector2){.x=BugVel*1.5, .y=0};

    BugSet[6].spawn = (Vector3){.x=3, .y=7, .z=1};
    BugSet[6].EndPos = (Vector3){.x=3, .y=9, .z=1};
    BugSet[6].vel = (Vector2){.x=0, .y=BugVel};

    BugSet[7].spawn = (Vector3){.x=1, .y=12, .z=1};
    BugSet[7].EndPos = (Vector3){.x=3, .y=12, .z=1};
    BugSet[7].vel = (Vector2){.x=BugVel, .y=0};

    BugSet[8].spawn = (Vector3){.x=9, .y=18, .z=1};
    BugSet[8].EndPos = (Vector3){.x=15, .y=18, .z=1};
    BugSet[8].vel = (Vector2){.x=BugVel*2, .y=0};

    BugSet[9].spawn = (Vector3){.x=17, .y=15, .z=1};
    BugSet[9].EndPos = (Vector3){.x=15, .y=15, .z=1};
    BugSet[9].vel = (Vector2){.x=-BugVel, .y=0};

    for (int i = 0; i < NumBugs; i++) {
        BugSet[i].pos = BugSet[i].spawn;
        BugSet[i].alive = 1;
    }
}