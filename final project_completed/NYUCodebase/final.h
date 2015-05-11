
#ifndef final_h
#define final_h
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include "Entity.h"
#include "AI_bfs.h"
#include <stdio.h>
#include <iostream>
#include <SDL_mixer.h>
//#include <SDL_mixer.h>
enum GameState{STATE_MENU,STATE_GAME_ONE,STATE_GG};
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
    void collision(Entity* one);
    void checkX(const float worldX, const float worldY,Entity* one);
    void checkY(const float worldX, const float worldY,Entity* one);
    void readleveldata(std::ifstream& level,std::vector<std::vector<int>> *leveldata);
    void reset();
    void drawObjects();
    void collideObjects();
    void drawBack(unsigned int back);
    bool climbing();
    void enemyNavigation();
    void enemymoving(Entity* other,Entity* one);
    void makeGraph(std::vector<std::vector<int>> leveldata);
    int findOnG(Entity* one);
    int FindNode(int x,int y);
    Node* BFS(Node* first,Node* second);
    
   // void makingAmmo();
    //void throwAmmo();
    
private:
    int playerDirection;

    int level;
    unsigned int playertexture;
bool done;
float lastFrameTicks;
SDL_Window* displayWindow;
Entity* player;
Entity* door;
Mix_Music *background;
Mix_Chunk *collide;
Mix_Chunk *die;
Mix_Chunk *jump;
Mix_Chunk *poopsound;
Mix_Chunk *coin;
Mix_Chunk *found;
std::vector<Node*> graph;
std::vector<Entity*> opBlocks;
std::vector<Entity*> food;
std::vector<Entity*> poops;
std::vector<Entity*> poopOnG;
std::vector<Entity*> ladders;
std::vector<Entity*> enemies;
//std::vector<std::vector<int>> leveldata3;
//std::vector<std::vector<int>> leveldata2;
std::vector<std::vector<int>> leveldata;
std::vector<std::vector<float>> playerdata;
//fstream levelone;



float timeLeftOver;
int EnemyAnimation[3];
int enemyAction;
int playerAnimation[4][2];
int playerAction;
int blockAnimation[2];
unsigned int font;
unsigned int poop;
unsigned int back;
unsigned int menuback;
unsigned int tiles;
unsigned int ghost;
int ammo;
float animationElapsed;
int ammotime;
float ammotimeO;
bool loading;
float titleaAnimation;
GameState save;
GameState state;
   // int level;

};
#endif /* defined(__NYUCodebase__final__) */
