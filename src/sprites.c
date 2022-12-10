#include <sprites.h>
#include <globals.h>
#include <utilities.h>
#include <levels.h>

#include <math.h>
#include <stdlib.h>

static const float JUMP_VEL = 0.1;
static const float MAX_VEL = 0.5;

int FrameCount = 0;
int FrameTime = 20;

void MovePlayer(void)
{
    float oldX = player.pos.x, oldY = player.pos.y, oldZ = player.pos.z;
    // Movement
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        player.vel.x = -0.1*DELTA_TIME;
        player.face = (Vector2){-1, 0};
        player.framepos = 3;
    } else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        player.vel.x = 0.1*DELTA_TIME;
        player.face = (Vector2){1, 0};
        player.framepos = 1;
    } else if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
        player.vel.y = -0.1*DELTA_TIME;
        player.face = (Vector2){0, -1};
        player.framepos = 2;
    } else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
        player.vel.y = 0.1*DELTA_TIME;
        player.face = (Vector2){0, 1};
        player.framepos = 0;
    }

    if (IsKeyPressed(KEY_SPACE) && player.jump == 0) {
        player.vel.z = 0.85;
        player.jump = 1;
        if (LevelNum > 3) {
            PlaySound(jump);
        }
    } else {
        player.vel.z -= JUMP_VEL;
        if (player.vel.z < -MAX_VEL-(JUMP_VEL/2)) {
            player.vel.z = -MAX_VEL;
        }
    }

    // Tile Collisions
    // X-axis collision
    if (player.alive) {
        player.pos.x += player.vel.x;
    }
    if (player.pos.x <= -1) {
        player.pos.x = -1;
    } else if (player.pos.x >= TileSheetWidth) {
        player.pos.x = TileSheetWidth;
    }
    for (int z = 0; z < Layers; z++) {
        if ((int)floor(player.pos.z) == z) {
            for (int y = 0; y < TileSheetHeight; y++) {
                for (int x = 0; x < TileSheetWidth; x++) {
                    if (CheckCollisionRecs((Rectangle){.x=player.pos.x, .y=player.pos.y, .width=1, .height=1}, (Rectangle){.x=x, .y=y, .width=1, .height=1}) && TileSet[z*TileSheetWidth*TileSheetHeight + y*TileSheetWidth + x] != 0) {
                        if (player.vel.x < 0) {
                            player.pos.x = x+1;
                        } else if (player.vel.x > 0){
                            player.pos.x = x-1;
                        }
                    }
                }
            }
        }
    }

    // Y-axis collision
    if (player.alive) {
        player.pos.y += player.vel.y;
    }
    if (player.pos.y <= -1) {
        player.pos.y = -1;
    } else if (player.pos.y >= TileSheetHeight) {
        player.pos.y = TileSheetHeight;
    }
    for (int z = 0; z < Layers; z++) {
        if ((int)floor(player.pos.z) == z) {
            for (int y = 0; y < TileSheetHeight; y++) {
                for (int x = 0; x < TileSheetWidth; x++) {
                    if (CheckCollisionRecs((Rectangle){.x=player.pos.x, .y=player.pos.y, .width=1, .height=1}, (Rectangle){.x=x, .y=y, .width=1, .height=1}) && TileSet[z*TileSheetWidth*TileSheetHeight + y*TileSheetWidth + x] != 0) {
                        if (player.vel.y < 0) {
                            player.pos.y = y+1;
                        } else if (player.vel.y > 0) {
                            player.pos.y = y-1;
                        }
                    }
                }
            }
        }
    }

    // Z-axis collision
    if (player.alive) {
        player.pos.z += player.vel.z;
    }
    for (int z = 0; z < Layers; z++) {
        if ((player.pos.x > -1 && player.pos.x < TileSheetWidth) && (player.pos.y > -1 && player.pos.y < TileSheetHeight) && player.pos.z <= z+1 && player.pos.z > z) {
            for (int y = 0; y < 2; y++) {
                for (int x = 0; x < 2; x++) {
                    if (CheckCollisionRecs((Rectangle){.x=player.pos.x, .y=player.pos.y, .width=1, .height=1}, (Rectangle){.x=floor(player.pos.x)+x+0.1, .y=floor(player.pos.y)+y+0.1, .width=0.8, .height=0.8}) && TileSet[z*TileSheetWidth*TileSheetHeight + ((int)floor(player.pos.y)+y)*TileSheetWidth + ((int)floor(player.pos.x)+x)] != 0) {
                        player.jump = 0;
                        player.pos.z = z+1;
                        goto JUMP_LOOP;
                    }
                }
            }
            JUMP_LOOP:
        }
    }

    // Bug collisions
    if ((int)floor(player.pos.z) == 1) {
        for (int i = 0; i < NumBugs; i++) {
            if (CheckCollisionRecs((Rectangle){.x=player.pos.x, .y=player.pos.y, .width=1, .height=1}, (Rectangle){.x=BugSet[i].pos.x, .y=BugSet[i].pos.y, .width=1, .height=1}) && BugSet[i].alive) {
                if (player.pos.z < oldZ) {
                    BugSet[i].alive = 0;
                    BugsSquashed++;
                    PlaySound(kill);
                } else {
                    player.alive = 0;
                    PlaySound(hurt);
                }
            }
        }
    }

    // Make sure the player can't jump when falling
    if (player.pos.z != oldZ) {
        player.jump = 1;
    }

    // Camera offset controls
    if ((player.pos.x < 9-offset.x+(spacing)) && player.pos.x != oldX) {
        offset.x += fabs(player.vel.x);
    } else if ((player.pos.x + 1 > 17-offset.x-(spacing)) && player.pos.x != oldX) {
        offset.x -= fabs(player.vel.x);
    }
    if ((player.pos.y < -offset.y+(spacing)) && player.pos.y != oldY) {
        offset.y += fabs(player.vel.y);
    } else if ((player.pos.y + 1 > 12-offset.y-(spacing)) && player.pos.y != oldY) {
        offset.y -= fabs(player.vel.y);
    }

    if (player.pos.z < -10 || !player.alive) {
        // Reset the level
        player.alive = 1;
        offset = SpawnOffset;
        player.pos = player.respawn;
        BugsSquashed = 0;
        for (int i = 0; i < NumBugs; i++) {
            BugSet[i].alive = 1;
        }
        InitLevers();
    }

    player.vel = (Vector3){.x=0, .y=0, .z=player.vel.z};
    
}

