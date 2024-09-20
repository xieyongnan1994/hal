#pragma once
#ifndef _GENERALFUNCTION_H
#define _GENERALFUNCTION_H
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include "json/Json.h"

#include <winsock2.h>
#include <iphlpapi.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")
//#ifdef _DEBUG
//#pragma comment(lib, "json_vc71_libmtd")  
//#else
//#pragma comment(lib, "json_vc71_libmt") 
//#endif

using namespace std;

class GeneralFunction
{
public:
	GeneralFunction();
	~GeneralFunction();

public:																						
	//延时
	int Delay(double time);	

	//获取程序路径
	void GetAppPath(CString& AppPath);								

	//确保路径存在
	int EnsureFolderPathExist(CString strFolder);

	//获取路径中所有文件列表
	int GetFolderFileList(CString folderpath, CString strFileArr[], int arrsize);

	//绝对路径/相对路径转换,mode 0:绝对路径转相对路径,1:相对路径转绝对路径
	CString ConvertRelAbsPath(CString inpath, int mode);
	
	//读取INI文件
	CString ReadIniPara(CString section, CString key, CString strError, CString path);	
	
	//写入INI文件
	void WriteIniPara(CString section, CString key, CString WriteStr, CString path);	

	//写入路径的Txt日志
	int WriteLog(CString strmsg, CString filetype, CString foldername, bool bneedtime = TRUE, CString strtitle = _T(""));	

	//获取时间格式字符串 例如: 2022-06-23 11:11:11
	CString GetTimeStr(CTime t = CTime::GetCurrentTime(), bool bShowDates = TRUE, bool bShowTimes = TRUE);					

	//字符串分隔,单个字符版本
	int SplitString(CString mainstr, CString delistr, CString strarr[], int arrsize);	
	
	//字符串分隔,多个分隔符版本
	int SplitStringM(CString mainstr, CString delistr, CString strarr[], int arrsize);	
	
	//字符串分隔,分隔符长度可以为多位
	int SplitStringN(CString mainstr, CString delistr, CString strarr[], int arrsize);								


	//json解析
	int FindIntFromJson(CString strbody, CString strkey);
	double FindDoubleFromJson(CString strbody, CString strkey);
	CString FindMessageFromJson(CString strbody, CString strkey);

};
#endif // !_GENERALFUNCTION_H
