#include "CSystemStatus.h"

CSystemStatus::CSystemStatus()
{
	m_Query = NULL;
	m_CpuTotal = NULL;
	m_DiskRead = NULL;
	m_DiskWrite = NULL;
	m_NetDownload = NULL;
	m_NetUpload = NULL;
	//SystemInit();
}

CSystemStatus::~CSystemStatus()
{
	if (m_CpuTotal) {
		PdhRemoveCounter(m_CpuTotal);
		m_CpuTotal = NULL;
	}
	if (m_CpuTotal) {
		PdhRemoveCounter(m_CpuTotal);
		m_CpuTotal = NULL;
	}
	if (m_DiskRead) {
		PdhRemoveCounter(m_DiskRead);
		m_DiskRead = NULL;
	}
	if (m_DiskWrite) {
		PdhRemoveCounter(m_DiskWrite);
		m_DiskWrite = NULL;
	}
	if (m_NetDownload)
	{
		PdhRemoveCounter(m_NetDownload);
		m_NetDownload = NULL;
	}
	if (m_NetUpload)
	{
		PdhRemoveCounter(m_NetUpload);
		m_NetUpload = NULL;
	}
	if (m_Query) {
		PdhCloseQuery(m_Query);
		m_Query = NULL;
	}
	//SystemUnInit();
}
//��ʼ��
void CSystemStatus::SystemInit(DWORD object)
{
	PDH_STATUS state;
	state = PdhOpenQuery(NULL, NULL, &m_Query);

	if (object & SYSSTATE_CPU_USAGE)	//CPUʹ����
		state = PdhAddCounter(m_Query, _T("\\Processor(_Total)\\% Processor Time"), NULL, &m_CpuTotal);
	if (object & SYSSTATE_DISK_READ)	//���̶��ٶ�
		state = PdhAddCounter(m_Query, _T("\\PhysicalDisk(_Total)\\Disk Read Bytes/sec"), NULL, &m_DiskRead);
	if (object & SYSSTATE_DISK_WRITE)	//����д�ٶ�
		state = PdhAddCounter(m_Query, _T("\\PhysicalDisk(_Total)\\Disk Write Bytes/sec"), NULL, &m_DiskWrite);
	if (object & SYSSTATE_NET_DOWNLOAD)//���������ٶ�
		state = PdhAddCounter(m_Query, _T("\\Network Interface(killer E2200 Gigabit Ethernet Controller)\\Bytes Received/sec"), NULL, &m_NetDownload);
	if (object & SYSSTATE_NET_UPLOAD)	//�����ϴ��ٶ�
		state = PdhAddCounter(m_Query, _T("\\Network Interface(killer E2200 Gigabit Ethernet Controller)\\Bytes Sent/sec"), NULL, &m_NetUpload);

	state = PdhCollectQueryData(m_Query);
	Sleep(500);             //����Ҫ����ʱ��Ȼ����൱��׼ȷ  
	state = PdhCollectQueryData(m_Query);
}

//��ȡ���������ٶȣ�K/s��
double CSystemStatus::GetSystemNetDownloadRate()
{
	PDH_FMT_COUNTERVALUE counterVal;
	PDH_STATUS lStatus = PdhGetFormattedCounterValue(m_NetDownload, PDH_FMT_DOUBLE, NULL, &counterVal);
	if (ERROR_SUCCESS != lStatus)  return -1;
	double dbVal = counterVal.doubleValue;
	double NetDownload = (double)((dbVal / (1024 * 1.0f)));
	return NetDownload;
}

//��ȡ�����ϴ��ٶ�
double CSystemStatus::GetSystemNetUploadRate()
{
	PDH_FMT_COUNTERVALUE counterVal;
	PDH_STATUS lStatus = PdhGetFormattedCounterValue(m_NetUpload, PDH_FMT_DOUBLE, NULL, &counterVal);
	if (ERROR_SUCCESS != lStatus)  return -1;
	double dbVal = counterVal.doubleValue;
	double NetUpload = (double)((dbVal / (1024 * 1.0f)));
	return NetUpload;
}

//��ȡϵͳ��ǰ���̶�����(K/s)
double CSystemStatus::GetSystemDiskReadRate()
{
	PDH_FMT_COUNTERVALUE counterVal;
	PDH_STATUS lStatus = PdhGetFormattedCounterValue(m_DiskRead, PDH_FMT_DOUBLE, NULL, &counterVal);
	if (ERROR_SUCCESS != lStatus)  return -1;
	double dbVal = counterVal.doubleValue;
	double dbDiskRead = (double)((dbVal / (1024 * 1.0f)));
	return dbDiskRead;
}

//��ȡϵͳ��ǰ����д����
double CSystemStatus::GetSystemDiskWriteRate()
{
	PDH_FMT_COUNTERVALUE counterVal;
	PDH_STATUS lStatus = PdhGetFormattedCounterValue(m_DiskWrite, PDH_FMT_DOUBLE, NULL, &counterVal);
	if (ERROR_SUCCESS != lStatus)  return -1;
	double dbVal = counterVal.doubleValue;
	double dbDiskWrite = (double)((dbVal / (1024 * 1.0f)));
	return dbDiskWrite;
}

