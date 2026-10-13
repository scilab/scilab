/* CruiseControl_newmapping.c */

#include <stddef.h>

#include "NewSmuTypes.h"
#include "SmuMapping.h"
#include "CruiseControl_newtype.h"
#include "CruiseControl_newmapping.h"

/* iterators */

/* clock active helper functions */

/* forward declarations */
#define MAP_DECL(ident, nb) static const MappingEntry ident##_entries[nb]; static const MappingScope ident
MAP_DECL(scope_30, 1);
MAP_DECL(scope_28, 7);
MAP_DECL(scope_25, 3);
MAP_DECL(scope_24, 2);
MAP_DECL(scope_23, 5);
MAP_DECL(scope_22, 1);
MAP_DECL(scope_21, 1);
MAP_DECL(scope_20, 1);
MAP_DECL(scope_19, 1);
MAP_DECL(scope_18, 1);
MAP_DECL(scope_17, 1);
MAP_DECL(scope_16, 1);
MAP_DECL(scope_15, 2);
MAP_DECL(scope_12, 2);
MAP_DECL(scope_11, 2);
MAP_DECL(scope_8, 2);
MAP_DECL(scope_7, 3);
MAP_DECL(scope_6, 1);
MAP_DECL(scope_5, 1);
MAP_DECL(scope_2, 2);
MAP_DECL(scope_1, 16);
MAP_DECL(scope_0, 1);

/* CruiseControl::CruiseRegulation/ CruiseRegulation_CruiseControllinear::Gain 3/ */
static const MappingEntry scope_30_entries[1] = {
  /* 0 */ { MAP_LOCAL, "_L1", NULL, sizeof(kcg_real), offsetof(outC_CruiseRegulation_CruiseControl, _L10), &_Type_kcg_real_Utils, NULL, NULL, NULL}
};
static const MappingScope scope_30 = {
  "CruiseControl::CruiseRegulation/ CruiseRegulation_CruiseControllinear::Gain 3/",
  scope_30_entries, 1,
};

/* CruiseControl::CruiseRegulation/ CruiseRegulation_CruiseControl */
static const MappingEntry scope_28_entries[7] = {
  /* 0 */ { MAP_LOCAL, "@kcg8", NULL, sizeof(kcg_bool), offsetof(outC_CruiseRegulation_CruiseControl, init), &_Type_kcg_bool_Utils, NULL, NULL, NULL},
  /* 1 */ { MAP_OUTPUT, "Throttle", NULL, sizeof(tPercent_CarType), offsetof(outC_CruiseRegulation_CruiseControl, Throttle), &_Type_tPercent_CarType_Utils, NULL, NULL, NULL},
  /* 2 */ { MAP_LOCAL, "_L10@mem", NULL, sizeof(kcg_real), offsetof(outC_CruiseRegulation_CruiseControl, _L10), &_Type_kcg_real_Utils, NULL, NULL, NULL},
  /* 3 */ { MAP_LOCAL, "_L13", NULL, sizeof(tPercent_CarType), offsetof(outC_CruiseRegulation_CruiseControl, Throttle), &_Type_tPercent_CarType_Utils, NULL, NULL, NULL},
  /* 4 */ { MAP_LOCAL, "_L14@mem", NULL, sizeof(kcg_bool), offsetof(outC_CruiseRegulation_CruiseControl, _L14), &_Type_kcg_bool_Utils, NULL, NULL, NULL},
  /* 5 */ { MAP_EXPANDED, "linear::Gain 2", NULL, 0, 0, NULL, NULL, NULL, NULL},
  /* 6 */ { MAP_EXPANDED, "linear::Gain 3", NULL, 0, 0, NULL, NULL, NULL, &scope_30}
};
static const MappingScope scope_28 = {
  "CruiseControl::CruiseRegulation/ CruiseRegulation_CruiseControl",
  scope_28_entries, 7,
};