void DrawPlayer(void)
{
    FrameCount++;
    if (FrameCount == FrameTime) {
        FrameCount = 0;
        player.frame += player.frameadd;
        if (player.frame == player.NumFrames-1 || player.frame == 0) {
            player.frameadd *= -1;
        }
    }
    Vector2 pos = Coord2Screen(player.pos.x+offset.x, player.pos.y+offset.y, player.pos.z);
    float xpos = pos.x, ypos = pos.y - 8;
    if (LevelNum == 1) {
        DrawTexturePro(player.img, (Rectangle){.x=32*12, .y=0, .width=32, .height=40}, (Rectangle){.x=xpos, .y=ypos, .width=32, .height=40}, (Vector2){0, 0}, 0, WHITE);
    } else {
        DrawTexturePro(player.img, (Rectangle){.x=(96*player.framepos)+(player.frame*32), .y=0, .width=32, .height=40}, (Rectangle){.x=xpos, .y=ypos, .width=32, .height=40}, (Vector2){0, 0}, 0, WHITE);
    }
}


void DrawBaseTile(int x, int y, int z)
{
    Vector2 pos = Coord2Screen((float)x+offset.x, (float)y+offset.y, (float)z);
    float xpos = pos.x, ypos = pos.y;
    DrawTexturePro(TileSheet, (Rectangle){.x=0, .y=0, 32, 32}, (Rectangle){.x=xpos, .y=ypos, TILESIZE, TILESIZE}, (Vector2){0, 0}, 0, WHITE);
}

