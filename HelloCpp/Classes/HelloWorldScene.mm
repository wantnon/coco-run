//
//  HelloWorldScene.cpp
//  mybox2dtest
//
//  Created by apple on 13-7-4.
//  Copyright __MyCompanyName__ 2013Äê. All rights reserved.
//
#include <vector>
#include <string>

#include "HelloWorldScene.h"
#include "AppMacros.h"

#include "SimpleAudioEngine.h"
#include "CCControlButton.h"
#include "myContactListener.h"
#include "myDef.h"
#include "player.h"
#include "myBox2dFunc.h"
#include "tinyxml2.h"
#include "myfunc.h"
#import "WXApiObject.h"
#import "WXApi.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace tinyxml2;
using namespace std;




CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...
    //set projection to 2D (default is 3D). if use 3D projection, may cause tiny projection error, in some situation, if the error accumulated across frames, may cause effect wrong.
    CCDirector::sharedDirector()->setProjection(kCCDirectorProjection2D);
    setTouchEnabled( true );
    setAccelerometerEnabled( false );
    CCDirector::sharedDirector()->setDisplayStats(true);

    //---------------------create roots
	//----rollingSceneRoot
	rollingSceneRoot=CCNode::create();
	addChild(rollingSceneRoot);
    //----nonRollingSceneRoot
    nonRollingSceneRoot= CCNode::create();
    addChild(nonRollingSceneRoot,-1);
    //----uiRoot
    uiRoot=CCNode::create();
    addChild(uiRoot);
    //----objsRoot
    objsRoot=CCNode::create();
    rollingSceneRoot->addChild(objsRoot,100);
    //---------------------init
    //----box2d world
    initBox2d();
    //----init UI
    initUI();
    //---------------------init things
	//----sky
    {
        CCSprite*skySprite=CCSprite::create();
        skySprite->initWithFile("data/global/tex/skyBackGround.png");
        skySprite->setAnchorPoint(ccp(0,0));
        nonRollingSceneRoot->addChild(skySprite,-2);
    }
    //----objs
    {
        vector<string> objNameList;
        objNameList.push_back("ball0");
       // objNameList.push_back("stone0");
       // objNameList.push_back("platform0");
       // objNameList.push_back("platform1");
      //  objNameList.push_back("groundBlock0");
      //  objNameList.push_back("slope0");
        int nObj=(int)objNameList.size();
        for(int i=0;i<nObj;i++){
            string objName=objNameList[i];
            CmyObj*obj=CmyObj::createObjWithXML(string("data/objs/")+objName+"/obj.xml", world);
            obj->name=objName;
            objsRoot->addChild(obj,100);
        }
        CCArray*children=objsRoot->getChildren();
        for(int i=0;i<children->count();i++){
            CmyObj*obj=(CmyObj*)children->objectAtIndex(i);
            if(obj->name=="ball0"){
                ball=obj;
                break;
            }
        }
        assert(ball);
    }
    //----water
    {
        water = new Cwater();
        water->autorelease();
        water->init(getCameraRect(rollingSceneRoot->getCamera(), winSize));
        rollingSceneRoot->addChild(water);
    }
    //----player
    {
        pPlayer = new Cplayer(world);
        pPlayer->autorelease();
        pPlayer->init(CCSize(128,128), CCPoint(300,600), world);
        rollingSceneRoot->addChild(pPlayer,100);
        pPlayer->name="player";
    }
    //----looping background
    {
        CloopingBackGround*loopingBackGround=new CloopingBackGround();
        loopingBackGround->autorelease();
        rollingSceneRoot->addChild(loopingBackGround,-1);
        const float margin=10;//the margin is to fix the follower lag than followee problem
        loopingBackGround->init("data/global/tex/backGround.png",winSize.width+margin);
        loopingBackGround->setFollowOffset_x(-winSize.width*playerPosPercentInWin.x-margin/2);
        loopingBackGround->setFollowee(pPlayer);
        loopingBackGround->scheduleUpdate();
        
        loopingBackGround->setPositionY(-100);//-40
    }
    //----box2dDebugDrawNode
    {
        Cbox2dDebugDrawNode*box2dDebugDrawNode=new Cbox2dDebugDrawNode();
        box2dDebugDrawNode->autorelease();
        box2dDebugDrawNode->init();
        rollingSceneRoot->addChild(box2dDebugDrawNode,1000);
        box2dDebugDrawNode->setWorld(world);
        box2dDebugDrawNode->setVisible(showb2BodyWireFrame);
    }

 
    //schedule
    this->schedule(schedule_selector(HelloWorld::addBallToScene),2.5);

    //start update
    scheduleUpdate();
    return true;
}
void HelloWorld::addBallToScene(float dt){
    
}
void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

