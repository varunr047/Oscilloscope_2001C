#include "tek2001c.h"

#define XSUCCESS   "1"
#define XFAIL      "0"

const ViChar  szTypeValue[XTYPESIZE][XTYPESIZE]={"freq\n","PERIod\n","MEAN\n","PK2pk\n","CRMs\n","RMS\n","MINImum\n","MAXimum\n","cursorrms\n","rise\n","fall\n","pwidth\n","nwidth\n","pduty\n","phase\n","delay\n","none\n"};
/********************************************************************************
 * Function Name:CTek2001c
 * Pupose:Constructor
 * Input: None
 * Output: None
 * Author :Varun
 * Date of Creation: 7/8/2015
 * Modification Date:
 * Modified By:
 * *******************************************************************************/
CTek2001c::CTek2001c()
{

}

/********************************************************************************
 * Function Name:fnConnect
 * Pupose:Create Connection btw System and Oscilloscope
 * Input: Device name
 * Output: SUCESS / FAIL
 * Author :Varun
 * Date of Creation: 7/8/2015
 * Modification Date:
 * Modified By:
 * *******************************************************************************/
QString CTek2001c::fnConnect(ViRsrc szDeviceName)
{

    iStatus = viOpenDefaultRM(&ObjDRM);                                    // Open the device until it get connected

   /*******************************SERIES OF COMMAND TO CONNECT OSCILLOSCOPE**************************************/

        iStatus = viOpen(ObjDRM,szDeviceName , VI_NULL, VI_NULL, &ObjVi);   //Open a session to the specified resource,szDeviceName (RESOURCE NAME)
        if(iStatus<0)
        {
           Error = fnError();
           return Error;
        }

        iStatus =  viGetAttribute(ObjVi, VI_ATTR_TMO_VALUE, &uiGetAttri1);  //Retrieves the state of an attribute for the specified session
        if(iStatus<0)
        {
            Error = fnError();
            return Error;
        }

        iStatus =  viSetAttribute(ObjVi, VI_ATTR_TMO_VALUE,uiStat );        //Sets the state of an attribute for the specified session
        if(iStatus<0)
        {
            Error = fnError();
            return Error;
        }

        iStatus = viGetAttribute(ObjVi, VI_ATTR_INTF_TYPE, &uiGetAttri2);
        if(iStatus<0)
        {
            Error = fnError();
            return Error;
        }

      return (QString)XSUCCESS;


}

/********************************************************************************
 * Function Name:fnEndConnect
 * Pupose:Ends the Connection btw Computer and Oscilloscope
 * Input: None
 * Output: SUCESS / FAIL
 * Author :Varun
 * Date of Creation: 7/8/2015
 * Modification Date:
 * Modified By:
 * *******************************************************************************/

QString CTek2001c::fnEndConnect()
{
    iStatus = viClose(ObjDRM);  //Closes the specified session
    if(iStatus < 0)
    {
        Error = fnError();
        return Error;
    }
    return XSUCCESS;

}

/********************************************************************************
 * Function Name:fnMeasure
 * Pupose:Measures the type for particular channel and Dispplay
 * Input: MeasureWindowNumber,Source Channel.no,Type of measurment
 * Output:Display Measure on Oscilloscope ,value
 * Author :Varun
 * Date of Creation: 7/8/2015
 * Modification Date:
 * Modified By:
 * *******************************************************************************/
