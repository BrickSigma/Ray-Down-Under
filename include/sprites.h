#ifndef SPRITES_H
#define SPRITES_H

#include <raylib.h>

// An entity struct that controls all
typedef struct _Entity {
    int alive; // Check if the entity is alive
    Vector3 pos;  // Position of the entity in 3D space
    Vector3 respawn;  // Position to respawn if dead/restarted
    Vector2 face; // Direction entity is facing
    Vector3 vel;  // Current 3D speed of the entity
    int jump;  // Is the entity jumping?
    Texture2D img;  // Store all animation frames for the entity here
    int NumFrames;  // Number of animation frames
    int framepos;
    int frameadd;
    int frame;  // Store the current entity frame to be drawn
} Entity, Player;

typedef struct _Lever {
    Vector3 pos;  // Position of the entity in 3D space
    int flipped;  // Flipped state
    int frame;  // Store the current entity frame to be drawn
} Lever;

typedef struct _Bug {
    int alive;  // Check if the bug is alive
    Vector3 pos;  // Position of the entity in 3D space
    Vector3 spawn;  // Position of the entity in 3D space
    Vector3 EndPos;  // Position to respawn if dead/restarted
    Vector2 vel;  // Current 2D speed of the entity
    int NumFrames;  // Number of animation frames
    int frame;  // Store the current entity frame to be drawn
    Vector3 particles[4];
    float radii[4];
} Bug;

void MovePlayer(void);
void DrawPlayer(void);

void DrawBaseTile(int x, int y, int z);
void DrawAnyTile(int x, int y, int z, int n, int *i);

void DrawLever(int x, int y, int z, int *i);

void MoveBugs(void);
void DrawBugs(void);
void DrawSingleBug(Bug bug);
void DrawBugsOrdered1(void);
void DrawBugsOrdered2(void);

#endif // SPRITES_H