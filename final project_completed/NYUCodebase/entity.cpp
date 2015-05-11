
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>
#include <stdlib.h>     
#include "entity.h"
#define FIXED_TIMESTEP 0.0166666f

float lerp(float v0, float v1, float t) {
    return (1.0-t)*v0 + t*v1;
}
Entity::Entity(unsigned int texture,int index, float x,float y):texture(texture),width(0.04),height(0.04),velocity_x(0.0),velocity_y(0.0),acceleration_x(0.0),acceleration_y(-1.8),friction_x(10.0),friction_y(0.0),mass(0.5),dead(false),collidedTop(false),collidedBottom(false),collidedLeft(false),collidedRight(false),x(x),y(y),index(index),shown(true),spritecountx(30),spritecounty(30),slowed(false),slowtime(0),slowtimeO(0.0)
{
    
}

void Entity::render(){
    if (shown) {
  
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glMatrixMode(GL_MODELVIEW);
    glTranslatef(x,y , 0.0);
    float u = (float)(((int)index) % spritecountx) / (float) spritecountx;
    float v = (float)(((int)index) / spritecountx) / (float) spritecounty;
    float spriteWidth = 1.0/(float)spritecountx;
    float spriteHeight = 1.0/(float)spritecounty;
    GLfloat quad[] = {-width  , height , -width, -height,width , -height , width , height};
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
                    
    
}
void Entity::stop(){
    velocity_x=0;
    velocity_y=0;
}
void Entity::reset()
{
    collidedBottom=false;
    collidedTop=false;
    collidedLeft=false;
    collidedRight=false;
    
  

    
    
}

void Entity::headingTo(Entity* one)
{
    if (x<one->x)
{
    velocity_x=0.2;
    
}
else if (x>one->x) {
    velocity_x=-0.2;
}
else {
    velocity_x=0;
}
    if (y<one->y)
    {
        velocity_y=0.2;
        
    }
    else if (y>one->y) {
        velocity_y=-0.2;
    }
    else {
        velocity_y=0;
    }

    
    
    velocity_x = lerp(velocity_x, 0.0f, FIXED_TIMESTEP * friction_x);
    velocity_x += acceleration_x * FIXED_TIMESTEP;
    x += velocity_x * FIXED_TIMESTEP;
    velocity_y = lerp(velocity_y, 0.0f, FIXED_TIMESTEP * friction_y);
    velocity_y += acceleration_y * FIXED_TIMESTEP;
    y += velocity_y * FIXED_TIMESTEP;
    
}
bool Entity::collidewith(Entity *entity)
{
    if (fabs((y-entity->y))<(height/2+entity->height/2)&&fabs((x-entity->x))<(width/2+entity->width/2)) {
        
        return true;
    }
    else
    {return false;}
}
bool Entity::collided()
{
    return (collidedTop);
}
//bool Entity::isStatic()
//{   if(velocity_y==0.0&&velocity_x==0.0)
//{return true;}
//else{return false;}
//}