//��ȡCPUʹ��ֵ
double CSystemStatus::GetSystemCpuCurrentUsage()
{
	PDH_FMT_COUNTERVALUE counterVal;
	// CPUʱ�䣬ע��������PDH_FMT_NOCAP100������������CPU��������
	PDH_STATUS lStatus = PdhGetFormattedCounterValue(m_CpuTotal, PDH_FMT_DOUBLE | PDH_FMT_NOCAP100, NULL, &counterVal);
	if (ERROR_SUCCESS != lStatus)  return -1;
	return counterVal.doubleValue;//PDH_INVALID_ARGUMENT PDH_INVALID_DATA 
}

//����ʼ��
void CSystemStatus::SystemUnInit()
{
	if (m_CpuTotal) {
		PdhRemoveCounter(m_CpuTotal);
		m_CpuTotal = NULL;
	}
	if (m_DiskRead) {
		PdhRemoveCounter(m_DiskRead);
		m_DiskRead = NULL;
	}
	if (m_DiskWrite) {
		PdhRemoveCounter(m_DiskWrite);
		m_DiskWrite = NULL;
	}
	if (m_NetDownload)
	{
		PdhRemoveCounter(m_NetDownload);
		m_NetDownload = NULL;
	}
	if (m_NetUpload)
	{
		PdhRemoveCounter(m_NetUpload);
		m_NetUpload = NULL;
	}
	if (m_Query) {
		PdhCloseQuery(m_Query);
		m_Query = NULL;
	}
}

// ��ȡϵͳ����Ӳ��ʹ�����		
void CSystemStatus::GetSystemDiskStatus(ULONGLONG& AllDiskTotal, ULONGLONG& AllDiskFree)
{
	int DType = 0;
	int si = 0;
	BOOL bResult = FALSE;
	int DiskCount = 0;
	ULONGLONG nTempTotal = 0;
	ULONGLONG nTempFree = 0;

	ULONGLONG i64FreeBytesToCaller;
	ULONGLONG i64TotalBytes;
	ULONGLONG i64FreeBytes;

	//ö�ٴ�������
	DWORD DiskInfo = GetLogicalDrives();
	while (DiskInfo) {
		if (DiskInfo & 1) {
			++DiskCount;
		}
		DiskInfo = DiskInfo >> 1;
	}
	int test = DiskCount;
	int DSLength = GetLogicalDriveStrings(0, NULL);

	TCHAR* DStr = NULL;
	while (!DStr) {
		DStr = new TCHAR[DSLength];
	}
	if (NULL == DStr) {
		return;
	}
	int nRet = GetLogicalDriveStrings(DSLength, (LPTSTR)DStr);

	//ö�ٴ�������
	for (int i = 0; i < DSLength / 4; i++) {
		TCHAR strDisk[3] = { 0 };
		_stprintf(strDisk, _T("%c:"), DStr[si]);
		DType = GetDriveType(DStr + i * 4);
		bResult = GetDiskFreeSpaceEx(
			strDisk,
			(PULARGE_INTEGER)&i64FreeBytesToCaller,
			(PULARGE_INTEGER)&i64TotalBytes,
			(PULARGE_INTEGER)&i64FreeBytes);

		if (bResult) {
			nTempTotal += (ULONGLONG)i64TotalBytes / 1024 / 1024 / 1024;
			nTempFree += (ULONGLONG)i64FreeBytesToCaller / 1024 / 1024 / 1024;
		}
		else {
			OutputDebugString(_T("�豸δ׼��..."));
		}
		si += 4;
	}
	AllDiskTotal = nTempTotal;
	AllDiskFree = nTempFree;

	delete[] DStr;
}

// ��ȡϵͳ����Ӳ��ʹ�����		
void CSystemStatus::GetSystemDiskStatus(std::vector<EACHDISKSTATUS>& vectorDisk)
{
	int DType = 0;
	int si = 0;
	BOOL bResult = FALSE;
	int DiskCount = 0;
	ULONGLONG nTempTotal = 0;
	ULONGLONG nTempFree = 0;

	ULONGLONG i64FreeBytesToCaller;
	ULONGLONG i64TotalBytes;
	ULONGLONG i64FreeBytes;

	//ö�ٴ�������
	DWORD DiskInfo = GetLogicalDrives();
	while (DiskInfo) {
		if (DiskInfo & 1) {
			++DiskCount;
		}
		DiskInfo = DiskInfo >> 1;
	}
	int test = DiskCount;
	int DSLength = GetLogicalDriveStrings(0, NULL);

	TCHAR* DStr = NULL;
	while (!DStr) {
		DStr = new TCHAR[DSLength];
	}
	if (NULL == DStr) {
		return;
	}
	int nRet = GetLogicalDriveStrings(DSLength, (LPTSTR)DStr);

	EACHDISKSTATUS diskstatus;
	//ö�ٴ�������
	for (int i = 0; i < DSLength / 4; i++) {
		TCHAR strDisk[3] = { 0 };
		_stprintf(strDisk, _T("%c:"), DStr[si]);
		DType = GetDriveType(DStr + i * 4);
		bResult = GetDiskFreeSpaceEx(
			strDisk,
			(PULARGE_INTEGER)&i64FreeBytesToCaller,
			(PULARGE_INTEGER)&i64TotalBytes,
			(PULARGE_INTEGER)&i64FreeBytes);

		if (bResult)
		{
			diskstatus.strdir = strDisk[0];
			double total = ((i64TotalBytes / 1024 / 1024) * 100) / 1024;
			diskstatus._Total = total / 100.0f;
			double OfFree = ((i64FreeBytesToCaller / 1024 / 1024) * 100) / 1024;
			diskstatus._OfFree = OfFree / 100.0f;
			vectorDisk.push_back(diskstatus);
		}
		else
		{
			OutputDebugString(_T("�豸δ׼��..."));
		}
		si += 4;
	}

	delete[] DStr;
}

