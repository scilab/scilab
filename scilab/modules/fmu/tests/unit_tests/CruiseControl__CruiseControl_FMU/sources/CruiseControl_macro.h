#ifndef _CRUISECONTROL_MACRO_H_
#define _CRUISECONTROL_MACRO_H_


/*$************** SCADE Macro wrapper *********************
** Begin of file CruiseControl_macro.h
***********************************************************$*/


/*$**********************************************************
*                        INCLUDES
***********************************************************$*/

/* includes */
#include "CruiseControl_CruiseControl.h"

/*$**********************************************************
*                           CONTEXT
***********************************************************$*/

typedef struct {
    inC_CruiseControl_CruiseControl SAO_CTX_inC_CruiseControl_CruiseControl;
    outC_CruiseControl_CruiseControl SAO_CTX_outC_CruiseControl_CruiseControl;
} WU_CruiseControl_CruiseControl;
#define WU_CTX_TYPE_CruiseControl WU_CruiseControl_CruiseControl
#define WU_CTX_TYPE_CruiseControl_SIZE sizeof(WU_CruiseControl_CruiseControl)

/*$**********************************************************
*                           SENSORS
***********************************************************$*/

#define DECLARE_SENSORS_CruiseControl() 

#define DECLARE_EXT_SENSORS_CruiseControl(MODE) 

/*$**********************************************************
*                           INPUTS
***********************************************************$*/

#define VARC_CruiseControl_On(context) context.SAO_CTX_inC_CruiseControl_CruiseControl.On  /* CruiseControl::CruiseControl/On */
#define VAR_CruiseControl_On VARC_CruiseControl_On(CTX_CruiseControl_CruiseControl)

#define VARC_CruiseControl_Off(context) context.SAO_CTX_inC_CruiseControl_CruiseControl.Off  /* CruiseControl::CruiseControl/Off */
#define VAR_CruiseControl_Off VARC_CruiseControl_Off(CTX_CruiseControl_CruiseControl)

#define VARC_CruiseControl_Resume(context) context.SAO_CTX_inC_CruiseControl_CruiseControl.Resume  /* CruiseControl::CruiseControl/Resume */
#define VAR_CruiseControl_Resume VARC_CruiseControl_Resume(CTX_CruiseControl_CruiseControl)

#define VARC_CruiseControl_Set(context) context.SAO_CTX_inC_CruiseControl_CruiseControl.Set  /* CruiseControl::CruiseControl/Set */
#define VAR_CruiseControl_Set VARC_CruiseControl_Set(CTX_CruiseControl_CruiseControl)

#define VARC_CruiseControl_QuickAccel(context) context.SAO_CTX_inC_CruiseControl_CruiseControl.QuickAccel  /* CruiseControl::CruiseControl/QuickAccel */
#define VAR_CruiseControl_QuickAccel VARC_CruiseControl_QuickAccel(CTX_CruiseControl_CruiseControl)

#define VARC_CruiseControl_QuickDecel(context) context.SAO_CTX_inC_CruiseControl_CruiseControl.QuickDecel  /* CruiseControl::CruiseControl/QuickDecel */
#define VAR_CruiseControl_QuickDecel VARC_CruiseControl_QuickDecel(CTX_CruiseControl_CruiseControl)

#define VARC_CruiseControl_Accel(context) context.SAO_CTX_inC_CruiseControl_CruiseControl.Accel  /* CruiseControl::CruiseControl/Accel */
#define VAR_CruiseControl_Accel VARC_CruiseControl_Accel(CTX_CruiseControl_CruiseControl)

#define VARC_CruiseControl_Brake(context) context.SAO_CTX_inC_CruiseControl_CruiseControl.Brake  /* CruiseControl::CruiseControl/Brake */
#define VAR_CruiseControl_Brake VARC_CruiseControl_Brake(CTX_CruiseControl_CruiseControl)

#define VARC_CruiseControl_Speed(context) context.SAO_CTX_inC_CruiseControl_CruiseControl.Speed  /* CruiseControl::CruiseControl/Speed */
#define VAR_CruiseControl_Speed VARC_CruiseControl_Speed(CTX_CruiseControl_CruiseControl)


/*$**********************************************************
*                           OUTPUTS
***********************************************************$*/

