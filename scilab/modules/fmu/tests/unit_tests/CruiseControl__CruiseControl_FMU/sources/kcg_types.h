/* $*************** KCG Version 6.1.3 (build i6) ****************
** Command: s2c613 -config C:/Program Files (x86)/Esterel Technologies/Esterel SCADE 6.4.2i25/examples/CruiseControl/CruiseControl/KCG\kcg_s2c_config.txt
** Generation date: 2013-06-12T22:26:25
*************************************************************$ */
#ifndef _KCG_TYPES_H_
#define _KCG_TYPES_H_

#define KCG_MAPW_CPY

#include "./user_macros.h"

#ifndef kcg_int
#define kcg_int kcg_int
typedef int kcg_int;
#endif /* kcg_int */

#ifndef kcg_bool
#define kcg_bool kcg_bool
typedef unsigned char kcg_bool;
#endif /* kcg_bool */

#ifndef kcg_real
#define kcg_real kcg_real
typedef double kcg_real;
#endif /* kcg_real */

#ifndef kcg_char
#define kcg_char kcg_char
typedef unsigned char kcg_char;
#endif /* kcg_char */

#ifndef kcg_false
#define kcg_false ((kcg_bool) 0)
#endif /* kcg_false */

#ifndef kcg_true
#define kcg_true ((kcg_bool) 1)
#endif /* kcg_true */

#ifndef kcg_assign
#include "kcg_assign.h"
#endif /* kcg_assign */

#ifndef kcg_assign_struct
#define kcg_assign_struct kcg_assign
#endif /* kcg_assign_struct */

#ifndef kcg_assign_array
#define kcg_assign_array kcg_assign
#endif /* kcg_assign_array */

/* CruiseControl::tCruiseState */
typedef enum {
  OFF_CruiseControl,
  INT_CruiseControl,
  STDBY_CruiseControl,
  ON_CruiseControl
} tCruiseState_CruiseControl;
/* CruiseControl::CruiseControl::SM1 */
typedef enum {
  SSM_TR_no_trans_SM1,
  SSM_TR_Off_1_SM1,
  SSM_TR_Enabled_1_SM1
} SSM_TR_SM1;
/* CruiseControl::CruiseControl::SM1 */
typedef enum { SSM_st_Off_SM1, SSM_st_Enabled_SM1 } SSM_ST_SM1;
/* CruiseControl::CruiseControl::SM1::Enabled::SM2 */
typedef enum {
  SSM_TR_no_trans_SM2_SM1_Enabled,
  SSM_TR_Active_1_SM2_SM1_Enabled,
  SSM_TR_Interrupt_1_SM2_SM1_Enabled
} SSM_TR_SM2_SM1_Enabled;
/* CruiseControl::CruiseControl::SM1::Enabled::SM2 */
typedef enum {
  SSM_st_Active_SM1_Enabled_SM2,
  SSM_st_Interrupt_SM1_Enabled_SM2
} SSM_ST_SM2_SM1_Enabled;
/* CruiseControl::CruiseControl::SM1::Enabled::SM2::Active::SM3 */
typedef enum {
  SSM_TR_no_trans_SM3_SM1_Enabled_SM2_Active,
  SSM_TR_On_1_SM3_SM1_Enabled_SM2_Active,
  SSM_TR_StandBy_1_SM3_SM1_Enabled_SM2_Active
} SSM_TR_SM3_SM1_Enabled_SM2_Active;
/* CruiseControl::CruiseControl::SM1::Enabled::SM2::Active::SM3 */
typedef enum {
  SSM_st_On_SM1_Enabled_SM2_Active_SM3,
  SSM_st_StandBy_SM1_Enabled_SM2_Active_SM3
} SSM_ST_SM3_SM1_Enabled_SM2_Active;
/* CarType::tPercent */
typedef kcg_real tPercent_CarType;

/* CarType::tSpeed */
typedef kcg_real tSpeed_CarType;

#endif /* _KCG_TYPES_H_ */
/* $*************** KCG Version 6.1.3 (build i6) ****************
** kcg_types.h
** Generation date: 2013-06-12T22:26:25
*************************************************************$ */