/* CruiseControl::CruiseSpeedMgt/ CruiseSpeedMgt_CruiseControlIfBlock1: */
static const MappingEntry scope_25_entries[3] = {
  /* 0 */ { MAP_LOCAL, "@condition", NULL, sizeof(kcg_bool), 0, &_Type_kcg_bool_Utils, NULL, NULL, NULL},
  /* 1 */ { MAP_ACTION, "else:", NULL, 0, 0, NULL, NULL, NULL, NULL},
  /* 2 */ { MAP_ACTION, "then:", NULL, 0, 0, NULL, NULL, NULL, NULL}
};
static const MappingScope scope_25 = {
  "CruiseControl::CruiseSpeedMgt/ CruiseSpeedMgt_CruiseControlIfBlock1:",
  scope_25_entries, 3,
};

/* CruiseControl::CruiseSpeedMgt/ CruiseSpeedMgt_CruiseControlpwlinear::LimiterUnSymmetrical 1/ */
static const MappingEntry scope_24_entries[2] = {
  /* 0 */ { MAP_OUTPUT, "LUS_Output", NULL, sizeof(tSpeed_CarType), offsetof(outC_CruiseSpeedMgt_CruiseControl, CruiseSpeed), &_Type_tSpeed_CarType_Utils, NULL, NULL, NULL},
  /* 1 */ { MAP_LOCAL, "_L9", NULL, sizeof(tSpeed_CarType), offsetof(outC_CruiseSpeedMgt_CruiseControl, CruiseSpeed), &_Type_tSpeed_CarType_Utils, NULL, NULL, NULL}
};
static const MappingScope scope_24 = {
  "CruiseControl::CruiseSpeedMgt/ CruiseSpeedMgt_CruiseControlpwlinear::LimiterUnSymmetrical 1/",
  scope_24_entries, 2,
};

/* CruiseControl::CruiseSpeedMgt/ CruiseSpeedMgt_CruiseControl */
static const MappingEntry scope_23_entries[5] = {
  /* 0 */ { MAP_LOCAL, "@kcg7", NULL, sizeof(kcg_bool), offsetof(outC_CruiseSpeedMgt_CruiseControl, init), &_Type_kcg_bool_Utils, NULL, NULL, NULL},
  /* 1 */ { MAP_OUTPUT, "CruiseSpeed", NULL, sizeof(tSpeed_CarType), offsetof(outC_CruiseSpeedMgt_CruiseControl, CruiseSpeed), &_Type_tSpeed_CarType_Utils, NULL, NULL, NULL},
  /* 2 */ { MAP_IF, "IfBlock1:", NULL, 0, 0, NULL, NULL, NULL, &scope_25},
  /* 3 */ { MAP_LOCAL, "_L42", NULL, sizeof(tSpeed_CarType), offsetof(outC_CruiseSpeedMgt_CruiseControl, CruiseSpeed), &_Type_tSpeed_CarType_Utils, NULL, NULL, NULL},
  /* 4 */ { MAP_EXPANDED, "pwlinear::LimiterUnSymmetrical 1", NULL, 0, 0, NULL, NULL, NULL, &scope_24}
};
static const MappingScope scope_23 = {
  "CruiseControl::CruiseSpeedMgt/ CruiseSpeedMgt_CruiseControl",
  scope_23_entries, 5,
};

/* CruiseControl::CruiseControl/ CruiseControl_CruiseControlSM1:Enabled:<1 */
static const MappingEntry scope_22_entries[1] = {
  /* 0 */ { MAP_TRANSITION, ">:", NULL, 0, 0, NULL, NULL, NULL, NULL}
};
static const MappingScope scope_22 = {
  "CruiseControl::CruiseControl/ CruiseControl_CruiseControlSM1:Enabled:<1",
  scope_22_entries, 1,
};

/* CruiseControl::CruiseControl/ CruiseControl_CruiseControlSM1:Enabled:SM2:Interrupt:<1 */
static const MappingEntry scope_21_entries[1] = {
  /* 0 */ { MAP_TRANSITION, ">:", NULL, 0, 0, NULL, NULL, NULL, NULL}
};
static const MappingScope scope_21 = {
  "CruiseControl::CruiseControl/ CruiseControl_CruiseControlSM1:Enabled:SM2:Interrupt:<1",
  scope_21_entries, 1,
};