void DrawAnyTile(int x, int y, int z, int n, int *i)
{
    if (n == 1) {
        DrawBaseTile(x, y, z);
    } else if (n == 2) {
        DrawLever(x, y , z, i);
    }
}


void DrawLever(int x, int y, int z, int *i)
{
    Vector2 pos = Coord2Screen((float)x+offset.x, (float)y+offset.y, (float)z);
    float xpos = pos.x, ypos = pos.y;
    DrawTexturePro(TileSheet, (Rectangle){.x=(LeverSet[*i].frame * TILESIZE) + (5*TILESIZE), .y=0, TILESIZE, TILESIZE}, (Rectangle){.x=xpos, .y=ypos, TILESIZE, TILESIZE}, (Vector2){0, 0}, 0, WHITE);
    *i += 1;
}


void MoveBugs(void)
{
    for (int i = 0; i < NumBugs; i++) {
        if (BugSet[i].alive) {
            BugSet[i].pos.x += BugSet[i].vel.x*DELTA_TIME;
            BugSet[i].pos.y += BugSet[i].vel.y*DELTA_TIME;
            if (BugSet[i].spawn.x < BugSet[i].EndPos.x) {
                if (BugSet[i].pos.x >= BugSet[i].EndPos.x) {
                    BugSet[i].pos.x = BugSet[i].EndPos.x;
                    BugSet[i].vel.x *= -1;
                } else if (BugSet[i].pos.x <= BugSet[i].spawn.x) {
                    BugSet[i].pos.x = BugSet[i].spawn.x;
                    BugSet[i].vel.x *= -1;
                }
            } else if (BugSet[i].spawn.x > BugSet[i].EndPos.x) {
                if (BugSet[i].pos.x <= BugSet[i].EndPos.x) {
                    BugSet[i].pos.x = BugSet[i].EndPos.x;
                    BugSet[i].vel.x *= -1;
                } else if (BugSet[i].pos.x >= BugSet[i].spawn.x) {
                    BugSet[i].pos.x = BugSet[i].spawn.x;
                    BugSet[i].vel.x *= -1;
                }
            }
            
            if (BugSet[i].spawn.y < BugSet[i].EndPos.y) {
                if (BugSet[i].pos.y >= BugSet[i].EndPos.y) {
                    BugSet[i].pos.y = BugSet[i].EndPos.y;
                    BugSet[i].vel.y *= -1;
                } else if (BugSet[i].pos.y <= BugSet[i].spawn.y) {
                    BugSet[i].pos.y = BugSet[i].spawn.y;
                    BugSet[i].vel.y *= -1;
                }
            } else if (BugSet[i].spawn.y > BugSet[i].EndPos.y) {
                if (BugSet[i].pos.y <= BugSet[i].EndPos.y) {
                    BugSet[i].pos.y = BugSet[i].EndPos.y;
                    BugSet[i].vel.y *= -1;
                } else if (BugSet[i].pos.y >= BugSet[i].spawn.y) {
                    BugSet[i].pos.y = BugSet[i].spawn.y;
                    BugSet[i].vel.y *= -1;
                }
            }
        }
    }
}

void DrawBugs(void)
{
    for (int i = 0; i < NumBugs; i++) {
        if (BugSet[i].alive) {
            Vector2 pos = Coord2Screen(BugSet[i].pos.x+offset.x, BugSet[i].pos.y+offset.y, 1);
            float xpos = pos.x, ypos = pos.y;
            //DrawTexture(BugSheet, xpos, ypos, WHITE);
            DrawTexturePro(TileSheet, (Rectangle){.x=TILESIZE, .y=TILESIZE, TILESIZE, TILESIZE}, (Rectangle){.x=xpos, .y=ypos, TILESIZE, TILESIZE}, (Vector2){0, 0}, 0, WHITE);
        }
    }
}

