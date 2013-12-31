//
//  myfunc.cpp
//  mybox2dtest
//
//  Created by apple on 13-8-1.
//
//

#include "myfunc.h"
string toString(const char* cstr){
    if(cstr==NULL)return string();//如果cstr为NULL，则返回空string
    string str=cstr;
    return str;
}
vector<float> strToValueList(const string&_str)
//从string中提取数值
//如果string中存在数字、点号、负号或空格之外的字符，则返回列表为empty
//如果_str为空或只有空格，则返回列表为empty
{
    const vector<float> emptyValueList;
    if(_str.empty())return emptyValueList;
    //将str后面补充一个空格，以便最后一个数值能提取出来
    string str=_str+" ";
    vector<float> valueList;//数值列表
    string tstr;//用于提取单个数值的字串
    int len=(int)str.size();
    for(int i=0;i<len;i++){
        bool isNumberOrDotOrSpace=((str[i]>='0'&&str[i]<='9')||str[i]=='.'||str[i]=='-'||str[i]==' '||str[i]=='('||str[i]==')'||str[i]==',');//是数字，点号，负号，空格，括号，逗号
        if(isNumberOrDotOrSpace==false){
            cout<<"warning:不是数字，点号，负号，空格，括号，逗号!"<<endl;
            return emptyValueList;//如果不可能是数值字符串，则返回空数值列表
        }
        if(str[i]!=' '&&str[i]!=','&&str[i]!=')'&&str[i]!='('){//如果不是空格，逗号，括号
            //加入到tstr
            tstr+=str[i];
        }else{//如果是空格，逗号，括号
            if(tstr.empty()){//如果tstr为空
                //无动作
            }else{//如果tstr非空
                //tstr提取完成
                //将tstr转化成数值并添加到valueList
                float value=atof(tstr.c_str());
                valueList.push_back(value);
                //将tstr清空，以便提取下一个数值的字串
                tstr.clear();
            }
        }
    }//得到valueList
    return valueList;

}
CCRect getTexRect(CCTexture2D*tex){
    return CCRect(0,0,tex->getContentSize().width,tex->getContentSize().height);
}
float random01(){
    return rand()*1.0/RAND_MAX;
}
float randomInRange(float min,float max){
    return min+random01()*(max-min);
}

bool pointEqual(const CCPoint&point1,const CCPoint&point2,float eps){
    if(fabsf(point1.x-point2.x)<eps&&fabsf(point1.y-point2.y)<eps){
        return true;
    }else{
        return false;
    }
}
CCRect getCameraRect(CCCamera*camera,CCSize winSize){
    float cx,cy,cz;
    float eyex,eyey,eyez;
    camera->getCenterXYZ(&cx, &cy, &cz);
    camera->getEyeXYZ(&eyex, &eyey, &eyez);
    assert(eyex==cx&&eyey==cy);
    CCRect rect=CCRect(eyex,eyey,winSize.width,winSize.height);
    return rect;
}
void cameraFollowTarget(CCCamera*camera,CCPoint targetPos,CCPoint targetPosPercentInWin,CCSize winSize){
    float cx,cy,cz;
    float eyex,eyey,eyez;
    camera->getCenterXYZ(&cx, &cy, &cz);
    camera->getEyeXYZ(&eyex, &eyey, &eyez);
    camera->setCenterXYZ(targetPos.x-winSize.width*targetPosPercentInWin.x, targetPos.y-winSize.height*targetPosPercentInWin.y, cz);
    camera->setEyeXYZ(targetPos.x-winSize.width*targetPosPercentInWin.x, targetPos.y-winSize.height*targetPosPercentInWin.y, eyez);
}
CCPoint rotateVec(const CCPoint&vec,float A){//A is degree
    CCPoint rs;
    float A_radian=A*M_PI/180;
    float cosA=cosf(A_radian);
    float sinA=sinf(A_radian);
    rs.x=vec.x*cosA-vec.y*sinA;
    rs.y=vec.x*sinA+vec.y*cosA;
    return rs;
}
CCPoint rotateVec(const CCPoint&vec,float cosA,float sinA){
    CCPoint rs;
    rs.x=vec.x*cosA-vec.y*sinA;
    rs.y=vec.x*sinA+vec.y*cosA;
    return rs;
}
void setNodeWorldPosition(CCNode*node,CCPoint worldPosition){//let node's world position equals to worldPosition
    //http://user.qzone.qq.com/350479720/blog/1380908200
    //http://user.qzone.qq.com/350479720/blog/1384483239
    //convert worldPosition to node's father space
    CCPoint posInFather=node->getParent()->convertToNodeSpace(worldPosition);
    //set node's position(position in father space)
    node->setPosition(posInFather);

}
struct _node{
    _node*foe;
    bool value;
};
float nPow(float a,int n){
    assert(!(a==0&&n==0));//0^0 nonsense
    if(n==0&&a!=0)return 1;
    if(a==0&&n!=0)return 0;
    if(n==1)return a;
    if(a==1)return 1;
    _node*tail=NULL;
    while(n!=1){
        _node*node=new _node();
        node->value=n%2;
        node->foe=tail;
        tail=node;
        n=n>>1;
    }//got a chain use tail as tail pointer
    float rs=a;
    while(tail){
        if(tail->value){
            rs=rs*rs*a;
        }else{
            rs=rs*rs;
        }
        _node*temp=tail;
        tail=tail->foe;
        delete temp;
    }
    return rs;
    
}
float calculateAreaOfCircleBelowHorizonLine(CCPoint c,float r,float lineH){
    if(lineH<=c.y-r)return 0;
    if(lineH>=c.y+r)return M_PI*r*r;
    //lineH between c.y-r and c.y+r
    if(lineH==c.y){
        return 0.5*M_PI*r*r;
    }else if(lineH>c.y){
        float d=lineH-c.y;
        float cosA=d/r;
        if(cosA>1)cosA=1;
        float A_rad=acosf(cosA);
        float fanPercentage=1-A_rad/M_PI;
        if(fanPercentage>1)fanPercentage=1;
        if(fanPercentage<0)fanPercentage=0;
        // CCLOG("fanPercentage:%f",fanPercentage);
        float S_fan=fanPercentage*M_PI*r*r;
        float S_tri=d*r*sinf(A_rad);
        float S=S_fan+S_tri;
        return S;
    }else{
        float d=c.y-lineH;
        float cosA=d/r;
        if(cosA>1)cosA=1;
        float A_rad=acosf(cosA);
        float fanPercentage=A_rad/M_PI;
        if(fanPercentage>1)fanPercentage=1;
        if(fanPercentage<0)fanPercentage=0;
       // CCLOG("fanPercentage:%f",fanPercentage);
        float S_fan=fanPercentage*M_PI*r*r;
        float S_tri=d*r*sinf(A_rad);
        float S=S_fan-S_tri;
        return S;
    }


}
