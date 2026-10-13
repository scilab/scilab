/* $*************** KCG Version 6.1.3 (build i6) ****************
** Command: s2c613 -config C:/Program Files (x86)/Esterel Technologies/Esterel SCADE 6.4.2i25/examples/CruiseControl/CruiseControl/KCG\kcg_s2c_config.txt
** Generation date: 2013-06-12T22:26:25
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "CruiseRegulation_CruiseControl.h"

void CruiseRegulation_reset_CruiseControl(
  outC_CruiseRegulation_CruiseControl *outC)
{
  outC->init = kcg_true;
}

/* CruiseControl::CruiseRegulation */
void CruiseRegulation_CruiseControl(
  /* CruiseControl::CruiseRegulation::CruiseSpeed */tSpeed_CarType CruiseSpeed,
  /* CruiseControl::CruiseRegulation::VehiculeSpeed */tSpeed_CarType VehiculeSpeed,
  outC_CruiseRegulation_CruiseControl *outC)
{
  tSpeed_CarType tmp1;
  tSpeed_CarType tmp;
  /* CruiseControl::CruiseRegulation::_L3 */ kcg_real _L3;
  
  _L3 = CruiseSpeed - VehiculeSpeed;
  if (outC->init) {
    outC->init = kcg_false;
    tmp1 = ZeroSpeed_CruiseControl;
    tmp = ZeroSpeed_CruiseControl;
  }
  else {
    if (outC->_L14) {
      tmp1 = ZeroSpeed_CruiseControl;
    }
    else {
      tmp1 = _L3;
    }
    tmp = outC->_L10;
  }
  outC->_L10 = tmp1 + tmp;
  /* 1 */
  SaturateThrottle_CruiseControl(
    _L3 * Kp_CruiseControl + outC->_L10 * Ki_CruiseControl,
    &outC->Throttle,
    &outC->_L14);
}

/* $*************** KCG Version 6.1.3 (build i6) ****************
** CruiseRegulation_CruiseControl.c
** Generation date: 2013-06-12T22:26:25
*************************************************************$ */

