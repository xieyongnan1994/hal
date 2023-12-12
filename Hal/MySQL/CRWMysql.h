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
