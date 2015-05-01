//
//  entity.cpp
//  NYUCodebase
//
//  Created by Alvong on 4/30/15.
//  Copyright (c) 2015 Ivan Safrin. All rights reserved.
//
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>
#include "entity.h"
#define FIXED_TIMESTEP 0.0166666f
float lerp(float v0, float v1, float t) {
    return (1.0-t)*v0 + t*v1;
}
Entity::Entity(unsigned int texture,int index, float x,float y):texture(texture),width(0.04),height(0.04),velocity_x(0.0),velocity_y(0.0),acceleration_x(0.0),acceleration_y(-1.8),friction_x(2.2),friction_y(0.2),mass(0.5),enableCollisions(true),collidedTop(false),collidedBottom(false),collidedLeft(false),collidedRight(false),x(x),y(y),index(index)
{
    
}

void Entity::render(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glMatrixMode(GL_MODELVIEW);
    glTranslatef(x,y , 0.0);
    float u = (float)(((int)index) % spritecountx) / (float) spritecountx;
    float v = (float)(((int)index) / spritecountx) / (float) spritecounty;
    float spriteWidth = 1.0/(float)spritecountx;
    float spriteHeight = 1.0/(float)spritecounty;
    GLfloat quad[] = {-0.04, 0.04, -0.04, -0.04, 0.04, -0.04, 0.04, 0.04};
    glVertexPointer(2, GL_FLOAT, 0, quad);
    glEnableClientState(GL_VERTEX_ARRAY);
    GLfloat quadUVs[] = { u, v,
        u, v+spriteHeight,u+spriteWidth, v+spriteHeight,
        u+spriteWidth, v    };
    
    glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_QUADS, 0, 4);
    //glMatrixMode(GL_PROJECTION);
    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
}
void Entity::moving(){
    
    if (!collidedRight&&!collidedLeft&&!collidedTop&&!collidedBottom)
        
    {
        
        velocity_x = lerp(velocity_x, 0.0f, FIXED_TIMESTEP * friction_x);
        velocity_x += acceleration_x * FIXED_TIMESTEP;
        x += velocity_x * FIXED_TIMESTEP;
        velocity_y = lerp(velocity_y, 0.0f, FIXED_TIMESTEP * friction_y);
        velocity_y += acceleration_y * FIXED_TIMESTEP;
        y += velocity_y * FIXED_TIMESTEP;
        
        
        
    }
    //if its on the ground only update the x
    else if(!collidedRight&&!collidedLeft&&!collidedTop&&collidedBottom)
        
        
    {
        velocity_x = lerp(velocity_x, 0.0f, FIXED_TIMESTEP * friction_x);
        velocity_x += acceleration_x * FIXED_TIMESTEP;
        x += velocity_x * FIXED_TIMESTEP;
        velocity_y=0.0;
    }
    //if the player is not static move the camera with the same speed
    if(!isStatic()){
        camerax=-velocity_x*FIXED_TIMESTEP;
        cameray=-velocity_y*FIXED_TIMESTEP;
    }
    
    
}

void Entity::reset()
{
    collidedBottom=false;
    collidedTop=false;
    collidedLeft=false;
    collidedRight=false;
    
}
bool Entity::collided()
{
    return (collidedTop);
}
bool Entity::isStatic()
{   if(velocity_y==0.0&&velocity_x==0.0)
{return true;}
else{return false;}
}
