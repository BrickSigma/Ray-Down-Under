#ifndef GLOBALS_H
#define GLOBALS_H

#include <raylib.h>
#include <sprites.h>

#define FPS (60)
#define TILESIZE (32)
#define SCALE (3)

#define SCREEN_WIDTH (256)
#define SCREEN_HEIGHT (256)


extern int state;
extern int LevelLoaded;
extern int LevelNum;

extern char msg1[255];
extern char msg2[255];
extern Vector2 msg1v, msg2v;
extern int show, add;

extern int delay;
extern int timer;

extern Vector2 offset, SpawnOffset;  // Camera offset
extern float spacing;

extern int TileSheetWidth, TileSheetHeight, Layers;
extern int *TileSet;

extern int NumLevers;
extern Lever *LeverSet;

extern float BugVel;
extern int NumBugs;
extern Bug *BugSet;
extern int BugsInit;

extern Sound jump, LeverFlipped, hurt, kill;

extern RenderTexture2D screen;
extern Texture2D HomeBackground;
extern Texture2D EndBackground;
extern Texture2D start;
extern Texture2D TileSheet;
extern Texture2D BugSheet;

extern Player player;

#endif // GLOBALS_H