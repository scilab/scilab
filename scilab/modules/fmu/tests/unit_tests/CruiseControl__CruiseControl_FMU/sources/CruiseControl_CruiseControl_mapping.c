#include "CruiseControl_CruiseControl_mapping.h"
#include "CruiseControl_CruiseControl_interface.h"
#include "kcg_sensors.h"

/****************************************************************
 ** Boolean entity activation
 ****************************************************************/
static ControlUtils _SCSIM_BoolEntity_Control_Utils = {_SCSIM_BoolEntity_is_active};
/****************************************************************
 ** Mapping creation function
 ****************************************************************/
void _SCSIM_Mapping_Create() {
	_SCSIM_Mapping_CruiseControl_CruiseControl();
	pSimulator->m_pfnFinalizeMapping(pSimulator);
}

/****************************************************************
 ** CruiseControl::CruiseControl/ mapping function
 ****************************************************************/
void _SCSIM_Mapping_CruiseControl_CruiseControl() {
	pSimulator->m_pfnSetRoot(pSimulator, "CruiseControl::CruiseControl/", &outputs_ctx, _SCSIM_Get_CruiseControl_CruiseControl_Handle);
	pSimulator->m_pfnPushStateMachine(pSimulator, "SM1");
	pSimulator->m_pfnPushState(pSimulator, "Off", 0, -1);
	pSimulator->m_pfnPushStrongTransition(pSimulator, "1", 0, -1);
	pSimulator->m_pfnPopStrongTransition(pSimulator);
	pSimulator->m_pfnPopState(pSimulator);
	pSimulator->m_pfnPushState(pSimulator, "Enabled", 0, -1);
	_SCSIM_Mapping_CruiseSpeedMgt_CruiseControl("CruiseControl::CruiseSpeedMgt", "1", 1, 0, 0);
	pSimulator->m_pfnPushStateMachine(pSimulator, "SM2");
	pSimulator->m_pfnPushState(pSimulator, "Active", 0, -1);
	pSimulator->m_pfnPushStateMachine(pSimulator, "SM3");
	pSimulator->m_pfnPushState(pSimulator, "On", 0, -1);
	_SCSIM_Mapping_CruiseRegulation_CruiseControl("CruiseControl::CruiseRegulation", "2", 2, 0, 0);
	pSimulator->m_pfnPushStrongTransition(pSimulator, "1", 0, -1);
	pSimulator->m_pfnPopStrongTransition(pSimulator);
	pSimulator->m_pfnPopState(pSimulator);
	pSimulator->m_pfnPushState(pSimulator, "StandBy", 0, -1);
	pSimulator->m_pfnPushStrongTransition(pSimulator, "1", 0, -1);
	pSimulator->m_pfnPopStrongTransition(pSimulator);
	pSimulator->m_pfnPopState(pSimulator);
	pSimulator->m_pfnPopStateMachine(pSimulator);
	pSimulator->m_pfnPushStrongTransition(pSimulator, "1", 0, -1);
	pSimulator->m_pfnPopStrongTransition(pSimulator);
	pSimulator->m_pfnPopState(pSimulator);
	pSimulator->m_pfnPushState(pSimulator, "Interrupt", 0, -1);
	pSimulator->m_pfnPushStrongTransition(pSimulator, "1", 0, -1);
	pSimulator->m_pfnPopStrongTransition(pSimulator);
	pSimulator->m_pfnPopState(pSimulator);
	pSimulator->m_pfnPopStateMachine(pSimulator);
	pSimulator->m_pfnPushStrongTransition(pSimulator, "1", 0, -1);
	pSimulator->m_pfnPopStrongTransition(pSimulator);
	pSimulator->m_pfnPopState(pSimulator);
	pSimulator->m_pfnPopStateMachine(pSimulator);
	pSimulator->m_pfnAddOutput(pSimulator, "CruiseSpeed", &_SCSIM_tSpeed_CarType_Utils, 3, valid, 0, 0);
	pSimulator->m_pfnAddOutput(pSimulator, "ThrottleCmd", &_SCSIM_tPercent_CarType_Utils, 4, valid, 0, 0);
	pSimulator->m_pfnAddOutput(pSimulator, "CruiseState", &_SCSIM_tCruiseState_CruiseControl_Utils, 5, valid, 0, 0);
	pSimulator->m_pfnAddInput(pSimulator, "On", &_SCSIM_kcg_bool_Utils, 6, valid, 0, 0);
	pSimulator->m_pfnAddInput(pSimulator, "Off", &_SCSIM_kcg_bool_Utils, 7, valid, 0, 0);
	pSimulator->m_pfnAddInput(pSimulator, "Resume", &_SCSIM_kcg_bool_Utils, 8, valid, 0, 0);
	pSimulator->m_pfnAddInput(pSimulator, "Set", &_SCSIM_kcg_bool_Utils, 9, valid, 0, 0);
	pSimulator->m_pfnAddInput(pSimulator, "QuickAccel", &_SCSIM_kcg_bool_Utils, 10, valid, 0, 0);
	pSimulator->m_pfnAddInput(pSimulator, "QuickDecel", &_SCSIM_kcg_bool_Utils, 11, valid, 0, 0);
	pSimulator->m_pfnAddInput(pSimulator, "Accel", &_SCSIM_tPercent_CarType_Utils, 12, valid, 0, 0);
	pSimulator->m_pfnAddInput(pSimulator, "Brake", &_SCSIM_tPercent_CarType_Utils, 13, valid, 0, 0);
	pSimulator->m_pfnAddInput(pSimulator, "Speed", &_SCSIM_tSpeed_CarType_Utils, 14, valid, 0, 0);
}

