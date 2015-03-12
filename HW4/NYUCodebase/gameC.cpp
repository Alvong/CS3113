//
//  gameC.cpp
//  NYUCodebase
//
//  Created by Alvong on 3/2/15.
//  Copyright (c) 2015 Ivan Safrin. All rights reserved.
//
#include "Entity.h"
#include "gameC.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
#define FRAME_PER_SEC 10.0f
using namespace std;
float lerp(float v0, float v1, float t) {
    return (1.0-t)*v0 + t*v1;
}
void DrawText(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a,float x,float y) {
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    float texture_size = 1.0/16.0f;
    vector<float> vertexData;
    vector<float> texCoordData;
    vector<float> colorData;
    for(int i=0; i < text.size(); i++) {
        float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
        float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
        colorData.insert(colorData.end(), {r,g,b,a, r,g,b,a, r,g,b,a, r,g,b,a});
        vertexData.insert(vertexData.end(), {((size+spacing) * i) + (-0.5f * size), 0.5f * size, ((size+spacing) * i) +
            (-0.5f * size), -0.5f * size, ((size+spacing) * i) + (0.5f * size), -0.5f * size, ((size+spacing) * i) + (0.5f * size), 0.5f
            * size});
        texCoordData.insert(texCoordData.end(), {texture_x, texture_y, texture_x, texture_y + texture_size, texture_x +
            texture_size, texture_y + texture_size, texture_x + texture_size, texture_y});
    }
    glLoadIdentity();
    glTranslatef(x, y, 0.0);
    glColorPointer(4, GL_FLOAT, 0, colorData.data());
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
    glEnableClientState(GL_VERTEX_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDrawArrays(GL_QUADS, 0, text.size() * 4.0);
    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
    
}

GLuint LoadTexture(const char *image_path)
{
    SDL_Surface *surface= IMG_Load(image_path);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexImage2D(GL_TEXTURE_2D, 0.0, GL_RGBA, surface->w, surface->h, 0.0, GL_BGRA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    SDL_FreeSurface(surface);
    
    return textureID;
}


SDL_Window* displayWindow;

gameC::gameC(): done(false),lastFrameTicks(0.0),timeLeftOver(0.0),playerAction(0),animationElapsed(0.0),state(STATE_GAME){
    Init();
    done = false;
    lastFrameTicks = 0.0f;
}
void gameC::Init() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
    /* initialize terrain and player set attributes*/
    //player texture
    GLuint playertexture=LoadTexture("alienYellow.png");
    GLuint terrainEnemy=LoadTexture("enemies.png");
    GLuint back=LoadTexture("xnt9w1.png");
    //making terrains
    makeTerrain(terrainEnemy);
    makeEnemies(terrainEnemy);



    //player actions index: walking , jump
    float arrayP[3][4]={
        {0.0f/256.0f, 339.0f/512.0f,68.0f/256.0f, 83.0f/512.0f},
        {0.0f/256.0f, 0.0f/512.0f,70.0f/256.0f, 86.0f/512.0f},
        {69.0f/256.0f, 173.0f/512.0f,66.0f/256.0f, 82.0f/512.0f}
    };
    float arrayE[2][4]={
        {71.0f/1024.0f,372.0f/512.0f,64.0f/1024.0f,24.0f/512.0f},
        {71.0f/1024.0f,395.0f/512.0f,65.0f/1024.0f,23.0f/512.0f}
    };
    for (int i=0; i<3; i++) {
        for (int j=0;j<4; j++) {
            playerAnimation[i][j]=arrayP[i][j];
        }
    }
    for (int i=0; i<2; i++) {
        for (int j=0;j<4; j++) {
            enemyAnimation[i][j]=arrayE[i][j];
        }
    }
//index initial is 0
    playerAction=0;
    //setting up player
    Entity *first=new Entity(playertexture, playerAnimation[0][0], playerAnimation[0][1], playerAnimation[0][2], playerAnimation[0][3],0.0,-0.9);
    player=first;
    backGround=back;//set background
    
}
gameC::~gameC() {
    SDL_Quit();
}
//making 30 enemies
void gameC::makeEnemies(unsigned int terrainEnemy)
{
    Entity *temp=new Entity(terrainEnemy,71.0f/1024.0f,395.0f/512.0f,65.0f/1024.0f,23.0f/512.0f,0.8,1.0);
    Entity *temp2=new Entity(terrainEnemy,71.0f/1024.0f,395.0f/512.0f,65.0f/1024.0f,23.0f/512.0f,-0.3,0.4);
    Entity *temp3=new Entity(terrainEnemy,71.0f/1024.0f,395.0f/512.0f,65.0f/1024.0f,23.0f/512.0f,-0.8,0.2);
    enemies.push_back(temp);
    enemies.push_back(temp2);
    enemies.push_back(temp3);
        
    
}
void gameC::makeTerrain(unsigned int terrainEnemy)
{
    //push terrain onto vecoter
    for (int i=0; i<26; ++i) {
        
        Entity *temp=new Entity(terrainEnemy,0.0f/1024.0f,141.0f/512.0f,70.0f/1024.0f,69.5f/512.0f,(-0.98+0.08*i),-1.0);
        terrain.push_back(temp);
        
    }
    for (int i=0; i<10; ++i) {
        
        Entity *temp=new Entity(terrainEnemy,0.0f/1024.0f,141.0f/512.0f,70.0f/1024.0f,69.5f/512.0f,(-0.98+0.08*i),-0.4);
        terrain.push_back(temp);
    }
    for (int i=0; i<10; ++i) {
        
        Entity *temp=new Entity(terrainEnemy,0.0f/1024.0f,141.0f/512.0f,70.0f/1024.0f,69.5f/512.0f,(0.98-0.08*i),-0.4);
        terrain.push_back(temp);
    }
    for (int i=0; i<10; ++i) {
        
        Entity *temp=new Entity(terrainEnemy,0.0f/1024.0f,141.0f/512.0f,70.0f/1024.0f,69.5f/512.0f,(0.35-0.08*i),0.2);
        terrain.push_back(temp);
    }
    for (int i=0; i<10; ++i) {
        
        Entity *temp=new Entity(terrainEnemy,0.0f/1024.0f,141.0f/512.0f,70.0f/1024.0f,69.5f/512.0f,(-0.98+0.08*i),0.8);
        terrain.push_back(temp);
    }
    for (int i=0; i<10; ++i) {
        
        Entity *temp=new Entity(terrainEnemy,0.0f/1024.0f,141.0f/512.0f,70.0f/1024.0f,69.5f/512.0f,(0.98-0.08*i),0.8);
        terrain.push_back(temp);
    }

}
void gameC::Render() {
        /* draw terrain*/
    glClear(GL_COLOR_BUFFER_BIT);
    if (state==STATE_GG) {
        glClear(GL_COLOR_BUFFER_BIT);
         DrawText(LoadTexture("font2.png"), "THE END", .1, .0005, 1.0, 1.0, 1.0, 1.0,-1.0,-1.0);
    }
    if (state==STATE_GAME) {
        
    drawBack();
    for (Entity* one: terrain) {
        one->draw(0.6);
    }
    for (Entity* one: enemies) {
        one->draw(0.8);
    }

    
    player->draw();
    }
       SDL_GL_SwapWindow(displayWindow);
}
void gameC::Update(float elapsed) {
    
    SDL_Event event;
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true; }
    }
    
    /* update position:
     checking collision
     gravvity
     accelaration
     fix updata
     *///use player action for enemy movement
    animationElapsed+=FIXED_TIMESTEP;
    if (animationElapsed>1.0/FRAME_PER_SEC) {
        playerAction++;
        animationElapsed=0.0;
    }
    if (playerAction>1) {
        playerAction=0;
    }
    
  //if player moves
    
    if(keys[SDL_SCANCODE_LEFT]){
        player->isStatic=false;
        player->setTexture(
        playerAnimation[playerAction][0],
        playerAnimation[playerAction][1],
        playerAnimation[playerAction][2],
        playerAnimation[playerAction][3]
                           );
        player->velocity_x=-0.5;
            }
    if(keys[SDL_SCANCODE_RIGHT]){
        player->isStatic=false;
        player->setTexture(
       playerAnimation[playerAction][0],
       playerAnimation[playerAction][1],
       playerAnimation[playerAction][2],
       playerAnimation[playerAction][3]
                           );
        player->velocity_x=.5;
    }

    
    if(keys[SDL_SCANCODE_UP]&&player->isStatic&&player->velocity_y==0.0){
        player->isStatic=false;
        playerAction=2;
        player->setTexture(
       playerAnimation[playerAction][0],
       playerAnimation[playerAction][1],
       playerAnimation[playerAction][2],
       playerAnimation[playerAction][3]
                           );
        
        player->velocity_y=1.7;
    }
    player->moving();
    for (Entity *one: terrain) {
        player->collidesWith(one);
        //player->collided();
    }
    
    for (int i=0; i<enemies.size(); i++)
    {
        if(player->collided(enemies[i]))
        {
           
            enemies[i]=nullptr;
            enemies.erase(enemies.begin()+i);
        }
        
    }
    if (enemies.size()==0) {
        state=STATE_GG;
    }

    

    
    
}
bool gameC::UpdateAndRender() {
    
    float ticks = (float)SDL_GetTicks()/1000.0f;
    float elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    float fixedElapsed = elapsed + timeLeftOver;
    
    if(fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
        fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
    }
    while (fixedElapsed >= FIXED_TIMESTEP )
    {
        fixedElapsed -= FIXED_TIMESTEP;
    }
    timeLeftOver = fixedElapsed;
    Update(elapsed);
    Render();
    return done;
}








