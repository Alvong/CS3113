
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include <iostream>
#include <cmath>
using namespace std;
int randomN()
{
    // Initialize
    vector<int> a;
    for(int i = 0; i <17; i++)
    { a.push_back(i);}
    
    // shuffle
    for(int i = 17- 1; i > 0; i--) {
      int  j = rand() % i;
        swap(a[i], a[j]);
    }
    return a[0];
}
class bullet
{
public:
    float x;
    float y;
    float length;
    unsigned int texture;
    int direction;
    bool explode;
    bullet():x(0),y(0),length(0),texture(0){}
    bullet(float x,float y,unsigned int texture,int direction):x(x),y(y),length(.2),texture(texture),direction(direction),explode(false){}
    void Draw(float rotation=0)
    {   //if(explode){}
        //y=y+direction*0.02;
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(x, y, 0.0);
        glRotatef(rotation, 0.0, 0.0, 1.0);
        GLfloat quad[] = {-0.02f, 0.05f, -0.02f, -0.05f, 0.02f, -0.05f, 0.02f, 0.05f};
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

    }

    
    };
GLuint LoadTexture(const char *image_path)
{
    SDL_Surface *surface= IMG_Load(image_path);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    SDL_FreeSurface(surface);
    
    return textureID;
}


class object{
    
public:
    float scale;
    float u;
    float v;
    float width;
    float height;
    friend GLuint LoadTexture(const char *image_path);

    object():u(0),v(0),width(0),height(0){}
    object(float u, float v, float width, float height):u(u),v(v),width(width),height(height){}
   
    

};
class player:public object
{
public:
    unsigned int textureID;
    unsigned int bulletID;
    float x;
    float y;
    //bullet abullet;
    bool fired;
    player():textureID(0),bulletID(0),object(),x(0),y(0),fired(false){}
    player(unsigned int texture,unsigned int bulletid, float u, float v, float width, float height,float x,float y,bool fired=false):textureID(texture),bulletID(bulletid),object( u,v,width,height),x(x),y(y){}
    void Draw(float rotation=0, float scale=1)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(x, y, 0.0);
        glRotatef(rotation, 0.0, 0.0, 1.0);
        GLfloat quad[] = {-width * scale , height * scale, -width * scale, -height * scale,width * scale, -height * scale, width * scale, height * scale};
        glVertexPointer(2, GL_FLOAT, 0, quad);
        glEnableClientState(GL_VERTEX_ARRAY);
        GLfloat quadUVs[] = {u, v, u, v+height, u+width, v+height, u+width, v};
        glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisable(GL_TEXTURE_2D);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    }
//    void shoot()
//    {
//        abullet.Draw();
//        if (abullet.y>1 || abullet.y==2) {
//            fired=false;
//            abullet.y=-0.8;
//        }
//        
//    }

};
class enemy:public object
{
public:
    unsigned int textureID;
    unsigned int bulletID;
    float x;
    float y;
    int direction;
    enemy():textureID(0),bulletID(0),object(),x(0),y(0){}
    enemy(unsigned int texture,unsigned int bulletid,float u, float v, float width, float height,float x,float y):textureID(texture),bulletID(bulletid),x(x),y(y),object(u,v,width,height),direction(1){}
    void Draw(float speed,float rotation, float scale=1)
    {

        //x=x+speed*direction;
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(x, y, 0.0);
        glRotatef(rotation, 0.0, 0.0, 1.0);
        GLfloat quad[] = {-width * scale , height * scale, -width * scale, -height * scale,width * scale, -height * scale, width * scale, height * scale};
        glVertexPointer(2, GL_FLOAT, 0, quad);
        glEnableClientState(GL_VERTEX_ARRAY);
        GLfloat quadUVs[] = {u, v, u, v+height, u+width, v+height, u+width, v};
        glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisable(GL_TEXTURE_2D);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    }



};
class terrain:public object
{
public:
    GLuint textureID;
    float x;
    float y;
    terrain():textureID(0),object(),x(0),y(0){}
    terrain(unsigned int texture,float u, float v, float width, float height,float x,float y):textureID(texture),object( u,v,width,height),x(x),y(y){}
    void Draw(float rotation=0, float scale=1)
    {

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(x, y, 0.0);
        glRotatef(rotation, 0.0, 0.0, 1.0);
        GLfloat quad[] = {-width * scale , height * scale, -width * scale, -height * scale,width * scale, -height * scale, width * scale, height * scale};
        glVertexPointer(2, GL_FLOAT, 0, quad);
        glEnableClientState(GL_VERTEX_ARRAY);
        GLfloat quadUVs[] = {u, v, u, v+height, u+width, v+height, u+width, v};
        glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisable(GL_TEXTURE_2D);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    }
    

    
};
enum GameState{STATE_MAIN_MENU, STATE_GAME, STATE_GG};
class spaceInvader {
private:
    unsigned int fonts;
    bool done;
    float lastFrameTicks;
    float timing=0.0;
    SDL_Window* displayWindow;
//    vector<enemy*>enemiesA;
//    vector<enemy*>enemiesB;
//    vector<enemy*>enemiesC;
    vector<enemy*> enemies;
    //vector<enemy> shooters;
    vector<bullet*> bullets;
    player firstplayer;
    friend GLuint LoadTexture(const char *image_path);
    GameState state;
    terrain terrainA;
    terrain terrainB;
    terrain terrainC;
    float limitx;
    float limity;


public:
    

