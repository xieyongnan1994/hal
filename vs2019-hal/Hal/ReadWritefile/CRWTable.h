#pragma once
#ifndef _CRWTABLE_H
#define _CRWTABLE_H
//#include <atlstr.h>
#include <afxdisp.h>
#include "CApplication.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"
#include "CRange.h"

class CRWTable
{
public:
	CRWTable();
	~CRWTable();

public:
	CApplication m_ExcelApp;
	CWorkbook m_ExcelBook;			//当前处理的文件
	CWorkbooks m_ExcelBooks;		//ExcelBook集合，多文件时使用
	CWorksheet m_ExcelSheet;		//当前使用sheet
	CWorksheets m_ExcelSheets;		//Excel的sheet集合
	CRange m_ExcelRange;			//当前操作区域
	BOOL m_bNewTable = FALSE;
	BOOL m_bServeStart = FALSE;
	CString m_openFilePath;
public:
	void OpenTable(CString OpenPath);//打开表格，OpenPath为要表格路径
	void WriteTable(CString clocow, CString strWrite);//写入表格数据，clocow为表格位置（比如“A5”）,strWrite为要写入的字符
	void ReadTable(CString clocow, CString& strRead);//读取数据
	void CloseTable();//保存并关闭表格

	void WriteCSV();
	void ExportRecord(CString strmsg, CString strpath);

	void show(bool bShow);
	//检查一个cell是否为字符串
	bool isCellString(long iRow, long iColumn);
	//检查一个cell是否为数值
	bool isCellInt(long iRow, long iColumn);
	//得到一个cell的string
	CString getCellString(long iRow, long iColumn);
	//得到一个cell的总数
	int getCellInt(long iRow, long iColumn);
	//得到一个cell的double数据
	double getCellDouble(long iRow, long iColumn);
	//取得行的总数
	int getRowCount();
	//取得列的总数
	int getColumnCount();
	//使用某个shell
	bool loadSheet(long tableId, bool preLoaded = false);
	bool loadSheet(CString sheet, bool preLoaded = false);

	//通过序号取得某个sheet的名称
	CString getSheetName(long tableID);

	//得到sheet的总数
	int getSheetCount();

	//打开excel文件
	bool open(const char* fileName);

	//关闭打开的excel文件
	void close(bool ifSave = false);

	//另存为一个excel文件
	void saveAsXLSFile(const CString& xlsFile);

	//取得打开文件的名称
	CString getOpenFileName();

	//取得打开sheel的名称
	CString getOpenSheelName();

	//向cell中写入一个int值
	void setCellInt(long iRow, long iColumn, int newInt);

	//向cell中写入一个字符串
	void setCellString(long iRow, long iColumn, CString newString);

public:
	//初始化 Excel_OLE
	static bool initExcel();

	//释放Excel_OLE
	static void release();

	//取得列的名称
	static char* getColumnName(long iColumn);

protected:
	void preLoadSheet();

private:
	CString openFileName;

	bool isLoad;                   //是否已经加载了某个sheet数据
	COleSafeArray safeArray;

protected:
	static CApplication application;   //Excel进程实例

};

#endif // !_CRWTABLE_H

