#include "CRWAccess.h"

bool CDataBase::ms_bInitState = 0;

CDataBase::CDataBase()
{

}

CDataBase::~CDataBase()
{

}

void CDataBase::AccessInit(bool bPassWordUseOrNot)
{

	if (!ms_bInitState)
	{
		//CoInitialize(NULL);
		//if (!AfxOleInit())                          
		//{
		//	AfxMessageBox(_T("OLE初始化失败!"));
		//}
		ms_bInitState = 1;
	}


	if (bPassWordUseOrNot)  //判断数据库是否使用密码打开
	{
		SetPassWordState(USEPASSWORD);
	}
	else
	{
		SetPassWordState(NOTUSEPASSWORD);
	}
}

void CDataBase::CompactDB(CString Path)		//压缩修复数据库
{
	//CString l_NameTemp;
	//CString l_NameTemp2;
	//
	//l_NameTemp = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=") + Path;
	//l_NameTemp2 = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=tmp.mdb");
	// try
	// {
	// JRO::IJetEnginePtr jet(__uuidof(JRO::JetEngine));
	// jet->CompactDatabase(
	//  (LPCTSTR)l_NameTemp, (LPCTSTR)l_NameTemp2);
	// }
	// catch(_com_error e) 
	// {    

	//	 AfxMessageBox((LPCTSTR)e.Description( ));
	// }

	// CopyFile(_T("tmp.mdb"),Path,FALSE);
	// DeleteFile(_T("tmp.mdb"));
}

bool CDataBase::Connect2DataBase(CString Path)
{
	DisconnectDataBase();
	try
	{
		CString l_NameTemp;
		if (GetPassWordState())
		{
			l_NameTemp = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=") + Path + _T(";Jet OLEDB:DataBase password=")
				+ GetPassWord();
		}
		else
		{
			l_NameTemp = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=") + Path;
		}

		m_pConnection.CreateInstance(__uuidof(ADODB::Connection));
		if (m_pConnection == NULL)
		{
			::MessageBox(NULL,_T("创建数据库连接失败，请重试"),_T("创建失败"), MB_OK);
			return false;
		}
		m_pConnection->Open((_bstr_t)l_NameTemp, _T(""), _T(""), ADODB::adModeUnknown);
	}
	catch (_com_error e)
	{
		MessageBox(NULL,(_T("数据库连接失败，确认数据库") + Path + _T("是否存在")), _T("连接失败"), MB_OK);
		return false;
	}

	return true;
}

void CDataBase::DisconnectDataBase(void)
{
	if (m_pConnection)
	{
		if (m_pConnection->GetState())
		{
			//m_pRecordset.Close();
			m_pConnection->Close();
			m_pConnection.Release();

		}
		m_pConnection = NULL;
	}
}

BOOL CDataBase::OpenForm(CString FormName)
{
	try
	{

		m_pRecordset.CreateInstance(__uuidof(ADODB::Recordset));
		m_pRecordset->Open((_variant_t)(_T("SELECT * FROM ") + FormName),       // 查询DemoTable表中所有字段
			m_pConnection.GetInterfacePtr(),	 // 获取库接库的IDispatch指针
			ADODB::adOpenDynamic,
			ADODB::adLockOptimistic,
			ADODB::adCmdText);
	}
	catch (_com_error e)
	{
		MessageBox(NULL,e.ErrorMessage(), _T("报错消息"), MB_OK);

		return FALSE;
	}

	return TRUE;
}

void CDataBase::SetPassWordState(bool bNewState)
{
	m_bPassWordEn = bNewState;
}

bool CDataBase::GetPassWordState(void)
{
	return m_bPassWordEn;
}

void CDataBase::SetPassWord(CString PassWord)
{
	m_PassWord = PassWord;
}

CString CDataBase::GetPassWord(void)
{
	return m_PassWord;
}

void CDataBase::WriteData(variant_t DataField, variant_t Content)
{
	if (m_pRecordset != NULL)
	{
		m_pRecordset->PutCollect(DataField, Content);
		m_pRecordset->Update();
	}
}

void CDataBase::WriteDataEx(variant_t DataField, variant_t Content, int nRow)
{
	if (m_pRecordset != NULL)
	{
		if (!m_pRecordset->BOF)
		{
			m_pRecordset->MoveFirst();
			for (int i = 0; i < nRow; i++)
			{
				m_pRecordset->MoveNext();
			}
			m_pRecordset->PutCollect(DataField, Content);
			m_pRecordset->Update();
		}
	}
}


variant_t CDataBase::ReadData(variant_t Index)
{
	//variant_t Ret;
	//try{
	//	Ret = m_pRecordset->GetCollect(Index);
	//}
	//catch(::_com_error e)
	//{
	//	::AfxMessageBox(_T("数据库操作失败！"));
	//	Ret = NULL;
	//}
	//return Ret;
	return m_pRecordset->GetCollect(Index);
}

variant_t CDataBase::ReadDataEx(variant_t Index, int nRow)
{
	//variant_t Ret;
	//try
	//{
	if (!m_pRecordset->BOF)
	{
		m_pRecordset->MoveFirst();
		for (int i = 0; i < nRow; i++)
		{
			//try
			//{
			m_pRecordset->MoveNext();
			//}
			//catch(_com_error e)
			//{
			//	::AfxMessageBox(_T("没有下一行了"));
			//	return NULL;
			//}
		}
	}
	//}
	//catch(_com_error e)
	//{
	//	::AfxMessageBox(_T("数据库操作失败！"));
	//	return NULL;
	//}
	return m_pRecordset->GetCollect(Index);
}

int CDataBase::GetRowCount()
{
	int l_nCount = 0;

	if (m_pRecordset != NULL)
	{
		if (!m_pRecordset->BOF)
		{
			m_pRecordset->MoveFirst();
			for (; !m_pRecordset->adoEOF; m_pRecordset->MoveNext())
			{
				l_nCount++;
			}
		}
	}

	return l_nCount;
}
int CDataBase::GetColCount()
{
	long l_nColCount = 0;
	HRESULT hr;
	ADODB::Fields* fields = NULL;

	if (m_pRecordset != NULL)
	{
		hr = m_pRecordset->get_Fields(&fields);
		if (SUCCEEDED(hr))
		{
			fields->get_Count(&l_nColCount);
		}
		fields->Release();//释放指针 
		fields = NULL;
	}
	return l_nColCount;
}

void CDataBase::ClrCurSheet()
{
	if (!m_pRecordset->BOF)			//如果数据库不为空
		m_pRecordset->MoveFirst();
	// 循环删除数据库中内容
	while (!m_pRecordset->adoEOF)
	{
		m_pRecordset->Delete(ADODB::adAffectCurrent);
		m_pRecordset->MoveNext();
	}
}