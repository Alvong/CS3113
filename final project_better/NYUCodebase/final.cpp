

#include "final.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <queue>
#include <math.h>

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
#define FRAME_PER_SEC 10.0f
#define TILE_SIZE .08f
#define SPRITE_COUNT_X 30
#define SPRITE_COUNT_Y 30
#define WORLD_OFFSET_X -0.04
#define WORLD_OFFSET_Y -0.05
using namespace std;
float easeIn(float from, float to, float k) {
    float tVal = sin( k * 3.14 / 2 );
    return (1.0f-tVal)*from + tVal*to;
}
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
void final::drawBack(unsigned int back)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, back);
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
    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
};

void drawTile(unsigned int texture,float x, float y,int index)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glMatrixMode(GL_MODELVIEW);
   // glTranslatef(-TILE_SIZE * 40.0f/2, TILE_SIZE * 20.0f/2, 0.0f);
    float u = (float)(((int)index) % SPRITE_COUNT_X) / (float) SPRITE_COUNT_X;
    float v = (float)(((int)index) / SPRITE_COUNT_X) / (float) SPRITE_COUNT_Y;
    
    float spriteWidth = 1.0f/(float)SPRITE_COUNT_X;//count for the pixel offset
    float spriteHeight = 1.0f/(float)SPRITE_COUNT_Y;
    GLfloat quad[] = {TILE_SIZE * x, -TILE_SIZE * y,
        TILE_SIZE * x, (-TILE_SIZE * y)-TILE_SIZE,
        (TILE_SIZE * x)+TILE_SIZE, (-TILE_SIZE * y)-TILE_SIZE,
        (TILE_SIZE * x)+TILE_SIZE, -TILE_SIZE * y
    };
    glVertexPointer(2, GL_FLOAT, 0, quad);
    glEnableClientState(GL_VERTEX_ARRAY);
    GLfloat quadUVs[] = { u, v,
        u, v+spriteHeight,u+spriteWidth, v+spriteHeight,
        u+spriteWidth, v };
    
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
    //glMatrixMode(GL_MODELVIEW);
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
   // glLoadIdentity();
    glTranslatef(x, y, 0.0);
    glColorPointer(4, GL_FLOAT, 0, colorData.data());
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
    glEnableClientState(GL_VERTEX_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDrawArrays(GL_QUADS, 0, text.size() * 4.0);
    //glMatrixMode(GL_PROJECTION);
    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
    
}

