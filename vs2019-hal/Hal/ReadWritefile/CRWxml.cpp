#include "pch.h"
#include "CRWxml.h"

CRWxml::CRWxml()
{
	conFilename = "";
	xmlContent = "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
}

CRWxml::CRWxml(const char* _fileName)
{
	conFilename = _fileName;
	xmlContent = "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
}

CRWxml::~CRWxml()
{
}

void CRWxml::creatRoot(const char* conroot)
{
	root = docXml.NewElement(conroot);
	docXml.InsertEndChild(root);
}

void CRWxml::createNode(const char* context, const char* conAttname, const char* convalue, const char* content)
{
	messageNode = docXml.NewElement(context);
	messageNode->SetAttribute(conAttname, convalue);//����
	messageNode->SetText(content);//����
	root->InsertEndChild(messageNode);
}

bool CRWxml::addNode(const char* context, const char* conAttname, const char* convalue, const char* content)
{
	assert(conFilename != NULL);
	xmlError = docXml.LoadFile(conFilename);
	if (xmlError == tinyxml2::XML_SUCCESS)
	{
		//��ȡroot�ڵ�
		root = docXml.RootElement();
		createNode(context, conAttname, convalue, content);
		//�����ļ�
		saveXML(conFilename);
		return true;
	}
	return false;
}

bool CRWxml::deleteNode(const char* context, const char* conAttname, const char* convalue)
{
	xmlError = docXml.LoadFile(conFilename);
	if (xmlError == tinyxml2::XML_SUCCESS) 
	{
		//��ȡroot�ڵ�
		root = docXml.RootElement();
		//��ѯ
		messageNode = queryXMLElementByAttribute(root, context, conAttname, convalue);
		//ɾ����ѯ���Ľڵ�
		root->DeleteChild(messageNode);
		//����
		saveXML(conFilename);
		return true;
	}
	return false;
}

CString CRWxml::checkAttribute(const char* context, const char* conAttname, const char* convalue, const char* content)
{
	CString strret = _T("");
	xmlError = docXml.LoadFile(conFilename);
	if (xmlError == tinyxml2::XML_SUCCESS)
	{
		//��ȡroot�ڵ�
		root = docXml.RootElement();
		//��ѯ
		messageNode = queryXMLElementByAttribute(root, context, conAttname, convalue);
		strret.Format(_T("<%s %s=\"%s\">%s</%s>"), context, conAttname, convalue, content, context);
		return strret;
	}
	return false;
}

bool CRWxml::modifyNodeAttribute(const char* context, const char* conAttname,  const char* conmodAttname, const char* convalue)
{
	xmlError = docXml.LoadFile(conFilename);
	if (xmlError == tinyxml2::XML_SUCCESS) 
	{
		//��ȡroot�ڵ�
		root = docXml.RootElement();
		//��ѯ
		messageNode = queryXMLElementByAttribute(root, context, conAttname, convalue);
		//��������
		messageNode->SetAttribute(conAttname, conmodAttname);
		//����
		saveXML(conFilename);
		return true;
	}
	return false;
}

bool CRWxml::modifyNodeText(const char* context, const char* conAttname, const char* convalue, const char* content)
{

	xmlError = docXml.LoadFile(conFilename);
	if (xmlError == tinyxml2::XML_SUCCESS)
	{
		//��ȡroot�ڵ�
		root = docXml.RootElement();
		//��ѯ
		messageNode = queryXMLElementByAttribute(root, context, conAttname, convalue);
		//��������
		messageNode->SetText(content);
		//����
		saveXML(conFilename);
		return true;
	}
	return false;
}

void CRWxml::createXml()
{
	docXml.Parse(xmlContent);
	root = docXml.NewElement("root");
	docXml.InsertEndChild(root);
	saveXML(conFilename);
}

bool CRWxml::saveXML(const char* fileName)
{
	assert(fileName != NULL);
	int nRel = docXml.SaveFile(fileName);
	if (nRel != -1)
	{
		return true;
	}
	return false;
}

