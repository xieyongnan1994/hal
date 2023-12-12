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
	messageNode->SetAttribute(conAttname, convalue);//属性
	messageNode->SetText(content);//内容
	root->InsertEndChild(messageNode);
}

bool CRWxml::addNode(const char* context, const char* conAttname, const char* convalue, const char* content)
{
	assert(conFilename != NULL);
	xmlError = docXml.LoadFile(conFilename);
	if (xmlError == tinyxml2::XML_SUCCESS)
	{
		//获取root节点
		root = docXml.RootElement();
		createNode(context, conAttname, convalue, content);
		//保存文件
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
		//获取root节点
		root = docXml.RootElement();
		//查询
		messageNode = queryXMLElementByAttribute(root, context, conAttname, convalue);
		//删除查询到的节点
		root->DeleteChild(messageNode);
		//保存
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
		//获取root节点
		root = docXml.RootElement();
		//查询
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
		//获取root节点
		root = docXml.RootElement();
		//查询
		messageNode = queryXMLElementByAttribute(root, context, conAttname, convalue);
		//更新属性
		messageNode->SetAttribute(conAttname, conmodAttname);
		//保存
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
		//获取root节点
		root = docXml.RootElement();
		//查询
		messageNode = queryXMLElementByAttribute(root, context, conAttname, convalue);
		//更新内容
		messageNode->SetText(content);
		//保存
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
		//获取root节点
		root = docXml.RootElement();
		//添加节点属性内容
		messageNode = docXml.NewElement(context);
		messageNode->SetAttribute(conAttname, Ascii2Unicode2Utf8(convalue));//属性
		messageNode->SetText(Ascii2Unicode2Utf8(content));//内容
		root->InsertEndChild(messageNode);
		//保存文件
		saveXML(conFilename);
	}
	else
	{
		MessageBox(NULL, _T("load xml file failed."), _T("文件不存在"), MB_OK);
	}
}

CString CRWxml::gettent(const char* context, const char* conAttname)
{
	CString str = _T("");
	xmlError = docXml.LoadFile(conFilename);//读取生成的XML文件，如果还没有生成，会报错：load xml file failed.
	if (xmlError == tinyxml2::XML_SUCCESS) 
	{
		//获取root节点
		root = docXml.RootElement();
		//获取message节点
		messageNode = root->FirstChildElement(context);

		//获取message->username属性的值
		const char* username = messageNode->FindAttribute(conAttname)->Value();
		//获取message的内容
		const char* content = messageNode->GetText();
		//弹窗展示读取到的信息
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
/// 根据属性/值查询节点
/// </summary>
/// <param name="root">xml root节点</param>
/// <param name="elementName">要查询的节点，一般是多行的</param>
/// <param name="attributeName">节点的属性名</param>
/// <param name="attributeValue">节点的属性值</param>
/// <returns></returns>
tinyxml2::XMLElement* CRWxml::queryXMLElementByAttribute(tinyxml2::XMLElement*root, const char* elementName, const char* attributeName, const char* attributeValue)
{
	tinyxml2::XMLElement* node = root->FirstChildElement(elementName);
	while (node != NULL)
	{
		const char* value = node->Attribute(attributeName);
		//比较两个字符串char*是否相等（const char*，不能直接使用等号比较）
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
/// <param name="CodePage">默认CP_UTF8，要求XML文件的编码也要是UTF-8</param>
/// <returns></returns>
wchar_t* CRWxml::Char2Wchar(const char* str, UINT CodePage)
{
	//先获取转换成宽字符后的长度
	int nLen = MultiByteToWideChar(CodePage, MB_PRECOMPOSED, str, -1, NULL, 0);
	//声明一个宽字符类型变量，用于存放转换后的字符
	wchar_t* wstr = new wchar_t[nLen];
	//利用微软ANSI转宽字符的函数（name:ANSI字符，wname:宽字符）
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




