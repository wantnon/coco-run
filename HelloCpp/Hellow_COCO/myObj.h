//
//  myObj.h
//  mybox2dtest
//
//  Created by apple on 13-7-8.
//
//

#ifndef __mybox2dtest__myObj__
#define __mybox2dtest__myObj__

#include <iostream>
using namespace std;
#include "cocos2d.h"
#include "Box2D.h"
#include "myfunc.h"
#include "myDef.h"
#include "xmlLoader_myObj.h"
#include "water.h"
using namespace cocos2d;
class CmyObj:public cocos2d::CCSprite
{
public:

	b2World* world;
	string sceneNodeType;
	b2Body* pb2Body;
    string name;
    void initParameter(){
        world=NULL;
        pb2Body=NULL;
    }
    CmyObj(){
        initParameter();
    }
	CmyObj(b2World*_world){
        initParameter();
		world=_world;
	}
	virtual ~CmyObj(){
		if(pb2Body!=NULL){
			world->DestroyBody(pb2Body);
			pb2Body=NULL;
		}
	}
	void initWithInfo(const CmyObjInfo&objInfo){
		//----bodyList
        vector<b2Body*> bodyList;
		int nbody=(int)objInfo.bodyInfoList.size();
        assert(nbody<=1);
		for(int i=0;i<nbody;i++)
		{
			b2Body*body=NULL;
			const CbodyInfo&bodyInfo=objInfo.bodyInfoList[i];
			b2BodyDef bodyDef;
			bodyDef.type = bodyInfo.bodyType;
			body = world->CreateBody(&bodyDef);
			//----fixture
			int nfixture=(int)bodyInfo.fixtureInfoList.size();
			for(int i=0;i<nfixture;i++){
				const CfixtureInfo&fixtureInfo=bodyInfo.fixtureInfoList[i];
				b2FixtureDef fixtureDef;
				//fixture
				fixtureDef.density=fixtureInfo.density;
				fixtureDef.restitution=fixtureInfo.restitution;
				fixtureDef.friction=fixtureInfo.friction;
				fixtureDef.isSensor=fixtureInfo.isSensor;
				fixtureDef.filter.categoryBits=fixtureInfo.filterInfo.categoryBits;
				fixtureDef.filter.maskBits=fixtureInfo.filterInfo.maskBits;
				fixtureDef.filter.groupIndex=fixtureInfo.filterInfo.groupIndex;
				//shape
				const CshapeInfo&shapeInfo=fixtureInfo.shapeInfo;
				switch (shapeInfo.shapeType) {
				case b2Shape::e_polygon:
					{
						b2PolygonShape shape;
						int nv=(int)shapeInfo.vertices.size();
						if(nv!=0){
							shape.Set(&shapeInfo.vertices[0], nv);
						}else{
                            assert(shapeInfo.rx!=0&&shapeInfo.ry!=0);
                            shape.SetAsBox(shapeInfo.rx,shapeInfo.ry, shapeInfo.c, shapeInfo.rot);
						}
						fixtureDef.shape=&shape;
						body->CreateFixture(&fixtureDef);//�����ڴ˴�createFixture���Ա���shapeʧЧ
					}break;
				case b2Shape::e_circle:
					{
						b2CircleShape circle;
                        assert(shapeInfo.r!=0);
                        circle.m_p.Set(shapeInfo.c.x, shapeInfo.c.y);
                        circle.m_radius=shapeInfo.r;
						fixtureDef.shape=&circle;
						body->CreateFixture(&fixtureDef);
					}break;
				case b2Shape::e_edge:
					{
						assert(false);//not Implemented yet
					}break;
				case b2Shape::e_chain:
					{
						assert(false);////not Implemented yet
					}break;
				default:
                        assert(false);
					break;
				}
			}
			//----body transform
			{
				body->SetTransform(bodyInfo.pos, bodyInfo.rot);
			}
			//----gravityScale
			{
				body->SetGravityScale(bodyInfo.gravityScale);
			}
			//----add to bodyList
			bodyList.push_back(body);
		}
		//----sprite
		{
			const CspriteInfo&spriteInfo=objInfo.spriteInfo;
			CCTexture2D*texture=CCTextureCache::sharedTextureCache()->addImage(spriteInfo.texImage.c_str());
            CCLOG("spriteInfo.texImage:%s",spriteInfo.texImage.c_str());
			this->initWithTexture(texture);
            this->setAnchorPoint(spriteInfo.anchorPoint);
			this->setPosition(spriteInfo.pos);
			this->pb2Body=bodyList.empty()?NULL:bodyList[0];
			if(this->pb2Body!=NULL){
				this->pb2Body->SetUserData(this);
			}
		}

	}
	static CmyObj* createObjWithXML(const string&xmlPathShort,b2World*world)
	{
		string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(xmlPathShort.c_str());
		tinyxml2::XMLDocument doc;
		doc.LoadFile(fullPath.c_str());
		XMLElement *root=doc.RootElement();
		CmyObjInfo myObjInfo;
		myObjInfo.initWithXMLElement(root);
		//myObjListInfo.print();
        //create obj
        CmyObj*pObj=new CmyObj(world);
        pObj->autorelease();
        pObj->initWithInfo(myObjInfo);
		return pObj;
	}
  	void removeMeFromSceneAndDestroyBody(bool cleanup){
		world->DestroyBody(this->pb2Body);
		this->pb2Body=NULL;
		this->removeFromParentAndCleanup(cleanup);
	}