void* _SCSIM_Get_CruiseControl_CruiseControl_Handle(void* pInstance, int nHandleIdent, int* pIteratorFilter, int nSize) {
	switch (nHandleIdent) {
		case 1:
			return &(outputs_ctx.Context_1);
		case 2:
			return &(outputs_ctx.Context_2);
		case 3:
			return &(outputs_ctx.CruiseSpeed);
		case 4:
			return &(outputs_ctx.ThrottleCmd);
		case 5:
			return &(outputs_ctx.CruiseState);
		case 6:
			return &(inputs_ctx.On);
		case 7:
			return &(inputs_ctx.Off);
		case 8:
			return &(inputs_ctx.Resume);
		case 9:
			return &(inputs_ctx.Set);
		case 10:
			return &(inputs_ctx.QuickAccel);
		case 11:
			return &(inputs_ctx.QuickDecel);
		case 12:
			return &(inputs_ctx.Accel);
		case 13:
			return &(inputs_ctx.Brake);
		case 14:
			return &(inputs_ctx.Speed);
		default:
			break;
	}
	return 0;
}

/****************************************************************
 ** CruiseControl::CruiseSpeedMgt/ mapping function
 ****************************************************************/
void _SCSIM_Mapping_CruiseSpeedMgt_CruiseControl(const char* pszPath, const char* pszInstanceName, int nHandleIdent, int nClockHandleIdent, int (*pfnClockActive)(void*)) {
	pSimulator->m_pfnPushInstance(pSimulator, pszPath, pszInstanceName, nHandleIdent, _SCSIM_Get_CruiseSpeedMgt_CruiseControl_Handle, nClockHandleIdent, pfnClockActive);
	/*<< Inlined pwlinear::LimiterUnSymmetrical*/
	pSimulator->m_pfnPushInstance(pSimulator, "pwlinear::LimiterUnSymmetrical", "1", 0, 0, 0, 0);
	pSimulator->m_pfnAddOutput(pSimulator, "LUS_Output", &_SCSIM_tSpeed_CarType_Utils, 15, valid, 0, 0);
	pSimulator->m_pfnAddLocal(pSimulator, "_L9", &_SCSIM_tSpeed_CarType_Utils, 15, valid, 0, 0);
	pSimulator->m_pfnPopInstance(pSimulator);
	/*>>*/
	pSimulator->m_pfnPushActivateIf(pSimulator, "IfBlock1");
	pSimulator->m_pfnPushBranch(pSimulator, "else", 0, -1);
	pSimulator->m_pfnPopBranch(pSimulator);
	pSimulator->m_pfnPushBranch(pSimulator, "then", 0, -1);
	pSimulator->m_pfnPopBranch(pSimulator);
	pSimulator->m_pfnPopActivateIf(pSimulator);
	pSimulator->m_pfnAddLocal(pSimulator, "_L42", &_SCSIM_tSpeed_CarType_Utils, 15, valid, 0, 0);
	pSimulator->m_pfnAddOutput(pSimulator, "CruiseSpeed", &_SCSIM_tSpeed_CarType_Utils, 15, valid, 0, 0);
	pSimulator->m_pfnPopInstance(pSimulator);
}