final::final(): done(false),lastFrameTicks(0.0),timeLeftOver(0.0),playerAction(0),animationElapsed(0.0),state(STATE_MENU),loading(false),titleaAnimation(0.0){
    Init();
    done = false;
    lastFrameTicks = 0.0f;
}
void final::reset()
{
    //leveldata2.clear();
    for (vector<int> vec:leveldata) {
        vec.clear();
    }
    leveldata.clear();
    playerdata.clear();
    if ( !food.empty() )
    {
        for( std::vector<Entity*>::iterator i = food.begin(); i != food.end(); ++i )
        {
            delete *i ;
            *i=nullptr;
        }
    }
    food.clear();
    if ( !poops.empty() )
    {
        for( std::vector<Entity*>::iterator i = poops.begin(); i != poops.end(); ++i )
        {
            delete *i ;
            *i=nullptr;
        }
    }
    poops.clear();
    if ( !poopOnG.empty() )
    {
        for( std::vector<Entity*>::iterator i = poopOnG.begin(); i != poopOnG.end(); ++i )
        {
            delete *i ;
            *i=nullptr;
        }
    }
    poopOnG.clear();
    if ( !ladders.empty() )
    {
        for( std::vector<Entity*>::iterator i = ladders.begin(); i != ladders.end(); ++i )
        {
            delete *i ;
            *i=nullptr;
        }
    }
    ladders.clear();
    if ( !enemies.empty() )
    {
        for( std::vector<Entity*>::iterator i = enemies.begin(); i != enemies.end(); ++i )
        {
            delete *i ;
            *i=nullptr;
        }
    }
    enemies.clear();


    delete player;
    player=nullptr;
    
    delete door;
    door=nullptr;
    
//    for (int i=0; i<enemies.size(); i++) {
//        delete enemies[i];
//        enemies[i]= nullptr;
//    }
//    enemies.clear();
    
}
final::~final(){
    
    //Mix_FreeChunk(someSound);
   // Mix_FreeMusic(background);
    reset();
    SDL_Quit();
       
}
void final::Init(){
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);    displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 1200, 800, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
   // Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 );
    GLuint map=LoadTexture("/Users/Alvong/Documents/game programming/final project/NYUCodebase/spritesheet2_rgba.png");
    GLuint playertextures=LoadTexture("/Users/Alvong/Documents/game programming/final project/NYUCodebase/playertexture.png");
    GLuint poopt=LoadTexture("/Users/Alvong/Documents/game programming/final project/NYUCodebase/200_s.png");
    GLuint backg=LoadTexture("/Users/Alvong/Documents/game programming/final project/NYUCodebase/xnt9w1.png");
    GLuint menub=LoadTexture("/Users/Alvong/Documents/game programming/final project/NYUCodebase/menu.png");
    GLuint fontT=LoadTexture("/Users/Alvong/Documents/game programming/final project/NYUCodebase/font2.png");
    GLuint ghostT=LoadTexture("/Users/Alvong/Documents/game programming/final project/NYUCodebase/enemytexture.png");
    ifstream level1("/Users/Alvong/Desktop/game2.txt");
    //ifstream level2("/Users/Alvong/Desktop/game3.txt");
    //ifstream level3("/Users/Alvong/Desktop/game4.txt");
    
    //Mix_Music *music=Mix_LoadMUS("/Users/Alvong/Documents/game programming/hw55/NYUCodebase/Music.mp3");
   // Mix_Chunk *music2=Mix_LoadWAV("/Users/Alvong/Documents/game programming/hw55/NYUCodebase/punch-3.wav");
   // background=music;
    //collide=music2;
    playertexture=playertextures;
    menuback=menub;
    ghost=ghostT;
    font=fontT;
    back=backg;
    tiles=map;
    poop=poopt;
   // playertexture=playertextures;
    readleveldata(level1,&leveldata);
    level=1;
    //readleveldata(level2,&leveldata2);
    //readleveldata(level3,&leveldata3);
    //player actions index: walking , jump
    int Paction[4][2]={{2,5},{3,4},{6,0},{7,8}};
    
    for (int i=0; i<4; i++) {
        for (int j=0;j<2;j++) {
            
            playerAnimation[i][j]=Paction[i][j];
        }
        
    }
    //enemy action
    int Eaction[3]={0,1,2};
    for (int i=0; i<3; i++) {
        EnemyAnimation[i]=Eaction[i];
    }

    //index initial is 0
    playerAction=0;
    //setting up player
    Entity *first=new Entity(playertexture, playerAnimation[0][0], playerdata[0][0],playerdata[0][1]+0.01);
    player=first;
    player->spritecountx=9;
    player->spritecounty=1;
    player->dead=false;
    player->shown=true;
    makeGraph(leveldata);
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
        if (state==STATE_MENU) {
            titleaAnimation=easeIn(-0.7, -0.8,lastFrameTicks);
        }
        
        if (state==STATE_GAME_ONE) {
            
        
        player->moving();
        collision(player);
        collideObjects();
        enemyNavigation();
        if (enemies.size()>0) {
                
            
        for(int i=0;i<enemies.size();i++)
        {
            collision(enemies[i]);
        }
            }
        
        }
    }
    timeLeftOver = fixedElapsed;
    Update(elapsed);
    Render();

    return done;
}