//��ȡϵͳ��ǰ����״̬
void CSystemStatus::GetSystemCurrentDiskStatus(ULONGLONG& TatolMB, ULONGLONG& FreeCaller)
{
	BOOL bResult = FALSE;
	ULONGLONG nTempTotal = 0;
	ULONGLONG nTempFree = 0;

	ULONGLONG ui64FreeBytesToCaller;
	ULONGLONG ui64TotalBytes;
	ULONGLONG ui64FreeBytes;

	TCHAR szModule[MAX_PATH * 2] = { 0 };
	GetModuleFileName(NULL, szModule, MAX_PATH * 2);
#ifdef _UNICODE
	std::wstring strPath(szModule);
	std::wstring strDirve = strPath.substr(0, 1);
#else
	CString strPath(szModule);
	CString strDirve = strPath.substr(0, 1);
#endif
	TCHAR szDisk[3] = { 0 };
	_stprintf(szDisk, _T("%c:"), strDirve[0]);
	bResult = GetDiskFreeSpaceEx(
		szDisk,
		(PULARGE_INTEGER)&ui64FreeBytesToCaller,	//�û����õĴ��̿ռ�
		(PULARGE_INTEGER)&ui64TotalBytes,			//�����ܹ��Ŀռ�
		(PULARGE_INTEGER)&ui64FreeBytes);			//���̿��еĿռ�.���϶����ֽ�Ϊ��λ��
	if (bResult) {
		nTempTotal = ui64TotalBytes / 1024 / 1024 / 1024;
		nTempFree = ui64FreeBytesToCaller / 1024 / 1024 / 1024;
		TatolMB = nTempTotal;
		FreeCaller = nTempFree;
	}
}

//��ȡϵͳ��ǰ����ʹ����
double CSystemStatus::GetSystemCurrentDiskUsage()
{
	BOOL bResult = FALSE;
	ULONGLONG ui64FreeBytesToCaller;
	ULONGLONG ui64TotalBytes;
	ULONGLONG ui64FreeBytes;

	TCHAR szModule[MAX_PATH * 2] = { 0 };
	GetModuleFileName(NULL, szModule, MAX_PATH * 2);
	::PathRemoveFileSpec(szModule);
#ifdef _UNICODE
	std::wstring strPath(szModule);
	std::wstring strDirve = strPath.substr(0, 1);
#else
	CString strPath(szModule);
	CString strDirve = strPath.substr(0, 1);
#endif

	TCHAR szDisk[3] = { 0 };
	_stprintf(szDisk, _T("%c:"), strDirve[0]);
	bResult = GetDiskFreeSpaceEx(
		szDisk,
		(PULARGE_INTEGER)&ui64FreeBytesToCaller,	//�û����õĴ��̿ռ�
		(PULARGE_INTEGER)&ui64TotalBytes,			//�����ܹ��Ŀռ�
		(PULARGE_INTEGER)&ui64FreeBytes);			//���̿��еĿռ�.���϶����ֽ�Ϊ��λ��
	if (bResult) {
		double TempNum = ((ui64TotalBytes / 1024 / 1024 / 1024) - (ui64FreeBytesToCaller / 1024 / 1024 / 1024)) * 100.0 / (ui64TotalBytes / 1024 / 1024 / 1024);
		return TempNum;
	}
	return 0;
}

//�����ڴ��ʹ��
BOOL CSystemStatus::GetPhysicalMemoryState(ULONGLONG& totalPhysMem, ULONGLONG& physMemUsed)
{
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	totalPhysMem = memInfo.ullTotalPhys;
	physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
	return TRUE;
}

//���������ڴ�����(�ܿ����ڴ�)
double CSystemStatus::GetTotalPhysicalMemory()
{
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	double totalPhysMem = ((memInfo.ullTotalPhys / 1024 / 1024) * 100) / 1024;
	double fTemptotal = totalPhysMem / 100.0;
	return fTemptotal;
}

//���������ڴ�����(�����ڴ�)
double CSystemStatus::GetTotalPhysicalMemoryFree()
{
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	double physMemFree = (((memInfo.ullAvailPhys) / 1024 / 1024) * 100) / 1024;;
	double fTemMemFree = physMemFree / 100.0;
	return fTemMemFree;
}

//���������ڴ�ʹ��(��ʹ���ڴ�)
double CSystemStatus::GetTotalPhysicalMemoryUsed()
{
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	double physMemUsed = (((memInfo.ullTotalPhys - memInfo.ullAvailPhys) / 1024 / 1024) * 100) / 1024;;
	double fTemMemUsed = physMemUsed / 100.0;
	return fTemMemUsed;
}

