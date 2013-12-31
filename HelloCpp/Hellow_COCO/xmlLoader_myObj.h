
#ifndef __xmlLoader_myObj__
#define __xmlLoader_myObj__

#include <iostream>
using namespace std;
#include "cocos2d.h"
#include "Box2D.h"
#include "tinyxml2.h"
#include "myfunc.h"
#include "myDef.h"
using namespace cocos2d;
using namespace tinyxml2;
class CshapeInfo{
public:
	b2Shape::Type shapeType;
	b2Vec2 c;
	float r;
	float rx,ry;
	float rot;
	vector<b2Vec2> vertices;
	void print();
	void initWithXMLElement(XMLElement *p);
};
class CfilterInfo{
public:
	uint16 categoryBits;
	uint16 maskBits;
	int16 groupIndex;
	void print();
	void initWithXMLElement(XMLElement *p);
};
class CfixtureInfo{
public:
	CshapeInfo shapeInfo;
	float friction;
	float restitution;
	float density;
	bool isSensor;
	CfilterInfo filterInfo;
	void print();
	void initWithXMLElement(XMLElement *p);
};
class CbodyInfo{
public:
	b2BodyType bodyType;
	vector<CfixtureInfo> fixtureInfoList;
	b2Vec2 pos;
	float rot;
	float gravityScale;
	void print();
	void initWithXMLElement(XMLElement *p);
private:
	b2BodyType getBodyType(const string&str);

};

class CspriteInfo{
public:
	string texImage;
	CCPoint anchorPoint;
	CCPoint pos;
	void print();
	void initWithXMLElement(XMLElement *p);
};
class CmyObjInfo{
public:
	string name;
	vector<CbodyInfo> bodyInfoList;
	CspriteInfo spriteInfo;
	void print();
	void initWithXMLElement(XMLElement *p);


};
#endif