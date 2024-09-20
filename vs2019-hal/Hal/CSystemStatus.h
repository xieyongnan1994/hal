#pragma once
#ifndef __CSYSTEMSTATUS_H__
#define __CSYSTEMSTATUS_H__
#include<iostream>
#include <sstream>
#include <string>
#include <atlstr.h>
#include <vector>
#include <Shlwapi.h>
#include <Iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib") 
#pragma comment(lib, "Pdh.lib")
#pragma comment(lib, "Shlwapi.lib")

typedef struct tagDISKSTATUS
{
	DWORD	_FreeAvalidToCaller;	//�������ɵ����߿��õ��ֽ����� BYTE
	DWORD	_TotalNummber;			//�������ɴ����ϵ����ֽ���	BYTE
	DWORD	_TotalNummberOfFree;	//�������ɴ����Ͽ��õ��ֽ���	BYTE
}DISKSTATUS, * LPDISKSTATUS;
//���д��̵�״̬
typedef struct tagAllDISKSTATUS
{
	UINT	_DiskCount;				//��������
	DWORD	_Total;					//���д���������MB
	DWORD	_OfFree;				//���д���ʣ������MB
}AllDISKSTATUS, * LPAllDISKSTATUS;

typedef struct tagEACHDISKSTATUS
{
	CString strdir;					//��������
	float	_Total;					//����������MB
	float	_OfFree;				//����ʣ������MB
}EACHDISKSTATUS, * LPEACHDISKSTATUS;

typedef struct tagNETCARDINFO
{
	CString Name;				//��������
	CString Description;		//��������
	CString Local_IP;			//IP��ַ
	CString Local_Mac;			//MAC��ַ
}NETCARDINFO, * LPNETCARDINFO;


#define SYSSTATE_NONE			0x00000000
#define SYSSTATE_CPU_USAGE		0x00000001
#define SYSSTATE_DISK_READ		0x00000002
#define SYSSTATE_DISK_WRITE		0x00000004
#define SYSSTATE_NET_DOWNLOAD	0x00000008
#define SYSSTATE_NET_UPLOAD		0x00000010

typedef struct _GETVERSIONOUTPARAMS
{
	BYTE bVersion; // Binary driver version. 
	BYTE bRevision; // Binary driver revision. 
	BYTE bReserved; // Not used. 
	BYTE bIDEDeviceMap; // Bit map of IDE devices. 
	DWORD fCapabilities; // Bit mask of driver capabilities. 
	DWORD dwReserved[4]; // For future use. 
} GETVERSIONOUTPARAMS, * PGETVERSIONOUTPARAMS, * LPGETVERSIONOUTPARAMS;

//typedef struct _IDEREGS
//{
//	BYTE bFeaturesReg; // Used for specifying SMART "commands". 
//	BYTE bSectorCountReg; // IDE sector count register 
//	BYTE bSectorNumberReg; // IDE sector number register 
//	BYTE bCylLowReg; // IDE low order cylinder value 
//	BYTE bCylHighReg; // IDE high order cylinder value 
//	BYTE bDriveHeadReg; // IDE drive/head register 
//	BYTE bCommandReg; // Actual IDE command. 
//	BYTE bReserved; // reserved for future use. Must be zero. 
//} IDEREGS, * PIDEREGS, * LPIDEREGS;

//typedef struct _SENDCMDINPARAMS
//{
//	DWORD cBufferSize; // Buffer size in bytes 
//	IDEREGS irDriveRegs; // Structure with drive register values. 
//	BYTE bDriveNumber; // Physical drive number to send command to (0,1,2,3). 
//	BYTE bReserved[3]; // Reserved for future expansion. 
//	DWORD dwReserved[4]; // For future use. 
//} SENDCMDINPARAMS, * PSENDCMDINPARAMS, * LPSENDCMDINPARAMS;

//typedef struct _DRIVERSTATUS
//{
//	BYTE bDriverError; // Error code from driver, 
//	// or 0 if no error. 
//	BYTE bIDEStatus; // Contents of IDE Error register. 
//	// Only valid when bDriverError 
//	// is SMART_IDE_ERROR. 
//	BYTE bReserved[2]; // Reserved for future expansion. 
//	DWORD dwReserved[2]; // Reserved for future expansion. 
//} DRIVERSTATUS, * PDRIVERSTATUS, * LPDRIVERSTATUS;

//typedef struct _SENDCMDOUTPARAMS
//{
//	DWORD cBufferSize; // Size of bBuffer in bytes 
//	DRIVERSTATUS DriverStatus; // Driver status structure. 
//	BYTE bBuffer[512]; // Buffer of arbitrary length 
//	// in which to store the data read from the drive. 
//} SENDCMDOUTPARAMS, * PSENDCMDOUTPARAMS, * LPSENDCMDOUTPARAMS;

typedef struct _IDSECTOR
{
	USHORT wGenConfig;
	USHORT wNumCyls;
	USHORT wReserved;
	USHORT wNumHeads;
	USHORT wBytesPerTrack;
	USHORT wBytesPerSector;
	USHORT wSectorsPerTrack;
	USHORT wVendorUnique[3];
	CHAR sSerialNumber[20];
	USHORT wBufferType;
	USHORT wBufferSize;
	USHORT wECCSize;
	CHAR sFirmwareRev[8];
	CHAR sModelNumber[40];
	USHORT wMoreVendorUnique;
	USHORT wDoubleWordIO;
	USHORT wCapabilities;
	USHORT wReserved1;
	USHORT wPIOTiming;
	USHORT wDMATiming;
	USHORT wBS;
	USHORT wNumCurrentCyls;
	USHORT wNumCurrentHeads;
	USHORT wNumCurrentSectorsPerTrack;
	ULONG ulCurrentSectorCapacity;
	USHORT wMultSectorStuff;
	ULONG ulTotalAddressableSectors;
	USHORT wSingleWordDMA;
	USHORT wMultiWordDMA;
	BYTE bReserved[128];
} IDSECTOR, * PIDSECTOR;

