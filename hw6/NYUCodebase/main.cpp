
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
    
    Entity(float x, float y,float scalex=2,float scaley=2):x(x),y(y),scale_x(scalex),scale_y(scaley),rotation(0)
    {

        vec.x=x;
        vec.y=y;
        vec.z=0;
        
    }
    void draw()
    {
        build();
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glMultMatrixf(matrix.ml);
        //buildvec();
        glTranslatef(x, y, 0.0);
        //glRotatef(rotation, 0.0, 0.0, 1.0);
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
        glMultMatrixf(matrix.ml);
        //buildvec();
        glTranslatef(x, y, 0.0);
        //glRotatef(rotation, 0.0, 0.0, 1.0);
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
        matrix= matrix.scale(scale_x,scale_y)*matrix.roate(rotation);
    }
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
    Entity *one=new Entity(0.0,0.0);
    player=one;

    
}
    ~spacegame() {
    SDL_Quit(
    );
}
void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    


    player->draw();
    
    // render stuff
    SDL_GL_SwapWindow(displayWindow);
    
    
}
void Update(float elapsed) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true; }
        
    }
    //player->vec.x+=0.05;
    //player->rotation+=45;
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