//���������ڴ�ʹ���� 
double CSystemStatus::GetPhysicalMemoryUsage()
{
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	double MemUsage = memInfo.dwMemoryLoad;
	return MemUsage;
}
//��ȡ������Ϣ���ڻ�ȡ����ʱ��Ҫ�õ�����������
void CSystemStatus::GetNetCardInfo(std::vector<NETCARDINFO>& vectorNetCard)
{

	NETCARDINFO NetCardInfo;

	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	//�õ��ṹ���С,����GetAdaptersInfo����
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	//����GetAdaptersInfo����,���pIpAdapterInfoָ�����;����stSize��������һ��������Ҳ��һ�������
	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	//��¼��������
	int netCardNum = 0;
	//��¼ÿ�������ϵ�IP��ַ����
	int IPnumPerNetCard = 0;
	if (ERROR_BUFFER_OVERFLOW == nRel)
	{
		//����������ص���ERROR_BUFFER_OVERFLOW
		//��˵��GetAdaptersInfo�������ݵ��ڴ�ռ䲻��,ͬʱ�䴫��stSize,��ʾ��Ҫ�Ŀռ��С
		//��Ҳ��˵��ΪʲôstSize����һ��������Ҳ��һ�������
		//�ͷ�ԭ�����ڴ�ռ�
		delete pIpAdapterInfo;
		//���������ڴ�ռ������洢����������Ϣ
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		//�ٴε���GetAdaptersInfo����,���pIpAdapterInfoָ�����
		nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	}
	if (ERROR_SUCCESS == nRel)
	{
		//���������Ϣ
		//�����ж�����,���ͨ��ѭ��ȥ�ж�
		while (pIpAdapterInfo)
		{
			++netCardNum;				//��������

			NetCardInfo.Name = pIpAdapterInfo->AdapterName;		//��������
			NetCardInfo.Description = pIpAdapterInfo->Description;//��������

			switch (pIpAdapterInfo->Type)//��������
			{
			case MIB_IF_TYPE_OTHER:break;
			case MIB_IF_TYPE_ETHERNET:break;
			case MIB_IF_TYPE_TOKENRING:break;
			case MIB_IF_TYPE_FDDI:break;
			case MIB_IF_TYPE_PPP:break;
			case MIB_IF_TYPE_LOOPBACK:break;
			case MIB_IF_TYPE_SLIP:break;
			}
			//����MAC��ַ
			char tempchar[4];
			for (DWORD i = 0; i < pIpAdapterInfo->AddressLength; i++)
				if (i < pIpAdapterInfo->AddressLength - 1)
				{
					printf("%02X-", pIpAdapterInfo->Address[i]);

					sprintf(tempchar, "%02X-", pIpAdapterInfo->Address[i]);
					NetCardInfo.Local_Mac += tempchar;


				}
				else
				{
					printf("%02X", pIpAdapterInfo->Address[i]);

					sprintf(tempchar, "%02X", pIpAdapterInfo->Address[i]);
					NetCardInfo.Local_Mac += tempchar;

				}

			//����IP��ַ
			//���������ж�IP,���ͨ��ѭ��ȥ�ж�
			IP_ADDR_STRING* pIpAddrString = &(pIpAdapterInfo->IpAddressList);
			do
			{
				//cout<<"�������ϵ�IP������"<<++IPnumPerNetCard<<endl;
				//cout<<"IP ��ַ��"<<pIpAddrString->IpAddress.String<<endl;
				//cout<<"������ַ��"<<pIpAddrString->IpMask.String<<endl;
				//cout<<"���ص�ַ��"<<pIpAdapterInfo->GatewayList.IpAddress.String<<endl;

				NetCardInfo.Local_IP = pIpAddrString->IpAddress.String;

				pIpAddrString = pIpAddrString->Next;
			} while (pIpAddrString);

			vectorNetCard.push_back(NetCardInfo);

			pIpAdapterInfo = pIpAdapterInfo->Next;
		}

	}
	if (pIpAdapterInfo)
	{
		delete pIpAdapterInfo;

		pIpAdapterInfo = NULL;
	}
}
//��ȡ����ϵͳ��Ϣ 
void CSystemStatus::GetOsInfo(CString& osinfo)
{
	// get os name according to version number
	SYSTEM_INFO system_info;
	memset(&system_info, 0, sizeof(SYSTEM_INFO));
	GetSystemInfo(&system_info);

	OSVERSIONINFOEX  osver = { sizeof(OSVERSIONINFO) };
	osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx((OSVERSIONINFO*)&osver);
	CString os_name;
	if (osver.dwMajorVersion == 4 && osver.dwMinorVersion == 0)
	{
		if (osver.dwPlatformId == VER_PLATFORM_WIN32_NT)
			os_name =_T( "Microsoft Windows NT 4.0 ");
		else if (osver.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
			os_name = _T("Microsoft Windows 95 ");
	}
	else if (osver.dwMajorVersion == 4 && osver.dwMinorVersion == 10)
		os_name = _T("Microsoft Windows 98");
	else if (osver.dwMajorVersion == 4 && osver.dwMinorVersion == 90)
		os_name = _T("Microsoft Windows Me");
	else if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 0)
	{
		os_name = _T("Microsoft Windows 2000");
		if (osver.wSuiteMask == VER_SUITE_ENTERPRISE)
			os_name += _T(" Advanced Server ");
	}
	else if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 1)
	{
		os_name = _T("Microsoft Windows XP");

		if (osver.wSuiteMask == VER_SUITE_EMBEDDEDNT)
			os_name += _T(" Embedded ");
		else if (osver.wSuiteMask == VER_SUITE_PERSONAL)
			os_name += _T(" Home Edition ");
		else
			os_name += _T(" Professional ");
	}
	else if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 2)
	{
		//os_name = "Microsoft Windows vista";
		if (osver.wProductType == VER_NT_WORKSTATION && system_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
			os_name = _T("Microsoft Windows XP Professional x64 Edition ");
		if (GetSystemMetrics(SM_SERVERR2) == 0 && osver.wSuiteMask == VER_SUITE_BLADE)
			os_name = _T("Microsoft Windows Server 2003 Web Edition ");
		else if (GetSystemMetrics(SM_SERVERR2) == 0 && osver.wSuiteMask == VER_SUITE_COMPUTE_SERVER)
			os_name = _T("Microsoft Windows Server 2003 Compute Cluster Edition ");
		else if (GetSystemMetrics(SM_SERVERR2) == 0 && osver.wSuiteMask == VER_SUITE_STORAGE_SERVER)
			os_name = _T("Microsoft Windows Server 2003 Storage Server ");
		else if (GetSystemMetrics(SM_SERVERR2) == 0 && osver.wSuiteMask == VER_SUITE_DATACENTER)
			os_name = _T("Microsoft Windows Server 2003 Datacenter Edition ");
		else if (GetSystemMetrics(SM_SERVERR2) == 0 && osver.wSuiteMask == VER_SUITE_ENTERPRISE)
			os_name = _T("Microsoft Windows Server 2003 Enterprise Edition ");
		else if (GetSystemMetrics(SM_SERVERR2) != 0 && osver.wSuiteMask == VER_SUITE_STORAGE_SERVER)
			os_name = _T("Microsoft Windows Server 2003 R2 Storage Server ");
	}
	else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 0)
	{
		//os_name = "Microsoft Windows 2003";

		if (osver.wProductType == VER_NT_WORKSTATION)
		{
			os_name = _T("Microsoft Windows Vista ");
			if (osver.wSuiteMask == VER_SUITE_PERSONAL)
				os_name += _T(" Home");
		}
		else if (osver.wProductType != VER_NT_WORKSTATION)
		{
			os_name = _T("Microsoft Windows Server 2008 ");
			if (osver.wSuiteMask == VER_SUITE_DATACENTER)
				os_name += _T(" Datacenter Server ");
			else if (osver.wSuiteMask == VER_SUITE_ENTERPRISE)
				os_name += _T(" Enterprise ");
		}
	}
	else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 1)
	{
		os_name = _T("Microsoft Windows 7.0");
		/*if(osver.wProductType==VER_NT_WORKSTATION)
		os_name="Microsoft Windows 7 ";
		else
		os_name="Microsoft Windows Server 2008 R2 ";*/
	}
	else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 2)
	{
		os_name = _T("Microsoft Windows 8.0");
		/*if (osver.wProductType == VER_NT_WORKSTATION)
		os_name = "Microsoft Windows 8";
		else
		os_name = "Microsoft Windows Server 2012";*/
	}
	else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 3)
	{
		os_name = _T("Microsoft Windows 8.1");
		/*if (osver.wProductType == VER_NT_WORKSTATION)
		os_name = "Microsoft Windows 8.1";
		else
		os_name = "Microsoft Windows Server 2012 R2";*/
	}
	else if (osver.dwMajorVersion == 10 && osver.dwMinorVersion == 0)
	{
		os_name = _T("Microsoft Windows 10.0");
		/*	if (osver.wProductType == VER_NT_WORKSTATION)
		os_name = "Microsoft Windows 10.0";
		else
		os_name = "Microsoft Windows Server 2016";*/
	}

	SYSTEM_INFO si;
	GetNativeSystemInfo(&si);
	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
		si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
		os_name += _T(" X64");
	else os_name += _T(" x86");

	osinfo = os_name;
}