	void create_pb2Body_box(float rx,float ry,b2BodyType bodyType,float friction=0.5,const b2Filter*filter=NULL){
        assert(this->pb2Body==NULL);
		//define body
		b2BodyDef bodyDef;
		bodyDef.type = bodyType;
		bodyDef.userData=this;
		//define shape
		b2PolygonShape Box;
		Box.SetAsBox(rx,ry);
		//define fixtures
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &Box;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = friction;
		if(filter!=NULL){
			fixtureDef.filter =*filter;
		}
        //create body
        b2Body *body = world->CreateBody(&bodyDef);
		body->CreateFixture(&fixtureDef);
		this->pb2Body=body;
	}
	void create_pb2Body_circle(float r,
								b2BodyType bodyType,
								float friction,float restitution,float density,bool isSensor,
								int categoryBits,int maskBits,int groupIndex,
								float gravityScale){
		assert(this->pb2Body==NULL);
        //make filter
        b2Filter filter;
        filter.categoryBits=categoryBits;
        filter.maskBits=maskBits;
        filter.groupIndex=groupIndex;
		//define body.
		b2BodyDef bodyDef;
		bodyDef.type = bodyType;
		bodyDef.userData=this;
		//define shape
		b2CircleShape circleShape;
		circleShape.m_p.Set(0, 0);
		circleShape.m_radius=r;
		//define fixtures.
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &circleShape;
		fixtureDef.density =density;
		fixtureDef.friction = friction;
		fixtureDef.filter =filter;
        //create body
        b2Body *body = world->CreateBody(&bodyDef);
		body->CreateFixture(&fixtureDef);
		body->SetGravityScale(gravityScale);
		this->pb2Body=body;
	}
	void create_pb2Body_polygon(const vector<b2Vec2>&vertList,
								b2BodyType bodyType,
								float friction,float restitution,float density,bool isSensor,
								int categoryBits,int maskBits,int groupIndex,
								float gravityScale){
		assert(this->pb2Body==NULL);
        //make filter
        b2Filter filter;
        filter.categoryBits=categoryBits;
        filter.maskBits=maskBits;
        filter.groupIndex=groupIndex;
		//define body
		b2BodyDef bodyDef;
		bodyDef.type = bodyType;
		bodyDef.userData=this;
		//define shape
		b2PolygonShape polygonShape;
		int nv=(int)vertList.size();
		polygonShape.Set(&vertList[0],nv);
		//define fixtures.
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &polygonShape;
		fixtureDef.density =density;
		fixtureDef.friction = friction;
		fixtureDef.filter =filter;
        //create body
        b2Body *body = world->CreateBody(&bodyDef);
		body->CreateFixture(&fixtureDef);
		body->SetGravityScale(gravityScale);
		this->pb2Body=body;
	}
    void changeVelocityXbyImpuse(float vx){//apply impulse to let player's velocity's x component equals to vx
        if(this->pb2Body){
            float m=this->pb2Body->GetMass();
            b2Vec2 v=this->pb2Body->GetLinearVelocity();
            b2Vec2 dv=b2Vec2(vx-v.x,0);
            b2Vec2 impluse=m*dv;
            this->pb2Body->ApplyLinearImpulse(impluse, this->pb2Body->GetWorldCenter());
        }
    }
    void changeVelocityYbyImpuse(float vy){//apply impulse to let player's velocity's y component equals to vy
        if(this->pb2Body){
            float m=pb2Body->GetMass();
            b2Vec2 v=pb2Body->GetLinearVelocity();
            b2Vec2 dv=b2Vec2(0,vy-v.y);
            b2Vec2 impluse=m*dv;
            pb2Body->ApplyLinearImpulse(impluse, pb2Body->GetWorldCenter());
        }
    }
    void pressWater(Cwater*water,bool isInWater,float pressStrengthX,float pressStrengthY){
        //if in water, the fast player moves, the intense it press the water
        if(isInWater){
            b2Vec2 v=this->pb2Body->GetLinearVelocity();
            float vx=v.x/PTM_RATIO;
            float vy=v.y/PTM_RATIO;
            float pressWaterIntense=vx*pressStrengthX+vy*pressStrengthY;
            water->waterSurface.pressAtX(this->getPositionX(),pressWaterIntense);
        }
    }
};
#endif /* defined(__mybox2dtest__myObj__) */