QString CTek2001c::fnMeasure(unsigned int uiMesureWind,unsigned int uiChannel,unsigned int uitype)
{

        ViChar cCmd[] = "measurement:meas";
        ViChar cSur[] = ":source ch";
        ViChar cEnd[] = "\n";
        ViChar cTyp[] = ":type ";
        ViChar cValu[] = ":value?\n";
        QString cChannelNo;
        cChannelNo = (QString)(48+uiChannel);
        QString MesureWindno;
        MesureWindno = (QString)(48+uiMesureWind);

        memset(szCmd1, 0, sizeof(szCmd1));
        strcpy(szCmd1,cCmd);
        strcat(szCmd1,MesureWindno.toLatin1());
        strcpy(szCmd0,szCmd1);
        strcat(szCmd0,cSur);
        memset(szCmd2, 0, sizeof(szCmd2));
        strcat(szCmd2,szCmd1);
        strcat(szCmd2,cValu);
        strcat(szCmd1,cTyp);
        strcat(szCmd1,szTypeValue[uitype]);
        strcat(szCmd0,cChannelNo.toLatin1());
        strcat(szCmd0,cEnd);

    /*******************************************MEASURE WINDOW CHANNEL TYPE QUERY******************************************/
        iStatus = viWrite(ObjVi,(ViBuf)szCmd0,sizeof(szCmd0),&uiRectOnOff);  //Writes data synchronously to a device from the specified buffer.
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }

        iStatus = viWrite(ObjVi,(ViBuf)szCmd1,sizeof(szCmd1),&uiRectOn);
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }

        iStatus = viWrite(ObjVi,(ViBuf)szCmd2,25,&uiRectValue);
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }

  /*******************************************QUERY VALUE STORING IN BUFFER******************************************/
        memset(szBufferMes, 0, sizeof(szBufferMes));
        iStatus = viRead(ObjVi, (ViBuf)szBufferMes, sizeof(szBufferMes), &uiRectBuffMes);  //Reads data synchronously from a device into the specified buffer.
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }
        return (QString)szBufferMes;



}
/********************************************************************************
 * Function Name:fnChannel
 * Pupose:Set the Channel ON OFF
 * Input: Source Channel.no,Set on/off
 * Output:Display respective channel on/off on Oscilloscope,SUCESS / FAIL
 * Author :Varun
 * Date of Creation: 7/8/2015
 * Modification Date:
 * Modified By:
 * *******************************************************************************/
QString CTek2001c::fnChannel(unsigned int uiChannel,QString szOnOffValue)
{
    ViChar  cCmd[]="select:ch";
    QString cChannelNo;
    cChannelNo = (QString)(48+uiChannel);                               //ASCII
    memset(szCmd0, 0, sizeof(szCmd0));
    strcpy(szCmd0,cCmd);
    strcat(szCmd0,cChannelNo.toLatin1());                               //CONCATE CHANNEL NO
    strcat(szCmd0," ");

 /*******************************************OFF STATE******************************************/

    if((QString)XFAIL == szOnOffValue)
    {
        strcpy(szOnOff,"Off");
    }

 /*******************************************OFF STATE******************************************/

    else if((QString)XSUCCESS == szOnOffValue)
    {
        strcpy(szOnOff,"On");
    }
    strcat(szCmd0,szOnOff);
    iStatus = viWrite(ObjVi,(ViBuf) szCmd0,14,&uiRectOnOff);
    if(iStatus < 0)
    {
        Error = fnError();
        return Error;
    }
    return XSUCCESS;
}

/********************************************************************************
 * Function Name:fnChannel
 * Pupose:Functions RUN/STOP
 * Input:Set Run/stop
 * Output:Run/Stop waveform on Oscilloscope,SUCESS / FAIL
 * Author :Varun
 * Date of Creation: 7/8/2015
 * Modification Date:
 * Modified By:
 * *******************************************************************************/
QString CTek2001c::fnRunStop(QString szRunStopValue)
{
  /*******************************************RUN STATE******************************************/
    if(XSUCCESS == szRunStopValue)
    {

            /*************************STOP ACQUIRE AND THEN RUN *************************************/

                iStatus = viWrite(ObjVi,(ViBuf)"acquire:stopafter runstop" ,26,&uiRectRunStop);    //26 size of command
                if(iStatus < 0)
                {
                    Error = fnError();
                    return Error;
                }

                iStatus = viWrite(ObjVi,(ViBuf)"acquire:state run" ,18,&uiRectRun);     //18 size of command
                if(iStatus < 0)
                {
                    Error = fnError();
                    return Error;
                }

    }

 /*******************************************STOP STATE******************************************/

    else if(XFAIL == szRunStopValue)
    {


        iStatus = viWrite(ObjVi,(ViBuf) "acquire:state stop",19,&uiRectStop);           //19 size of command
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }
    }
    return XSUCCESS;
}