void final::drawObjects()
{
    makeTerrain(tiles);
//    if (key) {
//        
//    glPushMatrix();
//    key->render();
//    glPopMatrix();
//    }
    if (door) {

    glPushMatrix();
    door->render();
    glPopMatrix();
    }
    for (Entity* block: opBlocks) {
        glPushMatrix();
        block->render();
        glPopMatrix();

    }
    for (Entity* block: enemies) {
        glPushMatrix();
        block->render();
        glPopMatrix();
        
    }

    for (Entity* block: food) {
        glPushMatrix();
        block->render();
        glPopMatrix();

    }
    
    if (poopOnG.size()>0)
    {
        
        for (Entity* block: poopOnG) {
            glPushMatrix();
            block->render();
            glPopMatrix();
            
        }
        
    }
    for (Entity* block: ladders) {
        glPushMatrix();
        block->render();
        glPopMatrix();
        
    }

    glPushMatrix();
    player->render();
    glPopMatrix();


}
void final::Render(){
    /* draw terrain*/
    glClear(GL_COLOR_BUFFER_BIT);
    if (state==STATE_MENU) {
        glClear(GL_COLOR_BUFFER_BIT);
        drawBack(menuback);
        glLoadIdentity();
        DrawText(font, "Alien Adventure", .1, .0007, 128.0, 128.0, 128.0, 1.0,titleaAnimation,0.3);
         glLoadIdentity();
        DrawText(font, "Press Space to continue..", .05, .0005, 0.0, 0.0, 0.0, .8,-0.6,-.5);
        
    }
    if (state==STATE_GG) {
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
        DrawText(font, "The End", .08, .0005, 1.0, 1.0, 1.0, 1.0,-0.8,0.0);
        glLoadIdentity();
        DrawText(font, "Hit Space to exit..", .05, .0007, 128.0, 128.0, 128.0, 1.0,-.8,-0.5);
        
    }
    if (state==STATE_GAME_ONE) {
        
        glMatrixMode(GL_MODELVIEW);
        drawBack(back);
        glPushMatrix();
        DrawText(font, "Food:"+to_string(food.size()), .04, .0005, 1.0, 1.0, 1.0, 1.0,-0.95,0.9);
        glPopMatrix();
        glPushMatrix();
        DrawText(font, "Ammo Time:"+to_string(ammotime)+"/15", .04, .0005, 1.0, 1.0, 1.0, 1.0,-.5,0.9);
        glPopMatrix();
        glPushMatrix();
        DrawText(font, "Ammo:"+to_string(ammo), .04, .0005, 1.0, 1.0, 1.0, 1.0,0.7,0.9);
        glPopMatrix();
        if (player->slowed){
        glPushMatrix();
        DrawText(font, "SLOW:"+to_string(player->slowtime), .05, .0005, 255.0, 0.0, 0.0, 1.0,-0.1,0.8);
        glPopMatrix();
        }

        glPushMatrix();
        glTranslatef(-player->x, -player->y-0.4, 0.0);
        drawObjects();
        glPopMatrix();
           }
    SDL_GL_SwapWindow(displayWindow);
}
bool final::climbing()
{
   bool flag=false;
    for (Entity* block: ladders)
    {
        if (player->collidewith(block)) {
            flag=true;
        }
        
    }
    return flag;
    
}
void final::makeGraph(std::vector<std::vector<int>> leveldata)
{
    for( int y=0; y < leveldata.size(); y++) {
        for(int x=0; x < leveldata[y].size(); x++) {
            if(leveldata[y][x] == 0)
            {
                Node* temp=new Node(x,y);
                graph.push_back(temp);
            }
        }
    }
    ///connect them
    int x,y;
    for (int i=0; i<graph.size(); ++i)
    {
        x=graph[i]->x;
        y=graph[i]->y;
        if (x-1>-1&&leveldata[y][x-1]==0) {
            int index=FindNode(x-1, y);
            if (index!=-1) {
                graph[i]->addEdge(graph[index]);
            }
           
        }
        if (x+1<40&&leveldata[y][x+1]==0) {
            int index=FindNode(x+1, y);
            if (index!=-1) {
                graph[i]->addEdge(graph[index]);
            }
        }
        if (y-1>-1&&leveldata[y-1][x]==0) {
            int index=FindNode(x, y-1);
            if (index!=-1) {
                graph[i]->addEdge(graph[index]);
            }

        }
        if (y+1<20&&leveldata[y+1][x]==0) {
            int index=FindNode(x, y+1);
            if (index!=-1) {
                graph[i]->addEdge(graph[index]);
            }        }

    }
    
    

}
int final::FindNode(int x, int y)
{
    int index=-1;
    for (int i=0; i<graph.size(); i++) {
        if (graph[i]->x==x&&graph[i]->y==y) {
            index=i;
        }
    }
    return index;
}
int final::findOnG(Entity* one)
{
    int index=-1;
    int xp,yp;
    worldToTileCoordinates(one->x, one->y, &xp, &yp);
    for (int i=0; i<graph.size(); i++) {
        if (graph[i]->x==xp&&graph[i]->y==yp) {
            index=i;
        }
    }
    return index;
}
//void final::enemymoving(Entity* enemy,Entity* player)
//{   int start=-1;
//    int end=-1;
//    start=findOnG(enemy);
//    end=findOnG(player);
//    vector<vector<float>> directions;
//    if (start!=-1&&end!=-1) {
//        if (graph[start]->BFS(graph[end]))
//        {
//            Node* temp;
//            //vector<vector<float>> directions;
//            //vector<float> direction;
//            temp=graph[end];
//            while (temp->cameFrom) {
//                float x,y;
//                float vx,vy;
//                vector<float> direction;
//
//                //cout<<temp->x<<","<<temp->y<<endl;
//                tileToWorld(&x, &y, temp->x, temp->y);
//                if (enemy->x<x)
//                {
//                    vx=0.03;
//                    
//                }
//                else if (enemy->x>x) {
//                    vx=-0.03;
//                }
//                else {
//                    vx=0;
//                }
//                if (enemy->y<y)
//                {
//                    vy=0.03;
//                    
//                }
//                else if (enemy->y>y) {
//                    vy=-0.03;
//                }
//                else {
//                    vy=0;
//                }
//
//                direction.push_back(vx);
//                direction.push_back(vy);
//                directions.push_back(direction);
//                
//                //enemy->headingTo(x, y);
//                temp=temp->cameFrom;
//            }
//            
//
//        }
//        if (directions.size()>0) {
// 
//        for (int i=directions.size()-1.0; i>=0; --i) {
//            //enemy->headingTo(directions[i][0], directions[i][1]);
//            enemy->velocity_x=directions[i][0];
//            enemy->velocity_y=directions[i][1];
//            cout<<directions[i][0]<<","<<directions[i][1]<<endl;
//            enemy->headingTo();
//        }
//
//        }
//        //cout<<directions.size();
//        //enemy->headingTo(directions[4][0], 0.5);
//
//    }
////    cout<<enemy->x<<","<<enemy->y<<endl;
////
////    for (int i=directions.size()-1.0; i>=0; --i) {
////        enemy->headingTo(directions[i][0], directions[i][1]);
////        //enemy->x=directions[i][0];
////        //enemy->y=directions[i][1];
////        
////        cout<<directions[i][0]<<","<<directions[i][1]<<endl;
////        
////   }
//
//    
//}

