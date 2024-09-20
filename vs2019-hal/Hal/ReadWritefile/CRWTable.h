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
	CWorkbook m_ExcelBook;			//��ǰ������ļ�
	CWorkbooks m_ExcelBooks;		//ExcelBook���ϣ����ļ�ʱʹ��
	CWorksheet m_ExcelSheet;		//��ǰʹ��sheet
	CWorksheets m_ExcelSheets;		//Excel��sheet����
	CRange m_ExcelRange;			//��ǰ��������
	BOOL m_bNewTable = FALSE;
	BOOL m_bServeStart = FALSE;
	CString m_openFilePath;
public:
	void OpenTable(CString OpenPath);//�򿪱��OpenPathΪҪ���·��
	void WriteTable(CString clocow, CString strWrite);//д�������ݣ�clocowΪ���λ�ã����硰A5����,strWriteΪҪд����ַ�
	void ReadTable(CString clocow, CString& strRead);//��ȡ����
	void CloseTable();//���沢�رձ��

	void WriteCSV();
	void ExportRecord(CString strmsg, CString strpath);

	void show(bool bShow);
	//���һ��cell�Ƿ�Ϊ�ַ���
	bool isCellString(long iRow, long iColumn);
	//���һ��cell�Ƿ�Ϊ��ֵ
	bool isCellInt(long iRow, long iColumn);
	//�õ�һ��cell��string
	CString getCellString(long iRow, long iColumn);
	//�õ�һ��cell������
	int getCellInt(long iRow, long iColumn);
	//�õ�һ��cell��double����
	double getCellDouble(long iRow, long iColumn);
	//ȡ���е�����
	int getRowCount();
	//ȡ���е�����
	int getColumnCount();
	//ʹ��ĳ��shell
	bool loadSheet(long tableId, bool preLoaded = false);
	bool loadSheet(CString sheet, bool preLoaded = false);

	//ͨ�����ȡ��ĳ��sheet������
	CString getSheetName(long tableID);

	//�õ�sheet������
	int getSheetCount();

	//��excel�ļ�
	bool open(const char* fileName);

	//�رմ򿪵�excel�ļ�
	void close(bool ifSave = false);

	//���Ϊһ��excel�ļ�
	void saveAsXLSFile(const CString& xlsFile);

	//ȡ�ô��ļ�������
	CString getOpenFileName();

	//ȡ�ô�sheel������
	CString getOpenSheelName();

	//��cell��д��һ��intֵ
	void setCellInt(long iRow, long iColumn, int newInt);

	//��cell��д��һ���ַ���
	void setCellString(long iRow, long iColumn, CString newString);

public:
	//��ʼ�� Excel_OLE
	static bool initExcel();

	//�ͷ�Excel_OLE
	static void release();

	//ȡ���е�����
	static char* getColumnName(long iColumn);

protected:
	void preLoadSheet();

private:
	CString openFileName;

	bool isLoad;                   //�Ƿ��Ѿ�������ĳ��sheet����
	COleSafeArray safeArray;

protected:
	static CApplication application;   //Excel����ʵ��

};

#endif // !_CRWTABLE_H

