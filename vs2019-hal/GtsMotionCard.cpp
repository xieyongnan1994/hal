#include "StdAfx.h"
#include "GtsMotionCard.h"
#include "math.h"



//自定义的固高控制卡全局变量和函数20160320

//运动停止标志位
volatile BOOL m_motor_stop=FALSE;
CCriticalSection m_CriticalCard;
//固高卡异常标志位
BOOL g_MotionError=FALSE;
//软件的错误信息
CString g_ErrorMessage=NULL;
//软件的一般信息
CString g_GeneralMessage=NULL;
//报警开始时间
DWORD g_AlarmBeginTime=0;
//有报警发生
BOOL g_AlarmHappen=FALSE;
//板卡初始化成功标志
volatile BOOL g_bInitCardOK=FALSE;
//消息循环
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
	//下面是黎工写的函数
	MSG msg;
	while(::PeekMessage(&msg,NULL,0,0,PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	return true;

}

//打开运动控制卡
GTSMOTION_API UCT_OpenMotionCard(short cardNum,short count)
{
	/*(1)    开环（脉冲）控制模式
	1)  打开运动控制器，GT_Open()；
	2)  复位运动控制器，GT_Reset()。复位后默认的控制模式为“脉冲+方向”的脉冲控制方式。
	若不是采用“脉冲+方向”的控制方式，则可调用 GT_StepPulse()修改；当需要还原为“脉
	冲+方向”的控制方式时，则可调用 GT_StepDir()指令；
	3)  检查相关轴驱动器报警信号有没有连接。（一般若采用步进电机，可能没有驱动器报警信
	号），若没有连接，则应该调用 GT_AlarmOff()指令，使驱动器报警无效，默认是有效的；
	4)  检查相关轴的限位开关，若没有连接，则需要通过调用 GT_LmtsOff()，使限位无效，默认
	是有效的；若有连接，则要检查触发电平是否设置正确，可通过 GT_LmtSns()指令修改；
	5)  在确认前面两步操作之后，调用 GT_ClrSts()，更新设置的状态；
	6)  调用 GT_AxisOn()，使能驱动器，这样相应的电机便能工作了；
	7)  使轴运动，运动后，若出现编码器位置和规划位置方向不一致，则可通过调用 GT_EncSns()
	改变编码器的计数方向*/


	// 指令返回值变量
	short  sRtn;


	for(int i=0;i<count;i++)
	{
		m_CriticalCard.Lock();
		// 清除报警状态
		sRtn = GT_Open(cardNum);
		
		if (sRtn)
		{   
			g_ErrorMessage.Format(_T("板卡%d的GT_Open指令执行异常！"),cardNum+1+i);
			::MessageBox(NULL,g_ErrorMessage,"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
		m_CriticalCard.Unlock();
	}

	//没有错误
	return 0;


}
//关闭运动控制卡
GTSMOTION_API UCT_CloseMotionCard(short cardNum,short count)
{

	// 指令返回值变量
	short  sRtn;


	for(int i=0;i<count;i++)
	{
		m_CriticalCard.Lock();
		// 清除报警状态
		sRtn = GT_Close(cardNum);
		
		if (sRtn)
		{   
			g_ErrorMessage.Format(_T("板卡%d的GT_Close指令执行异常！"),cardNum+1+i);
			::MessageBox(NULL,g_ErrorMessage,"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
		m_CriticalCard.Unlock();
	}

	//没有错误
	return 0;


}
//复位运动控制卡
GTSMOTION_API UCT_ResetMotionCard(short cardNum,short count)
{

	// 指令返回值变量
	short  sRtn;


	for(int i=0;i<count;i++)
	{
		m_CriticalCard.Lock();
		// 清除报警状态
		sRtn = GT_Reset(cardNum);
		
		if (sRtn)
		{   
			g_ErrorMessage.Format(_T("板卡%d的GT_Reset指令执行异常！"),cardNum+1+i);
			::MessageBox(NULL,g_ErrorMessage,"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
		m_CriticalCard.Unlock();
	}

	//没有错误
	return 0;


}
//加载配置文件
GTSMOTION_API UCT_LoadConfig(short cardNum,char *pFile)
{

	// 指令返回值变量
	short  sRtn;

	m_CriticalCard.Lock();
	// 清除报警状态
	sRtn = GT_LoadConfig(cardNum,pFile);
	
	if (sRtn)
	{   
		g_ErrorMessage.Format(_T("板卡%d的GT_LoadConfig指令执行异常！"),cardNum+1);
		::MessageBox(NULL,g_ErrorMessage,"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	m_CriticalCard.Unlock();

	//没有错误
	return 0;

}
//清除报警状态
GTSMOTION_API UCT_ClrSts(short cardNum,short axis,short count)
{

	// 指令返回值变量
	short  sRtn;


	m_motor_stop=FALSE;

	for(int i=0;i<count;i++)
	{
		m_CriticalCard.Lock();
		// 清除报警状态
		sRtn = GT_ClrSts(cardNum,axis+i);
		
		if (sRtn)
		{   
			g_ErrorMessage.Format("板卡%d轴%d的GT_ClrSts指令执行异常！",cardNum+1,axis+i);
			::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
		m_CriticalCard.Unlock();
	}

	//没有错误
	return 0;
}

//伺服on
GTSMOTION_API UCT_AxisOn(short cardNum,short axis,short count)
{
	// 指令返回值变量
	short  sRtn;



	for(int i=0;i<count;i++)
	{
		//m_CriticalCard.Lock();
		// 伺服使能
		sRtn = GT_AxisOn(cardNum,axis+i);
		
		if (sRtn)
		{   
			g_ErrorMessage.Format("板卡%d轴%d的GT_AxisOn指令执行异常！",cardNum+1,axis+i);
			::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
		//m_CriticalCard.Unlock();
	}

	//没有错误
	return 0;

}
//伺服off
GTSMOTION_API UCT_AxisOff(short cardNum,short axis,short count)
{
	// 指令返回值变量
	short  sRtn;


	for(int i=0;i<count;i++)
	{
		//m_CriticalCard.Lock();
		// 伺服使能
		sRtn = GT_AxisOff(cardNum,axis+i);
		
		if (sRtn)
		{   
			g_ErrorMessage.Format("板卡%d轴%d的GT_AxisOn指令执行异常！",cardNum+1,axis+i);
			::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
		//m_CriticalCard.Unlock();
	}

	//没有错误
	return 0;
}

//写输出
GTSMOTION_API UCT_WriteOutBit(short cardNum,short bit,short value,short count)
{
	if (!g_bInitCardOK)
	{
		//AfxMessageBox(_T("运动控制卡未初始化！"));
		return 1;
	}
	// 指令返回值变量
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
		//写输出
		sRtn =GT_SetDoBit(cardNum,MC_GPO,bit+i+1,m_te);
		
		if (sRtn)
		{   
			g_ErrorMessage.Format("板卡%d输出%d的GT_SetDoBit指令执行异常！",cardNum+1,bit+i);
			::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
		//m_CriticalCard.Unlock();
	}

	//没有错误
	return 0;

}
//写脉冲输出
GTSMOTION_API UCT_WriteTimeOutBit(short cardNum,short bit,short value,short reverseTime)
{
	if (!g_bInitCardOK)
	{
		//AfxMessageBox(_T("运动控制卡未初始化！"));
		return 1;
	}
	// 指令返回值变量
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
		//写输出
		sRtn =GT_SetDoBitReverse(cardNum,MC_GPO,bit+1,m_te,reverseTime);

		if (sRtn)
		{   
			g_ErrorMessage.Format("板卡%d输出%d的GT_SetDoBit指令执行异常！",cardNum+1,bit);
			//::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
		//m_CriticalCard.Unlock();
	
	//没有错误
	return 0;

}
//读输入
GTSMOTION_API UCT_ReadInBit(short cardNum,short bit,BOOL *pValue)
{
	// 指令返回值变量
	short  sRtn;
	long lGpiValue;
	if (!g_bInitCardOK)
	{
		//AfxMessageBox(_T("运动控制卡未初始化！"));
		return 1;
	}
	//m_CriticalCard.Lock();
	// 读取板卡通用信号输入值
	
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
			g_ErrorMessage.Format("板卡%d输入%d的GT_GetDi指令执行异常！",cardNum+1,bit);
			::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
	
	
	//m_CriticalCard.Unlock();
	//没有错误
	return 0;
}
//读输出
GTSMOTION_API UCT_ReadOutBit(short cardNum,short bit,BOOL *pValue)
{
	// 指令返回值变量
	short  sRtn;

	long lGpOValue;

	//m_CriticalCard.Lock();
	// 读取板卡通用信号输出值
	sRtn=GT_GetDo(cardNum,MC_GPO,&lGpOValue);
	
	if (sRtn)
	{   
		g_ErrorMessage.Format("板卡%d获取输出值%d的GT_GetDo指令执行异常！",cardNum+1,bit);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
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
	//没有错误
	return 0;
}
//平滑停止
GTSMOTION_API UCT_IMStopMove(short cardNum,short axis,short count)
{

	// 指令返回值变量
	short  sRtn;


	//m_motor_stop=TRUE;

	for(int i=0;i<count;i++)
	{
		//m_CriticalCard.Lock();
		// 平滑停止
		sRtn = GT_Stop(cardNum,1<<(axis+i-1),IM_STOP);
		
		if (sRtn)
		{   
			g_ErrorMessage.Format("板卡%d轴%d的GT_Stop指令执行异常！",cardNum+1,axis+i);
			::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
		//m_CriticalCard.Unlock();
	}

	//没有错误
	return 0;
}
//紧急停止
GTSMOTION_API UCT_EMStopMove(short cardNum,short axis,short count)
{

	// 指令返回值变量
	short  sRtn;

	m_motor_stop=TRUE;

	for(int i=0;i<count;i++)
	{
		//m_CriticalCard.Lock();
		// 紧急停止
		sRtn = GT_Stop(cardNum,1<<(axis+i-1),EM_STOP);
		
		if (sRtn)
		{   
			g_ErrorMessage.Format("板卡%d轴%d的GT_Stop指令执行异常！",cardNum+1,axis+i);
			::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
		//m_CriticalCard.Unlock();
	}

	//没有错误
	return 0;
}

//读取轴状态
GTSMOTION_API UCT_GetAxisSts(short cardNum,short axis,long *pValue)
{
	// 指令返回值变量
	short  sRtn;
	long status;

	//m_CriticalCard.Lock();
	// 读取板卡通用信号输入值
	sRtn=GT_GetSts(cardNum,axis,&status);
	

	*pValue=status;

	if (sRtn)
	{   
		g_ErrorMessage.Format("板卡%d输出%d的GT_GetSts指令执行异常！",cardNum+1,axis);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	//m_CriticalCard.Unlock();
	//没有错误
	return 0;
}
//读取轴伺服报警
GTSMOTION_API UCT_GetServerAlarm(short cardNum,short axis,BOOL *pValue)
{
	// 指令返回值变量
	short  sRtn;
	long status;

	//m_CriticalCard.Lock();
	// 读取板卡通用信号输入值
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
		g_ErrorMessage.Format("板卡%d轴%d的GT_GetSts指令执行异常！",cardNum+1,axis);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	//m_CriticalCard.Unlock();
	//没有错误
	return 0;
}
//读取轴正限位报警
GTSMOTION_API UCT_GetPosLimitAlarm(short cardNum,short axis,BOOL *pValue)
{
	// 指令返回值变量
	short  sRtn;
	long status;

	//m_CriticalCard.Lock();
	// 读取板卡通用信号输入值
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
		g_ErrorMessage.Format("板卡%d轴%d的GT_GetSts指令执行异常！",cardNum+1,axis);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	//m_CriticalCard.Unlock();
	//没有错误
	return 0;
}
//读取轴负限位报警
GTSMOTION_API UCT_GetNegLimitAlarm(short cardNum,short axis,BOOL *pValue)
{
	// 指令返回值变量
	short  sRtn;
	long status;

	//m_CriticalCard.Lock();
	// 读取板卡通用信号输入值
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
		g_ErrorMessage.Format("板卡%d轴%d的GT_GetSts指令执行异常！",cardNum+1,axis);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}

	//没有错误
	return 0;
}
//获取正限位值
GTSMOTION_API UCT_GetPosLimitValue(short cardNum,short axis,BOOL *pValue)
{

	long lPosLmtValue;
	short  sRtn;

	//m_CriticalCard.Lock();
	// 读取板卡正限位信号输入值
	sRtn=GT_GetDi(cardNum,MC_LIMIT_POSITIVE,&lPosLmtValue);
	
	if (sRtn)
	{   
		g_ErrorMessage.Format("板卡%d的GT_GetDi获取正极限值指令执行异常！",cardNum+1);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
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
	//没有错误
	return 0;

}
//获取负限位值
GTSMOTION_API UCT_GetNegLimitValue(short cardNum,short axis,BOOL *pValue)
{

	long lNegLmtValue;
	short  sRtn;

	//m_CriticalCard.Lock();
	// 读取板卡正限位信号输入值
	sRtn=GT_GetDi(cardNum,MC_LIMIT_NEGATIVE,&lNegLmtValue);
	
	if (sRtn)
	{   
		g_ErrorMessage.Format("板卡%d的GT_GetDi获取负限位值指令执行异常！",cardNum+1);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
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
	//没有错误
	return 0;
}
//获取原点值
GTSMOTION_API UCT_GetHomeValue(short cardNum,short axis,BOOL *pValue)
{
	long lOrgValue;
	short  sRtn;

	//m_CriticalCard.Lock();
	// 读取板卡正限位信号输入值
	sRtn=GT_GetDi(cardNum,MC_HOME,&lOrgValue);
	
	if (sRtn)
	{   
		g_ErrorMessage.Format("板卡%d的GT_GetDi获取原点值指令执行异常！",cardNum+1);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
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
	//没有错误
	return 0;
}
//获取规划器的值
GTSMOTION_API UCT_GetPrfPos(short cardNum,short axis,double *pValue)
{

	double dPrfPos;
	short  sRtn;

	//m_CriticalCard.Lock();
	// 获取规划器的值
	sRtn=GT_GetPrfPos(cardNum,axis, &dPrfPos);
	
	if (sRtn)
	{   
		g_ErrorMessage.Format("板卡%d轴%d的GT_GetPrfPos获取原点值指令执行异常！",cardNum+1,axis);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}

	*pValue=dPrfPos;
	//m_CriticalCard.Unlock();
	//没有错误
	return 0;
}
//获取编码器的值
GTSMOTION_API UCT_GetEncPos(short cardNum,short axis,double *pValue)
{

	double dEncPos;
	short  sRtn;

	//m_CriticalCard.Lock();
	// 获取规划器的值
	sRtn=GT_GetEncPos(cardNum,axis, &dEncPos);
	
	if (sRtn)
	{   
		g_ErrorMessage.Format("板卡%d轴%d的GT_GetEncPos获取原点值指令执行异常！",cardNum+1,axis);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}

	*pValue=dEncPos;
	//m_CriticalCard.Unlock();
	//没有错误
	return 0;
}

//单轴回原点（有原点感应器）
GTSMOTION_API UCT_BackToHomeORIGIN(short cardNum,short axis,long search_pos,long search_home,long home_offset,double home_speed,short sHomeCapture)
{
	//m_CriticalCard.Lock();
	//马达平滑停止
	if (!g_bInitCardOK)
	{
		//AfxMessageBox(_T("运动控制卡未初始化！"));
		return 1;
	}
	if (m_motor_stop)
	{
		//马达平滑停止
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}

	short capture;
	short sRtn;
	TTrapPrm trapPrm;
	long status,pos;
	double prfPos,dEncPos;

	// 清除轴的报警和限位
	GT_ClrSts(cardNum,axis);

	// 实际位置清零，并进行零漂补偿
	GT_ZeroPos(cardNum,axis);

	// 切换到点位运动模式
	GT_PrfTrap(cardNum,axis);

	// 读取点位模式运动参数
	GT_GetTrapPrm(cardNum,axis, &trapPrm);

	trapPrm.acc = 0.5;
	trapPrm.dec = 0.5;
	trapPrm.smoothTime = SMOOTHTIME;
	// 设置点位模式运动参数
	GT_SetTrapPrm(cardNum,axis, &trapPrm);

	//第一次回零
	// 设置点位模式目标速度，即运动速度
	GT_SetVel(cardNum,axis, home_speed);

	//读取规划器位置
	GT_GetPrfPos(cardNum,axis, &prfPos);
	// 正向运动一段距离
	GT_SetPos(cardNum,axis, long(prfPos)+search_pos);

	//马达平滑停止
	if (m_motor_stop)
	{
		//马达平滑停止
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}

	// 启动运动
	GT_Update(cardNum,1<<(axis-1));

	Sleep(100);
	do
	{
		//消息循环
		UCT_PeekAndPump();
		// 读取轴状态
		GT_GetSts(cardNum,axis, &status);

		//马达平滑停止
		if (m_motor_stop)
		{
			//马达平滑停止
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

	}while( status & 0x400 );

	// 清除各轴的报警和限位
	GT_ClrSts(cardNum,axis);

	// 启动Home捕获
	GT_SetCaptureMode(cardNum,axis,CAPTURE_HOME);
	//上升沿捕获
	if(1 == sHomeCapture)
		GT_SetCaptureSense(cardNum,axis,CAPTURE_HOME,1); 
	else
		GT_SetCaptureSense(cardNum,axis,CAPTURE_HOME,0); 
	// 设置点位模式目标速度，即回原点速度
	GT_SetVel(cardNum,axis, home_speed*2);
	////读取编码器位置
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	//读取规划器位置
	GT_GetPrfPos(cardNum,axis, &prfPos);
	//  设置点位模式目标位置，即原点搜索距离
	GT_SetPos(cardNum,axis, long(prfPos)-search_home);

	//马达平滑停止
	if (m_motor_stop)
	{
		//马达平滑停止
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}

	// 启动运动
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//消息循环
		UCT_PeekAndPump();
		// 读取轴状态
		GT_GetSts(cardNum,axis, &status);

		// 读取捕获状态
		sRtn = GT_GetCaptureStatus(cardNum,axis, &capture, &pos);
		//马达平滑停止
		if (m_motor_stop)
		{
			//马达平滑停止
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

		// 如果运动停止，返回出错信息
		if( 0 == ( status & 0x400 ) )
		{
			m_motor_stop=1;
			g_ErrorMessage.Format("回原点中，板卡%d输出%d马达找不到原点或是马达异常停止！",cardNum+1,axis);
			::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}


		// 等待捕获触发
	}while( 0 == capture );
	GT_ClrSts(cardNum,axis);
	// 运动到"捕获位置+偏移量"
	GT_SetPos(cardNum,axis, pos + home_offset);

	// 在运动状态下更新目标位置
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//消息循环
		UCT_PeekAndPump();
		// 读取轴状态
		sRtn = GT_GetSts(cardNum,axis, &status);

		//马达平滑停止
		if (m_motor_stop)
		{
			//马达平滑停止
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

		// 等待运动停止
	}while( status & 0x400 );


	// 延时一段时间，等待电机停稳
	Sleep(50);



	//第二次回零
	// 清除各轴的报警和限位
	GT_ClrSts(cardNum,axis);

	// 设置点位模式目标速度，即运动速度
	GT_SetVel(cardNum,axis, home_speed/2);

	//读取规划器位置
	GT_GetPrfPos(cardNum,axis, &prfPos);
	// 正向运动一段距离
	GT_SetPos(cardNum,axis, long(prfPos)+search_pos/10);
	//马达平滑停止
	if (m_motor_stop)
	{
		//马达平滑停止
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}
	// 启动运动
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//消息循环
		UCT_PeekAndPump();
		// 读取轴状态
		GT_GetSts(cardNum,axis, &status);

		//马达平滑停止
		if (m_motor_stop)
		{
			//马达平滑停止
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

	}while( status & 0x400 );

	// 清除各轴的报警和限位
	GT_ClrSts(cardNum,axis);

	// 启动Home捕获
	GT_SetCaptureMode(cardNum,axis,CAPTURE_HOME);
	//上升沿捕获
	if(1 == sHomeCapture)
		GT_SetCaptureSense(cardNum,axis,CAPTURE_HOME,1); 
	else
		GT_SetCaptureSense(cardNum,axis,CAPTURE_HOME,0); 

	// 设置点位模式目标速度，即回原点速度
	GT_SetVel(cardNum,axis, home_speed/15);
	////读取编码器位置
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	//读取规划器位置
	GT_GetPrfPos(cardNum,axis, &prfPos);
	//  设置点位模式目标位置，即原点搜索距离
	GT_SetPos(cardNum,axis, long(prfPos)-search_home);

	//马达平滑停止
	if (m_motor_stop)
	{
		//马达平滑停止
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}

	// 启动运动
	GT_Update(cardNum,1<<(axis-1));



	do
	{
		//消息循环
		UCT_PeekAndPump();
		// 读取轴状态
		GT_GetSts(cardNum,axis, &status);

		// 读取捕获状态
		sRtn = GT_GetCaptureStatus(cardNum,axis, &capture, &pos);

		//马达平滑停止
		if (m_motor_stop)
		{
			//马达平滑停止
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

		// 如果运动停止，返回出错信息
		if( 0 == ( status & 0x400 ) )
		{
			m_motor_stop=1;
			g_ErrorMessage.Format("回原点中，板卡%d输出%d马达找不到原点或是马达异常停止！",cardNum+1,axis);
			::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
		// 等待捕获触发
	}while( 0 == capture );



	// 运动到"捕获位置+偏移量"
	GT_SetPos(cardNum,axis, pos + home_offset);

	//马达平滑停止
	if (m_motor_stop)
	{
		//马达平滑停止
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}
	// 在运动状态下更新目标位置
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//消息循环
		UCT_PeekAndPump();
		// 读取轴状态
		sRtn = GT_GetSts(cardNum,axis, &status);

		//马达平滑停止
		if (m_motor_stop)
		{
			//马达平滑停止
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

		// 等待运动停止
	}while( status & 0x400 );

	// 延时一段时间，等待电机停稳
	Sleep(600);


	////读取编码器位置
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	////读取规划器位置
	//GT_GetPrfPos(cardNum,axis, &prfPos);

	//if (abs(long(dEncPos-prfPos))>=DIF_PULSE)
	//{
	//	m_motor_stop=1;
	//	g_ErrorMessage.Format("回原点中，板卡%d输出%d马达目标位置和实际位置差异太大,差距%ld脉冲!",cardNum+1,axis,abs(long(dEncPos-prfPos)));
	//	::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
	//	return 1;
	//}

	// 清除各轴的报警和限位
	GT_ClrSts(cardNum,axis);
	// 实际位置清零，并进行零漂补偿
	GT_ZeroPos(cardNum,axis);

	//m_CriticalCard.Unlock();
	return 0;

}
//单轴回原点（有原点感应器）
GTSMOTION_API UCT_BackToHomeINDEX(short cardNum,short axis,long search_pos,long search_home,long home_offset,double home_speed)
{
	//m_CriticalCard.Lock();
	//马达平滑停止
	if (!g_bInitCardOK)
	{
		//AfxMessageBox(_T("运动控制卡未初始化！"));
		return 1;
	}
	if (m_motor_stop)
	{
		//马达平滑停止
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}

	short capture;
	short sRtn;
	TTrapPrm trapPrm;
	long status,pos;
	double prfPos,dEncPos;

	// 清除轴的报警和限位
	GT_ClrSts(cardNum,axis);

	// 实际位置清零，并进行零漂补偿
	GT_ZeroPos(cardNum,axis);

	// 切换到点位运动模式
	GT_PrfTrap(cardNum,axis);

	// 读取点位模式运动参数
	GT_GetTrapPrm(cardNum,axis, &trapPrm);

	trapPrm.acc = 0.5;
	trapPrm.dec = 0.5;
	trapPrm.smoothTime = SMOOTHTIME;
	// 设置点位模式运动参数
	GT_SetTrapPrm(cardNum,axis, &trapPrm);

	//第一次回零
	// 设置点位模式目标速度，即运动速度
	GT_SetVel(cardNum,axis, home_speed);

	//读取规划器位置
	GT_GetPrfPos(cardNum,axis, &prfPos);
	// 正向运动一段距离
	GT_SetPos(cardNum,axis, long(prfPos)+search_pos);

	//马达平滑停止
	if (m_motor_stop)
	{
		//马达平滑停止
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}

	// 启动运动
	GT_Update(cardNum,1<<(axis-1));

	Sleep(100);
	do
	{
		//消息循环
		UCT_PeekAndPump();
		// 读取轴状态
		GT_GetSts(cardNum,axis, &status);

		//马达平滑停止
		if (m_motor_stop)
		{
			//马达平滑停止
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

	}while( status & 0x400 );

	// 清除各轴的报警和限位
	GT_ClrSts(cardNum,axis);

	// 启动Home捕获
	GT_SetCaptureMode(cardNum,axis,CAPTURE_INDEX);
	//上升沿捕获
	//GT_SetCaptureSense(axis,CAPTURE_HOME,0); 

	// 设置点位模式目标速度，即回原点速度
	GT_SetVel(cardNum,axis, home_speed/2);
	////读取编码器位置
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	//读取规划器位置
	GT_GetPrfPos(cardNum,axis, &prfPos);
	//  设置点位模式目标位置，即原点搜索距离
	GT_SetPos(cardNum,axis, long(prfPos)-search_home);

	//马达平滑停止
	if (m_motor_stop)
	{
		//马达平滑停止
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}

	// 启动运动
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//消息循环
		UCT_PeekAndPump();
		// 读取轴状态
		GT_GetSts(cardNum,axis, &status);

		// 读取捕获状态
		sRtn = GT_GetCaptureStatus(cardNum,axis, &capture, &pos);
		//马达平滑停止
		if (m_motor_stop)
		{
			//马达平滑停止
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

		// 如果运动停止，返回出错信息
		if( 0 == ( status & 0x400 ) )
		{
			m_motor_stop=1;
			g_ErrorMessage.Format("回原点中，板卡%d输出%d马达找不到原点或是马达异常停止！",cardNum+1,axis);
			::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}


		// 等待捕获触发
	}while( 0 == capture );

	// 运动到"捕获位置+偏移量"
	GT_SetPos(cardNum,axis, pos - home_offset);

	// 在运动状态下更新目标位置
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//消息循环
		UCT_PeekAndPump();
		// 读取轴状态
		sRtn = GT_GetSts(cardNum,axis, &status);

		//马达平滑停止
		if (m_motor_stop)
		{
			//马达平滑停止
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

		// 等待运动停止
	}while( status & 0x400 );


	// 延时一段时间，等待电机停稳
	Sleep(50);



	//第二次回零
	// 清除各轴的报警和限位
	GT_ClrSts(cardNum,axis);

	// 设置点位模式目标速度，即运动速度
	GT_SetVel(cardNum,axis, home_speed/2);

	//读取规划器位置
	GT_GetPrfPos(cardNum,axis, &prfPos);
	// 正向运动一段距离
	GT_SetPos(cardNum,axis, long(prfPos)+search_pos/10);
	//马达平滑停止
	if (m_motor_stop)
	{
		//马达平滑停止
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}
	// 启动运动
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//消息循环
		UCT_PeekAndPump();
		// 读取轴状态
		GT_GetSts(cardNum,axis, &status);

		//马达平滑停止
		if (m_motor_stop)
		{
			//马达平滑停止
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

	}while( status & 0x400 );

	// 清除各轴的报警和限位
	GT_ClrSts(cardNum,axis);

	// 启动Home捕获
	GT_SetCaptureMode(cardNum,axis,CAPTURE_HOME);
	//上升沿捕获
	//GT_SetCaptureSense(axis,CAPTURE_HOME,0); 

	// 设置点位模式目标速度，即回原点速度
	GT_SetVel(cardNum,axis, home_speed/15);
	////读取编码器位置
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	//读取规划器位置
	GT_GetPrfPos(cardNum,axis, &prfPos);
	//  设置点位模式目标位置，即原点搜索距离
	GT_SetPos(cardNum,axis, long(prfPos)-search_home);

	//马达平滑停止
	if (m_motor_stop)
	{
		//马达平滑停止
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}

	// 启动运动
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//消息循环
		UCT_PeekAndPump();
		// 读取轴状态
		GT_GetSts(cardNum,axis, &status);

		// 读取捕获状态
		sRtn = GT_GetCaptureStatus(cardNum,axis, &capture, &pos);

		//马达平滑停止
		if (m_motor_stop)
		{
			//马达平滑停止
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

		// 如果运动停止，返回出错信息
		if( 0 == ( status & 0x400 ) )
		{
			m_motor_stop=1;
			g_ErrorMessage.Format("回原点中，板卡%d输出%d马达找不到原点或是马达异常停止！",cardNum+1,axis);
			::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
		// 等待捕获触发
	}while( 0 == capture );

	// 运动到"捕获位置+偏移量"
	GT_SetPos(cardNum,axis, pos - home_offset);
	//马达平滑停止
	if (m_motor_stop)
	{
		//马达平滑停止
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}
	// 在运动状态下更新目标位置
	GT_Update(cardNum,1<<(axis-1));
	do
	{
		//消息循环
		UCT_PeekAndPump();
		// 读取轴状态
		GT_GetSts(cardNum,axis, &status);

		//马达平滑停止
		if (m_motor_stop)
		{
			//马达平滑停止
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

	}while( status & 0x400 );


	// 延时一段时间，等待电机停稳
	Sleep(50);

	GT_SetCaptureMode(cardNum,axis, CAPTURE_INDEX);
	// 设置点位模式目标速度，即回原点速度
	GT_SetVel(cardNum,axis, home_speed/18);
	// 运动到"捕获位置+偏移量"
	GT_SetPos(cardNum,axis, pos - search_home);


	//马达平滑停止
	if (m_motor_stop)
	{
		//马达平滑停止
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}
	// 在运动状态下更新目标位置
	GT_Update(cardNum,1<<(axis-1));


	do
	{
		//消息循环
		UCT_PeekAndPump();
		// 读取轴状态
		GT_GetSts(cardNum,axis, &status);

		// 读取捕获状态
		sRtn = GT_GetCaptureStatus(cardNum,axis, &capture, &pos);

		//马达平滑停止
		if (m_motor_stop)
		{
			//马达平滑停止
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

		// 如果运动停止，返回出错信息
		if( 0 == ( status & 0x400 ))
		{
			m_motor_stop=1;
			g_ErrorMessage.Format("回原点中，板卡%d输出%d马达捕获不到Z相脉冲或是马达异常停止！",cardNum+1,axis);
			::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
		// 等待捕获触发
	}while( 0 == capture );

	// 运动到"捕获位置+偏移量"
	GT_SetPos(cardNum,axis, pos - home_offset);
	//马达平滑停止
	if (m_motor_stop)
	{
		//马达平滑停止
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}
	// 在运动状态下更新目标位置
	GT_Update(cardNum,1<<(axis-1));
	do
	{
		//消息循环
		UCT_PeekAndPump();
		// 读取轴状态
		GT_GetSts(cardNum,axis, &status);

		//马达平滑停止
		if (m_motor_stop)
		{
			//马达平滑停止
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

	}while( status & 0x400 );


	// 延时一段时间，等待电机停稳
	Sleep(600);

	////读取编码器位置
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	////读取规划器位置
	//GT_GetPrfPos(cardNum,axis, &prfPos);

	//if (abs(long(dEncPos-prfPos))>=DIF_PULSE)
	//{
	//	m_motor_stop=1;
	//	g_ErrorMessage.Format("回原点中，板卡%d输出%d马达目标位置和实际位置差异太大,差距%ld脉冲!",cardNum+1,axis,abs(long(dEncPos-prfPos)));
	//	::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
	//	return 1;
	//}

	// 清除各轴的报警和限位
	GT_ClrSts(cardNum,axis);
	// 实际位置清零，并进行零漂补偿
	GT_ZeroPos(cardNum,axis);

	//m_CriticalCard.Unlock();
	return 0;

}
//单轴回原点（正限位作为原点）
GTSMOTION_API UCT_BackToHomePOSITIVE(short cardNum,short axis,long search_pos,long search_home,long home_offset,double home_speed)
{
	if (!g_bInitCardOK)
	{
		//AfxMessageBox(_T("运动控制卡未初始化！"));
		return 1;
	}
	//马达平滑停止
	if (m_motor_stop)
	{
		//马达平滑停止
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}

	TTrapPrm trapPrm;
	long status;
	double prfPos,dEncPos;

	// 清除各轴的报警和限位
	GT_ClrSts(cardNum,axis);

	// 实际位置清零，并进行零漂补偿
	GT_ZeroPos(cardNum,axis);
	// 切换到点位运动模式
	GT_PrfTrap(cardNum,axis);

	// 读取点位模式运动参数
	GT_GetTrapPrm(cardNum,axis, &trapPrm);

	trapPrm.acc = 0.5;
	trapPrm.dec = 0.5;
	trapPrm.smoothTime = SMOOTHTIME;
	// 设置点位模式运动参数
	GT_SetTrapPrm(cardNum,axis, &trapPrm);

	//第一次回零
	// 设置点位模式目标速度，即运动速度
	GT_SetVel(cardNum,axis, home_speed);
	////读取编码器位置
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	//读取规划器位置
	GT_GetPrfPos(cardNum,axis, &prfPos);
	// 负向运动一段距离
	GT_SetPos(cardNum,axis, long(prfPos)-search_pos);

	//马达平滑停止
	if (m_motor_stop)
	{
		//马达平滑停止
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}
	// 启动运动
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//消息循环
		UCT_PeekAndPump();
		// 读取轴状态
		GT_GetSts(cardNum,axis, &status);
		//马达平滑停止
		if (m_motor_stop)
		{
			//马达平滑停止
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

	}while( status & 0x400 );

	// 清除各轴的报警和限位
	GT_ClrSts(cardNum,axis);


	// 设置点位模式目标速度，即回原点速度
	GT_SetVel(cardNum,axis, home_speed/2);
	////读取编码器位置
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	//读取规划器位置
	GT_GetPrfPos(cardNum,axis, &prfPos);
	//  设置点位模式目标位置，即原点搜索距离
	GT_SetPos(cardNum,axis,long(prfPos)+search_home);

	//马达平滑停止
	if (m_motor_stop)
	{
		//马达平滑停止
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}
	// 启动运动
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//消息循环
		UCT_PeekAndPump();
		// 读取轴状态
		GT_GetSts(cardNum,axis, &status);
		//马达平滑停止
		if (m_motor_stop)
		{
			//马达平滑停止
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

		// 等待运动停止
	}while( status & 0x400 );

	// 延时一段时间，等待电机停稳
	Sleep(50);



	//第二次回零
	// 清除各轴的报警和限位
	GT_ClrSts(cardNum,axis);

	// 设置点位模式目标速度，即运动速度
	GT_SetVel(cardNum,axis, home_speed/2);
	////读取编码器位置
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	//读取规划器位置
	GT_GetPrfPos(cardNum,axis, &prfPos);
	// 负向运动一段距离
	GT_SetPos(cardNum,axis, long(prfPos)-search_pos/10);

	//马达平滑停止
	if (m_motor_stop)
	{
		//马达平滑停止
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}

	// 启动运动
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//消息循环
		UCT_PeekAndPump();
		// 读取轴状态
		GT_GetSts(cardNum,axis, &status);

		//马达平滑停止
		if (m_motor_stop)
		{
			//马达平滑停止
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

	}while( status & 0x400 );

	// 清除各轴的报警和限位
	GT_ClrSts(cardNum,axis);

	// 设置点位模式目标速度，即回原点速度
	GT_SetVel(cardNum,axis, home_speed/10);
	////读取编码器位置
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	//读取规划器位置
	GT_GetPrfPos(cardNum,axis, &prfPos);
	//设置点位模式目标位置，即原点搜索距离
	GT_SetPos(cardNum,axis, long(prfPos)+search_home);

	//马达平滑停止
	if (m_motor_stop)
	{
		//马达平滑停止
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}
	// 启动运动
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//消息循环
		UCT_PeekAndPump();
		// 读取轴状态
		GT_GetSts(cardNum,axis, &status);

		//马达平滑停止
		if (m_motor_stop)
		{
			//马达平滑停止
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}


		// 等待捕获触发
	}while( status & 0x400);


	// 延时一段时间，等待电机停稳
	Sleep(600);

	////读取编码器位置
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	////读取规划器位置
	//GT_GetPrfPos(cardNum,axis, &prfPos);

	//if (abs(long(dEncPos-prfPos))>=DIF_PULSE)
	//{
	//	g_ErrorMessage.Format("回原点中，板卡%d输出%d马达目标位置和实际位置差异太大！",cardNum+1,axis);
	//	::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
	//	return 1;
	//}

	// 清除各轴的报警和限位
	GT_ClrSts(cardNum,axis);
	// 位置清零
	GT_ZeroPos(cardNum,axis);

	return 0;
}
//单轴回原点（负限位作为原点）
GTSMOTION_API UCT_BackToHomeNEGATIVE(short cardNum,short axis,long search_pos,long search_home,long home_offset,double home_speed)
{
	if (!g_bInitCardOK)
	{
		//AfxMessageBox(_T("运动控制卡未初始化！"));
		return 1;
	}
	//马达平滑停止
	if (m_motor_stop)
	{
		//马达平滑停止
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}

	TTrapPrm trapPrm;
	long status;
	double prfPos,dEncPos;

	// 清除各轴的报警和限位
	GT_ClrSts(cardNum,axis);

	// 实际位置清零，并进行零漂补偿
	GT_ZeroPos(cardNum,axis);
	// 切换到点位运动模式
	GT_PrfTrap(cardNum,axis);

	// 读取点位模式运动参数
	GT_GetTrapPrm(cardNum,axis, &trapPrm);

	trapPrm.acc = 0.5;
	trapPrm.dec = 0.5;
	trapPrm.smoothTime = SMOOTHTIME;
	// 设置点位模式运动参数
	GT_SetTrapPrm(cardNum,axis, &trapPrm);

	//第一次回零
	// 设置点位模式目标速度，即运动速度
	//GT_SetVel(cardNum,axis, home_speed);
	//////读取编码器位置
	////GT_GetEncPos(cardNum,axis, &dEncPos);
	////读取规划器位置
	//GT_GetPrfPos(cardNum,axis, &prfPos);
	//// 正向运动一段距离
	//GT_SetPos(cardNum,axis, long(prfPos)+search_pos);

	////马达平滑停止
	//if (m_motor_stop)
	//{
	//	//马达平滑停止
	//	GT_Stop(cardNum,1<<(axis-1),IM_STOP);
	//	return 1;
	//}
	//// 启动运动
	//GT_Update(cardNum,1<<(axis-1));

	//do
	//{
	//	//消息循环
	//	UCT_PeekAndPump();
	//	// 读取轴状态
	//	GT_GetSts(cardNum,axis, &status);

	//	//马达平滑停止
	//	if (m_motor_stop)
	//	{
	//		//马达平滑停止
	//		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
	//		return 1;
	//	}

	//}while( status & 0x400 );

	// 清除各轴的报警和限位
	GT_ClrSts(cardNum,axis);


	// 设置点位模式目标速度，即回原点速度
	GT_SetVel(cardNum,axis, home_speed/2);
	////读取编码器位置
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	//读取规划器位置
	GT_GetPrfPos(cardNum,axis, &prfPos);
	//  设置点位模式目标位置，即原点搜索距离
	GT_SetPos(cardNum,axis,long(prfPos)-search_home);

	//马达平滑停止
	if (m_motor_stop)
	{
		//马达平滑停止
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}
	// 启动运动
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//消息循环
		UCT_PeekAndPump();
		// 读取轴状态
		GT_GetSts(cardNum,axis, &status);
		//马达平滑停止
		if (m_motor_stop)
		{
			//马达平滑停止
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

		// 等待运动停止
	}while( status & 0x400 );

	// 延时一段时间，等待电机停稳
	Sleep(50);



	//第二次回零
	// 清除各轴的报警和限位
	GT_ClrSts(cardNum,axis);

	// 设置点位模式目标速度，即运动速度
	GT_SetVel(cardNum,axis, home_speed/2);
	////读取编码器位置
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	//读取规划器位置
	GT_GetPrfPos(cardNum,axis, &prfPos);
	// 负向运动一段距离
	GT_SetPos(cardNum,axis, long(prfPos)+search_pos/10);

	//马达平滑停止
	if (m_motor_stop)
	{
		//马达平滑停止
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}

	// 启动运动
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//消息循环
		UCT_PeekAndPump();
		// 读取轴状态
		GT_GetSts(cardNum,axis, &status);

		//马达平滑停止
		if (m_motor_stop)
		{
			//马达平滑停止
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

	}while( status & 0x400 );

	// 清除各轴的报警和限位
	GT_ClrSts(cardNum,axis);

	// 设置点位模式目标速度，即回原点速度
	GT_SetVel(cardNum,axis, home_speed/10);
	////读取编码器位置
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	//读取规划器位置
	GT_GetPrfPos(cardNum,axis, &prfPos);
	//设置点位模式目标位置，即原点搜索距离
	GT_SetPos(cardNum,axis, long(prfPos)-search_home);

	//马达平滑停止
	if (m_motor_stop)
	{
		//马达平滑停止
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}
	// 启动运动
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//消息循环
		UCT_PeekAndPump();
		// 读取轴状态
		GT_GetSts(cardNum,axis, &status);

		//马达平滑停止
		if (m_motor_stop)
		{
			//马达平滑停止
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}


		// 等待捕获触发
	}while( status & 0x400);


	// 延时一段时间，等待电机停稳
	Sleep(600);

	////读取编码器位置
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	////读取规划器位置
	//GT_GetPrfPos(cardNum,axis, &prfPos);

	//if (abs(long(dEncPos-prfPos))>=DIF_PULSE)
	//{
	//	g_ErrorMessage.Format("回原点中，板卡%d输出%d马达目标位置和实际位置差异太大！",cardNum+1,axis);
	//	::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
	//	return 1;
	//}

	// 清除各轴的报警和限位
	GT_ClrSts(cardNum,axis);
	// 位置清零
	GT_ZeroPos(cardNum,axis);

	return 0;
}

//单轴运动到停止（bRelative=true相对运动，bRelative=false绝对运动，start_speed起点速度，speed目标速度）
GTSMOTION_API UCT_1AxisMoveDone(short cardNum,short axis,long LDis,double start_speed,double speed,double acc,double dec,bool bRelative)
{
		
	if (!g_bInitCardOK)
	{
		//AfxMessageBox(_T("运动控制卡未初始化！"));
		return 1;
	}
	CString str;
	TTrapPrm trap;
	long status;
	short  sRtn;
	double dPrfPos,dEncPos;                         //规划位置

	//马达平滑停止
	if (m_motor_stop)
	{
		//马达平滑停止
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}
	// 清除各轴的报警和限位
	GT_ClrSts(cardNum,axis);
	// 将AXIS轴设为点位模式
	GT_PrfTrap(cardNum,axis);

	// 读取点位运动参数
	sRtn=GT_GetTrapPrm(cardNum,axis,&trap);
	//初始速度
	trap.velStart = start_speed;
	//加速度
	trap.acc = acc;
	//减速度
	trap.dec = dec;

	// 设置AXIS轴的目标速度
	sRtn=GT_SetVel(cardNum,axis, speed);
	//平滑时间
	trap.smoothTime = SMOOTHTIME;
	// 设置点位运动参数
	sRtn=GT_SetTrapPrm(cardNum,axis, &trap);


	//读取规划位置
	GT_GetPrfPos(cardNum,axis, &dPrfPos);

	if (bRelative==true)
	{
		// 设置AXIS轴的目标位置（相对）
		GT_SetPos(cardNum,axis, long(dPrfPos+LDis));

	} 
	else
	{
		// 设置AXIS轴的目标位置（绝对）
		GT_SetPos(cardNum,axis, LDis);
	}

	/*double dEncPos1,dEncPos2;
	dEncPos1=0;
	dEncPos2=0;

	GT_GetEncPos(cardNum,axis, &dEncPos1);*/

	// 启动AXIS轴的运动
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//消息循环
		UCT_PeekAndPump();
		Sleep(5);
		// 读取AXIS轴的状态
		GT_GetSts(cardNum,axis, &status);
		//马达平滑停止
		if (m_motor_stop)
		{
			//马达平滑停止
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}


	}while(status&0x400);	// 等待AXIS轴规划停止


	/*GT_GetEncPos(cardNum,axis, &dEncPos2);

	if (dEncPos2 == dEncPos1)
	{
		g_ErrorMessage.Format(_T("卡%d轴%d编码器反馈异常，请检查线路或板卡是否有问题！"),cardNum,axis);
	}*/

	if (status & 0x020)
	{

		//AfxMessageBox("正限位被触发，请排除异常后，复位机台!");
		return 2;

	}
	if (status & 0x040)
	{

		//AfxMessageBox("负限位被触发，请排除异常后，复位机台!");
		return 3;
	}

	//Sleep(20);  //减CT  modify by Lance at 2019.12.11
	//读取编码器位置,板卡1的1~4轴为伺服电机
	/*if ((CARD_NO1==cardNum)&&(axis<=AXIS_NO4))
	{*/
		//GT_GetEncPos(cardNum,axis, &dEncPos);
		//if (bRelative)
		//{
		//	if (abs(long(dEncPos)-(long)dPrfPos-LDis)>=DIF_PULSE)
		//	{
		//		CString str;
		//		str.Format("板卡%d第%d轴目标位置和实际位置差异太大！",cardNum+1,axis);
		//		AfxMessageBox(str);
		//		//AfxMessageBox(_T("目标位置和实际位置差异太大！"));
		//		return 4;
		//	}
		//} 
		//else
		//{
		//	if (abs(long(dEncPos)-LDis)>=DIF_PULSE)
		//	{
		//		CString str;
		//		str.Format("板卡%d第%d轴目标位置和实际位置差异太大！",cardNum+1,axis);
		//		AfxMessageBox(str);
		//		//AfxMessageBox(_T("目标位置和实际位置差异太大！"));
		//		return 4;
		//	}
		//}
	//}
	return 0;
}
//单轴开始运动（bRelative=true相对运动，bRelative=false绝对运动，start_speed起点速度，speed目标速度）
GTSMOTION_API UCT_1AxisStartMove(short cardNum,short axis,long LDis,double start_speed,double speed,double acc,double dec,bool bRelative)
{
	if (!g_bInitCardOK)
	{
		//AfxMessageBox(_T("运动控制卡未初始化！"));
		return 1;
	}
	CString str;
	TTrapPrm trap;
	short  sRtn;
	double dPrfPos;                         //规划位置


	//马达平滑停止
	if (m_motor_stop)
	{
		//马达平滑停止
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}
	// 清除各轴的报警和限位
	GT_ClrSts(cardNum,axis);
	// 将AXIS轴设为点位模式
	GT_PrfTrap(cardNum,axis);

	// 读取点位运动参数
	sRtn=GT_GetTrapPrm(cardNum,axis,&trap);
	//初始速度
	trap.velStart = start_speed;
	//加速度
	trap.acc = acc;
	//减速度
	trap.dec = dec;

	// 设置AXIS轴的目标速度
	sRtn=GT_SetVel(cardNum,axis, speed);
	//平滑时间
	trap.smoothTime = SMOOTHTIME;
	// 设置点位运动参数
	sRtn=GT_SetTrapPrm(cardNum,axis, &trap);


	//读取规划位置
	GT_GetPrfPos(cardNum,axis, &dPrfPos);

	if (bRelative==true)
	{
		// 设置AXIS轴的目标位置（相对）
		GT_SetPos(cardNum,axis, long(dPrfPos+LDis));
	} 
	else
	{
		// 设置AXIS轴的目标位置（绝对）
		GT_SetPos(cardNum,axis, LDis);
	}

	// 启动AXIS轴的运动
	GT_Update(cardNum,1<<(axis-1));


	return 0;
}
//单轴运动状态(pValue=0停止 pValue=1运动中)
GTSMOTION_API UCT_1AxisCheckMoveSts(short cardNum,short axis,long *pValue)
{
	// 指令返回值变量
	short  sRtn;

	long status;


	//m_CriticalCard.Lock();
	// 读取板卡通用信号输入值
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
		g_ErrorMessage.Format("板卡%d轴%d的GT_GetSts指令执行异常！",cardNum+1,axis);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	//m_CriticalCard.Unlock();
	//没有错误
	return 0;
}
//检查单轴运动是否到位
GTSMOTION_API UCT_1AxisCheckMoveDone(short cardNum,short axis,long *pValue)
{
	// 指令返回值变量
	short  sRtn;
	long status;
	
	//m_CriticalCard.Lock();
	// 读取板卡通用信号输入值
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
		g_ErrorMessage.Format("板卡%d轴%d的GT_GetSts指令执行异常！",cardNum+1,axis);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	//m_CriticalCard.Unlock();
	//没有错误
	return 0;
}


//坐标系对应的直线插补开始运动
GTSMOTION_API UCT_LMotionStart(short cardNum, short coordinate, short buffer)
{
	if (!g_bInitCardOK)
	{
//		AfxMessageBox(_T("运动控制卡未初始化！"));
		return 1;
	}
	// 指令返回值变量
	short  sRtn;

	//m_CriticalCard.Lock();
	UCT_ClrSts(cardNum,AXIS_NO1,8);
	// 启动板卡cardNum坐标系coordinate缓存区buffer的插补运动
	sRtn = GT_CrdStart(cardNum,coordinate,buffer); 
	

	if (sRtn)
	{   
		g_ErrorMessage.Format("板卡%d坐标系%d的缓存区%d的GT_CrdStart指令执行异常！",cardNum+1,coordinate,buffer);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	//m_CriticalCard.Unlock();

	//没有错误
	return 0;

}
//插补运动坐标系运动状态（pRun=1运动中，pSegment当前已经完成的插补段数）
GTSMOTION_API UCT_CheckLMotionSts(short cardNum,short *pRun,long *pSegment, short coordinate, short buffer)
{
	// 指令返回值变量
	short sRtn;
	// 坐标系运动状态查询变量
	short run;
	// 坐标系运动完成段查询变量
	long segment;

	m_CriticalCard.Lock();
	// 查询坐标系1的FIFO的插补运动状态
	sRtn = GT_CrdStatus(
		cardNum,     // 卡号cardNum
		coordinate,	// 坐标系是坐标系1
		&run,		// 读取插补运动状态
		&segment,	// 读取当前已经完成的插补段数
		buffer);	// 查询坐标系1的buffer缓存区
	m_CriticalCard.Unlock();

	if (sRtn)
	{   
		g_ErrorMessage.Format("板卡%d坐标系%d的缓存区%d的GT_CrdStatus指令执行异常！",cardNum+1,coordinate,buffer);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}

	*pRun=run;
	*pSegment=segment;
	

	//没有错误
	return 0;
}
//清除此坐标系缓存区中的数据
GTSMOTION_API UCT_CrdClear(short cardNum, short coordinate, short buffer)
{
	// 指令返回值变量
	short  sRtn;

	m_CriticalCard.Lock();
	// 清除此缓存区中的数据
	sRtn = GT_CrdClear(cardNum,coordinate,buffer);

	m_CriticalCard.Unlock();

	if (sRtn)
	{   
		g_ErrorMessage.Format("板卡%d坐标系%d的缓存区%d的GT_CrdClear指令执行异常！",cardNum+1,coordinate,buffer);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}

	//没有错误
	return 0;

}
// 缓存区延时指令
GTSMOTION_API UCT_BufDelay(short cardNum, short delay, short coordinate, short buffer)
{
	// 指令返回值变量
	short  sRtn;


	// 缓存区延时指令
	sRtn = GT_BufDelay(cardNum,coordinate,delay,buffer);

	if (sRtn)
	{   
		g_ErrorMessage.Format("板卡%d坐标系%d的缓存区%d的GT_BufDelay指令执行异常！",cardNum+1,coordinate,buffer);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}

	//没有错误
	return 0;
}

//两轴直线插补初始化
GTSMOTION_API UCT_2AxisLMotionInit(short cardNum, short axis1, short axis2, short coordinate, short buffer)
{
	// 指令返回值变量
	short  sRtn;
	//m_CriticalCard.Lock();
	// 建立坐标系
	TCrdPrm crdPrm;

	memset(&crdPrm,0,sizeof(crdPrm));
	crdPrm.dimension = 2;                        // 建立二维的坐标系
	crdPrm.synVelMax = SYNVELMAX;                // 坐标系的最大合成速度是: SYNVELMAX pulse/ms
	crdPrm.synAccMax = SYNACCMAX;                // 坐标系的最大合成加速度是: SYNACCMAX pulse/ms^2
	crdPrm.evenTime = EVENTIME;                 // 坐标系的最小匀速时间为EVENTIME ms
	crdPrm.profile[axis1-1] = 1;                  // 规划器1对应到X轴                       
	crdPrm.profile[axis2-1] = 2;                  // 规划器2对应到Y轴                   
	crdPrm.setOriginFlag = 1;                    // 需要设置加工坐标系原点位置
	crdPrm.originPos[axis1-1] = 0;               // 加工坐标系原点位置在(0,0)，即与机床坐标系原点重合
	crdPrm.originPos[axis2-1] = 0;

	sRtn = GT_SetCrdPrm(cardNum,coordinate,&crdPrm);
	if (sRtn)
	{   
		g_ErrorMessage.Format("板卡%d轴坐标系%d的GT_SetCrdPrm指令执行异常！",cardNum+1,coordinate);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	// 即将把数据存入坐标系1的FIFO0中，所以要首先清除此缓存区中的数据
	m_CriticalCard.Lock();
	sRtn = GT_CrdClear(cardNum,coordinate,buffer);
	m_CriticalCard.Unlock();
	if (sRtn)
	{   
		g_ErrorMessage.Format("板卡%d坐标系%d的缓存区%d的GT_CrdClear指令执行异常！",cardNum+1,coordinate,buffer);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 2;
	}
	//m_CriticalCard.Unlock();
	//没有错误
	return 0;

}
//两轴直线插补写入数据(acc加速度，speed目标速度，end_speed终点速度，axis1_data轴1数据，axis2_data轴2数据,coordinate坐标系，buffer缓冲区)
GTSMOTION_API UCT_2AxisLMotionData(short cardNum,double acc,double speed, double end_speed,long axis1_data,long axis2_data, short coordinate, short buffer)
{
	// 指令返回值变量
	short  sRtn;

	// 坐标系的缓存区剩余空间查询变量
	long space;
	//m_CriticalCard.Lock();
	
	// 查询坐标系1的FIFO0所剩余的空间
	sRtn = GT_CrdSpace(cardNum,coordinate, &space, 0);
	if (sRtn)
	{   
		g_ErrorMessage.Format("板卡%d坐标系%d的GT_CrdSpace指令执行异常！",cardNum+1,coordinate);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}

	if (space==0)
	{   
		g_ErrorMessage.Format("板卡%d坐标系%d的缓存区%d剩余空间为零，数据写入失败！",cardNum+1,coordinate,buffer);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 2;
	}


	// 向缓存区写入第一段插补数据（高速段）
	sRtn = GT_LnXY( 
		cardNum,                      //卡号cardNum
		coordinate,				      // 该插补段的坐标系是坐标系coordinate
		axis1_data, axis2_data,		  // 该插补段的终点坐标: pulse
		speed,			             // 该插补段的目标速度：pulse/ms
		acc,			             // 插补段的加速度：pulse/ms^2
		end_speed,				     // 终点速度为end_speed
		buffer);				     // 向坐标系1的FIFO0缓存区传递该直线插补数据

	if (sRtn)
	{   
		g_ErrorMessage.Format("板卡%d坐标系%d的GT_LnXY指令执行异常！",cardNum+1,coordinate);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 3;
	}
	//m_CriticalCard.Unlock();
	//没有错误
	return 0;

}

//三轴直线插补初始化
GTSMOTION_API UCT_3AxisLMotionInit(short cardNum, short axis1, short axis2, short axis3,short coordinate, short buffer)
{
	// 指令返回值变量
	short  sRtn;
	//m_CriticalCard.Lock();
	// 建立坐标系
	TCrdPrm crdPrm;


	memset(&crdPrm,0,sizeof(crdPrm));
	crdPrm.dimension = 3;                        // 建立三维的坐标系
	crdPrm.synVelMax = SYNVELMAX;                // 坐标系的最大合成速度是: SYNVELMAX pulse/ms
	crdPrm.synAccMax = SYNACCMAX;                // 坐标系的最大合成加速度是: SYNACCMAX pulse/ms^2
	crdPrm.evenTime = EVENTIME;                 // 坐标系的最小匀速时间为EVENTIME ms
	crdPrm.profile[axis1-1] = 1;                 // 规划器1对应到X轴                       
	crdPrm.profile[axis2-1] = 2;                 // 规划器2对应到Y轴
	crdPrm.profile[axis3-1] = 3;                 // 规划器3对应到Z轴  
	crdPrm.setOriginFlag = 1;                    // 需要设置加工坐标系原点位置
	crdPrm.originPos[axis1-1] = 0;               // 加工坐标系原点位置在(0,0,0)，即与机床坐标系原点重合
	crdPrm.originPos[axis2-1] = 0;
	crdPrm.originPos[axis3-1] = 0;


	sRtn = GT_SetCrdPrm(cardNum,coordinate,&crdPrm);
	if (sRtn)
	{   
		g_ErrorMessage.Format("板卡%d轴坐标系%d的GT_SetCrdPrm指令执行异常！",cardNum+1,coordinate);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	// 即将把数据存入坐标系1的FIFO0中，所以要首先清除此缓存区中的数据
	m_CriticalCard.Lock();
	sRtn = GT_CrdClear(cardNum,coordinate,buffer);
	m_CriticalCard.Unlock();
	if (sRtn)
	{   
		g_ErrorMessage.Format("板卡%d坐标系%d的缓存区%d的GT_CrdClear指令执行异常！",cardNum+1,coordinate,buffer);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 2;
	}
	//m_CriticalCard.Unlock();
	//没有错误
	return 0;

}
//三轴直线插补写入数据(acc加速度，speed目标速度，end_speed终点速度，axis1_data轴1数据，axis2_data轴2数据，axis3_data轴3数据,coordinate坐标系，buffer缓冲区)
GTSMOTION_API UCT_3AxisLMotionData(short cardNum,double acc,double speed, double end_speed,long axis1_data,long axis2_data,long axis3_data, short coordinate, short buffer)
{
	// 指令返回值变量
	short  sRtn;

	// 坐标系的缓存区剩余空间查询变量
	long space;
	//m_CriticalCard.Lock();

	// 查询坐标系1的FIFO0所剩余的空间
	sRtn = GT_CrdSpace(cardNum,coordinate, &space, 0);
	if (sRtn)
	{   
		g_ErrorMessage.Format("板卡%d坐标系%d的GT_CrdSpace指令执行异常！",cardNum+1,coordinate);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}

	if (space==0)
	{   
		g_ErrorMessage.Format("板卡%d坐标系%d的缓存区%d剩余空间为零，数据写入失败！",cardNum+1,coordinate,buffer);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 2;
	}


	// 向缓存区写入第一段插补数据（高速段）
	sRtn = GT_LnXYZ( 
		cardNum,                                       //卡号cardNum
		coordinate,				                      // 该插补段的坐标系是坐标系coordinate
		axis1_data, axis2_data, axis3_data,    		  // 该插补段的终点坐标: pulse
		speed,			                              // 该插补段的目标速度：pulse/ms
		acc,			                              // 插补段的加速度：pulse/ms^2
		end_speed,				                      // 终点速度为end_speed
		buffer);				                      // 向坐标系1的FIFO0缓存区传递该直线插补数据

	if (sRtn)
	{   
		g_ErrorMessage.Format("板卡%d坐标系%d的GT_LnXYZ指令执行异常！",cardNum+1,coordinate);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 3;
	}
	//m_CriticalCard.Unlock();
	//没有错误
	return 0;

}

//四轴直线插补初始化
GTSMOTION_API UCT_4AxisLMotionInit(short cardNum, short axis1, short axis2, short axis3, short axis4,short coordinate, short buffer)
{
	// 指令返回值变量
	short  sRtn;

	// 建立坐标系
	TCrdPrm crdPrm;


	memset(&crdPrm,0,sizeof(crdPrm));
	crdPrm.dimension = 4;                        // 建立四维的坐标系
	crdPrm.synVelMax = SYNVELMAX;                // 坐标系的最大合成速度是: SYNVELMAX pulse/ms
	crdPrm.synAccMax = SYNACCMAX;                // 坐标系的最大合成加速度是: SYNACCMAX pulse/ms^2
	crdPrm.evenTime = EVENTIME;                 // 坐标系的最小匀速时间为EVENTIME ms
	crdPrm.profile[axis1-1] = 1;                 // 规划器1对应到X轴                       
	crdPrm.profile[axis2-1] = 2;                 // 规划器2对应到Y轴
	crdPrm.profile[axis3-1] = 3;                 // 规划器3对应到Z轴  
	crdPrm.profile[axis4-1] = 4;                 // 规划器4对应到A轴  
	crdPrm.setOriginFlag = 1;                    // 需要设置加工坐标系原点位置
	crdPrm.originPos[axis1-1] = 0;               // 加工坐标系原点位置在(0,0,0)，即与机床坐标系原点重合
	crdPrm.originPos[axis2-1] = 0;
	crdPrm.originPos[axis3-1] = 0;
	crdPrm.originPos[axis4-1] = 0;


	sRtn = GT_SetCrdPrm(cardNum,coordinate,&crdPrm);
	if (sRtn)
	{   
		g_ErrorMessage.Format("板卡%d轴坐标系%d的GT_SetCrdPrm指令执行异常！",cardNum+1,coordinate);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	// 即将把数据存入坐标系1的FIFO0中，所以要首先清除此缓存区中的数据
	m_CriticalCard.Lock();
	sRtn = GT_CrdClear(cardNum,coordinate,buffer);
	m_CriticalCard.Unlock();

	if (sRtn)
	{   
		g_ErrorMessage.Format("板卡%d坐标系%d的缓存区%d的GT_CrdClear指令执行异常！",cardNum+1,coordinate,buffer);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 2;
	}

	//没有错误
	return 0;

}
//四轴直线插补写入数据(acc加速度，speed目标速度，end_speed终点速度，axis1_data轴1数据，axis2_data轴2数据，axis3_data轴2数据,axis4_data轴4数据,coordinate坐标系，buffer缓冲区)
GTSMOTION_API UCT_4AxisLMotionData(short cardNum,double acc,double speed, double end_speed,long axis1_data,long axis2_data,long axis3_data,long axis4_data,short coordinate, short buffer)
{
	// 指令返回值变量
	short  sRtn;

	// 坐标系的缓存区剩余空间查询变量
	long space;


	// 查询坐标系1的FIFO0所剩余的空间
	sRtn = GT_CrdSpace(cardNum,coordinate, &space, 0);
	if (sRtn)
	{   
		g_ErrorMessage.Format("板卡%d坐标系%d的GT_CrdSpace指令执行异常！",cardNum+1,coordinate);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}

	if (space==0)
	{   
		g_ErrorMessage.Format("板卡%d坐标系%d的缓存区%d剩余空间为零，数据写入失败！",cardNum+1,coordinate,buffer);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 2;
	}


	// 向缓存区写入第一段插补数据（高速段）
	sRtn = GT_LnXYZA( 
		cardNum,                                                   //卡号cardNum
		coordinate,				                                  // 该插补段的坐标系是坐标系coordinate
		axis1_data, axis2_data, axis3_data,axis4_data,    		  // 该插补段的终点坐标: pulse
		speed,			                                          // 该插补段的目标速度：pulse/ms
		acc,			                                          // 插补段的加速度：pulse/ms^2
		end_speed,				                                 // 终点速度为end_speed
		buffer);				                                 // 向坐标系1的FIFO0缓存区传递该直线插补数据

	if (sRtn)
	{   
		g_ErrorMessage.Format("板卡%d坐标系%d的GT_LnXYZA指令执行异常！",cardNum+1,coordinate);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 3;
	}

	//没有错误
	return 0;

}

//二维位置比较功能初始化
GTSMOTION_API UCT_2DCompareInit(short cardNum, short chn,short axis1, short axis2)
{
	//GT_CompareData 设置位置比较输出数据

	short sRtn;
	T2DComparePrm Prm;


	//m_CriticalCard.Lock();
	sRtn =GT_2DCompareClear(cardNum,chn);
	if (sRtn)
	{   
		g_ErrorMessage.Format("板卡%d通道%d的GT_2DCompareClear指令执行异常！",cardNum+1,chn);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}



	sRtn =GT_2DCompareMode(cardNum,chn,COMPARE2D_MODE_2D);
	if (sRtn)
	{
		g_ErrorMessage.Format("板卡%d通道%d的GT_2DCompareMode指令执行异常！",cardNum+1,chn);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}


	//// 在 HSIO 通道 0 输出一个宽度为 5000 微秒的脉冲。
	//sRtn =GT_2DComparePulse(cardNum,chn,1,0,5000); // 脉冲宽度为 5000us	
	//if (sRtn)
	//{
	//	g_ErrorMessage.Format("板卡%d通道%d的GT_2DComparePulse指令执行异常！",cardNum+1,chn);
	//	::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
	//	return 1;
	//}

	//设置参数
	Prm.encx =axis1; // X 轴为轴
	Prm.ency =axis2; // Y 轴为轴
	Prm.maxerr =500; // 最大误差Pulse
	Prm.outputType =0; // 输出类型脉冲
	Prm.source =1; // 比较源编码器
	Prm.startLevel =0; // 无效参数
	Prm.threshold =10; // 最优点计算阈值
	Prm.time =1500; // 脉冲宽度 us
	sRtn =GT_2DCompareSetPrm(cardNum,chn,&Prm);
	if (sRtn)
	{
		g_ErrorMessage.Format("板卡%d通道%d的GT_2DCompareSetPrm指令执行异常！",cardNum+1,chn);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	//m_CriticalCard.Unlock();
	//没有错误
	return 0;

}

//二维位置比较功能写入数据(cardNum卡号，chn通道号，databuf二维数据,fifo缓冲区)
GTSMOTION_API UCT_2DCompareData(short cardNum,short chn,short count,T2DCompareData *databuf,short fifo)
{
	//m_CriticalCard.Lock();
	short sRtn;
	//位置比较输出通道 0,1 分别输出HSIO0 ,HSIO1 上
	sRtn =GT_SetComparePort(cardNum,COMPARE_PORT_HSIO,0,1); 
	if (sRtn)
	{
		g_ErrorMessage.Format("板卡%d的GT_SetComparePort指令执行异常！",cardNum+1);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	//清空二维位置比较输出数据。清空所有缓冲区剩余的位置数据
	sRtn =GT_2DCompareClear(cardNum,chn);
	if (sRtn)
	{
		g_ErrorMessage.Format("板卡%d通道%d的GT_2DCompareClear指令执行异常！",cardNum+1,chn);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}

	sRtn =GT_2DCompareData(cardNum,chn,
		count,
		databuf,
		fifo
		);

	if (sRtn)
	{
		g_ErrorMessage.Format("板卡%d通道%d的GT_2DCompareData指令执行异常！",cardNum+1,chn);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	//m_CriticalCard.Unlock();

	//没有错误
	return 0;

}

//启动二维位置比较功能
GTSMOTION_API UCT_2DCompareStart(short cardNum, short chn)
{
	//m_CriticalCard.Lock();
	short sRtn;
	sRtn =GT_2DCompareStart(cardNum,chn);
	if (sRtn)
	{
		g_ErrorMessage.Format("板卡%d通道%d的GT_2DCompareStart指令执行异常！",cardNum+1,chn);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	//m_CriticalCard.Unlock();
	//没有错误
	return 0;
}

//停止二维位置比较功能
GTSMOTION_API UCT_2DCompareStop(short cardNum, short chn)
{
	//m_CriticalCard.Lock();
	short sRtn;
	sRtn =GT_2DCompareStop(cardNum,chn);
	if (sRtn)
	{
		g_ErrorMessage.Format("板卡%d通道%d的GT_2DCompareStop指令执行异常！",cardNum+1,chn);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	//m_CriticalCard.Unlock();
	//没有错误
	return 0;
}
//打开扩展IO卡
GTSMOTION_API UCT_OpenExtCard(short cardNum,char *pDllName,short count)
{
	// 指令返回值变量
	short  sRtn;

	for(int i=0;i<count;i++)
	{
		m_CriticalCard.Lock();
		// 清除报警状态
		sRtn = GT_OpenExtMdl(cardNum,pDllName);

		if (sRtn)
		{   
			g_ErrorMessage.Format(_T("扩展IO板卡%d的GT_OpenExtMdl指令执行异常！"),cardNum+1+i);
			::MessageBox(NULL,g_ErrorMessage,"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
		m_CriticalCard.Unlock();
	}

	//没有错误
	return 0;
}
//加载扩展IO卡文件
GTSMOTION_API UCT_LoadExtConfig(short cardNum,char *pFileName,short count)
{
	// 指令返回值变量
	short  sRtn;

	for(int i=0;i<count;i++)
	{
		m_CriticalCard.Lock();
		// 清除报警状态
		sRtn = GT_LoadExtConfig(cardNum,pFileName);

		if (sRtn)
		{   
			g_ErrorMessage.Format(_T("扩展IO板卡%d的GT_LoadExtConfig指令执行异常！"),cardNum+1+i);
			::MessageBox(NULL,g_ErrorMessage,"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
		m_CriticalCard.Unlock();
	}

	//没有错误
	return 0;
}
//写输出
GTSMOTION_API UCT_WriteExtOutBit(short cardNum,short mdl,short bit,short value,short count)
{
	if (!g_bInitCardOK)
	{
//		AfxMessageBox(_T("运动控制卡未初始化！"));
		return 1;
	}
	// 指令返回值变量
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
	//	//写输出
	//	sRtn =GT_SetExtIoBit(cardNum,mdl,bit+i,m_te);
	//	g_ErrorMessage.Format("板卡%d输出%d的GT_SetExtIoBit指令！",cardNum+1,bit+i);

	//	if (sRtn)
	//	{   
	//		g_ErrorMessage.Format("板卡%d输出%d的GT_SetExtIoBit指令执行异常！",cardNum+1,bit+i);
	//		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
	//		return 1;
	//	}
	//	//m_CriticalCard.Unlock();
	//}
	sRtn =GT_SetExtIoBit(cardNum,mdl,bit,m_te);
	g_ErrorMessage.Format("板卡%d输出%d的GT_SetExtIoBit指令！",cardNum+1,bit);
	if (sRtn)
	{   
		g_ErrorMessage.Format("板卡%d输出%d的GT_SetExtIoBit指令执行异常！",cardNum+1,bit);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	//没有错误
	return 0;

}
//读输入
GTSMOTION_API UCT_ReadExtInBit(short cardNum,short mdl,short bit,BOOL *pValue)
{
	// 指令返回值变量
	short  sRtn;
	unsigned short usGpiValue;

	//m_CriticalCard.Lock();
	// 读取板卡通用信号输入值

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
		g_ErrorMessage.Format("板卡%d输入%d的GT_GetExtIoValue指令执行异常！",cardNum+1,bit);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}


	//m_CriticalCard.Unlock();
	//没有错误
	return 0;
}
//读输出
GTSMOTION_API UCT_ReadExtOutBit(short cardNum,short mdl,short bit,BOOL *pValue)
{
	// 指令返回值变量
	short  sRtn;

	unsigned short usGpiValue;

	//m_CriticalCard.Lock();
	// 读取板卡通用信号输出值
	sRtn=GT_GetExtDoValue(cardNum,mdl,&usGpiValue);

	if (sRtn)
	{   
		g_ErrorMessage.Format("板卡%d获取输出值%d的GT_GetExtDoValue指令执行异常！",cardNum+1,bit);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
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
	//没有错误
	return 0;
}


//Jog模式单轴运动到停止
GTSMOTION_API JCT_1AxisMoveDone(short cardNum,short axis,long LDis,double speed,double acc,double dec)
{
		
	if (!g_bInitCardOK)
	{
		//AfxMessageBox(_T("运动控制卡未初始化！"));
		return 1;
	}
	CString str;
	TJogPrm jog;
	long status;
	short  sRtn;
	double dPrfPos,dEncPos;                         //规划位置

	//马达平滑停止
	if (m_motor_stop)
	{
		//马达平滑停止
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}
	// 清除各轴的报警和限位
	GT_ClrSts(cardNum,axis);
	// 将AXIS轴设为Jog模式
	GT_PrfJog(cardNum,axis);

	// 读取Jog运动参数
	sRtn=GT_GetJogPrm(cardNum,axis,&jog);
// 	//初始速度  Jog模式没有初始速度
// 	trap.velStart = start_speed;
	//加速度
	jog.acc = acc;
	//减速度
	jog.dec = dec;

	// 设置AXIS轴的目标速度
	sRtn=GT_SetVel(cardNum,axis, speed);
//	//平滑时间     Jog模式没有平滑
//	trap.smoothTime = SMOOTHTIME;
	// 设置Jog运动参数
	sRtn=GT_SetJogPrm(cardNum,axis, &jog);

	//读取规划位置
	GT_GetPrfPos(cardNum,axis, &dPrfPos);


	// 启动AXIS轴的运动
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//消息循环
		UCT_PeekAndPump();
		//马达平滑停止
		if (m_motor_stop)
		{
			//马达平滑停止
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

		// 读取AXIS轴的状态
		GT_GetSts(cardNum,axis, &status);


	}while(status&0x400);	// 等待AXIS轴规划停止

	Sleep(400);
	
	return 0;
}
//Jog模式单轴开始运动
GTSMOTION_API JCT_1AxisStartMove(short cardNum,short axis,long LDis,double speed,double acc,double dec)
{
	if (!g_bInitCardOK)
	{
		//AfxMessageBox(_T("运动控制卡未初始化！"));
		return 1;
	}
	CString str;
	TJogPrm jog;
	short  sRtn;
	double dPrfPos;                         //规划位置


	//马达平滑停止
	if (m_motor_stop)
	{
		//马达平滑停止
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}
	// 清除各轴的报警和限位
	GT_ClrSts(cardNum,axis);
	// 将AXIS轴设为Jog模式
	GT_PrfJog(cardNum,axis);

	// 读取Jog运动参数
	sRtn=GT_GetJogPrm(cardNum,axis,&jog);
// 	//初始速度
// 	trap.velStart = start_speed;
	//加速度
	jog.acc = acc;
	//减速度
	jog.dec = dec;

	// 设置AXIS轴的目标速度
	sRtn=GT_SetVel(cardNum,axis, speed);
//	//平滑时间
//	trap.smoothTime = SMOOTHTIME;
	// 设置Jog运动参数
	sRtn=GT_SetJogPrm(cardNum,axis, &jog);

	//读取规划位置
	GT_GetPrfPos(cardNum,axis, &dPrfPos);

	// 启动AXIS轴的运动
	GT_Update(cardNum,1<<(axis-1));

	return 0;
}


//一维位置比较功能写入数据(cardNum卡号，chn通道号，databuf二维数据,fifo缓冲区)
GTSMOTION_API UCT_CompareData(short cardNum,short encoder,short source,short pulseType,short startLevel,short time,long *pBuf,short count)
{

	short sRtn;
	sRtn = GT_CompareStop(cardNum);
	if (sRtn)
	{
		g_ErrorMessage.Format("板卡%d的GT_CompareStop指令执行异常！",cardNum+1);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}

	sRtn =GT_CompareData(cardNum,encoder,source,pulseType,startLevel,time,pBuf,count,pBuf,0);

	if (sRtn)
	{
		g_ErrorMessage.Format("板卡%d的GT_CompareData指令执行异常！",cardNum+1);
		::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
		return 1;
	}
	//没有错误
	return 0;
}

//负向会原点
GTSMOTION_API UCT_BackToHomeNegORIGIN(short cardNum,short axis,long search_pos,long search_home,long home_offset,double home_speed, short home_capture,double acc_speed, double dec_speed)
{
	//m_CriticalCard.Lock();
	//马达平滑停止
	if (!g_bInitCardOK)
	{
		//AfxMessageBox(_T("运动控制卡未初始化！"));
		return 1;
	}
	if (m_motor_stop)
	{
		//马达平滑停止
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}

	short capture;
	short sRtn;
	TTrapPrm trapPrm;
	long status,pos;
	double prfPos,dEncPos;

	// 清除轴的报警和限位
	GT_ClrSts(cardNum,axis);

	// 实际位置清零，并进行零漂补偿
	GT_ZeroPos(cardNum,axis);

	// 切换到点位运动模式
	GT_PrfTrap(cardNum,axis);

	// 读取点位模式运动参数
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
	// 设置点位模式运动参数
	GT_SetTrapPrm(cardNum,axis, &trapPrm);

	BOOL bValue;
	short sRst;
	sRst = UCT_GetHomeValue(cardNum,axis,&bValue);
	if (sRst!=0)
		return 1;


	if(!bValue)
	{
		// 启动Home捕获
		GT_SetCaptureMode(cardNum,axis,CAPTURE_HOME);
		if(home_capture == 0)
			GT_SetCaptureSense(cardNum,axis,CAPTURE_HOME,0);
		else
			GT_SetCaptureSense(cardNum,axis,CAPTURE_HOME,1);
		// 设置点位模式目标速度，即回原点速度
		GT_SetVel(cardNum,axis, home_speed/2);
		////读取编码器位置
		//GT_GetEncPos(cardNum,axis, &dEncPos);
		//读取规划器位置
		GT_GetPrfPos(cardNum,axis, &prfPos);
		//  设置点位模式目标位置，即原点搜索距离
		GT_SetPos(cardNum,axis, long(prfPos)-search_home);

		//马达平滑停止
		if (m_motor_stop)
		{
			//马达平滑停止
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

		// 启动运动
		GT_Update(cardNum,1<<(axis-1));

		do
		{
			//消息循环
			UCT_PeekAndPump();
			// 读取轴状态
			GT_GetSts(cardNum,axis, &status);

			// 读取捕获状态
			sRtn = GT_GetCaptureStatus(cardNum,axis, &capture, &pos);
			//马达平滑停止
			if (m_motor_stop)
			{
				//马达平滑停止
				GT_Stop(cardNum,1<<(axis-1),IM_STOP);
				return 1;
			}

			// 如果运动停止，返回出错信息
			if( 0 == ( status & 0x400 ) )
			{
				m_motor_stop=1;
				g_ErrorMessage.Format("回原点中，板卡%d输出%d马达找不到原点或是马达异常停止！",cardNum+1,axis);
				::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
				return 1;
			}


			// 等待捕获触发
		}while( 0 == capture );

		// 运动到"捕获位置+偏移量"
		GT_SetPos(cardNum,axis, pos - home_offset);

		// 在运动状态下更新目标位置
		GT_Update(cardNum,1<<(axis-1));

		do
		{
			//消息循环
			UCT_PeekAndPump();
			// 读取轴状态
			sRtn = GT_GetSts(cardNum,axis, &status);

			//马达平滑停止
			if (m_motor_stop)
			{
				//马达平滑停止
				GT_Stop(cardNum,1<<(axis-1),IM_STOP);
				return 1;
			}

			// 等待运动停止
		}while( status & 0x400 );


		// 延时一段时间，等待电机停稳
		Sleep(50);

	}

	//反方向脱离原点
	{
		// 启动Home捕获
		GT_SetCaptureMode(cardNum,axis,CAPTURE_HOME);

		if(home_capture == 0)
			GT_SetCaptureSense(cardNum,axis,CAPTURE_HOME,1);
		else
			GT_SetCaptureSense(cardNum,axis,CAPTURE_HOME,0);

		// 设置点位模式目标速度，即回原点速度
		GT_SetVel(cardNum,axis, home_speed/2);
		////读取编码器位置
		//GT_GetEncPos(cardNum,axis, &dEncPos);
		//读取规划器位置
		GT_GetPrfPos(cardNum,axis, &prfPos);
		//  设置点位模式目标位置，即原点搜索距离
		GT_SetPos(cardNum,axis, long(prfPos)+search_pos*8);

		//马达平滑停止
		if (m_motor_stop)
		{
			//马达平滑停止
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

		// 启动运动
		GT_Update(cardNum,1<<(axis-1));

		do
		{
			//消息循环
			UCT_PeekAndPump();
			// 读取轴状态
			GT_GetSts(cardNum,axis, &status);

			// 读取捕获状态
			sRtn = GT_GetCaptureStatus(cardNum,axis, &capture, &pos);
			//马达平滑停止
			if (m_motor_stop)
			{
				//马达平滑停止
				GT_Stop(cardNum,1<<(axis-1),IM_STOP);
				return 1;
			}

			// 如果运动停止，返回出错信息
			if( 0 == ( status & 0x400 ) )
			{
				m_motor_stop=1;
				g_ErrorMessage.Format("回原点中，板卡%d输出%d马达找不到原点或是马达异常停止！",cardNum+1,axis);
				::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
				return 1;
			}


			// 等待捕获触发
		}while( 0 == capture );

		// 运动到"捕获位置+偏移量"
		GT_SetPos(cardNum,axis, pos + search_pos);

		// 在运动状态下更新目标位置
		GT_Update(cardNum,1<<(axis-1));

		do
		{
			//消息循环
			UCT_PeekAndPump();
			// 读取轴状态
			sRtn = GT_GetSts(cardNum,axis, &status);

			//马达平滑停止
			if (m_motor_stop)
			{
				//马达平滑停止
				GT_Stop(cardNum,1<<(axis-1),IM_STOP);
				return 1;
			}

			// 等待运动停止
		}while( status & 0x400 );


		// 延时一段时间，等待电机停稳
		Sleep(50);
	}


	sRst = UCT_GetHomeValue(cardNum,axis,&bValue);
	if (sRst!=0)
		return 1;
	if(bValue)
	{
		return 1;
	}

	// 清除各轴的报警和限位
	GT_ClrSts(cardNum,axis);

	// 启动Home捕获
	GT_SetCaptureMode(cardNum,axis,CAPTURE_HOME);

	if(home_capture == 0)
		GT_SetCaptureSense(cardNum,axis,CAPTURE_HOME,0);
	else
		GT_SetCaptureSense(cardNum,axis,CAPTURE_HOME,1);

	// 设置点位模式目标速度，即回原点速度
	GT_SetVel(cardNum,axis, home_speed/15);
	////读取编码器位置
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	//读取规划器位置
	GT_GetPrfPos(cardNum,axis, &prfPos);
	//  设置点位模式目标位置，即原点搜索距离
	GT_SetPos(cardNum,axis, long(prfPos)-search_home);

	//马达平滑停止
	if (m_motor_stop)
	{
		//马达平滑停止
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}

	// 启动运动
	GT_Update(cardNum,1<<(axis-1));



	do
	{
		//消息循环
		UCT_PeekAndPump();
		// 读取轴状态
		GT_GetSts(cardNum,axis, &status);

		// 读取捕获状态
		sRtn = GT_GetCaptureStatus(cardNum,axis, &capture, &pos);

		//马达平滑停止
		if (m_motor_stop)
		{
			//马达平滑停止
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

		// 如果运动停止，返回出错信息
		if( 0 == ( status & 0x400 ) )
		{
			m_motor_stop=1;
			g_ErrorMessage.Format("回原点中，板卡%d输出%d马达找不到原点或是马达异常停止！",cardNum+1,axis);
			::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
			return 1;
		}
		// 等待捕获触发
	}while( 0 == capture );



	// 运动到"捕获位置+偏移量"
	GT_SetPos(cardNum,axis, pos - home_offset);

	//马达平滑停止
	if (m_motor_stop)
	{
		//马达平滑停止
		GT_Stop(cardNum,1<<(axis-1),IM_STOP);
		return 1;
	}
	// 在运动状态下更新目标位置
	GT_Update(cardNum,1<<(axis-1));

	do
	{
		//消息循环
		UCT_PeekAndPump();
		// 读取轴状态
		sRtn = GT_GetSts(cardNum,axis, &status);

		//马达平滑停止
		if (m_motor_stop)
		{
			//马达平滑停止
			GT_Stop(cardNum,1<<(axis-1),IM_STOP);
			return 1;
		}

		// 等待运动停止
	}while( status & 0x400 );

	// 延时一段时间，等待电机停稳
	Sleep(400);


	////读取编码器位置
	//GT_GetEncPos(cardNum,axis, &dEncPos);
	////读取规划器位置
	//GT_GetPrfPos(cardNum,axis, &prfPos);

	//if (abs(long(dEncPos-prfPos))>=DIF_PULSE)
	//{
	//	m_motor_stop=1;
	//	g_ErrorMessage.Format("回原点中，板卡%d输出%d马达目标位置和实际位置差异太大,差距%ld脉冲!",cardNum+1,axis,abs(long(dEncPos-prfPos)));
	//	::MessageBox(NULL,_T(g_ErrorMessage),"异常对话框",MB_SYSTEMMODAL|MB_ICONERROR);
	//	return 1;
	//}

	// 清除各轴的报警和限位
	GT_ClrSts(cardNum,axis);
	// 实际位置清零，并进行零漂补偿
	GT_ZeroPos(cardNum,axis);

	//m_CriticalCard.Unlock();
	return 0;

}

GTSMOTION_API UCT_Calc1AxisPTMoveDone(short sCardNum,short sAxis,double dbDis,double dbSpeed,double dbAcc, double dbDecDis,double dbDec,bool bRelative)
{
	double dbPrfPos;
	GT_GetPrfPos(sCardNum,sAxis, &dbPrfPos);
	//终止位置
	double dbDesPos;
	if (bRelative==true)
	{
		dbDesPos = dbPrfPos+dbDis;
	} 
	else
	{
		if((dbDis-dbPrfPos)<dbDecDis)//减距离超过运动距离
			return 1;
		dbDesPos = dbDis;
	}
	//降低减速的位置 以及最后时间
	double dbPos[4];
	long lTime[4];

	
	double dbHighDis = dbSpeed*dbSpeed/dbAcc/2;//0增加到最高速的距离
	double dbHighTime = dbSpeed/dbAcc;//0增加到最高速的时间
	//实际距离与时间
	int nHighTime = dbHighTime;   //第一段时间
	dbHighDis =  dbHighTime*dbHighTime*dbAcc/2;//第一段距离
	double dbRealSpeed = dbAcc*dbHighTime;

	double dbDecSpeed = sqrt(2.0*dbDec*dbDecDis);//开始减速停的速度
	double dbDecTime = sqrt(dbDecDis*2.0/dbDec);//减速停时间
	int nDecTime = dbDecTime+1;   //最后减速时间
	dbDecSpeed =  nDecTime*dbDec; //最后减速速度
	double dbRealDecDis = nDecTime*nDecTime*dbDec/2;//最后减速距离


	double dbHighDecDis =(dbRealSpeed*dbRealSpeed-dbDecSpeed*dbDecSpeed)/dbAcc/2;//最高速到变减速段距离	
	double dbHighDecTime = (dbRealSpeed-dbDecSpeed)/dbAcc;//最高速到变减速时间
	int nHighDecTime = dbHighDecTime+1;   //最高速到变减速时间
	double dbTempAcc = (dbRealSpeed-dbDecSpeed)/nHighDecTime;
	double dbRealHighDecDis = (dbRealSpeed*dbRealSpeed-dbDecSpeed*dbDecSpeed)/dbTempAcc/2;;//最高速到变减速段距离

	
	if((dbHighDis+dbRealHighDecDis+dbRealDecDis)>=(dbDesPos-dbPrfPos))//未达到最高速
	{
		double dbVHigh = sqrt((2.0*(dbDesPos-dbPrfPos-dbRealDecDis)*dbAcc+dbDecSpeed*dbDecSpeed)/2);
		dbPos[0] = dbVHigh*dbVHigh/2/dbAcc+dbPrfPos;
		lTime[0] = dbVHigh/dbAcc+1;//时间变长   终点速度变小
		//3333 26
		//4897 20
		//5600  53
		dbVHigh = (dbPos[0]-dbPrfPos)/lTime[0]*2;//实际终点速度
		dbPos[1] = dbDesPos-dbRealDecDis;
		lTime[1] = 2*(dbPos[1]-dbPos[0])/(dbVHigh+dbDecSpeed);//时间变短   终点速度变大
		dbPos[2] = dbDesPos;//开始减速位置
		lTime[2] = dbDecTime+1;
		dbPos[0]-=dbPrfPos;
		dbPos[1]-=dbPrfPos;
		dbPos[2]-=dbPrfPos;
		return UCT_1AxisPTMoveDone(sCardNum,sAxis,dbPos,lTime,3);
	}
	else//达到最高速
	{
		 dbPos[0] = dbHighDis+dbPrfPos;
		 lTime[0] = nHighTime;
		 dbPos[1] = dbDesPos-dbRealDecDis-dbHighDecDis;
		 lTime[1] = (dbDesPos-dbRealDecDis-dbHighDecDis-dbHighDis)/dbSpeed+1;
		 dbPos[2] = dbDesPos-dbRealDecDis;//开始减速位置
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
	//马达平滑停止
	if (m_motor_stop)
	{
		//马达平滑停止
		GT_Stop(sCardNum,1<<(sAxis-1),IM_STOP);
		return 1;
	}
	if(sDataNum<0)
		return 4;
	// 清除各轴的报警和限位
	GT_ClrSts(sCardNum,sAxis);
	// 将AXIS轴设为PT模式
	GT_PrfPt(sCardNum,sAxis);
	// 清楚AXIS轴的FIFO
	GT_PtClear(sCardNum,sAxis);
	
	short space;
	int ii = 0;
	double dbTempPos =0 ;
	long lTempTime = 0; 
	for (ii =0; ii <sDataNum; ++ii)
	{
		// 查询PT模式FIFO的剩余空间
		sRtn =  GT_PtSpace (sCardNum,sAxis, &space);
		// 向FIFO中增加运动数据
		dbTempPos = dbDis[ii];
		lTempTime += lTime[ii]; 
		if(ii == sDataNum-1)
			sRtn = GT_PtData(sCardNum,sAxis, dbTempPos, lTempTime,PT_SEGMENT_STOP);//pluse和ms
		else
			sRtn = GT_PtData(sCardNum,sAxis, dbTempPos, lTempTime);//pluse和ms
	}
	
	//启动PT运动
	sRtn = GT_PtStart (sCardNum,1<<(sAxis-1));

	do
	{
		//消息循环
		UCT_PeekAndPump();
		Sleep(5);
		// 读取AXIS轴的状态
		GT_GetSts(sCardNum,sAxis, &status);
		//马达平滑停止
		if (m_motor_stop)
		{
			//马达平滑停止
			GT_Stop(sCardNum,1<<(sAxis-1),IM_STOP);
			return 1;
		}
	}while(status&0x400);	// 等待AXIS轴规划停止


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