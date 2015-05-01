
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include "final.h"
#include "Entity.h"
using namespace std;


int main(int argc, char *argv[])
{
    
    final app;
    while(!app.UpdateAndRender()) {}
    return 0;}