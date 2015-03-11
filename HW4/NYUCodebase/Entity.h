//
//  Entity.h
//  NYUCodebase
//
//  Created by Alvong on 3/2/15.
//  Copyright (c) 2015 Ivan Safrin. All rights reserved.
//

#ifndef Entity_h
#define Entity_h
class Entity {
public:
    
    Entity(unsigned int texture,float u, float v, float width, float height,float x,float y);
    //void Update(float elapsed);
    //void Render();
    void collidesWith(Entity *entity);
    //void FixedUpdate(int direction);
    void draw(float scale=0.5);
//    void setPhysics(float mass, float accy, float accx, float fricy, float fricx);//sets friction, mass etc...
    void setTexture(float u, float v, float w, float h);
    void moving();
    float lerp(float v0, float v1, float t);
    bool collided(Entity* enemy);
    unsigned int texture;
    float x;
    float y;
    float u;
    float v;
    float width;
    float height;
    float velocity_x;
    float velocity_y;
    float acceleration_x;
    float acceleration_y;
    float friction_x;
    float friction_y;
    float mass;
    bool isStatic;
    bool enableCollisions;
    bool collidedTop;
    bool collidedBottom;
    bool collidedLeft;
    bool collidedRight;
};


#endif
