/* $*************** KCG Version 6.1.3 (build i6) ****************
** Command: s2c613 -config C:/Program Files (x86)/Esterel Technologies/Esterel SCADE 6.4.2i25/examples/CruiseControl/CruiseControl/KCG\kcg_s2c_config.txt
** Generation date: 2013-06-12T22:26:25
*************************************************************$ */
#ifndef _CruiseControl_CruiseControl_H_
#define _CruiseControl_CruiseControl_H_

#include "kcg_types.h"
#include "CruiseRegulation_CruiseControl.h"
#include "CruiseSpeedMgt_CruiseControl.h"

/* ========================  input structure  ====================== */
typedef struct {
  kcg_bool /* CruiseControl::CruiseControl::On */ On;
  kcg_bool /* CruiseControl::CruiseControl::Off */ Off;
  kcg_bool /* CruiseControl::CruiseControl::Resume */ Resume;
  kcg_bool /* CruiseControl::CruiseControl::Set */ Set;
  kcg_bool /* CruiseControl::CruiseControl::QuickAccel */ QuickAccel;
  kcg_bool /* CruiseControl::CruiseControl::QuickDecel */ QuickDecel;
  tPercent_CarType /* CruiseControl::CruiseControl::Accel */ Accel;
  tPercent_CarType /* CruiseControl::CruiseControl::Brake */ Brake;
  tSpeed_CarType /* CruiseControl::CruiseControl::Speed */ Speed;
} inC_CruiseControl_CruiseControl;

/* ========================  context type  ========================= */
typedef struct {
  /* ---------------------------  outputs  --------------------------- */
  tSpeed_CarType /* CruiseControl::CruiseControl::CruiseSpeed */ CruiseSpeed;
  tPercent_CarType /* CruiseControl::CruiseControl::ThrottleCmd */ ThrottleCmd;
  tCruiseState_CruiseControl /* CruiseControl::CruiseControl::CruiseState */ CruiseState;
  /* -----------------------  no local probes  ----------------------- */
  /* -------------------- initialization variables  ------------------ */
  kcg_bool init2;
  kcg_bool init1;
  kcg_bool init;
  /* ----------------------- local memories  ------------------------- */
  SSM_ST_SM2_SM1_Enabled /* CruiseControl::CruiseControl::SM1::Enabled::SM2 */ SM2_state_nxt_SM1_Enabled;
  SSM_ST_SM3_SM1_Enabled_SM2_Active /* CruiseControl::CruiseControl::SM1::Enabled::SM2::Active::SM3 */ SM3_state_nxt_SM1_Enabled_SM2_Active;
  SSM_ST_SM1 /* CruiseControl::CruiseControl::SM1 */ SM1_state_nxt;
  /* ---------------------  sub nodes' contexts  --------------------- */
  outC_CruiseSpeedMgt_CruiseControl /* 1 */ Context_1;
  outC_CruiseRegulation_CruiseControl /* 2 */ Context_2;
  /* ----------------- no clocks of observable data ------------------ */
} outC_CruiseControl_CruiseControl;

/* ===========  node initialization and cycle functions  =========== */
/* CruiseControl::CruiseControl */
extern void CruiseControl_CruiseControl(
  inC_CruiseControl_CruiseControl *inC,
  outC_CruiseControl_CruiseControl *outC);

extern void CruiseControl_reset_CruiseControl(
  outC_CruiseControl_CruiseControl *outC);

#endif /* _CruiseControl_CruiseControl_H_ */
/* $*************** KCG Version 6.1.3 (build i6) ****************
** CruiseControl_CruiseControl.h
** Generation date: 2013-06-12T22:26:25
*************************************************************$ */

