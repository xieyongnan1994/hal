#include "CRWMysql.h"
//#include "pch.h"
//#include "CRWMysql.h"
//
//CRWMysql::CRWMysql()
//{
//}
//
//CRWMysql::~CRWMysql()
//{
//}
//
//	// strlocal:��������ַ������ֱ������IP��
//	// strroot:�˺ţ�
//	// strpass:���룻
//	// strdia:���ݿ�����
//	// 3306:����˿�
//BOOL CRWMysql::MySQL_InitDataBase(CString strlocal, CString strroot, CString strpass, CString strdia, int nport)
//{
//	USES_CONVERSION;
//	mysql_init(&m_sqlCon);
//	if (NULL == mysql_real_connect(&m_sqlCon, T2A(strlocal), T2A(strroot), T2A(strpass), T2A(strdia), nport, NULL, 0))
//	{
//		AfxMessageBox(_T("���ݿ�����ʧ�ܣ�"));
//		mysql_close(&m_sqlCon);
//		return FALSE;
//	}
//	else // MySQL���ӳɹ�������������ݿ�
//	{
//		AfxMessageBox(_T("���ݿ����ӳɹ���"));
//
//		mysql_set_character_set(&m_sqlCon, "gb2312"); //�����ֽڷ�����ֹ��������.
//		return TRUE;
//	}
//}
//BOOL CRWMysql::MySQL_CreateTable(CString sql_create_table)
//{
//	//�������UserInfo������������
//	/*CString sql_create_table = _T("CREATE TABLE IF NOT EXISTS UserInfo(\
//		id       INT PRIMARY KEY NOT NULL,\
//		Account  VARCHAR(20) NOT NULL UNIQUE,\
//		Pass     VARCHAR(20)    NOT NULL\
//		);\
//		");*/
//	char buffer[512];
//	wcstombs(buffer, sql_create_table.GetBuffer(), sizeof(buffer));
//	int result = mysql_query(&m_sqlCon, buffer); // ִ�в�ѯ���
//	sql_create_table.ReleaseBuffer();
//	if (result != 0)
//	{
//		AfxMessageBox(_T("����ʧ�ܣ�"));
//		mysql_close(&m_sqlCon);
//		return FALSE;
//	}
//	//AfxMessageBox(_T("���ݿ����б�"));
//	return TRUE;
//}
//
//BOOL CRWMysql::MySQL_SelectData(char* sql_select)
//{
//	// ��ѯ����֮ǰ�����list control�е���������
//	//m_List.DeleteAllItems(); //������б���
//		MYSQL_RES* res; // �������ݲ�ѯ���
//		MYSQL_ROW row; // ȡÿ�н���Ķ���
//		mysql_real_query(&m_sqlCon, sql_select, strlen(sql_select)); // ��ѯ������������ַ�����ʾ
//		res = mysql_store_result(&m_sqlCon); // �õ���ѯ���
//
//		//�ظ���ȡ�У������ݷ����б��У�ֱ��rowΪNULL
//		//
//		int selectCount = 0; // ��¼��ѯ����ĸ���������ѯ���Ϊ�գ�Ҳ����0
//		while (row = mysql_fetch_row(res))
//		{
//			CString strId, strAccount, strPass;
//			strId = row[0];
//			strAccount = row[1];
//			strPass = row[2];
//
//			// ��list control�в�������
//			/*m_List.InsertItem(selectCount, strId);
//			m_List.SetItemText(selectCount, 1, strAccount);
//			m_List.SetItemText(selectCount, 2, strPass);*/
//
//			selectCount++;
//		}
//		mysql_close(&m_sqlCon);
//
//		if (selectCount == 0) return FALSE; // û��ѯ������
//		else return TRUE;
//}
//// MySQL��������
//BOOL CRWMysql::MySQL_InsertData(char* ctable, char* cfield, char* cvalue)
//{
//	CString strSQL;
//	strSQL.Format(_T("insert into \'%s\'(\'%s\')' values(/'%s/')"), ctable, cfield, cvalue);
//	if (mysql_query(&m_sqlCon, (char*)(LPCTSTR)strSQL) == 0)
//	{
//		return TRUE;// ����ɹ�
//	}
//	return FALSE; // ����ʧ��
//
//}
//
//BOOL CRWMysql::MySQL_DeleteData(char* ctable, char* cfield, char* cvalue)
//{
//	CString strSQL;
//	strSQL.Format(_T("delete from \'%s\' where \'%s\'=\'%s\'"), ctable, cfield, cvalue);
//	if (mysql_real_query(&m_sqlCon, (char*)(LPCTSTR)strSQL, (UINT)strSQL.GetLength()) != 0)
//	{
//		return FALSE;
//	}
//	return TRUE;
//}
//
//void CRWMysql::CloseMySQL()
//{
//	mysql_close(&m_sqlCon);
//}
//