    spaceInvader():done(false),lastFrameTicks(0.0),enemies(NULL),state(STATE_MAIN_MENU),fonts(0),limitx(0.0),limity(0.0)
    {
        
    }
    
    ~spaceInvader()
    {
        SDL_Quit();
    }
    void Init()
    {
        SDL_Init(SDL_INIT_VIDEO);
        displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED,
                                         SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
        SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
        SDL_GL_MakeCurrent(displayWindow, context);
        GLuint ebullet=LoadTexture("/Users/Alvong/Documents/game programming/hw3/NYUCodebase/laserRed13.png");
        GLuint pbullet=LoadTexture("/Users/Alvong/Documents/game programming/hw3/NYUCodebase/laserGreen10.png");
        fonts=LoadTexture("/Users/Alvong/Documents/game programming/hw3/NYUCodebase/font2.png");
        for (int i=0; i<6; ++i) {
            enemy *temp=new enemy(LoadTexture("/Users/Alvong/Documents/game programming/hw3/NYUCodebase/sheet.png"),ebullet,444.0f/1024.0f,91.0f/1024.0f,91.0f/1024.0f,91.0f/1024.0f,(-.6+i*0.25),0.7);
            enemy *temp2=new enemy(LoadTexture("/Users/Alvong/Documents/game programming/hw3/NYUCodebase/sheet.png"),ebullet,434.0f/1024.0f,234.0f/1024.0f,91.0f/1024.0f,91.0f/1024.0f,(-.6+i*0.25),0.5);
            enemy *temp3=new enemy(LoadTexture("/Users/Alvong/Documents/game programming/hw3/NYUCodebase/sheet.png"),ebullet,444.0f/1024.0f,0.0f/1024.0f,91.0f/1024.0f,91.0f/1024.0f,(-.6+i*0.25),0.3);
            enemies.push_back(temp);
            enemies.push_back(temp2);
            enemies.push_back(temp3);
           
            
        }
        
        player player1(LoadTexture("/Users/Alvong/Documents/game programming/hw3/NYUCodebase/sheet.png"),pbullet,211.0f/1024.0f,941.0f/1024.0f,99.0f/1024.0f,75.0f/1024.0f,0, -.8);
        firstplayer=player1;
        
        terrain terrain1(LoadTexture("/Users/Alvong/Documents/game programming/hw3/NYUCodebase/sheet.png"),224.0f/1024.0f,664.0f/1024.0f,101.0f/1024.0f,84.0f/1024.0f,-0.6,-0.6);
        terrain terrain2(LoadTexture("/Users/Alvong/Documents/game programming/hw3/NYUCodebase/sheet.png"),224.0f/1024.0f,664.0f/1024.0f,101.0f/1024.0f,84.0f/1024.0f,0,-0.6);
        terrain terrain3(LoadTexture("/Users/Alvong/Documents/game programming/hw3/NYUCodebase/sheet.png"),224.0f/1024.0f,664.0f/1024.0f,101.0f/1024.0f,84.0f/1024.0f,0.6,-0.6);


        terrainA=terrain1;
        terrainB=terrain2;
        terrainC=terrain3;
        
    }
    void enemiesFire(enemy *one)
    {
        bullet *bulleta=new bullet(one->x,one->y-0.2,one->bulletID,-1);
        bullets.push_back(bulleta);


    }
    void DrawSpark(float x, float y, float rotation=1)
    {
        GLuint texture=LoadTexture("/Users/Alvong/Documents/game programming/hw3/NYUCodebase/flash00.png");
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(x, y, 0.0);
        glRotatef(rotation, 0.0, 0.0, 1.0);
        GLfloat quad[] = {-0.1f, 0.1f, -0.1f, -0.1f, 0.1f, -0.1f, 0.1f, 0.1f};
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

        
    }


