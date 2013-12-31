//
//  fractalTerrain.h
//  HelloCpp
//
//  Created by apple on 13-12-15.
//
//

#ifndef __HelloCpp__fractalTerrain__
#define __HelloCpp__fractalTerrain__

#include <iostream>
#include <vector.h>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "myfunc.h"
#include "Box2D.h"
#include "myDef.h"
#include "simpleClasses.h"
#include "indexVBO.h"
class ClineSeg final{
public:
    CCPoint startPoint;
    CCPoint endPoint;
    ClineSeg*father;
    ClineSeg*child;
    void initParameters(){
        father=NULL;
        child=NULL;
    }
    ClineSeg(){
        initParameters();
    }
    ClineSeg(const CCPoint&start,const CCPoint&end){
        initParameters();
        init(start, end);
    }
    ~ClineSeg(){};
    void init(CCPoint start,CCPoint end){
        startPoint=start;
        endPoint=end;
    }
    float getLen()const {
        return ccpDistance(startPoint, endPoint);
    }
    CCPoint getDir()const {
        return ccpNormalize(endPoint-startPoint);
    }
    CCPoint getVec()const {
        return endPoint-startPoint;
    }
    CCPoint getRightNormal()const {//right side normal or outer normal
        CCPoint dir=getDir();
        CCPoint normal(dir.y,-dir.x);
        return normal;
    }
    CCPoint getMidPoint()const {
        return ccpMidpoint(startPoint, endPoint);
    }
};
class Cpolygon{
public:
    vector<CCPoint> vlist;
};
class CfractalTerrainSprite:public CCSprite
{
public:
    CfractalTerrainSprite(){
        nGeneration=5;
        k_offset=0.15;
        headSeg=NULL;
        tailSeg=NULL;
        pb2Body=NULL;
        world=NULL;
        texSprite=NULL;
        texSprite_verySmooth=NULL;
    }
    virtual ~CfractalTerrainSprite(){
        if(texSprite)texSprite->release();
        if(texSprite_verySmooth)texSprite_verySmooth->release();
        //clear segList
        int nSeg=(int)segList.size();
        for(int i=0;i<nSeg;i++){
            delete segList[i];
            segList[i]=NULL;
        }
        segList.clear();
        //destoryBody
        if (world) {
            if(pb2Body)world->DestroyBody(pb2Body);
            fixtureList.clear();
        }else{
            assert(pb2Body==NULL);
            assert(fixtureList.empty());
        }
        
    }
    bool init();
    void setStartPoint(CCPoint start){startPoint=this->convertToNodeSpace(start);}
    void setEndPoint(CCPoint end){endPoint=this->convertToNodeSpace(end);}
    CCPoint getStartPoint() {return this->convertToWorldSpace(startPoint);}
    CCPoint getEndPoint() {return this->convertToWorldSpace(endPoint);}
    vector<ClineSeg*> splitSeg(ClineSeg*seg,int generationID);
    void genSeges();
    void doGenerationOnce(vector<ClineSeg*>&segList,int generationID);
    void genBodyAndMesh(b2World*world,float wantedYmin);
    virtual void draw();
    void setNGenerationBySegLen(float segLen);
    void setK_offset(float value){k_offset=value;};
private:
    CCPoint startPoint;//local
    CCPoint endPoint;//local
    vector<ClineSeg*> segList;
    int nGeneration;
    float k_offset;//maxOffset div segLen
    ClineSeg*headSeg;
    ClineSeg*tailSeg;
    vector<b2Fixture*> fixtureList;
    b2Body* pb2Body;
    b2World*world;
    Cmesh mesh;
    Cmesh mesh_verySmooth;
    CindexVBO indexVBO;
    CindexVBO indexVBO_verySmooth;
    CCSprite*texSprite;
    CCSprite*texSprite_verySmooth;
    
};
#endif /* defined(__HelloCpp__fractalTerrain__) */
