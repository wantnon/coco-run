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
#include "stateMachine.h"
CCAnimation* createAnimationWithStripeTex(CCTexture2D*stripeTex,string aniName);
CCAnimation* createAnimationWithStripeImage(string imageName);
class CstateMachine_player;
class Cplayer:public CmyObj
{
public:
    friend class CstateMachine_player;
public:
    Cplayer(b2World*world):CmyObj(world){
        m_fixture_body=NULL;
        m_fixture_foot=NULL;
        m_fixture_foot2=NULL;
        m_ani_runRight=NULL;
        m_ani_swimRight=NULL;
        m_tex_standRight=NULL;
        m_tex_jumpRight=NULL;
        m_isTouchSolid=false;
        m_water=NULL;
        m_stateMachine=NULL;
    }
    virtual~Cplayer(){
        if(m_water)m_water->release();
        if(m_stateMachine)m_stateMachine->release();
    }
public:
    bool init(CCSize size,CCPoint postion,b2World*world);
    CstateMachine_player*getStateMachine(){return (CstateMachine_player*)m_stateMachine;};
    //external ref
    void setWater(Cwater*water);
    //update  condition
    void updateIsTouchSolid();
    //additional effect
    void pressWater();
protected:
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
    b2Fixture*m_fixture_body;
    b2Fixture*m_fixture_foot;
    b2Fixture*m_fixture_foot2;
    bool m_isTouchSolid;
    Cwater*m_water;
    CstateMachine*m_stateMachine;
};

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
class CstateMachine_player:public CstateMachine
{
    
public:
    CstateMachine_player(){
        m_state=player_state_jump_still_right;
    }
    virtual void execute();
    static CstateMachine_player*create(){
        CstateMachine_player*p=new CstateMachine_player();
        p->autorelease();
        return p;
    }
    //----forced state switch
    void forcedTurnToRight();
    void forcedTurnToLeft();
    void forcedTurnToStill();
    void forcedTurnToJump();
protected:
    bool changeStateAndApearance(int newState);
    //----state combination
    bool is_jump_still()const ;
    bool is_jump_forward()const ;
    bool is_forward()const ;
    bool is_still()const ;
    bool is_swim_forward()const ;
    bool is_swim_still()const ;
    bool is_jump()const ;
    bool is_swim()const ;
    bool is_xx_still()const ;
    bool is_xx_forward()const ;
    bool isFacingRight()const ;
    //----print state
    void print_sate()const ;
    //----in state logic
    void run();
    void jump();
    void swim();
    //----condition triggered automatic state switch
    void turnToFallToGround();
    void turnToRunOutOfWater();
    void turnToFallToWater();
    void turnToFallCliff();
    void turnToJumpOutOfWater();
protected:
    int m_state;
};


#endif /* defined(__mybox2dtest__player__) */
