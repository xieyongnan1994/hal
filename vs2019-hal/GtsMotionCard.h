#pragma once
#include "gts.h"


//�Զ���Ĺ̸߿��ƿ�ȫ�ֱ����ͺ���20160320
#define GTSMOTION_API extern "C" short __stdcall

#define CARD_NO1                        0          //����
#define CARD_NO2                        1
#define CARD_NO3                        2
#define CARD_NO4                        3

#define AXIS_NO1                        1          //���
#define AXIS_NO2                        2
#define AXIS_NO3                        3
#define AXIS_NO4                        4
#define AXIS_NO5                        5
#define AXIS_NO6                        6
#define AXIS_NO7                        7
#define AXIS_NO8                        8

#define IM_STOP                         0          //ƽ��ֹͣ
#define EM_STOP                         1          //����ֹͣ

#define DIF_PULSE                       1000         //�滮�����������ֵ
#define HOME_OFFSET                     300        //ԭ��ƫ����

#define SMOOTHTIME                      20         //����ƽ��ʱ��(ms)

#define SYNVELMAX                       10000      //ֱ�߲岹���ϳ��ٶ�(pulse/ms)
#define SYNACCMAX                       100        //ֱ�߲岹���ϳɼ��ٶ�(pulse/ms^2)
#define EVENTIME                        40         //ֱ�߲岹��С����ʱ��(ms)

//�˶�ֹͣ��־λ
extern volatile BOOL m_motor_stop;
extern CCriticalSection m_CriticalCard;
//�̸߿��쳣��־λ
extern BOOL g_MotionError;
//����Ĵ�����Ϣ
extern CString g_ErrorMessage;
//�����һ����Ϣ
extern CString g_GeneralMessage;

extern DWORD g_AlarmBeginTime;
extern BOOL  g_AlarmHappen;

//�忨��ʼ���ɹ���־
extern volatile BOOL g_bInitCardOK;
//��Ϣѭ��
GTSMOTION_API UCT_PeekAndPump();

//���˶����ƿ�
GTSMOTION_API UCT_OpenMotionCard(short cardNum,short count=1);
//�ر��˶����ƿ�
GTSMOTION_API UCT_CloseMotionCard(short cardNum,short count=1);
//��λ�˶����ƿ�
GTSMOTION_API UCT_ResetMotionCard(short cardNum,short count=1);
//���������ļ�
GTSMOTION_API UCT_LoadConfig(short cardNum,char *pFile);
//�������״̬
GTSMOTION_API UCT_ClrSts(short cardNum,short axis,short count=1);

//�ŷ�On
GTSMOTION_API UCT_AxisOn(short cardNum,short axis,short count=1);
//�ŷ�Off
GTSMOTION_API UCT_AxisOff(short cardNum,short axis,short count=1);

//д���
GTSMOTION_API UCT_WriteOutBit(short cardNum,short bit,short value,short count=1);
//д�������
GTSMOTION_API UCT_WriteTimeOutBit(short cardNum,short bit,short value,short reverseTime);
//������
GTSMOTION_API UCT_ReadInBit(short cardNum,short bit,BOOL *pValue);
//�����
GTSMOTION_API UCT_ReadOutBit(short cardNum,short bit,BOOL *pValue);

//ƽ��ֹͣ
GTSMOTION_API UCT_IMStopMove(short cardNum,short axis,short count=1);
//����ֹͣ
GTSMOTION_API UCT_EMStopMove(short cardNum,short axis,short count=1);

