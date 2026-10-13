/* $*************** KCG Version 6.1.3 (build i6) ****************
** Command: s2c613 -config C:/Program Files (x86)/Esterel Technologies/Esterel SCADE 6.4.2i25/examples/CruiseControl/CruiseControl/KCG\kcg_s2c_config.txt
** Generation date: 2013-06-12T22:26:25
*************************************************************$ */
#ifndef _CruiseSpeedMgt_CruiseControl_H_
#define _CruiseSpeedMgt_CruiseControl_H_

#include "kcg_types.h"

/* =====================  no input structure  ====================== */

/* ========================  context type  ========================= */
typedef struct {
  /* ---------------------------  outputs  --------------------------- */
  tSpeed_CarType /* CruiseControl::CruiseSpeedMgt::CruiseSpeed */ CruiseSpeed;
  /* -----------------------  no local probes  ----------------------- */
  /* -------------------- initialization variables  ------------------ */
  kcg_bool init;
  /* -----------------------  no local memory  ----------------------- */
  /* -------------------- no sub nodes' contexts  -------------------- */
  /* ----------------- no clocks of observable data ------------------ */
} outC_CruiseSpeedMgt_CruiseControl;

/* ===========  node initialization and cycle functions  =========== */
/* CruiseControl::CruiseSpeedMgt */
extern void CruiseSpeedMgt_CruiseControl(
  /* CruiseControl::CruiseSpeedMgt::Set */kcg_bool Set,
  /* CruiseControl::CruiseSpeedMgt::QuickAccel */kcg_bool QuickAccel,
  /* CruiseControl::CruiseSpeedMgt::QuickDecel */kcg_bool QuickDecel,
  /* CruiseControl::CruiseSpeedMgt::Speed */tSpeed_CarType Speed,
  outC_CruiseSpeedMgt_CruiseControl *outC);

extern void CruiseSpeedMgt_reset_CruiseControl(
  outC_CruiseSpeedMgt_CruiseControl *outC);

#endif /* _CruiseSpeedMgt_CruiseControl_H_ */
/* $*************** KCG Version 6.1.3 (build i6) ****************
** CruiseSpeedMgt_CruiseControl.h
** Generation date: 2013-06-12T22:26:25
*************************************************************$ */

