#include <tchar.h>
#include "stdafx.h"
#include "CRWTable.h"

CApplication CRWTable::application;

COleVariant
covTrue((short)TRUE),
covFalse((short)FALSE),
covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

CRWTable::CRWTable()
{
	
}

CRWTable::~CRWTable()
{
}

void CRWTable::OpenTable(CString OpenPath)
{
	if (m_bServeStart)
		CloseTable();
	m_openFilePath = OpenPath;
	//����CRWTable ������(����CRWTable)
	if (!m_ExcelApp.CreateDispatch(_T("CRWTable.Application")))
	{
		AfxMessageBox(_T("����CRWTable����ʧ��"), MB_OK | MB_ICONWARNING);
		return;
	}
	m_bServeStart = TRUE;
	/*�жϵ�ǰCRWTable�İ汾*/
	CString strExcelVersion = m_ExcelApp.get_Version();//��ȡ�汾��Ϣ
	int iStart = 0;
	strExcelVersion = strExcelVersion.Tokenize(_T("."), iStart);
	if (_T("11") == strExcelVersion)
	{
		//AfxMessageBox("��ǰCRWTable�İ汾��2003��");
	}
	else if (_T("12") == strExcelVersion)
	{
		//AfxMessageBox("��ǰCRWTable�İ汾��2007��");
	}
	else
	{
		//AfxMessageBox("��ǰCRWTable�İ汾�������汾��");
	}
	m_ExcelApp.put_Visible(FALSE);
	m_ExcelApp.put_UserControl(FALSE);

	m_ExcelBooks.AttachDispatch(m_ExcelApp.get_Workbooks());//�õ�����������	
	try
	{
		m_ExcelBook = m_ExcelBooks.Open(m_openFilePath,
			vtMissing, vtMissing, vtMissing, vtMissing, vtMissing,
			vtMissing, vtMissing, vtMissing, vtMissing, vtMissing,
			vtMissing, vtMissing, vtMissing, vtMissing);
	}
	catch (...)
	{
		m_ExcelBook = m_ExcelBooks.Add(vtMissing);
		m_bNewTable = TRUE;
	}

	/*�õ��������е�Sheet������*/
	m_ExcelSheets.AttachDispatch(m_ExcelBook.get_Sheets());

	/*��һ��Sheet���粻���ڣ�������һ��Sheet*/
	CString strSheetName = _T("Sheet1");
	try
	{
		/*��һ�����е�Sheet*/
		m_ExcelSheet = m_ExcelSheets.get_Item(_variant_t(strSheetName));
	}
	catch (...)
	{
		/*����һ���µ�Sheet*/
		m_ExcelSheet = m_ExcelSheets.Add(vtMissing, vtMissing, _variant_t((long)1), vtMissing);
		m_ExcelSheet.put_Name(strSheetName);
	}
}

void CRWTable::WriteTable(CString clocow, CString strWrite)
{
	m_ExcelRange = m_ExcelSheet.get_Range(COleVariant(clocow), COleVariant(clocow));
	m_ExcelRange.put_Value2(COleVariant(strWrite));
}

void CRWTable::ReadTable(CString clocow, CString& strRead)
{
	variant_t rValue;
	m_ExcelRange = m_ExcelSheet.get_Range(COleVariant(clocow), COleVariant(clocow));
	rValue = m_ExcelRange.get_Value2();
	switch (rValue.vt)
	{
	case VT_R8:
		strRead.Format(_T("%f"), (float)rValue.dblVal);
		break;
	case VT_BSTR:
		strRead = rValue.bstrVal;
		break;
	case VT_I4:
		strRead.Format(_T("%ld"), (int)rValue.dblVal);
		break;
	default:
		break;
	}
}

void CRWTable::CloseTable()
{
	m_ExcelBook.put_Saved(TRUE);
	if (m_bNewTable)
		m_ExcelBook.SaveCopyAs(COleVariant(m_openFilePath));//���Ϊ
	else
		m_ExcelBook.Save();
	// �ͷŶ���
	m_ExcelBooks.ReleaseDispatch();
	m_ExcelBook.ReleaseDispatch();
	m_ExcelSheets.ReleaseDispatch();
	m_ExcelSheet.ReleaseDispatch();
	m_ExcelRange.ReleaseDispatch();
	m_ExcelApp.Quit();
	m_ExcelApp.ReleaseDispatch();

	m_bServeStart = FALSE;
}