    void terrainCheck(bullet *one,int index)
    {   if(state==STATE_GAME){
        float Aleft=terrainA.x-0.1;
        float Aright=terrainA.x+0.1;
        float Bleft=terrainB.x-0.1;
        float Bright=terrainB.x+0.1;
        float Cleft=terrainC.x-0.1;
        float Cright=terrainC.x+0.1;

        
        one->explode=false;
        if (one->y>1 || one->y<-1) { //if it goes out of screen
            bullets.erase(bullets.begin()+index);
            firstplayer.fired=false;
            
        }
        
        if ((one->y>-.7)&&(one->direction==1)&&( //if player's bullet hits terrain
            (Aleft<(one->x)&&(one->x)<Aright)||
            (Bleft<(one->x)&&(one->x)<Bright)||
            (Cleft<(one->x)&&(one->x)<Cright))) {
            bullets.erase(bullets.begin()+index);
            firstplayer.fired=false;
            DrawSpark(one->x, one->y);

        }
        if ((one->y<-.5)&&(one->direction==-1)&&(//if enemy bullets hit terrain
            (Aleft<(one->x)&&(one->x)<Aright)||
            (Bleft<(one->x)&&(one->x)<Bright)||
            (Cleft<(one->x)&&(one->x)<Cright))) {
            bullets.erase(bullets.begin()+index);
            DrawSpark(one->x, one->y);
        }
        if(one->y<firstplayer.y &&one->x<firstplayer.x+0.1&&one->x>firstplayer.x-0.1)//if player dies
        {
            state=STATE_GG;
            bullets.erase(bullets.begin()+index);
            DrawSpark(one->x, one->y);
            firstplayer.fired=true;
        }
//        if (one->direction==2&&bullets.size()>2)//if two bullets collide
//        {
//            for(int i=0;i<bullets.size();++i)
//            {
//                if(index!=i)
//                {
//                if (bullets[i]->x==one->x&&bullets[i]->y>one->y)
//                {
//                    bullets.erase(bullets.begin()+index);
//                    bullets.erase(bullets.begin()+i);
//                    firstplayer.fired=false;
//            }
//                }
//            }
//        }
        if (one->direction==1)
        {
                for(int a=0;a<enemies.size();++a)
                    {
                        if ((enemies[a]->y-0.1<one->y)&&(enemies[a]->x-0.1<(one->x)&&(one->x)<enemies[a]->x+0.1)) {
                            
                            enemies.erase(enemies.begin()+a);
                            bullets.erase(bullets.begin()+index);
                            firstplayer.fired=false;
                            DrawSpark(one->x, one->y);
                        }
                    }
        }
        
    }
    }
    
