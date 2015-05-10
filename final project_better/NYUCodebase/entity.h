
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
    bool collidewith(Entity* one);
    void headingTo(Entity* one);
    void stop();
    
    //bool isStatic();
    
    
    

unsigned int texture;
bool slowed;
int slowtime;
float slowtimeO;
bool shown;
int spritecountx;
int spritecounty;
float x;
float y;
int index;
float width;
float height;
float velocity_x;
float velocity_y;
float acceleration_x;
float acceleration_y;
float friction_x;
float friction_y;
float mass;
bool dead;
bool collidedTop;
bool collidedBottom;
bool collidedLeft;
bool collidedRight;
    
};
#endif /* defined(__NYUCodebase__entity__) */
