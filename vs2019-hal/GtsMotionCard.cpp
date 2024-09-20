#include "StdAfx.h"
#include "GtsMotionCard.h"
#include "math.h"



//�Զ���Ĺ̸߿��ƿ�ȫ�ֱ����ͺ���20160320

//�˶�ֹͣ��־λ
volatile BOOL m_motor_stop=FALSE;
CCriticalSection m_CriticalCard;
//�̸߿��쳣��־λ
BOOL g_MotionError=FALSE;
//����Ĵ�����Ϣ
CString g_ErrorMessage=NULL;
//�����һ����Ϣ
CString g_GeneralMessage=NULL;
//������ʼʱ��
DWORD g_AlarmBeginTime=0;
//�б�������
BOOL g_AlarmHappen=FALSE;
//�忨��ʼ���ɹ���־
volatile BOOL g_bInitCardOK=FALSE;
//��Ϣѭ��
GTSMOTION_API UCT_PeekAndPump()
{

	//Sleep(1);
	//MSG msg;
	//while(::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
	//{
	//	if(!AfxGetApp()->PumpMessage())
	//	{
	//		::PostQuitMessage(0);
	//		return false;
	//	}
	//}
	//// let MFC do its idle processing
	//LONG lIdle = 0;
	//while ( AfxGetApp()->OnIdle(lIdle++ ) )
	//	;  
	//// Perform some background processing here 
	//// using another call to OnIdle
	//return true;
	//�������蹤д�ĺ���
	MSG msg;
	while(::PeekMessage(&msg,NULL,0,0,PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	return true;

}

//���˶����ƿ�
GTSMOTION_API UCT_OpenMotionCard(short cardNum,short count)
{
	/*(1)    ���������壩����ģʽ
	1)  ���˶���������GT_Open()��
	2)  ��λ�˶���������GT_Reset()����λ��Ĭ�ϵĿ���ģʽΪ������+���򡱵�������Ʒ�ʽ��
	�����ǲ��á�����+���򡱵Ŀ��Ʒ�ʽ����ɵ��� GT_StepPulse()�޸ģ�����Ҫ��ԭΪ����
	��+���򡱵Ŀ��Ʒ�ʽʱ����ɵ��� GT_StepDir()ָ�
	3)  �������������������ź���û�����ӡ���һ�������ò������������û��������������
	�ţ�����û�����ӣ���Ӧ�õ��� GT_AlarmOff()ָ�ʹ������������Ч��Ĭ������Ч�ģ�
	4)  �����������λ���أ���û�����ӣ�����Ҫͨ������ GT_LmtsOff()��ʹ��λ��Ч��Ĭ��
	����Ч�ģ��������ӣ���Ҫ��鴥����ƽ�Ƿ�������ȷ����ͨ�� GT_LmtSns()ָ���޸ģ�
	5)  ��ȷ��ǰ����������֮�󣬵��� GT_ClrSts()���������õ�״̬��
	6)  ���� GT_AxisOn()��ʹ����������������Ӧ�ĵ�����ܹ����ˣ�
	7)  ʹ���˶����˶��������ֱ�����λ�ú͹滮λ�÷���һ�£����ͨ������ GT_EncSns()
	�ı�������ļ�������*/


	// ָ���ֵ����
	short  sRtn;


	for(int i=0;i<count;i++)
	{
		m_CriticalCard.Lock();
		// �������״̬
		sRtn = GT_Open(cardNum);
		
		if (sRtn)
		{   
			g_ErrorMessage.Format(_T("�忨%d��GT_Openָ��ִ���쳣��"),cardNum+1+i);
			::MessageBox(NULL,g_ErrorMessage,"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
		m_CriticalCard.Unlock();
	}

	//û�д���
	return 0;


}
//�ر��˶����ƿ�
GTSMOTION_API UCT_CloseMotionCard(short cardNum,short count)
{

	// ָ���ֵ����
	short  sRtn;


	for(int i=0;i<count;i++)
	{
		m_CriticalCard.Lock();
		// �������״̬
		sRtn = GT_Close(cardNum);
		
		if (sRtn)
		{   
			g_ErrorMessage.Format(_T("�忨%d��GT_Closeָ��ִ���쳣��"),cardNum+1+i);
			::MessageBox(NULL,g_ErrorMessage,"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
		m_CriticalCard.Unlock();
	}

	//û�д���
	return 0;


}
//��λ�˶����ƿ�
GTSMOTION_API UCT_ResetMotionCard(short cardNum,short count)
{

	// ָ���ֵ����
	short  sRtn;


	for(int i=0;i<count;i++)
	{
		m_CriticalCard.Lock();
		// �������״̬
		sRtn = GT_Reset(cardNum);
		
		if (sRtn)
		{   
			g_ErrorMessage.Format(_T("�忨%d��GT_Resetָ��ִ���쳣��"),cardNum+1+i);
			::MessageBox(NULL,g_ErrorMessage,"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
		m_CriticalCard.Unlock();
	}

	//û�д���
	return 0;


}
//���������ļ�
GTSMOTION_API UCT_LoadConfig(short cardNum,char *pFile)
{

	// ָ���ֵ����
	short  sRtn;

	m_CriticalCard.Lock();
	// �������״̬
	sRtn = GT_LoadConfig(cardNum,pFile);
	
	if (sRtn)
	{   
		g_ErrorMessage.Format(_T("�忨%d��GT_LoadConfigָ��ִ���쳣��"),cardNum+1);
		::MessageBox(NULL,g_ErrorMessage,"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	m_CriticalCard.Unlock();

	//û�д���
	return 0;

}
//�������״̬
GTSMOTION_API UCT_ClrSts(short cardNum,short axis,short count)
{

	// ָ���ֵ����
	short  sRtn;


	m_motor_stop=FALSE;

	for(int i=0;i<count;i++)
	{
		m_CriticalCard.Lock();
		// �������״̬
		sRtn = GT_ClrSts(cardNum,axis+i);
		
		if (sRtn)
		{   
			g_ErrorMessage.Format("�忨%d��%d��GT_ClrStsָ��ִ���쳣��",cardNum+1,axis+i);
			::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
		m_CriticalCard.Unlock();
	}

	//û�д���
	return 0;
}

//�ŷ�on
GTSMOTION_API UCT_AxisOn(short cardNum,short axis,short count)
{
	// ָ���ֵ����
	short  sRtn;



	for(int i=0;i<count;i++)
	{
		//m_CriticalCard.Lock();
		// �ŷ�ʹ��
		sRtn = GT_AxisOn(cardNum,axis+i);
		
		if (sRtn)
		{   
			g_ErrorMessage.Format("�忨%d��%d��GT_AxisOnָ��ִ���쳣��",cardNum+1,axis+i);
			::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
		//m_CriticalCard.Unlock();
	}

	//û�д���
	return 0;

}
//�ŷ�off
GTSMOTION_API UCT_AxisOff(short cardNum,short axis,short count)
{
	// ָ���ֵ����
	short  sRtn;


	for(int i=0;i<count;i++)
	{
		//m_CriticalCard.Lock();
		// �ŷ�ʹ��
		sRtn = GT_AxisOff(cardNum,axis+i);
		
		if (sRtn)
		{   
			g_ErrorMessage.Format("�忨%d��%d��GT_AxisOnָ��ִ���쳣��",cardNum+1,axis+i);
			::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
		//m_CriticalCard.Unlock();
	}

	//û�д���
	return 0;
}

//д���
GTSMOTION_API UCT_WriteOutBit(short cardNum,short bit,short value,short count)
{
	if (!g_bInitCardOK)
	{
		//AfxMessageBox(_T("�˶����ƿ�δ��ʼ����"));
		return 1;
	}
	// ָ���ֵ����
	short  sRtn;
	short m_te;

	if (value==1)
	{
		m_te=0;
	} 

	if (value==0)
	{
		m_te=1;
	}

	for(int i=0;i<count;i++)
	{
		//m_CriticalCard.Lock();
		//д���
		sRtn =GT_SetDoBit(cardNum,MC_GPO,bit+i+1,m_te);
		
		if (sRtn)
		{   
			g_ErrorMessage.Format("�忨%d���%d��GT_SetDoBitָ��ִ���쳣��",cardNum+1,bit+i);
			::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
		//m_CriticalCard.Unlock();
	}

	//û�д���
	return 0;

}
//д�������
GTSMOTION_API UCT_WriteTimeOutBit(short cardNum,short bit,short value,short reverseTime)
{
	if (!g_bInitCardOK)
	{
		//AfxMessageBox(_T("�˶����ƿ�δ��ʼ����"));
		return 1;
	}
	// ָ���ֵ����
	short  sRtn;
	short m_te;

	if (value==1)
	{
		m_te=0;
	} 

	if (value==0)
	{
		m_te=1;
	}

	//m_CriticalCard.Lock();
		//д���
		sRtn =GT_SetDoBitReverse(cardNum,MC_GPO,bit+1,m_te,reverseTime);

		if (sRtn)
		{   
			g_ErrorMessage.Format("�忨%d���%d��GT_SetDoBitָ��ִ���쳣��",cardNum+1,bit);
			//::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
		//m_CriticalCard.Unlock();
	
	//û�д���
	return 0;

}
//������
GTSMOTION_API UCT_ReadInBit(short cardNum,short bit,BOOL *pValue)
{
	// ָ���ֵ����
	short  sRtn;
	long lGpiValue;
	if (!g_bInitCardOK)
	{
		//AfxMessageBox(_T("�˶����ƿ�δ��ʼ����"));
		return 1;
	}
	//m_CriticalCard.Lock();
	// ��ȡ�忨ͨ���ź�����ֵ
	
		sRtn=GT_GetDi(cardNum,MC_GPI,&lGpiValue);

		if(lGpiValue & (1<<bit))
		{
			*pValue=0;
		}
		else
		{
			*pValue=1;
		}

		if (sRtn)
		{   
			g_ErrorMessage.Format("�忨%d����%d��GT_GetDiָ��ִ���쳣��",cardNum+1,bit);
			::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
	
	
	//m_CriticalCard.Unlock();
	//û�д���
	return 0;
}
//�����
GTSMOTION_API UCT_ReadOutBit(short cardNum,short bit,BOOL *pValue)
{
	// ָ���ֵ����
	short  sRtn;

	long lGpOValue;

	//m_CriticalCard.Lock();
	// ��ȡ�忨ͨ���ź����ֵ
	sRtn=GT_GetDo(cardNum,MC_GPO,&lGpOValue);
	
	if (sRtn)
	{   
		g_ErrorMessage.Format("�忨%d��ȡ���ֵ%d��GT_GetDoָ��ִ���쳣��",cardNum+1,bit);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}

	if(lGpOValue & (1<<bit))
	{
		*pValue=0;
	}
	else
	{
		*pValue=1;
	}

	//m_CriticalCard.Unlock();
	//û�д���
	return 0;
}
//ƽ��ֹͣ
GTSMOTION_API UCT_IMStopMove(short cardNum,short axis,short count)
{

	// ָ���ֵ����
	short  sRtn;


	//m_motor_stop=TRUE;

	for(int i=0;i<count;i++)
	{
		//m_CriticalCard.Lock();
		// ƽ��ֹͣ
		sRtn = GT_Stop(cardNum,1<<(axis+i-1),IM_STOP);
		
		if (sRtn)
		{   
			g_ErrorMessage.Format("�忨%d��%d��GT_Stopָ��ִ���쳣��",cardNum+1,axis+i);
			::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
		//m_CriticalCard.Unlock();
	}

	//û�д���
	return 0;
}
//����ֹͣ
GTSMOTION_API UCT_EMStopMove(short cardNum,short axis,short count)
{

	// ָ���ֵ����
	short  sRtn;

	m_motor_stop=TRUE;

	for(int i=0;i<count;i++)
	{
		//m_CriticalCard.Lock();
		// ����ֹͣ
		sRtn = GT_Stop(cardNum,1<<(axis+i-1),EM_STOP);
		
		if (sRtn)
		{   
			g_ErrorMessage.Format("�忨%d��%d��GT_Stopָ��ִ���쳣��",cardNum+1,axis+i);
			::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
		//m_CriticalCard.Unlock();
	}

	//û�д���
	return 0;
}

//��ȡ��״̬
GTSMOTION_API UCT_GetAxisSts(short cardNum,short axis,long *pValue)
{
	// ָ���ֵ����
	short  sRtn;
	long status;

	//m_CriticalCard.Lock();
	// ��ȡ�忨ͨ���ź�����ֵ
	sRtn=GT_GetSts(cardNum,axis,&status);
	

	*pValue=status;

	if (sRtn)
	{   
		g_ErrorMessage.Format("�忨%d���%d��GT_GetStsָ��ִ���쳣��",cardNum+1,axis);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	//m_CriticalCard.Unlock();
	//û�д���
	return 0;
}
//��ȡ���ŷ�����
GTSMOTION_API UCT_GetServerAlarm(short cardNum,short axis,BOOL *pValue)
{
	// ָ���ֵ����
	short  sRtn;
	long status;

	//m_CriticalCard.Lock();
	// ��ȡ�忨ͨ���ź�����ֵ
	sRtn=GT_GetSts(cardNum,axis,&status);
	

	if (status & 0x002)
	{
		*pValue=1;
	}
	else
	{
		*pValue=0;
	}

	if (sRtn)
	{   
		g_ErrorMessage.Format("�忨%d��%d��GT_GetStsָ��ִ���쳣��",cardNum+1,axis);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	//m_CriticalCard.Unlock();
	//û�д���
	return 0;
}
//��ȡ������λ����
GTSMOTION_API UCT_GetPosLimitAlarm(short cardNum,short axis,BOOL *pValue)
{
	// ָ���ֵ����
	short  sRtn;
	long status;

	//m_CriticalCard.Lock();
	// ��ȡ�忨ͨ���ź�����ֵ
	sRtn=GT_GetSts(cardNum,axis,&status);
	

	if (status & 0x020)
	{
		*pValue=1;
	}
	else
	{
		*pValue=0;
	}

	if (sRtn)
	{   
		g_ErrorMessage.Format("�忨%d��%d��GT_GetStsָ��ִ���쳣��",cardNum+1,axis);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	//m_CriticalCard.Unlock();
	//û�д���
	return 0;
}
//��ȡ�Ḻ��λ����
GTSMOTION_API UCT_GetNegLimitAlarm(short cardNum,short axis,BOOL *pValue)
{
	// ָ���ֵ����
	short  sRtn;
	long status;

	//m_CriticalCard.Lock();
	// ��ȡ�忨ͨ���ź�����ֵ
	sRtn=GT_GetSts(cardNum,axis,&status);
	//m_CriticalCard.Unlock();

	if (status & 0x040)
	{
		*pValue=1;
	}
	else
	{
		*pValue=0;
	}

	if (sRtn)
	{   
		g_ErrorMessage.Format("�忨%d��%d��GT_GetStsָ��ִ���쳣��",cardNum+1,axis);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}

	//û�д���
	return 0;
}
//��ȡ����λֵ
GTSMOTION_API UCT_GetPosLimitValue(short cardNum,short axis,BOOL *pValue)
{

	long lPosLmtValue;
	short  sRtn;

	//m_CriticalCard.Lock();
	// ��ȡ�忨����λ�ź�����ֵ
	sRtn=GT_GetDi(cardNum,MC_LIMIT_POSITIVE,&lPosLmtValue);
	
	if (sRtn)
	{   
		g_ErrorMessage.Format("�忨%d��GT_GetDi��ȡ������ֵָ��ִ���쳣��",cardNum+1);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}


	if( lPosLmtValue & (1<<(axis-1)))
	{
		*pValue=1;
	}
	else
	{
		*pValue=0;
	}
	//m_CriticalCard.Unlock();
	//û�д���
	return 0;

}
//��ȡ����λֵ
GTSMOTION_API UCT_GetNegLimitValue(short cardNum,short axis,BOOL *pValue)
{

	long lNegLmtValue;
	short  sRtn;

	//m_CriticalCard.Lock();
	// ��ȡ�忨����λ�ź�����ֵ
	sRtn=GT_GetDi(cardNum,MC_LIMIT_NEGATIVE,&lNegLmtValue);
	
	if (sRtn)
	{   
		g_ErrorMessage.Format("�忨%d��GT_GetDi��ȡ����λֵָ��ִ���쳣��",cardNum+1);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}

	if( lNegLmtValue & (1<<(axis-1)))
	{
		*pValue=1;
	}
	else
	{
		*pValue=0;
	}
	//m_CriticalCard.Unlock();
	//û�д���
	return 0;
}
//��ȡԭ��ֵ
GTSMOTION_API UCT_GetHomeValue(short cardNum,short axis,BOOL *pValue)
{
	long lOrgValue;
	short  sRtn;

	//m_CriticalCard.Lock();
	// ��ȡ�忨����λ�ź�����ֵ
	sRtn=GT_GetDi(cardNum,MC_HOME,&lOrgValue);
	
	if (sRtn)
	{   
		g_ErrorMessage.Format("�忨%d��GT_GetDi��ȡԭ��ֵָ��ִ���쳣��",cardNum+1);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}


	if( lOrgValue & (1<<(axis-1)))
	{
		*pValue=1;
	}
	else
	{
		*pValue=0;
	}
	//m_CriticalCard.Unlock();
	//û�д���
	return 0;
}
//��ȡ�滮����ֵ
GTSMOTION_API UCT_GetPrfPos(short cardNum,short axis,double *pValue)
{

	double dPrfPos;
	short  sRtn;

	//m_CriticalCard.Lock();
	// ��ȡ�滮����ֵ
	sRtn=GT_GetPrfPos(cardNum,axis, &dPrfPos);
	
	if (sRtn)
	{   
		g_ErrorMessage.Format("�忨%d��%d��GT_GetPrfPos��ȡԭ��ֵָ��ִ���쳣��",cardNum+1,axis);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}

	*pValue=dPrfPos;
	//m_CriticalCard.Unlock();
	//û�д���
	return 0;
}
//��ȡ��������ֵ
GTSMOTION_API UCT_GetEncPos(short cardNum,short axis,double *pValue)
{

	double dEncPos;
	short  sRtn;

	//m_CriticalCard.Lock();
	// ��ȡ�滮����ֵ
	sRtn=GT_GetEncPos(cardNum,axis, &dEncPos);
	
	if (sRtn)
	{   
		g_ErrorMessage.Format("�忨%d��%d��GT_GetEncPos��ȡԭ��ֵָ��ִ���쳣��",cardNum+1,axis);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}

	*pValue=dEncPos;
	//m_CriticalCard.Unlock();
	//û�д���
	return 0;
}

//�����ԭ�㣨��ԭ���Ӧ����
GTSMOTION_API UCT_BackToHomeORIGIN(short cardNum,short axis,long search_pos,long search_home,long home_offset,double home_speed,short sHomeCapture)
{
	//m_CriticalCard.Lock();
	//���ƽ��ֹͣ
	if (!g_bInitCardOK)
	{
		//AfxMessageBox(_T("�˶����ƿ�δ��ʼ����"));
		return 1;
	}
	if (m_motor_stop)
	{
		//���ƽ��ֹͣ
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}

	short capture;
	short sRtn;
	TTrapPrm trapPrm;
	long status,pos;
	double prfPos,dEncPos;

	// �����ı�������λ
	GT_ClrSts(cardNum,axis);

	// ʵ��λ�����㣬��������Ư����
	GT_ZeroPos(cardNum,axis);

	// �л�����λ�˶�ģʽ
	GT_PrfTrap(cardNum,axis);

	// ��ȡ��λģʽ�˶�����
	GT_GetTrapPrm(cardNum,axis, &trapPrm);

	trapPrm.acc = 0.5;
	trapPrm.dec = 0.5;
	trapPrm.smoothTime = SMOOTHTIME;
	// ���õ�λģʽ�˶�����
	GT_SetTrapPrm(cardNum,axis, &trapPrm);

	//��һ�λ���
	// ���õ�λģʽĿ���ٶȣ����˶��ٶ�
	GT_SetVel(cardNum,axis, home_speed);

	//��ȡ�滮��λ��
	GT_GetPrfPos(cardNum,axis, &prfPos);
	// �����˶�һ�ξ���
	GT_SetPos(cardNum,axis, long(prfPos)+search_pos);

	//���ƽ��ֹͣ
	if (m_motor_stop)
	{
		//���ƽ��ֹͣ
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}

	// �����˶�
	GT_Update(cardNum,1<<(axis-1));

	Sleep(100);
	do
	{
		//��Ϣѭ��
		UCT_PeekAndPump();
		// ��ȡ��״̬
		GT_GetSts(cardNum,axis, &status);

		//���ƽ��ֹͣ
		if (m_motor_stop)
		{
			//���ƽ��ֹͣ
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

	}while( status & 0x400 );

	// �������ı�������λ
	GT_ClrSts(cardNum,axis);

	// ����Home����
	GT_SetCaptureMode(cardNum,axis,CAPTURE_HOME);
	//�����ز���
	if(1 == sHomeCapture)
		GT_SetCaptureSense(cardNum,axis,CAPTURE_HOME,1); 
	else
		GT_SetCaptureSense(cardNum,axis,CAPTURE_HOME,0); 
	// ���õ�λģʽĿ���ٶȣ�����ԭ���ٶ�
	GT_SetVel(cardNum,axis, home_speed*2);
	////��ȡ������λ��
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	//��ȡ�滮��λ��
	GT_GetPrfPos(cardNum,axis, &prfPos);
	//  ���õ�λģʽĿ��λ�ã���ԭ����������
	GT_SetPos(cardNum,axis, long(prfPos)-search_home);

	//���ƽ��ֹͣ
	if (m_motor_stop)
	{
		//���ƽ��ֹͣ
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}

	// �����˶�
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//��Ϣѭ��
		UCT_PeekAndPump();
		// ��ȡ��״̬
		GT_GetSts(cardNum,axis, &status);

		// ��ȡ����״̬
		sRtn = GT_GetCaptureStatus(cardNum,axis, &capture, &pos);
		//���ƽ��ֹͣ
		if (m_motor_stop)
		{
			//���ƽ��ֹͣ
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

		// ����˶�ֹͣ�����س�����Ϣ
		if( 0 == ( status & 0x400 ) )
		{
			m_motor_stop=1;
			g_ErrorMessage.Format("��ԭ���У��忨%d���%d����Ҳ���ԭ���������쳣ֹͣ��",cardNum+1,axis);
			::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}


		// �ȴ����񴥷�
	}while( 0 == capture );
	GT_ClrSts(cardNum,axis);
	// �˶���"����λ��+ƫ����"
	GT_SetPos(cardNum,axis, pos + home_offset);

	// ���˶�״̬�¸���Ŀ��λ��
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//��Ϣѭ��
		UCT_PeekAndPump();
		// ��ȡ��״̬
		sRtn = GT_GetSts(cardNum,axis, &status);

		//���ƽ��ֹͣ
		if (m_motor_stop)
		{
			//���ƽ��ֹͣ
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

		// �ȴ��˶�ֹͣ
	}while( status & 0x400 );


	// ��ʱһ��ʱ�䣬�ȴ����ͣ��
	Sleep(50);



	//�ڶ��λ���
	// �������ı�������λ
	GT_ClrSts(cardNum,axis);

	// ���õ�λģʽĿ���ٶȣ����˶��ٶ�
	GT_SetVel(cardNum,axis, home_speed/2);

	//��ȡ�滮��λ��
	GT_GetPrfPos(cardNum,axis, &prfPos);
	// �����˶�һ�ξ���
	GT_SetPos(cardNum,axis, long(prfPos)+search_pos/10);
	//���ƽ��ֹͣ
	if (m_motor_stop)
	{
		//���ƽ��ֹͣ
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}
	// �����˶�
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//��Ϣѭ��
		UCT_PeekAndPump();
		// ��ȡ��״̬
		GT_GetSts(cardNum,axis, &status);

		//���ƽ��ֹͣ
		if (m_motor_stop)
		{
			//���ƽ��ֹͣ
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

	}while( status & 0x400 );

	// �������ı�������λ
	GT_ClrSts(cardNum,axis);

	// ����Home����
	GT_SetCaptureMode(cardNum,axis,CAPTURE_HOME);
	//�����ز���
	if(1 == sHomeCapture)
		GT_SetCaptureSense(cardNum,axis,CAPTURE_HOME,1); 
	else
		GT_SetCaptureSense(cardNum,axis,CAPTURE_HOME,0); 

	// ���õ�λģʽĿ���ٶȣ�����ԭ���ٶ�
	GT_SetVel(cardNum,axis, home_speed/15);
	////��ȡ������λ��
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	//��ȡ�滮��λ��
	GT_GetPrfPos(cardNum,axis, &prfPos);
	//  ���õ�λģʽĿ��λ�ã���ԭ����������
	GT_SetPos(cardNum,axis, long(prfPos)-search_home);

	//���ƽ��ֹͣ
	if (m_motor_stop)
	{
		//���ƽ��ֹͣ
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}

	// �����˶�
	GT_Update(cardNum,1<<(axis-1));



	do
	{
		//��Ϣѭ��
		UCT_PeekAndPump();
		// ��ȡ��״̬
		GT_GetSts(cardNum,axis, &status);

		// ��ȡ����״̬
		sRtn = GT_GetCaptureStatus(cardNum,axis, &capture, &pos);

		//���ƽ��ֹͣ
		if (m_motor_stop)
		{
			//���ƽ��ֹͣ
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

		// ����˶�ֹͣ�����س�����Ϣ
		if( 0 == ( status & 0x400 ) )
		{
			m_motor_stop=1;
			g_ErrorMessage.Format("��ԭ���У��忨%d���%d����Ҳ���ԭ���������쳣ֹͣ��",cardNum+1,axis);
			::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
		// �ȴ����񴥷�
	}while( 0 == capture );



	// �˶���"����λ��+ƫ����"
	GT_SetPos(cardNum,axis, pos + home_offset);

	//���ƽ��ֹͣ
	if (m_motor_stop)
	{
		//���ƽ��ֹͣ
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}
	// ���˶�״̬�¸���Ŀ��λ��
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//��Ϣѭ��
		UCT_PeekAndPump();
		// ��ȡ��״̬
		sRtn = GT_GetSts(cardNum,axis, &status);

		//���ƽ��ֹͣ
		if (m_motor_stop)
		{
			//���ƽ��ֹͣ
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

		// �ȴ��˶�ֹͣ
	}while( status & 0x400 );

	// ��ʱһ��ʱ�䣬�ȴ����ͣ��
	Sleep(600);


	////��ȡ������λ��
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	////��ȡ�滮��λ��
	//GT_GetPrfPos(cardNum,axis, &prfPos);

	//if (abs(long(dEncPos-prfPos))>=DIF_PULSE)
	//{
	//	m_motor_stop=1;
	//	g_ErrorMessage.Format("��ԭ���У��忨%d���%d���Ŀ��λ�ú�ʵ��λ�ò���̫��,���%ld����!",cardNum+1,axis,abs(long(dEncPos-prfPos)));
	//	::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
	//	return 1;
	//}

	// �������ı�������λ
	GT_ClrSts(cardNum,axis);
	// ʵ��λ�����㣬��������Ư����
	GT_ZeroPos(cardNum,axis);

	//m_CriticalCard.Unlock();
	return 0;

}
//�����ԭ�㣨��ԭ���Ӧ����
GTSMOTION_API UCT_BackToHomeINDEX(short cardNum,short axis,long search_pos,long search_home,long home_offset,double home_speed)
{
	//m_CriticalCard.Lock();
	//���ƽ��ֹͣ
	if (!g_bInitCardOK)
	{
		//AfxMessageBox(_T("�˶����ƿ�δ��ʼ����"));
		return 1;
	}
	if (m_motor_stop)
	{
		//���ƽ��ֹͣ
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}

	short capture;
	short sRtn;
	TTrapPrm trapPrm;
	long status,pos;
	double prfPos,dEncPos;

	// �����ı�������λ
	GT_ClrSts(cardNum,axis);

	// ʵ��λ�����㣬��������Ư����
	GT_ZeroPos(cardNum,axis);

	// �л�����λ�˶�ģʽ
	GT_PrfTrap(cardNum,axis);

	// ��ȡ��λģʽ�˶�����
	GT_GetTrapPrm(cardNum,axis, &trapPrm);

	trapPrm.acc = 0.5;
	trapPrm.dec = 0.5;
	trapPrm.smoothTime = SMOOTHTIME;
	// ���õ�λģʽ�˶�����
	GT_SetTrapPrm(cardNum,axis, &trapPrm);

	//��һ�λ���
	// ���õ�λģʽĿ���ٶȣ����˶��ٶ�
	GT_SetVel(cardNum,axis, home_speed);

	//��ȡ�滮��λ��
	GT_GetPrfPos(cardNum,axis, &prfPos);
	// �����˶�һ�ξ���
	GT_SetPos(cardNum,axis, long(prfPos)+search_pos);

	//���ƽ��ֹͣ
	if (m_motor_stop)
	{
		//���ƽ��ֹͣ
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}

	// �����˶�
	GT_Update(cardNum,1<<(axis-1));

	Sleep(100);
	do
	{
		//��Ϣѭ��
		UCT_PeekAndPump();
		// ��ȡ��״̬
		GT_GetSts(cardNum,axis, &status);

		//���ƽ��ֹͣ
		if (m_motor_stop)
		{
			//���ƽ��ֹͣ
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

	}while( status & 0x400 );

	// �������ı�������λ
	GT_ClrSts(cardNum,axis);

	// ����Home����
	GT_SetCaptureMode(cardNum,axis,CAPTURE_INDEX);
	//�����ز���
	//GT_SetCaptureSense(axis,CAPTURE_HOME,0); 

	// ���õ�λģʽĿ���ٶȣ�����ԭ���ٶ�
	GT_SetVel(cardNum,axis, home_speed/2);
	////��ȡ������λ��
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	//��ȡ�滮��λ��
	GT_GetPrfPos(cardNum,axis, &prfPos);
	//  ���õ�λģʽĿ��λ�ã���ԭ����������
	GT_SetPos(cardNum,axis, long(prfPos)-search_home);

	//���ƽ��ֹͣ
	if (m_motor_stop)
	{
		//���ƽ��ֹͣ
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}

	// �����˶�
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//��Ϣѭ��
		UCT_PeekAndPump();
		// ��ȡ��״̬
		GT_GetSts(cardNum,axis, &status);

		// ��ȡ����״̬
		sRtn = GT_GetCaptureStatus(cardNum,axis, &capture, &pos);
		//���ƽ��ֹͣ
		if (m_motor_stop)
		{
			//���ƽ��ֹͣ
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

		// ����˶�ֹͣ�����س�����Ϣ
		if( 0 == ( status & 0x400 ) )
		{
			m_motor_stop=1;
			g_ErrorMessage.Format("��ԭ���У��忨%d���%d����Ҳ���ԭ���������쳣ֹͣ��",cardNum+1,axis);
			::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}


		// �ȴ����񴥷�
	}while( 0 == capture );

	// �˶���"����λ��+ƫ����"
	GT_SetPos(cardNum,axis, pos - home_offset);

	// ���˶�״̬�¸���Ŀ��λ��
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//��Ϣѭ��
		UCT_PeekAndPump();
		// ��ȡ��״̬
		sRtn = GT_GetSts(cardNum,axis, &status);

		//���ƽ��ֹͣ
		if (m_motor_stop)
		{
			//���ƽ��ֹͣ
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

		// �ȴ��˶�ֹͣ
	}while( status & 0x400 );


	// ��ʱһ��ʱ�䣬�ȴ����ͣ��
	Sleep(50);



	//�ڶ��λ���
	// �������ı�������λ
	GT_ClrSts(cardNum,axis);

	// ���õ�λģʽĿ���ٶȣ����˶��ٶ�
	GT_SetVel(cardNum,axis, home_speed/2);

	//��ȡ�滮��λ��
	GT_GetPrfPos(cardNum,axis, &prfPos);
	// �����˶�һ�ξ���
	GT_SetPos(cardNum,axis, long(prfPos)+search_pos/10);
	//���ƽ��ֹͣ
	if (m_motor_stop)
	{
		//���ƽ��ֹͣ
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}
	// �����˶�
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//��Ϣѭ��
		UCT_PeekAndPump();
		// ��ȡ��״̬
		GT_GetSts(cardNum,axis, &status);

		//���ƽ��ֹͣ
		if (m_motor_stop)
		{
			//���ƽ��ֹͣ
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

	}while( status & 0x400 );

	// �������ı�������λ
	GT_ClrSts(cardNum,axis);

	// ����Home����
	GT_SetCaptureMode(cardNum,axis,CAPTURE_HOME);
	//�����ز���
	//GT_SetCaptureSense(axis,CAPTURE_HOME,0); 

	// ���õ�λģʽĿ���ٶȣ�����ԭ���ٶ�
	GT_SetVel(cardNum,axis, home_speed/15);
	////��ȡ������λ��
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	//��ȡ�滮��λ��
	GT_GetPrfPos(cardNum,axis, &prfPos);
	//  ���õ�λģʽĿ��λ�ã���ԭ����������
	GT_SetPos(cardNum,axis, long(prfPos)-search_home);

	//���ƽ��ֹͣ
	if (m_motor_stop)
	{
		//���ƽ��ֹͣ
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}

	// �����˶�
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//��Ϣѭ��
		UCT_PeekAndPump();
		// ��ȡ��״̬
		GT_GetSts(cardNum,axis, &status);

		// ��ȡ����״̬
		sRtn = GT_GetCaptureStatus(cardNum,axis, &capture, &pos);

		//���ƽ��ֹͣ
		if (m_motor_stop)
		{
			//���ƽ��ֹͣ
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

		// ����˶�ֹͣ�����س�����Ϣ
		if( 0 == ( status & 0x400 ) )
		{
			m_motor_stop=1;
			g_ErrorMessage.Format("��ԭ���У��忨%d���%d����Ҳ���ԭ���������쳣ֹͣ��",cardNum+1,axis);
			::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
		// �ȴ����񴥷�
	}while( 0 == capture );

	// �˶���"����λ��+ƫ����"
	GT_SetPos(cardNum,axis, pos - home_offset);
	//���ƽ��ֹͣ
	if (m_motor_stop)
	{
		//���ƽ��ֹͣ
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}
	// ���˶�״̬�¸���Ŀ��λ��
	GT_Update(cardNum,1<<(axis-1));
	do
	{
		//��Ϣѭ��
		UCT_PeekAndPump();
		// ��ȡ��״̬
		GT_GetSts(cardNum,axis, &status);

		//���ƽ��ֹͣ
		if (m_motor_stop)
		{
			//���ƽ��ֹͣ
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

	}while( status & 0x400 );


	// ��ʱһ��ʱ�䣬�ȴ����ͣ��
	Sleep(50);

	GT_SetCaptureMode(cardNum,axis, CAPTURE_INDEX);
	// ���õ�λģʽĿ���ٶȣ�����ԭ���ٶ�
	GT_SetVel(cardNum,axis, home_speed/18);
	// �˶���"����λ��+ƫ����"
	GT_SetPos(cardNum,axis, pos - search_home);


	//���ƽ��ֹͣ
	if (m_motor_stop)
	{
		//���ƽ��ֹͣ
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}
	// ���˶�״̬�¸���Ŀ��λ��
	GT_Update(cardNum,1<<(axis-1));


	do
	{
		//��Ϣѭ��
		UCT_PeekAndPump();
		// ��ȡ��״̬
		GT_GetSts(cardNum,axis, &status);

		// ��ȡ����״̬
		sRtn = GT_GetCaptureStatus(cardNum,axis, &capture, &pos);

		//���ƽ��ֹͣ
		if (m_motor_stop)
		{
			//���ƽ��ֹͣ
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

		// ����˶�ֹͣ�����س�����Ϣ
		if( 0 == ( status & 0x400 ))
		{
			m_motor_stop=1;
			g_ErrorMessage.Format("��ԭ���У��忨%d���%d��ﲶ�񲻵�Z�������������쳣ֹͣ��",cardNum+1,axis);
			::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
		// �ȴ����񴥷�
	}while( 0 == capture );

	// �˶���"����λ��+ƫ����"
	GT_SetPos(cardNum,axis, pos - home_offset);
	//���ƽ��ֹͣ
	if (m_motor_stop)
	{
		//���ƽ��ֹͣ
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}
	// ���˶�״̬�¸���Ŀ��λ��
	GT_Update(cardNum,1<<(axis-1));
	do
	{
		//��Ϣѭ��
		UCT_PeekAndPump();
		// ��ȡ��״̬
		GT_GetSts(cardNum,axis, &status);

		//���ƽ��ֹͣ
		if (m_motor_stop)
		{
			//���ƽ��ֹͣ
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

	}while( status & 0x400 );


	// ��ʱһ��ʱ�䣬�ȴ����ͣ��
	Sleep(600);

	////��ȡ������λ��
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	////��ȡ�滮��λ��
	//GT_GetPrfPos(cardNum,axis, &prfPos);

	//if (abs(long(dEncPos-prfPos))>=DIF_PULSE)
	//{
	//	m_motor_stop=1;
	//	g_ErrorMessage.Format("��ԭ���У��忨%d���%d���Ŀ��λ�ú�ʵ��λ�ò���̫��,���%ld����!",cardNum+1,axis,abs(long(dEncPos-prfPos)));
	//	::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
	//	return 1;
	//}

	// �������ı�������λ
	GT_ClrSts(cardNum,axis);
	// ʵ��λ�����㣬��������Ư����
	GT_ZeroPos(cardNum,axis);

	//m_CriticalCard.Unlock();
	return 0;

}
//�����ԭ�㣨����λ��Ϊԭ�㣩
GTSMOTION_API UCT_BackToHomePOSITIVE(short cardNum,short axis,long search_pos,long search_home,long home_offset,double home_speed)
{
	if (!g_bInitCardOK)
	{
		//AfxMessageBox(_T("�˶����ƿ�δ��ʼ����"));
		return 1;
	}
	//���ƽ��ֹͣ
	if (m_motor_stop)
	{
		//���ƽ��ֹͣ
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}

	TTrapPrm trapPrm;
	long status;
	double prfPos,dEncPos;

	// �������ı�������λ
	GT_ClrSts(cardNum,axis);

	// ʵ��λ�����㣬��������Ư����
	GT_ZeroPos(cardNum,axis);
	// �л�����λ�˶�ģʽ
	GT_PrfTrap(cardNum,axis);

	// ��ȡ��λģʽ�˶�����
	GT_GetTrapPrm(cardNum,axis, &trapPrm);

	trapPrm.acc = 0.5;
	trapPrm.dec = 0.5;
	trapPrm.smoothTime = SMOOTHTIME;
	// ���õ�λģʽ�˶�����
	GT_SetTrapPrm(cardNum,axis, &trapPrm);

	//��һ�λ���
	// ���õ�λģʽĿ���ٶȣ����˶��ٶ�
	GT_SetVel(cardNum,axis, home_speed);
	////��ȡ������λ��
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	//��ȡ�滮��λ��
	GT_GetPrfPos(cardNum,axis, &prfPos);
	// �����˶�һ�ξ���
	GT_SetPos(cardNum,axis, long(prfPos)-search_pos);

	//���ƽ��ֹͣ
	if (m_motor_stop)
	{
		//���ƽ��ֹͣ
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}
	// �����˶�
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//��Ϣѭ��
		UCT_PeekAndPump();
		// ��ȡ��״̬
		GT_GetSts(cardNum,axis, &status);
		//���ƽ��ֹͣ
		if (m_motor_stop)
		{
			//���ƽ��ֹͣ
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

	}while( status & 0x400 );

	// �������ı�������λ
	GT_ClrSts(cardNum,axis);


	// ���õ�λģʽĿ���ٶȣ�����ԭ���ٶ�
	GT_SetVel(cardNum,axis, home_speed/2);
	////��ȡ������λ��
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	//��ȡ�滮��λ��
	GT_GetPrfPos(cardNum,axis, &prfPos);
	//  ���õ�λģʽĿ��λ�ã���ԭ����������
	GT_SetPos(cardNum,axis,long(prfPos)+search_home);

	//���ƽ��ֹͣ
	if (m_motor_stop)
	{
		//���ƽ��ֹͣ
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}
	// �����˶�
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//��Ϣѭ��
		UCT_PeekAndPump();
		// ��ȡ��״̬
		GT_GetSts(cardNum,axis, &status);
		//���ƽ��ֹͣ
		if (m_motor_stop)
		{
			//���ƽ��ֹͣ
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

		// �ȴ��˶�ֹͣ
	}while( status & 0x400 );

	// ��ʱһ��ʱ�䣬�ȴ����ͣ��
	Sleep(50);



	//�ڶ��λ���
	// �������ı�������λ
	GT_ClrSts(cardNum,axis);

	// ���õ�λģʽĿ���ٶȣ����˶��ٶ�
	GT_SetVel(cardNum,axis, home_speed/2);
	////��ȡ������λ��
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	//��ȡ�滮��λ��
	GT_GetPrfPos(cardNum,axis, &prfPos);
	// �����˶�һ�ξ���
	GT_SetPos(cardNum,axis, long(prfPos)-search_pos/10);

	//���ƽ��ֹͣ
	if (m_motor_stop)
	{
		//���ƽ��ֹͣ
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}

	// �����˶�
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//��Ϣѭ��
		UCT_PeekAndPump();
		// ��ȡ��״̬
		GT_GetSts(cardNum,axis, &status);

		//���ƽ��ֹͣ
		if (m_motor_stop)
		{
			//���ƽ��ֹͣ
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

	}while( status & 0x400 );

	// �������ı�������λ
	GT_ClrSts(cardNum,axis);

	// ���õ�λģʽĿ���ٶȣ�����ԭ���ٶ�
	GT_SetVel(cardNum,axis, home_speed/10);
	////��ȡ������λ��
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	//��ȡ�滮��λ��
	GT_GetPrfPos(cardNum,axis, &prfPos);
	//���õ�λģʽĿ��λ�ã���ԭ����������
	GT_SetPos(cardNum,axis, long(prfPos)+search_home);

	//���ƽ��ֹͣ
	if (m_motor_stop)
	{
		//���ƽ��ֹͣ
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}
	// �����˶�
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//��Ϣѭ��
		UCT_PeekAndPump();
		// ��ȡ��״̬
		GT_GetSts(cardNum,axis, &status);

		//���ƽ��ֹͣ
		if (m_motor_stop)
		{
			//���ƽ��ֹͣ
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}


		// �ȴ����񴥷�
	}while( status & 0x400);


	// ��ʱһ��ʱ�䣬�ȴ����ͣ��
	Sleep(600);

	////��ȡ������λ��
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	////��ȡ�滮��λ��
	//GT_GetPrfPos(cardNum,axis, &prfPos);

	//if (abs(long(dEncPos-prfPos))>=DIF_PULSE)
	//{
	//	g_ErrorMessage.Format("��ԭ���У��忨%d���%d���Ŀ��λ�ú�ʵ��λ�ò���̫��",cardNum+1,axis);
	//	::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
	//	return 1;
	//}

	// �������ı�������λ
	GT_ClrSts(cardNum,axis);
	// λ������
	GT_ZeroPos(cardNum,axis);

	return 0;
}
//�����ԭ�㣨����λ��Ϊԭ�㣩
GTSMOTION_API UCT_BackToHomeNEGATIVE(short cardNum,short axis,long search_pos,long search_home,long home_offset,double home_speed)
{
	if (!g_bInitCardOK)
	{
		//AfxMessageBox(_T("�˶����ƿ�δ��ʼ����"));
		return 1;
	}
	//���ƽ��ֹͣ
	if (m_motor_stop)
	{
		//���ƽ��ֹͣ
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}

	TTrapPrm trapPrm;
	long status;
	double prfPos,dEncPos;

	// �������ı�������λ
	GT_ClrSts(cardNum,axis);

	// ʵ��λ�����㣬��������Ư����
	GT_ZeroPos(cardNum,axis);
	// �л�����λ�˶�ģʽ
	GT_PrfTrap(cardNum,axis);

	// ��ȡ��λģʽ�˶�����
	GT_GetTrapPrm(cardNum,axis, &trapPrm);

	trapPrm.acc = 0.5;
	trapPrm.dec = 0.5;
	trapPrm.smoothTime = SMOOTHTIME;
	// ���õ�λģʽ�˶�����
	GT_SetTrapPrm(cardNum,axis, &trapPrm);

	//��һ�λ���
	// ���õ�λģʽĿ���ٶȣ����˶��ٶ�
	//GT_SetVel(cardNum,axis, home_speed);
	//////��ȡ������λ��
	////GT_GetEncPos(cardNum,axis, &dEncPos);
	////��ȡ�滮��λ��
	//GT_GetPrfPos(cardNum,axis, &prfPos);
	//// �����˶�һ�ξ���
	//GT_SetPos(cardNum,axis, long(prfPos)+search_pos);

	////���ƽ��ֹͣ
	//if (m_motor_stop)
	//{
	//	//���ƽ��ֹͣ
	//	GT_Stop(cardNum,1<<(axis-1),IM_STOP);
	//	return 1;
	//}
	//// �����˶�
	//GT_Update(cardNum,1<<(axis-1));

	//do
	//{
	//	//��Ϣѭ��
	//	UCT_PeekAndPump();
	//	// ��ȡ��״̬
	//	GT_GetSts(cardNum,axis, &status);

	//	//���ƽ��ֹͣ
	//	if (m_motor_stop)
	//	{
	//		//���ƽ��ֹͣ
	//		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
	//		return 1;
	//	}

	//}while( status & 0x400 );

	// �������ı�������λ
	GT_ClrSts(cardNum,axis);


	// ���õ�λģʽĿ���ٶȣ�����ԭ���ٶ�
	GT_SetVel(cardNum,axis, home_speed/2);
	////��ȡ������λ��
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	//��ȡ�滮��λ��
	GT_GetPrfPos(cardNum,axis, &prfPos);
	//  ���õ�λģʽĿ��λ�ã���ԭ����������
	GT_SetPos(cardNum,axis,long(prfPos)-search_home);

	//���ƽ��ֹͣ
	if (m_motor_stop)
	{
		//���ƽ��ֹͣ
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}
	// �����˶�
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//��Ϣѭ��
		UCT_PeekAndPump();
		// ��ȡ��״̬
		GT_GetSts(cardNum,axis, &status);
		//���ƽ��ֹͣ
		if (m_motor_stop)
		{
			//���ƽ��ֹͣ
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

		// �ȴ��˶�ֹͣ
	}while( status & 0x400 );

	// ��ʱһ��ʱ�䣬�ȴ����ͣ��
	Sleep(50);



	//�ڶ��λ���
	// �������ı�������λ
	GT_ClrSts(cardNum,axis);

	// ���õ�λģʽĿ���ٶȣ����˶��ٶ�
	GT_SetVel(cardNum,axis, home_speed/2);
	////��ȡ������λ��
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	//��ȡ�滮��λ��
	GT_GetPrfPos(cardNum,axis, &prfPos);
	// �����˶�һ�ξ���
	GT_SetPos(cardNum,axis, long(prfPos)+search_pos/10);

	//���ƽ��ֹͣ
	if (m_motor_stop)
	{
		//���ƽ��ֹͣ
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}

	// �����˶�
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//��Ϣѭ��
		UCT_PeekAndPump();
		// ��ȡ��״̬
		GT_GetSts(cardNum,axis, &status);

		//���ƽ��ֹͣ
		if (m_motor_stop)
		{
			//���ƽ��ֹͣ
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

	}while( status & 0x400 );

	// �������ı�������λ
	GT_ClrSts(cardNum,axis);

	// ���õ�λģʽĿ���ٶȣ�����ԭ���ٶ�
	GT_SetVel(cardNum,axis, home_speed/10);
	////��ȡ������λ��
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	//��ȡ�滮��λ��
	GT_GetPrfPos(cardNum,axis, &prfPos);
	//���õ�λģʽĿ��λ�ã���ԭ����������
	GT_SetPos(cardNum,axis, long(prfPos)-search_home);

	//���ƽ��ֹͣ
	if (m_motor_stop)
	{
		//���ƽ��ֹͣ
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}
	// �����˶�
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//��Ϣѭ��
		UCT_PeekAndPump();
		// ��ȡ��״̬
		GT_GetSts(cardNum,axis, &status);

		//���ƽ��ֹͣ
		if (m_motor_stop)
		{
			//���ƽ��ֹͣ
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}


		// �ȴ����񴥷�
	}while( status & 0x400);


	// ��ʱһ��ʱ�䣬�ȴ����ͣ��
	Sleep(600);

	////��ȡ������λ��
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	////��ȡ�滮��λ��
	//GT_GetPrfPos(cardNum,axis, &prfPos);

	//if (abs(long(dEncPos-prfPos))>=DIF_PULSE)
	//{
	//	g_ErrorMessage.Format("��ԭ���У��忨%d���%d���Ŀ��λ�ú�ʵ��λ�ò���̫��",cardNum+1,axis);
	//	::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
	//	return 1;
	//}

	// �������ı�������λ
	GT_ClrSts(cardNum,axis);
	// λ������
	GT_ZeroPos(cardNum,axis);

	return 0;
}

//�����˶���ֹͣ��bRelative=true����˶���bRelative=false�����˶���start_speed����ٶȣ�speedĿ���ٶȣ�
GTSMOTION_API UCT_1AxisMoveDone(short cardNum,short axis,long LDis,double start_speed,double speed,double acc,double dec,bool bRelative)
{
		
	if (!g_bInitCardOK)
	{
		//AfxMessageBox(_T("�˶����ƿ�δ��ʼ����"));
		return 1;
	}
	CString str;
	TTrapPrm trap;
	long status;
	short  sRtn;
	double dPrfPos,dEncPos;                         //�滮λ��

	//���ƽ��ֹͣ
	if (m_motor_stop)
	{
		//���ƽ��ֹͣ
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}
	// �������ı�������λ
	GT_ClrSts(cardNum,axis);
	// ��AXIS����Ϊ��λģʽ
	GT_PrfTrap(cardNum,axis);

	// ��ȡ��λ�˶�����
	sRtn=GT_GetTrapPrm(cardNum,axis,&trap);
	//��ʼ�ٶ�
	trap.velStart = start_speed;
	//���ٶ�
	trap.acc = acc;
	//���ٶ�
	trap.dec = dec;

	// ����AXIS���Ŀ���ٶ�
	sRtn=GT_SetVel(cardNum,axis, speed);
	//ƽ��ʱ��
	trap.smoothTime = SMOOTHTIME;
	// ���õ�λ�˶�����
	sRtn=GT_SetTrapPrm(cardNum,axis, &trap);


	//��ȡ�滮λ��
	GT_GetPrfPos(cardNum,axis, &dPrfPos);

	if (bRelative==true)
	{
		// ����AXIS���Ŀ��λ�ã���ԣ�
		GT_SetPos(cardNum,axis, long(dPrfPos+LDis));

	} 
	else
	{
		// ����AXIS���Ŀ��λ�ã����ԣ�
		GT_SetPos(cardNum,axis, LDis);
	}

	/*double dEncPos1,dEncPos2;
	dEncPos1=0;
	dEncPos2=0;

	GT_GetEncPos(cardNum,axis, &dEncPos1);*/

	// ����AXIS����˶�
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//��Ϣѭ��
		UCT_PeekAndPump();
		Sleep(5);
		// ��ȡAXIS���״̬
		GT_GetSts(cardNum,axis, &status);
		//���ƽ��ֹͣ
		if (m_motor_stop)
		{
			//���ƽ��ֹͣ
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}


	}while(status&0x400);	// �ȴ�AXIS��滮ֹͣ


	/*GT_GetEncPos(cardNum,axis, &dEncPos2);

	if (dEncPos2 == dEncPos1)
	{
		g_ErrorMessage.Format(_T("��%d��%d�����������쳣��������·��忨�Ƿ������⣡"),cardNum,axis);
	}*/

	if (status & 0x020)
	{

		//AfxMessageBox("����λ�����������ų��쳣�󣬸�λ��̨!");
		return 2;

	}
	if (status & 0x040)
	{

		//AfxMessageBox("����λ�����������ų��쳣�󣬸�λ��̨!");
		return 3;
	}

	//Sleep(20);  //��CT  modify by Lance at 2019.12.11
	//��ȡ������λ��,�忨1��1~4��Ϊ�ŷ����
	/*if ((CARD_NO1==cardNum)&&(axis<=AXIS_NO4))
	{*/
		//GT_GetEncPos(cardNum,axis, &dEncPos);
		//if (bRelative)
		//{
		//	if (abs(long(dEncPos)-(long)dPrfPos-LDis)>=DIF_PULSE)
		//	{
		//		CString str;
		//		str.Format("�忨%d��%d��Ŀ��λ�ú�ʵ��λ�ò���̫��",cardNum+1,axis);
		//		AfxMessageBox(str);
		//		//AfxMessageBox(_T("Ŀ��λ�ú�ʵ��λ�ò���̫��"));
		//		return 4;
		//	}
		//} 
		//else
		//{
		//	if (abs(long(dEncPos)-LDis)>=DIF_PULSE)
		//	{
		//		CString str;
		//		str.Format("�忨%d��%d��Ŀ��λ�ú�ʵ��λ�ò���̫��",cardNum+1,axis);
		//		AfxMessageBox(str);
		//		//AfxMessageBox(_T("Ŀ��λ�ú�ʵ��λ�ò���̫��"));
		//		return 4;
		//	}
		//}
	//}
	return 0;
}
//���Ὺʼ�˶���bRelative=true����˶���bRelative=false�����˶���start_speed����ٶȣ�speedĿ���ٶȣ�
GTSMOTION_API UCT_1AxisStartMove(short cardNum,short axis,long LDis,double start_speed,double speed,double acc,double dec,bool bRelative)
{
	if (!g_bInitCardOK)
	{
		//AfxMessageBox(_T("�˶����ƿ�δ��ʼ����"));
		return 1;
	}
	CString str;
	TTrapPrm trap;
	short  sRtn;
	double dPrfPos;                         //�滮λ��


	//���ƽ��ֹͣ
	if (m_motor_stop)
	{
		//���ƽ��ֹͣ
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}
	// �������ı�������λ
	GT_ClrSts(cardNum,axis);
	// ��AXIS����Ϊ��λģʽ
	GT_PrfTrap(cardNum,axis);

	// ��ȡ��λ�˶�����
	sRtn=GT_GetTrapPrm(cardNum,axis,&trap);
	//��ʼ�ٶ�
	trap.velStart = start_speed;
	//���ٶ�
	trap.acc = acc;
	//���ٶ�
	trap.dec = dec;

	// ����AXIS���Ŀ���ٶ�
	sRtn=GT_SetVel(cardNum,axis, speed);
	//ƽ��ʱ��
	trap.smoothTime = SMOOTHTIME;
	// ���õ�λ�˶�����
	sRtn=GT_SetTrapPrm(cardNum,axis, &trap);


	//��ȡ�滮λ��
	GT_GetPrfPos(cardNum,axis, &dPrfPos);

	if (bRelative==true)
	{
		// ����AXIS���Ŀ��λ�ã���ԣ�
		GT_SetPos(cardNum,axis, long(dPrfPos+LDis));
	} 
	else
	{
		// ����AXIS���Ŀ��λ�ã����ԣ�
		GT_SetPos(cardNum,axis, LDis);
	}

	// ����AXIS����˶�
	GT_Update(cardNum,1<<(axis-1));


	return 0;
}
//�����˶�״̬(pValue=0ֹͣ pValue=1�˶���)
GTSMOTION_API UCT_1AxisCheckMoveSts(short cardNum,short axis,long *pValue)
{
	// ָ���ֵ����
	short  sRtn;

	long status;


	//m_CriticalCard.Lock();
	// ��ȡ�忨ͨ���ź�����ֵ
	sRtn=GT_GetSts(cardNum,axis,&status);
	

	if (status & 0x400)
	{
		*pValue=1;
	}
	else
	{
		*pValue=0;
	}

	if (sRtn)
	{   
		g_ErrorMessage.Format("�忨%d��%d��GT_GetStsָ��ִ���쳣��",cardNum+1,axis);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	//m_CriticalCard.Unlock();
	//û�д���
	return 0;
}
//��鵥���˶��Ƿ�λ
GTSMOTION_API UCT_1AxisCheckMoveDone(short cardNum,short axis,long *pValue)
{
	// ָ���ֵ����
	short  sRtn;
	long status;
	
	//m_CriticalCard.Lock();
	// ��ȡ�忨ͨ���ź�����ֵ
	sRtn=GT_GetSts(cardNum,axis,&status);
	

	if (status & 0x800)
	{
		*pValue=1;
	}
	else
	{
		*pValue=0;
	}

	if (sRtn)
	{   
		g_ErrorMessage.Format("�忨%d��%d��GT_GetStsָ��ִ���쳣��",cardNum+1,axis);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	//m_CriticalCard.Unlock();
	//û�д���
	return 0;
}


//����ϵ��Ӧ��ֱ�߲岹��ʼ�˶�
GTSMOTION_API UCT_LMotionStart(short cardNum, short coordinate, short buffer)
{
	if (!g_bInitCardOK)
	{
//		AfxMessageBox(_T("�˶����ƿ�δ��ʼ����"));
		return 1;
	}
	// ָ���ֵ����
	short  sRtn;

	//m_CriticalCard.Lock();
	UCT_ClrSts(cardNum,AXIS_NO1,8);
	// �����忨cardNum����ϵcoordinate������buffer�Ĳ岹�˶�
	sRtn = GT_CrdStart(cardNum,coordinate,buffer); 
	

	if (sRtn)
	{   
		g_ErrorMessage.Format("�忨%d����ϵ%d�Ļ�����%d��GT_CrdStartָ��ִ���쳣��",cardNum+1,coordinate,buffer);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	//m_CriticalCard.Unlock();

	//û�д���
	return 0;

}
//�岹�˶�����ϵ�˶�״̬��pRun=1�˶��У�pSegment��ǰ�Ѿ���ɵĲ岹������
GTSMOTION_API UCT_CheckLMotionSts(short cardNum,short *pRun,long *pSegment, short coordinate, short buffer)
{
	// ָ���ֵ����
	short sRtn;
	// ����ϵ�˶�״̬��ѯ����
	short run;
	// ����ϵ�˶���ɶβ�ѯ����
	long segment;

	m_CriticalCard.Lock();
	// ��ѯ����ϵ1��FIFO�Ĳ岹�˶�״̬
	sRtn = GT_CrdStatus(
		cardNum,     // ����cardNum
		coordinate,	// ����ϵ������ϵ1
		&run,		// ��ȡ�岹�˶�״̬
		&segment,	// ��ȡ��ǰ�Ѿ���ɵĲ岹����
		buffer);	// ��ѯ����ϵ1��buffer������
	m_CriticalCard.Unlock();

	if (sRtn)
	{   
		g_ErrorMessage.Format("�忨%d����ϵ%d�Ļ�����%d��GT_CrdStatusָ��ִ���쳣��",cardNum+1,coordinate,buffer);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}

	*pRun=run;
	*pSegment=segment;
	

	//û�д���
	return 0;
}
//���������ϵ�������е�����
GTSMOTION_API UCT_CrdClear(short cardNum, short coordinate, short buffer)
{
	// ָ���ֵ����
	short  sRtn;

	m_CriticalCard.Lock();
	// ����˻������е�����
	sRtn = GT_CrdClear(cardNum,coordinate,buffer);

	m_CriticalCard.Unlock();

	if (sRtn)
	{   
		g_ErrorMessage.Format("�忨%d����ϵ%d�Ļ�����%d��GT_CrdClearָ��ִ���쳣��",cardNum+1,coordinate,buffer);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}

	//û�д���
	return 0;

}
// ��������ʱָ��
GTSMOTION_API UCT_BufDelay(short cardNum, short delay, short coordinate, short buffer)
{
	// ָ���ֵ����
	short  sRtn;


	// ��������ʱָ��
	sRtn = GT_BufDelay(cardNum,coordinate,delay,buffer);

	if (sRtn)
	{   
		g_ErrorMessage.Format("�忨%d����ϵ%d�Ļ�����%d��GT_BufDelayָ��ִ���쳣��",cardNum+1,coordinate,buffer);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}

	//û�д���
	return 0;
}

//����ֱ�߲岹��ʼ��
GTSMOTION_API UCT_2AxisLMotionInit(short cardNum, short axis1, short axis2, short coordinate, short buffer)
{
	// ָ���ֵ����
	short  sRtn;
	//m_CriticalCard.Lock();
	// ��������ϵ
	TCrdPrm crdPrm;

	memset(&crdPrm,0,sizeof(crdPrm));
	crdPrm.dimension = 2;                        // ������ά������ϵ
	crdPrm.synVelMax = SYNVELMAX;                // ����ϵ�����ϳ��ٶ���: SYNVELMAX pulse/ms
	crdPrm.synAccMax = SYNACCMAX;                // ����ϵ�����ϳɼ��ٶ���: SYNACCMAX pulse/ms^2
	crdPrm.evenTime = EVENTIME;                 // ����ϵ����С����ʱ��ΪEVENTIME ms
	crdPrm.profile[axis1-1] = 1;                  // �滮��1��Ӧ��X��                       
	crdPrm.profile[axis2-1] = 2;                  // �滮��2��Ӧ��Y��                   
	crdPrm.setOriginFlag = 1;                    // ��Ҫ���üӹ�����ϵԭ��λ��
	crdPrm.originPos[axis1-1] = 0;               // �ӹ�����ϵԭ��λ����(0,0)�������������ϵԭ���غ�
	crdPrm.originPos[axis2-1] = 0;

	sRtn = GT_SetCrdPrm(cardNum,coordinate,&crdPrm);
	if (sRtn)
	{   
		g_ErrorMessage.Format("�忨%d������ϵ%d��GT_SetCrdPrmָ��ִ���쳣��",cardNum+1,coordinate);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	// ���������ݴ�������ϵ1��FIFO0�У�����Ҫ��������˻������е�����
	m_CriticalCard.Lock();
	sRtn = GT_CrdClear(cardNum,coordinate,buffer);
	m_CriticalCard.Unlock();
	if (sRtn)
	{   
		g_ErrorMessage.Format("�忨%d����ϵ%d�Ļ�����%d��GT_CrdClearָ��ִ���쳣��",cardNum+1,coordinate,buffer);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 2;
	}
	//m_CriticalCard.Unlock();
	//û�д���
	return 0;

}
//����ֱ�߲岹д������(acc���ٶȣ�speedĿ���ٶȣ�end_speed�յ��ٶȣ�axis1_data��1���ݣ�axis2_data��2����,coordinate����ϵ��buffer������)
GTSMOTION_API UCT_2AxisLMotionData(short cardNum,double acc,double speed, double end_speed,long axis1_data,long axis2_data, short coordinate, short buffer)
{
	// ָ���ֵ����
	short  sRtn;

	// ����ϵ�Ļ�����ʣ��ռ��ѯ����
	long space;
	//m_CriticalCard.Lock();
	
	// ��ѯ����ϵ1��FIFO0��ʣ��Ŀռ�
	sRtn = GT_CrdSpace(cardNum,coordinate, &space, 0);
	if (sRtn)
	{   
		g_ErrorMessage.Format("�忨%d����ϵ%d��GT_CrdSpaceָ��ִ���쳣��",cardNum+1,coordinate);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}

	if (space==0)
	{   
		g_ErrorMessage.Format("�忨%d����ϵ%d�Ļ�����%dʣ��ռ�Ϊ�㣬����д��ʧ�ܣ�",cardNum+1,coordinate,buffer);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 2;
	}


	// �򻺴���д���һ�β岹���ݣ����ٶΣ�
	sRtn = GT_LnXY( 
		cardNum,                      //����cardNum
		coordinate,				      // �ò岹�ε�����ϵ������ϵcoordinate
		axis1_data, axis2_data,		  // �ò岹�ε��յ�����: pulse
		speed,			             // �ò岹�ε�Ŀ���ٶȣ�pulse/ms
		acc,			             // �岹�εļ��ٶȣ�pulse/ms^2
		end_speed,				     // �յ��ٶ�Ϊend_speed
		buffer);				     // ������ϵ1��FIFO0���������ݸ�ֱ�߲岹����

	if (sRtn)
	{   
		g_ErrorMessage.Format("�忨%d����ϵ%d��GT_LnXYָ��ִ���쳣��",cardNum+1,coordinate);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 3;
	}
	//m_CriticalCard.Unlock();
	//û�д���
	return 0;

}

//����ֱ�߲岹��ʼ��
GTSMOTION_API UCT_3AxisLMotionInit(short cardNum, short axis1, short axis2, short axis3,short coordinate, short buffer)
{
	// ָ���ֵ����
	short  sRtn;
	//m_CriticalCard.Lock();
	// ��������ϵ
	TCrdPrm crdPrm;


	memset(&crdPrm,0,sizeof(crdPrm));
	crdPrm.dimension = 3;                        // ������ά������ϵ
	crdPrm.synVelMax = SYNVELMAX;                // ����ϵ�����ϳ��ٶ���: SYNVELMAX pulse/ms
	crdPrm.synAccMax = SYNACCMAX;                // ����ϵ�����ϳɼ��ٶ���: SYNACCMAX pulse/ms^2
	crdPrm.evenTime = EVENTIME;                 // ����ϵ����С����ʱ��ΪEVENTIME ms
	crdPrm.profile[axis1-1] = 1;                 // �滮��1��Ӧ��X��                       
	crdPrm.profile[axis2-1] = 2;                 // �滮��2��Ӧ��Y��
	crdPrm.profile[axis3-1] = 3;                 // �滮��3��Ӧ��Z��  
	crdPrm.setOriginFlag = 1;                    // ��Ҫ���üӹ�����ϵԭ��λ��
	crdPrm.originPos[axis1-1] = 0;               // �ӹ�����ϵԭ��λ����(0,0,0)�������������ϵԭ���غ�
	crdPrm.originPos[axis2-1] = 0;
	crdPrm.originPos[axis3-1] = 0;


	sRtn = GT_SetCrdPrm(cardNum,coordinate,&crdPrm);
	if (sRtn)
	{   
		g_ErrorMessage.Format("�忨%d������ϵ%d��GT_SetCrdPrmָ��ִ���쳣��",cardNum+1,coordinate);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	// ���������ݴ�������ϵ1��FIFO0�У�����Ҫ��������˻������е�����
	m_CriticalCard.Lock();
	sRtn = GT_CrdClear(cardNum,coordinate,buffer);
	m_CriticalCard.Unlock();
	if (sRtn)
	{   
		g_ErrorMessage.Format("�忨%d����ϵ%d�Ļ�����%d��GT_CrdClearָ��ִ���쳣��",cardNum+1,coordinate,buffer);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 2;
	}
	//m_CriticalCard.Unlock();
	//û�д���
	return 0;

}
//����ֱ�߲岹д������(acc���ٶȣ�speedĿ���ٶȣ�end_speed�յ��ٶȣ�axis1_data��1���ݣ�axis2_data��2���ݣ�axis3_data��3����,coordinate����ϵ��buffer������)
GTSMOTION_API UCT_3AxisLMotionData(short cardNum,double acc,double speed, double end_speed,long axis1_data,long axis2_data,long axis3_data, short coordinate, short buffer)
{
	// ָ���ֵ����
	short  sRtn;

	// ����ϵ�Ļ�����ʣ��ռ��ѯ����
	long space;
	//m_CriticalCard.Lock();

	// ��ѯ����ϵ1��FIFO0��ʣ��Ŀռ�
	sRtn = GT_CrdSpace(cardNum,coordinate, &space, 0);
	if (sRtn)
	{   
		g_ErrorMessage.Format("�忨%d����ϵ%d��GT_CrdSpaceָ��ִ���쳣��",cardNum+1,coordinate);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}

	if (space==0)
	{   
		g_ErrorMessage.Format("�忨%d����ϵ%d�Ļ�����%dʣ��ռ�Ϊ�㣬����д��ʧ�ܣ�",cardNum+1,coordinate,buffer);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 2;
	}


	// �򻺴���д���һ�β岹���ݣ����ٶΣ�
	sRtn = GT_LnXYZ( 
		cardNum,                                       //����cardNum
		coordinate,				                      // �ò岹�ε�����ϵ������ϵcoordinate
		axis1_data, axis2_data, axis3_data,    		  // �ò岹�ε��յ�����: pulse
		speed,			                              // �ò岹�ε�Ŀ���ٶȣ�pulse/ms
		acc,			                              // �岹�εļ��ٶȣ�pulse/ms^2
		end_speed,				                      // �յ��ٶ�Ϊend_speed
		buffer);				                      // ������ϵ1��FIFO0���������ݸ�ֱ�߲岹����

	if (sRtn)
	{   
		g_ErrorMessage.Format("�忨%d����ϵ%d��GT_LnXYZָ��ִ���쳣��",cardNum+1,coordinate);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 3;
	}
	//m_CriticalCard.Unlock();
	//û�д���
	return 0;

}

//����ֱ�߲岹��ʼ��
GTSMOTION_API UCT_4AxisLMotionInit(short cardNum, short axis1, short axis2, short axis3, short axis4,short coordinate, short buffer)
{
	// ָ���ֵ����
	short  sRtn;

	// ��������ϵ
	TCrdPrm crdPrm;


	memset(&crdPrm,0,sizeof(crdPrm));
	crdPrm.dimension = 4;                        // ������ά������ϵ
	crdPrm.synVelMax = SYNVELMAX;                // ����ϵ�����ϳ��ٶ���: SYNVELMAX pulse/ms
	crdPrm.synAccMax = SYNACCMAX;                // ����ϵ�����ϳɼ��ٶ���: SYNACCMAX pulse/ms^2
	crdPrm.evenTime = EVENTIME;                 // ����ϵ����С����ʱ��ΪEVENTIME ms
	crdPrm.profile[axis1-1] = 1;                 // �滮��1��Ӧ��X��                       
	crdPrm.profile[axis2-1] = 2;                 // �滮��2��Ӧ��Y��
	crdPrm.profile[axis3-1] = 3;                 // �滮��3��Ӧ��Z��  
	crdPrm.profile[axis4-1] = 4;                 // �滮��4��Ӧ��A��  
	crdPrm.setOriginFlag = 1;                    // ��Ҫ���üӹ�����ϵԭ��λ��
	crdPrm.originPos[axis1-1] = 0;               // �ӹ�����ϵԭ��λ����(0,0,0)�������������ϵԭ���غ�
	crdPrm.originPos[axis2-1] = 0;
	crdPrm.originPos[axis3-1] = 0;
	crdPrm.originPos[axis4-1] = 0;


	sRtn = GT_SetCrdPrm(cardNum,coordinate,&crdPrm);
	if (sRtn)
	{   
		g_ErrorMessage.Format("�忨%d������ϵ%d��GT_SetCrdPrmָ��ִ���쳣��",cardNum+1,coordinate);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	// ���������ݴ�������ϵ1��FIFO0�У�����Ҫ��������˻������е�����
	m_CriticalCard.Lock();
	sRtn = GT_CrdClear(cardNum,coordinate,buffer);
	m_CriticalCard.Unlock();

	if (sRtn)
	{   
		g_ErrorMessage.Format("�忨%d����ϵ%d�Ļ�����%d��GT_CrdClearָ��ִ���쳣��",cardNum+1,coordinate,buffer);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 2;
	}

	//û�д���
	return 0;

}
//����ֱ�߲岹д������(acc���ٶȣ�speedĿ���ٶȣ�end_speed�յ��ٶȣ�axis1_data��1���ݣ�axis2_data��2���ݣ�axis3_data��2����,axis4_data��4����,coordinate����ϵ��buffer������)
GTSMOTION_API UCT_4AxisLMotionData(short cardNum,double acc,double speed, double end_speed,long axis1_data,long axis2_data,long axis3_data,long axis4_data,short coordinate, short buffer)
{
	// ָ���ֵ����
	short  sRtn;

	// ����ϵ�Ļ�����ʣ��ռ��ѯ����
	long space;


	// ��ѯ����ϵ1��FIFO0��ʣ��Ŀռ�
	sRtn = GT_CrdSpace(cardNum,coordinate, &space, 0);
	if (sRtn)
	{   
		g_ErrorMessage.Format("�忨%d����ϵ%d��GT_CrdSpaceָ��ִ���쳣��",cardNum+1,coordinate);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}

	if (space==0)
	{   
		g_ErrorMessage.Format("�忨%d����ϵ%d�Ļ�����%dʣ��ռ�Ϊ�㣬����д��ʧ�ܣ�",cardNum+1,coordinate,buffer);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 2;
	}


	// �򻺴���д���һ�β岹���ݣ����ٶΣ�
	sRtn = GT_LnXYZA( 
		cardNum,                                                   //����cardNum
		coordinate,				                                  // �ò岹�ε�����ϵ������ϵcoordinate
		axis1_data, axis2_data, axis3_data,axis4_data,    		  // �ò岹�ε��յ�����: pulse
		speed,			                                          // �ò岹�ε�Ŀ���ٶȣ�pulse/ms
		acc,			                                          // �岹�εļ��ٶȣ�pulse/ms^2
		end_speed,				                                 // �յ��ٶ�Ϊend_speed
		buffer);				                                 // ������ϵ1��FIFO0���������ݸ�ֱ�߲岹����

	if (sRtn)
	{   
		g_ErrorMessage.Format("�忨%d����ϵ%d��GT_LnXYZAָ��ִ���쳣��",cardNum+1,coordinate);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 3;
	}

	//û�д���
	return 0;

}

//��άλ�ñȽϹ��ܳ�ʼ��
GTSMOTION_API UCT_2DCompareInit(short cardNum, short chn,short axis1, short axis2)
{
	//GT_CompareData ����λ�ñȽ��������

	short sRtn;
	T2DComparePrm Prm;


	//m_CriticalCard.Lock();
	sRtn =GT_2DCompareClear(cardNum,chn);
	if (sRtn)
	{   
		g_ErrorMessage.Format("�忨%dͨ��%d��GT_2DCompareClearָ��ִ���쳣��",cardNum+1,chn);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}



	sRtn =GT_2DCompareMode(cardNum,chn,COMPARE2D_MODE_2D);
	if (sRtn)
	{
		g_ErrorMessage.Format("�忨%dͨ��%d��GT_2DCompareModeָ��ִ���쳣��",cardNum+1,chn);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}


	//// �� HSIO ͨ�� 0 ���һ�����Ϊ 5000 ΢������塣
	//sRtn =GT_2DComparePulse(cardNum,chn,1,0,5000); // ������Ϊ 5000us	
	//if (sRtn)
	//{
	//	g_ErrorMessage.Format("�忨%dͨ��%d��GT_2DComparePulseָ��ִ���쳣��",cardNum+1,chn);
	//	::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
	//	return 1;
	//}

	//���ò���
	Prm.encx =axis1; // X ��Ϊ��
	Prm.ency =axis2; // Y ��Ϊ��
	Prm.maxerr =500; // ������Pulse
	Prm.outputType =0; // �����������
	Prm.source =1; // �Ƚ�Դ������
	Prm.startLevel =0; // ��Ч����
	Prm.threshold =10; // ���ŵ������ֵ
	Prm.time =1500; // ������ us
	sRtn =GT_2DCompareSetPrm(cardNum,chn,&Prm);
	if (sRtn)
	{
		g_ErrorMessage.Format("�忨%dͨ��%d��GT_2DCompareSetPrmָ��ִ���쳣��",cardNum+1,chn);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	//m_CriticalCard.Unlock();
	//û�д���
	return 0;

}

//��άλ�ñȽϹ���д������(cardNum���ţ�chnͨ���ţ�databuf��ά����,fifo������)
GTSMOTION_API UCT_2DCompareData(short cardNum,short chn,short count,T2DCompareData *databuf,short fifo)
{
	//m_CriticalCard.Lock();
	short sRtn;
	//λ�ñȽ����ͨ�� 0,1 �ֱ����HSIO0 ,HSIO1 ��
	sRtn =GT_SetComparePort(cardNum,COMPARE_PORT_HSIO,0,1); 
	if (sRtn)
	{
		g_ErrorMessage.Format("�忨%d��GT_SetComparePortָ��ִ���쳣��",cardNum+1);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	//��ն�άλ�ñȽ�������ݡ�������л�����ʣ���λ������
	sRtn =GT_2DCompareClear(cardNum,chn);
	if (sRtn)
	{
		g_ErrorMessage.Format("�忨%dͨ��%d��GT_2DCompareClearָ��ִ���쳣��",cardNum+1,chn);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}

	sRtn =GT_2DCompareData(cardNum,chn,
		count,
		databuf,
		fifo
		);

	if (sRtn)
	{
		g_ErrorMessage.Format("�忨%dͨ��%d��GT_2DCompareDataָ��ִ���쳣��",cardNum+1,chn);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	//m_CriticalCard.Unlock();

	//û�д���
	return 0;

}

//������άλ�ñȽϹ���
GTSMOTION_API UCT_2DCompareStart(short cardNum, short chn)
{
	//m_CriticalCard.Lock();
	short sRtn;
	sRtn =GT_2DCompareStart(cardNum,chn);
	if (sRtn)
	{
		g_ErrorMessage.Format("�忨%dͨ��%d��GT_2DCompareStartָ��ִ���쳣��",cardNum+1,chn);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	//m_CriticalCard.Unlock();
	//û�д���
	return 0;
}

//ֹͣ��άλ�ñȽϹ���
GTSMOTION_API UCT_2DCompareStop(short cardNum, short chn)
{
	//m_CriticalCard.Lock();
	short sRtn;
	sRtn =GT_2DCompareStop(cardNum,chn);
	if (sRtn)
	{
		g_ErrorMessage.Format("�忨%dͨ��%d��GT_2DCompareStopָ��ִ���쳣��",cardNum+1,chn);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	//m_CriticalCard.Unlock();
	//û�д���
	return 0;
}
//����չIO��
GTSMOTION_API UCT_OpenExtCard(short cardNum,char *pDllName,short count)
{
	// ָ���ֵ����
	short  sRtn;

	for(int i=0;i<count;i++)
	{
		m_CriticalCard.Lock();
		// �������״̬
		sRtn = GT_OpenExtMdl(cardNum,pDllName);

		if (sRtn)
		{   
			g_ErrorMessage.Format(_T("��չIO�忨%d��GT_OpenExtMdlָ��ִ���쳣��"),cardNum+1+i);
			::MessageBox(NULL,g_ErrorMessage,"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
		m_CriticalCard.Unlock();
	}

	//û�д���
	return 0;
}
//������չIO���ļ�
GTSMOTION_API UCT_LoadExtConfig(short cardNum,char *pFileName,short count)
{
	// ָ���ֵ����
	short  sRtn;

	for(int i=0;i<count;i++)
	{
		m_CriticalCard.Lock();
		// �������״̬
		sRtn = GT_LoadExtConfig(cardNum,pFileName);

		if (sRtn)
		{   
			g_ErrorMessage.Format(_T("��չIO�忨%d��GT_LoadExtConfigָ��ִ���쳣��"),cardNum+1+i);
			::MessageBox(NULL,g_ErrorMessage,"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
		m_CriticalCard.Unlock();
	}

	//û�д���
	return 0;
}
//д���
GTSMOTION_API UCT_WriteExtOutBit(short cardNum,short mdl,short bit,short value,short count)
{
	if (!g_bInitCardOK)
	{
//		AfxMessageBox(_T("�˶����ƿ�δ��ʼ����"));
		return 1;
	}
	// ָ���ֵ����
	short  sRtn;
	short m_te;

	if (value==1)
	{
		m_te=0;
	} 

	if (value==0)
	{
		m_te=1;
	}

	//for(int i=0;i<count;i++)
	//{
	//	//m_CriticalCard.Lock();
	//	//д���
	//	sRtn =GT_SetExtIoBit(cardNum,mdl,bit+i,m_te);
	//	g_ErrorMessage.Format("�忨%d���%d��GT_SetExtIoBitָ�",cardNum+1,bit+i);

	//	if (sRtn)
	//	{   
	//		g_ErrorMessage.Format("�忨%d���%d��GT_SetExtIoBitָ��ִ���쳣��",cardNum+1,bit+i);
	//		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
	//		return 1;
	//	}
	//	//m_CriticalCard.Unlock();
	//}
	sRtn =GT_SetExtIoBit(cardNum,mdl,bit,m_te);
	g_ErrorMessage.Format("�忨%d���%d��GT_SetExtIoBitָ�",cardNum+1,bit);
	if (sRtn)
	{   
		g_ErrorMessage.Format("�忨%d���%d��GT_SetExtIoBitָ��ִ���쳣��",cardNum+1,bit);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	//û�д���
	return 0;

}
//������
GTSMOTION_API UCT_ReadExtInBit(short cardNum,short mdl,short bit,BOOL *pValue)
{
	// ָ���ֵ����
	short  sRtn;
	unsigned short usGpiValue;

	//m_CriticalCard.Lock();
	// ��ȡ�忨ͨ���ź�����ֵ

	sRtn=GT_GetExtIoValue(cardNum,mdl,&usGpiValue);

	if(usGpiValue & (1<<bit))
	{
		*pValue=0;
	}
	else
	{
		*pValue=1;
	}

	if (sRtn)
	{   
		g_ErrorMessage.Format("�忨%d����%d��GT_GetExtIoValueָ��ִ���쳣��",cardNum+1,bit);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}


	//m_CriticalCard.Unlock();
	//û�д���
	return 0;
}
//�����
GTSMOTION_API UCT_ReadExtOutBit(short cardNum,short mdl,short bit,BOOL *pValue)
{
	// ָ���ֵ����
	short  sRtn;

	unsigned short usGpiValue;

	//m_CriticalCard.Lock();
	// ��ȡ�忨ͨ���ź����ֵ
	sRtn=GT_GetExtDoValue(cardNum,mdl,&usGpiValue);

	if (sRtn)
	{   
		g_ErrorMessage.Format("�忨%d��ȡ���ֵ%d��GT_GetExtDoValueָ��ִ���쳣��",cardNum+1,bit);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}

	if(usGpiValue & (1<<bit))
	{
		*pValue=0;
	}
	else
	{
		*pValue=1;
	}

	//m_CriticalCard.Unlock();
	//û�д���
	return 0;
}


//Jogģʽ�����˶���ֹͣ
GTSMOTION_API JCT_1AxisMoveDone(short cardNum,short axis,long LDis,double speed,double acc,double dec)
{
		
	if (!g_bInitCardOK)
	{
		//AfxMessageBox(_T("�˶����ƿ�δ��ʼ����"));
		return 1;
	}
	CString str;
	TJogPrm jog;
	long status;
	short  sRtn;
	double dPrfPos,dEncPos;                         //�滮λ��

	//���ƽ��ֹͣ
	if (m_motor_stop)
	{
		//���ƽ��ֹͣ
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}
	// �������ı�������λ
	GT_ClrSts(cardNum,axis);
	// ��AXIS����ΪJogģʽ
	GT_PrfJog(cardNum,axis);

	// ��ȡJog�˶�����
	sRtn=GT_GetJogPrm(cardNum,axis,&jog);
// 	//��ʼ�ٶ�  Jogģʽû�г�ʼ�ٶ�
// 	trap.velStart = start_speed;
	//���ٶ�
	jog.acc = acc;
	//���ٶ�
	jog.dec = dec;

	// ����AXIS���Ŀ���ٶ�
	sRtn=GT_SetVel(cardNum,axis, speed);
//	//ƽ��ʱ��     Jogģʽû��ƽ��
//	trap.smoothTime = SMOOTHTIME;
	// ����Jog�˶�����
	sRtn=GT_SetJogPrm(cardNum,axis, &jog);

	//��ȡ�滮λ��
	GT_GetPrfPos(cardNum,axis, &dPrfPos);


	// ����AXIS����˶�
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//��Ϣѭ��
		UCT_PeekAndPump();
		//���ƽ��ֹͣ
		if (m_motor_stop)
		{
			//���ƽ��ֹͣ
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

		// ��ȡAXIS���״̬
		GT_GetSts(cardNum,axis, &status);


	}while(status&0x400);	// �ȴ�AXIS��滮ֹͣ

	Sleep(400);
	
	return 0;
}
//Jogģʽ���Ὺʼ�˶�
GTSMOTION_API JCT_1AxisStartMove(short cardNum,short axis,long LDis,double speed,double acc,double dec)
{
	if (!g_bInitCardOK)
	{
		//AfxMessageBox(_T("�˶����ƿ�δ��ʼ����"));
		return 1;
	}
	CString str;
	TJogPrm jog;
	short  sRtn;
	double dPrfPos;                         //�滮λ��


	//���ƽ��ֹͣ
	if (m_motor_stop)
	{
		//���ƽ��ֹͣ
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}
	// �������ı�������λ
	GT_ClrSts(cardNum,axis);
	// ��AXIS����ΪJogģʽ
	GT_PrfJog(cardNum,axis);

	// ��ȡJog�˶�����
	sRtn=GT_GetJogPrm(cardNum,axis,&jog);
// 	//��ʼ�ٶ�
// 	trap.velStart = start_speed;
	//���ٶ�
	jog.acc = acc;
	//���ٶ�
	jog.dec = dec;

	// ����AXIS���Ŀ���ٶ�
	sRtn=GT_SetVel(cardNum,axis, speed);
//	//ƽ��ʱ��
//	trap.smoothTime = SMOOTHTIME;
	// ����Jog�˶�����
	sRtn=GT_SetJogPrm(cardNum,axis, &jog);

	//��ȡ�滮λ��
	GT_GetPrfPos(cardNum,axis, &dPrfPos);

	// ����AXIS����˶�
	GT_Update(cardNum,1<<(axis-1));

	return 0;
}


//һάλ�ñȽϹ���д������(cardNum���ţ�chnͨ���ţ�databuf��ά����,fifo������)
GTSMOTION_API UCT_CompareData(short cardNum,short encoder,short source,short pulseType,short startLevel,short time,long *pBuf,short count)
{

	short sRtn;
	sRtn = GT_CompareStop(cardNum);
	if (sRtn)
	{
		g_ErrorMessage.Format("�忨%d��GT_CompareStopָ��ִ���쳣��",cardNum+1);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}

	sRtn =GT_CompareData(cardNum,encoder,source,pulseType,startLevel,time,pBuf,count,pBuf,0);

	if (sRtn)
	{
		g_ErrorMessage.Format("�忨%d��GT_CompareDataָ��ִ���쳣��",cardNum+1);
		::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	//û�д���
	return 0;
}

//�����ԭ��
GTSMOTION_API UCT_BackToHomeNegORIGIN(short cardNum,short axis,long search_pos,long search_home,long home_offset,double home_speed, short home_capture,double acc_speed, double dec_speed)
{
	//m_CriticalCard.Lock();
	//���ƽ��ֹͣ
	if (!g_bInitCardOK)
	{
		//AfxMessageBox(_T("�˶����ƿ�δ��ʼ����"));
		return 1;
	}
	if (m_motor_stop)
	{
		//���ƽ��ֹͣ
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}

	short capture;
	short sRtn;
	TTrapPrm trapPrm;
	long status,pos;
	double prfPos,dEncPos;

	// �����ı�������λ
	GT_ClrSts(cardNum,axis);

	// ʵ��λ�����㣬��������Ư����
	GT_ZeroPos(cardNum,axis);

	// �л�����λ�˶�ģʽ
	GT_PrfTrap(cardNum,axis);

	// ��ȡ��λģʽ�˶�����
	GT_GetTrapPrm(cardNum,axis, &trapPrm);

	/*trapPrm.acc = 0.25;
	trapPrm.dec = 0.25;*/

	if(acc_speed<0)
		trapPrm.acc = 0.25;
	else
		trapPrm.acc = acc_speed;
	if(dec_speed<0)
		trapPrm.dec = 0.25;
	else
		trapPrm.dec = dec_speed;

	trapPrm.smoothTime = SMOOTHTIME;
	// ���õ�λģʽ�˶�����
	GT_SetTrapPrm(cardNum,axis, &trapPrm);

	BOOL bValue;
	short sRst;
	sRst = UCT_GetHomeValue(cardNum,axis,&bValue);
	if (sRst!=0)
		return 1;


	if(!bValue)
	{
		// ����Home����
		GT_SetCaptureMode(cardNum,axis,CAPTURE_HOME);
		if(home_capture == 0)
			GT_SetCaptureSense(cardNum,axis,CAPTURE_HOME,0);
		else
			GT_SetCaptureSense(cardNum,axis,CAPTURE_HOME,1);
		// ���õ�λģʽĿ���ٶȣ�����ԭ���ٶ�
		GT_SetVel(cardNum,axis, home_speed/2);
		////��ȡ������λ��
		//GT_GetEncPos(cardNum,axis, &dEncPos);
		//��ȡ�滮��λ��
		GT_GetPrfPos(cardNum,axis, &prfPos);
		//  ���õ�λģʽĿ��λ�ã���ԭ����������
		GT_SetPos(cardNum,axis, long(prfPos)-search_home);

		//���ƽ��ֹͣ
		if (m_motor_stop)
		{
			//���ƽ��ֹͣ
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

		// �����˶�
		GT_Update(cardNum,1<<(axis-1));

		do
		{
			//��Ϣѭ��
			UCT_PeekAndPump();
			// ��ȡ��״̬
			GT_GetSts(cardNum,axis, &status);

			// ��ȡ����״̬
			sRtn = GT_GetCaptureStatus(cardNum,axis, &capture, &pos);
			//���ƽ��ֹͣ
			if (m_motor_stop)
			{
				//���ƽ��ֹͣ
				GT_Stop(cardNum,1<<(axis-1),IM_STOP);
				return 1;
			}

			// ����˶�ֹͣ�����س�����Ϣ
			if( 0 == ( status & 0x400 ) )
			{
				m_motor_stop=1;
				g_ErrorMessage.Format("��ԭ���У��忨%d���%d����Ҳ���ԭ���������쳣ֹͣ��",cardNum+1,axis);
				::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
				return 1;
			}


			// �ȴ����񴥷�
		}while( 0 == capture );

		// �˶���"����λ��+ƫ����"
		GT_SetPos(cardNum,axis, pos - home_offset);

		// ���˶�״̬�¸���Ŀ��λ��
		GT_Update(cardNum,1<<(axis-1));

		do
		{
			//��Ϣѭ��
			UCT_PeekAndPump();
			// ��ȡ��״̬
			sRtn = GT_GetSts(cardNum,axis, &status);

			//���ƽ��ֹͣ
			if (m_motor_stop)
			{
				//���ƽ��ֹͣ
				GT_Stop(cardNum,1<<(axis-1),IM_STOP);
				return 1;
			}

			// �ȴ��˶�ֹͣ
		}while( status & 0x400 );


		// ��ʱһ��ʱ�䣬�ȴ����ͣ��
		Sleep(50);

	}

	//����������ԭ��
	{
		// ����Home����
		GT_SetCaptureMode(cardNum,axis,CAPTURE_HOME);

		if(home_capture == 0)
			GT_SetCaptureSense(cardNum,axis,CAPTURE_HOME,1);
		else
			GT_SetCaptureSense(cardNum,axis,CAPTURE_HOME,0);

		// ���õ�λģʽĿ���ٶȣ�����ԭ���ٶ�
		GT_SetVel(cardNum,axis, home_speed/2);
		////��ȡ������λ��
		//GT_GetEncPos(cardNum,axis, &dEncPos);
		//��ȡ�滮��λ��
		GT_GetPrfPos(cardNum,axis, &prfPos);
		//  ���õ�λģʽĿ��λ�ã���ԭ����������
		GT_SetPos(cardNum,axis, long(prfPos)+search_pos*8);

		//���ƽ��ֹͣ
		if (m_motor_stop)
		{
			//���ƽ��ֹͣ
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

		// �����˶�
		GT_Update(cardNum,1<<(axis-1));

		do
		{
			//��Ϣѭ��
			UCT_PeekAndPump();
			// ��ȡ��״̬
			GT_GetSts(cardNum,axis, &status);

			// ��ȡ����״̬
			sRtn = GT_GetCaptureStatus(cardNum,axis, &capture, &pos);
			//���ƽ��ֹͣ
			if (m_motor_stop)
			{
				//���ƽ��ֹͣ
				GT_Stop(cardNum,1<<(axis-1),IM_STOP);
				return 1;
			}

			// ����˶�ֹͣ�����س�����Ϣ
			if( 0 == ( status & 0x400 ) )
			{
				m_motor_stop=1;
				g_ErrorMessage.Format("��ԭ���У��忨%d���%d����Ҳ���ԭ���������쳣ֹͣ��",cardNum+1,axis);
				::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
				return 1;
			}


			// �ȴ����񴥷�
		}while( 0 == capture );

		// �˶���"����λ��+ƫ����"
		GT_SetPos(cardNum,axis, pos + search_pos);

		// ���˶�״̬�¸���Ŀ��λ��
		GT_Update(cardNum,1<<(axis-1));

		do
		{
			//��Ϣѭ��
			UCT_PeekAndPump();
			// ��ȡ��״̬
			sRtn = GT_GetSts(cardNum,axis, &status);

			//���ƽ��ֹͣ
			if (m_motor_stop)
			{
				//���ƽ��ֹͣ
				GT_Stop(cardNum,1<<(axis-1),IM_STOP);
				return 1;
			}

			// �ȴ��˶�ֹͣ
		}while( status & 0x400 );


		// ��ʱһ��ʱ�䣬�ȴ����ͣ��
		Sleep(50);
	}


	sRst = UCT_GetHomeValue(cardNum,axis,&bValue);
	if (sRst!=0)
		return 1;
	if(bValue)
	{
		return 1;
	}

	// �������ı�������λ
	GT_ClrSts(cardNum,axis);

	// ����Home����
	GT_SetCaptureMode(cardNum,axis,CAPTURE_HOME);

	if(home_capture == 0)
		GT_SetCaptureSense(cardNum,axis,CAPTURE_HOME,0);
	else
		GT_SetCaptureSense(cardNum,axis,CAPTURE_HOME,1);

	// ���õ�λģʽĿ���ٶȣ�����ԭ���ٶ�
	GT_SetVel(cardNum,axis, home_speed/15);
	////��ȡ������λ��
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	//��ȡ�滮��λ��
	GT_GetPrfPos(cardNum,axis, &prfPos);
	//  ���õ�λģʽĿ��λ�ã���ԭ����������
	GT_SetPos(cardNum,axis, long(prfPos)-search_home);

	//���ƽ��ֹͣ
	if (m_motor_stop)
	{
		//���ƽ��ֹͣ
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}

	// �����˶�
	GT_Update(cardNum,1<<(axis-1));



	do
	{
		//��Ϣѭ��
		UCT_PeekAndPump();
		// ��ȡ��״̬
		GT_GetSts(cardNum,axis, &status);

		// ��ȡ����״̬
		sRtn = GT_GetCaptureStatus(cardNum,axis, &capture, &pos);

		//���ƽ��ֹͣ
		if (m_motor_stop)
		{
			//���ƽ��ֹͣ
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

		// ����˶�ֹͣ�����س�����Ϣ
		if( 0 == ( status & 0x400 ) )
		{
			m_motor_stop=1;
			g_ErrorMessage.Format("��ԭ���У��忨%d���%d����Ҳ���ԭ���������쳣ֹͣ��",cardNum+1,axis);
			::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
		// �ȴ����񴥷�
	}while( 0 == capture );



	// �˶���"����λ��+ƫ����"
	GT_SetPos(cardNum,axis, pos - home_offset);

	//���ƽ��ֹͣ
	if (m_motor_stop)
	{
		//���ƽ��ֹͣ
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}
	// ���˶�״̬�¸���Ŀ��λ��
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//��Ϣѭ��
		UCT_PeekAndPump();
		// ��ȡ��״̬
		sRtn = GT_GetSts(cardNum,axis, &status);

		//���ƽ��ֹͣ
		if (m_motor_stop)
		{
			//���ƽ��ֹͣ
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

		// �ȴ��˶�ֹͣ
	}while( status & 0x400 );

	// ��ʱһ��ʱ�䣬�ȴ����ͣ��
	Sleep(400);


	////��ȡ������λ��
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	////��ȡ�滮��λ��
	//GT_GetPrfPos(cardNum,axis, &prfPos);

	//if (abs(long(dEncPos-prfPos))>=DIF_PULSE)
	//{
	//	m_motor_stop=1;
	//	g_ErrorMessage.Format("��ԭ���У��忨%d���%d���Ŀ��λ�ú�ʵ��λ�ò���̫��,���%ld����!",cardNum+1,axis,abs(long(dEncPos-prfPos)));
	//	::MessageBox(NULL,_T(g_ErrorMessage),"�쳣�Ի���",MB_SYSTEMMODAL|MB_ICONERROR);
	//	return 1;
	//}

	// �������ı�������λ
	GT_ClrSts(cardNum,axis);
	// ʵ��λ�����㣬��������Ư����
	GT_ZeroPos(cardNum,axis);

	//m_CriticalCard.Unlock();
	return 0;

}

GTSMOTION_API UCT_Calc1AxisPTMoveDone(short sCardNum,short sAxis,double dbDis,double dbSpeed,double dbAcc, double dbDecDis,double dbDec,bool bRelative)
{
	double dbPrfPos;
	GT_GetPrfPos(sCardNum,sAxis, &dbPrfPos);
	//��ֹλ��
	double dbDesPos;
	if (bRelative==true)
	{
		dbDesPos = dbPrfPos+dbDis;
	} 
	else
	{
		if((dbDis-dbPrfPos)<dbDecDis)//�����볬���˶�����
			return 1;
		dbDesPos = dbDis;
	}
	//���ͼ��ٵ�λ�� �Լ����ʱ��
	double dbPos[4];
	long lTime[4];

	
	double dbHighDis = dbSpeed*dbSpeed/dbAcc/2;//0���ӵ�����ٵľ���
	double dbHighTime = dbSpeed/dbAcc;//0���ӵ�����ٵ�ʱ��
	//ʵ�ʾ�����ʱ��
	int nHighTime = dbHighTime;   //��һ��ʱ��
	dbHighDis =  dbHighTime*dbHighTime*dbAcc/2;//��һ�ξ���
	double dbRealSpeed = dbAcc*dbHighTime;

	double dbDecSpeed = sqrt(2.0*dbDec*dbDecDis);//��ʼ����ͣ���ٶ�
	double dbDecTime = sqrt(dbDecDis*2.0/dbDec);//����ͣʱ��
	int nDecTime = dbDecTime+1;   //������ʱ��
	dbDecSpeed =  nDecTime*dbDec; //�������ٶ�
	double dbRealDecDis = nDecTime*nDecTime*dbDec/2;//�����پ���


	double dbHighDecDis =(dbRealSpeed*dbRealSpeed-dbDecSpeed*dbDecSpeed)/dbAcc/2;//����ٵ�����ٶξ���	
	double dbHighDecTime = (dbRealSpeed-dbDecSpeed)/dbAcc;//����ٵ������ʱ��
	int nHighDecTime = dbHighDecTime+1;   //����ٵ������ʱ��
	double dbTempAcc = (dbRealSpeed-dbDecSpeed)/nHighDecTime;
	double dbRealHighDecDis = (dbRealSpeed*dbRealSpeed-dbDecSpeed*dbDecSpeed)/dbTempAcc/2;;//����ٵ�����ٶξ���

	
	if((dbHighDis+dbRealHighDecDis+dbRealDecDis)>=(dbDesPos-dbPrfPos))//δ�ﵽ�����
	{
		double dbVHigh = sqrt((2.0*(dbDesPos-dbPrfPos-dbRealDecDis)*dbAcc+dbDecSpeed*dbDecSpeed)/2);
		dbPos[0] = dbVHigh*dbVHigh/2/dbAcc+dbPrfPos;
		lTime[0] = dbVHigh/dbAcc+1;//ʱ��䳤   �յ��ٶȱ�С
		//3333 26
		//4897 20
		//5600  53
		dbVHigh = (dbPos[0]-dbPrfPos)/lTime[0]*2;//ʵ���յ��ٶ�
		dbPos[1] = dbDesPos-dbRealDecDis;
		lTime[1] = 2*(dbPos[1]-dbPos[0])/(dbVHigh+dbDecSpeed);//ʱ����   �յ��ٶȱ��
		dbPos[2] = dbDesPos;//��ʼ����λ��
		lTime[2] = dbDecTime+1;
		dbPos[0]-=dbPrfPos;
		dbPos[1]-=dbPrfPos;
		dbPos[2]-=dbPrfPos;
		return UCT_1AxisPTMoveDone(sCardNum,sAxis,dbPos,lTime,3);
	}
	else//�ﵽ�����
	{
		 dbPos[0] = dbHighDis+dbPrfPos;
		 lTime[0] = nHighTime;
		 dbPos[1] = dbDesPos-dbRealDecDis-dbHighDecDis;
		 lTime[1] = (dbDesPos-dbRealDecDis-dbHighDecDis-dbHighDis)/dbSpeed+1;
		 dbPos[2] = dbDesPos-dbRealDecDis;//��ʼ����λ��
		 lTime[2] = dbHighDecTime+1;
		 dbPos[3] = dbDesPos;
		 lTime[3] = nDecTime+1;

		 dbPos[0]-=dbPrfPos;
		 dbPos[1]-=dbPrfPos;
		 dbPos[2]-=dbPrfPos;
		 dbPos[3]-=dbPrfPos;
		return UCT_1AxisPTMoveDone(sCardNum,sAxis,dbPos,lTime,4);
	}
}

GTSMOTION_API UCT_1AxisPTMoveDone(short sCardNum,short sAxis,double dbDis[],long lTime[],short sDataNum)
{	
	if (!g_bInitCardOK)
	{
		return 1;
	}
	CString str;
	long status;
	short  sRtn;
	//���ƽ��ֹͣ
	if (m_motor_stop)
	{
		//���ƽ��ֹͣ
		GT_Stop(sCardNum,1<<(sAxis-1),IM_STOP);
		return 1;
	}
	if(sDataNum<0)
		return 4;
	// �������ı�������λ
	GT_ClrSts(sCardNum,sAxis);
	// ��AXIS����ΪPTģʽ
	GT_PrfPt(sCardNum,sAxis);
	// ���AXIS���FIFO
	GT_PtClear(sCardNum,sAxis);
	
	short space;
	int ii = 0;
	double dbTempPos =0 ;
	long lTempTime = 0; 
	for (ii =0; ii <sDataNum; ++ii)
	{
		// ��ѯPTģʽFIFO��ʣ��ռ�
		sRtn =  GT_PtSpace (sCardNum,sAxis, &space);
		// ��FIFO�������˶�����
		dbTempPos = dbDis[ii];
		lTempTime += lTime[ii]; 
		if(ii == sDataNum-1)
			sRtn = GT_PtData(sCardNum,sAxis, dbTempPos, lTempTime,PT_SEGMENT_STOP);//pluse��ms
		else
			sRtn = GT_PtData(sCardNum,sAxis, dbTempPos, lTempTime);//pluse��ms
	}
	
	//����PT�˶�
	sRtn = GT_PtStart (sCardNum,1<<(sAxis-1));

	do
	{
		//��Ϣѭ��
		UCT_PeekAndPump();
		Sleep(5);
		// ��ȡAXIS���״̬
		GT_GetSts(sCardNum,sAxis, &status);
		//���ƽ��ֹͣ
		if (m_motor_stop)
		{
			//���ƽ��ֹͣ
			GT_Stop(sCardNum,1<<(sAxis-1),IM_STOP);
			return 1;
		}
	}while(status&0x400);	// �ȴ�AXIS��滮ֹͣ


	if (status & 0x020)
	{
		return 2;
	}
	if (status & 0x040)
	{
		return 3;
	}
	return 0;
}