/********************************************************************************
 * Function Name:fnSingle
 * Pupose:Single button functionality
 * Input:None
 * Output:Display AcqComplete waveform,SUCESS / FAIL
 * Author :Varun
 * Date of Creation: 7/8/2015
 * Modification Date:
 * Modified By:
 * *******************************************************************************/
QString CTek2001c::fnSingle()
{
    /*************************SINGLE BUTTON FUNCTIONALITY***********************/

    iStatus = viWrite(ObjVi,(ViBuf)"acquire:stopafter sequence" ,27,&uiRectSingle);       //27 size of command
    if(iStatus < 0)
    {
        Error = fnError();
        return Error;
    }
    return XSUCCESS;
}
/********************************************************************************
 * Function Name:fnAutoset
 * Pupose:Autoset button functionality
 * Input:None
 * Output:AutoSet the Display,SUCESS / FAIL
 * Author :Varun
 * Date of Creation: 7/8/2015
 * Modification Date:
 * Modified By:
 * *******************************************************************************/
QString CTek2001c::fnAutoset()
{
    /*************************AUTO SET THE LEVEL***********************/

    iStatus = viWrite(ObjVi,(ViBuf)"AUTOSET EXECUTE",16, &uiRectAutoset);          //16 size of command
    if(iStatus < 0)
    {
        Error = fnError();
        return Error;
    }
    return XSUCCESS;
}
/********************************************************************************
 * Function Name:fnTriggerSetTo50
 * Pupose:Set the level to 50%
 * Input:None
 * Output:Set to 50% Oscilloscope,SUCESS / FAIL
 * Author :Varun
 * Date of Creation: 10/8/2015
 * Modification Date:
 * Modified By:
 * *******************************************************************************/
QString CTek2001c::fnTriggerSetTo50()
{
     /*************************TRIGGER LEVEL SET TO 50***********************/

    iStatus = viWrite(ObjVi,(ViBuf)"Trigger:main setlevel",22, &uiRectAutoset);          //16 size of command
    if(iStatus < 0)
    {
        Error = fnError();
        return Error;
    }
    return XSUCCESS;
}
/********************************************************************************
 * Function Name:fnTriggerForce
 * Pupose:Force Trigger
 * Input:None
 * Output:Trigger forced,SUCESS / FAIL
 * Author :Varun
 * Date of Creation: 10/8/2015
 * Modification Date:
 * Modified By:
 * *******************************************************************************/
QString CTek2001c::fnTriggerForce()
{

 /*********************************FORCE TRIGGER********************************/

    iStatus = viWrite(ObjVi,(ViBuf)"Trigger force\n",14, &uiRectTrigForce);          //16 size of command
    if(iStatus < 0)
    {
        Error = fnError();
        return Error;
    }
    return XSUCCESS;
}
/********************************************************************************
 * Function Name:fnTriggerLevel
 * Pupose:Setting the level of trigger
 * Input:Trigger level (float)
 * Output:Trigger level,SUCESS / FAIL
 * Author :Varun
 * Date of Creation: 10/8/2015
 * Modification Date:
 * Modified By:
 * *******************************************************************************/
QString CTek2001c::fnTriggerLevel(float fTriglevel)
{

 /**************************************TRIGGER LEVEL SELECTION*****************************************/

    ViChar  cCmd[]="Trigger:main:level ";
    QString sTemp;
    memset(szCmd0, 0, sizeof(szCmd0));
    strcpy(szCmd0,cCmd);
    sTemp=QString::number(fTriglevel);
    strcat(szCmd0,sTemp.toLatin1());
    strcat(szCmd0,"\n");
    iStatus = viWrite(ObjVi,(ViBuf)szCmd0,sizeof(szCmd0), &uiRectTrigLevel);          //16 size of command
    if(iStatus < 0)
    {
        Error = fnError();
        return Error;
    }
    return XSUCCESS;
}

