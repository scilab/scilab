#ifndef CRUISECONTROL_TYPES_CONVERTION
#define CRUISECONTROL_TYPES_CONVERTION

#include "NewSmuTypes.h"

/****************************************************************
 ** SSM_ST_SM1 
 ****************************************************************/
extern int SSM_ST_SM1_to_string(const void *pValue, int (*pfnStrAppend)(const char *str, void *pData), void *pData);
extern int check_SSM_ST_SM1_string(const char *str, const char** endptr);
extern int string_to_SSM_ST_SM1(const char *str, void* pValue, const char** endptr);
extern int is_SSM_ST_SM1_allow_double_convertion();
extern int SSM_ST_SM1_to_double(const void* pValue, double *nValue);
extern int get_SSM_ST_SM1_signature(int (*pfnStrAppend)(const char *str, void *pData), void *pData);
extern int compare_SSM_ST_SM1(int *nStatus, const void *pValue1, const void *pValue2, void *pData);
extern int set_SSM_ST_SM1_default_value(void *pValue);
extern TypeUtils _Type_SSM_ST_SM1_Utils;

/****************************************************************
 ** SSM_TR_SM1 
 ****************************************************************/
extern int SSM_TR_SM1_to_string(const void *pValue, int (*pfnStrAppend)(const char *str, void *pData), void *pData);
extern int check_SSM_TR_SM1_string(const char *str, const char** endptr);
extern int string_to_SSM_TR_SM1(const char *str, void* pValue, const char** endptr);
extern int is_SSM_TR_SM1_allow_double_convertion();
extern int SSM_TR_SM1_to_double(const void* pValue, double *nValue);
extern int get_SSM_TR_SM1_signature(int (*pfnStrAppend)(const char *str, void *pData), void *pData);
extern int compare_SSM_TR_SM1(int *nStatus, const void *pValue1, const void *pValue2, void *pData);
extern int set_SSM_TR_SM1_default_value(void *pValue);
extern TypeUtils _Type_SSM_TR_SM1_Utils;

/****************************************************************
 ** SSM_ST_SM2_SM1_Enabled 
 ****************************************************************/
extern int SSM_ST_SM2_SM1_Enabled_to_string(const void *pValue, int (*pfnStrAppend)(const char *str, void *pData), void *pData);
extern int check_SSM_ST_SM2_SM1_Enabled_string(const char *str, const char** endptr);
extern int string_to_SSM_ST_SM2_SM1_Enabled(const char *str, void* pValue, const char** endptr);
extern int is_SSM_ST_SM2_SM1_Enabled_allow_double_convertion();
extern int SSM_ST_SM2_SM1_Enabled_to_double(const void* pValue, double *nValue);
extern int get_SSM_ST_SM2_SM1_Enabled_signature(int (*pfnStrAppend)(const char *str, void *pData), void *pData);
extern int compare_SSM_ST_SM2_SM1_Enabled(int *nStatus, const void *pValue1, const void *pValue2, void *pData);
extern int set_SSM_ST_SM2_SM1_Enabled_default_value(void *pValue);
extern TypeUtils _Type_SSM_ST_SM2_SM1_Enabled_Utils;

/****************************************************************
 ** SSM_TR_SM2_SM1_Enabled 
 ****************************************************************/
extern int SSM_TR_SM2_SM1_Enabled_to_string(const void *pValue, int (*pfnStrAppend)(const char *str, void *pData), void *pData);
extern int check_SSM_TR_SM2_SM1_Enabled_string(const char *str, const char** endptr);
extern int string_to_SSM_TR_SM2_SM1_Enabled(const char *str, void* pValue, const char** endptr);
extern int is_SSM_TR_SM2_SM1_Enabled_allow_double_convertion();
extern int SSM_TR_SM2_SM1_Enabled_to_double(const void* pValue, double *nValue);
extern int get_SSM_TR_SM2_SM1_Enabled_signature(int (*pfnStrAppend)(const char *str, void *pData), void *pData);
extern int compare_SSM_TR_SM2_SM1_Enabled(int *nStatus, const void *pValue1, const void *pValue2, void *pData);
extern int set_SSM_TR_SM2_SM1_Enabled_default_value(void *pValue);
extern TypeUtils _Type_SSM_TR_SM2_SM1_Enabled_Utils;

