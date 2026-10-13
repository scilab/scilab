add_library(CruiseControl__CruiseControl_FMU SHARED
	CruiseControl_CruiseControl.c
	CruiseControl_CruiseControl_dll.c
	CruiseControl__CruiseControl_FMU.c
	CruiseRegulation_CruiseControl.c
	CruiseSpeedMgt_CruiseControl.c
	kcg_consts.c
	kcg_types.c
	SaturateThrottle_CruiseControl.c
)
set_target_properties(CruiseControl__CruiseControl_FMU PROPERTIES PREFIX "")