/********************************************************************************
 * Function Name:fnTriggerMenu
 * Pupose:Set the type of trigger menu
 * Input:TriggerType Edge/Video/Pulse
 * Output:Sucess/Fail
 * Author :Varun
 * Date of Creation: 10/8/2015
 * Modification Date:
 * Modified By:
 * *******************************************************************************/
QString CTek2001c::fnTriggerMenu(unsigned int uiType)
{

 /**************************************TRIGGER TYPE SELECTION*****************************************/
    switch(uiType)                                                                               //TRIGGER TYPE
    {
     case 1:
        iStatus = viWrite(ObjVi,(ViBuf)"Trigger:main:type Edge\n",23, &uiRectMenuType);          //23 size of command
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }
        break;
    case 2:
        iStatus = viWrite(ObjVi,(ViBuf)"Trigger:main:type video\n",24, &uiRectMenuType);         //24 size of command
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }
        break;
    case 3:
        iStatus = viWrite(ObjVi,(ViBuf)"Trigger:main:type pulse\n",24, &uiRectMenuType);         //24 size of command
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }
        break;

    }
    return (QString)XSUCCESS;

}
/********************************************************************************
 * Function Name:fnSetEdgeTrigger
 * Pupose:Set the Edge Trigger Values
 * Input:Channel.no,Slope,Mode,Coupling
 * Output:Edge trigger Output,SUCESS / FAIL
 * Author :Varun
 * Date of Creation: 10/8/2015
 * Modification Date:
 * Modified By:
 * *******************************************************************************/
QString CTek2001c::fnSetEdgeTrigger(unsigned int uiChannel,bool bSlope,bool bMode,unsigned int uiCoupling)
{

  /**************************************SOURCE CHANNEL SELECTION*****************************************/
        if(1 == uiChannel)
         {
            iStatus = viWrite(ObjVi,(ViBuf)"Trigger:main:edge:source ch1\n",29, &uiRectEdgeTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }
        }
        else if(2 == uiChannel)
          {
            iStatus = viWrite(ObjVi,(ViBuf)"Trigger:main:edge:source ch2\n",29, &uiRectEdgeTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }
           }
   /*****************************************SLOPE TYPE SELECTION**************************************/
        if(0 == bSlope)
        {
            iStatus = viWrite(ObjVi,(ViBuf)"Trigger:main:edge:slope rise\n",29, &uiRectEdgeTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }
        }
        else
        {
            iStatus = viWrite(ObjVi,(ViBuf)"Trigger:main:edge:slope fall\n",29, &uiRectEdgeTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }
        }
   /*************************************MODE TYPE SELECTION******************************************/
        if(0 == bMode)
        {
            iStatus = viWrite(ObjVi,(ViBuf)"Trigger:main:mode norm\n",23, &uiRectEdgeTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }
        }
        else
        {
            iStatus = viWrite(ObjVi,(ViBuf)"Trigger:main:mode auto\n",23, &uiRectEdgeTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }
        }
   /*****************************************COUPLING TYPE SELECTION**************************************/
        if(1 == uiCoupling)
        {
            iStatus = viWrite(ObjVi,(ViBuf)"TRIGger:MAIn:EDGE:COUPling ac\n",30, &uiRectEdgeTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }
        }
        else if(2 == uiCoupling)
        {
            iStatus = viWrite(ObjVi,(ViBuf)"TRIGger:MAIn:EDGE:COUPling dc\n",30, &uiRectEdgeTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }

        }
        else if(3 == uiCoupling)
        {
            iStatus = viWrite(ObjVi,(ViBuf)"TRIGger:MAIn:EDGE:COUPling noiserej\n",36, &uiRectEdgeTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }

        }
        else if(4 == uiCoupling)
        {
            iStatus = viWrite(ObjVi,(ViBuf)"TRIGger:MAIn:EDGE:COUPling hfrej\n",33, &uiRectEdgeTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }

        }
        else if(5 == uiCoupling)
        {
            iStatus = viWrite(ObjVi,(ViBuf)"TRIGger:MAIn:EDGE:COUPling lfrej\n",33, &uiRectEdgeTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }

        }

    return XSUCCESS;

}

