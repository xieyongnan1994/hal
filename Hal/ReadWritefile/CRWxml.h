#pragma once
#ifndef _CRWXML_H
#define _CRWXML_H

#include "tinyxml2.h"

class CRWxml
{
public:
	CRWxml();
	CRWxml(const char* _fileName);
	~CRWxml();

public:	
	//创建根节点
	void creatRoot(const char* conroot);						
	//创建子节点
	void createNode(const char* context, const char* conAttname, const char* convalue, const char* content);
	//添加子节点
	bool addNode(const char* context, const char* conAttname, const char* convalue, const char* content);
	//删除子节点
	bool deleteNode(const char* context, const char* conAttname, const char* convalue);
	//查询节点
	CString checkAttribute(const char* context, const char* conAttname, const char* convalue, const char* content);
	//修改节点属性
	bool modifyNodeAttribute(const char* context, const char* conAttname, const char* conmodAttname, const char* convalue);
	//修改节点内容
	bool modifyNodeText(const char* context, const char* conAttname, const char* convalue, const char* content);
	void createXml();											//创建XML文件
	bool saveXML(const char* fileName);							//保存XML文件
	//添加中文子节点
	void addUTFNode(const char* context, const char* conAttname, const char* convalue, const char* content);
	//读取XML内容
	CString gettent(const char* context, const char* conAttname);
	//获取全部内容
	bool getAlltent(CString & strtent);
private:
	const char* conFilename;			//XML文件名字
	const char* xmlContent;				//XML 声明，定义版本和使用的编码
	tinyxml2::XMLDocument docXml;
	tinyxml2::XMLElement* root;
	tinyxml2::XMLElement* messageNode;
	tinyxml2::XMLError xmlError;
	// 根据属性/值查询节点
	tinyxml2::XMLElement* queryXMLElementByAttribute(tinyxml2::XMLElement* root, const char* elementName, const char* attributeName, const char* attributeValue);

	wchar_t* Char2Wchar(const char* str, UINT CodePage = CP_UTF8);
	char* CRWxml::Ascii2Unicode2Utf8(const char* str);
};

#endif // !_CRWXML_H

