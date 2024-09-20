#ifndef __DATAOPER_H__
#define __DATAOPER_H__
#include <afxinet.h>

#import "msado15.dll" rename("EOF","adoEOF")
//#import "C:\Program Files\Common Files\System\ADO\msado15.dll" rename("EOF", "EndOfFile")

#define USEPASSWORD		1
#define NOTUSEPASSWORD	0

class CDataBase
{
public:
	CDataBase();
	~CDataBase();
public:
	void AccessInit(bool bPassWordUseOrNot);
	static void CompactDB(CString Path);		//Ñ¹ËõÐÞ¸´Êý¾Ý¿â
	bool Connect2DataBase(CString Path);
	void DisconnectDataBase(void);
	BOOL OpenForm(CString FormName);
	void SetPassWordState(bool bNewState);
	bool GetPassWordState(void);
	void SetPassWord(CString PassWord);
	CString GetPassWord(void);
	void WriteData(variant_t DataField, variant_t Content);
	void WriteDataEx(variant_t DataField, variant_t Content, int nRow);
	variant_t ReadData(variant_t Index);
	variant_t ReadDataEx(variant_t Index, int nRow);
	void ClrCurSheet();
	int GetRowCount();
	int GetColCount();
public:
	static bool ms_bInitState;
	ADODB::_ConnectionPtr m_pConnection;
	ADODB::_RecordsetPtr  m_pRecordset;
private:
	bool m_bPassWordEn;
	CString m_PassWord;
};

#endif