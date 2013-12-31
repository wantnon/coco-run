//
//  infiniteTerrainManager.h
//  HelloCpp
//
//  Created by ck02-084 on 13-12-17.
//
//

#ifndef __HelloCpp__infiniteTerrainManager__
#define __HelloCpp__infiniteTerrainManager__

#include <iostream>
using namespace std;
#include "fractalTerrain.h"
class CinfiniteTerrainManager{
public:
    vector<CfractalTerrainSprite*> terrainList;
private:
    void removeTerrainInTerrainList(CfractalTerrainSprite*terrain)
    //just remove terrain in terrainList, not remove it from scene tree
    {
        int nTerrain=(int)terrainList.size();
        for(int i=0;i<nTerrain;i++){
            CfractalTerrainSprite*_terrain=terrainList[i];
            if(_terrain==terrain){
                terrainList.erase(terrainList.begin()+i);
                i--;
                nTerrain--;
                return;
            }
        }
    }
public:
    void updateTerrain(CCRect cameraRect,CCNode*&fatherNode,b2World*world,float wantedYmin)
    //if all terrain on the left side of cameraRect's right edge, then generate a terrain at right
    //if all terrain on the right side of cameraRect's left edge, then generate a terrain at left
    //if have two or more terrain on the right side of cameraRect's right edge, then only reserve the very left one and delete others
    //if have two or more terrain on the left side of cameraRect's left edge, then only reserve the very right one and delete others
    {
        const float segLen=128;
        if(terrainList.empty()){//if no terrain yet
            //generate a terrain in cameraRect (startPoint in cameraRect), with random length
            float terrainLen=1500;//randomInRange(500, 1500);
            CCPoint startPoint=CCPoint(cameraRect.getMinX()+random01()*(cameraRect.getMaxX()-cameraRect.getMinX()),//random positionX in cameraRect
                                       0);
            CCPoint endPoint=CCPoint(startPoint.x+terrainLen,0);
            CfractalTerrainSprite*terrain=new CfractalTerrainSprite();
            terrain->autorelease();
            terrain->init();
            terrain->setStartPoint(startPoint);
            terrain->setEndPoint(endPoint);
            terrain->setNGenerationBySegLen(segLen);//must called after setStartPoint and setEndPoint
            terrain->setK_offset(randomInRange(0.1, 0.2));
            terrain->genSeges();//must called afther other settings setted
            terrain->genBodyAndMesh(world,wantedYmin);
            fatherNode->addChild(terrain,100);
            terrainList.push_back(terrain);
        }else{//if have terrain in list
            //calculate range of terrainList
            float minX=INFINITY;
            float maxX=-INFINITY;
            {
                int nTerrain=(int)terrainList.size();
                for(int i=0;i<nTerrain;i++){
                    CfractalTerrainSprite*terrain=terrainList[i];
                    CCPoint startPoint=terrain->getStartPoint();
                    CCPoint endPoint=terrain->getEndPoint();
                    assert(startPoint.x<endPoint.x);
                    if(startPoint.x<minX)minX=startPoint.x;
                    if(endPoint.x>maxX)maxX=endPoint.x;
                }//got minX and maxX
            }
            assert(minX<maxX);
            if(maxX<cameraRect.getMaxX()){//all terrain on left side of cameraRect's right edge
                //generate a terrain on the right of maxX
                float whiteSpace=0;//randomInRange(0, 500);//the space between new terrain's startPointX and maxX
                float terrainLen=1500;//randomInRange(500, 1500);
                CCPoint startPoint=CCPoint(maxX+whiteSpace,0);
                CCPoint endPoint=CCPoint(startPoint.x+terrainLen,0);
                CfractalTerrainSprite*terrain=new CfractalTerrainSprite();
                terrain->autorelease();
                terrain->init();
                terrain->setStartPoint(startPoint);
                terrain->setEndPoint(endPoint);
                terrain->setNGenerationBySegLen(segLen);
                terrain->setK_offset(randomInRange(0.1, 0.2));
                terrain->genSeges();
                terrain->genBodyAndMesh(world,wantedYmin);
                fatherNode->addChild(terrain,100);
                terrainList.push_back(terrain);
            }
            if(minX>cameraRect.getMinX()){//all terrain on right side of cameraRect's left edge
                //generate a terrain on the left of minX
                float whiteSpace=0;//randomInRange(0, 500);//the space between new terrain's endPointX and minX
                float terrainLen=1500;//randomInRange(500, 1500);
                CCPoint endPoint=CCPoint(minX-whiteSpace,0);
                CCPoint startPoint=CCPoint(endPoint.x-terrainLen,0);
                CfractalTerrainSprite*terrain=new CfractalTerrainSprite();
                terrain->autorelease();
                terrain->init();
                terrain->setStartPoint(startPoint);
                terrain->setEndPoint(endPoint);
                terrain->setNGenerationBySegLen(segLen);
                terrain->setK_offset(randomInRange(0.1, 0.2));
                terrain->genSeges();
                terrain->genBodyAndMesh(world,wantedYmin);
                fatherNode->addChild(terrain,100);
                terrainList.push_back(terrain);
            }
            vector<CfractalTerrainSprite*> rightTerrainList;
            vector<CfractalTerrainSprite*> leftTerrainList;
            {
                int nTerrain=(int)terrainList.size();
                for(int i=0;i<nTerrain;i++){
                    CfractalTerrainSprite*terrain=terrainList[i];
                    CCPoint startPoint=terrain->getStartPoint();
                    CCPoint endPoint=terrain->getEndPoint();
                    assert(startPoint.x<endPoint.x);
                    if(startPoint.x>cameraRect.getMaxX())rightTerrainList.push_back(terrain);
                    if(endPoint.x<cameraRect.getMinX())leftTerrainList.push_back(terrain);
                }//got rightTerrainList and leftTerrainList
            }
            //find the very left terrain in rightTerrainList, reserve it and delete others
            if(rightTerrainList.empty()==false)
            {
                CfractalTerrainSprite* veryLeftTerrain=NULL;
                float minStartX=INFINITY;
                int nRightTerrain=(int)rightTerrainList.size();
                for(int i=0;i<nRightTerrain;i++){
                    CfractalTerrainSprite* terrain=rightTerrainList[i];
                    if(terrain->getStartPoint().x<minStartX){
                        minStartX=terrain->getStartPoint().x;
                        veryLeftTerrain=terrain;
                    }
                }//got veryLeftTerrain
                assert(veryLeftTerrain);
                //delete all terrains in rightTerrainList except veryLeftTerrain
                for(int i=0;i<nRightTerrain;i++){
                    CfractalTerrainSprite* terrain=rightTerrainList[i];
                    if(terrain!=veryLeftTerrain){
                        removeTerrainInTerrainList(terrain);
                        terrain->removeFromParentAndCleanup(true);
                    }
                }
            }
            //find the very right terrain in leftTerrainList, reserve it and delete others
            if(leftTerrainList.empty()==false)
            {
                CfractalTerrainSprite* veryRightTerrain=NULL;
                float maxEndX=-INFINITY;
                int nLeftTerrain=(int)leftTerrainList.size();
                for(int i=0;i<nLeftTerrain;i++){
                    CfractalTerrainSprite* terrain=leftTerrainList[i];
                    if(terrain->getEndPoint().x>maxEndX){
                        maxEndX=terrain->getEndPoint().x;
                        veryRightTerrain=terrain;
                    }
                }//got veryLeftTerrain
                assert(veryRightTerrain);
                //delete all terrains in leftTerrainList except veryRightTerrain
                for(int i=0;i<nLeftTerrain;i++){
                    CfractalTerrainSprite* terrain=leftTerrainList[i];
                    if(terrain!=veryRightTerrain){
                        removeTerrainInTerrainList(terrain);
                        terrain->removeFromParentAndCleanup(true);
                    }
                }
            
            }
        }
    }
    
};
#endif /* defined(__HelloCpp__infiniteTerrainManager__) */