CRWMysql::CRWMysql()
{
	CoInitialize(NULL);
}

CRWMysql::~CRWMysql()
{
	ExitADOConn();
}


BOOL CRWMysql::ExecuteSQL(_bstr_t bstrSQL)
{
	try
	{
		if (m_pCon == NULL)
			OnInitADOConn();
		m_pCon->Execute(bstrSQL, NULL, adCmdText);
		return true;
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
}

_RecordsetPtr& CRWMysql::GetRecordSet(_bstr_t bstrSQL)
{
	try
	{
		if (m_pCon == NULL)
			OnInitADOConn();
		m_pRs.CreateInstance("ADODB.Recordset");
		m_pRs->Open(bstrSQL, m_pCon.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
	return m_pRs;
}

void CRWMysql::ExitADOConn()
{
	if (m_pRs != NULL)      //���жϼ�¼���Ƿ�Ϊ��
		m_pRs->Close();  //��Ϊ�����ȹرռ�¼��
	m_pCon->Close();     //�رջ���
	CoUninitialize();
}

void CRWMysql::OnInitADOConn()
{
	CString DBCONNSTR = _T("");
	DBCONNSTR.Format(_T("Dsn=%s;UID=%s;PWD=%s;db=%s"),m_sDBInfo.strDsn,m_sDBInfo.strUid,m_sDBInfo.strPwd,m_sDBInfo.strDataName);
	try
	{
		::CoInitialize(NULL);
		m_pCon.CreateInstance("ADODB.Connection");//����Connection����
		m_pCon->ConnectionTimeout = 5;//����������ʱ
		//m_pCon->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=..\\data\\animal.mdb","","",adModeUnknown);
		m_pCon->Open(_bstr_t(DBCONNSTR), "", "", adModeUnknown);
	}
	catch (_com_error e)//�����쳣
	{
		AfxMessageBox(e.Description());//��ʾ������Ϣ
	}
}

int CRWMysql::DBAddData(CString strData, CString strVal)
{
	CString strCmd=_T("");
	strCmd.Format(_T("INSERT INTO %s(%s) value(%s);"),m_sDBInfo.strTableName, strData, strVal);
	
	OnInitADOConn();
	m_pCommanda.CreateInstance(__uuidof(Command));
	m_pCommanda->CommandTimeout = 10;
	m_pCommanda->ActiveConnection = m_pCon;
	m_pCon->Execute(_bstr_t(strCmd), &RecordsAffected, adCmdText);
	m_pCon->Close();
	::CoUninitialize();

	return 0;
}

int CRWMysql::DBDeleteData(int nid)
{
	CString strCmd = _T("");
	strCmd.Format(_T("delete from %s where nid=%d;"), m_sDBInfo.strTableName, nid);
	//strCmd.Format(_T("alter table %s drop %s;"), m_sDBInfo.strTableName, ����);
	OnInitADOConn();
	m_pCommanda.CreateInstance(__uuidof(Command));
	m_pCommanda->CommandTimeout = 10;
	m_pCommanda->ActiveConnection = m_pCon;
	m_pCon->Execute(_bstr_t(strCmd), &RecordsAffected, adCmdText);
	m_pCon->Close();
	::CoUninitialize();

	return 0;
}

CString CRWMysql::DBFindData(CString strKeyName, CString strKey)
{
	CString strCmd = _T(""),strRet=_T("");
	strCmd.Format(_T("select * from %s where %s='%s';"), m_sDBInfo.strTableName,strKeyName, strKey);
	//strCmd.Format(_T("alter table %s drop %s;"), m_sDBInfo.strTableName, ����);
	OnInitADOConn();
	m_pRs.CreateInstance(__uuidof(Command));
	m_pRs->Open(_bstr_t(strCmd), m_pCon.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	if (!m_pRs->adoEOF && m_pRs != NULL)
	{
		m_pRs->MoveFirst();
		strRet = (CString)m_pRs->GetCollect((_variant_t)(LPCTSTR)strKeyName);
	}
	m_pCon->Close();
	::CoUninitialize();
	return strRet;
}
void CRWMysql::DBAlterData(CString strKeyName, CString strKey, int nid)
{
	CString strCmd = _T("");
	strCmd.Format(_T("update %s set %s='%s' where id = %d;"), m_sDBInfo.strTableName, strKeyName, strKey, nid);
	OnInitADOConn();
	m_pCommanda.CreateInstance(__uuidof(Command));
	m_pCommanda->CommandTimeout = 10;
	m_pCommanda->ActiveConnection = m_pCon;
	m_pCon->Execute(_bstr_t(strCmd), &RecordsAffected, adCmdText);
	m_pCon->Close();
	::CoUninitialize();
}