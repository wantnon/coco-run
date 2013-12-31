//
//  myBox2dFunc.h
//  mybox2dtest
//
//  Created by apple on 13-7-23.
//
//

#ifndef __mybox2dtest__myBox2dFunc__
#define __mybox2dtest__myBox2dFunc__

#include <iostream>
#include <vector>
#include <string>
using namespace std;
#include "Box2D.h"
#include "myDef.h"
#include "myfunc.h"
b2Vec2 getAveragePoint(const vector<b2Vec2>&b2PointList);
b2AABB getAABB(b2Shape*shape,const b2Transform&transform);
int getFixtureCount(b2Body*body);
b2Fixture* getFixtureAt_backWards(b2Body*body,int index);
b2BodyType bodyTypeStrTobodyTypeValue(string bodyTypeStr);

void splitPolygonShapeWithHoriLine(b2PolygonShape&shape,b2Transform transform,float H_horiLine,
                                    bool&upperShapeGot,bool&lowerShapeGot,
                                   b2PolygonShape&upperShape,b2PolygonShape&lowerShape);

void buoyancyEffect(b2Body*body,int tgFixtureIndex,float waterSurfaceHeight,float waterDensity,b2World*world,float dt);


#endif /* defined(__mybox2dtest__myBox2dFunc__) */