//��ȡCPUӲ����Ϣ 
void CSystemStatus::GetCpuInfo(CString& CPUinfo)
{
	int cpuInfo[4] = { -1 };
	char cpu_manufacture[32] = { 0 };
	char cpu_type[32] = { 0 };
	char cpu_freq[32] = { 0 };

	__cpuid(cpuInfo, 0x80000002);
	memcpy(cpu_manufacture, cpuInfo, sizeof(cpuInfo));

	__cpuid(cpuInfo, 0x80000003);
	memcpy(cpu_type, cpuInfo, sizeof(cpuInfo));

	__cpuid(cpuInfo, 0x80000004);
	memcpy(cpu_freq, cpuInfo, sizeof(cpuInfo));

	CString manufacture = cpu_manufacture;//������
	CString type = cpu_type;				//�ͺ�
	CString freq = cpu_freq;				//Ƶ��
	CPUinfo = manufacture + type + freq;
}

//��ȡCPUid
void CSystemStatus::GetCPUid(CString& CPUid)
{
	unsigned long s1, s2, s3, s4;
	__asm
	{
		mov eax, 00h
		xor edx, edx
		cpuid
		mov s1, edx
		mov s2, eax
	}
	__asm
	{
		mov eax, 01h
		xor ecx, ecx
		xor edx, edx
		cpuid
		mov s3, edx
		mov s4, ecx
	}

	CString st;
	st.Format(_T("%08X%08X%08X%08X"), s1, s2, s3, s4);
	CPUid = st.GetBuffer(0);
	st.ReleaseBuffer();
}



