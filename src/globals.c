#include <globals.h>

int state = 0;
int LevelLoaded = 0;
int LevelNum = 0;

char msg1[255];
char msg2[255];
Vector2 msg1v, msg2v;
int show = 0, add = 1;

int delay = 0;
int timer = 0;

Vector2 offset = {.x=0, .y=0};
Vector2 SpawnOffset = {.x=0, .y=0};
float spacing = 2.5;

int TileSheetWidth, TileSheetHeight, Layers;
int *TileSet;

int NumLevers;
Lever *LeverSet;

float BugVel = 0.05;
int NumBugs;
Bug *BugSet;
int BugsInit = 0;

Sound jump, LeverFlipped, hurt, kill;

Texture2D HomeBackground;
Texture2D EndBackground;
Texture2D start;
RenderTexture2D screen;
Texture2D TileSheet;
Texture2D BugSheet;

Player player;