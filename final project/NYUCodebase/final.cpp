//
//  final.cpp
//  NYUCodebase
//
//  Created by Alvong on 4/30/15.
//  Copyright (c) 2015 Ivan Safrin. All rights reserved.
//

#include "final.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
#define FRAME_PER_SEC 10.0f
#define TILE_SIZE .1f
#define SPRITE_COUNT_X 28
#define SPRITE_COUNT_Y 29
#define WORLD_OFFSET_X 0.853
#define WORLD_OFFSET_Y 1.55
using namespace std;
GLuint LoadTexture(const char *image_path)
{
    SDL_Surface *surface= IMG_Load(image_path);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    SDL_FreeSurface(surface);
    
    return textureID;
}
void drawTile(unsigned int texture,float x, float y,int index )
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glMatrixMode(GL_MODELVIEW);
    glTranslatef(-TILE_SIZE * 128.0f/2, TILE_SIZE * 32.0f/2, 0.0f);
    float u = (float)(((int)index) % SPRITE_COUNT_X) / (float) SPRITE_COUNT_X;
    float v = (float)(((int)index) / SPRITE_COUNT_X) / (float) SPRITE_COUNT_Y;
    
    float spriteWidth = 1.0f/(float)SPRITE_COUNT_X-0.01f;//count for the pixel offset
    float spriteHeight = 1.0f/(float)SPRITE_COUNT_Y-0.01f;
    GLfloat quad[] = {TILE_SIZE * x, -TILE_SIZE * y,
        TILE_SIZE * x, (-TILE_SIZE * y)-TILE_SIZE,
        (TILE_SIZE * x)+TILE_SIZE, (-TILE_SIZE * y)-TILE_SIZE,
        (TILE_SIZE * x)+TILE_SIZE, -TILE_SIZE * y
    };
    glVertexPointer(2, GL_FLOAT, 0, quad);
    glEnableClientState(GL_VERTEX_ARRAY);
    GLfloat quadUVs[] = { u, v,
        u, v+spriteHeight,u+spriteWidth, v+spriteHeight,
        u+spriteWidth, v    };
    
    glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
void DrawText(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a,float x,float y) {
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_MODELVIEW);
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
    glMatrixMode(GL_PROJECTION);
    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
    
}

final::final(): done(false),lastFrameTicks(0.0),timeLeftOver(0.0),playerAction(0),animationElapsed(0.0),state(STATE_GAME){
    Init();
    done = false;
    lastFrameTicks = 0.0f;
}
final::~final(){
    //Mix_FreeChunk(someSound);
   // Mix_FreeMusic(background);
    SDL_Quit();
    SDL_Quit();
    
}
void final::Init(){
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
   // Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 );
    GLuint terrain=LoadTexture("/Users/Alvong/Documents/game programming/hw55/NYUCodebase/spritesheet_rgba.png");
    GLuint playertexture=LoadTexture("/Users/Alvong/Documents/game programming/hw55/NYUCodebase/george_0.png");
    //Mix_Music *music=Mix_LoadMUS("/Users/Alvong/Documents/game programming/hw55/NYUCodebase/Music.mp3");
   // Mix_Chunk *music2=Mix_LoadWAV("/Users/Alvong/Documents/game programming/hw55/NYUCodebase/punch-3.wav");
   // background=music;
    //collide=music2;
    tiles=terrain;
    readleveldata();
    //player actions index: walking , jump
    int Paction[3][2]={{3,7},{1,5},{6,2}};
    
    for (int i=0; i<3; i++) {
        for (int j=0;j<2;j++) {
            
            playerAnimation[i][j]=Paction[i][j];
        }
        
    }
    
    //index initial is 0
    playerAction=0;
    //setting up player
    Entity *first=new Entity(playertexture, playerAnimation[0][0], playerdata[0][0],playerdata[0][1]);
    player=first;
    //player->isStatic=false;
    player->spritecountx=4;
    player->spritecounty=4;
    //Mix_PlayMusic(background, -1);
    
}

bool final::UpdateAndRender(){
    
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
        player->moving();
        
        collision();
        if (player->collided()) {
            //Mix_PlayChannel(1, collide, 0);
        }
        
    }
    timeLeftOver = fixedElapsed;
    Update(elapsed);
    Render();
    return done;
}
void final::Render(){
    /* draw terrain*/
    glClear(GL_COLOR_BUFFER_BIT);
    if (state==STATE_GG) {
        glClear(GL_COLOR_BUFFER_BIT);
        DrawText(LoadTexture("font2.png"), "THE END", .1, .0005, 1.0, 1.0, 1.0, 1.0,0.0,0.0);
    }
    if (state==STATE_GAME) {
        
        
        glMatrixMode(GL_MODELVIEW);
        //camera
        glTranslatef(player->camerax, player->cameray, 0.0);
        makeTerrain(tiles);
        
        
        glPushMatrix();
        player->render();
        glPopMatrix();
    }
    SDL_GL_SwapWindow(displayWindow);
}

