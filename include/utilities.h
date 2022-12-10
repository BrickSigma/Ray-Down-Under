#ifndef UTILS_H
#define UTILS_H

#include <globals.h>
#include <raylib.h>

// Delta-time value
#define DELTA_TIME (GetFPS() > 0 ? ((float)FPS/(float)GetFPS()) : 0)  // Make sure there is no division by zero error

// Read a level from file
void ReadLevel(const char *file);

// Convert Cartesian plane to screen
Vector2 Coord2Screen(float x, float y, float z);

int InRange(int val, int start, int end);

#endif // UTILS_H