/* CruiseControl::CruiseControl/ CruiseControl_CruiseControlSM1:Enabled:SM2:Interrupt: */
static const MappingEntry scope_20_entries[1] = {
  /* 0 */ { MAP_FORK, "<1", NULL, 0, 0, NULL, NULL, NULL, &scope_21}
};
static const MappingScope scope_20 = {
  "CruiseControl::CruiseControl/ CruiseControl_CruiseControlSM1:Enabled:SM2:Interrupt:",
  scope_20_entries, 1,
};

/* CruiseControl::CruiseControl/ CruiseControl_CruiseControlSM1:Enabled:SM2:Active:<1 */
static const MappingEntry scope_19_entries[1] = {
  /* 0 */ { MAP_TRANSITION, ">:", NULL, 0, 0, NULL, NULL, NULL, NULL}
};
static const MappingScope scope_19 = {
  "CruiseControl::CruiseControl/ CruiseControl_CruiseControlSM1:Enabled:SM2:Active:<1",
  scope_19_entries, 1,
};

/* CruiseControl::CruiseControl/ CruiseControl_CruiseControlSM1:Enabled:SM2:Active:SM3:StandBy:<1 */
static const MappingEntry scope_18_entries[1] = {
  /* 0 */ { MAP_TRANSITION, ">:", NULL, 0, 0, NULL, NULL, NULL, NULL}
};
static const MappingScope scope_18 = {
  "CruiseControl::CruiseControl/ CruiseControl_CruiseControlSM1:Enabled:SM2:Active:SM3:StandBy:<1",
  scope_18_entries, 1,
};

/* CruiseControl::CruiseControl/ CruiseControl_CruiseControlSM1:Enabled:SM2:Active:SM3:StandBy: */
static const MappingEntry scope_17_entries[1] = {
  /* 0 */ { MAP_FORK, "<1", NULL, 0, 0, NULL, NULL, NULL, &scope_18}
};
static const MappingScope scope_17 = {
  "CruiseControl::CruiseControl/ CruiseControl_CruiseControlSM1:Enabled:SM2:Active:SM3:StandBy:",
  scope_17_entries, 1,
};

/* CruiseControl::CruiseControl/ CruiseControl_CruiseControlSM1:Enabled:SM2:Active:SM3:On:<1 */
static const MappingEntry scope_16_entries[1] = {
  /* 0 */ { MAP_TRANSITION, ">:", NULL, 0, 0, NULL, NULL, NULL, NULL}
};
static const MappingScope scope_16 = {
  "CruiseControl::CruiseControl/ CruiseControl_CruiseControlSM1:Enabled:SM2:Active:SM3:On:<1",
  scope_16_entries, 1,
};

/* CruiseControl::CruiseControl/ CruiseControl_CruiseControlSM1:Enabled:SM2:Active:SM3:On: */
static const MappingEntry scope_15_entries[2] = {
  /* 0 */ { MAP_FORK, "<1", NULL, 0, 0, NULL, NULL, NULL, &scope_16},
  /* 1 */ { MAP_INSTANCE, "CruiseControl::CruiseRegulation 2", NULL, sizeof(outC_CruiseRegulation_CruiseControl), (int)&outputs_ctx.Context_2, NULL, NULL, NULL, &scope_28}
};
static const MappingScope scope_15 = {
  "CruiseControl::CruiseControl/ CruiseControl_CruiseControlSM1:Enabled:SM2:Active:SM3:On:",
  scope_15_entries, 2,
};

/* CruiseControl::CruiseControl/ CruiseControl_CruiseControlSM1:Enabled:SM2:Active:SM3: */
static const MappingEntry scope_12_entries[2] = {
  /* 0 */ { MAP_STATE, "On:", NULL, 0, 0, NULL, NULL, NULL, &scope_15},
  /* 1 */ { MAP_STATE, "StandBy:", NULL, 0, 0, NULL, NULL, NULL, &scope_17}
};
static const MappingScope scope_12 = {
  "CruiseControl::CruiseControl/ CruiseControl_CruiseControlSM1:Enabled:SM2:Active:SM3:",
  scope_12_entries, 2,
};