HelloWorld::~HelloWorld()
{

    delete world;
    world = NULL;
    
}

void HelloWorld::weixinShare(CCObject *senderz, cocos2d::extension::CCControlEvent controlEvent){
    
    WXMediaMessage *message = [WXMediaMessage message];
    message.title = @"hellow COCO";
    message.description = @"let play hellow COCO!";
    [message setThumbImage:[UIImage imageNamed:@"iphone/weixin_share_image.png"]];
    
    WXWebpageObject *ext = [WXWebpageObject object];
    ext.webpageUrl = @"http://user.qzone.qq.com/350479720/blog/1375017261";
    
    message.mediaObject = ext;
    
    SendMessageToWXReq* req = [[[SendMessageToWXReq alloc] init]autorelease];
    req.bText = NO;
    req.message = message;
    req.scene = WXSceneSession;//_scene;
    
    [WXApi sendReq:req];
    
    
    
}


void HelloWorld::rightKeyDown(CCObject *senderz, cocos2d::extension::CCControlEvent controlEvent){
    pPlayer->turnToRight();
}

void HelloWorld::rightKeyUpInside(CCObject *senderz, cocos2d::extension::CCControlEvent controlEvent){
    pPlayer->turnToStill();
    
}
void HelloWorld::rightKeyDragExit(CCObject *senderz, cocos2d::extension::CCControlEvent controlEvent){
    pPlayer->turnToStill();

}
void HelloWorld::rightKeyDragEnter(CCObject *senderz, cocos2d::extension::CCControlEvent controlEvent){
    pPlayer->turnToRight();
}

void HelloWorld::leftKeyDown(CCObject *senderz, cocos2d::extension::CCControlEvent controlEvent){
    pPlayer->turnToLeft();

}

void HelloWorld::leftKeyUpInside(CCObject *senderz, cocos2d::extension::CCControlEvent controlEvent){
    pPlayer->turnToStill();
    
}
void HelloWorld::leftKeyDragExit(CCObject *senderz, cocos2d::extension::CCControlEvent controlEvent){
    pPlayer->turnToStill();

}
void HelloWorld::leftKeyDragEnter(CCObject *senderz, cocos2d::extension::CCControlEvent controlEvent){
    pPlayer->turnToLeft();
}


void HelloWorld::jumpKeyDown(CCObject *senderz, cocos2d::extension::CCControlEvent controlEvent){
    
    pPlayer->turnToJump();
  //  cout<<"player state:";pPlayer->print_sate();cout<<endl;


}


void HelloWorld::visit(void){
    
    CCLayer::visit();
    
}
void HelloWorld::draw(){
    CCLayer::draw();
}

