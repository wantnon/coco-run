//
//  water.h
//  HelloCpp
//
//  Created by ck02-084 on 13-12-16.
//
//

#ifndef __HelloCpp__water__
#define __HelloCpp__water__

#include <iostream>
#include <vector>
#include <string>
using namespace std;
#include "Box2D.h"
#include "cocos2d.h"
using namespace cocos2d;
#include "simpleClasses.h"
#include "indexVBO.h"
#include "myfunc.h"
class CwaterSurface{
public:
    vector<CCPoint> surfacePointList;
    vector<CCPoint> surfacePointList_back;
    float dx;
    float height;
    float r_press;
    CwaterSurface(){
        dx=10;
        height=0;
        r_press=60;
    }
    virtual~CwaterSurface(){
    }
    void makeSurfacePointList(CCRect cameraRect){
        float cameraRectWidth=cameraRect.getMaxX()-cameraRect.getMinX();
        float startx=cameraRect.getMinX()-cameraRectWidth/3;
        float endx=cameraRect.getMaxX()+cameraRectWidth/3;
        assert(endx>=startx);
        int nseg=ceilf((endx-startx)/dx);
        dx=(endx-startx)/nseg;//revise dx
        int nPoint=nseg+1;
        for(int i=0;i<nPoint;i++){
            float x=startx+i*dx;
            float y=height;
            CCPoint point(x,y);
            surfacePointList.push_back(point);
        }//got surfacePointList;
        //surfacePointList_back copy surfacePointList
        surfacePointList_back=surfacePointList;
    }
    void pressAtX(float x,float h_press)
    //if h_press>0 is press
    //if h_press<0 is pull up
    {
        if(x<surfacePointList[0].x||x>surfacePointList[(int)surfacePointList.size()-1].x)return;//x not fall on surface
        //press
        int nPoint=(int)surfacePointList.size();
        for(int i=0;i<nPoint;i++){
            CCPoint&point=surfacePointList[i];
            float r=fabsf(point.x-x);
            if(r>r_press)continue;
            point.y-=(cosf(r/r_press*M_PI)+1)/2*h_press;
        }

    }
    void update(CCRect cameraRect){
        //update surfacePointList's point range to fit cameraRect
        //remove point out of cameraRect and generate new point to fulfill empty space in cameraRect
        //collect all surfacePointList's points which is in cameraRect to pointList
        float cameraRectWidth=cameraRect.getMaxX()-cameraRect.getMinX();
        float startx=cameraRect.getMinX()-cameraRectWidth/3;
        float endx=cameraRect.getMaxX()+cameraRectWidth/3;
     //   CCLOG("startx,endx:%f,%f",startx,endx);
        assert(endx>=startx);
        {
            vector<CCPoint> pointList;
            int nPoint=(int)surfacePointList.size();
            for(int i=0;i<nPoint;i++){
                CCPoint point=surfacePointList[i];
                if(point.x>=startx&&point.x<=endx){
                    pointList.push_back(point);
                }
            }//got pointList
            //grow pointList, let fulfill range [startx,endx]
            //grow toward right
            vector<CCPoint> rightPoints;
            assert((int)pointList.size()>0);
            CCPoint lastPoint=pointList[(int)pointList.size()-1];
            for(float x=lastPoint.x+dx;x<endx;x+=dx){
                CCPoint point(x,height);
                rightPoints.push_back(point);
            }
            //grow toward left
            vector<CCPoint> leftPoints;
            assert((int)pointList.size()>0);
            CCPoint firstPoint=pointList[0];
            for(float x=firstPoint.x-dx;x>startx;x-=dx){
                CCPoint point(x,height);
                leftPoints.push_back(point);
            }
            //reverse leftPoints
            leftPoints=reverseList(leftPoints);
            //let surfacePointList=[leftPoints,pointList,rightPoints]
            surfacePointList=conectTwoList(conectTwoList(leftPoints,pointList),rightPoints);
        }
        //do same thing for surfacePointList_back
        {
            vector<CCPoint> pointList;
            int nPoint=(int)surfacePointList_back.size();
            for(int i=0;i<nPoint;i++){
                CCPoint point=surfacePointList_back[i];
                if(point.x>=startx&&point.x<=endx){
                    pointList.push_back(point);
                }
            }//got pointList
            //grow pointList, let fulfill range [startx,endx]
            //grow toward right
            vector<CCPoint> rightPoints;
            assert((int)pointList.size()>0);
            CCPoint lastPoint=pointList[(int)pointList.size()-1];
            for(float x=lastPoint.x+dx;x<endx;x+=dx){
                CCPoint point(x,height);
                rightPoints.push_back(point);
            }
            //grow toward left
            vector<CCPoint> leftPoints;
            assert((int)pointList.size()>0);
            CCPoint firstPoint=pointList[0];
            for(float x=firstPoint.x-dx;x>startx;x-=dx){
                CCPoint point(x,height);
                leftPoints.push_back(point);
            }
            //reverse leftPoints
            leftPoints=reverseList(leftPoints);
            //let surfacePointList_back=[leftPoints,pointList,rightPoints]
            surfacePointList_back=conectTwoList(conectTwoList(leftPoints, pointList), rightPoints);
            
        }
        
        
        
        //update surfacePointList's point height
        {
            int nPoint=(int)surfacePointList.size();
            for(int i=0;i<nPoint;i++){
                //start and end point not update
                if(i==0)continue;
                if(i==nPoint-1)continue;
             //   CCPoint&point=surfacePointList[i];
                CCPoint&pointf=surfacePointList[i-1];
                CCPoint&pointn=surfacePointList[i+1];
                CCPoint&point_back=surfacePointList_back[i];
                point_back.y=(pointf.y+pointn.y-point_back.y)*(1.0-1.0/10);
            }
            //switch surfacePointList and surfacePointList_back
            vector<CCPoint> temp=surfacePointList;
            surfacePointList=surfacePointList_back;
            surfacePointList_back=temp;
       
        }
    }
    void debugDraw(){
        //draw segList
        glLineWidth(2);
        ccPointSize(4);
        int nPoint=(int)surfacePointList.size();
        int nSeg=nPoint-1;
        for(int i=0;i<nSeg;i++){
            CCPoint point=surfacePointList[i];
            CCPoint pointn=surfacePointList[i+1];
            ccDrawLine(point, pointn);
            ccDrawPoint(point);
            ccDrawPoint(pointn);
        }
    }
    float getHeight()const {return height;};
    void setHeight(float _height){height=_height;};
    
};
class Cwater:public CCSprite
{
public:
    Cwater(){
    }
    virtual~Cwater(){};
    bool init(CCRect _cameraRect){
        cameraRect=_cameraRect;
        CCSprite::initWithFile("data/global/tex/water.png");
        this->setAnchorPoint(ccp(0,0));//must do this
        waterSurface.makeSurfacePointList(cameraRect);
        indexVBO.genBuffers();
        return true;
    }
    float getSurfaceHeight()const {return waterSurface.getHeight();};
    void setSurfaceHeight(float _surfaceHeight){waterSurface.setHeight(_surfaceHeight);};
    void updateCameraRect(CCRect _cameraRect){cameraRect=_cameraRect;};
    void updateMesh(){
        mesh.vlist.clear();
        mesh.texCoordList.clear();
        mesh.IDtriList.clear();
        const vector<CCPoint>&pointList=waterSurface.surfacePointList;
        float bottomY=-150;
        int nPoint=(int)pointList.size();
        for(int i=0;i<nPoint-1;i++){
            //   p--pn
            //    |       |
            //  _p--_pn
            const CCPoint&p=pointList[i];
            const CCPoint&pn=pointList[i+1];
            CCPoint _p=CCPoint(p.x,bottomY);
            CCPoint _pn=CCPoint(pn.x,bottomY);
            mesh.vlist.push_back(Cv2(p.x,p.y));int ID0=(int)mesh.vlist.size()-1;
            mesh.vlist.push_back(Cv2(_p.x,_p.y));int ID1=(int)mesh.vlist.size()-1;
            mesh.vlist.push_back(Cv2(_pn.x,_pn.y));int ID2=(int)mesh.vlist.size()-1;
            mesh.vlist.push_back(Cv2(pn.x,pn.y));int ID3=(int)mesh.vlist.size()-1;
            mesh.texCoordList.push_back(Cv2(0,0));
            mesh.texCoordList.push_back(Cv2(0,1));
            mesh.texCoordList.push_back(Cv2(1,1));
            mesh.texCoordList.push_back(Cv2(1,0));
            CIDTriangle IDtri0(ID0,ID1,ID2);
            CIDTriangle IDtri1(ID0,ID2,ID3);
            mesh.IDtriList.push_back(IDtri0);
            mesh.IDtriList.push_back(IDtri1);
        }
        //submit mesh to indexVBO
        {
            //submit data to VBO
            indexVBO.submitPos(mesh.vlist,GL_DYNAMIC_DRAW);//GL_DYNAMIC_DRAW
            indexVBO.submitTexCoord(mesh.texCoordList,GL_DYNAMIC_DRAW);
            indexVBO.submitIndex(mesh.IDtriList,GL_DYNAMIC_DRAW);
            //check gl error
            CHECK_GL_ERROR_DEBUG();
        }
        
    }
    void draw(){
       
        waterSurface.update(this->cameraRect);
        updateMesh();//call updateMesh after waterSurface.update
     //   waterSurface.debugDraw();
        //draw mesh
        this->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture));
        ccGLEnable(m_eGLServerState);
        //pass values for cocos2d-x build-in uniforms
        getShaderProgram()->use();
        getShaderProgram()->setUniformsForBuiltins();
        //----enable attribute arrays (must do every frame)
        CindexVBO::enableAttribArrays();
        //ccGLEnableVertexAttribs( kCCVertexAttribFlag_PosColorTex );
        //----bindTexture
        ccGLBindTexture2D( this->getTexture()->getName());//do not use glBindTexture2D, or next sprite use cocos2d-x default draw may be affected
        //----draw
        indexVBO.setPointers();
        indexVBO.draw(GL_TRIANGLES);
        //----unbindTexture
        ccGLBindTexture2D(0);//do not use glBindTexture2D, or next sprite use cocos2d-x default draw may be affected
        

    }
public:
    CwaterSurface waterSurface;
    CCRect cameraRect;
    Cmesh mesh;
    CindexVBO indexVBO;
    

};
#endif /* defined(__HelloCpp__water__) */