#define VARC_CruiseControl_CruiseSpeed(context) context.SAO_CTX_outC_CruiseControl_CruiseControl.CruiseSpeed  /* CruiseControl::CruiseControl/CruiseSpeed */
#define VAR_CruiseControl_CruiseSpeed VARC_CruiseControl_CruiseSpeed(CTX_CruiseControl_CruiseControl)

#define VARC_CruiseControl_ThrottleCmd(context) context.SAO_CTX_outC_CruiseControl_CruiseControl.ThrottleCmd  /* CruiseControl::CruiseControl/ThrottleCmd */
#define VAR_CruiseControl_ThrottleCmd VARC_CruiseControl_ThrottleCmd(CTX_CruiseControl_CruiseControl)

#define VARC_CruiseControl_CruiseState(context) context.SAO_CTX_outC_CruiseControl_CruiseControl.CruiseState  /* CruiseControl::CruiseControl/CruiseState */
#define VAR_CruiseControl_CruiseState VARC_CruiseControl_CruiseState(CTX_CruiseControl_CruiseControl)


/*$**********************************************************
*                           ELEMENT ACCESS
***********************************************************$*/

#define GET_ELEMENT_AT(VARIABLE, INDEX)  (VARIABLE)[INDEX]
#define GET_STRUCTURE_FIELD(VARIABLE, FIELD) (VARIABLE).FIELD

/*$**********************************************************
*                        VARIABLES TYPES
***********************************************************$*/

#define T_On kcg_bool
#define CPY_On(DST, SRC) DST = SRC
#define T_Off kcg_bool
#define CPY_Off(DST, SRC) DST = SRC
#define T_Resume kcg_bool
#define CPY_Resume(DST, SRC) DST = SRC
#define T_Set kcg_bool
#define CPY_Set(DST, SRC) DST = SRC
#define T_QuickAccel kcg_bool
#define CPY_QuickAccel(DST, SRC) DST = SRC
#define T_QuickDecel kcg_bool
#define CPY_QuickDecel(DST, SRC) DST = SRC
#define T_Accel tPercent_CarType
#define CPY_Accel(DST, SRC) DST = SRC
#define T_Brake tPercent_CarType
#define CPY_Brake(DST, SRC) DST = SRC
#define T_Speed tSpeed_CarType
#define CPY_Speed(DST, SRC) DST = SRC
#define T_CruiseSpeed tSpeed_CarType
#define CPY_CruiseSpeed(DST, SRC) DST = SRC
#define T_ThrottleCmd tPercent_CarType
#define CPY_ThrottleCmd(DST, SRC) DST = SRC
#define T_CruiseState tCruiseState_CruiseControl
#define CPY_CruiseState(DST, SRC) DST = SRC


/*$**********************************************************
*                      CREATION, INIT AND PERFORM
***********************************************************$*/

#define DECLAREC_CTXT_CruiseControl(context)  WU_CruiseControl_CruiseControl context;

#define DECLARE_CTXT_CruiseControl() DECLAREC_CTXT_CruiseControl(CTX_CruiseControl_CruiseControl)

#define DECLAREC_EXT_CTXT_CruiseControl(MODE , context) MODE WU_CruiseControl_CruiseControl context;

#define DECLARE_EXT_CTXT_CruiseControl(MODE) DECLAREC_EXT_CTXT_CruiseControl(MODE , CTX_CruiseControl_CruiseControl)

#define INITC_CruiseControl(context) CruiseControl_reset_CruiseControl(&context.SAO_CTX_outC_CruiseControl_CruiseControl)
#define INIT_CruiseControl() INITC_CruiseControl(CTX_CruiseControl_CruiseControl)

#define PERFORMC_CruiseControl(context) 	CruiseControl_CruiseControl( \
		&context.SAO_CTX_inC_CruiseControl_CruiseControl /* input context */,\
		&context.SAO_CTX_outC_CruiseControl_CruiseControl /* output/memories context */\
	)

#define PERFORM_CruiseControl() PERFORMC_CruiseControl(CTX_CruiseControl_CruiseControl)

/*$************** SCADE Macro wrapper *********************
** End of file CruiseControl_macro.h
***********************************************************$*/

#endif /* _CRUISECONTROL_MACRO_H_ */
