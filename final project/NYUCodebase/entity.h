//
//  entity.h
//  NYUCodebase
//
//  Created by Alvong on 4/30/15.
//  Copyright (c) 2015 Ivan Safrin. All rights reserved.
//
#ifndef entity_h
#define entity_h

#include <stdio.h>

class Entity{
public:
    Entity(unsigned int texture,int index, float x,float y);
    void render();
    void moving();
    void reset();
    bool collided();
    bool isStatic();
    
    
    

unsigned int texture;
int spritecountx;
int spritecounty;
float x;
float y;
int index;
float camerax;
float cameray;
float width;
float height;
float velocity_x;
float velocity_y;
float acceleration_x;
float acceleration_y;
float friction_x;
float friction_y;
float mass;
bool enableCollisions;
bool collidedTop;
bool collidedBottom;
bool collidedLeft;
bool collidedRight;
};
#endif /* defined(__NYUCodebase__entity__) */
