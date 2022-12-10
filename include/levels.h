#ifndef LEVELS_H
#define LEVELS_H

extern int LeversFlipped;
extern int BugsSquashed;

void HomeScreen(void);

void EndScreen(void);

void DrawDelay(void);

void InitLevers(void);

void UnloadLevers(void);

void ControlLever(void);

void ShowScore(void);

void Level1(void);
void Level2(void);
void Level3(void);
void Level4(void);

#endif // LEVELS_H