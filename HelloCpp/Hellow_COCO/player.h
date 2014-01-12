//
//  player.h
//  mybox2dtest
//
//  Created by apple on 13-7-21.
//
//

#ifndef __mybox2dtest__player__
#define __mybox2dtest__player__

#include <iostream>
#include "myObj.h"
using namespace std;
#include "cocos2d.h"
#include "Box2D.h"
using namespace cocos2d;
#include "water.h"
CCAnimation* createAnimationWithStripeTex(CCTexture2D*stripeTex,string aniName);
CCAnimation* createAnimationWithStripeImage(string imageName);
#define player_state_still_right 0
#define player_state_still_left 1
#define player_state_forward_right 2
#define player_state_forward_left 3
#define player_state_jump_still_right 4
#define player_state_jump_still_left 5
#define player_state_jump_forward_right 6
#define player_state_jump_forward_left 7
#define player_state_swim_still_right 8
#define player_state_swim_still_left 9
#define player_state_swim_forward_right 10
#define player_state_swim_forward_left 11

class Cplayer:public CmyObj
{
public:
    Cplayer(b2World*world):CmyObj(world){
        m_fixture_body=NULL;
        m_fixture_foot=NULL;
        m_fixture_foot2=NULL;
        m_state=player_state_jump_still_right;
        m_ani_runRight=NULL;
        m_ani_swimRight=NULL;
        m_tex_standRight=NULL;
        m_tex_jumpRight=NULL;
        m_isInWater=false;
        m_isTouchSolid=false;
    }
public:
    void print_sate()const ;
public://init
    bool init(CCSize size,CCPoint postion,b2World*world);
public:
    void updateIsInWater(Cwater*water){
        m_isInWater=(this->boundingBox().getMinY()<water->getSurfaceHeight());
    }
    void updateIsTouchSolid();
public://move
    void run();
    void jump();
    void swim();
public://turn to...
    void turnToRight();
    void turnToLeft();
    void turnToStill();
    void turnToJump();
    void turnToFallToGround(Cwater*water);
    void turnToRunOutOfWater(Cwater*water);
    void turnToFallToWater(Cwater*water);
    void turnToFallCliff(Cwater*water);
    void turnToJumpOutOfWater(Cwater*water);
public://effect
    void pressWater(Cwater*water){
        CmyObj::pressWater(water, m_isInWater,15,30);
    }
    
protected://state judgement
    bool is_jump_still(int _state)const ;
    bool is_jump_forward(int _state)const ;
    bool is_forward(int _state)const ;
    bool is_still(int _state)const ;
    bool is_swim_forward(int _state)const ;
    bool is_swim_still(int _state)const ;
    bool is_jump(int _state)const ;
    bool is_swim(int _state)const ;
    bool is_xx_still(int _state)const ;
    bool is_xx_forward(int _state)const ;
    bool isFacingRight(int _state)const ;
protected://set posture or action
    void runAction_runRight();
    void runAction_swimRight();
    void setPosture_jumpRight();
    void setPosture_standRight();
    void runAction_runLeft();
    void runAction_swimLeft();
    void setPosture_jumpLeft();
    void setPosture_standLeft();
protected:
    bool changeStateAndApearance(int newState);
    void setApearanceByState(int _state);
protected:
    void createTexAndAni();
    void createB2Body(b2World* world,float rx,float ry,const b2Filter*fliter=NULL);
protected:
    CCAnimation*m_ani_runRight;
    CCAnimation*m_ani_swimRight;
    CCTexture2D*m_tex_standRight;
    CCTexture2D*m_tex_jumpRight;
    int m_state;
    b2Fixture*m_fixture_body;
    b2Fixture*m_fixture_foot;
    b2Fixture*m_fixture_foot2;
    bool m_isInWater;
    bool m_isTouchSolid;

};
#endif /* defined(__mybox2dtest__player__) */