/********************************************************************************
 * Function Name:fnSetVideoTrigger
 * Pupose:Set the Video Trigger Values
 * Input:Channel.no,Polarity,Sync,Standard
 * Output:Video trigger Output,SUCESS / FAIL
 * Author :Varun
 * Date of Creation: 10/8/2015
 * Modification Date:
 * Modified By:
 * *******************************************************************************/
QString CTek2001c::fnSetVideoTrigger(unsigned int uiChannel,bool bPolarity,unsigned int uiSync,bool bStandard)
{


   /**************************************SOURCE CHANNEL SELECTION*****************************************/
        if(1 == uiChannel)
         {
            iStatus = viWrite(ObjVi,(ViBuf)"Trigger:main:video:source ch1\n",30, &uiRectVideoTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }
          }
        else if(2 == uiChannel)
         {
            iStatus = viWrite(ObjVi,(ViBuf)"Trigger:main:video:source ch2\n",30, &uiRectVideoTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }
          }
  /**************************************POLARITY TYPE SELECTION*****************************************/
        if(0 == bPolarity)
        {
            iStatus = viWrite(ObjVi,(ViBuf)"Trigger:main:video:polarity normal\n",35, &uiRectVideoTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }
        }
        else
        {
            iStatus = viWrite(ObjVi,(ViBuf)"Trigger:main:video:polarity invert\n",35, &uiRectVideoTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }
        }
  /**************************************SYNC TYPE SELECTION  *****************************************/
        if(1 == uiSync)
        {
            iStatus = viWrite(ObjVi,(ViBuf)"TRIGger:MAIn:video:sync field\n",30, &uiRectVideoTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }
        }
        else if(2 == uiSync)
        {
            iStatus = viWrite(ObjVi,(ViBuf)"TRIGger:MAIn:video:sync line\n",29, &uiRectVideoTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }

        }
        else if(3 == uiSync)
        {
            iStatus = viWrite(ObjVi,(ViBuf)"TRIGger:MAIn:video:sync linenum\n",32, &uiRectVideoTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }

        }
        else if(4 == uiSync)
        {
            iStatus = viWrite(ObjVi,(ViBuf)"TRIGger:MAIn:video:sync odd\n",28, &uiRectVideoTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }

        }
        else if(5 == uiSync)
        {
            iStatus = viWrite(ObjVi,(ViBuf)"TRIGger:MAIn:video:sync even\n",29, &uiRectVideoTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }

        }
  /**************************************VIDEO STANDARD TYPE SELECTION*****************************************/
        if(0 == bStandard)
        {
            iStatus = viWrite(ObjVi,(ViBuf)"Trigger:main:video:standard ntsc\n",33, &uiRectVideoTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }
        }
        else
        {
            iStatus = viWrite(ObjVi,(ViBuf)"Trigger:main:video:standard pal\n",32, &uiRectVideoTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }
        }

    return XSUCCESS;
}


/********************************************************************************
 * Function Name:fnSetPulseTrigger
 * Pupose:Set the Pulse Trigger Values
 * Input:Channel.no,When,Polarity,Mode,Coupling
 * Output:Pulse trigger Output,SUCESS / FAIL
 * Author :Varun
 * Date of Creation: 10/8/2015
 * Modification Date:
 * Modified By:
 * *******************************************************************************/