void final::enemyNavigation()
{
    //enemy moves based on the position of player/ammo
    
    
        for(int i=0;i<enemies.size();i++)
        {   if (poopOnG.size()>0)
            {
            for (int p=0;p<poopOnG.size();p++)
                {
                if (fabs(enemies[i]->x-poopOnG[p]->x)<0.6&&fabs(enemies[i]->y-poopOnG[p]->y)<0.7)
                    {
                        enemies[i]->headingTo(poopOnG[p]);
                        enemies[i]->index=0;
                    }
                if (enemies[i]->collidewith(poopOnG[p]))
                {
                    delete poopOnG[p];
                    enemies[i]->slowed=true;
                    enemies[i]->slowtimeO=SDL_GetTicks();
                    poopOnG.erase(poopOnG.begin()+p);
                    
                        enemies[i]->dead=true;
                        enemies[i]->index=1;
                
                }

                }
            }
            else if(poopOnG.size()==0)
            {
                
                if (fabs(enemies[i]->x-player->x)<0.4&&fabs(enemies[i]->y-player->y)<0.3)
                {
                    enemies[i]->headingTo(player);
                    enemies[i]->index=0;
                }
                else
                {
                    
                    enemies[i]->x+=enemies[i]->velocity_x*FIXED_TIMESTEP;
                    enemies[i]->index=2;
                    
                   
                }
            }
            

        }
//    for (int p=0;p<poopOnG.size();++p) {
//        for(int i=0;i<enemies.size();i++)
//        {
//            if (enemies[i]->collidewith(poopOnG[p]))
//            {
//                delete poopOnG[p];
//                enemies[i]->slowed=true;
//                enemies[i]->slowtimeO=SDL_GetTicks();
//                poopOnG.erase(poopOnG.begin()+p);
//            }
//        }
//    }

}
    //enemy collision

    //enemy slow when touch ammo
    

