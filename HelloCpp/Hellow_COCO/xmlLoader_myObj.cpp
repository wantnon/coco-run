
#include "xmlLoader_myObj.h"
//////////////////////////////////////////// CshapeInfo
	void CshapeInfo::print(){
		//shapeType
		{
			cout<<"shapeType:"<<shapeType<<endl;
		}
		//c
		{
			cout<<"c:"<<c.x<<","<<c.y<<endl;
		}
		//r
		{
			cout<<"r:"<<r<<endl;
		}
		//rx,ry
		{
			cout<<"rx,ry:"<<rx<<","<<ry<<endl;

		}
		//rot
		{
			cout<<"rot:"<<rot<<endl;
		}
		//vertices
		{
			cout<<"vertices:";
			int nv=(int)vertices.size();
			for(int i=0;i<nv;i++){
				cout<<"("<<vertices[i].x<<","<<vertices[i].y<<")";
			}
			cout<<endl;
		}
	}
	void CshapeInfo::initWithXMLElement(XMLElement *p){
        assert(toString(p->Name())=="shape");
		//shapeType
		string shapeTypeStr=p->FirstChildElement("shapeType")->GetText();
		if(shapeTypeStr=="circle"){
			shapeType=b2Shape::e_circle;
		}else if(shapeTypeStr=="edge"){
			shapeType=b2Shape::e_edge;
		}else if(shapeTypeStr=="polygon"){
			shapeType=b2Shape::e_polygon;
		}else if(shapeTypeStr=="e_chain"){
			shapeType=b2Shape::e_chain;
		}else{
			cout<<"error:未知类型!"<<endl;
			exit(0);
		}
		//c
		{
			vector<float> valist=strToValueList(p->FirstChildElement("c")->GetText());
			if((int)valist.size()!=2){
				cout<<"error:valist.size()!=2"<<endl;
				exit(0);
			}
			c.Set(valist[0]/PTM_RATIO, valist[1]/PTM_RATIO);
		}
		//r
		{
			vector<float> valist=strToValueList(p->FirstChildElement("r")->GetText());
			r=valist[0]/PTM_RATIO;
		}
		//rxry
		{
			vector<float> valist=strToValueList(p->FirstChildElement("rxy")->GetText());
			rx=valist[0]/PTM_RATIO;
			ry=valist[1]/PTM_RATIO;
		}
		//rot
		{
			vector<float> valist=strToValueList(p->FirstChildElement("rot")->GetText());
			rot=valist[0];

		}
		//vertices
		{
			//如果vertices节点没有内容，则p->FirstChildElement("vertices")->GetText()返回NULL,
			//直接将NULL传给string参数会崩溃，所以要加一个toString转一下，它将非空的char*转成string
			//将NULL转成空string
			vector<float> valist=strToValueList(toString(p->FirstChildElement("vertices")->GetText()));
			int nvalue=(int)valist.size();
            assert(nvalue%2==0);
			for(int i=0;i<nvalue;i+=2){
				b2Vec2 v(valist[i]/PTM_RATIO,valist[i+1]/PTM_RATIO);
				vertices.push_back(v);
			}
		}
	}

//////////////////////////////////////////////// CfilterInfo
	void CfilterInfo::print(){

		//categoryBits
		{
			cout<<"categoryBits:"<<categoryBits<<endl;
		}
		//maskBits
		{
			cout<<"maskBits:"<<maskBits<<endl;
		}
		//groupIndex
		{
			cout<<"groupIndex:"<<groupIndex<<endl;
		}

	}
	void CfilterInfo::initWithXMLElement(XMLElement *p){
        assert(toString(p->Name())=="filter");
		//categoryBits
		{
			vector<float> valueList=strToValueList(p->FirstChildElement("categoryBits")->GetText());
			categoryBits=valueList[0];
		}
		//maskBits
		{
			vector<float> valueList=strToValueList(p->FirstChildElement("maskBits")->GetText());
			maskBits=valueList[0];

		}
		//groupIndex
		{
			vector<float> valueList=strToValueList(p->FirstChildElement("groupIndex")->GetText());
			groupIndex=valueList[0];
		}
	}

/////////////////////////////////////////////// CfixtureInfo

	void CfixtureInfo::print(){
		//shapeInfo
		shapeInfo.print();
		//friction
		{
			cout<<"friction:"<<friction<<endl;
		}
		//restitution
		{
			cout<<"restitution:"<<restitution<<endl;
		}
		//density
		{
			cout<<"density:"<<density<<endl;
		}
		//isSensor
		{
			cout<<"isSensor:"<<isSensor<<endl;
		}
		//filterInfo
		filterInfo.print();
	}
	void CfixtureInfo::initWithXMLElement(XMLElement *p){
        assert(toString(p->Name())=="fixture");
		//shapeInfo
		shapeInfo.initWithXMLElement(p->FirstChildElement("shape"));
		//friction
		{
			vector<float> valueList=strToValueList(p->FirstChildElement("friction")->GetText());
			friction=valueList[0];
		}
		//restitution
		{
			vector<float> valueList=strToValueList(p->FirstChildElement("restitution")->GetText());
			restitution=valueList[0];
		}
		//density
		{
			vector<float> valueList=strToValueList(p->FirstChildElement("density")->GetText());
			density=valueList[0];
		}
		//isSensor
		{
			vector<float> valueList=strToValueList(p->FirstChildElement("isSensor")->GetText());
			isSensor=valueList[0];
		}
		//filterInfo
		filterInfo.initWithXMLElement(p->FirstChildElement("filter"));
	}