QString CTek2001c::fnSetPulseTrigger(unsigned int uiChannel,unsigned int uiWhen,bool bPolarity,bool bMode,unsigned int uiCoupling)
{

    /**************************************SOURCE CHANNEL SELECTION*****************************************/
        if(1 == uiChannel)
        {
            iStatus = viWrite(ObjVi,(ViBuf)"Trigger:main:pulse:source ch1\n",30, &uiRectPulseTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }
         }
        else if(2 == uiChannel)
         {
            iStatus = viWrite(ObjVi,(ViBuf)"Trigger:main:pulse:source ch2\n",30, &uiRectPulseTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }
        }
    /**************************************WHEN PULSE WIDTH SELECTION*****************************************/
        if(1 == uiWhen)
        {
            iStatus = viWrite(ObjVi,(ViBuf)"TRIGger:MAIn:PULse:WIDth:WHEN notequal",39, &uiRectPulseTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }
        }
        else if(2 == uiWhen)
        {
            iStatus = viWrite(ObjVi,(ViBuf)"TRIGger:MAIn:PULse:WIDth:WHEN inside",37, &uiRectPulseTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }

        }
        else if(3 == uiWhen)
        {
            iStatus = viWrite(ObjVi,(ViBuf)"TRIGger:MAIn:PULse:WIDth:WHEN outside",38, &uiRectPulseTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }

        }
        else if(4 == uiWhen)
        {
            iStatus = viWrite(ObjVi,(ViBuf)"TRIGger:MAIn:PULse:WIDth:WHEN equal",36, &uiRectPulseTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }

        }
    /**************************************PULSE WIDTH POLARITY SELECTION*****************************************/
        if(0 == bPolarity)
        {
            iStatus = viWrite(ObjVi,(ViBuf)"TRIGger:MAIn:PULse:WIDth:POLarity positive\n",43, &uiRectPulseTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }
        }
        else
        {
            iStatus = viWrite(ObjVi,(ViBuf)"TRIGger:MAIn:PULse:WIDth:POLarity negative\n",43, &uiRectPulseTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }
        }
   /**************************************MAIN MODE SELECTION*********************************************/
        if(0 == bMode)
        {
            iStatus = viWrite(ObjVi,(ViBuf)"Trigger:main:mode norm\n",23, &uiRectPulseTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }
        }
        else
        {
            iStatus = viWrite(ObjVi,(ViBuf)"Trigger:main:mode auto\n",23, &uiRectPulseTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }
        }
  /**************************************MAIN EDGE COUPLING SELECTION*********************************************/
        if(1 == uiCoupling)
        {
            iStatus = viWrite(ObjVi,(ViBuf)"TRIGger:MAIn:EDGE:COUPling ac\n",30, &uiRectPulseTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }
        }
        else if(2 == uiCoupling)
        {
            iStatus = viWrite(ObjVi,(ViBuf)"TRIGger:MAIn:EDGE:COUPling dc\n",30, &uiRectPulseTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }

        }
        else if(3 == uiCoupling)
        {
            iStatus = viWrite(ObjVi,(ViBuf)"TRIGger:MAIn:EDGE:COUPling noiserej\n",36, &uiRectPulseTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }

        }
        else if(4 == uiCoupling)
        {
            iStatus = viWrite(ObjVi,(ViBuf)"TRIGger:MAIn:EDGE:COUPling hfrej\n",33, &uiRectPulseTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }

        }
        else if(5 == uiCoupling)
        {
            iStatus = viWrite(ObjVi,(ViBuf)"TRIGger:MAIn:EDGE:COUPling lfrej\n",33, &uiRectPulseTrigger);
            if(iStatus < 0)
            {
                Error = fnError();
                return Error;
            }

        }

    return XSUCCESS;

}


/********************************************************************************
 * Function Name:fnVoltage
 * Pupose:Get the Voltage of Respective Channel
 * Input:Channel.no
 * Output:Voltage in volts
 * Author :Varun
 * Date of Creation: 7/8/2015
 * Modification Date:
 * Modified By:
 * *******************************************************************************/
QString CTek2001c::fnVoltage(unsigned int uiChannel)
{

    /**************************************SOURCE CHANNEL SELECTION & VOLTAGE QUERY*****************************************/
        if(1 == uiChannel)
        {

            iStatus = viWrite(ObjVi,(ViBuf) "ch1:volts?\n",11, &uiRectMearVolt);   //11 size of command
        }
        else if(2 == uiChannel)
        {
            iStatus = viWrite(ObjVi,(ViBuf) "ch2:volts?\n",11, &uiRectMearVolt);
        }
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }
     /**************************************VOLTAGE VALUE READING IN BUFFER *****************************************/
        memset(szBufferVoltage, 0, sizeof(szBufferVoltage));
        iStatus = viRead(ObjVi, (ViBuf)szBufferVoltage, sizeof(szBufferVoltage), &uiRectBuff);
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }
        return (QString)szBufferVoltage;

}