//void final::makingAmmo()
//{
//        ammotime=0;
//        ++ammo;
//}
void final::Update(float elapsed){
    

    SDL_Event event;
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
            
        }
        if(event.type == SDL_KEYDOWN)
        {
            if(state==STATE_GAME_ONE){
            if(event.key.keysym.scancode == SDL_SCANCODE_SPACE&&player->collidedBottom&&!player->dead)
            {
                player->index=playerAnimation[2][0];
                if (ammo>0) {
                    --ammo;
                    Entity *block=new Entity(poop, 1, player->x+0.05,player->y-0.02);
                    block->spritecountx=1;
                    block->spritecounty=1;
                    block->width=0.02;
                    block->height=0.02;
                    poopOnG.push_back(block);
                            }
            }
            
            else if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE&&!player->dead)
            {
                
                if (state==STATE_MENU)
                {
                    state=save;
                }
                else{state=STATE_GG;}

            }
            
        }
            if (state==STATE_MENU)
            {
                if (event.key.keysym.scancode ==SDL_SCANCODE_SPACE){state=STATE_GAME_ONE;}
            }
            if (state==STATE_GG)
            {
                if (event.key.keysym.scancode ==SDL_SCANCODE_SPACE){
                    
                    done=true;
                    
                   
                    }
            }

        }
        


    }
    if(state==STATE_GAME_ONE&&player){
        save=state;
    animationElapsed+=elapsed;
    if (animationElapsed>1.0/FRAME_PER_SEC) {
        playerAction++;
        animationElapsed=0.0;
    }
    if (playerAction>1) {
        playerAction=0;
    }
    
    //if player moves
    if(keys[SDL_SCANCODE_LEFT]&&!player->collidedLeft&&!player->dead){
        
        player->index=playerAnimation[1][playerAction];
        player->velocity_x=-0.4;
        
    }
    else if(keys[SDL_SCANCODE_RIGHT]&&!player->collidedRight&&!player->dead){
        
        player->index=playerAnimation[0][playerAction];
        player->velocity_x=.4;
        
    }
   
    
   else if(keys[SDL_SCANCODE_UP]&&player->collidedBottom&&!player->dead){
        
        
        player->collidedBottom=false;
        player->index=playerAnimation[3][0];
        player->velocity_y=0.4;
       
    }
   else if(keys[SDL_SCANCODE_UP]&&climbing()&&!player->dead)
   {
       player->velocity_y=.5;
    player->index=playerAnimation[3][playerAction];
       
   }
  
    if (food.size()==0) {
        door->shown=true;
    }
    if (loading)
    {
        ammotime=(SDL_GetTicks()-ammotimeO)/100;
    
    }
    if (ammotime>15) {
        int count=ammotime/15;
        ammotime=0;
        ammo+=count;
        loading=false;
    }
    //////////////if player touches ammo
    if (player->slowed) {
        player->slowtime=(SDL_GetTicks()-player->slowtimeO)/100;
      
    }

    if (player->slowtime!=15&&player->slowed) {
        player->friction_x=50;
    }
    if (player->slowtime>15) {
        player->friction_x=10;
        player->slowed=false;
        player->slowtime=0;
    }
   
    //reset the position every frame
    player->reset();
    //if player falls
    if (player->y<-3.0) {
        state=STATE_GG;
    }
    if (player->dead) {
        player->index=playerAnimation[4][1];
    }
    for(Entity* one: enemies)
    {
        
        if (one->collidedLeft) {
            one->velocity_x=0.05;
        }
         if (one->collidedRight) {
            one->velocity_x=-0.05;
        }
        if (one->collidedTop) {
            one->velocity_x=-0.05;
            one->velocity_y=-0.05;
        }
        
    }
    for (int i=0; i<enemies.size(); ++i) {
        if (enemies[i]->dead) {
            delete enemies[i];
            enemies.erase(enemies.begin()+i);
        }
    }
    }
    
}
void final::fixUpdate(){}

