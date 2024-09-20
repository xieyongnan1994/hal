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
	//��ʱ
	int Delay(double time);	

	//��ȡ����·��
	void GetAppPath(CString& AppPath);								

	//ȷ��·������
	int EnsureFolderPathExist(CString strFolder);

	//��ȡ·���������ļ��б�
	int GetFolderFileList(CString folderpath, CString strFileArr[], int arrsize);

	//����·��/���·��ת��,mode 0:����·��ת���·��,1:���·��ת����·��
	CString ConvertRelAbsPath(CString inpath, int mode);
	
	//��ȡINI�ļ�
	CString ReadIniPara(CString section, CString key, CString strError, CString path);	
	
	//д��INI�ļ�
	void WriteIniPara(CString section, CString key, CString WriteStr, CString path);	

	//д��·����Txt��־
	int WriteLog(CString strmsg, CString filetype, CString foldername, bool bneedtime = TRUE, CString strtitle = _T(""));	

	//��ȡʱ���ʽ�ַ��� ����: 2022-06-23 11:11:11
	CString GetTimeStr(CTime t = CTime::GetCurrentTime(), bool bShowDates = TRUE, bool bShowTimes = TRUE);					

	//�ַ����ָ�,�����ַ��汾
	int SplitString(CString mainstr, CString delistr, CString strarr[], int arrsize);	
	
	//�ַ����ָ�,����ָ����汾
	int SplitStringM(CString mainstr, CString delistr, CString strarr[], int arrsize);	
	
	//�ַ����ָ�,�ָ������ȿ���Ϊ��λ
	int SplitStringN(CString mainstr, CString delistr, CString strarr[], int arrsize);								


	//json����
	int FindIntFromJson(CString strbody, CString strkey);
	double FindDoubleFromJson(CString strbody, CString strkey);
	CString FindMessageFromJson(CString strbody, CString strkey);

};
#endif // !_GENERALFUNCTION_H