void final::Update(float elapsed){
    
    SDL_Event event;
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true; }
    }
    animationElapsed+=elapsed;
    if (animationElapsed>1.0/FRAME_PER_SEC) {
        playerAction++;
        animationElapsed=0.0;
    }
    if (playerAction>1) {
        playerAction=0;
    }
    //if player moves
    
    if(keys[SDL_SCANCODE_LEFT]&&!player->collidedLeft){
        
        player->index=playerAnimation[1][playerAction];
        player->velocity_x=-0.5;
        
        
    }
    if(keys[SDL_SCANCODE_RIGHT]&&!player->collidedRight){
        
        player->index=playerAnimation[0][playerAction];
        player->velocity_x=.5;
        
    }
    
    
    if(keys[SDL_SCANCODE_UP]&&player->collidedBottom){
        
        
        player->collidedBottom=false;
        player->index=playerAnimation[2][0];
        player->velocity_y=1.4;
    }
    
    //reset the position every frame
    player->reset();
    //if player falls
    if (player->y<-10.0) {
        state=STATE_GG;
    }
    
}
void final::fixUpdate(){}

void final::makeTerrain(unsigned int terrainEnemy){//this function records the x,y and index for drawtiles
    for( int y=0; y < 32; y++) {
        for(int x=0; x < 128; x++) {
            if(leveldata[y][x] != 0)
            {
                glPushMatrix();
                //add 55 so the camera starts in the beginning
                drawTile(tiles, x+55, y, leveldata[y][x]);
                glPopMatrix();
                
            }
        }
    }
}
void final::worldToTileCoordinates(float worldX, float worldY, int *gridX, int *gridY) {
    *gridX = (int)((worldX + (WORLD_OFFSET_X)) / TILE_SIZE);
    *gridY = (int)((-worldY + (WORLD_OFFSET_Y)) / TILE_SIZE);
}
void final::tileToWorld(float *worldX, float *worldY, int gridX, int gridY) {
    *worldX = (float)((gridX)*TILE_SIZE-WORLD_OFFSET_X);
    *worldY =(float)(gridY*TILE_SIZE-WORLD_OFFSET_Y)*-1.0f;
}
void final::collision()
{
    float tx;
    float ty;
    //check the tiles around it
    for( int y=0; y < 32; y++) {
        for(int x=0; x < 128; x++) {
            if(leveldata[y][x] != 0)
            {
                tileToWorld(&tx, &ty, x,y);
                
                checkY(tx, ty);
                checkX(tx, ty);
            }
        }
    }
    
    
}
void final::checkX(const float worldX, const float worldY)
{
    float xpen=0.0;
    //collide left
    if ((player->x>worldX)&&(player->x-worldX)<(0.04f+TILE_SIZE/2.0f)&&
        player->y<(worldY+TILE_SIZE/2.0f)&&
        player->y>(worldY-TILE_SIZE/2.0f)
        ) {
        player->velocity_x=0.0;
        xpen=fabs((worldX+TILE_SIZE/2.0f)-(player->x-0.04f));
        player->x+=xpen;
        player->collidedLeft=true;
        cout<<"grid"<<worldX<<","<<worldY<<endl;
        player->camerax=0.0;
        
    }
    //collide right
    else if ((player->x<worldX)&&(worldX-player->x)<(0.04f+TILE_SIZE/2.0f)&&
             player->y<(worldY+TILE_SIZE/2.0f)&&
             player->y>(worldY-TILE_SIZE/2.0f)
             )
    {
        player->velocity_x=0.0;
        xpen=-fabs((player->x+0.04f)-(worldX-TILE_SIZE/2.0f));
        player->x+=xpen;
        player->collidedRight=true;
        cout<<"grid"<<worldX<<","<<worldY<<endl;
        player->camerax=0.0;
        
    }
    
}
void final::checkY(const float worldX, const float worldY)
{
    if ((player->y>worldY)&&(player->y-worldY)<(0.04f+TILE_SIZE/2)&&
        player->x<(worldX+TILE_SIZE/2)&&
        player->x>(worldX-TILE_SIZE/2)
        )
    {
        player->collidedBottom=true;
        player->velocity_y=0.0;
        float ypen=fabs(-(player->y-0.04f)+(worldY+TILE_SIZE/2));
        player->y+=ypen;
        cout<<"grid"<<worldX<<","<<worldY<<endl;
        player->cameray=0.0;
        
    }
    
    
    // collide top
    else if ((player->y<worldY)&&(worldY-player->y)<(0.04f+TILE_SIZE/2)&&
             player->x<(worldX+TILE_SIZE/2)&&
             player->x>(worldX-TILE_SIZE/2)
             )
    {   player->collidedTop=true;
        float ypen=-fabs((player->y+0.04f)-(worldY-TILE_SIZE/2));
        player->y+=ypen;
        cout<<"grid"<<worldX<<","<<worldY<<endl;
        player->cameray=0.0;
        
    }
    
    
}

