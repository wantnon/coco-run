//
//  fractalTerrain.cpp
//  HelloCpp
//
//  Created by apple on 13-12-15.
//
//

#include "fractalTerrain.h"


bool CfractalTerrainSprite::init(){
    CCSprite::init();
    setAnchorPoint(ccp(0,0));
    indexVBO.genBuffers();
    indexVBO_verySmooth.genBuffers();
    texSprite=new CCSprite();
    texSprite_verySmooth=new CCSprite();
    texSprite->initWithFile("data/global/tex/ground.png");

    texSprite_verySmooth->initWithFile("data/global/tex/ground_verySmooth.png");
    ccTexParams texParams={GL_LINEAR,GL_LINEAR,GL_REPEAT,GL_REPEAT};
    texSprite_verySmooth->getTexture()->setTexParameters(&texParams);
    return true;
}
void CfractalTerrainSprite::genBodyAndMesh(b2World*_world,float wantedYmin)
//if wantedYmin is not low enough, the program will change it to a lower value
{
    if(world==NULL){
        world=_world;
    }else{
        assert(world==_world);
    }
    //
    vector<CCPoint> pointList;
    vector<CCPoint> pointList_detail;
    vector<CCPoint> pointList_detail_verySmooth;
    //convert segList to pointList

    {
        int nseg=(int)segList.size();
        for(int i=0;i<nseg;i++){
            ClineSeg*seg=segList[i];
            pointList.push_back(seg->startPoint);
        }
        pointList.push_back(segList[(int)segList.size()-1]->endPoint);//got pointList
    }
    //continue do n generation for segList
    const int n=3;
    for(int i=nGeneration+1;i<=nGeneration+n;i++){
        doGenerationOnce(segList, i);
    }
    
    //convert segList to pointList_detail
   
    {
        int nseg=(int)segList.size();
        for(int i=0;i<nseg;i++){
            ClineSeg*seg=segList[i];
            pointList_detail.push_back(seg->startPoint);
        }
        pointList_detail.push_back(segList[(int)pointList_detail.size()-1]->endPoint);//got pointList
    }
    //let pointList_detail_verySmooth equals to current point_detail
    pointList_detail_verySmooth=pointList_detail;
    //fliter pointList_detail to let the tangent of start and end close to horizontal while keep the start and end point not move
    {
        int R=8;
        R=MIN(R,(int)pointList_detail.size());//R should not bigger than pointList_detail.size()
        float k=1;
        for(int i=1;i<=R;i++){
            CCPoint&point=pointList_detail[i];
            CCPoint pointf=pointList_detail[i-1];
            if(point.y==wantedYmin)continue;
            point.y+=(pointf.y-point.y)*k;
            k-=1.0/R;
        }
        int nPoint=(int)pointList_detail.size();
        k=1;
        for(int i=nPoint-2;i>=nPoint-1-R;i--){
            CCPoint&point=pointList_detail[i];
            CCPoint pointn=pointList_detail[i+1];
            if(point.y==wantedYmin)continue;
            point.y+=(pointn.y-point.y)*k;
            k-=1.0/R;
        }
        
        
    }
    //fliter pointList_detail_verySmooth to let the tangent of start and end close to horizontal while keep the start and end point not move
    {
        vector<CCPoint>&pointList=pointList_detail_verySmooth;
        int R=15;
        R=MIN(R,(int)pointList.size());//R should not bigger than pointList.size()
        float k=1;
        for(int i=1;i<=R;i++){
            CCPoint&point=pointList[i];
            CCPoint pointf=pointList[i-1];
            if(point.y==wantedYmin)continue;
            point.y+=(pointf.y-point.y)*k;
            k-=1.0/R;
        }
        int nPoint=(int)pointList.size();
        k=1;
        for(int i=nPoint-2;i>=nPoint-1-R;i--){
            CCPoint&point=pointList[i];
            CCPoint pointn=pointList[i+1];
            if(point.y==wantedYmin)continue;
            point.y+=(pointn.y-point.y)*k;
            k-=1.0/R;
        }
        
        
    }


    

    //fliter pointList_detail to let it looks more smooth and keep start and end not move
    {
        const int fliterR=2;
        int nPoint_detail=(int)pointList_detail.size();
        for(int i=0;i<nPoint_detail;i++){
            if(pointList_detail[i].y==wantedYmin)continue;
            float y=0;
            int count=0;
            int R=MIN(fliterR, MIN(i, nPoint_detail-1-i));
            for(int j=i-R;j<=i+R;j++){
                const CCPoint&_point=pointList_detail[j];
                y+=_point.y;
                count++;
            }
            assert(count>0);
            y/=count;
            pointList_detail[i].y=y;
        }
        
    }

    //fliter pointList_detail_verySmooth to let it looks more smooth and keep start and end not move
    {
        {
            const int fliterR=5;
            int nPoint=(int)pointList_detail_verySmooth.size();
            for(int i=0;i<nPoint;i++){
                if(pointList_detail_verySmooth[i].y==wantedYmin)continue;
                float y=0;
                int count=0;
                int R=MIN(fliterR, MIN(i, nPoint-1-i));
                for(int j=i-R;j<=i+R;j++){
                    const CCPoint&_point=pointList_detail_verySmooth[j];
                    y+=_point.y;
                    count++;
                }
                assert(count>0);
                y/=count;
                pointList_detail_verySmooth[i].y=y;
            }
        }
        {
            const int fliterR=5;
            int nPoint=(int)pointList_detail_verySmooth.size();
            for(int i=0;i<nPoint;i++){
                if(pointList_detail_verySmooth[i].y==wantedYmin)continue;
                float y=0;
                int count=0;
                int R=MIN(fliterR, MIN(i, nPoint-1-i));
                for(int j=i-R;j<=i+R;j++){
                    const CCPoint&_point=pointList_detail_verySmooth[j];
                    y+=_point.y;
                    count++;
                }
                assert(count>0);
                y/=count;
                pointList_detail_verySmooth[i].y=y;
            }

        }
        
    }
 /*   //scale pointList_detail_verySmooth
    {
        int nPoint=(int)pointList_detail_verySmooth.size();
        for(int i=0;i<nPoint;i++){
            if(pointList_detail_verySmooth[i].y==wantedYmin)continue;
            pointList_detail_verySmooth[i].y=wantedYmin+(pointList_detail_verySmooth[i].y-wantedYmin)*(4.0/5);
        }

        
    }*/
    //move pointList_detail_verySmooth a litte down
    {
        int nPoint=(int)pointList_detail_verySmooth.size();
        for(int i=0;i<nPoint;i++){
            pointList_detail_verySmooth[i].y-=35;
        }
        
        
    }
    //let all points in pointList and pointList_detail and pointList_detail_verySmooth not lower than wantedYmin
    {
        int nPoint=(int)pointList.size();
        for(int i=0;i<nPoint;i++){
            pointList[i].y=MAX(pointList[i].y, wantedYmin);
        }
        int nPoint_detail=(int)pointList_detail.size();
        for(int i=0;i<nPoint_detail;i++){
            pointList_detail[i].y=MAX(pointList_detail[i].y, wantedYmin);
        }
        int nPoint_detail_verySmooth=(int)pointList_detail_verySmooth.size();
        for(int i=0;i<nPoint_detail_verySmooth;i++){
            pointList_detail_verySmooth[i].y=MAX(pointList_detail_verySmooth[i].y, wantedYmin);
        }
    }
    
    
    
    //gen polygonList based on pointList
    vector<Cpolygon> polygonList;
    {
        int nPoint=(int)pointList.size();
        for(int i=0;i<nPoint-1;i++){
            CCPoint point=pointList[i];
            CCPoint pointn=pointList[i+1];
            //gen quad or triangle for seg:[point,pointn]
            //
            //   v3 ---  v2
            //        |      |      v3 and v0 may coincide to one point
            //        |___|      v2 and v1 may coincide to one point
            //     v0     v1
            CCPoint v0=CCPoint(point.x,wantedYmin);
            CCPoint v1=CCPoint(pointn.x,wantedYmin);
            CCPoint v2=pointn;
            CCPoint v3=point;
            Cpolygon polygon;
            polygon.vlist.push_back(v0);
            polygon.vlist.push_back(v1);
            polygon.vlist.push_back(v2);
            polygon.vlist.push_back(v3);
            polygonList.push_back(polygon);
        }//got polygonList
    }
    //gen polygonList_detail based on pointList_detail
    vector<Cpolygon> polygonList_detail;
    {
        int nPoint_detail=(int)pointList_detail.size();
        for(int i=0;i<nPoint_detail-1;i++){
            CCPoint point=pointList_detail[i];
            CCPoint pointn=pointList_detail[i+1];
            //gen quad or triangle for seg:[point,pointn]
            //
            //   v3 ---  v2
            //        |      |      v3 and v0 may coincide to one point
            //        |___|      v2 and v1 may coincide to one point
            //     v0     v1
            CCPoint v0=CCPoint(point.x,wantedYmin);
            CCPoint v1=CCPoint(pointn.x,wantedYmin);
            CCPoint v2=pointn;
            CCPoint v3=point;
            Cpolygon polygon;
            polygon.vlist.push_back(v0);
            polygon.vlist.push_back(v1);
            polygon.vlist.push_back(v2);
            polygon.vlist.push_back(v3);
            polygonList_detail.push_back(polygon);
        }//got polygonList_detail
    }
    //gen polygonList_detail_verySmooth based on pointList_detail
    vector<Cpolygon> polygonList_detail_verySmooth;
    {
        int nPoint=(int)pointList_detail_verySmooth.size();
        for(int i=0;i<nPoint-1;i++){
            CCPoint point=pointList_detail_verySmooth[i];
            CCPoint pointn=pointList_detail_verySmooth[i+1];
            //gen quad or triangle for seg:[point,pointn]
            //
            //   v3 ---  v2
            //        |      |      v3 and v0 may coincide to one point
            //        |___|      v2 and v1 may coincide to one point
            //     v0     v1
            CCPoint v0=CCPoint(point.x,wantedYmin);
            CCPoint v1=CCPoint(pointn.x,wantedYmin);
            CCPoint v2=pointn;
            CCPoint v3=point;
            Cpolygon polygon;
            polygon.vlist.push_back(v0);
            polygon.vlist.push_back(v1);
            polygon.vlist.push_back(v2);
            polygon.vlist.push_back(v3);
            polygonList_detail_verySmooth.push_back(polygon);
        }//got polygonList_detail_verySmooth
    }


    
    //----gen body based on polygonList
    //destoryBody
    if(pb2Body)world->DestroyBody(pb2Body);
    fixtureList.clear();
    //make polygons in polygonList fit box2d's requirement
    {
        vector<Cpolygon> tpolygonList;
        int nPolygon=(int)polygonList.size();
        for(int i=0;i<nPolygon;i++){
            Cpolygon polygon=polygonList[i];
            Cpolygon _polygon;//remove coincide points in polygon we get _polygon
            int nv=(int)polygon.vlist.size();
            for(int j=0;j<nv;j++){
                const CCPoint&p=polygon.vlist[j];//current point
                const CCPoint&pn=polygon.vlist[(j+1)%nv];//next point
                if(pointEqual(p, pn, 0.001)){
                    //skip current point
                    continue;
                }else{
                    _polygon.vlist.push_back(p);
                }
            }//got _polygon
            if((int)_polygon.vlist.size()<3)continue;//_polygon's vertex count must >3
            //use _polygon replace polygon
            polygon=_polygon;
            tpolygonList.push_back(polygon);
        }
        polygonList=tpolygonList;
    }
    // Define the dynamic body.
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    b2Body *body = world->CreateBody(&bodyDef);
    // Define shape
    int nPolygon=(int)polygonList.size();
    for(int i=0;i<nPolygon;i++){
        const Cpolygon&polygon=polygonList[i];
        //gen shape based on polygon
        b2PolygonShape polygonShape;
        int nv=(int)polygon.vlist.size();
        assert(nv<=8);
        b2Vec2 verts[8];
        for(int i=0;i<nv;i++){
            const CCPoint&p=polygon.vlist[i];
            verts[i].Set(p.x/PTM_RATIO,p.y/PTM_RATIO);
        }
        polygonShape.Set(verts, nv);
        //fixture
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &polygonShape;
        fixtureDef.density = 0;
        fixtureDef.friction = 0.5;
        fixtureDef.isSensor=false;
        //fixtureDef.filter=;
        b2Fixture*fixture=body->CreateFixture(&fixtureDef);
        fixtureList.push_back(fixture);
    }
    //pb2Body
    pb2Body=body;
    //-----gen mesh based on polygonList_detail
    {
        mesh.vlist.clear();
        mesh.texCoordList.clear();
        mesh.IDtriList.clear();
        int nPolygon=(int)polygonList_detail.size();
        for(int i=0;i<nPolygon;i++){
            const Cpolygon&polygon=polygonList_detail[i];
            assert((int)polygon.vlist.size()==4);
            if(pointEqual(polygon.vlist[0], polygon.vlist[3], 5)
               &&pointEqual(polygon.vlist[1], polygon.vlist[2], 5)){//polygon collapse to a line, or too small, skip it
                continue;
            }
            mesh.vlist.push_back(Cv2(polygon.vlist[0].x, polygon.vlist[0].y));int ID0=mesh.vlist.size()-1;
            mesh.vlist.push_back(Cv2(polygon.vlist[1].x, polygon.vlist[1].y));int ID1=ID0+1;
            mesh.vlist.push_back(Cv2(polygon.vlist[2].x, polygon.vlist[2].y));int ID2=ID1+1;
            mesh.vlist.push_back(Cv2(polygon.vlist[3].x, polygon.vlist[3].y));int ID3=ID2+1;
            mesh.texCoordList.push_back(Cv2(0, 1));
            mesh.texCoordList.push_back(Cv2(1, 1));
            mesh.texCoordList.push_back(Cv2(1, 0));
            mesh.texCoordList.push_back(Cv2(0, 0));
            CIDTriangle IDtri0(ID0,ID1,ID2);
            CIDTriangle IDtri1(ID0,ID2,ID3);
            mesh.IDtriList.push_back(IDtri0);
            mesh.IDtriList.push_back(IDtri1);
        }
        
        //submit mesh to indexVBO
        {
            //submit data to VBO
            CHECK_GL_ERROR_DEBUG();
            indexVBO.submitPos(mesh.vlist,GL_STATIC_DRAW);//GL_DYNAMIC_DRAW
            CHECK_GL_ERROR_DEBUG();
            indexVBO.submitTexCoord(mesh.texCoordList,GL_STATIC_DRAW);
            CHECK_GL_ERROR_DEBUG();
            indexVBO.submitIndex(mesh.IDtriList,GL_STATIC_DRAW);
            //check gl error
            CHECK_GL_ERROR_DEBUG();
        }
        
        
        
    }
    //-----gen mesh_verySmooth based on polygonList_detail_verySmooth
    {
        Cmesh&mesh=mesh_verySmooth;
        mesh.vlist.clear();
        mesh.texCoordList.clear();
        mesh.IDtriList.clear();
        int nPolygon=(int)polygonList_detail_verySmooth.size();
        for(int i=0;i<nPolygon;i++){
            const Cpolygon&polygon=polygonList_detail_verySmooth[i];
            assert((int)polygon.vlist.size()==4);
            if(pointEqual(polygon.vlist[0], polygon.vlist[3], 5)
               &&pointEqual(polygon.vlist[1], polygon.vlist[2], 5)){//polygon collapse to a line, or too small, skip it
                continue;
            }
            mesh.vlist.push_back(Cv2(polygon.vlist[0].x, polygon.vlist[0].y));int ID0=mesh.vlist.size()-1;
            mesh.vlist.push_back(Cv2(polygon.vlist[1].x, polygon.vlist[1].y));int ID1=ID0+1;
            mesh.vlist.push_back(Cv2(polygon.vlist[2].x, polygon.vlist[2].y));int ID2=ID1+1;
            mesh.vlist.push_back(Cv2(polygon.vlist[3].x, polygon.vlist[3].y));int ID3=ID2+1;
       /*     mesh.texCoordList.push_back(Cv2(polygon.vlist[0].x*0.02, polygon.vlist[0].y*0.02));
            mesh.texCoordList.push_back(Cv2(polygon.vlist[1].x*0.02, polygon.vlist[1].y*0.02));
            mesh.texCoordList.push_back(Cv2(polygon.vlist[2].x*0.02, polygon.vlist[2].y*0.02));
            mesh.texCoordList.push_back(Cv2(polygon.vlist[3].x*0.02, polygon.vlist[3].y*0.02));*/
            mesh.texCoordList.push_back(Cv2(0, 1));
            mesh.texCoordList.push_back(Cv2(1, 1));
            mesh.texCoordList.push_back(Cv2(1, 0));
            mesh.texCoordList.push_back(Cv2(0, 0));
            
            CIDTriangle IDtri0(ID0,ID1,ID2);
            CIDTriangle IDtri1(ID0,ID2,ID3);
            mesh.IDtriList.push_back(IDtri0);
            mesh.IDtriList.push_back(IDtri1);
        }
        
        //submit mesh to indexVBO
        {
            Cmesh&mesh=mesh_verySmooth;
            CindexVBO&indexVBO=indexVBO_verySmooth;
            //submit data to VBO
            CHECK_GL_ERROR_DEBUG();
            indexVBO.submitPos(mesh.vlist,GL_STATIC_DRAW);//GL_DYNAMIC_DRAW
            CHECK_GL_ERROR_DEBUG();
            indexVBO.submitTexCoord(mesh.texCoordList,GL_STATIC_DRAW);
            CHECK_GL_ERROR_DEBUG();
            indexVBO.submitIndex(mesh.IDtriList,GL_STATIC_DRAW);
            //check gl error
            CHECK_GL_ERROR_DEBUG();
        }
        
        
        
    }

}
void CfractalTerrainSprite::setNGenerationBySegLen(float segLen){
    float len=2*ccpDistance(startPoint, endPoint)*sqrtf(0.25+k_offset*k_offset);
    int count=0;
    while(len>segLen){
        len/=2;
        count++;
    }//count is the smallest number let mainSegLen*2^count not less than len
    nGeneration=count;
      CCLOG("nGeneration:%i",nGeneration);
}