/* CruiseControl::CruiseControl/ CruiseControl_CruiseControlSM1:Enabled:SM2:Active: */
static const MappingEntry scope_11_entries[2] = {
  /* 0 */ { MAP_FORK, "<1", NULL, 0, 0, NULL, NULL, NULL, &scope_19},
  /* 1 */ { MAP_AUTOMATON, "SM3:", NULL, 0, 0, NULL, NULL, NULL, &scope_12}
};
static const MappingScope scope_11 = {
  "CruiseControl::CruiseControl/ CruiseControl_CruiseControlSM1:Enabled:SM2:Active:",
  scope_11_entries, 2,
};

/* CruiseControl::CruiseControl/ CruiseControl_CruiseControlSM1:Enabled:SM2: */
static const MappingEntry scope_8_entries[2] = {
  /* 0 */ { MAP_STATE, "Active:", NULL, 0, 0, NULL, NULL, NULL, &scope_11},
  /* 1 */ { MAP_STATE, "Interrupt:", NULL, 0, 0, NULL, NULL, NULL, &scope_20}
};
static const MappingScope scope_8 = {
  "CruiseControl::CruiseControl/ CruiseControl_CruiseControlSM1:Enabled:SM2:",
  scope_8_entries, 2,
};

/* CruiseControl::CruiseControl/ CruiseControl_CruiseControlSM1:Enabled: */
static const MappingEntry scope_7_entries[3] = {
  /* 0 */ { MAP_FORK, "<1", NULL, 0, 0, NULL, NULL, NULL, &scope_22},
  /* 1 */ { MAP_INSTANCE, "CruiseControl::CruiseSpeedMgt 1", NULL, sizeof(outC_CruiseSpeedMgt_CruiseControl), (int)&outputs_ctx.Context_1, NULL, NULL, NULL, &scope_23},
  /* 2 */ { MAP_AUTOMATON, "SM2:", NULL, 0, 0, NULL, NULL, NULL, &scope_8}
};
static const MappingScope scope_7 = {
  "CruiseControl::CruiseControl/ CruiseControl_CruiseControlSM1:Enabled:",
  scope_7_entries, 3,
};

/* CruiseControl::CruiseControl/ CruiseControl_CruiseControlSM1:Off:<1 */
static const MappingEntry scope_6_entries[1] = {
  /* 0 */ { MAP_TRANSITION, ">:", NULL, 0, 0, NULL, NULL, NULL, NULL}
};
static const MappingScope scope_6 = {
  "CruiseControl::CruiseControl/ CruiseControl_CruiseControlSM1:Off:<1",
  scope_6_entries, 1,
};

/* CruiseControl::CruiseControl/ CruiseControl_CruiseControlSM1:Off: */
static const MappingEntry scope_5_entries[1] = {
  /* 0 */ { MAP_FORK, "<1", NULL, 0, 0, NULL, NULL, NULL, &scope_6}
};
static const MappingScope scope_5 = {
  "CruiseControl::CruiseControl/ CruiseControl_CruiseControlSM1:Off:",
  scope_5_entries, 1,
};

/* CruiseControl::CruiseControl/ CruiseControl_CruiseControlSM1: */
static const MappingEntry scope_2_entries[2] = {
  /* 0 */ { MAP_STATE, "Enabled:", NULL, 0, 0, NULL, NULL, NULL, &scope_7},
  /* 1 */ { MAP_STATE, "Off:", NULL, 0, 0, NULL, NULL, NULL, &scope_5}
};
static const MappingScope scope_2 = {
  "CruiseControl::CruiseControl/ CruiseControl_CruiseControlSM1:",
  scope_2_entries, 2,
};