/****************************************************************
 ** SSM_ST_SM3_SM1_Enabled_SM2_Active 
 ****************************************************************/
extern int SSM_ST_SM3_SM1_Enabled_SM2_Active_to_string(const void *pValue, int (*pfnStrAppend)(const char *str, void *pData), void *pData);
extern int check_SSM_ST_SM3_SM1_Enabled_SM2_Active_string(const char *str, const char** endptr);
extern int string_to_SSM_ST_SM3_SM1_Enabled_SM2_Active(const char *str, void* pValue, const char** endptr);
extern int is_SSM_ST_SM3_SM1_Enabled_SM2_Active_allow_double_convertion();
extern int SSM_ST_SM3_SM1_Enabled_SM2_Active_to_double(const void* pValue, double *nValue);
extern int get_SSM_ST_SM3_SM1_Enabled_SM2_Active_signature(int (*pfnStrAppend)(const char *str, void *pData), void *pData);
extern int compare_SSM_ST_SM3_SM1_Enabled_SM2_Active(int *nStatus, const void *pValue1, const void *pValue2, void *pData);
extern int set_SSM_ST_SM3_SM1_Enabled_SM2_Active_default_value(void *pValue);
extern TypeUtils _Type_SSM_ST_SM3_SM1_Enabled_SM2_Active_Utils;

/****************************************************************
 ** SSM_TR_SM3_SM1_Enabled_SM2_Active 
 ****************************************************************/
extern int SSM_TR_SM3_SM1_Enabled_SM2_Active_to_string(const void *pValue, int (*pfnStrAppend)(const char *str, void *pData), void *pData);
extern int check_SSM_TR_SM3_SM1_Enabled_SM2_Active_string(const char *str, const char** endptr);
extern int string_to_SSM_TR_SM3_SM1_Enabled_SM2_Active(const char *str, void* pValue, const char** endptr);
extern int is_SSM_TR_SM3_SM1_Enabled_SM2_Active_allow_double_convertion();
extern int SSM_TR_SM3_SM1_Enabled_SM2_Active_to_double(const void* pValue, double *nValue);
extern int get_SSM_TR_SM3_SM1_Enabled_SM2_Active_signature(int (*pfnStrAppend)(const char *str, void *pData), void *pData);
extern int compare_SSM_TR_SM3_SM1_Enabled_SM2_Active(int *nStatus, const void *pValue1, const void *pValue2, void *pData);
extern int set_SSM_TR_SM3_SM1_Enabled_SM2_Active_default_value(void *pValue);
extern TypeUtils _Type_SSM_TR_SM3_SM1_Enabled_SM2_Active_Utils;

/****************************************************************
 ** kcg_real 
 ****************************************************************/
extern int kcg_real_to_string(const void *pValue, int (*pfnStrAppend)(const char *str, void *pData), void *pData);
extern int check_kcg_real_string(const char *str, const char** endptr);
extern int string_to_kcg_real(const char *str, void* pValue, const char** endptr);
extern int is_kcg_real_allow_double_convertion();
extern int kcg_real_to_double(const void* pValue, double *nValue);
extern int get_kcg_real_signature(int (*pfnStrAppend)(const char *str, void *pData), void *pData);
extern int compare_kcg_real(int *nStatus, const void *pValue1, const void *pValue2, void *pData);
extern int set_kcg_real_default_value(void *pValue);
extern TypeUtils _Type_kcg_real_Utils;

/****************************************************************
 ** kcg_bool 
 ****************************************************************/
extern int kcg_bool_to_string(const void *pValue, int (*pfnStrAppend)(const char *str, void *pData), void *pData);
extern int check_kcg_bool_string(const char *str, const char** endptr);
extern int string_to_kcg_bool(const char *str, void* pValue, const char** endptr);
extern int is_kcg_bool_allow_double_convertion();
extern int kcg_bool_to_double(const void* pValue, double *nValue);
extern int get_kcg_bool_signature(int (*pfnStrAppend)(const char *str, void *pData), void *pData);
extern int compare_kcg_bool(int *nStatus, const void *pValue1, const void *pValue2, void *pData);
extern int set_kcg_bool_default_value(void *pValue);
extern TypeUtils _Type_kcg_bool_Utils;

/****************************************************************
 ** kcg_char 
 ****************************************************************/
