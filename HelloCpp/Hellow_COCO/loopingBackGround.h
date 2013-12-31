//
//  loopingBackGround.h
//  HelloCpp
//
//  Created by ck02-084 on 13-12-16.
//
//

#ifndef __HelloCpp__loopingBackGround__
#define __HelloCpp__loopingBackGround__

#include <iostream>
#include <string>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "simpleClasses.h"
#include "indexVBO.h"
//var change texCoord achieve looping background effect
class CloopingBackGround:public CCNode
{
public:
    CloopingBackGround(){
        followee=NULL;
        x_old=0;
        texture=NULL;
    }
    virtual~CloopingBackGround(){
        CCTextureCache::sharedTextureCache()->removeTexture(texture);
    }
    bool init(string texFileName,float width){
        texture=CCTextureCache::sharedTextureCache()->addImage(texFileName.c_str());
        size=CCSize(width,texture->getContentSize().height);
        ccTexParams texParams={GL_LINEAR,GL_LINEAR,GL_REPEAT,GL_CLAMP_TO_EDGE};
        texture->setTexParameters(&texParams);
        this->setAnchorPoint(ccp(0,0));
        indexVBO.genBuffers();
        genMesh(size);
        submitMesh(GL_STATIC_DRAW);
        return true;
    }
    void texCoordOffsetAdd_s(float texOffset_s){
        Cv2&texCoord0=mesh.texCoordList[0];
        Cv2&texCoord1=mesh.texCoordList[1];
        Cv2&texCoord2=mesh.texCoordList[2];
        Cv2&texCoord3=mesh.texCoordList[3];
        texCoord0=Cv2(texCoord0.v[0]+texOffset_s, texCoord0.v[1]);
        texCoord1=Cv2(texCoord1.v[0]+texOffset_s, texCoord1.v[1]);
        texCoord2=Cv2(texCoord2.v[0]+texOffset_s, texCoord2.v[1]);
        texCoord3=Cv2(texCoord3.v[0]+texOffset_s, texCoord3.v[1]);
        //let texCoord0.v[0]~texCoord3.v[0] not grow too big
        float smin=MIN(MIN(MIN(texCoord0.v[0],texCoord1.v[0]),texCoord2.v[0]),texCoord3.v[0]);
        if(smin>0){
            int sminFloor=floorf(smin);
            texCoord0.v[0]-=sminFloor;
            texCoord1.v[0]-=sminFloor;
            texCoord2.v[0]-=sminFloor;
            texCoord3.v[0]-=sminFloor;
        }
        //submit texCoord to indexVBO
        indexVBO.submitTexCoord(mesh.texCoordList,GL_DYNAMIC_DRAW);
        
    }
    void setFollowee(CCNode*node){
        followee=node;
    }
    void setFollowOffset_x(float offset){
        followOffset_x=offset;
    }
    void submitMesh(GLenum usage){//submit mesh to indexVBO
        //submit data to VBO
        indexVBO.submitPos(mesh.vlist,usage);
        indexVBO.submitTexCoord(mesh.texCoordList,usage);
        indexVBO.submitIndex(mesh.IDtriList,usage);
        //check gl error
        CHECK_GL_ERROR_DEBUG();
    }
    void update(float t){
        x_old=this->getPositionX();
        this->setPositionX(followee->getPositionX()+followOffset_x);
        //calculate delta x of *this between last frame and current frame
        float dx=this->getPositionX()-x_old;
        float ds=dx/size.width;
        this->texCoordOffsetAdd_s(ds);
    }
    void genMesh(CCSize size){
        //generate mesh
        //generate a quad mesh use local origin as the center and contentSize as the size
        {
            float w=size.width;
            float h=size.height;
            //  v3          v2
            //  -------------
            //  |           |
            //  | h         |    v0 is at the local (0,0)
            //  |     w     |
            //  -------------
            //  v0          v1
            CCPoint v0=CCPoint(0,0);
            CCPoint v1=CCPoint(w,0);
            CCPoint v2=CCPoint(w,h);
            CCPoint v3=CCPoint(0,h);
            mesh.vlist.push_back(Cv2(v0.x,v0.y));
            mesh.vlist.push_back(Cv2(v1.x,v1.y));
            mesh.vlist.push_back(Cv2(v2.x,v2.y));
            mesh.vlist.push_back(Cv2(v3.x,v3.y));
            CIDTriangle IDtri0(0,1,2);
            CIDTriangle IDtri1(0,2,3);
            mesh.IDtriList.push_back(IDtri0);
            mesh.IDtriList.push_back(IDtri1);
            float sMax=size.width/texture->getContentSize().width;
            mesh.texCoordList.push_back(Cv2(0, 1));
            mesh.texCoordList.push_back(Cv2(sMax, 1));
            mesh.texCoordList.push_back(Cv2(sMax, 0));
            mesh.texCoordList.push_back(Cv2(0, 0));
            
        }
    }
    virtual void draw(){
        //draw mesh
        //----change shader
        //search "ccShader" can get all cocos2d-x built-in shader files
        this->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture));//if use kCCShader_PositionTextureColor, will get a black result
        ccGLEnable(m_eGLServerState);
        //pass values for cocos2d-x build-in uniforms
        getShaderProgram()->use();
        getShaderProgram()->setUniformsForBuiltins();
        //----enable attribute arrays (must do every frame)
        CindexVBO::enableAttribArrays();
        //ccGLEnableVertexAttribs( kCCVertexAttribFlag_PosColorTex );
        //----bindTexture
        ccGLBindTexture2D(texture->getName());//do not use glBindTexture2D, or next sprite use cocos2d-x default draw may be affected
        //----draw
        indexVBO.setPointers();
        indexVBO.draw(GL_TRIANGLES);
        //----unbindTexture
        ccGLBindTexture2D(0);//do not use glBindTexture2D, or next sprite use cocos2d-x default draw may be affected
    }
public:
    CCTexture2D*texture;
    Cmesh mesh;
    CindexVBO indexVBO;
    CCNode*followee;//the node to be followed by *this, *this's x component will follow followee's x component
    float followOffset_x;//*this->x=followee->x+followOffset_x
    float x_old;//*this's old x
    CCSize size;//size of *this

};
#endif /* defined(__HelloCpp__loopingBackGround__) */