void HelloWorld::update(float dt)
{

    
    //It is recommended that a fixed time step is used with Box2D for stability
    //of the simulation, however, we are using a variable time step here.
    //You need to make an informed choice, the following URL is useful
    //http://gafferongames.com/game-physics/fix-your-timestep/
    
    int velocityIterations = 8;
    int positionIterations = 1;
    
    // Instruct the world to perform a single step of simulation. It is
    // generally best to keep the time step and iterations fixed.
    world->Step(dt, velocityIterations, positionIterations);
    
    //Iterate over the bodies in the physics world
    //let body's sprite move with body
    for (b2Body* body = world->GetBodyList(); body; body = body->GetNext())
    {
		b2Vec2 bodyPos=body->GetPosition();//box2d world position
		CCPoint spritePos=ccp(bodyPos.x * PTM_RATIO, bodyPos.y * PTM_RATIO);
        if (body->GetUserData() != NULL) {
            //synchronize the sprite position and rotation with the corresponding body
            CmyObj* myObj = (CmyObj*)body->GetUserData();
            //set world pos
            setNodeWorldPosition(myObj,spritePos);
			//set ratation
            myObj->setRotation( -1 * CC_RADIANS_TO_DEGREES(body->GetAngle()) );
        }    
    }
    
    //----get winSize
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    //-----------player move
    //run
    pPlayer->run();
    //jump
    pPlayer->jump();
    //swim
    pPlayer->swim();

    //--------player store state
    pPlayer->storeState();
    
    
// this method is not acurate, use contact listener is better
 //--------player update isTouchSolid
    pPlayer->updateIsTouchSolid();
 //     cout<<" "<<pPlayer->isTouchSolid<<endl;
 
    //--------player update isInWater
    pPlayer->updateIsInWater(water);
    //--------player turn to...
    //fall to ground
    pPlayer->turnToFallToGround(water);
    //fall cliff
    pPlayer->turnToFallCliff(water);
    //fall to water
    pPlayer->turnToFallToWater(water);
    //jump out of water
    pPlayer->turnToJumpOutOfWater(water);
    //run out of water
    pPlayer->turnToRunOutOfWater(water);

    
    //print player state
    //cout<<"player state:";pPlayer->print_sate();cout<<" "<<pPlayer->isInWater<<" "<<pPlayer->isInWaterFoe;cout<<" "<<pPlayer->isTouchSolid<<endl;
    
    //----player press water
    pPlayer->pressWater(water);

    
    //----camera of rollingScene follow player
    cameraFollowTarget(rollingSceneRoot->getCamera(), pPlayer->convertToWorldSpaceAR(ccp(0,0)), playerPosPercentInWin, winSize);
    //---objs buoyancyEffect
    {
        CCArray*objs=objsRoot->getChildren();
        int nObj=objs->count();
        for(int i=0;i<nObj;i++){
            CmyObj*obj=(CmyObj*)objs->objectAtIndex(i);
            buoyancyEffect(obj->pb2Body,0,water->getSurfaceHeight(),20,world,dt);
        }
    }
    //----player buoyancyEffect
	buoyancyEffect(pPlayer->pb2Body,0,water->getSurfaceHeight(),20,world,dt);
    //----update terrain
    infiniteTerrainManager.updateTerrain(getCameraRect(rollingSceneRoot->getCamera(), winSize), rollingSceneRoot, world, -150);
    //----update water
    water->updateCameraRect(getCameraRect(rollingSceneRoot->getCamera(), winSize));
    
    //----if ball fall behind of the cameraRect, then place it to the right side of cameraRect
    {
        CCRect cameraRect=getCameraRect(rollingSceneRoot->getCamera(),winSize);
        b2Vec2 ballPos_b2=ball->pb2Body->GetPosition();//http://user.qzone.qq.com/350479720/blog/1375257108
        b2CircleShape *circleShape=(b2CircleShape*)ball->pb2Body->GetFixtureList()->GetShape();
        float32 r_b2=circleShape->m_radius;
        CCPoint ballPos=CCPoint(ballPos_b2.x*PTM_RATIO,ballPos_b2.y*PTM_RATIO);
        float r=r_b2*PTM_RATIO;
        if(ballPos.x+r<cameraRect.getMinX()){//ball fall behind of the cameraRect
            CCPoint newBallPos=CCPoint(cameraRect.getMaxX(),cameraRect.getMaxY());
            b2Vec2 newBallPos_b2=b2Vec2(newBallPos.x/PTM_RATIO,newBallPos.y/PTM_RATIO);
            ball->pb2Body->SetTransform(newBallPos_b2, 0 );
            ball->changeVelocityXbyImpuse(randomInRange(-10, -2));
        }
    }
    //----ball press water
    {
        b2Vec2 ballPos_b2=ball->pb2Body->GetPosition();//http://user.qzone.qq.com/350479720/blog/1375257108
        b2CircleShape *circleShape=(b2CircleShape*)ball->pb2Body->GetFixtureList()->GetShape();
        float32 r_b2=circleShape->m_radius;
        CCPoint ballPos=CCPoint(ballPos_b2.x*PTM_RATIO,ballPos_b2.y*PTM_RATIO);
        float r=r_b2*PTM_RATIO;
        bool ballIsInWater=(ballPos.y-r<water->getSurfaceHeight());
        ball->pressWater(water, ballIsInWater,8,15);
        
    
    }
      
    
    
    //b2d body count
    //cout<<"bodyCount:"<<world->GetBodyCount()<<endl;
}