void final::makeTerrain(unsigned int terrainEnemy){//this function records the x,y and index for drawtiles
    for( int y=0; y < leveldata.size(); y++) {
        for(int x=0; x < leveldata[y].size(); x++) {
            if(leveldata[y][x] != 0)
            {
                glPushMatrix();
                drawTile(tiles, x, y, leveldata[y][x]);
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

void final::collideObjects()
{
    if(player->collidewith(door)&&door->shown){
        if(level==1)
        {
            level++;
            ifstream leveltwo("/Users/Alvong/Desktop/game3.txt");
            reset();
            readleveldata(leveltwo,&leveldata) ;
            Entity *first=new Entity(playertexture, playerAnimation[0][0], playerdata[0][0],playerdata[0][1]+0.01);
            player=first;
            player->spritecountx=9;
            player->spritecounty=1;
            player->dead=false;
            player->shown=true;
            state=STATE_MENU;
            return;
        }
        else if(level==2)
        {
            level++;
            ifstream levelthree("/Users/Alvong/Desktop/game4.txt");
            reset();
            readleveldata(levelthree,&leveldata) ;
            Entity *first=new Entity(playertexture, playerAnimation[0][0], playerdata[0][0],playerdata[0][1]+0.01);
            player=first;
            player->spritecountx=9;
            player->spritecounty=1;
            player->dead=false;
            player->shown=true;
            state=STATE_MENU;
            return;

        }
        else if(level==3){ state=STATE_GG;
            return;}
        //state=STATE_GG;
    }
    
    for (int i=0;i<food.size();++i)
    {
        if (player->collidewith(food[i])) {
            delete food[i];
            food.erase(food.begin()+i);
            ammotimeO=SDL_GetTicks();
            loading=true;
        }
        
    }
    for (int i=0;i<poopOnG.size();++i) {
        if (player->collidewith(poopOnG[i])) {
            delete poopOnG[i];
            player->slowed=true;
            player->slowtimeO=SDL_GetTicks();
            poopOnG.erase(poopOnG.begin()+i);
        }
    }
    for (int i=0;i<enemies.size();++i) {
        if (player->collidewith(enemies[i])) {
            player->dead=true;
        }
    }

    


}
void final::collision(Entity* one)
{
    if (!one->dead) {
        
    float tx=0;
    float ty=0;
    //check the tiles around it
    for( int y=0; y < leveldata.size(); y++) {
        for(int x=0; x < leveldata[y].size(); x++) {
            if(leveldata[y][x] != 0)
            {
                tileToWorld(&tx, &ty, x,y);
                checkX(tx, ty,one);
                checkY(tx, ty,one);
            }
        }
    }
    //cout<<player->x<<player->y<<endl;
    
    }
}
void final::checkX(const float worldX, const float worldY,Entity* one)
{
    float xpen=0.0;
    //collide left
    if ((one->x>worldX)&&(one->x-worldX)<(0.04f+TILE_SIZE/2)&&
        one->y<(worldY+TILE_SIZE/2)&&
        one->y>(worldY-TILE_SIZE/2)
        ) {
        one->collidedLeft=true;
        one->velocity_x=0.0;
        xpen=fabs((worldX+TILE_SIZE/2)-(one->x-0.04f));
        one->x+=xpen;
        //cout<<"grid"<<worldX<<","<<worldY<<endl;
    }
    //collide right
    else if ((one->x<worldX)&&(worldX-one->x)<(0.04f+TILE_SIZE/2)&&
             one->y<(worldY+TILE_SIZE/2)&&
             one->y>(worldY-TILE_SIZE/2)
             )
    {
        one->velocity_x=0.0;
        xpen=-fabs((one->x+0.04f)-(worldX-TILE_SIZE/2));
        one->x+=xpen;
        one->collidedRight=true;
        //cout<<"grid"<<worldX<<","<<worldY<<endl;
       
        
    }
    
}
void final::checkY(const float worldX, const float worldY,Entity* one)
{
    if ((one->y>worldY)&&(one->y-worldY)<(0.04f+TILE_SIZE/2)&&
        one->x<(worldX+TILE_SIZE/2)&&
        one->x>(worldX-TILE_SIZE/2)
        )
    {
        one->collidedBottom=true;
        one->velocity_y=0.0;
        float ypen=fabs(-(one->y-0.04f)+(worldY+TILE_SIZE/2));
        one->y+=ypen;
        //cout<<"grid"<<worldX<<","<<worldY<<endl;
       
        
    }
    
    
    // collide top
    else if ((one->y<worldY)&&(worldY-one->y)<(0.04f+TILE_SIZE/2)&&
             one->x<(worldX+TILE_SIZE/2)&&
             one->x>(worldX-TILE_SIZE/2)
             )
        
    {
        one->velocity_y=0.0;
        one->collidedTop=true;
        float ypen=-fabs((one->y+0.04f)-(worldY-TILE_SIZE/2));
        one->y+=ypen;
        //cout<<"grid"<<worldX<<","<<worldY<<endl;
           }
    
    
}

void final::readleveldata(ifstream& infile,std::vector<std::vector<int>> *leveldata){
    float width=0;
    float height=0;
    string heightvalue,widthvalue;
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
        if (line == "[object]") {
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
                temp.push_back(wxp);
                temp.push_back(wyp);
                playerdata.push_back(temp);
                
                
                
            }
            if (line=="# food") {
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
                float wxp;
                float wyp;
                tileToWorld(&wxp, &wyp, xp, yp);
                Entity *block=new Entity(tiles, 596, wxp,wyp+0.08);
               // block->shown=true;
                food.push_back(block);
                           }
//            if (line=="# key") {
//                //vector<float> temp3;
//                getline(infile, line);
//                getline(infile, line);
//                istringstream lineStream(line);
//                string location,value;
//                getline(lineStream, location, '=');
//                getline(lineStream, value);
//                string x,y;
//                istringstream xy(value);
//                getline(xy, x, ',');
//                getline(xy, y, ',');
//                int xp = atoi(x.c_str());
//                int yp = atoi(y.c_str());
//                float wxp;
//                float wyp;
//                tileToWorld(&wxp, &wyp, xp, yp);
//                Entity *keydata=new Entity(tiles, 404, wxp,wyp+0.05);
//                key=keydata;
//                key->shown=false;
//            }
            if (line=="# door") {
                vector<float> temp4;
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
                float wxp;
                float wyp;
                tileToWorld(&wxp, &wyp, xp, yp);
                Entity *doordata=new Entity(tiles, 167, wxp,wyp+0.08);
                door=doordata;
                door->shown=false;
            }
            if (line=="# enemy") {
                vector<float> temp5;
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
                float wxp;
                float wyp;
                tileToWorld(&wxp, &wyp, xp, yp);
                Entity *enemy=new Entity(ghost, 2, wxp,wyp+0.1);
                enemy->spritecountx=1;
                enemy->spritecounty=3;
                enemy->acceleration_y=0;
                enemy->friction_x=0;
                enemy->shown=true;
                enemy->velocity_x=-0.05;
                enemies.push_back(enemy);
                
            }

            if (line=="# opblock") {
                vector<float> temp5;
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
                float wxp;
                float wyp;
                tileToWorld(&wxp, &wyp, xp, yp);
                Entity *opbdata=new Entity(tiles, 131, wxp,wyp+0.19);
                opbdata->shown=true;
                opBlocks.push_back(opbdata);
               
            }
            if (line=="# ladder") {
                vector<float> temp5;
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
                float wxp;
                float wyp;
                tileToWorld(&wxp, &wyp, xp, yp);
                Entity *ladder=new Entity(tiles, 100, wxp,wyp+0.1);
                ladders.push_back(ladder);
                
            }

            
        }
        
    }
    for (int i=0; i<height; ++i) {
        vector<int> heights;
        for (int j=0; j<width; ++j) {
            heights.push_back(temp[i][j]);
        }
        leveldata->push_back(heights);
    }
}
