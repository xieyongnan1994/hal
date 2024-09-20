//#pragma once
//#ifndef _CRWMYSQL_H
//#define _CRWMYSQL_H
//#include <mysql.h>
//#include "winsock.h"
//
////��¼��mysql - u root - p ����
////�������ݿ⣺ create database test;
////�鿴���������ݿ⣺show databases;
////ʹ�ô��������ݿ⣺use test;
////�鿴��ǰʹ�õ����ݿ⣺select database();
////�������ݱ�create table students(
////	id int(4) primary key auto_increment, //��Ϊ���� �Զ�����  
////	name varchar(40) not null,
////	age int(4) not null,
////	class varchar(40) not null);
////�鿴���������ݱ�desc students;
////������ݣ�insert into student(name, age, class) values('����', '����', '�༶');
////��ѯ���ݣ�select* from students;
////ɾ�����ݣ�delete from student where name = '����'
////�������ݣ�update student set age = '������', class = '�°༶' where name = '����';
////ɾ�����ݿ⣺drop database test;
//class CRWMysql
//{
//public:
//	CRWMysql();
//	~CRWMysql();
//public:
//	BOOL MySQL_InitDataBase(CString strlocal, CString strroot, CString strpass, CString strdia, int nport); // ��ʼ��MySQL���ݿ�
//	BOOL MySQL_CreateTable(CString sql_create_table);//������
//	
//	BOOL MySQL_SelectData(char* sql_select);//��ѯ����
//
//	BOOL MySQL_InsertData(char* ctable, char* cfield, char* cvalue);//��������
//	BOOL MySQL_DeleteData(char* ctable, char* cfield, char* cvalue);//ɾ������
//
//	void CloseMySQL();
//
//private:
//	MYSQL m_sqlCon;
//};
//
//#endif // !_CRWMYSQL_H
//
#pragma once
#ifndef _CRWMYSQL_H
#define _CRWMYSQL_H

#pragma once
#include<stack>
#include <queue>
#include <vector>
#include <afxinet.h>
#include "stdafx.h"

#import "C:\Program Files\Common Files\System\ADO\msado15.dll" named_guids rename("EOF","adoEOF"), rename("BOF","adoBOF")
#import "C:\Program Files\Common Files\System\ADO\Msadox.dll"  rename_namespace("ADOCG") rename("EOF", "adoXEOF") rename("DataTypeEnum","adoXDataTypeEnum") 

using namespace ADODB;
using namespace ADOCG;
using namespace std;

#define ARRAY_SIZE 80

struct DBInfo
{
	CString strDsn;
	CString strUid;
	CString strPwd;
	CString strDataName;
	CString strTableName;
	DBInfo()
	{
		strDsn = _T("server");
		strUid = _T("root");
		strPwd = _T("123");
		strDataName = _T("mysql");
		strTableName = _T("db");
	}

};

class CRWMysql
{
public:
	CRWMysql();
	~CRWMysql();


public:
	/// <summary>
	/// �������
	/// </summary>
	/// <param name="strData">��ӵ���������</param>
	/// <param name="strVal">����ֵ</param>
	int DBAddData(CString strData,CString strVal);


	/// <summary>
	/// ɾ������
	/// </summary>
	/// <param name="nid">ɾ��������</param>
	/// <returns></returns>
	int DBDeleteData(int nid); 

	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="strKeyName">���ҵ�����</param>
	/// <param name="strKey">���ҵ�����</param>
	/// <returns>��ֵ</returns>
	CString DBFindData(CString strKeyName,CString strKey);

	/// <summary>
	/// �޸�����
	/// </summary>
	/// <param name="strKeyName">�޸ĵ�����</param>
	/// <param name="strKey">�޸�ֵ</param>
	/// <param name="nid">����</param>
	/// <returns></returns>
	void DBAlterData(CString strKeyName,CString strKey,int nid);	

	/// <summary>
	/// �����ֿܷ�ʹ��2024/09/20
	/// </summary>
	BOOL ExecuteSQL(_bstr_t bstrSQL);					//����
	_RecordsetPtr& GetRecordSet(_bstr_t bstrSQL);		//�޸�
	void ExitADOConn();									//�ر�
	void OnInitADOConn();

private:
	_ConnectionPtr m_pCon;//���һ��ָ��Connection�����ָ��
	_RecordsetPtr m_pRs;  //���һ��ָ��Recordset�����ָ��
	_CommandPtr m_pCommanda;
	_variant_t RecordsAffected;

	DBInfo m_sDBInfo;

};

#endif // !_CRWMYSQL_H