//��ȡ��״̬
GTSMOTION_API UCT_GetAxisSts(short cardNum,short axis,long *pValue);
//��ȡ���ŷ�����
GTSMOTION_API UCT_GetServerAlarm(short cardNum,short axis,BOOL *pValue);
//��ȡ������λ����
GTSMOTION_API UCT_GetPosLimitAlarm(short cardNum,short axis,BOOL *pValue);
//��ȡ�Ḻ��λ����
GTSMOTION_API UCT_GetNegLimitAlarm(short cardNum,short axis,BOOL *pValue);
//��ȡ����λֵ
GTSMOTION_API UCT_GetPosLimitValue(short cardNum,short axis,BOOL *pValue);
//��ȡ����λֵ
GTSMOTION_API UCT_GetNegLimitValue(short cardNum,short axis,BOOL *pValue);
//��ȡԭ��ֵ
GTSMOTION_API UCT_GetHomeValue(short cardNum,short axis,BOOL *pValue);
//��ȡ�滮����ֵ
GTSMOTION_API UCT_GetPrfPos(short cardNum,short axis,double *pValue);
//��ȡ��������ֵ
GTSMOTION_API UCT_GetEncPos(short cardNum,short axis,double *pValue);

//�����ԭ�㣨��ԭ���Ӧ����
GTSMOTION_API UCT_BackToHomeORIGIN(short cardNum,short axis,long search_pos,long search_home,long home_offset,double home_speed,short sHomeCapture = 0);
//�����ԭ�㣨��ԭ���Ӧ����
GTSMOTION_API UCT_BackToHomeINDEX(short cardNum,short axis,long search_pos,long search_home,long home_offset,double home_speed);
//�����ԭ�㣨����λ��Ϊԭ�㣩
GTSMOTION_API UCT_BackToHomePOSITIVE(short cardNum,short axis,long search_pos,long search_home,long home_offset,double home_speed);
//�����ԭ�㣨����λ��Ϊԭ�㣩
GTSMOTION_API UCT_BackToHomeNEGATIVE(short cardNum,short axis,long search_pos,long search_home,long home_offset,double home_speed);

//�����˶���ֹͣ��bRelative=true����˶���bRelative=false�����˶���start_speed����ٶȣ�speedĿ���ٶȣ�
GTSMOTION_API UCT_1AxisMoveDone(short cardNum,short axis,long LDis,double start_speed,double speed,double acc,double dec,bool bRelative=false);
//���Ὺʼ�˶���bRelative=true����˶���bRelative=false�����˶���start_speed����ٶȣ�speedĿ���ٶȣ�
GTSMOTION_API UCT_1AxisStartMove(short cardNum,short axis,long LDis,double start_speed,double speed,double acc,double dec,bool bRelative=false);
//�����˶�״̬(pValue=0ֹͣ pValue=1�˶���)
GTSMOTION_API UCT_1AxisCheckMoveSts(short cardNum,short axis,long *pValue);
//��鵥���˶��Ƿ�λ
GTSMOTION_API UCT_1AxisCheckMoveDone(short cardNum,short axis,long *pValue);

//����ϵ��Ӧ��ֱ�߲岹��ʼ�˶�
GTSMOTION_API UCT_LMotionStart(short cardNum, short coordinate=1, short buffer=0);
//�岹�˶�����ϵ�˶�״̬��pRun=1�˶��У�pSegment��ǰ�Ѿ���ɵĲ岹������
GTSMOTION_API UCT_CheckLMotionSts(short cardNum,short *pRun,long *pSegment, short coordinate=1, short buffer=0);
//���������ϵ�������е�����
GTSMOTION_API UCT_CrdClear(short cardNum, short coordinate=1, short buffer=0);
// ��������ʱָ��
GTSMOTION_API UCT_BufDelay(short cardNum, short delay,short coordinate=1, short buffer=0);

//����ֱ�߲岹��ʼ��
GTSMOTION_API UCT_2AxisLMotionInit(short cardNum, short axis1, short axis2, short coordinate=1, short buffer=0);
//����ֱ�߲岹д������(acc���ٶȣ�speedĿ���ٶȣ�end_speed�յ��ٶȣ�axis1_data��1���ݣ�axis2_data��2����,coordinate����ϵ��buffer������)
GTSMOTION_API UCT_2AxisLMotionData(short cardNum,double acc,double speed, double end_speed,long axis1_data,long axis2_data, short coordinate=1, short buffer=0);

