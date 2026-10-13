/* $*************** KCG Version 6.1.3 (build i6) ****************
** Command: s2c613 -config C:/Program Files (x86)/Esterel Technologies/Esterel SCADE 6.4.2i25/examples/CruiseControl/CruiseControl/KCG\kcg_s2c_config.txt
** Generation date: 2013-06-12T22:26:25
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "SaturateThrottle_CruiseControl.h"

/* CruiseControl::SaturateThrottle */
void SaturateThrottle_CruiseControl(
  /* CruiseControl::SaturateThrottle::ThrottleIn */tPercent_CarType ThrottleIn,
  /* CruiseControl::SaturateThrottle::ThrottleOut */tPercent_CarType *ThrottleOut,
  /* CruiseControl::SaturateThrottle::Saturate */kcg_bool *Saturate)
{
  /* CruiseControl::SaturateThrottle::_L9 */ kcg_bool _L9;
  /* CruiseControl::SaturateThrottle::_L7 */ kcg_bool _L7;
  
  _L7 = ThrottleIn > RegulThrottleMax_CruiseControl;
  _L9 = ThrottleIn < ZeroPercent_CruiseControl;
  *Saturate = _L7 | _L9;
  if (_L7) {
    *ThrottleOut = RegulThrottleMax_CruiseControl;
  }
  else if (_L9) {
    *ThrottleOut = ZeroPercent_CruiseControl;
  }
  else {
    *ThrottleOut = ThrottleIn;
  }
}

/* $*************** KCG Version 6.1.3 (build i6) ****************
** SaturateThrottle_CruiseControl.c
** Generation date: 2013-06-12T22:26:25
*************************************************************$ */

