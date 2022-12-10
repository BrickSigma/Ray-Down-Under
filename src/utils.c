#include <utilities.h>
#include <globals.h>
#include <levels.h>

#include <stdio.h>
#include <stdlib.h>

static int TilesLoaded = 0;

void ReadLevel(const char *file)
{
    if (TilesLoaded) {
        free(TileSet);
        UnloadLevers();
    } else {
        TilesLoaded = 1;
    }

    FILE *ptr;
    ptr = fopen(file, "rb");
    if (ptr == NULL) {
        printf("UNABLE TO OPEN FILE!\n");
    }

    fscanf(ptr, "%d %d %d", &TileSheetWidth, &TileSheetHeight, &Layers);
    TileSet = calloc(TileSheetWidth*TileSheetHeight*Layers, sizeof(int));

    int x = 0, y = 0;
    char *data = (char *)calloc(TileSheetWidth+2, sizeof(char));

    NumLevers = 0;
    LeverSet = (Lever *)calloc(1, sizeof(Lever));
    while (fscanf(ptr, "%s", data) != EOF) {
        for (x = 0; x < TileSheetWidth; x++) {
            sscanf(data+x, "%c", (char *)&TileSet[y*TileSheetWidth + x]);
            if (TileSet[y*TileSheetWidth + x] == 2) {
                NumLevers++;
                LeverSet = (Lever *)realloc(LeverSet, NumLevers*sizeof(Lever));
                LeverSet[NumLevers-1].pos = (Vector3){.x=x, .y=y%TileSheetHeight, .z=y/TileSheetHeight};
            } else if (TileSet[y*TileSheetWidth + x] == 3) {
                TileSet[y*TileSheetWidth + x] = 0;
                player.pos = (Vector3){.x=x, .y=y%TileSheetHeight, .z=y/TileSheetHeight};
                player.respawn = (Vector3){.x=x, .y=y%TileSheetHeight, .z=y/TileSheetHeight};
            }
        }
        y++;
    }

    //printf("Number of levers -> %d\n", NumLevers);
    InitLevers();

    free(data);
    fclose(ptr);
    for (int z = 0; z < Layers; z++) {
        for (int i = 0; i < TileSheetHeight; i++) {
            for (int j = 0; j < TileSheetWidth; j++) {
                //printf("%d ", TileSet[z*TileSheetWidth*TileSheetHeight + i*TileSheetWidth + j]);
            }
            //printf("\n");
        }
        //printf("\n");
    }
}

Vector2 Coord2Screen(float x, float y, float z)
{
    float xpos = (x*(TILESIZE/2)) - (y*TILESIZE/2);
    float ypos = (x*(TILESIZE/4)) + (y*TILESIZE/4) - z*16;
    return (Vector2){.x=xpos, .y=ypos};
}

int InRange(int val, int start, int end)
{
    return (val >= start && val <= end);
}