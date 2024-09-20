#include "pch.h"
#include "GeneralFunction.h"
#include "winioctl.h"
#include <Nb30.h>


#pragma comment(lib,"netapi32.lib")

GeneralFunction::GeneralFunction()
{
}

GeneralFunction::~GeneralFunction()
{
}

int GeneralFunction::Delay(double time)
{
	static MSG msg;
	long Curtime1, Curtime2 = GetTickCount();
	while (true)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			AfxGetApp()->PumpMessage();
		}
		Sleep(1);
		Curtime1 = GetTickCount() - Curtime2;
		if (Curtime1 >= long(time)) return 0;
	}
}

void GeneralFunction::GetAppPath(CString& AppPath)
{
	CString strt = _T("");
	TCHAR temp[1024];
	::GetModuleFileName(NULL, temp, 1023);
	strt = (CString)temp;
	int n = strt.ReverseFind('\\');
	AppPath = strt.Left(n);
}

int GeneralFunction::EnsureFolderPathExist(CString strFolder)
{
	CString strArr[10];
	int n = SplitString(strFolder, _T("\\"), strArr, 10);

	CString str = _T("");

	for (int i = 0; i < n; i++)
	{
		if (i == (n - 1) && strArr[i].Find(_T(".")) != -1)
		{
			return 0;
		}
		if (i == 0)
		{
			str = strArr[0];
		}
		else
		{
			str += _T("\\") + strArr[i];
		}
		if (PathFileExists(str) == FALSE)
		{
			if (i == 0)//磁盘根目录不存在
			{
				return 1;
			}
			else
			{
				if (!CreateDirectory(str, NULL))
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

int GeneralFunction::GetFolderFileList(CString folderpath, CString strFileArr[], int arrsize)
{
	int nFileCnt = 0;
	CFileFind tempFind;
	BOOL bFound; //判断是否成功找到文件
	bFound = tempFind.FindFile(folderpath + _T("\\*.*"));
	while (bFound)      //遍历所有文件
	{
		bFound = tempFind.FindNextFile(); //第一次执行FindNextFile是选择到第一个文件，以后执行为选择

		if (tempFind.IsDirectory() || tempFind.IsDots())   //找到的是文件夹，则遍历该文件夹下的文件
		{
			continue;
		}
		else
		{
			strFileArr[nFileCnt] = tempFind.GetFileName(); //保存文件名，包括后缀名
			nFileCnt++;
		}
	}
	tempFind.Close();
	return nFileCnt;
}

CString GeneralFunction::ConvertRelAbsPath(CString inpath, int mode)
{
	CString exepath = _T(""), strexeArr[50], strinArr[50], newpath = _T("");
	int nexecnt = 0, nincnt = 0, nmincnt = 0, nsamecnt = 0;
	GetAppPath(exepath);

	nexecnt = SplitString(exepath, _T("\\"), strexeArr, 50);
	nincnt = SplitString(inpath, _T("\\"), strinArr, 50);
	nmincnt = (nexecnt > nincnt) ? nincnt : nexecnt;

	if (mode == 0)//0:绝对路径转相对路径
	{
		for (int i = 0; i < nmincnt; i++)
		{
			if (strexeArr[i] == strinArr[i])
			{
				nsamecnt++;
			}
			else
			{
				break;
			}
		}

		newpath = _T(".");
		for (int i = 0; i < (nexecnt - nsamecnt); i++)
		{
			newpath += _T(".");
		}

		for (int i = nsamecnt; i < nincnt; i++)
		{
			newpath += _T("\\") + strinArr[i];
		}

		return newpath;

	}
	else if (mode == 1)//1:相对路径转绝对路径
	{
		int deldircnt = 0;
		deldircnt = strinArr[0].GetLength() - 1;

		if (deldircnt > nexecnt)
		{
			return _T("");
		}

		newpath = strexeArr[0];
		for (int i = 1; i < (nexecnt - deldircnt); i++)
		{
			newpath += _T("\\") + strexeArr[i];
		}

		for (int i = 1; i < nincnt; i++)
		{
			newpath += _T("\\") + strinArr[i];
		}
		return newpath;
	}

	return _T("");
}

CString GeneralFunction::ReadIniPara(CString section, CString key, CString strError, CString path)
{
	WCHAR strBuffer[200];
	::GetPrivateProfileStringW(section, key, strError, strBuffer, 200, path);
	return (CString)strBuffer;
}

void GeneralFunction::WriteIniPara(CString section, CString key, CString WriteStr, CString path)
{
	WritePrivateProfileStringW(section, key, WriteStr, path);
}

int GeneralFunction::WriteLog(CString strmsg, CString filetype, CString foldername, bool bneedtime, CString strtitle)
{
	CStdioFile file;
	CString strFileName, strFloderYear, strFloderMonth, fpath, tempWrite, strTime = _T("");
	SYSTEMTIME t;
	GetLocalTime(&t);
	bool createfileflag = FALSE;

	strFileName.Format((_T("%s%d-%02d-%02d.txt"), filetype, t.wYear, t.wMonth, t.wDay));

	strFloderYear.Format(_T("%d年"), t.wYear);//
	strFloderMonth.Format(_T("%02d月"), t.wMonth);//

	fpath = _T("D:\\Test");
	if (PathIsDirectory(fpath) == FALSE)	 CreateDirectory(fpath, NULL);//判断路径是否存在，不存在则创建
	fpath = _T("D:\\Test\\") + foldername;
	if (PathIsDirectory(fpath) == FALSE)	 CreateDirectory(fpath, NULL);//判断路径是否存在，不存在则创建
	fpath = _T("D:\\Test\\") + foldername + _T("\\") + strFloderYear;
	if (PathIsDirectory(fpath) == FALSE)	 CreateDirectory(fpath, NULL);//判断路径是否存在，不存在则创建
	fpath = _T("D:\\Test\\") + foldername + _T("\\") + strFloderYear + _T("\\") + strFloderMonth;
	if (PathIsDirectory(fpath) == FALSE)	 CreateDirectory(fpath, NULL);//判断路径是否存在，不存在则创建

	fpath = _T("D:\\Test\\") + foldername + _T("\\") + strFloderYear + _T("\\") + strFloderMonth + _T("\\") + strFileName;

	if (!PathFileExists(fpath))
	{
		createfileflag = TRUE;
	}

	int retryTimes = 0;
retryWrite:
	if (file.Open(fpath, CStdioFile::modeNoTruncate | CStdioFile::modeCreate | CStdioFile::modeReadWrite | CStdioFile::typeText))	 //打开文件
	{
		USES_CONVERSION;

		if (createfileflag && !(strtitle.IsEmpty()))
		{
			tempWrite = strtitle + _T("\n");
			char* ss = T2A(tempWrite);
			file.Write(ss, strlen(ss));
		}

		file.SeekToEnd();

		if (bneedtime)
		{
			strTime.Format(_T("%d-%02d-%02d %02d:%02d:%02d,"), t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
		}
		tempWrite = strTime + strmsg + _T("\n");
		char* ss = T2A(tempWrite);
		file.Write(ss, strlen(ss));
		file.Close();
		return 0;
	}
	else
	{
		Delay(7);
		retryTimes++;
		if (retryTimes > 133)
		{
			return 1;
		}
		goto retryWrite;
	}
}

//取得当前时间,可选择是否显示日期,时间与毫秒数
CString GeneralFunction::GetTimeStr(CTime t, bool bShowDates, bool bShowTimes)
{
	CString strDates, strTimes;

	if (bShowDates)
	{
		strDates.Format(_T("%d-%d-%d"), t.GetYear(), t.GetMonth(), t.GetDay());
	}
	else
	{
		strDates = L"";
	}
	if (bShowTimes)
	{
		strTimes.Format(_T("%02d:%02d:%02d"), t.GetHour(), t.GetMinute(), t.GetSecond());
	}
	else
	{
		strTimes = L"";
	}
	if (!strTimes.IsEmpty() && !strDates.IsEmpty())
	{
		strDates += L"_";
	}
	return strDates + strTimes;
}

int GeneralFunction::SplitString(CString mainstr, CString delistr, CString strarr[], int arrsize)
//返回值:解析成功结果数量; mainstr:需要解析的字符串; delistr:分隔符字符串; strarr[]:用来返回的字符串数组; arrsize:strarr字符串数组大小,
{
	int ncount = 0;
	int aindex = -1, bindex = 0;
	CString strx;
	for (int i = 0; i < arrsize; i++)
	{
		bindex = mainstr.Find(delistr, aindex + 1);
		if (bindex != -1)
		{
			strx = mainstr.Mid(aindex + 1, bindex - aindex - 1);
			strarr[i] = strx;
			aindex = bindex;
			ncount++;
		}
		else
		{
			strx = mainstr.Mid(aindex + 1); //用来解析最后一项没有分隔符的字符串
			if (!strx.IsEmpty())
			{
				strarr[i] = strx;
				ncount++;
				break;
			}
			else
			{
				break;
			}
		}
	}
	for (int i = ncount; i < arrsize; i++)
	{
		strarr[i] = _T("");
	}
	return ncount;
}

int GeneralFunction::SplitStringM(CString mainstr, CString delistr, CString strarr[], int arrsize)
//返回值:解析成功结果数量; mainstr:需要解析的字符串; delistr:分隔符字符串; strarr[]:用来返回的字符串数组; arrsize:strarr字符串数组大小,
{
	int ncount = 0;
	int aindex = -1, bindex = 0;
	CString strx;
	int nmax = delistr.GetLength();
	if (nmax > 20) nmax = 20;
	CString deliArr[20];
	for (int i = 0; i < nmax; i++)
	{
		deliArr[i] = delistr.Mid(i, 1);
	}
	for (int i = 0; i < arrsize; i++)
	{
		for (int j = 0; j < nmax; j++)
		{
			bindex = mainstr.Find(deliArr[j], aindex + 1);
			if (bindex != -1 && deliArr[j] != _T(""))
			{
				break;
			}
		}
		//bindex=mainstr.Find(delistr,aindex+1);
		if (bindex != -1)
		{
			strx = mainstr.Mid(aindex + 1, bindex - aindex - 1);
			strarr[i] = strx;
			aindex = bindex;
			ncount++;
		}
		else
		{
			strx = mainstr.Mid(aindex + 1); //用来解析最后一项没有分隔符的字符串
			if (!strx.IsEmpty())
			{
				strarr[i] = strx;
				ncount++;
				break;
			}
			else
			{
				break;
			}
		}
	}
	for (int i = ncount; i < arrsize; i++)
	{
		strarr[i] = _T("");
	}
	return ncount;
}

int GeneralFunction::SplitStringN(CString mainstr, CString delistr, CString strarr[], int arrsize)
{
	int delistrlen = delistr.GetLength();
	int ncount = 0;
	int aindex = (0 - delistrlen), bindex = 0;
	CString strx;
	for (int i = 0; i < arrsize; i++)
	{
		bindex = mainstr.Find(delistr, aindex + delistrlen);
		if (bindex != -1)
		{
			strx = mainstr.Mid(aindex + delistrlen, bindex - aindex - delistrlen);
			strarr[i] = strx;
			aindex = bindex;
			ncount++;
		}
		else
		{
			strx = mainstr.Mid(aindex + delistrlen); //用来解析最后一项没有分隔符的字符串
			if (!strx.IsEmpty())
			{
				strarr[i] = strx;
				ncount++;
				break;
			}
			else
			{
				break;
			}
		}
	}
	for (int i = ncount; i < arrsize; i++)
	{
		strarr[i] = _T("");
	}
	return ncount;
}


int GeneralFunction::FindIntFromJson(CString strbody, CString strkey)
{
	try
	{
		Json::Value root;
		Json::Reader reader;
		USES_CONVERSION;
		const char* s = T2A(strbody);
		if (!reader.parse(s, root))
		{
			return -1;
		}

		CString strArr[10];
		int x = SplitString(strkey, _T("\\"), strArr, 10);
		Json::Value keyNode;

		switch (x)
		{
		case 2:
			if (root[T2A(strArr[0])].isArray())//判断是否是数组
			{
				for (unsigned int i = 0; i < root[T2A(strArr[0])].size(); i++)
				{
					keyNode = root[T2A(strArr[0])][i][T2A(strArr[1])];
				}
			}
			else
			{
				keyNode = root[T2A(strArr[0])][T2A(strArr[1])];
			}
			break;
		case 3:
			if (root[T2A(strArr[0])][T2A(strArr[1])].isArray())//判断是否是数组
			{
				for (unsigned int i = 0; i < root[T2A(strArr[0])][T2A(strArr[1])].size(); i++)
				{
					keyNode = root[T2A(strArr[0])][T2A(strArr[1])][i][T2A(strArr[2])];
				}
			}
			else
			{
				keyNode = root[T2A(strArr[0])][T2A(strArr[1])][T2A(strArr[2])];
			}
			break;
		case 4:
			if (root[T2A(strArr[0])][T2A(strArr[1])][T2A(strArr[2])].isArray())//判断是否是数组
			{
				for (unsigned int i = 0; i < root[T2A(strArr[0])][T2A(strArr[1])][T2A(strArr[2])].size(); i++)
				{
					keyNode = root[T2A(strArr[0])][T2A(strArr[1])][T2A(strArr[2])][i][T2A(strArr[3])];
				}
			}
			else
			{
				keyNode = root[T2A(strArr[0])][T2A(strArr[1])][T2A(strArr[2])][T2A(strArr[3])];
			}
			break;
		default:
			if (root.empty() || (root.isArray() == false && root.isObject() == false))
			{
				return -1;
			}
			else
			{
				keyNode = root[T2A(strArr[0])];
			}
			break;
		}
		if (keyNode.isNull())
		{
			return -1;
		}
		else if (keyNode.isNumeric())
		{
			return keyNode.asInt();
		}
		else
		{
			CString strval = _T("");
			strval = keyNode.asString().c_str();
			return _wtoi(strval);
		}
	}
	catch (...)
	{
		return -1;
	}
	return 0;
}

double GeneralFunction::FindDoubleFromJson(CString strbody, CString strkey)
{

	try
	{
		Json::Value root;
		Json::Reader reader;
		USES_CONVERSION;
		const char* s = T2A(strbody);
		if (!reader.parse(s, root))
		{
			return -1;
		}

		CString strArr[10];
		int x = SplitString(strkey, _T("\\"), strArr, 10);
		Json::Value keyNode;

		switch (x)
		{
		case 2:
			if (root[T2A(strArr[0])].isArray())//判断是否是数组
			{
				for (unsigned int i = 0; i < root[T2A(strArr[0])].size(); i++)
				{
					keyNode = root[T2A(strArr[0])][i][T2A(strArr[1])];
				}
			}
			else
			{
				keyNode = root[T2A(strArr[0])][T2A(strArr[1])];
			}
			break;
		case 3:
			if (root[T2A(strArr[0])][T2A(strArr[1])].isArray())//判断是否是数组
			{
				for (unsigned int i = 0; i < root[T2A(strArr[0])][T2A(strArr[1])].size(); i++)
				{
					keyNode = root[T2A(strArr[0])][T2A(strArr[1])][i][T2A(strArr[2])];
				}
			}
			else
			{
				keyNode = root[T2A(strArr[0])][T2A(strArr[1])][T2A(strArr[2])];
			}
			break;
		case 4:
			if (root[T2A(strArr[0])][T2A(strArr[1])][T2A(strArr[2])].isArray())//判断是否是数组
			{
				for (unsigned int i = 0; i < root[T2A(strArr[0])][T2A(strArr[1])][T2A(strArr[2])].size(); i++)
				{
					keyNode = root[T2A(strArr[0])][T2A(strArr[1])][T2A(strArr[2])][i][T2A(strArr[3])];
				}
			}
			else
			{
				keyNode = root[T2A(strArr[0])][T2A(strArr[1])][T2A(strArr[2])][T2A(strArr[3])];
			}
			break;
		default:
			if (root.empty() || (root.isArray() == false && root.isObject() == false))
			{
				return -1;
			}
			else
			{
				keyNode = root[T2A(strArr[0])];
			}
			break;
		}
		if (keyNode.isNull())
		{
			return -1;
		}
		else if (keyNode.isNumeric())
		{
			return keyNode.asDouble();
		}
		else
		{
			CString strval = _T("");
			strval = keyNode.asString().c_str();
			return _wtof(strval);
		}
	}
	catch (...)
	{
		return -1;
	}
	return 0;
}

CString GeneralFunction::FindMessageFromJson(CString strbody, CString strkey)
{
	CString strval = _T("");
	try
	{
		Json::Value root;
		Json::Reader reader;
		USES_CONVERSION;
		const char* s = T2A(strbody);
		if (!reader.parse(s, root))
		{
			return _T("");
		}

		CString strArr[10];
		int x = SplitString(strkey, _T("\\"), strArr, 10);
		Json::Value keyNode;

		switch (x)
		{
		case 2:
			if (root[T2A(strArr[0])].isArray())//判断是否是数组
			{
				for (unsigned int i = 0; i < root[T2A(strArr[0])].size(); i++)
				{
					keyNode = root[T2A(strArr[0])][i][T2A(strArr[1])];
				}
			}
			else
			{
				keyNode = root[T2A(strArr[0])][T2A(strArr[1])];
			}
			break;
		case 3:
			if (root[T2A(strArr[0])][T2A(strArr[1])].isArray())//判断是否是数组
			{
				for (unsigned int i = 0; i < root[T2A(strArr[0])][T2A(strArr[1])].size(); i++)
				{
					keyNode = root[T2A(strArr[0])][T2A(strArr[1])][i][T2A(strArr[2])];
				}
			}
			else
			{
				keyNode = root[T2A(strArr[0])][T2A(strArr[1])][T2A(strArr[2])];
			}
			break;
		case 4:
			if (root[T2A(strArr[0])][T2A(strArr[1])][T2A(strArr[2])].isArray())//判断是否是数组
			{
				for (unsigned int i = 0; i < root[T2A(strArr[0])][T2A(strArr[1])][T2A(strArr[2])].size(); i++)
				{
					keyNode = root[T2A(strArr[0])][T2A(strArr[1])][T2A(strArr[2])][i][T2A(strArr[3])];
				}
			}
			else
			{
				keyNode = root[T2A(strArr[0])][T2A(strArr[1])][T2A(strArr[2])][T2A(strArr[3])];
			}
			break;
		default:
			if (root.empty() || (root.isArray() == false && root.isObject() == false))
			{
				return _T("");
			}
			else
			{
				keyNode = root[T2A(strArr[0])];
			}
			break;
		}
		if (keyNode.isNull())
		{
			return _T("");
		}
		else if (keyNode.isNumeric())
		{
			double d = keyNode.asDouble();
			strval.Format(_T("%0.3f"), d);
			return strval;
		}
		else
		{
			strval = keyNode.asString().c_str();
			return strval;
		}
	}
	catch (...)
	{
		return _T("");
	}
	return _T("");
}
