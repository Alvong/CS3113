//
//  gameC.h
//  NYUCodebase
//
//  Created by Alvong on 3/2/15.
//  Copyright (c) 2015 Ivan Safrin. All rights reserved.
//
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include "Entity.h"
#ifndef gameC_h
#define gameC_h
enum GameState{STATE_GAME,STATE_GG};
class gameC {
private:
    bool done;
    float lastFrameTicks;
    SDL_Window* displayWindow;
    Entity* player;
    std::vector<Entity*> terrain;
    float timeLeftOver;
    float playerAnimation[3][4];
    int playerAction;
    float enemyAnimation[2][4];
    unsigned int backGround;
    float animationElapsed;
    std::vector<Entity*> enemies;
    GameState state;
public:
    gameC();
    ~gameC();
    void Init();
    bool UpdateAndRender();
    void Render();
    void Update(float elapsed);
    void fixUpdate();
    void makeTerrain(unsigned int terrainEnemy);
     void makeEnemies(unsigned int terrainEnemy);
    void drawBack()
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, backGround);
        glViewport(0, 0, 800, 600);
        glOrtho(-1, 1, -1.0, 1.0,
                -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0, 0.0, 0.0);
        //glRotatef(rotation, 0.0, 0.0, 1.0);
        GLfloat quad[] = {-1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f};
        glVertexPointer(2, GL_FLOAT, 0, quad);
        glEnableClientState(GL_VERTEX_ARRAY);
        GLfloat quadUVs[] = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0};
        glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawArrays(GL_QUADS, 0, 4);
        glMatrixMode(GL_MODELVIEW);
        glDisable(GL_TEXTURE_2D);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    };
};

#endif