    void Render() {
        //glClear(GL_COLOR_BUFFER_BIT);
        
        if (state==STATE_MAIN_MENU) {
            DrawText(fonts, "SPACE SHOOTER", .1, .0005, 1.0, 1.0, 1.0, 1.0,-0.6,0.0);
         
            DrawText(fonts, "Press Any Key to Continue..", .05, .0005, 1.0, 1.0, 1.0, 1.0,-0.6,-0.6);
            }
            
        
        else if (state==STATE_GAME)
        {
           
            terrainA.Draw();
            terrainB.Draw();
            terrainC.Draw();
           
         DrawText(fonts, "Unit Killed: "+to_string(trackScore()), .05, .0005, 1.0, 1.0, 1.0, 1.0,-0.85,0.9);
        firstplayer.Draw();
            
            for (int i=0; i<enemies.size(); ++i){enemies[i]->Draw(0.01,lastFrameTicks*100);}


        
        

            for (int i=0; i<bullets.size(); ++i)
            {
                bullets[i]->Draw();
                
                
            }


            
        }
       else if(state==STATE_GG)
        {
            DrawText(fonts, "THE END", .1, .0005, 1.0, 1.0, 1.0, 1.0,-0.6,0.0);
            DrawText(fonts, "NO, YOU CANT PLAY AGAIN", .08, .0005, 1.0, 0.0, 1.0, 1.0,-0.8,-0.3);
            //done=true;
        }

        SDL_GL_SwapWindow(displayWindow);
    }
    void Update(float elapsed) {
        
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true; }
        }
        
        
        ingame(elapsed,event);
    }
    bool UpdateAndRender() {
        
        float ticks = (float)SDL_GetTicks()/1000.0f;
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        Update(elapsed);
        Render();
        return done;
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

    
    int trackScore()
    {
        return 18.0-enemies.size();
    }
    void ingame(float elapsed,SDL_Event event)
    {

        glClear(GL_COLOR_BUFFER_BIT);
        
        if (state==STATE_GAME) {
            
        
                if(event.type == SDL_KEYDOWN)
        {
             if(event.key.keysym.scancode == SDL_SCANCODE_LEFT)
            {
                
                if (firstplayer.x>-1+(firstplayer.width/2))
                {
                    firstplayer.x-=0.02;
                    
                }
                else
                {
                    
                    firstplayer.x=-1+(firstplayer.width/2);
                    
                }
                
            }
             if(event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
            {
                
                if ((firstplayer.x+(firstplayer.width/2))<1-(firstplayer.width/2))
                {
                    
                   firstplayer.x+=0.02;
                    
                }
                else
                {
                    
                   firstplayer.x=1-(firstplayer.width/2);
                }
                
            }
            if(event.key.keysym.scancode==SDL_SCANCODE_UP&&!firstplayer.fired)
                {
                firstplayer.fired=true;
                bullet *abullet=new bullet(firstplayer.x,-0.8,firstplayer.bulletID,1);
                bullets.push_back(abullet);
                //firstplayer.abullet=abullet;


                }
           

            }
            if ((timing/2)>1)
            {
                for (int i=7; i>0; --i) {
                    enemiesFire(enemies[randomN()]);
                }
                
                timing=0;
            }
            
            for(bullet *one: bullets)
            {
                one->y+=(0.01*one->direction);
                //firstplayer.fired=true;
            }
            
            
            for (int i=0; i<bullets.size(); ++i)
            {
                terrainCheck(bullets[i],i);
                
            }
        
            cout<<limitx<<endl;
            for(enemy *one: enemies)
            {
                if(limitx<0.2){one->x+=(0.001*one->direction);}
                if(limitx>=0.2&&limitx<0.6){one->x-=(0.001*one->direction);}
                if(limitx>=0.6&&limitx<1.2){one->x+=(0.001*one->direction);}
                if(limitx>1.2){one->x-=(0.001*one->direction);}
                one->y-=(0.001*one->direction);
                //one->x+=(0.002*one->direction);
                
            }
            limitx+=0.001;
            limity+=0.001;

            if (limity>0.8) {
                state=STATE_GG;
            }
        }
        timing+=elapsed;
        if (state==STATE_GG)
        {
            timing+=elapsed;
            if ((timing/3)>1){done=true;}
            if(event.type == SDL_KEYDOWN){
                glClear(GL_COLOR_BUFFER_BIT);

            }
            
        }
        if (state==STATE_MAIN_MENU) {
            if(event.type == SDL_KEYDOWN){state=STATE_GAME;}
        }

        }
        
        
    

};




int main(int argc, char *argv[])
{
    
    spaceInvader app;
    app.Init();
    while(!app.UpdateAndRender()) {}
    return 0;
     
}