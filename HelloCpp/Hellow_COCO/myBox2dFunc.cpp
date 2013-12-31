//
//  myBox2dFunc.cpp
//  mybox2dtest
//
//  Created by apple on 13-7-23.
//
//

#include "myBox2dFunc.h"
b2Vec2 getAveragePoint(const vector<b2Vec2>&b2PointList){//ªÒµ√æ˘÷µµ„
	int npoint=b2PointList.size();
	assert(npoint!=0);
	b2Vec2 c(0,0);//“ª∂®“™≥ı ºªØ£¨∑Ò‘Úª· «ÀÊª˙÷µ
	for(int i=0;i<npoint;i++){
		const b2Vec2&v=b2PointList[i];
		c+=v;
	}
	c*=1.0/npoint;//µ√µΩwc
	return c;
}
b2AABB getAABB(b2Shape*shape,const b2Transform&transform){
	b2AABB aabb;
	int32 childIndex = 0;
	shape->ComputeAABB(&aabb, transform, childIndex);
	return aabb;
}
int getFixtureCount(b2Body*body){
	int count=0;
	for (b2Fixture* f =body->GetFixtureList(); f; f = f->GetNext())
	{
		count++;
	}
	return count;
}
b2Fixture* getFixture(b2Body*body,int index){
    //why iterate fixtureList backward? see:http://user.qzone.qq.com/350479720/blog/1381042354
	const int fixtureCount=getFixtureCount(body);
	const int index_forwards=fixtureCount-1-index;
	int i=0;
	for (b2Fixture* f =body->GetFixtureList(); f; f = f->GetNext())
	{
		if(i==index_forwards)return f;
		i++;
	}
	return NULL;
    
}
b2BodyType bodyTypeStrTobodyTypeValue(string bodyTypeStr){
	if(bodyTypeStr=="staticBody"){
		return b2_staticBody;
	}else if(bodyTypeStr=="kinematicBody"){
		return b2_kinematicBody;
	}else if(bodyTypeStr=="dynamicBody"){
		return b2_dynamicBody;
	}else{
		cout<<"error£∫Œ¥÷™bodyTypeStr:"<<bodyTypeStr<<endl;
		system("pause");
		exit(0);
	}
    
}
void splitPolygonShapeWithHoriLine(b2PolygonShape&shape,b2Transform transform,float H_horiLine,
                                   bool&upperShapeGot,bool&lowerShapeGot,
                                   b2PolygonShape&upperShape,b2PolygonShape&lowerShape){// π”√ÀÆ∆Ω÷±œﬂ«–∏Óshape
    
    
    //≈–∂œ «∑ÒŒ™polygon shape
    if(shape.GetType()!=b2Shape::e_polygon){
        cout<<"error:shape¿‡–Õ≤ª «e_polygon!"<<endl;
        exit(0);
    }
    //«Ûshapeµƒaabb
    b2AABB aabb;
    {
        int32 childIndex = 0;
        shape.ComputeAABB(&aabb, transform, childIndex);
        //      cout<<"aabb range:"<<aabb.lowerBound.x<<" "<<aabb.upperBound.x<<endl;
    }
    //ÀÆ∆Ωœﬂ”Îshape «∑Òœ‡Ωª
    bool intersect=true;
    //«Û◊ÛΩªµ„
    b2Vec2 hitPoint_left;
    bool hit_left;
    const float margin=1.0;
    {
        b2Vec2 p1(aabb.lowerBound.x-margin ,H_horiLine);
        b2Vec2 p2(aabb.upperBound.x+margin,H_horiLine);
        b2RayCastInput input;
        input.p1=p1;
        input.p2=p2;
        input.maxFraction =1;;
        int32 childIndex = 0;
        b2RayCastOutput output;
        hit_left = shape.RayCast(&output, input, transform, childIndex);
        if (hit_left){
            hitPoint_left = input.p1 + output.fraction *(input.p2-input.p1);
        }
    }
    if(hit_left==false)intersect=false;
    //«Û”“Ωªµ„
    b2Vec2 hitPoint_right;
    bool hit_right;
    if(intersect){
        b2Vec2 p1(aabb.upperBound.x+margin,H_horiLine);
        b2Vec2 p2(aabb.lowerBound.x-margin,H_horiLine);
        b2RayCastInput input;
        input.p1=p1;
        input.p2=p2;
        input.maxFraction = 1;
        int32 childIndex = 0;
        b2RayCastOutput output;
        hit_right = shape.RayCast(&output, input, transform, childIndex);
        if (hit_right){
            hitPoint_right = input.p1 + output.fraction *(input.p2-input.p1);
        }
        if(hit_right==false)intersect=false;
    }//µ√µΩintersect
    //   cout<<"hit_left,hit_right:"<<hit_left<<" "<<hit_right<<endl;
    if(intersect==false){//»Áπ˚≤ªœ‡Ωª
        //≈–∂œshape‘⁄ÀÆ∆Ωœﬂ÷Æ…œªπ «÷Æœ¬
        b2Vec2 shapeCentroid_world=b2Mul(transform, shape.m_centroid);//◊¢“‚“™◊™µΩ ¿ΩÁø’º‰
        if(shapeCentroid_world.y>H_horiLine){//‘⁄÷Æ…œ
            upperShape=shape;
            upperShapeGot=true;
            lowerShapeGot=false;
        }else{//‘⁄÷Æœ¬
            lowerShape=shape;
            lowerShapeGot=true;
            upperShapeGot=false;
        }
    }else{//»Áπ˚œ‡Ωª
        //Ω´shape∑÷¡—≥……œœ¬¡Ω≤ø∑÷
        vector<b2Vec2> vlist_upper1;
        vector<b2Vec2> vlist_upper2;//∏®÷˙
        vector<b2Vec2> vlist_lower1;
        vector<b2Vec2> vlist_lower2;//∏®÷˙
        int nv=shape.GetVertexCount();
        bool isUpperVertex=false;// «∑ÒŒ™upperΩ⁄µ„
        for(int i=0;i<nv;i++){
            b2Vec2 v=shape.GetVertex(i);
            v=b2Mul(transform, v);//◊™ªØµΩ ¿ΩÁ
            bool isUpperVertex_f=isUpperVertex;
            isUpperVertex=v.y>H_horiLine;
            if(i!=0&&isUpperVertex_f!=isUpperVertex){//»Áπ˚isUpperVertex∑¢…˙±‰ªØ£®±ÿ–Îº”i!=0µƒ≈–∂œ£¨“ÚŒ™ ◊∏ˆ∂•µ„ «æ¯≤ªø…ƒ‹∑¢…˙±‰ªØµƒ£©
                if(isUpperVertex==true){//»Áπ˚ «Ω¯»Î…œ∑Ω
                    //»Áπ˚vlist_upper1Œ™ø’£¨‘ÚΩ´vº”»ÎµΩvlist_upper1£¨∑Ò‘Ú”¶º”»ÎµΩvlist_upper2
                    if(vlist_upper1.empty()){//Ω´vº”»ÎµΩvlist_upper1
                        vlist_upper1.push_back(v);
                    }else{//Ω´vº”»ÎµΩvlist_upper2
                        vlist_upper2.push_back(v);
                    }
                }else{//»Áπ˚ «Ω¯»Îœ¬∑Ω
                    //»Áπ˚vlist_lower1Œ™ø’£¨‘ÚΩ´vº”»ÎµΩvlist_lower1£¨∑Ò‘Ú”¶º”»ÎµΩvlist_lower2
                    if(vlist_lower1.empty()){//Ω´vº”»ÎµΩvlist_lower1
                        vlist_lower1.push_back(v);
                    }else{//Ω´vº”»ÎµΩvlist_lower2
                        vlist_lower2.push_back(v);
                    }
                }
            }else{//»Áπ˚isUpperVertex√ª∑¢…˙±‰ªØ
                if(isUpperVertex){
                    //»Áπ˚vlist_upper2Œ™ø’£¨‘Úº”»ÎµΩvlist_upper1£¨»Áπ˚vlist_upper2≤ªŒ™ø’£¨‘Úº”»ÎµΩvlist_upper2
                    if(vlist_upper2.empty()){
                        vlist_upper1.push_back(v);
                    }else{
                        vlist_upper2.push_back(v);
                    }
                }else{
                    //»Áπ˚vlist_lower2Œ™ø’£¨‘Úº”»ÎµΩvlist_lower1£¨»Áπ˚vlist_lower2≤ªŒ™ø’£¨‘Úº”»ÎµΩvlist_lower2
                    if(vlist_lower2.empty()){
                        vlist_lower1.push_back(v);
                    }else{
                        vlist_lower2.push_back(v);
                    }
                }
            }
        }//µ√µΩvlist_upper1,vlist_upper2,vlist_lower1,vlist_lower2
        //vlist_upper=[hitPoint_right,vlist_upper2,vlist_upper1,hitPoint_left]
        //    cout<<(int)vlist_upper1.size()<<" "<<(int)vlist_upper2.size()<<endl;
        vector<b2Vec2> vlist_upper;
        {
            vlist_upper.push_back(hitPoint_right);
            int nv2=(int)vlist_upper2.size();
            for(int i=0;i<nv2;i++){
                vlist_upper.push_back(vlist_upper2[i]);
            }
            int nv1=(int)vlist_upper1.size();
            for(int i=0;i<nv1;i++){
                vlist_upper.push_back(vlist_upper1[i]);
            }
            vlist_upper.push_back(hitPoint_left);
            
        }//µ√µΩvlist_upper
        //vlist_lower=[hitPoint_left,vlist_lower2,vlist_lower1,hitPoint_right]
        vector<b2Vec2> vlist_lower;
        {
            vlist_lower.push_back(hitPoint_left);
            int nv2=(int)vlist_lower2.size();
            for(int i=0;i<nv2;i++){
                vlist_lower.push_back(vlist_lower2[i]);
            }
            int nv1=(int)vlist_lower1.size();
            for(int i=0;i<nv1;i++){
                vlist_lower.push_back(vlist_lower1[i]);
            }
            vlist_lower.push_back(hitPoint_right);
        }//µ√µΩvlist_lower
        //     cout<<"vlist_upper:"<<(int)vlist_upper.size()<<endl;
        //     cout<<"vlist_lower:"<<(int)vlist_lower.size()<<endl;
        //Ω´vlist_upper∫Õvlist_lower÷–µƒ◊¯±Íæ˘◊™ªØŒ™shapeæ÷≤ø◊¯±Í
        {
            int nv_upper=(int)vlist_upper.size();
            for(int i=0;i<nv_upper;i++){
                vlist_upper[i]=b2MulT(transform, vlist_upper[i]);
            }
            int nv_lower=(int)vlist_lower.size();
            for(int i=0;i<nv_lower;i++){
                vlist_lower[i]=b2MulT(transform, vlist_lower[i]);
            }
        }
        //“∆≥˝œ‡¡⁄µƒ÷ÿ∫œµ„
        {
            const float eps=0.001;
            int nv_upper=(int)vlist_upper.size();
            for(int i=0;i<nv_upper;i++){
                b2Vec2&v=vlist_upper[i];
                b2Vec2&vf=vlist_upper[(i-1+nv_upper)%nv_upper];
                if(fabsf(v.x-vf.x)<eps&&fabsf(v.y-vf.y)<eps){//v”Îvfœ‡µ»
                    //…æ≥˝v
                    vlist_upper.erase(vlist_upper.begin()+i);
                    i--;//œ¬¬÷iªπ‘⁄¥ÀŒª÷√
                    nv_upper--;//≤ª“™Õ¸¡À’‚æ‰
                }
            }
            int nv_lower=(int)vlist_lower.size();
            for(int i=0;i<nv_lower;i++){
                b2Vec2&v=vlist_lower[i];
                b2Vec2&vf=vlist_lower[(i-1+nv_lower)%nv_lower];
                if(fabsf(v.x-vf.x)<eps&&fabsf(v.y-vf.y)<eps){//v”Îvfœ‡µ»
                    //…æ≥˝v
                    vlist_lower.erase(vlist_lower.begin()+i);
                    i--;//œ¬¬÷iªπ‘⁄¥ÀŒª÷√
                    nv_lower--;//≤ª“™Õ¸¡À’‚æ‰
                }
            }
        }
        
        //÷∆◊˜∑µªÿΩ·π˚
        if((int)vlist_upper.size()>=3&&(int)vlist_upper.size()<=b2_maxPolygonVertices){
            upperShapeGot=true;
            upperShape.Set(&vlist_upper[0], (int)vlist_upper.size());
        }else{
            upperShapeGot=false;
        }
        if((int)vlist_lower.size()>=3&&(int)vlist_lower.size()<=b2_maxPolygonVertices){
            lowerShapeGot=true;
            lowerShape.Set(&vlist_lower[0], (int)vlist_lower.size());
        }else{
            lowerShapeGot=false;
        }
        
        
        
    }
    
}