vector<ClineSeg*> CfractalTerrainSprite::splitSeg(ClineSeg*seg,int generationID)
{
    //  *-------seg-------*
    //  *--seg0--*--seg1--*
    //
    vector<ClineSeg*> seges;
    float maxOffset=seg->getLen()*k_offset*nPow(0.8, generationID-1);
    float offset=randomInRange(-maxOffset, maxOffset);
    CCPoint midPoint=seg->getMidPoint()+ccpMult(seg->getRightNormal(),offset);
    ClineSeg* seg0=new ClineSeg(seg->startPoint,midPoint);
    ClineSeg* seg1=new ClineSeg(midPoint,seg->endPoint);
    //update links of seg0, seg1
    seg0->father=seg->father;
    seg0->child=seg1;
    seg1->father=seg0;
    seg1->child=seg->child;
    //update links of seg's neighbour
    if(seg->father){
        seg->father->child=seg0;
    }
    if(seg->child){
        seg->child->father=seg1;
    }
    //add seg0,seg1 to seges, notice the order, if we keep this order, we can guarantee segList[0] is headSeg and segList[(int)segList.size()-1] is tailSeg.
    //add seg0 to seges
    seges.push_back(seg0);
    //add seg1 to seges
    seges.push_back(seg1);
    return seges;
}
void CfractalTerrainSprite::doGenerationOnce(vector<ClineSeg*>&segList,int generationID){
    vector<ClineSeg*> tSegList;
    //split each seg in segList
    int nseg=(int)segList.size();
    for(int j=0;j<nseg;j++){
        ClineSeg*seg=segList[j];//cur seg
        vector<ClineSeg*> subseges=splitSeg(seg,generationID);//split seg, get sub seges
        //add sub seges to segList
        int nSubSeg=(int)subseges.size();
        for(int k=0;k<nSubSeg;k++){
            tSegList.push_back(subseges[k]);
        }
    }
    //clear segList
    for(int j=0;j<nseg;j++){
        delete segList[j];
        segList[j]=NULL;
    }
    segList.clear();
    //
    segList=tSegList;

}
void CfractalTerrainSprite::genSeges(){
    //clear segList
    int nSeg=(int)segList.size();
    for(int i=0;i<nSeg;i++){
        delete segList[i];
        segList[i]=NULL;
    }
    segList.clear();
    //create inital branchSeg
    ClineSeg* branchSeg=new ClineSeg(startPoint,endPoint);
    //use inital branchSeg to create segList
    //use branchSeg as inital seg
    segList.push_back(branchSeg);//branchSeg will be deleted in the loop below
    //begin generations
    for(int i=1;i<=nGeneration;i++){
        doGenerationOnce(segList, i);
    }
    //get headSeg and tailSeg
    headSeg=segList[0];
    tailSeg=segList[(int)segList.size()-1];
    assert(headSeg->father==NULL);
    assert(tailSeg->child==NULL);
    
    
}