/********************************************************************************
 * Function Name:fnFrequency
 * Pupose:Get the Frequency of Respective Channel
 * Input:Channel.no
 * Output:Frequency in Hz
 * Author :Varun
 * Date of Creation: 7/8/2015
 * Modification Date:
 * Modified By:
 * *******************************************************************************/
QString CTek2001c::fnFrequency(unsigned int uiChannel)
{

   /**************************************SOURCE CHANNEL SELECTION*****************************************/
        if(1 == uiChannel)
        {
            iStatus = viWrite(ObjVi,(ViBuf)"measurement:immed:source ch1\n" ,31 , &uiRectMearFeq);   //31 size of command
        }
        else if(2 == uiChannel)
        {
           iStatus = viWrite(ObjVi,(ViBuf) "measurement:immed:source ch2\n",31 , &uiRectMearFeq);   //31 size of command
        }
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }
  /**************************************MEASUREMENT TYPE SELECTION *****************************************/
        iStatus = viWrite(ObjVi,(ViBuf)"measurement:immed:type freq\n" ,25 , &uiRectMearFeq);        //25 size of command
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }
  /**************************************MEASUREMENT TYPE VALUE QUERY *****************************************/
        iStatus = viWrite(ObjVi,(ViBuf)"measurement:immed:value?\n" ,25 , &uiRectMearFeq);       //25 size of command
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }
  /**************************************FREQUENCY VALUE READING IN BUFFER *****************************************/
        memset(szBufferFreq, 0, sizeof(szBufferFreq));
        iStatus = viRead(ObjVi, (ViBuf)szBufferFreq, sizeof(szBufferFreq), &uiRectBuff);
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }
        return (QString)szBufferFreq;

}
/********************************************************************************
 * Function Name:fnCaptureImage
 * Pupose:Capture&Store the ScreenShot Image
 * Input:None
 * Output:Image,SUCESS / FAIL
 * Author :Varun
 * Date of Creation: 7/8/2015
 * Modification Date:
 * Modified By:
 * *******************************************************************************/