void CRWTable::WriteCSV()
{
	CString strPath = _T(""), strit = _T(""), strTitle = _T("");
	CString strdefaultname = _T("");
	CTime t = CTime::GetCurrentTime();
	strdefaultname.Format(_T("OeeRecord-%d-%d-%d-%d-%d-%d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	CFileDialog fdlg(FALSE, _T("csv"), strdefaultname, OFN_OVERWRITEPROMPT, _T("Csv Files(*.csv)|*.csv|Txt Files(*.txt)|*.txt|All Files(*.*)|*.*||;"));
	fdlg.m_ofn.lpstrTitle = _T("��ѡ���ļ�:");
	if (fdlg.DoModal() == IDOK)
	{
		strPath = fdlg.GetPathName();
		if (strPath.IsEmpty())
		{
			return;
		}

		if (PathFileExists(strPath))
		{
			DeleteFile(strPath);
		}
		
		strTitle = _T("���,��ʼʱ��,�쳣����,�쳣����ʱ��,�쳣��Ϣ\n");

		int nbase = 0, nIsEnd = 0;
		while (nIsEnd == 0)
		{
			if (nbase == 0)
			{
				strit = strTitle;
			}
			else
			{
				strit = _T("");
			}
			CString strk = _T("");
			for (int i = 0; i < 1000; i++)
			{
				/*if ((nbase + i) < m_listDowntimeRecord.GetItemCount())
				{
					strk.Format(_T("%s,%s,%s,%s,%s,%s\n"), m_listDowntimeRecord.GetItemText(nbase + i, 0), m_listDowntimeRecord.GetItemText(nbase + i, 1), m_listDowntimeRecord.GetItemText(nbase + i, 2),
						m_listDowntimeRecord.GetItemText(nbase + i, 3), m_listDowntimeRecord.GetItemText(nbase + i, 4), m_listDowntimeRecord.GetItemText(nbase + i, 5));
					strit += strk;
				}
				else
				{
					nIsEnd = 1;
					break;
				}*/
			}

			ExportRecord(strit, strPath);

			nbase += 1000;
		}
	}
}
void CRWTable::ExportRecord(CString strmsg, CString strpath)
{
	CStdioFile file;
	if (file.Open(strpath, CStdioFile::modeNoTruncate | CStdioFile::modeCreate | CStdioFile::modeReadWrite | CStdioFile::typeText))	 //���ļ�
	{
		file.SeekToEnd();
		USES_CONVERSION;

		char* ss = T2A(strmsg);
		file.Write(ss, strlen(ss));
		file.Close();
	}
}

//��һ���Ķ���

void CRWTable::show(bool bShow)
{
	application.put_Visible(bShow);
	application.put_UserControl(bShow);
}

bool CRWTable::isCellString(long iRow, long iColumn)
{
	CRange range;
	range.AttachDispatch(m_ExcelRange.get_Item(COleVariant((long)iRow), COleVariant((long)iColumn)).pdispVal, true);
	COleVariant vResult = range.get_Value2();
	//VT_BSTR��ʾ�ַ���  
	if (vResult.vt == VT_BSTR)
	{
		return true;
	}
	return false;
}

bool CRWTable::isCellInt(long iRow, long iColumn)
{

	CRange range;
	range.AttachDispatch(m_ExcelRange.get_Item(COleVariant((long)iRow), COleVariant((long)iColumn)).pdispVal, true);
	COleVariant vResult = range.get_Value2();
	//VT_BSTR��ʾ�ַ���  
	if (vResult.vt == VT_INT || vResult.vt == VT_R8)
	{
		return true;
	}
	return false;
}

CString CRWTable::getCellString(long iRow, long iColumn)
{
	COleVariant vResult;
	CString str;
	//�ַ���  
	if (isLoad == false)
	{
		CRange range;
		range.AttachDispatch(m_ExcelRange.get_Item(COleVariant((long)iRow), COleVariant((long)iColumn)).pdispVal, true);
		vResult = range.get_Value2();
		range.ReleaseDispatch();
	}
	//�����������Ԥ�ȼ�����  
	else
	{
		long read_address[2];
		VARIANT val;
		read_address[0] = iRow;
		read_address[1] = iColumn;
		safeArray.GetElement(read_address, &val);
		vResult = val;
	}

	if (vResult.vt == VT_BSTR)
	{
		str = vResult.bstrVal;
	}
	//����  
	else if (vResult.vt == VT_INT)
	{
		str.Format(_T("%d"), vResult.pintVal);
	}
	//8�ֽڵ�����   
	else if (vResult.vt == VT_R8)
	{
		str.Format(_T("%0.0f"), vResult.dblVal);
	}
	//ʱ���ʽ  
	else if (vResult.vt == VT_DATE)
	{
		SYSTEMTIME st;
		VariantTimeToSystemTime(vResult.date, &st);
		CTime tm(st);
		str = tm.Format(_T("%Y-%m-%d"));

	}
	//��Ԫ��յ�  
	else if (vResult.vt == VT_EMPTY)
	{
		str = "";
	}

	return str;
}

int CRWTable::getCellInt(long iRow, long iColumn)
{
	int num;
	COleVariant vresult;

	if (isLoad == FALSE)
	{
		CRange range;
		range.AttachDispatch(m_ExcelRange.get_Item(COleVariant((long)iRow), COleVariant((long)iColumn)).pdispVal, true);
		vresult = range.get_Value2();
		range.ReleaseDispatch();
	}
	else
	{
		long read_address[2];
		VARIANT val;
		read_address[0] = iRow;
		read_address[1] = iColumn;
		safeArray.GetElement(read_address, &val);
		vresult = val;
	}
	//  
	num = static_cast<int>(vresult.dblVal);

	return num;
}

double CRWTable::getCellDouble(long iRow, long iColumn)
{
	double rtn_value = 0;
	COleVariant vresult;
	//�ַ���  
	if (isLoad == false)
	{
		CRange range;
		range.AttachDispatch(m_ExcelRange.get_Item(COleVariant((long)iRow), COleVariant((long)iColumn)).pdispVal, true);
		vresult = range.get_Value2();
		range.ReleaseDispatch();
	}
	//�����������Ԥ�ȼ�����  
	else
	{
		long read_address[2];
		VARIANT val;
		read_address[0] = iRow;
		read_address[1] = iColumn;
		safeArray.GetElement(read_address, &val);
		vresult = val;
	}

	if (vresult.vt == VT_R8)
	{
		rtn_value = vresult.dblVal;
	}

	return rtn_value;
}

int CRWTable::getRowCount()
{
	CRange range;
	CRange usedRange;

	usedRange.AttachDispatch(m_ExcelSheet.get_UsedRange(), true);
	range.AttachDispatch(usedRange.get_Rows(), true);

	int count = range.get_Count();

	usedRange.ReleaseDispatch();
	range.ReleaseDispatch();

	return count;
}

int CRWTable::getColumnCount()
{
	CRange range;
	CRange usedRange;

	usedRange.AttachDispatch(m_ExcelSheet.get_UsedRange(), true);
	range.AttachDispatch(usedRange.get_Columns(), true);
	int count = range.get_Count();

	usedRange.ReleaseDispatch();
	range.ReleaseDispatch();

	return count;
}
//�������Ƽ���sheet���Ҳ����ǰ�������б��
bool CRWTable::loadSheet(long tableId, bool preLoaded)
{
	LPDISPATCH lpDis = nullptr;
	m_ExcelRange.ReleaseDispatch();
	m_ExcelRange.ReleaseDispatch();
	lpDis = m_ExcelSheets.get_Item(COleVariant((long)tableId));
	if (lpDis)
	{
		m_ExcelSheet.AttachDispatch(lpDis, true);
		m_ExcelRange.AttachDispatch(m_ExcelSheet.get_Cells(), true);
	}
	else
	{
		return false;
	}

	isLoad = false;
	//�������Ԥ�ȼ���  
	if (preLoaded)
	{
		preLoadSheet();
		isLoad = true;
	}

	return true;
}

bool CRWTable::loadSheet(CString sheet, bool preLoaded)
{
	LPDISPATCH lpDis = nullptr;
	m_ExcelRange.ReleaseDispatch();
	m_ExcelRange.ReleaseDispatch();

	lpDis = m_ExcelSheets.get_Item(COleVariant(sheet));
	if (lpDis)
	{
		m_ExcelSheet.AttachDispatch(lpDis, true);
		m_ExcelRange.AttachDispatch(m_ExcelSheet.get_Cells(), true);
	}
	else
	{
		return false;
	}

	isLoad = false;
	//�������Ԥ�ȼ���  
	if (preLoaded)
	{
		preLoadSheet();
		isLoad = true;
	}

	return true;
}

CString CRWTable::getSheetName(long tableID)
{
	CWorksheet sheet;
	sheet.AttachDispatch(m_ExcelSheets.get_Item(COleVariant((long)tableID)));
	CString name = sheet.get_Name();
	sheet.ReleaseDispatch();
	return name;
}

int CRWTable::getSheetCount()
{
	return m_ExcelSheets.get_Count();
}

bool CRWTable::open(const char* fileName)
{

	//�ȹر��ļ�
	close();

	//����ģ�彨�����ĵ�
	m_ExcelBooks.AttachDispatch(application.get_Workbooks(), true);


	LPDISPATCH lpDis = nullptr;
	lpDis = m_ExcelBooks.Add(COleVariant(CString(fileName)));

	if (lpDis)
	{
		m_ExcelBook.AttachDispatch(lpDis);

		m_ExcelSheets.AttachDispatch(m_ExcelBook.get_Worksheets());

		openFileName = fileName;
		return true;
	}

	return false;
}

void CRWTable::close(bool ifSave)
{
	//����ļ��Ѿ��򿪣��ر��ļ�
	if (!openFileName.IsEmpty())
	{
		//�������,�����û�����,���û��Լ��棬����Լ�SAVE�������Ī���ĵȴ�  
		if (ifSave)
		{
			//show(true);
		}
		else
		{
			m_ExcelBook.Close(COleVariant(short(FALSE)), COleVariant(openFileName), covOptional);
			m_ExcelBooks.Close();
		}

		//��մ��ļ�����
		openFileName.Empty();
	}


	m_ExcelSheets.ReleaseDispatch();
	m_ExcelSheet.ReleaseDispatch();
	m_ExcelRange.ReleaseDispatch();
	m_ExcelBook.ReleaseDispatch();
	m_ExcelBooks.ReleaseDispatch();
}

void CRWTable::saveAsXLSFile(const CString& xlsFile)
{
	m_ExcelBook.SaveAs(COleVariant(xlsFile),
		covOptional,
		covOptional,
		covOptional,
		covOptional,
		covOptional,
		0,
		covOptional,
		covOptional,
		covOptional,
		covOptional,
		covOptional);
	return;
}

CString CRWTable::getOpenFileName()
{
	return openFileName;
}

CString CRWTable::getOpenSheelName()
{
	return m_ExcelSheet.get_Name();
}

void CRWTable::setCellInt(long iRow, long iColumn, int newInt)
{
	COleVariant new_value((long)newInt);
	CRange start_range = m_ExcelSheet.get_Range(COleVariant(_T("A1")), covOptional);
	CRange write_range = start_range.get_Offset(COleVariant((long)iRow - 1), COleVariant((long)iColumn - 1));
	write_range.put_Value2(new_value);
	start_range.ReleaseDispatch();
	write_range.ReleaseDispatch();
}

void CRWTable::setCellString(long iRow, long iColumn, CString newString)
{
	COleVariant new_value(newString);
	CRange start_range = m_ExcelSheet.get_Range(COleVariant(_T("A1")), covOptional);
	CRange write_range = start_range.get_Offset(COleVariant((long)iRow - 1), COleVariant((long)iColumn - 1));
	write_range.put_Value2(new_value);
	start_range.ReleaseDispatch();
	write_range.ReleaseDispatch();
}

bool CRWTable::initExcel()
{
	//����CRWTable 2000������(����CRWTable)   
	if (!application.CreateDispatch(_T("CRWTable.application"), nullptr))
	{
		MessageBox(nullptr, _T("����CRWTable����ʧ��,�����û�а�װEXCEL������!"), _T("����"), MB_OK);
		return FALSE;
	}

	application.put_DisplayAlerts(FALSE);
	return true;
}

void CRWTable::release()
{
	application.Quit();
	application.ReleaseDispatch();
	application = nullptr;
}

char* CRWTable::getColumnName(long iColumn)
{
	static char column_name[64];
	size_t str_len = 0;

	while (iColumn > 0)
	{
		int num_data = iColumn % 26;
		iColumn /= 26;
		if (num_data == 0)
		{
			num_data = 26;
			iColumn--;
		}
		column_name[str_len] = (char)((num_data - 1) + 'A');
		str_len++;
	}
	column_name[str_len] = '\0';
	//��ת  
	_strrev(column_name);

	return column_name;
}

void CRWTable::preLoadSheet()
{
	CRange used_range;

	used_range = m_ExcelSheet.get_UsedRange();


	VARIANT ret_ary = used_range.get_Value2();
	if (!(ret_ary.vt & VT_ARRAY))
	{
		return;
	}
	//  
	safeArray.Clear();
	safeArray.Attach(ret_ary);
}