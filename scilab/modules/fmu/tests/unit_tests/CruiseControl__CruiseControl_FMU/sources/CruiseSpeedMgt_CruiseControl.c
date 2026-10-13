/* $*************** KCG Version 6.1.3 (build i6) ****************
** Command: s2c613 -config C:/Program Files (x86)/Esterel Technologies/Esterel SCADE 6.4.2i25/examples/CruiseControl/CruiseControl/KCG\kcg_s2c_config.txt
** Generation date: 2013-06-12T22:26:25
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "CruiseSpeedMgt_CruiseControl.h"

void CruiseSpeedMgt_reset_CruiseControl(outC_CruiseSpeedMgt_CruiseControl *outC)
{
  outC->init = kcg_true;
}

/* CruiseControl::CruiseSpeedMgt */
void CruiseSpeedMgt_CruiseControl(
  /* CruiseControl::CruiseSpeedMgt::Set */kcg_bool Set,
  /* CruiseControl::CruiseSpeedMgt::QuickAccel */kcg_bool QuickAccel,
  /* CruiseControl::CruiseSpeedMgt::QuickDecel */kcg_bool QuickDecel,
  /* CruiseControl::CruiseSpeedMgt::Speed */tSpeed_CarType Speed,
  outC_CruiseSpeedMgt_CruiseControl *outC)
{
  /* CruiseControl::CruiseSpeedMgt::CruiseSpeed */ tSpeed_CarType last_CruiseSpeed;
  /* CruiseControl::CruiseSpeedMgt::LocalCruiseSpeed */ tSpeed_CarType LocalCruiseSpeed;
  
  if (outC->init) {
    outC->init = kcg_false;
    last_CruiseSpeed = Speed;
  }
  else {
    last_CruiseSpeed = outC->CruiseSpeed;
  }
  if (Set) {
    LocalCruiseSpeed = Speed;
  }
  else if (QuickAccel) {
    LocalCruiseSpeed = last_CruiseSpeed + SpeedInc_CruiseControl;
  }
  else if (QuickDecel) {
    LocalCruiseSpeed = last_CruiseSpeed - SpeedInc_CruiseControl;
  }
  else {
    LocalCruiseSpeed = last_CruiseSpeed;
  }
  if (LocalCruiseSpeed >= SpeedMax_CruiseControl) {
    outC->CruiseSpeed = SpeedMax_CruiseControl;
  }
  else if (LocalCruiseSpeed <= SpeedMin_CruiseControl) {
    outC->CruiseSpeed = SpeedMin_CruiseControl;
  }
  else {
    outC->CruiseSpeed = LocalCruiseSpeed;
  }
}

/* $*************** KCG Version 6.1.3 (build i6) ****************
** CruiseSpeedMgt_CruiseControl.c
** Generation date: 2013-06-12T22:26:25
*************************************************************$ */