void HelloWorld::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    CCSetIterator it;
    CCTouch* touch;
    
    for( it = touches->begin(); it != touches->end(); it++)
    {
        touch = (CCTouch*)(*it);
        
        if(!touch)
            break;
        
        CCPoint location = touch->getLocationInView();
        
        CCPoint locationGL = CCDirector::sharedDirector()->convertToGL(location);
        
        

        
    }
}
void HelloWorld::ccTouchesMoved(cocos2d::CCSet* touches , cocos2d::CCEvent* event)
{
    CCSetIterator it;
    CCTouch* touch;
    
    for( it = touches->begin(); it != touches->end(); it++)
    {
        touch = (CCTouch*)(*it);
        
        if(!touch)
            break;
        
        CCPoint location = touch->getLocationInView();
        
        CCPoint locationGL = CCDirector::sharedDirector()->convertToGL(location);
        
    }

    
   
}

void HelloWorld::ccTouchesBegan(CCSet* touches, CCEvent* event)
{
    CCSetIterator it;
    CCTouch* touch;
    
    for( it = touches->begin(); it != touches->end(); it++)
    {
        touch = (CCTouch*)(*it);
        
        if(!touch)
            break;
        
        CCPoint location = touch->getLocationInView();
        
        CCPoint locationGL = CCDirector::sharedDirector()->convertToGL(location);
        
    }

}

