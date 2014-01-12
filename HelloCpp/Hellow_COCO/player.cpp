//
//  player.cpp
//  mybox2dtest
//
//  Created by apple on 13-7-21.
//
//

#include "player.h"
//----stateMachine_player
void CstateMachine_player::execute(){
    //Cplayer*player=(Cplayer*)this->getObj();
    //----in state
    run();
    jump();
    swim();
    //----state switch
    turnToFallToGround();
    turnToFallCliff();
    turnToFallToWater();
    turnToJumpOutOfWater();
    turnToRunOutOfWater();

}

bool CstateMachine_player::is_jump_still()const {
    return (m_state==player_state_jump_still_right||m_state==player_state_jump_still_left);
}
bool CstateMachine_player::is_jump_forward()const {
    return (m_state==player_state_jump_forward_right||m_state==player_state_jump_forward_left);
}
bool CstateMachine_player::is_forward()const {
    return (m_state==player_state_forward_left||m_state==player_state_forward_right);
}
bool CstateMachine_player::is_still()const {
    return (m_state==player_state_still_left||m_state==player_state_still_right);
}
bool CstateMachine_player::is_swim_forward()const {
    return (m_state==player_state_swim_forward_right||m_state==player_state_swim_forward_left);
}
bool CstateMachine_player::is_swim_still()const {
    return (m_state==player_state_swim_still_right||m_state==player_state_swim_still_left);
}
bool CstateMachine_player::is_jump()const {
    return (is_jump_forward()||is_jump_still());
}
bool CstateMachine_player::is_swim()const {
    return (is_swim_forward()||is_swim_still());
}
bool CstateMachine_player::is_xx_still()const {
    return (is_still()||is_jump_still()||is_swim_still());
}
bool CstateMachine_player::is_xx_forward()const {
    return (is_forward()||is_jump_forward()||is_swim_forward());
}
bool CstateMachine_player::isFacingRight()const {
    switch (m_state) {
        case player_state_still_right:
        case player_state_forward_right:
        case player_state_jump_still_right:
        case player_state_jump_forward_right:
        case player_state_swim_still_right:
        case player_state_swim_forward_right:
            return true;
            break;
        case player_state_still_left:
        case player_state_forward_left:
        case player_state_jump_still_left:
        case player_state_jump_forward_left:
        case player_state_swim_still_left:
        case player_state_swim_forward_left:
            return false;
            break;
        default:
            assert(false);
            break;
    }
}

void CstateMachine_player::print_sate()const {
    switch (m_state) {
        case player_state_still_right:
            cout<<"still_right";
            break;
        case player_state_still_left:
            cout<<"still_left";
            break;
        case player_state_forward_right:
            cout<<"run_forward_right";
            break;
        case player_state_forward_left:
            cout<<"run_forward_left";
            break;
        case player_state_jump_still_right:
            cout<<"jump_still_right";
            break;
        case player_state_jump_still_left:
            cout<<"jump_still_left";
            break;
        case player_state_jump_forward_right:
            cout<<"jump_foward_right";
            break;
        case player_state_jump_forward_left:
            cout<<"jump_forward_left";
            break;
        case player_state_swim_still_right:
            cout<<"swim_still_right";
            break;
        case player_state_swim_still_left:
            cout<<"swim_still_left";
            break;
        case player_state_swim_forward_right:
            cout<<"swim_forward_right";
            break;
        case player_state_swim_forward_left:
            cout<<"swim_forward_left";
            break;
        default:
            assert(false);
            break;
    }
}
bool CstateMachine_player::changeStateAndApearance(int newState)
//if state and apearance really changed, return true, else return false
{
    if(m_state!=newState){//if state is no difference with old state, do nothing and return false
        m_state=newState;
        ((Cplayer*)m_obj)->stopAllActions();
        ((Cplayer*)m_obj)->setApearanceByState(m_state);
        return true;
    }else{
        return false;
    }
}