void buoyancyEffect(b2Body*body,int tgFixtureIndex,float waterSurfaceHeight,float waterDensity,b2World*world,float dt)
{
    if(body==NULL)return;
    if(body->GetType()!=b2_dynamicBody)return;
    float waterSurfaceHeight_b2d=waterSurfaceHeight/PTM_RATIO;
    b2Fixture*tgFixture=getFixture(body,tgFixtureIndex);
    assert(tgFixture);
    b2AABB aabb=tgFixture->GetAABB(0);
    if(aabb.lowerBound.y<=waterSurfaceHeight_b2d){
        body->SetAngularDamping(0.3);//0.3
        body->SetLinearDamping(0.6);//0.8
        //tgFixtureshape
        b2Shape*shape=tgFixture->GetShape();
        //shape
        float S=0;
        if(tgFixture->GetShape()->GetType()==b2Shape::e_polygon){
            b2PolygonShape upperShape,lowerShape;
            bool upperShapeGot=false,lowerShapeGot=false;
            splitPolygonShapeWithHoriLine(*(b2PolygonShape*)shape, body->GetTransform(), waterSurfaceHeight,
                                          upperShapeGot,lowerShapeGot,
                                          upperShape, lowerShape);
            if(lowerShapeGot==false){
                //       cout<<"lowerShape not got!"<<endl;
                return;
            }
            //lowerShape
            b2MassData massData;
            lowerShape.ComputeMass(&massData, 1);
            S=massData.mass;//
            
        }else if(tgFixture->GetShape()->GetType()==b2Shape::e_circle){//circle
            
            b2Vec2 c=body->GetWorldCenter();
            float32 r=tgFixture->GetShape()->m_radius;
            S=calculateAreaOfCircleBelowHorizonLine(CCPoint(c.x,c.y), r, waterSurfaceHeight_b2d);
            
        }else{//
            //
            return;
        }
        
        b2Vec2 buoyancyForce=waterDensity*S*fabsf(world->GetGravity().y)*b2Vec2(0,1);//
        //       cout<<"buoyancyForce:"<<buoyancyForce.y<<endl;
        //       cout<<"gravityForce:"<<pObj->pb2Body->GetMass()*fabsf(world->GetGravity().y)<<endl;
        
        //body->ApplyForce(buoyancyForce, body->GetWorldCenter());
        b2Vec2 impluse=dt*buoyancyForce;
        body->ApplyLinearImpulse(impluse, body->GetWorldCenter());
    }
    else{
        body->SetAngularDamping(0);
        body->SetLinearDamping(0);
    }
}


