//
//  Entity.cpp
//  NYUCodebase
//
//  Created by Alvong on 3/2/15.
//  Copyright (c) 2015 Ivan Safrin. All rights reserved.
//
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>
#include "Entity.h"
#define FIXED_TIMESTEP 0.0166666f
Entity::Entity(unsigned int texture,float u, float v, float width, float height,float x,float y):texture(texture),u(u),v(v),width(width),height(height),velocity_x(0.0),velocity_y(0.0),acceleration_x(0.0),acceleration_y(-1.8),friction_x(-0.3),friction_y(0.0),mass(0.5),isStatic(true),enableCollisions(true),collidedTop(false),collidedBottom(false),collidedLeft(false),collidedRight(false),x(x),y(y)
{
    
}
float Entity::lerp(float v0, float v1, float t) {
    return (1.0-t)*v0 + t*v1;
}

//void Entity::Update(float elapsed){}

//void Entity::Render(){}

void Entity::collidesWith(Entity *entity)
{
    float xpen=0.0;
    float ypen=0.0;
    //collide bot
    if ((y>entity->y)&&(y-entity->y)<(height/2+entity->height/2)&&
        x<(entity->x+entity->width/2)&&
        x>(entity->x-entity->width/2)
        )
    {
        velocity_y=0.0;
        isStatic=true;
        ypen=fabs((entity->y+entity->height/2)-(y-height/2));
        //y+=ypen;

    }
    //collide top
    else if ((y<entity->y)&&(entity->y-y)<(height/2+entity->height/2)&&
        x<(entity->x+entity->width/2)&&
        x>(entity->x-entity->width/2)
        )
    {
        velocity_y=0.0;
        ypen=-fabs((y+height/2)-(entity->y-entity->height/2));
        //y+=ypen;

    }
    //collide left
    if ((x>entity->x)&&(x-entity->x)<(width/2+entity->width/2)&&
        y<(entity->y+entity->height/2)&&
        y>(entity->y-entity->height/2)
        ) {
        velocity_x=0.0;
        xpen=fabs((entity->x+entity->width/2)-(x-width/2));
        //x+=xpen;
    }
    //collide right
    else if ((x<entity->x)&&(entity->x-x)<(width/2+entity->width/2)&&
        y<(entity->y+entity->height/2)&&
        y>(entity->y-entity->height/2)
        )
    {
        velocity_x=0.0;
        xpen=-fabs((x+width/2)-(entity->x-entity->width/2));
        //x+=xpen;
    }
    //if player tries to go out of screen
    
    y+=ypen;
    x+=xpen;
    if (x>0.9) {
        x=0.9;
    }
   else if (x<-.9) {
        x=-.9;
    }

}
//when the snake collides withe player
bool Entity::collided(Entity* entity)
{
    if (fabs((y-entity->y))<(height/2+entity->height/2)&&fabs((x-entity->x))<(width/2+entity->width/2)) {

        return true;
    }
    else{return false;}
}

void Entity::moving()
{
    if (isStatic==false)
    {
    
    velocity_x = Entity::lerp(velocity_x, 0.0f, FIXED_TIMESTEP * friction_x);
    velocity_x += acceleration_x * FIXED_TIMESTEP;
    x += velocity_x * FIXED_TIMESTEP;

    velocity_y += acceleration_y * FIXED_TIMESTEP;
    y += velocity_y * FIXED_TIMESTEP;
    }
    
}
void Entity::setTexture(float up, float vp, float w, float h)
{
    u=up;
    v=vp;
    width=w;
    height=h;
    
}

void Entity::draw(float scale)
{   
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glViewport(0, 0, 800, 600);
    glOrtho(-1, 1, -1.0, 1.0,
            -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(x, y, 0.0);
    //glRotatef(rotation, 0.0, 0.0, 1.0);
    GLfloat quad[] = {-width * scale , height * scale, -width * scale, -height * scale,width * scale, -height * scale, width * scale, height * scale};
    glVertexPointer(2, GL_FLOAT, 0, quad);
    glEnableClientState(GL_VERTEX_ARRAY);
    GLfloat quadUVs[] = {u, v, u, v+height, u+width, v+height, u+width, v};
    glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glDisable(GL_BLEND);
    glDrawArrays(GL_QUADS, 0, 4);
    glMatrixMode(GL_PROJECTION);
    glDisable(GL_TEXTURE_2D);
    //glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
}