void CstateMachine_player::run(){
    if(this->m_state==player_state_forward_right){
        m_obj->changeVelocityXbyImpuse(5);
    }
    if(this->m_state==player_state_forward_left){
        m_obj->changeVelocityXbyImpuse(-5);
    }
}
void CstateMachine_player::jump(){
    if(this->m_state==player_state_jump_forward_right)
    {
        m_obj->changeVelocityXbyImpuse(5);
    }
    if(this->m_state==player_state_jump_forward_left)
    {
        m_obj->changeVelocityXbyImpuse(-5);
    }
}
void CstateMachine_player::swim(){
    if(this->m_state==player_state_swim_forward_right)
    {
        m_obj->changeVelocityXbyImpuse(5);
    }
    if(this->m_state==player_state_swim_forward_left)
    {
        m_obj->changeVelocityXbyImpuse(-5);
    }
}



void CstateMachine_player::forcedTurnToRight(){
    if(this->is_jump()){
        this->changeStateAndApearance(player_state_jump_forward_right);
    }else if(this->is_forward()||this->is_still()){
        this->changeStateAndApearance(player_state_forward_right);
    }else if(this->is_swim()){
        this->changeStateAndApearance(player_state_swim_forward_right);
    }
    
}
void CstateMachine_player::forcedTurnToLeft(){
    if(this->is_jump()){
        this->changeStateAndApearance(player_state_jump_forward_left);
    }else if(this->is_forward()||this->is_still()){
        this->changeStateAndApearance(player_state_forward_left);
    }else if(this->is_swim()){
        this->changeStateAndApearance(player_state_swim_forward_left);
    }
}
void CstateMachine_player::forcedTurnToStill(){
    //stop move
    b2Vec2 v=m_obj->pb2Body->GetLinearVelocity();
    m_obj->pb2Body->SetLinearVelocity(b2Vec2(0,v.y));
    
    if(this->isFacingRight()){
        if(this->is_jump()){
            this->changeStateAndApearance(player_state_jump_still_right);
        }else if(this->is_forward()||this->is_still()){
            this->changeStateAndApearance(player_state_still_right);
        }else if(this->is_swim()){
            this->changeStateAndApearance(player_state_swim_still_right);
        }
    }else{//facing left
        if(this->is_jump()){
            this->changeStateAndApearance(player_state_jump_still_left);
        }else if(this->is_forward()||this->is_still()){
            this->changeStateAndApearance(player_state_still_left);
        }else if(this->is_swim()){
            this->changeStateAndApearance(player_state_swim_still_left);
        }
    }
    
}

void CstateMachine_player::forcedTurnToJump(){
    if(this->is_jump()==false){
        if(this->is_forward()){
            if(this->isFacingRight()){
                this->changeStateAndApearance(player_state_jump_forward_right);
            }else{
                this->changeStateAndApearance(player_state_jump_forward_left);
            }
        }else if(this->is_still()){
            if(this->isFacingRight()){
                this->changeStateAndApearance(player_state_jump_still_right);
            }else{
                this->changeStateAndApearance(player_state_jump_still_left);
            }
        }
        m_obj->changeVelocityYbyImpuse(10.8);
    }
    
}

void CstateMachine_player::turnToRunOutOfWater(){
    Cplayer*player=(Cplayer*)m_obj;
    const float waterSurfaceHeight=player->m_water->getSurfaceHeight();
    if((player->boundingBox().getMinY()+player->boundingBox().getMaxY())/2>waterSurfaceHeight&&player->m_isTouchSolid){
        if(this->is_xx_forward()){
            if(this->isFacingRight()){
                this->changeStateAndApearance(player_state_forward_right);
            }else{
                this->changeStateAndApearance(player_state_forward_left);
            }
        }else if(this->is_xx_still()){
            if(this->isFacingRight()){
                this->changeStateAndApearance(player_state_still_right);
            }else{
                this->changeStateAndApearance(player_state_still_left);
            }
        }
    }
}

