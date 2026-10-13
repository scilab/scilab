/* $*************** KCG Version 6.1.3 (build i6) ****************
** Command: s2c613 -config C:/Program Files (x86)/Esterel Technologies/Esterel SCADE 6.4.2i25/examples/CruiseControl/CruiseControl/KCG\kcg_s2c_config.txt
** Generation date: 2013-06-12T22:26:25
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "CruiseControl_CruiseControl.h"

void CruiseControl_reset_CruiseControl(outC_CruiseControl_CruiseControl *outC)
{
  outC->init = kcg_true;
  outC->init1 = kcg_true;
  outC->init2 = kcg_true;
  /* 2 */ CruiseRegulation_reset_CruiseControl(&outC->Context_2);
  /* 1 */ CruiseSpeedMgt_reset_CruiseControl(&outC->Context_1);
}

/* CruiseControl::CruiseControl */
void CruiseControl_CruiseControl(
  inC_CruiseControl_CruiseControl *inC,
  outC_CruiseControl_CruiseControl *outC)
{
  /* CruiseControl::CruiseControl::SM1::Enabled::SM2 */ kcg_bool SM2_reset_act_SM1_Enabled;
  /* CruiseControl::CruiseControl::SM1::Enabled::SM2 */ SSM_ST_SM2_SM1_Enabled SM2_state_act_SM1_Enabled;
  /* CruiseControl::CruiseControl::SM1::Enabled::SM2 */ SSM_ST_SM2_SM1_Enabled SM2_state_sel_SM1_Enabled;
  /* CruiseControl::CruiseControl::SM1::Enabled::SM2::Active::SM3 */ SSM_ST_SM3_SM1_Enabled_SM2_Active SM3_state_sel_SM1_Enabled_SM2_Active;
  /* CruiseControl::CruiseControl::SM1::Enabled::SM2::Active::SM3 */ SSM_ST_SM3_SM1_Enabled_SM2_Active SM3_state_act_SM1_Enabled_SM2_Active;
  /* CruiseControl::CruiseControl::SM1::Enabled::SM2::Active::SM3 */ kcg_bool SM3_reset_act_SM1_Enabled_SM2_Active;
  /* CruiseControl::CruiseControl::SM1::Enabled::SM2::Active::StdbyCondition */ kcg_bool StdbyCondition_SM1_Enabled_SM2_Active;
  /* CruiseControl::CruiseControl::SM1 */ SSM_ST_SM1 SM1_state_sel;
  /* CruiseControl::CruiseControl::SM1 */ SSM_ST_SM1 SM1_state_act;
  /* CruiseControl::CruiseControl::SM1 */ kcg_bool SM1_reset_act;
  
  if (outC->init) {
    outC->init = kcg_false;
    SM1_state_sel = SSM_st_Off_SM1;
  }
  else {
    SM1_state_sel = outC->SM1_state_nxt;
  }
  switch (SM1_state_sel) {
    case SSM_st_Off_SM1 :
      if (inC->On) {
        SM1_state_act = SSM_st_Enabled_SM1;
      }
      else {
        SM1_state_act = SSM_st_Off_SM1;
      }
      SM1_reset_act = inC->On;
      break;
    case SSM_st_Enabled_SM1 :
      if (inC->Off) {
        SM1_state_act = SSM_st_Off_SM1;
      }
      else {
        SM1_state_act = SSM_st_Enabled_SM1;
      }
      SM1_reset_act = inC->Off;
      break;
    
  }
  switch (SM1_state_act) {
    case SSM_st_Enabled_SM1 :
      if (SM1_reset_act) {
        /* 1 */ CruiseSpeedMgt_reset_CruiseControl(&outC->Context_1);
        outC->init1 = kcg_true;
      }
      /* 1 */
      CruiseSpeedMgt_CruiseControl(
        inC->Set,
        inC->QuickAccel,
        inC->QuickDecel,
        inC->Speed,
        &outC->Context_1);
      if (outC->init1) {
        SM2_state_sel_SM1_Enabled = SSM_st_Active_SM1_Enabled_SM2;
      }
      else {
        SM2_state_sel_SM1_Enabled = outC->SM2_state_nxt_SM1_Enabled;
      }
      switch (SM2_state_sel_SM1_Enabled) {
        case SSM_st_Interrupt_SM1_Enabled_SM2 :
          if (inC->Resume) {
            SM2_state_act_SM1_Enabled = SSM_st_Active_SM1_Enabled_SM2;
          }
          else {
            SM2_state_act_SM1_Enabled = SSM_st_Interrupt_SM1_Enabled_SM2;
          }
          SM2_reset_act_SM1_Enabled = inC->Resume;
          break;
        case SSM_st_Active_SM1_Enabled_SM2 :
          SM2_reset_act_SM1_Enabled = inC->Brake > PedalsMin_CruiseControl;
          if (SM2_reset_act_SM1_Enabled) {
            SM2_state_act_SM1_Enabled = SSM_st_Interrupt_SM1_Enabled_SM2;
          }
          else {
            SM2_state_act_SM1_Enabled = SSM_st_Active_SM1_Enabled_SM2;
          }
          break;
        
      }
      switch (SM2_state_act_SM1_Enabled) {
        case SSM_st_Active_SM1_Enabled_SM2 :
          StdbyCondition_SM1_Enabled_SM2_Active = (inC->Accel >
              PedalsMin_CruiseControl) | (inC->Speed < SpeedMin_CruiseControl) |
            (inC->Speed > SpeedMax_CruiseControl);
          if (SM2_reset_act_SM1_Enabled) {
            outC->init2 = kcg_true;
          }
          break;
        
      }
      if (SM1_reset_act) {
        outC->init2 = kcg_true;
        /* 2 */ CruiseRegulation_reset_CruiseControl(&outC->Context_2);
      }
      outC->SM1_state_nxt = SSM_st_Enabled_SM1;
      outC->CruiseSpeed = outC->Context_1.CruiseSpeed;
      outC->init1 = kcg_false;
      switch (SM2_state_act_SM1_Enabled) {
        case SSM_st_Interrupt_SM1_Enabled_SM2 :
          outC->SM2_state_nxt_SM1_Enabled = SSM_st_Interrupt_SM1_Enabled_SM2;
          outC->CruiseState = INT_CruiseControl;
          outC->ThrottleCmd = inC->Accel;
          break;
        case SSM_st_Active_SM1_Enabled_SM2 :
          if (outC->init2) {
            SM3_state_sel_SM1_Enabled_SM2_Active =
              SSM_st_On_SM1_Enabled_SM2_Active_SM3;
          }
          else {
            SM3_state_sel_SM1_Enabled_SM2_Active =
              outC->SM3_state_nxt_SM1_Enabled_SM2_Active;
          }
          switch (SM3_state_sel_SM1_Enabled_SM2_Active) {
            case SSM_st_On_SM1_Enabled_SM2_Active_SM3 :
              if (StdbyCondition_SM1_Enabled_SM2_Active) {
                SM3_state_act_SM1_Enabled_SM2_Active =
                  SSM_st_StandBy_SM1_Enabled_SM2_Active_SM3;
              }
              else {
                SM3_state_act_SM1_Enabled_SM2_Active =
                  SSM_st_On_SM1_Enabled_SM2_Active_SM3;
              }
              SM3_reset_act_SM1_Enabled_SM2_Active =
                StdbyCondition_SM1_Enabled_SM2_Active;
              break;
            case SSM_st_StandBy_SM1_Enabled_SM2_Active_SM3 :
              SM3_reset_act_SM1_Enabled_SM2_Active =
                !StdbyCondition_SM1_Enabled_SM2_Active;
              if (SM3_reset_act_SM1_Enabled_SM2_Active) {
                SM3_state_act_SM1_Enabled_SM2_Active =
                  SSM_st_On_SM1_Enabled_SM2_Active_SM3;
              }
              else {
                SM3_state_act_SM1_Enabled_SM2_Active =
                  SSM_st_StandBy_SM1_Enabled_SM2_Active_SM3;
              }
              break;
            
          }
          if (SM2_reset_act_SM1_Enabled) {
            /* 2 */ CruiseRegulation_reset_CruiseControl(&outC->Context_2);
          }
          outC->SM2_state_nxt_SM1_Enabled = SSM_st_Active_SM1_Enabled_SM2;
          switch (SM3_state_act_SM1_Enabled_SM2_Active) {
            case SSM_st_StandBy_SM1_Enabled_SM2_Active_SM3 :
              outC->SM3_state_nxt_SM1_Enabled_SM2_Active =
                SSM_st_StandBy_SM1_Enabled_SM2_Active_SM3;
              outC->CruiseState = STDBY_CruiseControl;
              outC->ThrottleCmd = inC->Accel;
              break;
            case SSM_st_On_SM1_Enabled_SM2_Active_SM3 :
              if (SM3_reset_act_SM1_Enabled_SM2_Active) {
                /* 2 */ CruiseRegulation_reset_CruiseControl(&outC->Context_2);
              }
              /* 2 */
              CruiseRegulation_CruiseControl(
                outC->Context_1.CruiseSpeed,
                inC->Speed,
                &outC->Context_2);
              outC->SM3_state_nxt_SM1_Enabled_SM2_Active =
                SSM_st_On_SM1_Enabled_SM2_Active_SM3;
              outC->CruiseState = ON_CruiseControl;
              outC->ThrottleCmd = outC->Context_2.Throttle;
              break;
            
          }
          outC->init2 = kcg_false;
          break;
        
      }
      break;
    case SSM_st_Off_SM1 :
      outC->SM1_state_nxt = SSM_st_Off_SM1;
      outC->CruiseState = OFF_CruiseControl;
      outC->ThrottleCmd = inC->Accel;
      outC->CruiseSpeed = ZeroSpeed_CruiseControl;
      break;
    
  }
}

/* $*************** KCG Version 6.1.3 (build i6) ****************
** CruiseControl_CruiseControl.c
** Generation date: 2013-06-12T22:26:25
*************************************************************$ */

