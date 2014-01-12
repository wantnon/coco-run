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
        m_isTouchSolid=false;
        m_water=NULL;
    }
    virtual~Cplayer(){
        if(m_water)m_water->release();
    }
public:
    bool init(CCSize size,CCPoint postion,b2World*world);
    //----external ref
    void setWater(Cwater*water);
    //----update  condition
    void updateIsTouchSolid();
    //----print state
    void print_sate()const ;
    //----in state logic
    void run();
    void jump();
    void swim();
    //----switch state logic
    //forced
    void forcedTurnToRight();
    void forcedTurnToLeft();
    void forcedTurnToStill();
    void forcedTurnToJump();
    //automatic
    void turnToFallToGround();
    void turnToRunOutOfWater();
    void turnToFallToWater();
    void turnToFallCliff();
    void turnToJumpOutOfWater();
    //----additional effect
    void pressWater();
protected:
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
    void runAction_runRight();
    void runAction_swimRight();
    void setPosture_jumpRight();
    void setPosture_standRight();
    void runAction_runLeft();
    void runAction_swimLeft();
    void setPosture_jumpLeft();
    void setPosture_standLeft();
    bool changeStateAndApearance(int newState);
    void setApearanceByState(int _state);
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
    bool m_isTouchSolid;
    Cwater*m_water;

};
#endif /* defined(__mybox2dtest__player__) */
