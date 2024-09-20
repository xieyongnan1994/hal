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
	//�������ڵ�
	void creatRoot(const char* conroot);						
	//�����ӽڵ�
	void createNode(const char* context, const char* conAttname, const char* convalue, const char* content);
	//����ӽڵ�
	bool addNode(const char* context, const char* conAttname, const char* convalue, const char* content);
	//ɾ���ӽڵ�
	bool deleteNode(const char* context, const char* conAttname, const char* convalue);
	//��ѯ�ڵ�
	CString checkAttribute(const char* context, const char* conAttname, const char* convalue, const char* content);
	//�޸Ľڵ�����
	bool modifyNodeAttribute(const char* context, const char* conAttname, const char* conmodAttname, const char* convalue);
	//�޸Ľڵ�����
	bool modifyNodeText(const char* context, const char* conAttname, const char* convalue, const char* content);
	void createXml();											//����XML�ļ�
	bool saveXML(const char* fileName);							//����XML�ļ�
	//��������ӽڵ�
	void addUTFNode(const char* context, const char* conAttname, const char* convalue, const char* content);
	//��ȡXML����
	CString gettent(const char* context, const char* conAttname);
	//��ȡȫ������
	bool getAlltent(CString & strtent);
private:
	const char* conFilename;			//XML�ļ�����
	const char* xmlContent;				//XML ����������汾��ʹ�õı���
	tinyxml2::XMLDocument docXml;
	tinyxml2::XMLElement* root;
	tinyxml2::XMLElement* messageNode;
	tinyxml2::XMLError xmlError;
	// ��������/ֵ��ѯ�ڵ�
	tinyxml2::XMLElement* queryXMLElementByAttribute(tinyxml2::XMLElement* root, const char* elementName, const char* attributeName, const char* attributeValue);

	wchar_t* Char2Wchar(const char* str, UINT CodePage = CP_UTF8);
	char* CRWxml::Ascii2Unicode2Utf8(const char* str);
};

#endif // !_CRWXML_H