QString CTek2001c::fnCaptureImage()
{
    FILE* fp = fopen("OsCapture.bmp","wb+");                                             //open bmp file to Store Capture Image

    /*********************************** COMMANDS TO CAPTURE & STORE THE IMAGE SECTION*****************************************/

        iStatus = viSetAttribute(ObjVi,VI_ATTR_USER_DATA,uiStat1);           // data used a particular session
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }

        iStatus = viGetAttribute(ObjVi, VI_ATTR_INTF_TYPE,&uiGetAttri3);          // specifies the interface type
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }

        iStatus = viOpenDefaultRM(&ObjDRM_1);                                   //default resource session
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }

        iStatus = viGetAttribute(ObjVi, VI_ATTR_INTF_TYPE,&uiGetAttri3);          // specifies the interface type
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }

        iStatus = viWrite(ObjVi, (ViBuf)"*IDN?", 5,&uiRec);                   //5 is size of command
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }

        memset(szBuffer, 0, sizeof(szBuffer));
        iStatus = viRead(ObjVi, (ViBuf) szBuffer, sizeof(szBuffer),&uiRetCnt);    //Reads Device ID
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }

        uiStat1 = 30000;
        iStatus = viSetAttribute(ObjVi,VI_ATTR_USER_DATA,uiStat1);           // data used a particular session
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }

        iStatus = viLock(ObjVi, 1, 3000, VI_NULL, VI_NULL);                 //Establishes an access mode
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }

        uiStat1 = 4;
        iStatus = viSetAttribute(ObjVi,VI_ATTR_IO_PROT,uiStat1);             // specifies which protocol to use
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }

        iStatus = viWrite(ObjVi, (ViBuf)"Header Off", 11, &uiRecHeader);                  //write query causes the oscilloscope to omit headers
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }
        iStatus =  viWrite(ObjVi, (ViBuf)"Hardcopy:Format?",17, &uiRecHeaderFmt);       //output data format for hardcopies
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }
        memset(szBuffer, 0, sizeof(szBuffer));
        iStatus = viRead(ObjVi, (ViBuf) szBuffer, sizeof(szBuffer), &uiRetCntjpg);          //output read and stored in buffer
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }
        iStatus = viWrite(ObjVi, (ViBuf)"Hardcopy:Format BMP",20, &uiRecHeader);            //write query to change format to bmp
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }
        iStatus = viWrite(ObjVi,(ViBuf)"Hardcopy:Layout?" ,17, &uiRecHardLy);             //output layout for hardcopies
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }

        memset(szBuffer, 0, sizeof(szBuffer));
        iStatus = viRead(ObjVi, (ViBuf)szBuffer, sizeof(szBuffer), &uiRetCntjpg);            //output read and stored in buffer_2
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }
        iStatus = viWrite(ObjVi,(ViBuf) "Hardcopy:Layout Portrait", 25, &uiRecHeader);       //write query to change Layout to Portrait
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }
        iStatus = viWrite(ObjVi, (ViBuf)"Hardcopy:port?",15, &uiRecHardLy);               //output port for hardcopies
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }

        memset(szBuffer, 0, sizeof(szBuffer));
        iStatus = viRead(ObjVi, (ViBuf)szBuffer, sizeof(szBuffer), &uiRetCntjpg);            //output read and stored in buffer_3
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }
        iStatus = viWrite(ObjVi, (ViBuf)"Hardcopy Start", 15, &uiRecHardLy);               //write query Start Hardcopy
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }

        iStatus = viGetAttribute(ObjVi, VI_ATTR_INTF_TYPE, &uiGetAttri3);                         // specifies the interface type
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }

        iStatus=1;
        while(!(iStatus == 0))
        {
            memset(szBufferImage, 0, sizeof(szBufferImage));
            iStatus =viRead(ObjVi, (ViBuf)szBufferImage, sizeof(szBufferImage), &uiRectBuff);    //output read and stored in szBufferImage
            fwrite(szBufferImage,uiRectBuff,1,fp);                                          //output is stored in file test.bmp
        }
        fclose(fp);                                                                        //close bmp file
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }
        iStatus = viWrite(ObjVi,(ViBuf)"Hardcopy:Layout PORTRAIT\n" , 27, &uiRectHardLy2);          //write query to change Layout to Portrait
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }
        iStatus = viWrite(ObjVi,(ViBuf)"Hardcopy:Port USB\n" , 20, &uiRectUsbEnd);              //write query to change port to USB
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }
        iStatus = viWrite(ObjVi,(ViBuf)"Hardcopy:Format JPEG\n", 23, &uiRectFormEnd);           //write query to change format to jpeg
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }

        iStatus = viUnlock(ObjVi);                                                              //End lock for the specified resource
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }

        iStatus = viGetAttribute(ObjVi, VI_ATTR_USER_DATA, &uiRectEnd);
        if(iStatus < 0)
        {
            Error = fnError();
            return Error;
        }

    return XSUCCESS;
}
/********************************************************************************
 * Function Name:fnError
 * Pupose:Display Error Message
 * Input:None
 * Output:ErrorMessage
 * Author :Varun
 * Date of Creation: 7/8/2015
 * Modification Date:
 * Modified By:
 * *******************************************************************************/
QString CTek2001c::fnError()
{
    /********************** VISA COMMAND FAILURE ERROR ***********************/
    viStatusDesc(ObjVi, iStatus, szBuffer);                  // Report error and clean up
    fprintf(stderr, "failure: %s\n", szBuffer);
    return (QString)szBuffer;
}

/********************************************************************************
 * Function Name:~CTek2001c
 * Pupose:Destructor
 * Input:None
 * Output:None
 * Author :Varun
 * Date of Creation: 7/8/2015
 * Modification Date:
 * Modified By:
 * *******************************************************************************/
CTek2001c::~CTek2001c()
{

}

