#ifndef CRUISECONTROL_CRUISECONTROL_SCSIM_MAPPING
#define CRUISECONTROL_CRUISECONTROL_SCSIM_MAPPING

#include "SmuTypes.h"
#include "CruiseControl_CruiseControl_type.h"

void _SCSIM_Mapping_Create();
static ControlUtils _SCSIM_BoolEntity_Control_Utils;
#include "CruiseControl_CruiseControl.h"
void _SCSIM_Mapping_CruiseControl_CruiseControl();

void* _SCSIM_Get_CruiseControl_CruiseControl_Handle(void* pInstance, int nHandleIdent, int* pIteratorFilter, int nSize);

#include "CruiseSpeedMgt_CruiseControl.h"
void _SCSIM_Mapping_CruiseSpeedMgt_CruiseControl(const char* pszPath, const char* pszInstanceName, int nHandleIdent, int nClockHandleIdent, int (*pfnClockActive)(void*));
void* _SCSIM_Get_CruiseSpeedMgt_CruiseControl_Handle(void* pInstance, int nHandleIdent, int* pIteratorFilter, int nSize);

#include "CruiseRegulation_CruiseControl.h"
void _SCSIM_Mapping_CruiseRegulation_CruiseControl(const char* pszPath, const char* pszInstanceName, int nHandleIdent, int nClockHandleIdent, int (*pfnClockActive)(void*));
void* _SCSIM_Get_CruiseRegulation_CruiseControl_Handle(void* pInstance, int nHandleIdent, int* pIteratorFilter, int nSize);


#endif /*CRUISECONTROL_CRUISECONTROL_SCSIM_MAPPING */
