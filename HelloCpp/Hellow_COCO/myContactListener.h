//
//  myContactListener.h
//  mybox2dtest
//
//  Created by apple on 13-7-19.
//
//

#ifndef __mybox2dtest__myContactListener__
#define __mybox2dtest__myContactListener__

#include <iostream>
using namespace std;
#include "Box2D.h"
#include "myObj.h"
#include "player.h"
#include "myDef.h"
#include "shotBall.h"
#include "player.h"

class CmyContactListener:public b2ContactListener
{
public:
    void BeginContact(b2Contact* contact){
     //   CCLOG("contact begin");
     
        b2Fixture*fixtureA=contact->GetFixtureA();
        b2Fixture*fixtureB=contact->GetFixtureB();
        b2Body*bodyA=fixtureA->GetBody();
        b2Body*bodyB=fixtureB->GetBody();
        CmyObj*pObjA=(CmyObj*)bodyA->GetUserData();
        CmyObj*pObjB=(CmyObj*)bodyB->GetUserData();
              
    }
    void EndContact(b2Contact* contact){
      //  CCLOG("contact end");
        b2Fixture*fixtureA=contact->GetFixtureA();
        b2Fixture*fixtureB=contact->GetFixtureB();
        b2Body*bodyA=fixtureA->GetBody();
        b2Body*bodyB=fixtureB->GetBody();
        CmyObj*pObjA=(CmyObj*)bodyA->GetUserData();
        CmyObj*pObjB=(CmyObj*)bodyB->GetUserData();
    }

};
#endif /* defined(__mybox2dtest__myContactListener__) */
