//
//  myfunc.cpp
//  mybox2dtest
//
//  Created by apple on 13-8-1.
//
//

#include "myfunc.h"
string toString(const char* cstr){
    if(cstr==NULL)return string();//���cstrΪNULL���򷵻ؿ�string
    string str=cstr;
    return str;
}
vector<float> strToValueList(const string&_str)
//��string����ȡ��ֵ
//���string�д������֡���š����Ż�ո�֮����ַ����򷵻��б�Ϊempty
//���_strΪ�ջ�ֻ�пո��򷵻��б�Ϊempty
{
    const vector<float> emptyValueList;
    if(_str.empty())return emptyValueList;
    //��str���油��һ���ո��Ա����һ����ֵ����ȡ����
    string str=_str+" ";
    vector<float> valueList;//��ֵ�б�
    string tstr;//������ȡ������ֵ���ִ�
    int len=(int)str.size();
    for(int i=0;i<len;i++){
        bool isNumberOrDotOrSpace=((str[i]>='0'&&str[i]<='9')||str[i]=='.'||str[i]=='-'||str[i]==' '||str[i]=='('||str[i]==')'||str[i]==',');//�����֣���ţ����ţ��ո����ţ�����
        if(isNumberOrDotOrSpace==false){
            cout<<"warning:�������֣���ţ����ţ��ո����ţ�����!"<<endl;
            return emptyValueList;//�������������ֵ�ַ������򷵻ؿ���ֵ�б�
        }
        if(str[i]!=' '&&str[i]!=','&&str[i]!=')'&&str[i]!='('){//������ǿո񣬶��ţ�����
            //���뵽tstr
            tstr+=str[i];
        }else{//����ǿո񣬶��ţ�����
            if(tstr.empty()){//���tstrΪ��
                //�޶���
            }else{//���tstr�ǿ�
                //tstr��ȡ���
                //��tstrת������ֵ����ӵ�valueList
                float value=atof(tstr.c_str());
                valueList.push_back(value);
                //��tstr��գ��Ա���ȡ��һ����ֵ���ִ�
                tstr.clear();
            }
        }
    }//�õ�valueList
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