void CstateMachine_player::turnToFallToWater(){
    Cplayer*player=(Cplayer*)m_obj;
    const float waterSurfaceHeight=player->m_water->getSurfaceHeight();
    if(player->boundingBox().getMinY()<waterSurfaceHeight&&player->m_isTouchSolid==false){
        if(this->is_xx_forward()){
            if(this->isFacingRight()){
                this->changeStateAndApearance(player_state_swim_forward_right);
            }else{
                this->changeStateAndApearance(player_state_swim_forward_left);
            }
        }else if(this->is_xx_still()){
            if(this->isFacingRight()){
                this->changeStateAndApearance(player_state_swim_still_right);
            }else{
                this->changeStateAndApearance(player_state_swim_still_left);
            }
        }
        
    }
    
}
void CstateMachine_player::turnToFallCliff(){
    Cplayer*player=(Cplayer*)m_obj;
    const float waterSurfaceHeight=player->m_water->getSurfaceHeight();
    if(this->is_jump()==false
       &&player->m_isTouchSolid==false
       &&player->boundingBox().getMinY()>=waterSurfaceHeight){
        if(this->is_xx_forward()){
            if(this->isFacingRight()){
                this->changeStateAndApearance(player_state_jump_forward_right);
            }else{
                this->changeStateAndApearance(player_state_jump_forward_left);
            }
        }else if(this->is_xx_still()){
            if(this->isFacingRight()){
                this->changeStateAndApearance(player_state_jump_still_right);
            }else{
                this->changeStateAndApearance(player_state_jump_still_left);
            }
        }
    }
    
}
void CstateMachine_player::turnToJumpOutOfWater(){
    Cplayer*player=(Cplayer*)m_obj;
    const float waterSurfaceHeight=player->m_water->getSurfaceHeight();
    if(player->boundingBox().getMinY()>waterSurfaceHeight&&player->m_isTouchSolid==false){
        if(this->is_xx_forward()){
            if(this->isFacingRight()){
                this->changeStateAndApearance(player_state_jump_forward_right);
            }else{
                this->changeStateAndApearance(player_state_jump_forward_left);
            }
        }else if(this->is_xx_still()){
            if(this->isFacingRight()){
                this->changeStateAndApearance(player_state_jump_still_right);
            }else{
                this->changeStateAndApearance(player_state_jump_still_left);
            }
        }
    }
    
}

void CstateMachine_player::turnToFallToGround(){
    Cplayer*player=(Cplayer*)m_obj;
    const float waterSurfaceHeight=player->m_water->getSurfaceHeight();
    if(this->is_jump()&&player->m_isTouchSolid){
        if(this->is_xx_forward()){
            if(this->isFacingRight()){
                this->changeStateAndApearance(player_state_forward_right);
            }else{
                this->changeStateAndApearance(player_state_forward_left);
            }
        }else if(this->is_xx_still()){
            if(this->isFacingRight()){
                this->changeStateAndApearance(player_state_still_right);
            }else{
                this->changeStateAndApearance(player_state_still_left);
            }
        }
    }
}

//----player
CCAnimation* createAnimationWithStripeTex(CCTexture2D*stripeTex,string aniName){
    float w=stripeTex->getContentSize().width;
    float h=stripeTex->getContentSize().height;
    int nFrame=(int)w/(int)h;
    CCAnimation*animation=CCAnimation::create();
    for(int i=0;i<nFrame;i++){
        animation->addSpriteFrameWithTexture(stripeTex, CCRect(i*h,0,h,h));
    }
    CCAnimationCache::sharedAnimationCache()->addAnimation(animation, aniName.c_str());
    return animation;
}
CCAnimation* createAnimationWithStripeImage(string imageName){
    CCTexture2D*stripeTex=CCTextureCache::sharedTextureCache()->addImage(imageName.c_str());
    return createAnimationWithStripeTex(stripeTex,imageName);
}
void Cplayer::runAction_runRight(){
    this->setScaleX(fabsf(this->getScaleX()));
    this->m_ani_runRight->setDelayPerUnit(0.13f);
    this->runAction(CCRepeatForever::create(CCAnimate::create(this->m_ani_runRight)));
}
void Cplayer::runAction_swimRight(){
    this->setScaleX(fabsf(this->getScaleX()));
    this->m_ani_swimRight->setDelayPerUnit(0.13f);
    this->runAction(CCRepeatForever::create(CCAnimate::create(this->m_ani_swimRight)));
}
void Cplayer::setPosture_jumpRight(){
    this->setScaleX(fabsf(this->getScaleX()));
    this->setTexture(this->m_tex_jumpRight);
    this->setTextureRect(getTexRect(this->m_tex_jumpRight));
}
void Cplayer::setPosture_standRight(){
    this->setScaleX(fabsf(this->getScaleX()));
    this->setTexture(this->m_tex_standRight);
    this->setTextureRect(getTexRect(this->m_tex_standRight));
}
void Cplayer::runAction_runLeft(){
    this->setScaleX(-fabsf(this->getScaleX()));
    this->m_ani_runRight->setDelayPerUnit(0.13f);
    this->runAction(CCRepeatForever::create(CCAnimate::create(this->m_ani_runRight)));
}
void Cplayer::runAction_swimLeft(){
    this->setScaleX(-fabsf(this->getScaleX()));
    this->m_ani_swimRight->setDelayPerUnit(0.13f);
    this->runAction(CCRepeatForever::create(CCAnimate::create(this->m_ani_swimRight)));
}
void Cplayer::setPosture_jumpLeft(){
    this->setScaleX(-fabsf(this->getScaleX()));
    this->setTexture(this->m_tex_jumpRight);
    this->setTextureRect(getTexRect(this->m_tex_jumpRight));
}
void Cplayer::setPosture_standLeft(){
    this->setScaleX(-fabsf(this->getScaleX()));
    this->setTexture(this->m_tex_standRight);
    this->setTextureRect(getTexRect(this->m_tex_standRight));
}
bool Cplayer::init(CCSize size,CCPoint postion,b2World*world){
    this->createTexAndAni();
    b2Filter filter;
    filter.categoryBits=0x0004;
    filter.maskBits=0xFFFF & ~0x0008;
    this->createB2Body(world,(size.width/2)/PTM_RATIO,(size.height/2)/PTM_RATIO,&filter);
    this->pb2Body->SetTransform(b2Vec2(postion.x/PTM_RATIO, postion.y/PTM_RATIO), 0);
    this->pb2Body->SetType(b2_dynamicBody);
    this->pb2Body->SetFixedRotation(true);
    //
    assert(m_stateMachine==NULL);
    CstateMachine_player*stateMachine=CstateMachine_player::create();
    stateMachine->setObj(this);
    stateMachine->retain();
    m_stateMachine=stateMachine;
    return true;
}

