//
//  player.cpp
//  mybox2dtest
//
//  Created by apple on 13-7-21.
//
//

#include "player.h"
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

void Cplayer::print_sate()const {
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
bool Cplayer::is_jump_still(int _state)const {
    return (_state==player_state_jump_still_right||_state==player_state_jump_still_left);
}
bool Cplayer::is_jump_forward(int _state)const {
    return (_state==player_state_jump_forward_right||_state==player_state_jump_forward_left);
}
bool Cplayer::is_forward(int _state)const {
    return (_state==player_state_forward_left||_state==player_state_forward_right);
}
bool Cplayer::is_still(int _state)const {
    return (_state==player_state_still_left||_state==player_state_still_right);
}
bool Cplayer::is_swim_forward(int _state)const {
    return (_state==player_state_swim_forward_right||_state==player_state_swim_forward_left);
}
bool Cplayer::is_swim_still(int _state)const {
    return (_state==player_state_swim_still_right||_state==player_state_swim_still_left);
}
bool Cplayer::is_jump(int _state)const {
    return (is_jump_forward(_state)||is_jump_still(_state));
}
bool Cplayer::is_swim(int _state)const {
    return (is_swim_forward(_state)||is_swim_still(_state));
}
bool Cplayer::is_xx_still(int _state)const {
    return (is_still(_state)||is_jump_still(_state)||is_swim_still(_state));
}
bool Cplayer::is_xx_forward(int _state)const {
    return (is_forward(_state)||is_jump_forward(_state)||is_swim_forward(_state));
}
bool Cplayer::isFacingRight(int _state)const {
    switch (_state) {
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
bool Cplayer::init(CCSize size,CCPoint postion,b2World*world){
    this->createTexAndAni();
    b2Filter filter;
    filter.categoryBits=0x0004;
    filter.maskBits=0xFFFF & ~0x0008;
    this->createB2Body(world,(size.width/2)/PTM_RATIO,(size.height/2)/PTM_RATIO,&filter);
    this->pb2Body->SetTransform(b2Vec2(postion.x/PTM_RATIO, postion.y/PTM_RATIO), 0);
    this->pb2Body->SetType(b2_dynamicBody);
    this->pb2Body->SetFixedRotation(true);
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
void Cplayer::turnToRight(){
    if(this->is_jump(m_state)){
        this->changeStateAndApearance(player_state_jump_forward_right);
    }else if(this->is_forward(m_state)||this->is_still(m_state)){
        this->changeStateAndApearance(player_state_forward_right);
    }else if(this->is_swim(m_state)){
        this->changeStateAndApearance(player_state_swim_forward_right);
    }

}
void Cplayer::turnToLeft(){
    if(this->is_jump(m_state)){
        this->changeStateAndApearance(player_state_jump_forward_left);
    }else if(this->is_forward(m_state)||this->is_still(m_state)){
        this->changeStateAndApearance(player_state_forward_left);
    }else if(this->is_swim(m_state)){
        this->changeStateAndApearance(player_state_swim_forward_left);
    }
}
void Cplayer::turnToStill(){
    //stop move
    b2Vec2 v=this->pb2Body->GetLinearVelocity();
    this->pb2Body->SetLinearVelocity(b2Vec2(0,v.y));
    
    if(this->isFacingRight(m_state)){
        if(this->is_jump(m_state)){
            this->changeStateAndApearance(player_state_jump_still_right);
        }else if(this->is_forward(m_state)||this->is_still(m_state)){
            this->changeStateAndApearance(player_state_still_right);
        }else if(this->is_swim(m_state)){
            this->changeStateAndApearance(player_state_swim_still_right);
        }
    }else{//facing left
        if(this->is_jump(m_state)){
            this->changeStateAndApearance(player_state_jump_still_left);
        }else if(this->is_forward(m_state)||this->is_still(m_state)){
            this->changeStateAndApearance(player_state_still_left);
        }else if(this->is_swim(m_state)){
            this->changeStateAndApearance(player_state_swim_still_left);
        }
    }

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

void Cplayer::turnToJump(){
    if(this->is_jump(m_state)==false){
        if(this->is_forward(m_state)){
            if(this->isFacingRight(m_state)){
                this->changeStateAndApearance(player_state_jump_forward_right);
            }else{
                this->changeStateAndApearance(player_state_jump_forward_left);
            }
        }else if(this->is_still(m_state)){
            if(this->isFacingRight(m_state)){
                this->changeStateAndApearance(player_state_jump_still_right);
            }else{
                this->changeStateAndApearance(player_state_jump_still_left);
            }
        }
        this->changeVelocityYbyImpuse(10.8);
    }
    
}

void Cplayer::turnToRunOutOfWater(){
     const float waterSurfaceHeight=m_water->getSurfaceHeight();
    if((this->boundingBox().getMinY()+this->boundingBox().getMaxY())/2>waterSurfaceHeight&&this->m_isTouchSolid){
        if(this->is_xx_forward(m_state)){
            if(this->isFacingRight(m_state)){
                this->changeStateAndApearance(player_state_forward_right);
            }else{
                this->changeStateAndApearance(player_state_forward_left);
            }
        }else if(this->is_xx_still(m_state)){
            if(this->isFacingRight(m_state)){
                this->changeStateAndApearance(player_state_still_right);
            }else{
                this->changeStateAndApearance(player_state_still_left);
            }
        }
    }
}

void Cplayer::turnToFallToWater(){
    const float waterSurfaceHeight=m_water->getSurfaceHeight();
    if(this->boundingBox().getMinY()<waterSurfaceHeight&&this->m_isTouchSolid==false){
        if(this->is_xx_forward(m_state)){
            if(this->isFacingRight(m_state)){
                this->changeStateAndApearance(player_state_swim_forward_right);
            }else{
                this->changeStateAndApearance(player_state_swim_forward_left);
            }
        }else if(this->is_xx_still(m_state)){
            if(this->isFacingRight(m_state)){
                this->changeStateAndApearance(player_state_swim_still_right);
            }else{
                this->changeStateAndApearance(player_state_swim_still_left);
            }
        }
        
    }
    
}
void Cplayer::turnToFallCliff(){
    const float waterSurfaceHeight=m_water->getSurfaceHeight();
    if(this->is_jump(m_state)==false
       &&this->m_isTouchSolid==false
       &&this->boundingBox().getMinY()>=waterSurfaceHeight){
        if(this->is_xx_forward(m_state)){
            if(this->isFacingRight(m_state)){
                this->changeStateAndApearance(player_state_jump_forward_right);
            }else{
                this->changeStateAndApearance(player_state_jump_forward_left);
            }
        }else if(this->is_xx_still(m_state)){
            if(this->isFacingRight(m_state)){
                this->changeStateAndApearance(player_state_jump_still_right);
            }else{
                this->changeStateAndApearance(player_state_jump_still_left);
            }
        }
    }
    
}
void Cplayer::turnToJumpOutOfWater(){
    const float waterSurfaceHeight=m_water->getSurfaceHeight();
    if(this->boundingBox().getMinY()>waterSurfaceHeight&&this->m_isTouchSolid==false){
        if(this->is_xx_forward(m_state)){
            if(this->isFacingRight(m_state)){
                this->changeStateAndApearance(player_state_jump_forward_right);
            }else{
                this->changeStateAndApearance(player_state_jump_forward_left);
            }
        }else if(this->is_xx_still(m_state)){
            if(this->isFacingRight(m_state)){
                this->changeStateAndApearance(player_state_jump_still_right);
            }else{
                this->changeStateAndApearance(player_state_jump_still_left);
            }
        }
    }
    
}
void Cplayer::pressWater(){
    bool isInWater=(this->boundingBox().getMinY()<m_water->getSurfaceHeight());
    CmyObj::pressWater(m_water,isInWater,15,30);
}
void Cplayer::turnToFallToGround(){
    const float waterSurfaceHeight=m_water->getSurfaceHeight();
    if(this->is_jump(m_state)&&this->m_isTouchSolid){
        if(this->is_xx_forward(m_state)){
            if(this->isFacingRight(m_state)){
                this->changeStateAndApearance(player_state_forward_right);
            }else{
                this->changeStateAndApearance(player_state_forward_left);
            }
        }else if(this->is_xx_still(m_state)){
            if(this->isFacingRight(m_state)){
                this->changeStateAndApearance(player_state_still_right);
            }else{
                this->changeStateAndApearance(player_state_still_left);
            }
        }
    }
}
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
}
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
void Cplayer::run(){
    if(this->m_state==player_state_forward_right){
        this->changeVelocityXbyImpuse(5);
    }
    if(this->m_state==player_state_forward_left){
        this->changeVelocityXbyImpuse(-5);
    }
}
void Cplayer::jump(){
    if(this->m_state==player_state_jump_forward_right)
    {
        this->changeVelocityXbyImpuse(5);
    }
    if(this->m_state==player_state_jump_forward_left)
    {
        this->changeVelocityXbyImpuse(-5);
    }
}
void Cplayer::swim(){
    if(this->m_state==player_state_swim_forward_right)
    {
        this->changeVelocityXbyImpuse(5);
    }
    if(this->m_state==player_state_swim_forward_left)
    {
        this->changeVelocityXbyImpuse(-5);
    }
}