void DrawSingleBug(Bug bug)
{
    if (bug.alive) {
        Vector2 pos = Coord2Screen(bug.pos.x+offset.x, bug.pos.y+offset.y, 1);
        float xpos = pos.x, ypos = pos.y;
        //DrawTexture(BugSheet, xpos, ypos, WHITE);
        DrawTexturePro(TileSheet, (Rectangle){.x=TILESIZE, .y=TILESIZE, TILESIZE, TILESIZE}, (Rectangle){.x=xpos, .y=ypos, TILESIZE, TILESIZE}, (Vector2){0, 0}, 0, WHITE);
    }
}

void DrawBugsOrdered1(void)
{
    // Be warned it's a mess down here...
    // I'm too lazy to risk making this efficient, so enjoy this little solution to rendering bugs **with probably buggy code**. ;)

    Bug *set1, *set2;
    int set1n = 0, set2n = 0;

    for (int i = 0; i < NumBugs; i++) {
        if (BugSet[i].pos.x < player.pos.x && BugSet[i].pos.y < player.pos.y) {
            set1n++;
        } else if (BugSet[i].pos.x >= player.pos.x && BugSet[i].pos.y < player.pos.y) {
            set2n++;
        }
    }

    if (set1n) {
        set1 = (Bug *)calloc(set1n, sizeof(Bug));
    }
    if (set2n) {
        set2 = (Bug *)calloc(set2n, sizeof(Bug));
    }

    int c1 = 0, c2 = 0;
    for (int i = 0; i < NumBugs; i++) {
        if (BugSet[i].pos.x < player.pos.x && BugSet[i].pos.y < player.pos.y) {
            set1[c1] = BugSet[i];
            c1++;
        } else if (BugSet[i].pos.x >= player.pos.x && BugSet[i].pos.y < player.pos.y) {
            set2[c2] = BugSet[i];
            c2++;
        }
    }

    for (int i = 0; i < set1n; i++) {
        DrawSingleBug(set1[i]);
    }
    for (int i = 0; i < set2n; i++) {
        DrawSingleBug(set2[i]);
    }

    if (set1n) {
        free(set1);
    }
    if (set2n) {
        free(set2);
    }
}

void DrawBugsOrdered2(void)
{
    // Be warned it's a mess down here...
    // I'm too lazy to risk making this efficient, so enjoy this little solution to rendering bugs **with probably buggy code**. ;)

    Bug *set3, *set4;
    int set3n = 0, set4n = 0;

    for (int i = 0; i < NumBugs; i++) {
        if(BugSet[i].pos.x < player.pos.x && BugSet[i].pos.y >= player.pos.y) {
            set3n++;
        } else if (BugSet[i].pos.x >= player.pos.x && BugSet[i].pos.y >= player.pos.y) {
            set4n++;
        }
    }

    if (set3n) {
        set3 = (Bug *)calloc(set3n, sizeof(Bug));
    }
    if (set4n) {
        set4 = (Bug *)calloc(set4n, sizeof(Bug));
    }

    int c3 = 0, c4 = 0;
    for (int i = 0; i < NumBugs; i++) {
        if(BugSet[i].pos.x < player.pos.x && BugSet[i].pos.y >= player.pos.y) {
            set3[c3] = BugSet[i];
            c3++;
        } else if (BugSet[i].pos.x >= player.pos.x && BugSet[i].pos.y >= player.pos.y) {
            set4[c4] = BugSet[i];
            c4++;
        }
    }

    for (int i = 0; i < set3n; i++) {
        DrawSingleBug(set3[i]);
    }
    for (int i = 0; i < set4n; i++) {
        DrawSingleBug(set4[i]);
    }

    if (set3n) {
        free(set3);
    }
    if (set4n) {
        free(set4);
    }
}