void Cplayer::createB2Body(b2World* world,float rx,float ry,const b2Filter*fliter){
    // Define the dynamic body.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.userData=this;
    
    b2Body *body = world->CreateBody(&bodyDef);
    
    // Define another box shape for our dynamic body.
    const float margx=0.3;
    const float margy_up=0.15;
    const float margy_bottom=0.3;
    b2PolygonShape shape_body;
    {
        b2Vec2 verts[8];//max vertex number is 8 in box2d
        verts[0].Set(-rx/3+margx, ry-margy_up);
        verts[1].Set(-rx+margx, ry*0.6-margy_up);
        verts[2].Set(-rx+margx, -ry/3.5+margy_bottom);
        verts[3].Set(-rx*0.3+margx, -ry+margy_bottom);
        verts[4].Set(rx*0.3-margx, -ry+margy_bottom);
        verts[5].Set(rx-margx, -ry/3.5+margy_bottom);
        verts[6].Set(rx-margx, ry*0.6-margy_up);
        verts[7].Set(rx/3-margx, ry-margy_up);
        shape_body.Set(verts, 8);
    }
    //
    b2PolygonShape shape_foot;
    {
        b2Vec2 verts[4];
        verts[0].Set(-rx+0.5+margx+0.08, -ry*0.4);
        verts[1].Set(-rx*0.3+margx, -ry+margy_bottom);
        verts[2].Set(rx*0.3-margx, -ry+margy_bottom);
        verts[3].Set(rx-0.5-margx-0.08, -ry*0.4);
        shape_foot.Set(verts, 4);
        
    }
    //
    b2PolygonShape shape_foot2;
    {
        const float Ydown=5.0/PTM_RATIO;
        b2Vec2 verts[4];
        verts[0].Set(-rx+0.5+margx+0.08, -ry*0.4-Ydown);
        verts[1].Set(-rx*0.3+margx, -ry+margy_bottom-Ydown);
        verts[2].Set(rx*0.3-margx, -ry+margy_bottom-Ydown);
        verts[3].Set(rx-0.5-margx-0.08, -ry*0.4-Ydown);
        shape_foot2.Set(verts, 4);
        
    }


    
    //fixture_body
    {
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape_body;
        fixtureDef.density = 10.0f;
        fixtureDef.friction = 0;
        fixtureDef.isSensor=false;
        if(fliter!=NULL){
            fixtureDef.filter=*fliter;
        }
        m_fixture_body=body->CreateFixture(&fixtureDef);
        
    }
    
    //fixture_foot
    //fixture_foot is used for collision test with ground and friction.
    //because fixture_body have no friction, so we need fixture_foot to do this
    {
        b2FixtureDef fixtureDef;
        fixtureDef.shape=&shape_foot;
        fixtureDef.density=0.5f;
        fixtureDef.friction=1;
        fixtureDef.isSensor=false;
        if(fliter!=NULL){
            fixtureDef.filter=*fliter;
        }
        m_fixture_foot=body->CreateFixture(&fixtureDef);
        
    }
    //fixture_foot2
    //fixture_foot2 is used for collision test with ground
    //because fixture_foot for collision test with ground is not stable,
    //so we need a sensior which can overlap with ground to achieve more stable result
    {
        b2FixtureDef fixtureDef;
        fixtureDef.shape=&shape_foot2;
        fixtureDef.density=0;
        fixtureDef.friction=0;
        fixtureDef.isSensor=true;
        if(fliter!=NULL){
            fixtureDef.filter=*fliter;
        }
        m_fixture_foot2=body->CreateFixture(&fixtureDef);
        
    }

    //pb2Body
    pb2Body=body;
}
void Cplayer::setWater(Cwater*water){
    assert(water!=NULL);
    assert(m_water==NULL);
    m_water=water;
    m_water->retain();
}
void Cplayer::updateIsTouchSolid() {
    int nContact=0;
    for(b2ContactEdge*ce=this->pb2Body->GetContactList();ce;ce=ce->next){
        b2Contact* contact=ce->contact;
        //there is contact just means there has possiblity of contact
        //(for example the bounding boxes overlap),but not guarantee.
        //see: http://www.box2d.org/forum/viewtopic.php?f=3&t=7652
        //but here we want to know the acurate result (whether really contacted),
        //so we use contact->isTouching() to judge.
        //see: box2d manual 9.2 Contact Class
        if(contact->IsTouching()==false)continue;
        b2Fixture*fixtureA=contact->GetFixtureA();
        b2Fixture*fixtureB=contact->GetFixtureB();
        if(fixtureA==this->m_fixture_foot||fixtureA==this->m_fixture_foot2){
            nContact++;
        }
        if(fixtureB==this->m_fixture_foot||fixtureB==this->m_fixture_foot2){
            nContact++;
        }
    }
    m_isTouchSolid=(nContact>0);
}
void Cplayer::pressWater(){
    bool isInWater=(this->boundingBox().getMinY()<m_water->getSurfaceHeight());
    CmyObj::pressWater(m_water,isInWater,15,30);
}
/*
bool Cplayer::changeStateAndApearance(int newState)
//if state and apearance really changed, return true, else return false
{
    if(m_state!=newState){//if state is no difference with old state, do nothing and return false
        m_state=newState;
        this->stopAllActions();
        setApearanceByState(m_state);
        return true;
    }else{
        return false;
    }
}*/
void Cplayer::setApearanceByState(int _state){
    switch (_state) {
        case player_state_still_right:
            setPosture_standRight();
            break;
        case player_state_still_left:
            setPosture_standLeft();
            break;
        case player_state_forward_right:
            runAction_runRight();
            break;
        case player_state_forward_left:
            runAction_runLeft();
            break;
        case player_state_jump_still_right:
            setPosture_jumpRight();
            break;
        case player_state_jump_still_left:
            setPosture_jumpLeft();
            break;
        case player_state_jump_forward_right:
            setPosture_jumpRight();
            break;
        case player_state_jump_forward_left:
            setPosture_jumpLeft();
            break;
        case player_state_swim_still_right:
            setPosture_standRight();
            break;
        case player_state_swim_still_left:
            setPosture_standLeft();
            break;
        case player_state_swim_forward_right:
            runAction_swimRight();
            break;
        case player_state_swim_forward_left:
            runAction_swimLeft();
            break;
        default:
            assert(false);
            break;
    }
    
}
void Cplayer::createTexAndAni(){
    this->m_ani_runRight=createAnimationWithStripeImage("data/global/tex/player_run_right.png");
    this->m_ani_swimRight=createAnimationWithStripeImage("data/global/tex/player_swim_right.png");
    this->m_tex_standRight=CCTextureCache::sharedTextureCache()->addImage("data/global/tex/stand_right.png");
    this->m_tex_jumpRight=CCTextureCache::sharedTextureCache()->addImage("data/global/tex/jump_right.png");
    this->initWithTexture(this->m_tex_standRight);
}