//����ֱ�߲岹��ʼ��
GTSMOTION_API UCT_3AxisLMotionInit(short cardNum, short axis1, short axis2, short axis3,short coordinate=1, short buffer=0);
//����ֱ�߲岹д������(acc���ٶȣ�speedĿ���ٶȣ�end_speed�յ��ٶȣ�axis1_data��1���ݣ�axis2_data��2���ݣ�axis3_data��3����,coordinate����ϵ��buffer������)
GTSMOTION_API UCT_3AxisLMotionData(short cardNum,double acc,double speed, double end_speed,long axis1_data,long axis2_data,long axis3_data, short coordinate=1, short buffer=0);

//����ֱ�߲岹��ʼ��
GTSMOTION_API UCT_4AxisLMotionInit(short cardNum, short axis1, short axis2, short axis3, short axis4,short coordinate=1, short buffer=0);
//����ֱ�߲岹д������(acc���ٶȣ�speedĿ���ٶȣ�end_speed�յ��ٶȣ�axis1_data��1���ݣ�axis2_data��2���ݣ�axis3_data��2����,axis4_data��4����,coordinate����ϵ��buffer������)
GTSMOTION_API UCT_4AxisLMotionData(short cardNum,double acc,double speed, double end_speed,long axis1_data,long axis2_data,long axis3_data,long axis4_data,short coordinate=1, short buffer=0);


//��άλ�ñȽϹ��ܳ�ʼ��
GTSMOTION_API UCT_2DCompareInit(short cardNum, short chn,short axis1, short axis2);
//��άλ�ñȽϹ���д������(cardNum���ţ�chnͨ���ţ�databuf��ά����,fifo������)
GTSMOTION_API UCT_2DCompareData(short cardNum,short chn,short count,T2DCompareData *databuf,short fifo=0);
//������άλ�ñȽϹ���
GTSMOTION_API UCT_2DCompareStart(short cardNum, short chn);
//ֹͣ��άλ�ñȽϹ���
GTSMOTION_API UCT_2DCompareStop(short cardNum, short chn);

//����չIO��
GTSMOTION_API UCT_OpenExtCard(short cardNum,char *pDllName=NULL,short count=1);
//������չIO���ļ�
GTSMOTION_API UCT_LoadExtConfig(short cardNum,char *pFileName=NULL,short count=1);
//д���
GTSMOTION_API UCT_WriteExtOutBit(short cardNum,short mdl,short bit,short value,short count=1);
//������
GTSMOTION_API UCT_ReadExtInBit(short cardNum,short mdl,short bit,BOOL *pValue);
//�����
GTSMOTION_API UCT_ReadExtOutBit(short cardNum,short mdl,short bit,BOOL *pValue);


//Jogģʽ�����˶���ֹͣ
GTSMOTION_API JCT_1AxisMoveDone(short cardNum,short axis,long LDis,double speed,double acc,double dec);
//Jogģʽ���Ὺʼ�˶�
GTSMOTION_API JCT_1AxisStartMove(short cardNum,short axis,long LDis,double speed,double acc,double dec);


GTSMOTION_API UCT_CompareData(short cardNum,short encoder,short source,short pulseType,short startLevel,short time,long *pBuf,short count);


GTSMOTION_API UCT_BackToHomeNegORIGIN(short cardNum,short axis,long search_pos,long search_home,long home_offset,double home_speed, short home_capture =0,double acc_speed = 0.5, double dec_speed = 0.5);


GTSMOTION_API UCT_Calc1AxisPTMoveDone(short sCardNum,short sAxis,double dbDis,double dbSpeed,double dbAcc, double dbDecDis,double dbDec,bool bRelative=false);

GTSMOTION_API UCT_1AxisPTMoveDone(short sCardNum,short sAxis,double dbDis[],long lTime[],short sDataNum);