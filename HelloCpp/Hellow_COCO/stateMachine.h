//
//  stateMachine.h
//  HelloCpp
//
//  Created by apple on 14-1-12.
//
//

#ifndef __HelloCpp__stateMachine__
#define __HelloCpp__stateMachine__

#include <iostream>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "myObj.h"
class CstateMachine:public CCObject
{
public:
    CstateMachine(){
        m_obj=NULL;
    }
    virtual~CstateMachine(){
        if(m_obj)m_obj->release();
    }
    static CstateMachine*create(){
        CstateMachine*p=new CstateMachine();
        p->autorelease();
        return p;
    }
    virtual void execute(){
        
    }
    void setObj(CmyObj*obj){
        assert(obj);
        assert(m_obj==NULL);
        m_obj=obj;
        m_obj->retain();
    }
    CmyObj*getObj(){
        return m_obj;
    }
protected:
    CmyObj*m_obj;
    
    
};
#endif /* defined(__HelloCpp__stateMachine__) */
