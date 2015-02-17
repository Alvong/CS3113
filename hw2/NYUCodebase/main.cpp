
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include<iostream>
#include <cmath>
using namespace std;
class box
{
protected:
    float centerx;
    float centery;
    double topright[2];
    double topleft[2];
    double botright[2];
    double botleft[2];
    int directiony=0;
    int directionx=0;
public:
    box(float x, float y):centerx(x),centery(y)
    {

       
    }
    void move(float x,float y)
    {
        
        centerx+=x;
        centery+=y;
        topright[0]+=x;
        topright[1]+=y;
        topleft[0]+=x;
        topleft[1]+=y;
        botright[0]+=x;
        botright[1]+=y;
        botleft[0]+=x;
        botright[1]+=y;
        
    }
    void resetBall(float x,float y,int direction)
    {
        centerx=x;
        centery=y;
        topright[0]=x+0.05;
        topright[1]=0.05;
        topleft[0]=x-0.05;
        topleft[1]=topright[1];
        botright[0]=topright[0];
        botright[1]=-0.05;
        botleft[0]=topleft[0];
        botleft[1]=botright[1];
        directionx=direction;
        directiony=1;

        
    }
    void changedirectiony()
    {
        
        directiony=-directiony;
        
    }
    void changedirectionx()
    {
        directionx=-directionx;
    }
    float gety()
    {return centery;}
    float getx()
    {return centerx;}
    int getDx(){return directionx;}
    int getDy(){return directiony;}
    void detectcollisionright(const box *abox)
    {
        
        if (abox->botleft[1]<=centery-0.05&&abox->topleft[1]>=centery+0.05){changedirectionx();}
//        else{detectcollisionbot(abox);
//            detectcollisiontop(abox);}
}
    void detectcollisionleft(const box *abox)
    {
        if (abox->botright[1]<=centery-0.05&&abox->topright[1]>=centery+0.05){changedirectionx();}
//        else{detectcollisionbot(abox);
//            detectcollisiontop(abox);}
    }
    void detectcollisiontop(const box *abox)
    {
        if(gety()<abox->topright[1]+0.05){changedirectiony();
            changedirectionx();}
    }
    void detectcollisionbot(const box *abox)
    {
        if (gety()>abox->botright[1]-0.05) {
            changedirectiony();
            changedirectionx();
        }
    }
};
class pad:public box
{
public:
    pad(float x, float y):box(x,y)
    {
        topright[0]=x+0.02;
        topright[1]=0.3;
        topleft[0]=x-0.02;
        topleft[1]=topright[1];
        botright[0]=topright[0];
        botright[1]=-0.3;
        botleft[0]=topleft[0];
        botleft[1]=botright[1];
    }
    void max()
    {
        topleft[1]=1;
        topright[1]=1;
        botleft[1]=0.4;
        botright[1]=0.4;
        centery=0.7;
        
    }
    void min()
    {
        topleft[1]=-0.4;
        topright[1]=-0.4;
        botleft[1]=-1;
        botright[1]=-1;
        centery=-0.7;
    }
    void set(float value)
    {
        centery=value;
        topright[1]=0.3+value;
        topleft[1]=topright[1];
        botright[1]=-0.3+value;
        botleft[1]=botright[1];

        
    }
   };
class ball:public box
{
    public:
    ball(float x,float y):box(x,y)
    {
        topright[0]=x+0.05;
        topright[1]=0.05;
        topleft[0]=x-0.05;
        topleft[1]=topright[1];
        botright[0]=topright[0];
        botright[1]=-0.05;
        botleft[0]=topleft[0];
        botleft[1]=botright[1];
        directionx=-1;
        directiony=1;

    }

};
SDL_Window* displayWindow;

