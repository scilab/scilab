/* $*************** KCG Version 6.1.3 (build i6) ****************
** Command: s2c613 -config C:/Program Files (x86)/Esterel Technologies/Esterel SCADE 6.4.2i25/examples/CruiseControl/CruiseControl/KCG\kcg_s2c_config.txt
** Generation date: 2013-06-12T22:26:25
*************************************************************$ */
#ifndef _CruiseRegulation_CruiseControl_H_
#define _CruiseRegulation_CruiseControl_H_

#include "kcg_types.h"
#include "SaturateThrottle_CruiseControl.h"

/* =====================  no input structure  ====================== */

/* ========================  context type  ========================= */
typedef struct {
  /* ---------------------------  outputs  --------------------------- */
  tPercent_CarType /* CruiseControl::CruiseRegulation::Throttle */ Throttle;
  /* -----------------------  no local probes  ----------------------- */
  /* -------------------- initialization variables  ------------------ */
  kcg_bool init;
  /* ----------------------- local memories  ------------------------- */
  kcg_real /* CruiseControl::CruiseRegulation::_L10 */ _L10;
  kcg_bool /* CruiseControl::CruiseRegulation::_L14 */ _L14;
  /* -------------------- no sub nodes' contexts  -------------------- */
  /* ----------------- no clocks of observable data ------------------ */
} outC_CruiseRegulation_CruiseControl;

/* ===========  node initialization and cycle functions  =========== */
/* CruiseControl::CruiseRegulation */
extern void CruiseRegulation_CruiseControl(
  /* CruiseControl::CruiseRegulation::CruiseSpeed */tSpeed_CarType CruiseSpeed,
  /* CruiseControl::CruiseRegulation::VehiculeSpeed */tSpeed_CarType VehiculeSpeed,
  outC_CruiseRegulation_CruiseControl *outC);

extern void CruiseRegulation_reset_CruiseControl(
  outC_CruiseRegulation_CruiseControl *outC);

#endif /* _CruiseRegulation_CruiseControl_H_ */
/* $*************** KCG Version 6.1.3 (build i6) ****************
** CruiseRegulation_CruiseControl.h
** Generation date: 2013-06-12T22:26:25
*************************************************************$ */

