//
//  myfunc.h
//  mybox2dtest
//
//  Created by apple on 13-8-1.
//
//

#ifndef __mybox2dtest__myfunc__
#define __mybox2dtest__myfunc__

#include <iostream>
#include <string>
#include <vector>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;

string toString(const char* cstr);
vector<float> strToValueList(const string&str);
CCRect getTexRect(CCTexture2D*tex);
float random01();
float randomInRange(float min,float max);
bool pointEqual(const CCPoint&point1,const CCPoint&point2,float eps);
CCRect getCameraRect(CCCamera*camera,CCSize winSize);
void cameraFollowTarget(CCCamera*camera,CCPoint targetPos,CCPoint targetPosPercentInWin,CCSize winSize);
CCPoint rotateVec(const CCPoint &vec,float A);
CCPoint rotateVec(const CCPoint &vec,float cosA,float sinA);
void setNodeWorldPosition(CCNode*node,CCPoint worldPosition);
float nPow(float a,int n);
float calculateAreaOfCircleBelowHorizonLine(CCPoint c,float r,float lineH);
//template function must all in .h
template<class T>
vector<T> conectTwoList(const vector<T>&list1,const vector<T>&list2){
    vector<T> rsList;
    int n1=(int)list1.size();
    for(int i=0;i<n1;i++){
        rsList.push_back(list1[i]);
    }
    int n2=(int)list2.size();
    for(int i=0;i<n2;i++){
        rsList.push_back(list2[i]);
    }
    return rsList;
}
template<class T>
vector<T> reverseList(const vector<T>&list){
    int n=(int)list.size();
    vector<T> rsList(n);//must alloc space first
    for(int i=0;i<n;i++){
        rsList[i]=list[n-1-i];
    }
    return rsList;
}
#endif /* defined(__mybox2dtest__myfunc__) */
