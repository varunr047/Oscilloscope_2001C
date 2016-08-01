/********************************************************************************
 * Note:
 * Measuretypes (i.e, itype in fnMeasure) 0 to 16 values are "freq\n","PERIod\n",
 * "MEAN\n","PK2pk\n","CRMs\n","RMS\n","MINImum\n","MAXimum\n","cursorrms\n",
 * "rise\n","fall\n","pwidth\n","nwidth\n","pduty\n","phase\n",
 * "delay\n","none\n" Respectively stored.
 *
 * uiChannel values are 1 and 2 channel numbers ch1 and ch2 respectively.
 *
 * iOnOffValue 0 is OFF and 1 is ON .
 *
 * uiMesureWind is measure window numbers 1,2,3,4,5.
 *
 * iRunStopValue 0 is Stop and 1 is Start.
 *
 * OsCapture.bmp Screen shot is stored in default folder.
 *
 * TriggerEdge Coupling 1,2,3,4,5 values are respectively as ac,dc,noiserej,hfrej,lfrej
 *
 * TriggerEdge Slope 0 is falling and 1 is rising
 *
 * TriggerEdge mode 0 is Normal and 1 is AUTO
 *
 *
 * *****************************************************************************/

#ifndef CTEK2001C_H
#define CTEK2001C_H
#define XSIZEOFREADBUFFER 65536
#define XSIZEOFIMAGE 7786
#define XSIZEOFSETATTRI 10000
#define XTYPESIZE 17
#define XSIZEOFBUFFER  256
#define XSIZEOFCMD 256
#define XSETATTRVALUE 28598304

#include "visa.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <visa.h>
#include <string>
#include <windows.h>
#include<QLabel>
#include<QString>
#include <QGraphicsPixmapItem>
#include <QMessageBox>

class CTek2001c
{
public:
    CTek2001c();
    ~CTek2001c();
private:

    ViStatus iStatus;

    ViSession ObjDRM = VI_NULL;         //Default Resource Manager Session
    ViSession ObjVi = VI_NULL;          //Unique logical identifier Session
    ViSession ObjDRM_1 = VI_NULL;

    ViChar	szBuffer[XSIZEOFREADBUFFER];
    ViChar  szBufferMes[XSIZEOFBUFFER];
    ViChar  szBufferImage[XSIZEOFIMAGE];
    ViChar  szBufferVoltage[XSIZEOFBUFFER];
    ViChar  szBufferFreq[XSIZEOFBUFFER];
    ViChar  szCmd1[XSIZEOFBUFFER];
    ViChar  szCmd0[XSIZEOFBUFFER];
    ViChar  szCmd2[XSIZEOFBUFFER];
    ViChar  szOnOff[XSIZEOFBUFFER];
    ViChar  szCmdToWrite[XSIZEOFCMD];


    ViAttrState uiStat = XSIZEOFSETATTRI;
    ViAttrState uiGetAttri1 = VI_NULL;
    ViAttrState uiGetAttri2 = VI_NULL;
    ViAttrState uiStat1 = XSETATTRVALUE;
    ViAttrState uiGetAttri3 = VI_NULL;

    ViUInt32 uiRectOnOff = VI_NULL;
    ViUInt32 uiRectOn = VI_NULL;
    ViUInt32 uiRectStop=VI_NULL;
    ViUInt32 uiRectRunStop=VI_NULL;
    ViUInt32 uiRectRun=VI_NULL;
    ViUInt32 uiRectMearVolt=VI_NULL;
    ViUInt32 uiRectBuff=VI_NULL;
    ViUInt32 uiRec=VI_NULL;
    ViUInt32 uiRetCnt=VI_NULL;
    ViUInt32 uiRecHeader=VI_NULL;
    ViUInt32 uiRecHeaderFmt=VI_NULL;
    ViUInt32 uiRetCntjpg = VI_NULL;
    ViUInt32 uiRecHardLy=VI_NULL;
    ViUInt32 uiRectHardLy2= VI_NULL;
    ViUInt32 uiRectUsbEnd = VI_NULL;
    ViUInt32 uiRectFormEnd = VI_NULL;
    ViUInt32 uiRectEnd=VI_NULL;
    ViUInt32 uiRectSingle=VI_NULL;
    ViUInt32 uiRectAutoset=VI_NULL;
    ViUInt32 uiRectMearFeq=VI_NULL;
    ViUInt32 uiRectMenuType=VI_NULL;
    ViUInt32 uiRectEdgeTrigger=VI_NULL;
    ViUInt32 uiRectVideoTrigger=VI_NULL;
    ViUInt32 uiRectPulseTrigger=VI_NULL;
    ViUInt32 uiRectTrigForce=VI_NULL;
    ViUInt32 uiRectTrigLevel=VI_NULL;
    ViUInt32 uiRectBuffMes=VI_NULL;
    ViUInt32 uiRectValue=VI_NULL;

    QString Error;

public:
    QString fnConnect(ViRsrc szDeviceName);
    QString fnCaptureImage();
    QString fnFrequency(unsigned int uiChannel);
    QString fnVoltage(unsigned int uiChannel);
    QString fnEndConnect();
    QString fnAutoset();
    QString fnSingle();
    QString fnRunStop(QString szRunStopValue);
    QString fnStop();
    QString fnChannel(unsigned int uiChannel,QString iOnOffValue);
    QString fnMeasure(unsigned int uiMesureWind,unsigned int uiChannel,unsigned int uitype);
    QString fnTriggerSetTo50();
    QString fnTriggerMenu(unsigned int uiType);
    QString fnSetEdgeTrigger(unsigned int uiChannel,bool bSlope,bool bMode,unsigned int uiCoupling);
    QString fnSetVideoTrigger(unsigned int uiChannel,bool bPolarity,unsigned int uiSync,bool bStandard);
    QString fnSetPulseTrigger(unsigned int uiChannel,unsigned int uiWhen,bool bPolarity,bool bMode,unsigned int uiCoupling);
    QString fnTriggerForce();
    QString fnTriggerLevel(float fTriglevel);
    QString fnError();
};

#endif // CTEK2001C_H