void final::readleveldata(){
    float width=0;
    float height=0;
    string heightvalue,widthvalue;
    ifstream infile("/Users/Alvong/Documents/game programming/hw55/NYUCodebase/map.txt");
    string line;
    vector<vector<int>> temp;
    while (getline(infile, line)) {
        if(line=="[header]")
        {
            getline(infile, line);
            istringstream iss(line);
            string key1;
            getline(iss,key1,'=');
            getline(iss,widthvalue);
            //read height
            getline(infile, line);
            string key2;
            istringstream iss2(line);
            getline(iss2,key2,'=');
            getline(iss2,heightvalue);
            width = atoi(widthvalue.c_str());
            height = atoi(heightvalue.c_str());
        }
        if(line == "[layer]") {
            getline(infile, line);
            getline(infile, line);
            if (line=="data="&&height!=0&&width!=0)
            {
                for(int y=0; y < height; y++) {
                    getline(infile, line);
                    istringstream lineStream(line);
                    string tile;
                    vector<int>widthdata;
                    for(int x=0; x < width; x++) {
                        getline(lineStream, tile, ',');
                        unsigned  val =  (unsigned)atoi(tile.c_str());
                        if(val > 0)
                        {
                            // be careful, the tiles in this format are indexed from 1 not 0
                            widthdata.push_back(val-1);
                            
                        } else
                        {
                            widthdata.push_back(0);
                        }
                    }
                    temp.push_back(widthdata);
                }
                
            }
            
        }
        if (line == "[objects]") {
            getline(infile, line);
            if (line=="# player") {
                vector<float> temp;
                getline(infile, line);
                getline(infile, line);
                istringstream lineStream(line);
                string location,value;
                getline(lineStream, location, '=');
                getline(lineStream, value);
                string x,y;
                istringstream xy(value);
                getline(xy, x, ',');
                getline(xy, y, ',');
                float xp = atoi(x.c_str());
                float yp = atoi(y.c_str());
                float wxp;
                float wyp;
                tileToWorld(&wxp, &wyp, xp, yp);
                temp.push_back(wxp+0.2);//world offset
                temp.push_back(wyp);
                playerdata.push_back(temp);
                
                
                
            }
            if (line=="# enemy") {
                vector<float> temp2;
                getline(infile, line);
                getline(infile, line);
                istringstream lineStream(line);
                string location,value;
                getline(lineStream, location, '=');
                getline(lineStream, value);
                string x,y;
                istringstream xy(value);
                getline(xy, x, ',');
                getline(xy, y, ',');
                int xp = atoi(x.c_str());
                int yp = atoi(y.c_str());
                
                temp2.push_back(xp*TILE_SIZE-WORLD_OFFSET_X);
                temp2.push_back(-yp*TILE_SIZE+WORLD_OFFSET_Y);
                enemydata.push_back(temp2);
            }
            if (line=="# key") {
                vector<float> temp3;
                getline(infile, line);
                getline(infile, line);
                istringstream lineStream(line);
                string location,value;
                getline(lineStream, location, '=');
                getline(lineStream, value);
                string x,y;
                istringstream xy(value);
                getline(xy, x, ',');
                getline(xy, y, ',');
                int xp = atoi(x.c_str());
                int yp = atoi(y.c_str());
                temp3.push_back(xp);
                temp3.push_back(yp);
                keydata.push_back(temp3);
            }
        }
        
    }
    for (int i=0; i<height; ++i) {
        vector<int> heights;
        for (int j=0; j<width; ++j) {
            heights.push_back(temp[i][j]);
        }
        leveldata.push_back(heights);
    }
}