void* _SCSIM_Get_CruiseSpeedMgt_CruiseControl_Handle(void* pInstance, int nHandleIdent, int* pIteratorFilter, int nSize) {
	outC_CruiseSpeedMgt_CruiseControl* pContext = (outC_CruiseSpeedMgt_CruiseControl*)pInstance;
	switch (nHandleIdent) {
		case 15:
			return &((*pContext).CruiseSpeed);
		default:
			break;
	}
	return 0;
}

/****************************************************************
 ** CruiseControl::CruiseRegulation/ mapping function
 ****************************************************************/
void _SCSIM_Mapping_CruiseRegulation_CruiseControl(const char* pszPath, const char* pszInstanceName, int nHandleIdent, int nClockHandleIdent, int (*pfnClockActive)(void*)) {
	pSimulator->m_pfnPushInstance(pSimulator, pszPath, pszInstanceName, nHandleIdent, _SCSIM_Get_CruiseRegulation_CruiseControl_Handle, nClockHandleIdent, pfnClockActive);
	pSimulator->m_pfnAddLocal(pSimulator, "_L10", &_SCSIM_kcg_real_Utils, 16, valid, 0, 0);
	pSimulator->m_pfnAddLocal(pSimulator, "_L14", &_SCSIM_kcg_bool_Utils, 17, valid, 0, 0);
	/*<< Inlined linear::Gain*/
	pSimulator->m_pfnPushInstance(pSimulator, "linear::Gain", "2", 0, 0, 0, 0);
	pSimulator->m_pfnPopInstance(pSimulator);
	/*>>*/
	/*<< Inlined linear::Gain*/
	pSimulator->m_pfnPushInstance(pSimulator, "linear::Gain", "3", 0, 0, 0, 0);
	pSimulator->m_pfnAddInput(pSimulator, "G_Input", &_SCSIM_kcg_real_Utils, 16, valid, 0, 0);
	pSimulator->m_pfnAddLocal(pSimulator, "_L1", &_SCSIM_kcg_real_Utils, 16, valid, 0, 0);
	pSimulator->m_pfnPopInstance(pSimulator);
	/*>>*/
	pSimulator->m_pfnAddLocal(pSimulator, "_L13", &_SCSIM_tPercent_CarType_Utils, 18, valid, 0, 0);
	pSimulator->m_pfnAddOutput(pSimulator, "Throttle", &_SCSIM_tPercent_CarType_Utils, 18, valid, 0, 0);
	pSimulator->m_pfnPopInstance(pSimulator);
}

void* _SCSIM_Get_CruiseRegulation_CruiseControl_Handle(void* pInstance, int nHandleIdent, int* pIteratorFilter, int nSize) {
	outC_CruiseRegulation_CruiseControl* pContext = (outC_CruiseRegulation_CruiseControl*)pInstance;
	switch (nHandleIdent) {
		case 16:
			return &((*pContext)._L10);
		case 17:
			return &((*pContext)._L14);
		case 18:
			return &((*pContext).Throttle);
		default:
			break;
	}
	return 0;
}

