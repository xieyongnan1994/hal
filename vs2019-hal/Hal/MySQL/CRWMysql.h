//#pragma once
//#ifndef _CRWMYSQL_H
//#define _CRWMYSQL_H
//#include <mysql.h>
//#include "winsock.h"
//
////登录：mysql - u root - p 密码
////创建数据库： create database test;
////查看创建的数据库：show databases;
////使用创建的数据库：use test;
////查看当前使用的数据库：select database();
////创建数据表：create table students(
////	id int(4) primary key auto_increment, //设为主键 自动增加  
////	name varchar(40) not null,
////	age int(4) not null,
////	class varchar(40) not null);
////查看创建的数据表：desc students;
////添加数据：insert into student(name, age, class) values('姓名', '年龄', '班级');
////查询数据：select* from students;
////删除数据：delete from student where name = '姓名'
////更新数据：update student set age = '新年龄', class = '新班级' where name = '姓名';
////删除数据库：drop database test;
//class CRWMysql
//{
//public:
//	CRWMysql();
//	~CRWMysql();
//public:
//	BOOL MySQL_InitDataBase(CString strlocal, CString strroot, CString strpass, CString strdia, int nport); // 初始化MySQL数据库
//	BOOL MySQL_CreateTable(CString sql_create_table);//创建表
//	
//	BOOL MySQL_SelectData(char* sql_select);//查询数据
//
//	BOOL MySQL_InsertData(char* ctable, char* cfield, char* cvalue);//插入数据
//	BOOL MySQL_DeleteData(char* ctable, char* cfield, char* cvalue);//删除数据
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
	/// 添加数据
	/// </summary>
	/// <param name="strData">添加的数据名称</param>
	/// <param name="strVal">数据值</param>
	int DBAddData(CString strData,CString strVal);


	/// <summary>
	/// 删除数据
	/// </summary>
	/// <param name="nid">删除的行数</param>
	/// <returns></returns>
	int DBDeleteData(int nid); 

	/// <summary>
	/// 查找数据
	/// </summary>
	/// <param name="strKeyName">查找的列名</param>
	/// <param name="strKey">查找的列数</param>
	/// <returns>列值</returns>
	CString DBFindData(CString strKeyName,CString strKey);

	/// <summary>
	/// 修改数据
	/// </summary>
	/// <param name="strKeyName">修改的列名</param>
	/// <param name="strKey">修改值</param>
	/// <param name="nid">行数</param>
	/// <returns></returns>
	void DBAlterData(CString strKeyName,CString strKey,int nid);	

	/// <summary>
	/// 上下能分开使用2024/09/20
	/// </summary>
	BOOL ExecuteSQL(_bstr_t bstrSQL);					//插入
	_RecordsetPtr& GetRecordSet(_bstr_t bstrSQL);		//修改
	void ExitADOConn();									//关闭
	void OnInitADOConn();

private:
	_ConnectionPtr m_pCon;//添加一个指向Connection对象的指针
	_RecordsetPtr m_pRs;  //添加一个指向Recordset对象的指针
	_CommandPtr m_pCommanda;
	_variant_t RecordsAffected;

	DBInfo m_sDBInfo;

};

#endif // !_CRWMYSQL_H