#ifndef CRUISECONTROL_CRUISECONTROL_TYPES_CONVERTION
#define CRUISECONTROL_CRUISECONTROL_TYPES_CONVERTION

/****************************************************************
 ** Boolean entity activation
 ****************************************************************/
extern int _SCSIM_BoolEntity_is_active(void* pValue);

/****************************************************************
 ** Type utils declarations
 ****************************************************************/
extern TypeUtils _SCSIM_kcg_real_Utils;
extern TypeUtils _SCSIM_kcg_bool_Utils;
extern TypeUtils _SCSIM_kcg_char_Utils;
extern TypeUtils _SCSIM_kcg_int_Utils;
extern TypeUtils _SCSIM_tPercent_CarType_Utils;
extern TypeUtils _SCSIM_tSpeed_CarType_Utils;
extern TypeUtils _SCSIM_tCruiseState_CruiseControl_Utils;

/****************************************************************
 ** kcg_real
 ****************************************************************/
extern const char * kcg_real_to_string(const void* pValue);
extern int check_kcg_real_string(const char* strValue);
extern int string_to_kcg_real(const char* strValue, void* pValue);
int is_kcg_real_allow_double_convertion();
extern int kcg_real_to_double(double * nValue, const void*);
extern const char * get_kcg_real_signature();
extern int compare_kcg_real_type(int*, const char*, const void*);
#define kcg_real_filter_size 0
#define get_kcg_real_filter_utils 0
#define kcg_real_filter_values 0
/****************************************************************
 ** kcg_bool
 ****************************************************************/
extern const char * kcg_bool_to_string(const void* pValue);
extern int check_kcg_bool_string(const char* strValue);
extern int string_to_kcg_bool(const char* strValue, void* pValue);
int is_kcg_bool_allow_double_convertion();
extern int kcg_bool_to_double(double * nValue, const void*);
extern const char * get_kcg_bool_signature();
extern int compare_kcg_bool_type(int*, const char*, const void*);
#define kcg_bool_filter_size 0
#define get_kcg_bool_filter_utils 0
#define kcg_bool_filter_values 0
/****************************************************************
 ** kcg_char
 ****************************************************************/
extern const char * kcg_char_to_string(const void* pValue);
extern int check_kcg_char_string(const char* strValue);
extern int string_to_kcg_char(const char* strValue, void* pValue);
int is_kcg_char_allow_double_convertion();
extern int kcg_char_to_double(double * nValue, const void*);
extern const char * get_kcg_char_signature();
extern int compare_kcg_char_type(int*, const char*, const void*);
#define kcg_char_filter_size 0
#define get_kcg_char_filter_utils 0
#define kcg_char_filter_values 0
/****************************************************************
 ** kcg_int
 ****************************************************************/
extern const char * kcg_int_to_string(const void* pValue);
extern int check_kcg_int_string(const char* strValue);
extern int string_to_kcg_int(const char* strValue, void* pValue);
int is_kcg_int_allow_double_convertion();
extern int kcg_int_to_double(double * nValue, const void*);
extern const char * get_kcg_int_signature();
extern int compare_kcg_int_type(int*, const char*, const void*);
#define kcg_int_filter_size 0
#define get_kcg_int_filter_utils 0
#define kcg_int_filter_values 0
/****************************************************************
 ** tPercent_CarType
 ****************************************************************/
extern const char * tPercent_CarType_to_string(const void* pValue);
extern int check_tPercent_CarType_string(const char* strValue);
extern int string_to_tPercent_CarType(const char* strValue, void* pValue);
int is_tPercent_CarType_allow_double_convertion();
extern int tPercent_CarType_to_double(double * nValue, const void*);
#define get_tPercent_CarType_signature get_kcg_real_signature
#define compare_tPercent_CarType_type compare_kcg_real_type
#define tPercent_CarType_filter_size kcg_real_filter_size
#define get_tPercent_CarType_filter_utils get_kcg_real_filter_utils
#define tPercent_CarType_filter_values kcg_real_filter_values
/****************************************************************
 ** tSpeed_CarType
 ****************************************************************/
extern const char * tSpeed_CarType_to_string(const void* pValue);
extern int check_tSpeed_CarType_string(const char* strValue);
extern int string_to_tSpeed_CarType(const char* strValue, void* pValue);
int is_tSpeed_CarType_allow_double_convertion();
extern int tSpeed_CarType_to_double(double * nValue, const void*);
#define get_tSpeed_CarType_signature get_kcg_real_signature
#define compare_tSpeed_CarType_type compare_kcg_real_type
#define tSpeed_CarType_filter_size kcg_real_filter_size
#define get_tSpeed_CarType_filter_utils get_kcg_real_filter_utils
#define tSpeed_CarType_filter_values kcg_real_filter_values
/****************************************************************
 ** tCruiseState_CruiseControl
 ****************************************************************/
extern const char * tCruiseState_CruiseControl_to_string(const void* pValue);
extern int check_tCruiseState_CruiseControl_string(const char* strValue);
extern int string_to_tCruiseState_CruiseControl(const char* strValue, void* pValue);
int is_tCruiseState_CruiseControl_allow_double_convertion();
extern int tCruiseState_CruiseControl_to_double(double * nValue, const void*);
extern const char * get_tCruiseState_CruiseControl_signature();
extern int compare_tCruiseState_CruiseControl_type(int*, const char*, const void*);
#define tCruiseState_CruiseControl_filter_size 0
#define get_tCruiseState_CruiseControl_filter_utils 0
#define tCruiseState_CruiseControl_filter_values 0

#endif /*CRUISECONTROL_CRUISECONTROL_TYPES_CONVERTION */