void CRWxml::addUTFNode(const char* context, const char* conAttname, const char* convalue, const char* content)
{
	assert(conFilename != NULL);
	xmlError = docXml.LoadFile(conFilename);
	if (xmlError == tinyxml2::XML_SUCCESS)
	{
		//��ȡroot�ڵ�
		root = docXml.RootElement();
		//��ӽڵ���������
		messageNode = docXml.NewElement(context);
		messageNode->SetAttribute(conAttname, Ascii2Unicode2Utf8(convalue));//����
		messageNode->SetText(Ascii2Unicode2Utf8(content));//����
		root->InsertEndChild(messageNode);
		//�����ļ�
		saveXML(conFilename);
	}
	else
	{
		MessageBox(NULL, _T("load xml file failed."), _T("�ļ�������"), MB_OK);
	}
}

CString CRWxml::gettent(const char* context, const char* conAttname)
{
	CString str = _T("");
	xmlError = docXml.LoadFile(conFilename);//��ȡ���ɵ�XML�ļ��������û�����ɣ��ᱨ��load xml file failed.
	if (xmlError == tinyxml2::XML_SUCCESS) 
	{
		//��ȡroot�ڵ�
		root = docXml.RootElement();
		//��ȡmessage�ڵ�
		messageNode = root->FirstChildElement(context);

		//��ȡmessage->username���Ե�ֵ
		const char* username = messageNode->FindAttribute(conAttname)->Value();
		//��ȡmessage������
		const char* content = messageNode->GetText();
		//����չʾ��ȡ������Ϣ
		str.Format(_T("<%s %s=\"%s\">%s</%s>"),(CString)context, (CString)conAttname,Char2Wchar(username), Char2Wchar(content), (CString)context);
		return str;
	}
	str = _T("load xml file failed");
	return str;
}

bool CRWxml::getAlltent(CString& strtent)
{
	CString str = _T("");
	xmlError = docXml.LoadFile(conFilename);
	if (xmlError == tinyxml2::XML_SUCCESS) 
	{
		tinyxml2::XMLPrinter printer;
		docXml.Print(&printer);
		str.Format(Char2Wchar(printer.CStr()));
		strtent = str;
		return true;
	}
	return false;
}

/// <summary>
/// ��������/ֵ��ѯ�ڵ�
/// </summary>
/// <param name="root">xml root�ڵ�</param>
/// <param name="elementName">Ҫ��ѯ�Ľڵ㣬һ���Ƕ��е�</param>
/// <param name="attributeName">�ڵ��������</param>
/// <param name="attributeValue">�ڵ������ֵ</param>
/// <returns></returns>
tinyxml2::XMLElement* CRWxml::queryXMLElementByAttribute(tinyxml2::XMLElement*root, const char* elementName, const char* attributeName, const char* attributeValue)
{
	tinyxml2::XMLElement* node = root->FirstChildElement(elementName);
	while (node != NULL)
	{
		const char* value = node->Attribute(attributeName);
		//�Ƚ������ַ���char*�Ƿ���ȣ�const char*������ֱ��ʹ�õȺűȽϣ�
		if (strcmp(value, attributeValue) == 0)
			break;

		node = node->NextSiblingElement();
	}
	return node;
}

/// <summary>
/// 
/// </summary>
/// <param name="str"></param>
/// <param name="CodePage">Ĭ��CP_UTF8��Ҫ��XML�ļ��ı���ҲҪ��UTF-8</param>
/// <returns></returns>
wchar_t* CRWxml::Char2Wchar(const char* str, UINT CodePage)
{
	//�Ȼ�ȡת���ɿ��ַ���ĳ���
	int nLen = MultiByteToWideChar(CodePage, MB_PRECOMPOSED, str, -1, NULL, 0);
	//����һ�����ַ����ͱ��������ڴ��ת������ַ�
	wchar_t* wstr = new wchar_t[nLen];
	//����΢��ANSIת���ַ��ĺ�����name:ANSI�ַ���wname:���ַ���
	MultiByteToWideChar(CodePage, MB_PRECOMPOSED, str, -1, wstr, nLen);

	return wstr;
}

char* CRWxml::Ascii2Unicode2Utf8(const char* str)
{
	//ASCII to Unicode(CP_ACP)
	int nLen = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[nLen];
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, -1, wstr, nLen);

	//Unicode to UTF-8
	int nLenUTF8 = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* strUTF8 = new char[nLenUTF8];
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, strUTF8, nLenUTF8, NULL, NULL);

	return strUTF8;
}