void HelloWorld::initBox2d(){
    b2Vec2 gravity;
    gravity.Set(0.0f, -10.0f);
    world = new b2World(gravity);
    world->SetAllowSleeping(true); // Do we want to let bodies sleep?
    world->SetContinuousPhysics(true);
    world->SetContactListener(&myContactListener);

}
void HelloWorld::initUI(){
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();
    //--label
    CCLabelTTF *label = CCLabelTTF::create("Hellow COCO -- demo 2.0 -- by wantnon (2013-10-6)", "Helvetica", 32);
    addChild(label, 0);
    label->setColor(ccc3(0,0,255));
    label->setPosition(ccp( winSize.width/2, winSize.height-50));
    //--left
    {
        cocos2d::extension::CCScale9Sprite* btnUp=cocos2d::extension::CCScale9Sprite::create("data/global/tex/button.png");
        cocos2d::extension::CCScale9Sprite* btnDn=cocos2d::extension::CCScale9Sprite::create("data/global/tex/button_dn.png");
        CCLabelTTF*title=CCLabelTTF::create("<--", "Helvetica", 30);
        cocos2d::extension::CCControlButton* controlButton=cocos2d::extension::CCControlButton::create(title, btnUp);
        controlButton->setBackgroundSpriteForState(btnDn,cocos2d::extension::CCControlStateHighlighted);
        controlButton->setPreferredSize(CCSize(100,100));
        controlButton->setPosition(ccp(120+100,60));
        controlButton->addTargetWithActionForControlEvents(this, (cocos2d::extension::SEL_CCControlHandler)(&HelloWorld::leftKeyDown), cocos2d::extension::CCControlEventTouchDown);
        controlButton->addTargetWithActionForControlEvents(this, (cocos2d::extension::SEL_CCControlHandler)(&HelloWorld::leftKeyUpInside), cocos2d::extension::CCControlEventTouchUpInside);
        controlButton->addTargetWithActionForControlEvents(this, (cocos2d::extension::SEL_CCControlHandler)(&HelloWorld::leftKeyDragEnter), cocos2d::extension::CCControlEventTouchDragEnter);
        controlButton->addTargetWithActionForControlEvents(this, (cocos2d::extension::SEL_CCControlHandler)(&HelloWorld::leftKeyDragExit), cocos2d::extension::CCControlEventTouchDragExit);
        uiRoot->addChild(controlButton);
        m_controlButton_leftKey=controlButton;
    }
    //--right
    {
        cocos2d::extension::CCScale9Sprite* btnUp=cocos2d::extension::CCScale9Sprite::create("data/global/tex/button.png");
        cocos2d::extension::CCScale9Sprite* btnDn=cocos2d::extension::CCScale9Sprite::create("data/global/tex/button_dn.png");
        CCLabelTTF*title=CCLabelTTF::create("-->", "Helvetica", 30);
        cocos2d::extension::CCControlButton* controlButton=cocos2d::extension::CCControlButton::create(title, btnUp);
        controlButton->setBackgroundSpriteForState(btnDn,cocos2d::extension::CCControlStateHighlighted);
        controlButton->setPreferredSize(CCSize(100,100));
        controlButton->setPosition(ccp(300+100,60));
        controlButton->addTargetWithActionForControlEvents(this, (cocos2d::extension::SEL_CCControlHandler)(&HelloWorld::rightKeyDown), cocos2d::extension::CCControlEventTouchDown);
        controlButton->addTargetWithActionForControlEvents(this, (cocos2d::extension::SEL_CCControlHandler)(&HelloWorld::rightKeyUpInside), cocos2d::extension::CCControlEventTouchUpInside);
        controlButton->addTargetWithActionForControlEvents(this, (cocos2d::extension::SEL_CCControlHandler)(&HelloWorld::rightKeyDragEnter), cocos2d::extension::CCControlEventTouchDragEnter);
        controlButton->addTargetWithActionForControlEvents(this, (cocos2d::extension::SEL_CCControlHandler)(&HelloWorld::rightKeyDragExit), cocos2d::extension::CCControlEventTouchDragExit);
        uiRoot->addChild(controlButton);
        m_controlButton_rightKey=controlButton;
    }
    //--jump
    {
        cocos2d::extension::CCScale9Sprite* btnUp=cocos2d::extension::CCScale9Sprite::create("data/global/tex/button.png");
        cocos2d::extension::CCScale9Sprite* btnDn=cocos2d::extension::CCScale9Sprite::create("data/global/tex/button_dn.png");
        CCLabelTTF*title=CCLabelTTF::create("jump", "Helvetica", 30);
        cocos2d::extension::CCControlButton* controlButton=cocos2d::extension::CCControlButton::create(title, btnUp);
        controlButton->setBackgroundSpriteForState(btnDn,cocos2d::extension::CCControlStateHighlighted);
        controlButton->setPreferredSize(CCSize(100,100));
        controlButton->setPosition(ccp(850,60));
        controlButton->addTargetWithActionForControlEvents(this, (cocos2d::extension::SEL_CCControlHandler)(&HelloWorld::jumpKeyDown), cocos2d::extension::CCControlEventTouchDown);
        
        uiRoot->addChild(controlButton);
        m_controlButton_jumpKey=controlButton;
    }
    //--weixin share
    {
        cocos2d::extension::CCScale9Sprite* btnUp=cocos2d::extension::CCScale9Sprite::create("data/global/tex/button.png");
        cocos2d::extension::CCScale9Sprite* btnDn=cocos2d::extension::CCScale9Sprite::create("data/global/tex/button_dn.png");
        CCLabelTTF*title=CCLabelTTF::create("share", "Helvetica", 30);
        cocos2d::extension::CCControlButton* controlButton=cocos2d::extension::CCControlButton::create(title, btnUp);
        controlButton->setBackgroundSpriteForState(btnDn,cocos2d::extension::CCControlStateHighlighted);
        controlButton->setPreferredSize(CCSize(100,100));
        controlButton->setPosition(ccp(200,500));
        controlButton->addTargetWithActionForControlEvents(this, (cocos2d::extension::SEL_CCControlHandler)(&HelloWorld::weixinShare), cocos2d::extension::CCControlEventTouchDown);
        
        uiRoot->addChild(controlButton);
        m_controlButton_weixinShare=controlButton;
    }
    
    
}