/////////////////////////////////////// CbodyInfo

	void CbodyInfo::print(){
		//bodyType
		{
			cout<<"bodyType:"<<bodyType<<endl;
		}
		//fixtureInfoList
		{
			int nfixture=(int)fixtureInfoList.size();
			for(int i=0;i<nfixture;i++){
				fixtureInfoList[i].print();
			}
		}
		//pos
		{
			cout<<"pos:"<<pos.x<<","<<pos.y<<endl;
		}
		//rot
		{
			cout<<"rot:"<<rot<<endl;
		}
		//gravityScale
		{
			cout<<"gravityScale:"<<gravityScale<<endl;
		}

	}
	void CbodyInfo::initWithXMLElement(XMLElement *p){
        assert(toString(p->Name())=="body");
		//bodyType
		bodyType=getBodyType(p->FirstChildElement("bodyType")->GetText());
		//fixtureInfoList
		XMLElement*fixture=p->FirstChildElement("fixture");
		while(fixture){
			CfixtureInfo fixtureInfo;
			fixtureInfo.initWithXMLElement(fixture);
			fixtureInfoList.push_back(fixtureInfo);
			fixture=fixture->NextSiblingElement("fixture");
		}
		//pos
		{
			vector<float> valueList=strToValueList(p->FirstChildElement("pos")->GetText());
			pos.Set(valueList[0]/PTM_RATIO, valueList[1]/PTM_RATIO);
		}
		//rot
		{
			vector<float> valueList=strToValueList(p->FirstChildElement("rot")->GetText());
			rot=valueList[0];
		}
		//gravityScale
		{
			vector<float> valueList=strToValueList(p->FirstChildElement("gravityScale")->GetText());
			gravityScale=valueList[0];

		}
	}

	b2BodyType CbodyInfo::getBodyType(const string&str){
		if(str=="staticBody"){
			return b2_staticBody;
		}else if(str=="kinematicBody"){
			return b2_kinematicBody;
		}else if(str=="dynamicBody"){
			return b2_dynamicBody;
		}else{
			cout<<"error:未知类型"<<endl;
			exit(0);
		}
	}
    ///////////////////////////////////////////// CspriteInfo
	void CspriteInfo::print(){
		//texImage
		{
			cout<<"texImage:"<<texImage<<endl;
		}
		//anchorPoint
		{
			cout<<"anchorPoint:"<<anchorPoint.x<<","<<anchorPoint.y<<endl;
		}
		//pos
		{
			cout<<"pos:"<<pos.x<<" "<<pos.y<<endl;
		}
	}
	void CspriteInfo::initWithXMLElement(XMLElement *p){
        assert(toString(p->Name())=="sprite");
        //texImage
		{
            texImage=p->FirstChildElement("texImage")->GetText();
		}
        //anchorPoint
		{
			vector<float> valist=strToValueList(p->FirstChildElement("anchorPoint")->GetText());
			anchorPoint.setPoint(valist[0], valist[1]);
		}
		//pos
		{
			vector<float> valist=strToValueList(p->FirstChildElement("pos")->GetText());
			pos.setPoint(valist[0], valist[1]);
		}

	}

/////////////////////////////////////////// CmyObjInfo

	void CmyObjInfo::print(){
		//name
		cout<<"name:"<<name<<endl;
		//bodyInfoList
		int nBodyInfo=(int)bodyInfoList.size();
		for(int i=0;i<nBodyInfo;i++){
			bodyInfoList[i].print();
		}
		//spriteInfo
		spriteInfo.print();
	}
	void CmyObjInfo::initWithXMLElement(XMLElement *p){
        assert(toString(p->Name())=="myObj");
		//name
		name=p->Attribute("name");
		//bodyList
		{
			XMLElement*body_elem=p->FirstChildElement("body");
			while(body_elem){
				CbodyInfo bodyInfo;
				bodyInfo.initWithXMLElement(body_elem);
				bodyInfoList.push_back(bodyInfo);
				body_elem=body_elem->NextSiblingElement("body");
			}
            assert((int)bodyInfoList.size()<=1);

		}
		//sprite
		spriteInfo.initWithXMLElement(p->FirstChildElement("sprite"));
	}