#define DFP_GET_VERSION			0x00074080 
#define DFP_SEND_DRIVE_COMMAND	0x0007c084 
#define DFP_RECEIVE_DRIVE_DATA	0x0007c088 

#include "pdh.h"

class CSystemStatus
{
public:
	CSystemStatus();
	~CSystemStatus();
public:
	void		SystemInit(DWORD object = SYSSTATE_CPU_USAGE);							//ϵͳ��ʼ��(��ʼ�������Ŀʱʹ�û���������)
	void		SystemUnInit();															//�ͷ���Դ
	double		GetSystemNetDownloadRate();												//��ȡ���������ٶ�
	double		GetSystemNetUploadRate();												//��ȡ�����ϴ��ٶ�
	double		GetSystemDiskReadRate();												//��ȡ��ǰ���̶�����
	double		GetSystemDiskWriteRate();												//��ȡ��ǰ����д����
	double		GetSystemCpuCurrentUsage();												//ϵͳCPUʹ����

	void		GetSystemDiskStatus(std::vector<EACHDISKSTATUS>& vectorDisk);           //��ȡ��������ʹ��״̬
	void		GetSystemDiskStatus(ULONGLONG& AllDiskTotal, ULONGLONG& AllDiskFree);	//��ȡϵͳ�ܵô���ʹ��״̬
	void		GetSystemCurrentDiskStatus(ULONGLONG& TatolMB, ULONGLONG& FreeCaller);	//��ȡ��ǰ����ʹ��״̬
	double		GetSystemCurrentDiskUsage();											//��ȡ��ǰ����ʹ����

	BOOL		GetPhysicalMemoryState(ULONGLONG& totalPhysMem, ULONGLONG& physMemUsed);//��ȡ�����ڴ�״̬
	double		GetTotalPhysicalMemory();												//��ȡ�����ڴ��С
	double		GetTotalPhysicalMemoryFree();											//��ȡ�����ڴ�
	double		GetTotalPhysicalMemoryUsed();											//��ȡ��ʹ���ڴ��С
	double		GetPhysicalMemoryUsage();												//��ȡ�ڴ�ʹ����

	void		GetNetCardInfo(std::vector<NETCARDINFO>& vectorNetCard);				//��ȡ������Ϣ
	void		GetOsInfo(CString& osinfo);                                         //��ȡ����ϵͳ��Ϣ 
	void		GetCpuInfo(CString& CPUinfo);										//��ȡCPUӲ����Ϣ 	
	void		GetCPUid(CString& CPUid);											//��ȡCPUid

	BOOL		GetHDSerial(CString& HDSerial);										//��ȡӲ���������кţ���Ҫ����ԱȨ�ޣ�

	CString GetPcName();//��ȡ������
	CString GetPcMac();//��ȡMac
	CString GetHardDiskSerialNumber();//��ȡӲ��ID
	CString GetBaseBoardSerialNumber();//��ȡ����ID
	int GetTraceIP(CString& strIP, CString& m_StrIDC_EDIT1);//��ȡIP
	CString GetTraceMac(CString strip);	//��IP��ȡMAC
	CString GetSystemName();			//��ȡ����ϵͳ������
	bool IsInternetBar();				// �Ƿ�Ϊ����
	bool IsHaveKey(HKEY hKey, char* lpSubKey);

	bool m_bIsInternetBar;

private:
	PDH_HQUERY		m_Query;
	PDH_HCOUNTER	m_CpuTotal, m_DiskRead, m_DiskWrite, m_NetDownload, m_NetUpload;

public:
	/*
	����:const char *cmd
	systeminfo:�鿴��ϸ��ϵͳ��Ϣ
	wmic logicaldisk:�鿴�̷�
	fsutil volume diskfree + �̷�����:�鿴ĳ���̷���������С��
	wmic path win32_physicalmedia get SerialNumber;�鿴Ӳ��ϵ�к�
	wmic diskdrive get serialnumber;�鿴Ӳ��ϵ�к�(������Ч��һ��)
	wmic cpu:�鿴CPU������Ϣ
	wmic cpu list brief:�鿴CPUӲ����Ϣ
	wmic memorychip;�鿴ϵͳ�ڴ���Ϣ
	wmic bios:�鿴ϵͳ��bios��Ϣ
	wmic memorychip list brief:�鿴�ڴ�����
	wmic memcache list brief:�鿴�����ڴ�
	wmic diskdrive:�鿴������ϸ��Ϣ
	wmic diskdrive get Name, Model:�鿴Ӳ�����ƣ��ͺţ�ʹ��get��
	...
	*/
	CString execCmd(const char* cmd)
	{
		char buffer[128] = { 0 };
		CString result;
		FILE* pipe = _popen(cmd, "r");
		if (!pipe) throw std::runtime_error("_popen() failed!");
		while (!feof(pipe))
		{
			if (fgets(buffer, 128, pipe) != NULL)
				result += buffer;
		}
		_pclose(pipe);
		return result;
	}

	_inline void ChangeByteOrder(PCHAR szString, USHORT uscStrSize)
	{
		USHORT  i = 0;
		CHAR	temp = '\0';

		for (i = 0; i < uscStrSize; i += 2)
		{
			temp = szString[i];
			szString[i] = szString[i + 1];
			szString[i + 1] = temp;
		}
	}

};
#endif

