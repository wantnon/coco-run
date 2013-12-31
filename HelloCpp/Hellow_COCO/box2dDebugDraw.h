//
//  box2dDebugDraw.h
//  HelloCpp
//
//  Created by ck02-084 on 13-12-16.
//
//

#ifndef __HelloCpp__box2dDebugDraw__
#define __HelloCpp__box2dDebugDraw__

#include <iostream>
#include <vector>
#include <string>
using namespace std;
#include "Box2D.h"
#include "cocos2d.h"
#include "myDef.h"
#include "myfunc.h"
using namespace cocos2d;
class Cbox2dDebugDrawNode:public CCNode
{
public:
    Cbox2dDebugDrawNode(){
        world=NULL;
    }
    void setWorld(b2World*_world){world=_world;};
    virtual void draw(){
        //Iterate over the bodies in the physics world
        for (b2Body* body = world->GetBodyList(); body; body = body->GetNext())
        {
            b2Vec2 pos=body->GetPosition();//world position
            b2Fixture* fixture=body->GetFixtureList();
            for(;fixture;fixture=fixture->GetNext()){
                b2Shape*shape=fixture->GetShape();
                if(shape->GetType()==b2Shape::e_polygon){
                    b2PolygonShape*polyShape=(b2PolygonShape*)shape;
                    int nv=polyShape->GetVertexCount();
                    const b2Vec2* verts_local=polyShape->m_vertices;
                    float rolleAngle=body->GetAngle();//radian
                    vector<CCPoint> vlist;//cocos2d-x world vertexes
                    for(int i=0;i<nv;i++){
                        CCPoint vertex;
                        {
                            vertex=CCPoint(verts_local[i].x*PTM_RATIO,verts_local[i].y*PTM_RATIO);
                            vertex=rotateVec(vertex,CC_RADIANS_TO_DEGREES(rolleAngle));
                            vertex=vertex+CCPoint(pos.x*PTM_RATIO,pos.y*PTM_RATIO);
                        }
                        vlist.push_back(ccp(vertex.x,vertex.y));
                    }
                    ccDrawPoly(&vlist[0], nv, true);
                }else if(shape->GetType()==b2Shape::e_circle){
                    b2CircleShape*circleShape=(b2CircleShape*)shape;
                    b2Vec2 c_local=circleShape->m_p;
                    b2Vec2 c_world=c_local+pos;
                    float r=circleShape->m_radius;
                    float rolleAngle=body->GetAngle();//radian
                    ccDrawCircle(ccp(c_world.x*PTM_RATIO,c_world.y*PTM_RATIO), r*PTM_RATIO, rolleAngle, 10, true, 1, 1);
                }
            }
        }

        
    }
private:
    b2World*world;

};

#endif /* defined(__HelloCpp__box2dDebugDraw__) */
