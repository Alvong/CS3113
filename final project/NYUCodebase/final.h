//
//  final.h
//  NYUCodebase
//
//  Created by Alvong on 4/30/15.
//  Copyright (c) 2015 Ivan Safrin. All rights reserved.
//

#ifndef final_h
#define final_h
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include "Entity.h"
#include <stdio.h>
//#include <SDL_mixer.h>
enum GameState{STATE_GAME,STATE_GG};
class final{
public:
    final();
    ~final();
    void Init();
    bool UpdateAndRender();
    void Render();
    void Update(float elapsed);
    void fixUpdate();
    void makeTerrain(unsigned int terrainEnemy);
    void worldToTileCoordinates(float worldX, float worldY, int *gridX, int *gridY);
    void tileToWorld(float *worldX, float *worldY, int gridX, int gridY);
    void collision();
    void checkX(const float worldX, const float worldY);
    void checkY(const float worldX, const float worldY);
    void readleveldata();
    
private:
bool done;
float lastFrameTicks;
SDL_Window* displayWindow;
Entity* player;
//Mix_Music *background;
//Mix_Chunk *collide;
std::vector<std::vector<int>> leveldata;
std::vector<std::vector<float>> playerdata;
std::vector<std::vector<float>> enemydata;
std::vector<std::vector<float>> keydata;
float timeLeftOver;
int playerAnimation[3][2];
int playerAction;
int enemyAnimation[2];
unsigned int back;
unsigned int tiles;
float animationElapsed;
std::vector<Entity*> enemies;
GameState state;
};
#endif /* defined(__NYUCodebase__final__) */