void CfractalTerrainSprite::draw(){
 

    //----draw mesh
    this->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture));
    ccGLEnable(m_eGLServerState);
    //pass values for cocos2d-x build-in uniforms
    getShaderProgram()->use();
    getShaderProgram()->setUniformsForBuiltins();
    //----enable attribute arrays (must do every frame)
    CindexVBO::enableAttribArrays();
    //ccGLEnableVertexAttribs( kCCVertexAttribFlag_PosColorTex );
    //----bindTexture
    ccGLBindTexture2D( texSprite->getTexture()->getName());//do not use glBindTexture2D, or next sprite use cocos2d-x default draw may be affected
    //----draw
    indexVBO.setPointers();
    indexVBO.draw(GL_TRIANGLES);
    

    ccGLBindTexture2D( texSprite_verySmooth->getTexture()->getName());//do not use glBindTexture2D, or next sprite use cocos2d-x default draw may be affected
    //----draw
    indexVBO_verySmooth.setPointers();
    indexVBO_verySmooth.draw(GL_TRIANGLES);
    //----unbindTexture
    ccGLBindTexture2D(0);//do not use glBindTexture2D, or next sprite use cocos2d-x default draw may be affected
/*
    //draw segList
    glLineWidth(2);
    ccPointSize(5);
    int nSeg=(int)segList.size();
    for(int i=0;i<nSeg;i++){
        ClineSeg*seg=segList[i];
        ccDrawLine(seg->startPoint, seg->endPoint);
        ccDrawPoint(seg->startPoint);
        ccDrawPoint(seg->endPoint);
    }
   */ 
}
