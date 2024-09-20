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
		//	AfxMessageBox(_T("OLE��ʼ��ʧ��!"));
		//}
		ms_bInitState = 1;
	}


	if (bPassWordUseOrNot)  //�ж����ݿ��Ƿ�ʹ�������
	{
		SetPassWordState(USEPASSWORD);
	}
	else
	{
		SetPassWordState(NOTUSEPASSWORD);
	}
}

void CDataBase::CompactDB(CString Path)		//ѹ���޸����ݿ�
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
			::MessageBox(NULL,_T("�������ݿ�����ʧ�ܣ�������"),_T("����ʧ��"), MB_OK);
			return false;
		}
		m_pConnection->Open((_bstr_t)l_NameTemp, _T(""), _T(""), ADODB::adModeUnknown);
	}
	catch (_com_error e)
	{
		MessageBox(NULL,(_T("���ݿ�����ʧ�ܣ�ȷ�����ݿ�") + Path + _T("�Ƿ����")), _T("����ʧ��"), MB_OK);
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
		m_pRecordset->Open((_variant_t)(_T("SELECT * FROM ") + FormName),       // ��ѯDemoTable���������ֶ�
			m_pConnection.GetInterfacePtr(),	 // ��ȡ��ӿ��IDispatchָ��
			ADODB::adOpenDynamic,
			ADODB::adLockOptimistic,
			ADODB::adCmdText);
	}
	catch (_com_error e)
	{
		MessageBox(NULL,e.ErrorMessage(), _T("������Ϣ"), MB_OK);

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
	//	::AfxMessageBox(_T("���ݿ����ʧ�ܣ�"));
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
			//	::AfxMessageBox(_T("û����һ����"));
			//	return NULL;
			//}
		}
	}
	//}
	//catch(_com_error e)
	//{
	//	::AfxMessageBox(_T("���ݿ����ʧ�ܣ�"));
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
		fields->Release();//�ͷ�ָ�� 
		fields = NULL;
	}
	return l_nColCount;
}

void CDataBase::ClrCurSheet()
{
	if (!m_pRecordset->BOF)			//������ݿⲻΪ��
		m_pRecordset->MoveFirst();
	// ѭ��ɾ�����ݿ�������
	while (!m_pRecordset->adoEOF)
	{
		m_pRecordset->Delete(ADODB::adAffectCurrent);
		m_pRecordset->MoveNext();
	}
}