BOOL CSystemStatus::GetHDSerial(CString& HDSerial)
{
	BOOL		bRtn = FALSE;
	DWORD		bytesRtn = 0;
	char		szhd[80] = { 0 };
	PIDSECTOR	phdinfo;
	HANDLE		hDrive = NULL;
	GETVERSIONOUTPARAMS vers;
	SENDCMDINPARAMS		in;
	SENDCMDOUTPARAMS	out;

	ZeroMemory(&vers, sizeof(vers));
	ZeroMemory(&in, sizeof(in));
	ZeroMemory(&out, sizeof(out));

	//�����ĸ�����Ӳ�̣�ȡ��һ�������ݵ�����Ӳ�� 
	for (int j = 0; j < 4; j++)
	{
		sprintf(szhd, "\\\\.\\PhysicalDrive%d", j);
		hDrive = CreateFileA(szhd,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			0,
			OPEN_EXISTING,
			0,
			0);
		if (NULL == hDrive)continue;

		if (!DeviceIoControl(hDrive, DFP_GET_VERSION, 0, 0, &vers, sizeof(vers), &bytesRtn, 0))goto FOREND;
		//If IDE identify command not supported, fails 
		if (!(vers.fCapabilities & 1))goto FOREND;

		//Identify the IDE drives 
		if (j & 1)in.irDriveRegs.bDriveHeadReg = 0xb0;
		else in.irDriveRegs.bDriveHeadReg = 0xa0;

		if (vers.fCapabilities & (16 >> j))goto FOREND;	//We don't detect a ATAPI device. 
		else in.irDriveRegs.bCommandReg = 0xec;

		in.bDriveNumber = j;
		in.irDriveRegs.bSectorCountReg = 1;
		in.irDriveRegs.bSectorNumberReg = 1;
		in.cBufferSize = 512;

		if (!DeviceIoControl(hDrive, DFP_RECEIVE_DRIVE_DATA, &in, sizeof(in), &out, sizeof(out), &bytesRtn, 0))
		{
			//"DeviceIoControl failed:DFP_RECEIVE_DRIVE_DATA"<<endl; 
			goto FOREND;
		}
		phdinfo = (PIDSECTOR)out.bBuffer;

		char	s[21] = { 0 };
		memcpy(s, phdinfo->sSerialNumber, 20);
		s[20] = 0;
		ChangeByteOrder(s, 20);

		//ɾ���ո��ַ�
		int ix = 0;
		for (ix = 0; ix < 20; ix++)
		{
			if (s[ix] == ' ')continue;
			break;
		}
		char lpszHD[128] = { 0 };
		memcpy(lpszHD, s + ix, 20);

		HDSerial = lpszHD;

		bRtn = TRUE;
		if (bRtn)break;
	FOREND:
		CloseHandle(hDrive);
		hDrive = NULL;
	}

	CloseHandle(hDrive);
	hDrive = NULL;
	return(bRtn);
}