extern int kcg_char_to_string(const void *pValue, int (*pfnStrAppend)(const char *str, void *pData), void *pData);
extern int check_kcg_char_string(const char *str, const char** endptr);
extern int string_to_kcg_char(const char *str, void* pValue, const char** endptr);
extern int is_kcg_char_allow_double_convertion();
extern int kcg_char_to_double(const void* pValue, double *nValue);
extern int get_kcg_char_signature(int (*pfnStrAppend)(const char *str, void *pData), void *pData);
extern int compare_kcg_char(int *nStatus, const void *pValue1, const void *pValue2, void *pData);
extern int set_kcg_char_default_value(void *pValue);
extern TypeUtils _Type_kcg_char_Utils;

/****************************************************************
 ** kcg_int 
 ****************************************************************/
extern int kcg_int_to_string(const void *pValue, int (*pfnStrAppend)(const char *str, void *pData), void *pData);
extern int check_kcg_int_string(const char *str, const char** endptr);
extern int string_to_kcg_int(const char *str, void* pValue, const char** endptr);
extern int is_kcg_int_allow_double_convertion();
extern int kcg_int_to_double(const void* pValue, double *nValue);
extern int get_kcg_int_signature(int (*pfnStrAppend)(const char *str, void *pData), void *pData);
extern int compare_kcg_int(int *nStatus, const void *pValue1, const void *pValue2, void *pData);
extern int set_kcg_int_default_value(void *pValue);
extern TypeUtils _Type_kcg_int_Utils;

/****************************************************************
 ** tPercent_CarType 
 ****************************************************************/
extern int tPercent_CarType_to_string(const void *pValue, int (*pfnStrAppend)(const char *str, void *pData), void *pData);
extern int check_tPercent_CarType_string(const char *str, const char** endptr);
extern int string_to_tPercent_CarType(const char *str, void* pValue, const char** endptr);
extern int is_tPercent_CarType_allow_double_convertion();
extern int tPercent_CarType_to_double(const void* pValue, double *nValue);
extern int get_tPercent_CarType_signature(int (*pfnStrAppend)(const char *str, void *pData), void *pData);
extern int compare_tPercent_CarType(int *nStatus, const void *pValue1, const void *pValue2, void *pData);
extern int set_tPercent_CarType_default_value(void *pValue);
extern TypeUtils _Type_tPercent_CarType_Utils;

/****************************************************************
 ** tSpeed_CarType 
 ****************************************************************/
extern int tSpeed_CarType_to_string(const void *pValue, int (*pfnStrAppend)(const char *str, void *pData), void *pData);
extern int check_tSpeed_CarType_string(const char *str, const char** endptr);
extern int string_to_tSpeed_CarType(const char *str, void* pValue, const char** endptr);
extern int is_tSpeed_CarType_allow_double_convertion();
extern int tSpeed_CarType_to_double(const void* pValue, double *nValue);
extern int get_tSpeed_CarType_signature(int (*pfnStrAppend)(const char *str, void *pData), void *pData);
extern int compare_tSpeed_CarType(int *nStatus, const void *pValue1, const void *pValue2, void *pData);
extern int set_tSpeed_CarType_default_value(void *pValue);
extern TypeUtils _Type_tSpeed_CarType_Utils;

/****************************************************************
 ** tCruiseState_CruiseControl 
 ****************************************************************/
extern int tCruiseState_CruiseControl_to_string(const void *pValue, int (*pfnStrAppend)(const char *str, void *pData), void *pData);
extern int check_tCruiseState_CruiseControl_string(const char *str, const char** endptr);
extern int string_to_tCruiseState_CruiseControl(const char *str, void* pValue, const char** endptr);
extern int is_tCruiseState_CruiseControl_allow_double_convertion();
extern int tCruiseState_CruiseControl_to_double(const void* pValue, double *nValue);
extern int get_tCruiseState_CruiseControl_signature(int (*pfnStrAppend)(const char *str, void *pData), void *pData);
extern int compare_tCruiseState_CruiseControl(int *nStatus, const void *pValue1, const void *pValue2, void *pData);
extern int set_tCruiseState_CruiseControl_default_value(void *pValue);
extern TypeUtils _Type_tCruiseState_CruiseControl_Utils;


#endif /*CRUISECONTROL_TYPES_CONVERTION */
