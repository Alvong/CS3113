
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "Matrix.h"
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

using namespace std;

class Entity
{
    
public:
    Matrix matrix;
    Vector vec;
    float x;
    float y;
    float scale_x;
    float scale_y;
    float rotation;
    
    Entity(float x, float y,float scalex=1,float scaley=1):x(x),y(y),scale_x(scalex),scale_y(scaley),rotation(0)
    {

        vec.x=x;
        vec.y=y;
        vec.z=1;
        
    }
    void draw()
    {
        //my matrix class used row major ;/ so i have to use glMultTransposeMatrixf()
        build();
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glMultTransposeMatrixf(matrix.ml);
        GLfloat quad[] = {-0.05f, 0.05f, -0.05f, -0.05f, 0.05f, -0.05f, 0.05f, 0.05f};
        glVertexPointer(2, GL_FLOAT, 0, quad);
        glEnableClientState(GL_VERTEX_ARRAY);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        glPopMatrix();
        

    }
    void drawObject()
    {
        build();
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(x, y, 0.0);
        glMultTransposeMatrixf(matrix.ml);
        GLfloat triangle[] = {-0.05f, 0.05f, -0.05f, -0.05f, 0.05f, -0.05f};
        glVertexPointer(2, GL_FLOAT, 0, triangle);
        glEnableClientState(GL_VERTEX_ARRAY);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableClientState(GL_VERTEX_ARRAY);
        glPopMatrix();

    }
    void rotate(float angle)
    {
        matrix.roate(angle);
    }
    void changesize(float scalex, float scaley)
    {
        matrix.scale(scalex,scaley);
    }
    void build()
    {   //matrix.scale(scale_x, scale_y)*matrix.roate(rotation)**matrix.translatef(vec.x,vec.y,vec.z)
        //matrix.identity();
        matrix=matrix.scale(scale_x, scale_y)*matrix.roate(0)*matrix.translate(x,y,vec.z);
        vec.x=x;
        vec.y=y;
    }
    //matrix.scale(scale_x,scale_y)*matrix.roate(rotation)*
//    void buildvec()
//    {
//        
//        Vector temp=matrix.scale(scale_x, scale_y)*vec;
//        Vector temp2=matrix.roate(rotation)*temp;
//        Vector temp3=matrix.translatef(x, y)*temp2;
//        vec=temp3;
//        cout<<vec.x<<vec.y<<vec.z<<endl;
//        
//    }
    
    
};
class spacegame {
private:
Entity *player;
std::vector<Entity*> objects;
float timeLeftOver;
bool done;
float lastFrameTicks;
SDL_Window* displayWindow;
public:
spacegame(){
    timeLeftOver=0.0;
    Init();
    done = false;
    lastFrameTicks = 0.0f;
}
void Init() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    Entity *one=new Entity(0.5,0);
    player=one;
    
    Entity *enemy=new Entity(0.2,0.2);
    objects.push_back(enemy);
    Entity *enemy2=new Entity(-0.4,-0.2);
    objects.push_back(enemy2);
    Entity *enemy3=new Entity(-0.4,0.2);
    objects.push_back(enemy3);
    
}
    ~spacegame() {
    SDL_Quit(
    );
}
void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    

    player->draw();
    for (Entity* one: objects) {
        one->drawObject();
    }
    
    // render stuff
    SDL_GL_SwapWindow(displayWindow);
    
    
}
void Update(float elapsed) {
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true; }
        
    }
    if(keys[SDL_SCANCODE_LEFT]){
    
        player->x+=-0.01;
        
        
    }
    if(keys[SDL_SCANCODE_RIGHT]){
        
       player->x+=0.01;
        
    }
    
    
    if(keys[SDL_SCANCODE_UP])
    {
        
        player->y+=0.01;
    }
    if(keys[SDL_SCANCODE_DOWN]){
        
        player->y+=-0.01;
        
        
    }


    
}
bool UpdateAndRender()
    {
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
            //fix update here
            objects[0]->rotation+=45;
            objects[1]->rotation+=-25;
            player->rotation+=1;
        }
        timeLeftOver = fixedElapsed;
    Update(elapsed);
    Render();
    return done;
    }
};
int main(int argc, char *argv[])
{
    spacegame app;
    while(!app.UpdateAndRender()) {}
    return 0;
}