CString CSystemStatus::GetPcName()
{
	WCHAR n[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD d = MAX_COMPUTERNAME_LENGTH + 1;
	GetComputerName(n, &d);

	CString str = _T("");
	str.Format(_T("%s"), n);
	return str;
}

CString CSystemStatus::GetPcMac()
{
	char mac[200] = { 0 };
	NCB ncb;
	typedef struct _ASTAT_
	{
		ADAPTER_STATUS   adapt;
		NAME_BUFFER   NameBuff[30];
	}ASTAT, * PASTAT;

	ASTAT Adapter;

	typedef struct _LANA_ENUM
	{
		UCHAR   length;
		UCHAR   lana[MAX_LANA];
	}LANA_ENUM;

	LANA_ENUM lana_enum;
	UCHAR uRetCode;
	memset(&ncb, 0, sizeof(ncb));
	memset(&lana_enum, 0, sizeof(lana_enum));
	ncb.ncb_command = NCBENUM;
	ncb.ncb_buffer = (unsigned char*)&lana_enum;
	ncb.ncb_length = sizeof(LANA_ENUM);
	uRetCode = Netbios(&ncb);

	if (uRetCode != NRC_GOODRET)
		return CString("");

	for (int lana = 0; lana < lana_enum.length; lana++)
	{
		ncb.ncb_command = NCBRESET;
		ncb.ncb_lana_num = lana_enum.lana[lana];
		uRetCode = Netbios(&ncb);
		if (uRetCode == NRC_GOODRET)
			break;
	}

	if (uRetCode != NRC_GOODRET)
		return CString("");

	memset(&ncb, 0, sizeof(ncb));
	ncb.ncb_command = NCBASTAT;
	ncb.ncb_lana_num = lana_enum.lana[0];
	strcpy_s((char*)ncb.ncb_callname, 2, "*");
	ncb.ncb_buffer = (unsigned char*)&Adapter;
	ncb.ncb_length = sizeof(Adapter);
	uRetCode = Netbios(&ncb);

	if (uRetCode != NRC_GOODRET)
		return CString("");

	sprintf_s(mac, 200, "%02X-%02X-%02X-%02X-%02X-%02X",
		Adapter.adapt.adapter_address[0],
		Adapter.adapt.adapter_address[1],
		Adapter.adapt.adapter_address[2],
		Adapter.adapt.adapter_address[3],
		Adapter.adapt.adapter_address[4],
		Adapter.adapt.adapter_address[5]);

	return CString(mac);
}

CString CSystemStatus::GetHardDiskSerialNumber()
{
	CString strSerialNumber;

	HANDLE hFile = CreateFile(_T("\\\\.\\PHYSICALDRIVE0"), 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return strSerialNumber;

	DWORD retn;
	static BYTE buffer[0x1000];
	STORAGE_DEVICE_DESCRIPTOR* descriptor = (STORAGE_DEVICE_DESCRIPTOR*)buffer;
	STORAGE_PROPERTY_QUERY p = { StorageDeviceProperty };
	if (!DeviceIoControl(hFile, IOCTL_STORAGE_QUERY_PROPERTY, &p, sizeof(p), buffer, sizeof(buffer), &retn, 0))
		return strSerialNumber;

	if (descriptor->SerialNumberOffset > 0)
	{
		char Serial[256];
		BYTE* data;
		data = buffer + descriptor->SerialNumberOffset;
		sprintf_s(Serial, 256, "%s", data);
		std::string SerialNum = Serial;
		//ɾ��Ӳ�����к����˵Ŀո�
		SerialNum.erase(0, SerialNum.find_first_not_of(" "));
		SerialNum.erase(SerialNum.find_last_not_of(" ") + 1);
		strSerialNumber = SerialNum.c_str();
	}

	return strSerialNumber;
}

CString CSystemStatus::GetBaseBoardSerialNumber()
{
	CString result1 = _T("");
	const long MAX_COMMAND_SIZE = 10000; // ��������������С	
	WCHAR szFetCmd[] = _T("wmic BaseBoard get SerialNumber"); // ��ȡ�������к�������	
	CString strEnSearch;
	strEnSearch.Format(_T("SerialNumber")); // �������кŵ�ǰ����Ϣ


	char			szReadResult[MAX_COMMAND_SIZE + 1] = { 0 }; // ���������н�������������
	CString			strBuffer;
	unsigned long	count = 0;
	long			ipos = 0;

	BOOL   bret = FALSE;
	HANDLE hReadPipe = NULL; //��ȡ�ܵ�
	HANDLE hWritePipe = NULL; //д��ܵ�	
	PROCESS_INFORMATION pi;   //������Ϣ	
	STARTUPINFO			si;	  //���������д�����Ϣ
	SECURITY_ATTRIBUTES sa;   //��ȫ����

	memset(&pi, 0, sizeof(pi));
	memset(&si, 0, sizeof(si));
	memset(&sa, 0, sizeof(sa));

	pi.hProcess = NULL;
	pi.hThread = NULL;
	si.cb = sizeof(STARTUPINFO);
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	//1.0 �����ܵ�
	bret = CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
	if (!bret)
	{
		goto END;
	}

	//2.0 ���������д��ڵ���ϢΪָ���Ķ�д�ܵ�
	GetStartupInfo(&si);
	si.hStdError = hWritePipe;
	si.hStdOutput = hWritePipe;
	si.wShowWindow = SW_HIDE; //���������д���
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	//3.0 ������ȡ�����еĽ���
	bret = CreateProcess(NULL, szFetCmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
	if (!bret)
	{
		goto END;
	}

	//4.0 ��ȡ���ص�����
	WaitForSingleObject(pi.hProcess, 500/*INFINITE*/);
	bret = ReadFile(hReadPipe, szReadResult, MAX_COMMAND_SIZE, &count, 0);
	if (!bret)
	{
		goto END;
	}

	//5.0 �����������к�
	bret = FALSE;
	strBuffer = szReadResult;
	ipos = strBuffer.Find(strEnSearch);
	if (ipos < 0) // û���ҵ��������кŵ�ǰ����Ϣ����û���ҵ��������к�
	{
		goto END;
	}
	else
	{
		//����ȥ��ǰ����Ϣ
		strBuffer = strBuffer.Right(strBuffer.GetLength() - strEnSearch.GetLength());
		//Ȼ��ȥ���ո�
		result1 = strBuffer.Trim();
	}
	bret = TRUE;
END:
	//�ر����еľ��
	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return result1;
}

int CSystemStatus::GetTraceIP(CString& strIP, CString& m_StrIDC_EDIT1)
{
	CString str;
	char szText[1024];
	int m_return = -1;
	//��ȡ������������
	int iRet;
	iRet = gethostname(szText, 1024);
	int a = WSAGetLastError();
	if (iRet != 0)
	{
		//printf("gethostname() Failed!");
		return m_return;
	}
	//ͨ����������ȡ����ַ��Ϣ
	HOSTENT* host = gethostbyname(szText);
	if (NULL == host)
	{
		//printf("gethostbyname() Failed!");
		return m_return;
	}
	in_addr PcAddr;
	for (int i = 0;; i++)
	{
		char* p = host->h_addr_list[i];
		if (NULL == p)
		{
			break;
		}
		memcpy(&(PcAddr.S_un.S_addr), p, host->h_length);
		char* szIP = ::inet_ntoa(PcAddr);
		//printf("������ip��ַ�ǣ�%s\n",szIP);
		//strIP.Format(strIP,szIP);
		str = (CString)szIP;
		if (m_StrIDC_EDIT1 != (_T("")))
		{
			int index_start = str.Find(m_StrIDC_EDIT1);
			if (index_start > -1)
			{
				strIP = str;
				return 1;
			}
		}
		else
		{
			strIP.Format(_T("%s,%s"), strIP, str);
			m_return = 2;
		}

	}
	return m_return;
}

CString CSystemStatus::GetTraceMac(CString strip)
{
	CString str =_T("");
	ULONG DestIP;
	ULONG PMAC[3];
	ULONG MACLEN = 6;
	DWORD dwRetVal;
	USES_CONVERSION;
	char* pFileName = T2A(strip);
	DestIP = inet_addr(pFileName);

	if ((dwRetVal = SendARP(DestIP, 0, &PMAC, &MACLEN)) == ERROR_SUCCESS)
	{
		PBYTE pHexMAC = (PBYTE)PMAC;
		str.Format(_T("%02X-%02X-%02X-%02X-%02X-%02X"), pHexMAC[0], pHexMAC[1], pHexMAC[2], pHexMAC[3], pHexMAC[4], pHexMAC[5]);
		return str;
	}
	return str;
}

CString CSystemStatus::GetSystemName()
{
	SYSTEM_INFO info;        //��SYSTEM_INFO�ṹ�ж�64λAMD������ 
	GetSystemInfo(&info);    //����GetSystemInfo�������ṹ 
	OSVERSIONINFOEX os;
	os.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	CString strosname = _T("unknown OperatingSystem.");

	if (GetVersionEx((OSVERSIONINFO*)&os))
	{
		//������ݰ汾��Ϣ�жϲ���ϵͳ���� 
		switch (os.dwMajorVersion)//�ж����汾��
		{
		case 4:
			switch (os.dwMinorVersion)//�жϴΰ汾�� 
			{
			case 0:
				if (os.dwPlatformId == VER_PLATFORM_WIN32_NT)
					strosname = _T("Microsoft Windows NT 4.0"); //1996��7�·��� 
				else if (os.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
					strosname = _T("Microsoft Windows 95");
				break;
			case 10:
				strosname = _T("Microsoft Windows 98");
				break;
			case 90:
				strosname = _T("Microsoft Windows Me");
				break;
			}
			break;

		case 5:
			switch (os.dwMinorVersion)	//�ٱȽ�dwMinorVersion��ֵ
			{
			case 0:
				strosname = _T("Microsoft Windows 2000");//1999��12�·���
				break;

			case 1:
				strosname = _T("Microsoft Windows XP");//2001��8�·���
				break;

			case 2:
				if (os.wProductType == VER_NT_WORKSTATION
					&& info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
				{
					strosname = _T("Microsoft Windows XP Professional x64 Edition");
				}
				else if (GetSystemMetrics(SM_SERVERR2) == 0)
					strosname = _T("Microsoft Windows Server 2003");//2003��3�·��� 
				else if (GetSystemMetrics(SM_SERVERR2) != 0)
					strosname = _T("Microsoft Windows Server 2003 R2");
				break;
			}
			break;

		case 6:
			switch (os.dwMinorVersion)
			{
			case 0:
				if (os.wProductType == VER_NT_WORKSTATION)
					strosname = _T("Microsoft Windows Vista");
				else
					strosname = _T("Microsoft Windows Server 2008");//�������汾 
				break;
			case 1:
				if (os.wProductType == VER_NT_WORKSTATION)
					strosname = _T("Microsoft Windows 7");
				else
					strosname = _T("Microsoft Windows Server 2008 R2");
				break;
			case 2:
				if (os.wProductType == VER_NT_WORKSTATION)
					strosname = _T("Microsoft Windows 8");
				else
					strosname = _T("Microsoft Windows Server 2012");
				break;
			case 3:
				if (os.wProductType == VER_NT_WORKSTATION)
					strosname = _T("Microsoft Windows 8.1");
				else
					strosname = _T("Microsoft Windows Server 2012 R2");
				break;
			}
			break;

		case 10:
			switch (os.dwMinorVersion)
			{
			case 0:
				if (os.wProductType == VER_NT_WORKSTATION)
					strosname = _T("Microsoft Windows 10");
				else
					strosname = _T("Microsoft Windows Server 2016 Technical Preview");//�������汾 
				break;
			}
			break;
		}
	}
	return strosname;
}

bool CSystemStatus::IsInternetBar()				// �Ƿ�Ϊ����
{
	char* NetRoomKey[] =
	{
		"SOFTWARE\\Hintsoft\\PubwinClient",
		"SOFTWARE\\Hintsoft\\pubwin",
		"SOFTWARE\\Sicent\\wx2004Clt",
		"SOFTWARE\\MpSoft\\smenu",
		"SOFTWARE\\Microsoft\\jingzu",
		"SOFTWARE\\iCafe8",
		"SOFTWARE\\Goyoo\\i8desk",
		"SOFTWARE\\Hintsoft1\\XunShanPro",
		"SOFTWARE\\SyncExpertNetBar",
		"SOFTWARE\\EYOOCLIENTSTATUS",
		"SOFTWARE\\Richtech",
		"SOFTWARE\\\u676d\u5dde\u987a\u7f51\u4fe1\u606f\u6280\u672f\u6709\u9650\u516c\u53f8\\\u7f51\u7ef4\u5927\u5e08\u874c\u86aa\u5a31\u4e50\u5e73\u53f0",
		"SOFTWARE\\iCafe8",
		"SOFTWARE\\Sicent\\wx2004Clt",
		"SOFTWARE\\Sicent\\WxAdv",
		"SOFTWARE\\Grabsun\\Netsense",
	};

	for (int i = 0; i < sizeof(NetRoomKey) / sizeof(char*); i++)
	{
		if (IsHaveKey(HKEY_LOCAL_MACHINE, NetRoomKey[i]))
		{
			m_bIsInternetBar = true;
			break;
		}
	}

	return m_bIsInternetBar;
}
bool CSystemStatus::IsHaveKey(HKEY hKey, char* lpSubKey)
{
	HKEY phkResult = 0;
	if (ERROR_SUCCESS == RegOpenKeyExA(hKey, lpSubKey, 0, KEY_QUERY_VALUE, &phkResult))
	{
		RegCloseKey(phkResult);
		return true;
	}
	return false;
}