//loads textures
GLuint LoadTexture(const char *image_path) {
    SDL_Surface *surface = IMG_Load(image_path);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_FreeSurface(surface);
    return textureID;
}
//using textures to make mid barrier
void drawMid(GLint texture, float x, float y, float rotation) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(x, y, 0.0);
    glRotatef(rotation, 0.0, 0.0, 1.0);
    GLfloat quad[] = {-.02, 1, -.02, -1, .02, -1, .02, 1};
    glVertexPointer(2, GL_FLOAT, 0, quad);
    glEnableClientState(GL_VERTEX_ARRAY);
    GLfloat quadUVs[] = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0};
    glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisable(GL_TEXTURE_2D);
}
//this is a ball with self rotation
void drawBall(GLint texture, float x, float y, float rotation)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(x, y, 0.0);
    glRotatef(rotation, 0.0, 0.0, 1.0);
    GLfloat quad[] = {-0.05f, 0.05f, -0.05f, -0.05f, 0.05f, -0.05f, 0.05f, 0.05f};
    glVertexPointer(2, GL_FLOAT, 0, quad);
    glEnableClientState(GL_VERTEX_ARRAY);
    GLfloat quadUVs[] = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0};
    glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisable(GL_TEXTURE_2D);
    

}
//drawing pads for players
void drawPad(GLint texture, float x, float y, float rotation) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(x, y, 0.0);
    glRotatef(rotation, 0.0, 0.0, 1.0);
    GLfloat quad[] = {-.02, .3, -.02, -.3, .02, -.3, .02, .3};
    glVertexPointer(2, GL_FLOAT, 0, quad);
    glEnableClientState(GL_VERTEX_ARRAY);
    GLfloat quadUVs[] = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0};
    glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisable(GL_TEXTURE_2D);
}
//void checkCollision(ball* aball,const pad* leftpad, const pad* rightpad)
//{
//    cout<<aball->getDx()<<endl;
//    if (abs(aball->gety())>=0.92) {
//        aball->changedirectiony();
//    }
//    if (aball->getx()>=-.93) {
//        aball->detectcollisionleft(leftpad);
//           
//        }
//
//    
//    if (aball->getx()<=.93) {
//        aball->detectcollisionright(rightpad);
//        }
//    
//}
int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 780, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
    glViewport(0, 0, 1200, 780);
    glMatrixMode(GL_PROJECTION_MATRIX);
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_PROJECTION_MATRIX);
	bool done = false;
	float lastFrameTicks = 0.0f;
    float ballrotation=0;
    ball ball(0,0);
    pad leftPad(-0.94,0);
    pad rightPad(0.94,0);
    float leftkey=leftPad.gety();
    float rightkey=rightPad.gety();
    GLuint aball=LoadTexture("derp.png");
    GLuint mid=LoadTexture("grey_sliderVertical.png");
    GLuint left=LoadTexture("stoneLeft.png");
    GLuint right=LoadTexture("stoneleft.png");
    GLuint winner=LoadTexture("flat_medal3.png");
    SDL_Event event;
    
	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
             }
        
            if(event.type == SDL_KEYDOWN)
            {
                if(event.key.keysym.scancode == SDL_SCANCODE_UP)
                {
                    if (leftkey<0.7) {
                        drawPad(left, -0.94, leftkey+=.02*1, 0);
                        drawPad(right, 0.94, rightkey, 0);
                        leftPad.set(leftkey);
                    }
                    else {
                        leftkey=0.7;
                        drawPad(left, -0.94, 0.7, 0);
                        drawPad(right, 0.94, rightkey, 0);
                        leftPad.max();
                        }
                }
                if(event.key.keysym.scancode == SDL_SCANCODE_DOWN)
                {
                    if (leftkey>-0.7) {
                        drawPad(left, -0.94, leftkey-=.02*1, 0);
                        drawPad(right, 0.94, rightkey, 0);
                        leftPad.set(leftkey);;
                    }
                   else{
                       leftkey=-0.7;
                        drawPad(left, -0.94, -0.7, 0);
                        drawPad(right, 0.94, rightkey, 0);
                       leftPad.min();                    }

                }
                if(event.key.keysym.scancode == SDL_SCANCODE_LEFT)
                {
                    if (rightkey<0.7)
                    {
                    drawPad(right, .94, rightkey+=.02*1, 0);
                    drawPad(left, -.94, leftkey, 0);
                        rightPad.set(rightkey);

                    }
                    else
                    {   rightkey=0.7;
                        drawPad(right, .94, 0.7, 0);
                        drawPad(left, -.94, leftkey, 0);
                        rightPad.max();

                    }

                }
                if(event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
                {
                    
                    if (rightkey>-0.7)
                    {
                        drawPad(right, .94, rightkey-=.02*1, 0);
                        drawPad(left, -.94, leftkey, 0);
                        leftPad.set(rightkey);
                    }
                    else
                    {   rightkey=-0.7;
                        drawPad(right, .94, -0.7, 0);
                        drawPad(left, -.94, leftkey, 0);
                        rightPad.min();
                        
                    }

                }

            }
            else
            {
                drawPad(left, -.94, leftkey, 0);
                drawPad(right, 0.94, rightkey, 0);
                rightPad.set(rightkey);
                leftPad.set(leftkey);
                
            }

		SDL_GL_SwapWindow(displayWindow);
        glClear(GL_COLOR_BUFFER_BIT);
        //time keeping for moving objects
        float ticks = (float)SDL_GetTicks()/1000.0f;
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        ballrotation+=elapsed*200;
        drawMid(mid, 0, 0, 0);
        if (abs(ball.gety())>=0.92) {
            ball.changedirectiony();
        }
        
        if (ball.getx()<-.930+0.03) {
            ball.detectcollisionleft(&leftPad);
//            ball.detectcollisiontop(&leftPad);
//            ball.detectcollisionbot(&leftPad);
        }
        
        if (ball.getx()>.93-0.03) {
            ball.detectcollisionright(&rightPad);
//            ball.detectcollisiontop(&rightPad);
//            ball.detectcollisionbot(&rightPad);
        }

        if (ball.getx()>1.15)
        {
            drawBall(winner, -0.5, 0, 0);
            
        }
        if(ball.getx()<-1.15) {
            drawBall(winner, 0.5, 0, 0);
        }
        if(abs(ball.gety())>1.2||abs(ball.getx())>1.2)
        {ball.resetBall(0, 0,-ball.getDx());}
        ball.move(.01*ball.getDx(), .013*ball.getDy());
        drawBall(aball,ball.getx(), ball.gety(), ballrotation);
        if(event.key.keysym.scancode == SDL_SCANCODE_SPACE)
        {
            
            ball.resetBall(0, 0,1);
        }


    }

	SDL_Quit();
	return 0;
}