/* CruiseControl::CruiseControl/ CruiseControl_CruiseControl */
static const MappingEntry scope_1_entries[16] = {
  /* 0 */ { MAP_LOCAL, "@kcg0", NULL, sizeof(kcg_bool), (int)&outputs_ctx.init2, &_Type_kcg_bool_Utils, NULL, NULL /* CLOCK SM2_state_act_SM1_Enabled */, NULL},
  /* 1 */ { MAP_LOCAL, "@kcg1", NULL, sizeof(kcg_bool), (int)&outputs_ctx.init1, &_Type_kcg_bool_Utils, NULL, NULL /* CLOCK SM1_state_act */, NULL},
  /* 2 */ { MAP_LOCAL, "@kcg2", NULL, sizeof(kcg_bool), (int)&outputs_ctx.init, &_Type_kcg_bool_Utils, NULL, NULL, NULL},
  /* 3 */ { MAP_INPUT, "Accel", NULL, sizeof(tPercent_CarType), (int)&inputs_ctx.Accel, &_Type_tPercent_CarType_Utils, NULL, NULL, NULL},
  /* 4 */ { MAP_INPUT, "Brake", NULL, sizeof(tPercent_CarType), (int)&inputs_ctx.Brake, &_Type_tPercent_CarType_Utils, NULL, NULL, NULL},
  /* 5 */ { MAP_OUTPUT, "CruiseSpeed", NULL, sizeof(tSpeed_CarType), (int)&outputs_ctx.CruiseSpeed, &_Type_tSpeed_CarType_Utils, NULL, NULL, NULL},
  /* 6 */ { MAP_OUTPUT, "CruiseState", NULL, sizeof(tCruiseState_CruiseControl), (int)&outputs_ctx.CruiseState, &_Type_tCruiseState_CruiseControl_Utils, NULL, NULL, NULL},
  /* 7 */ { MAP_INPUT, "Off", NULL, sizeof(kcg_bool), (int)&inputs_ctx.Off, &_Type_kcg_bool_Utils, NULL, NULL, NULL},
  /* 8 */ { MAP_INPUT, "On", NULL, sizeof(kcg_bool), (int)&inputs_ctx.On, &_Type_kcg_bool_Utils, NULL, NULL, NULL},
  /* 9 */ { MAP_INPUT, "QuickAccel", NULL, sizeof(kcg_bool), (int)&inputs_ctx.QuickAccel, &_Type_kcg_bool_Utils, NULL, NULL, NULL},
  /* 10 */ { MAP_INPUT, "QuickDecel", NULL, sizeof(kcg_bool), (int)&inputs_ctx.QuickDecel, &_Type_kcg_bool_Utils, NULL, NULL, NULL},
  /* 11 */ { MAP_INPUT, "Resume", NULL, sizeof(kcg_bool), (int)&inputs_ctx.Resume, &_Type_kcg_bool_Utils, NULL, NULL, NULL},
  /* 12 */ { MAP_AUTOMATON, "SM1:", NULL, 0, 0, NULL, NULL, NULL, &scope_2},
  /* 13 */ { MAP_INPUT, "Set", NULL, sizeof(kcg_bool), (int)&inputs_ctx.Set, &_Type_kcg_bool_Utils, NULL, NULL, NULL},
  /* 14 */ { MAP_INPUT, "Speed", NULL, sizeof(tSpeed_CarType), (int)&inputs_ctx.Speed, &_Type_tSpeed_CarType_Utils, NULL, NULL, NULL},
  /* 15 */ { MAP_OUTPUT, "ThrottleCmd", NULL, sizeof(tPercent_CarType), (int)&outputs_ctx.ThrottleCmd, &_Type_tPercent_CarType_Utils, NULL, NULL, NULL}
};
static const MappingScope scope_1 = {
  "CruiseControl::CruiseControl/ CruiseControl_CruiseControl",
  scope_1_entries, 16,
};

/*  */
static const MappingEntry scope_0_entries[1] = {
  /* 0 */ { MAP_ROOT, "CruiseControl::CruiseControl", NULL, 0, 0, NULL, NULL, NULL, &scope_1}
};
static const MappingScope scope_0 = {
  "",
  scope_0_entries, 1,
};

const MappingScope* pTop = &scope_0;

