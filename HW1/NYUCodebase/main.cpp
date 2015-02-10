
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>
using namespace std;
SDL_Window* displayWindow;

typedef struct {
    float x;
    float y;
    float r;
    float g;
    float b;
} Vertex2D;

void drawTriangle()
{
    Vertex2D triangle[3] = {{0.0, 0.3, 1.0, 0.0, 0.0},
        {-0.3, -0.3, 0.0, 1.0, 0.0},
        {0.3, -0.3, 0.0, 0.0, 1.0}};
    glVertexPointer(2, GL_FLOAT, sizeof(float) * 5, triangle);
    glEnableClientState(GL_VERTEX_ARRAY);
    glColorPointer(3, GL_FLOAT, sizeof(float) * 5, &triangle[0].r);
    glEnableClientState(GL_COLOR_ARRAY);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    
}
//void glClearcolor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void DrawSprite(GLint texture, float x, float y, float rotation)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(x, y, 0.0);
    glRotatef(rotation, 0.0, 0.0, 1.0);
    GLfloat quad[] = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f};
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

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
    
	bool done = false;
    //////////////////////////////////////loading my texture
    GLuint something= LoadTexture("ufoYellow.png");
    GLuint somethingmore=LoadTexture("elementStone010.png");
    //GLuint another=LoadTexture("dieRed_border1.png");

    float lastFrameTicks = 0.0f;
    float angle=0;
    
	//drawing
    
//    Vertex2D triangle[3] = {{0.0, 0.5, 1.0, 0.0, 0.0},
//        {-0.5, -0.5, 0.0, 1.0, 0.0},
//        {0.5, -0.5, 0.0, 0.0, 1.0}};
//    glVertexPointer(2, GL_FLOAT, sizeof(float) * 5, triangle);
//    glEnableClientState(GL_VERTEX_ARRAY);
//    glColorPointer(3, GL_FLOAT, sizeof(float) * 5, &triangle[0].r);
//    glEnableClientState(GL_COLOR_ARRAY);
//    glDrawArrays(GL_TRIANGLES, 0, 3);
    
	SDL_Event event;
    
	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}
		SDL_GL_SwapWindow(displayWindow);
        
        float ticks = (float)SDL_GetTicks()/1000.0f;
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        angle -= elapsed*100;
        glClear(GL_COLOR_BUFFER_BIT);
        //drawTriangle();
        DrawSprite(something, 0.8, 0.8, angle);
        glLoadIdentity();
        drawTriangle();
        DrawSprite(somethingmore, -0.8, -0.8, angle);
       // DrawSprite(another, 0.0, 0.0, 0);
        

       // drawTriangle();

        
    }
    
	SDL_Quit();
	return 0;
}