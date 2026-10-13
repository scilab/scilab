#include <stdlib.h>
#include <stddef.h>
#include "NewSmuTypes.h"
#include "kcg_types.h"
#include "CruiseControl_newtype.h"

#define skip_whitespace(str) while(*str == ' ' || *str == '\t') str++

/****************************************************************
 ** utility functions 
 ****************************************************************/

static int string_to_VTable(const char* str, const struct SimTypeVTable* pVTable, void* pValue, const char** endptr)
{
    int nRet;
    if (pVTable != NULL && pVTable->m_pfnGetConvInfo(SptNone, SptString) == 1) {
        nRet = pVTable->m_pfnFromType(SptString, (const void*)&str, pValue);
        if (nRet != 0)
            *endptr = str; /* TODO */
        return nRet;
    };
    return 0;
}

static int is_VTable_double_convertion_allowed(const struct SimTypeVTable* pVTable)
{
    if (pVTable != NULL) {
        int nConvertionAllowed = 0;
        nConvertionAllowed |= pVTable->m_pfnGetConvInfo(SptNone, SptLong) == 1;
        nConvertionAllowed |= pVTable->m_pfnGetConvInfo(SptNone, SptShort) == 1;
        nConvertionAllowed |= pVTable->m_pfnGetConvInfo(SptNone, SptDouble) == 1;
        nConvertionAllowed |= pVTable->m_pfnGetConvInfo(SptNone, SptFloat) == 1;
        return nConvertionAllowed;
    }
    return 1;
}

int VTable_to_double(const void *pValue, const struct SimTypeVTable* pVTable, double *nRetValue)
{
    if (pVTable != NULL) {
        if (pVTable->m_pfnGetConvInfo(SptNone, SptLong) == 1)
            *nRetValue = (double)(*(long*)pVTable->m_pfnToType(SptLong, pValue));
        else if (pVTable->m_pfnGetConvInfo(SptNone, SptShort) == 1)
            *nRetValue = (double)(*(int*)pVTable->m_pfnToType(SptShort, pValue));
        else if (pVTable->m_pfnGetConvInfo(SptNone, SptDouble) == 1)
            *nRetValue = (*(double*)pVTable->m_pfnToType(SptDouble, pValue));
        else if (pVTable->m_pfnGetConvInfo(SptNone, SptFloat) == 1)
            *nRetValue = (double)(*(float*)pVTable->m_pfnToType(SptFloat, pValue));
        else
            return 0;
    }
    return 1;
}

static int get_enum_signature(const EnumValUtils *pEnumVals, int nSize, int (*pfnStrAppend)(const char *str, void *pData), void *pData)
{
    int i;
    pfnStrAppend("E", pData);
    for(i=0; i<nSize; i++) {
        pfnStrAppend("|", pData);
        pfnStrAppend(pEnumVals[i].m_name, pData);
    }
    return 1;
}

static int get_structure_signature(const FieldUtils *pFields, int nSize, int (*pfnStrAppend)(const char *str, void *pData), void *pData)
{
    int i;
    pfnStrAppend("(", pData);
    for(i=0; i<nSize; i++) {
        if (i > 0)
            pfnStrAppend(",", pData);
        pfnStrAppend(pFields[i].m_name, pData);
    }
    pfnStrAppend(")", pData);
    return 1;
}

/****************************************************************
 ** SSM_ST_SM1 
 ****************************************************************/

struct SimTypeVTable* pSimSSM_ST_SM1VTable;

static EnumValUtils SSM_ST_SM1_values[] = {
    { "Off", SSM_st_Off_SM1},
    { "Off", SSM_st_Off_SM1},
    { "Enabled", SSM_st_Enabled_SM1},
    { "Enabled", SSM_st_Enabled_SM1},
};

int SSM_ST_SM1_to_string(const void* pValue, int (*pfnStrAppend)(const char *str, void *pData), void *pData)
{
    if (pSimSSM_ST_SM1VTable != NULL
        && pSimSSM_ST_SM1VTable->m_pfnGetConvInfo(SptString, SptNone) == 1) {
        return pfnStrAppend(*(char**)pSimSSM_ST_SM1VTable->m_pfnToType(SptString, pValue), pData);
    }
    return pConverter->m_pfnEnumToString(*(SSM_ST_SM1*)pValue, SSM_ST_SM1_values, 4, pfnStrAppend, pData);
}

int string_to_SSM_ST_SM1(const char* str, void* pValue, const char** endptr)
{
    skip_whitespace(str);
    if (pSimSSM_ST_SM1VTable != NULL
        && pSimSSM_ST_SM1VTable->m_pfnGetConvInfo(SptNone, SptString) == 1) {
        return string_to_VTable(str, pSimSSM_ST_SM1VTable, pValue, endptr);
    }
    {
        int nTemp = 0;
        int nRet = pConverter->m_pfnStringToEnum(str, &nTemp, SSM_ST_SM1_values, 4, endptr);
        if (pValue != NULL && nRet != 0)
            *(SSM_ST_SM1*)pValue = nTemp;
        return nRet;
    }
}

int is_SSM_ST_SM1_double_convertion_allowed()
{
    if (pSimSSM_ST_SM1VTable != NULL) {
        return is_VTable_double_convertion_allowed(pSimSSM_ST_SM1VTable);
    }
    return 1;
}

int compare_SSM_ST_SM1(int *pStatus, const void* pValue1, const void* pValue2, void *pData)
{
    if (pSimSSM_ST_SM1VTable != NULL
        && pSimSSM_ST_SM1VTable->m_version >= Scv612
        && pSimSSM_ST_SM1VTable->m_pfnCompare != NULL)
        return pSimSSM_ST_SM1VTable->m_pfnCompare(pStatus, pValue1, pValue2);
    return pConverter->m_pfnEnumComparison(pStatus, *(SSM_ST_SM1*)pValue1, *(SSM_ST_SM1*)pValue2, SSM_ST_SM1_values, 4, pData);
}

int SSM_ST_SM1_to_double(const void *pValue, double *nRetValue)
{
    if (pSimSSM_ST_SM1VTable != NULL) {
        return  VTable_to_double(pValue, pSimSSM_ST_SM1VTable, nRetValue);
    }
    *nRetValue = (double)*((SSM_ST_SM1*)pValue);
    return 1;
}

int get_SSM_ST_SM1_signature(int (*pfnStrAppend)(const char* str, void *pData), void *pData)
{
    return get_enum_signature(SSM_ST_SM1_values, 4, pfnStrAppend, pData);
}

int set_SSM_ST_SM1_default_value(void *pValue)
{
    *(SSM_ST_SM1*)pValue = SSM_st_Off_SM1;
    return 1;
}

int check_SSM_ST_SM1_string(const char* str, const char** endptr)
{
    return string_to_SSM_ST_SM1(str, NULL, endptr);
}

TypeUtils _Type_SSM_ST_SM1_Utils = {
    SSM_ST_SM1_to_string,
    check_SSM_ST_SM1_string,
    string_to_SSM_ST_SM1,
    is_SSM_ST_SM1_double_convertion_allowed,
    SSM_ST_SM1_to_double,
    compare_SSM_ST_SM1,
    get_SSM_ST_SM1_signature,
    set_SSM_ST_SM1_default_value,
    sizeof(SSM_ST_SM1)
};

/****************************************************************
 ** SSM_TR_SM1 
 ****************************************************************/

struct SimTypeVTable* pSimSSM_TR_SM1VTable;

static EnumValUtils SSM_TR_SM1_values[] = {
    { "SSM_TR_SM1_no_trans", 0},
    { "SSM_TR_SM1_no_trans", 0},
    { "Off:<1>", SSM_TR_Off_1_SM1},
    { "Off:<1>", SSM_TR_Off_1_SM1},
    { "Enabled:<1>", SSM_TR_Enabled_1_SM1},
    { "Enabled:<1>", SSM_TR_Enabled_1_SM1},
};

int SSM_TR_SM1_to_string(const void* pValue, int (*pfnStrAppend)(const char *str, void *pData), void *pData)
{
    if (pSimSSM_TR_SM1VTable != NULL
        && pSimSSM_TR_SM1VTable->m_pfnGetConvInfo(SptString, SptNone) == 1) {
        return pfnStrAppend(*(char**)pSimSSM_TR_SM1VTable->m_pfnToType(SptString, pValue), pData);
    }
    return pConverter->m_pfnEnumToString(*(SSM_TR_SM1*)pValue, SSM_TR_SM1_values, 6, pfnStrAppend, pData);
}

int string_to_SSM_TR_SM1(const char* str, void* pValue, const char** endptr)
{
    skip_whitespace(str);
    if (pSimSSM_TR_SM1VTable != NULL
        && pSimSSM_TR_SM1VTable->m_pfnGetConvInfo(SptNone, SptString) == 1) {
        return string_to_VTable(str, pSimSSM_TR_SM1VTable, pValue, endptr);
    }
    {
        int nTemp = 0;
        int nRet = pConverter->m_pfnStringToEnum(str, &nTemp, SSM_TR_SM1_values, 6, endptr);
        if (pValue != NULL && nRet != 0)
            *(SSM_TR_SM1*)pValue = nTemp;
        return nRet;
    }
}

int is_SSM_TR_SM1_double_convertion_allowed()
{
    if (pSimSSM_TR_SM1VTable != NULL) {
        return is_VTable_double_convertion_allowed(pSimSSM_TR_SM1VTable);
    }
    return 1;
}

int compare_SSM_TR_SM1(int *pStatus, const void* pValue1, const void* pValue2, void *pData)
{
    if (pSimSSM_TR_SM1VTable != NULL
        && pSimSSM_TR_SM1VTable->m_version >= Scv612
        && pSimSSM_TR_SM1VTable->m_pfnCompare != NULL)
        return pSimSSM_TR_SM1VTable->m_pfnCompare(pStatus, pValue1, pValue2);
    return pConverter->m_pfnEnumComparison(pStatus, *(SSM_TR_SM1*)pValue1, *(SSM_TR_SM1*)pValue2, SSM_TR_SM1_values, 6, pData);
}

int SSM_TR_SM1_to_double(const void *pValue, double *nRetValue)
{
    if (pSimSSM_TR_SM1VTable != NULL) {
        return  VTable_to_double(pValue, pSimSSM_TR_SM1VTable, nRetValue);
    }
    *nRetValue = (double)*((SSM_TR_SM1*)pValue);
    return 1;
}

int get_SSM_TR_SM1_signature(int (*pfnStrAppend)(const char* str, void *pData), void *pData)
{
    return get_enum_signature(SSM_TR_SM1_values, 6, pfnStrAppend, pData);
}

int set_SSM_TR_SM1_default_value(void *pValue)
{
    *(SSM_TR_SM1*)pValue = 0;
    return 1;
}

int check_SSM_TR_SM1_string(const char* str, const char** endptr)
{
    return string_to_SSM_TR_SM1(str, NULL, endptr);
}

TypeUtils _Type_SSM_TR_SM1_Utils = {
    SSM_TR_SM1_to_string,
    check_SSM_TR_SM1_string,
    string_to_SSM_TR_SM1,
    is_SSM_TR_SM1_double_convertion_allowed,
    SSM_TR_SM1_to_double,
    compare_SSM_TR_SM1,
    get_SSM_TR_SM1_signature,
    set_SSM_TR_SM1_default_value,
    sizeof(SSM_TR_SM1)
};

/****************************************************************
 ** SSM_ST_SM2_SM1_Enabled 
 ****************************************************************/

struct SimTypeVTable* pSimSSM_ST_SM2_SM1_EnabledVTable;

static EnumValUtils SSM_ST_SM2_SM1_Enabled_values[] = {
    { "Active", SSM_st_Active_SM1_Enabled_SM2},
    { "Active", SSM_st_Active_SM1_Enabled_SM2},
    { "Interrupt", SSM_st_Interrupt_SM1_Enabled_SM2},
    { "Interrupt", SSM_st_Interrupt_SM1_Enabled_SM2},
};

int SSM_ST_SM2_SM1_Enabled_to_string(const void* pValue, int (*pfnStrAppend)(const char *str, void *pData), void *pData)
{
    if (pSimSSM_ST_SM2_SM1_EnabledVTable != NULL
        && pSimSSM_ST_SM2_SM1_EnabledVTable->m_pfnGetConvInfo(SptString, SptNone) == 1) {
        return pfnStrAppend(*(char**)pSimSSM_ST_SM2_SM1_EnabledVTable->m_pfnToType(SptString, pValue), pData);
    }
    return pConverter->m_pfnEnumToString(*(SSM_ST_SM2_SM1_Enabled*)pValue, SSM_ST_SM2_SM1_Enabled_values, 4, pfnStrAppend, pData);
}

int string_to_SSM_ST_SM2_SM1_Enabled(const char* str, void* pValue, const char** endptr)
{
    skip_whitespace(str);
    if (pSimSSM_ST_SM2_SM1_EnabledVTable != NULL
        && pSimSSM_ST_SM2_SM1_EnabledVTable->m_pfnGetConvInfo(SptNone, SptString) == 1) {
        return string_to_VTable(str, pSimSSM_ST_SM2_SM1_EnabledVTable, pValue, endptr);
    }
    {
        int nTemp = 0;
        int nRet = pConverter->m_pfnStringToEnum(str, &nTemp, SSM_ST_SM2_SM1_Enabled_values, 4, endptr);
        if (pValue != NULL && nRet != 0)
            *(SSM_ST_SM2_SM1_Enabled*)pValue = nTemp;
        return nRet;
    }
}

int is_SSM_ST_SM2_SM1_Enabled_double_convertion_allowed()
{
    if (pSimSSM_ST_SM2_SM1_EnabledVTable != NULL) {
        return is_VTable_double_convertion_allowed(pSimSSM_ST_SM2_SM1_EnabledVTable);
    }
    return 1;
}

int compare_SSM_ST_SM2_SM1_Enabled(int *pStatus, const void* pValue1, const void* pValue2, void *pData)
{
    if (pSimSSM_ST_SM2_SM1_EnabledVTable != NULL
        && pSimSSM_ST_SM2_SM1_EnabledVTable->m_version >= Scv612
        && pSimSSM_ST_SM2_SM1_EnabledVTable->m_pfnCompare != NULL)
        return pSimSSM_ST_SM2_SM1_EnabledVTable->m_pfnCompare(pStatus, pValue1, pValue2);
    return pConverter->m_pfnEnumComparison(pStatus, *(SSM_ST_SM2_SM1_Enabled*)pValue1, *(SSM_ST_SM2_SM1_Enabled*)pValue2, SSM_ST_SM2_SM1_Enabled_values, 4, pData);
}

int SSM_ST_SM2_SM1_Enabled_to_double(const void *pValue, double *nRetValue)
{
    if (pSimSSM_ST_SM2_SM1_EnabledVTable != NULL) {
        return  VTable_to_double(pValue, pSimSSM_ST_SM2_SM1_EnabledVTable, nRetValue);
    }
    *nRetValue = (double)*((SSM_ST_SM2_SM1_Enabled*)pValue);
    return 1;
}

int get_SSM_ST_SM2_SM1_Enabled_signature(int (*pfnStrAppend)(const char* str, void *pData), void *pData)
{
    return get_enum_signature(SSM_ST_SM2_SM1_Enabled_values, 4, pfnStrAppend, pData);
}

int set_SSM_ST_SM2_SM1_Enabled_default_value(void *pValue)
{
    *(SSM_ST_SM2_SM1_Enabled*)pValue = SSM_st_Active_SM1_Enabled_SM2;
    return 1;
}

int check_SSM_ST_SM2_SM1_Enabled_string(const char* str, const char** endptr)
{
    return string_to_SSM_ST_SM2_SM1_Enabled(str, NULL, endptr);
}

TypeUtils _Type_SSM_ST_SM2_SM1_Enabled_Utils = {
    SSM_ST_SM2_SM1_Enabled_to_string,
    check_SSM_ST_SM2_SM1_Enabled_string,
    string_to_SSM_ST_SM2_SM1_Enabled,
    is_SSM_ST_SM2_SM1_Enabled_double_convertion_allowed,
    SSM_ST_SM2_SM1_Enabled_to_double,
    compare_SSM_ST_SM2_SM1_Enabled,
    get_SSM_ST_SM2_SM1_Enabled_signature,
    set_SSM_ST_SM2_SM1_Enabled_default_value,
    sizeof(SSM_ST_SM2_SM1_Enabled)
};

/****************************************************************
 ** SSM_TR_SM2_SM1_Enabled 
 ****************************************************************/

struct SimTypeVTable* pSimSSM_TR_SM2_SM1_EnabledVTable;

static EnumValUtils SSM_TR_SM2_SM1_Enabled_values[] = {
    { "SSM_TR_SM2_SM1_Enabled_no_trans", 0},
    { "SSM_TR_SM2_SM1_Enabled_no_trans", 0},
    { "Active:<1>", SSM_TR_Active_1_SM2_SM1_Enabled},
    { "Active:<1>", SSM_TR_Active_1_SM2_SM1_Enabled},
    { "Interrupt:<1>", SSM_TR_Interrupt_1_SM2_SM1_Enabled},
    { "Interrupt:<1>", SSM_TR_Interrupt_1_SM2_SM1_Enabled},
};

int SSM_TR_SM2_SM1_Enabled_to_string(const void* pValue, int (*pfnStrAppend)(const char *str, void *pData), void *pData)
{
    if (pSimSSM_TR_SM2_SM1_EnabledVTable != NULL
        && pSimSSM_TR_SM2_SM1_EnabledVTable->m_pfnGetConvInfo(SptString, SptNone) == 1) {
        return pfnStrAppend(*(char**)pSimSSM_TR_SM2_SM1_EnabledVTable->m_pfnToType(SptString, pValue), pData);
    }
    return pConverter->m_pfnEnumToString(*(SSM_TR_SM2_SM1_Enabled*)pValue, SSM_TR_SM2_SM1_Enabled_values, 6, pfnStrAppend, pData);
}

int string_to_SSM_TR_SM2_SM1_Enabled(const char* str, void* pValue, const char** endptr)
{
    skip_whitespace(str);
    if (pSimSSM_TR_SM2_SM1_EnabledVTable != NULL
        && pSimSSM_TR_SM2_SM1_EnabledVTable->m_pfnGetConvInfo(SptNone, SptString) == 1) {
        return string_to_VTable(str, pSimSSM_TR_SM2_SM1_EnabledVTable, pValue, endptr);
    }
    {
        int nTemp = 0;
        int nRet = pConverter->m_pfnStringToEnum(str, &nTemp, SSM_TR_SM2_SM1_Enabled_values, 6, endptr);
        if (pValue != NULL && nRet != 0)
            *(SSM_TR_SM2_SM1_Enabled*)pValue = nTemp;
        return nRet;
    }
}

int is_SSM_TR_SM2_SM1_Enabled_double_convertion_allowed()
{
    if (pSimSSM_TR_SM2_SM1_EnabledVTable != NULL) {
        return is_VTable_double_convertion_allowed(pSimSSM_TR_SM2_SM1_EnabledVTable);
    }
    return 1;
}

int compare_SSM_TR_SM2_SM1_Enabled(int *pStatus, const void* pValue1, const void* pValue2, void *pData)
{
    if (pSimSSM_TR_SM2_SM1_EnabledVTable != NULL
        && pSimSSM_TR_SM2_SM1_EnabledVTable->m_version >= Scv612
        && pSimSSM_TR_SM2_SM1_EnabledVTable->m_pfnCompare != NULL)
        return pSimSSM_TR_SM2_SM1_EnabledVTable->m_pfnCompare(pStatus, pValue1, pValue2);
    return pConverter->m_pfnEnumComparison(pStatus, *(SSM_TR_SM2_SM1_Enabled*)pValue1, *(SSM_TR_SM2_SM1_Enabled*)pValue2, SSM_TR_SM2_SM1_Enabled_values, 6, pData);
}

int SSM_TR_SM2_SM1_Enabled_to_double(const void *pValue, double *nRetValue)
{
    if (pSimSSM_TR_SM2_SM1_EnabledVTable != NULL) {
        return  VTable_to_double(pValue, pSimSSM_TR_SM2_SM1_EnabledVTable, nRetValue);
    }
    *nRetValue = (double)*((SSM_TR_SM2_SM1_Enabled*)pValue);
    return 1;
}

int get_SSM_TR_SM2_SM1_Enabled_signature(int (*pfnStrAppend)(const char* str, void *pData), void *pData)
{
    return get_enum_signature(SSM_TR_SM2_SM1_Enabled_values, 6, pfnStrAppend, pData);
}

int set_SSM_TR_SM2_SM1_Enabled_default_value(void *pValue)
{
    *(SSM_TR_SM2_SM1_Enabled*)pValue = 0;
    return 1;
}

int check_SSM_TR_SM2_SM1_Enabled_string(const char* str, const char** endptr)
{
    return string_to_SSM_TR_SM2_SM1_Enabled(str, NULL, endptr);
}

TypeUtils _Type_SSM_TR_SM2_SM1_Enabled_Utils = {
    SSM_TR_SM2_SM1_Enabled_to_string,
    check_SSM_TR_SM2_SM1_Enabled_string,
    string_to_SSM_TR_SM2_SM1_Enabled,
    is_SSM_TR_SM2_SM1_Enabled_double_convertion_allowed,
    SSM_TR_SM2_SM1_Enabled_to_double,
    compare_SSM_TR_SM2_SM1_Enabled,
    get_SSM_TR_SM2_SM1_Enabled_signature,
    set_SSM_TR_SM2_SM1_Enabled_default_value,
    sizeof(SSM_TR_SM2_SM1_Enabled)
};

/****************************************************************
 ** SSM_ST_SM3_SM1_Enabled_SM2_Active 
 ****************************************************************/

struct SimTypeVTable* pSimSSM_ST_SM3_SM1_Enabled_SM2_ActiveVTable;

static EnumValUtils SSM_ST_SM3_SM1_Enabled_SM2_Active_values[] = {
    { "On", SSM_st_On_SM1_Enabled_SM2_Active_SM3},
    { "On", SSM_st_On_SM1_Enabled_SM2_Active_SM3},
    { "StandBy", SSM_st_StandBy_SM1_Enabled_SM2_Active_SM3},
    { "StandBy", SSM_st_StandBy_SM1_Enabled_SM2_Active_SM3},
};

int SSM_ST_SM3_SM1_Enabled_SM2_Active_to_string(const void* pValue, int (*pfnStrAppend)(const char *str, void *pData), void *pData)
{
    if (pSimSSM_ST_SM3_SM1_Enabled_SM2_ActiveVTable != NULL
        && pSimSSM_ST_SM3_SM1_Enabled_SM2_ActiveVTable->m_pfnGetConvInfo(SptString, SptNone) == 1) {
        return pfnStrAppend(*(char**)pSimSSM_ST_SM3_SM1_Enabled_SM2_ActiveVTable->m_pfnToType(SptString, pValue), pData);
    }
    return pConverter->m_pfnEnumToString(*(SSM_ST_SM3_SM1_Enabled_SM2_Active*)pValue, SSM_ST_SM3_SM1_Enabled_SM2_Active_values, 4, pfnStrAppend, pData);
}

int string_to_SSM_ST_SM3_SM1_Enabled_SM2_Active(const char* str, void* pValue, const char** endptr)
{
    skip_whitespace(str);
    if (pSimSSM_ST_SM3_SM1_Enabled_SM2_ActiveVTable != NULL
        && pSimSSM_ST_SM3_SM1_Enabled_SM2_ActiveVTable->m_pfnGetConvInfo(SptNone, SptString) == 1) {
        return string_to_VTable(str, pSimSSM_ST_SM3_SM1_Enabled_SM2_ActiveVTable, pValue, endptr);
    }
    {
        int nTemp = 0;
        int nRet = pConverter->m_pfnStringToEnum(str, &nTemp, SSM_ST_SM3_SM1_Enabled_SM2_Active_values, 4, endptr);
        if (pValue != NULL && nRet != 0)
            *(SSM_ST_SM3_SM1_Enabled_SM2_Active*)pValue = nTemp;
        return nRet;
    }
}

int is_SSM_ST_SM3_SM1_Enabled_SM2_Active_double_convertion_allowed()
{
    if (pSimSSM_ST_SM3_SM1_Enabled_SM2_ActiveVTable != NULL) {
        return is_VTable_double_convertion_allowed(pSimSSM_ST_SM3_SM1_Enabled_SM2_ActiveVTable);
    }
    return 1;
}

int compare_SSM_ST_SM3_SM1_Enabled_SM2_Active(int *pStatus, const void* pValue1, const void* pValue2, void *pData)
{
    if (pSimSSM_ST_SM3_SM1_Enabled_SM2_ActiveVTable != NULL
        && pSimSSM_ST_SM3_SM1_Enabled_SM2_ActiveVTable->m_version >= Scv612
        && pSimSSM_ST_SM3_SM1_Enabled_SM2_ActiveVTable->m_pfnCompare != NULL)
        return pSimSSM_ST_SM3_SM1_Enabled_SM2_ActiveVTable->m_pfnCompare(pStatus, pValue1, pValue2);
    return pConverter->m_pfnEnumComparison(pStatus, *(SSM_ST_SM3_SM1_Enabled_SM2_Active*)pValue1, *(SSM_ST_SM3_SM1_Enabled_SM2_Active*)pValue2, SSM_ST_SM3_SM1_Enabled_SM2_Active_values, 4, pData);
}

int SSM_ST_SM3_SM1_Enabled_SM2_Active_to_double(const void *pValue, double *nRetValue)
{
    if (pSimSSM_ST_SM3_SM1_Enabled_SM2_ActiveVTable != NULL) {
        return  VTable_to_double(pValue, pSimSSM_ST_SM3_SM1_Enabled_SM2_ActiveVTable, nRetValue);
    }
    *nRetValue = (double)*((SSM_ST_SM3_SM1_Enabled_SM2_Active*)pValue);
    return 1;
}

int get_SSM_ST_SM3_SM1_Enabled_SM2_Active_signature(int (*pfnStrAppend)(const char* str, void *pData), void *pData)
{
    return get_enum_signature(SSM_ST_SM3_SM1_Enabled_SM2_Active_values, 4, pfnStrAppend, pData);
}

int set_SSM_ST_SM3_SM1_Enabled_SM2_Active_default_value(void *pValue)
{
    *(SSM_ST_SM3_SM1_Enabled_SM2_Active*)pValue = SSM_st_On_SM1_Enabled_SM2_Active_SM3;
    return 1;
}

int check_SSM_ST_SM3_SM1_Enabled_SM2_Active_string(const char* str, const char** endptr)
{
    return string_to_SSM_ST_SM3_SM1_Enabled_SM2_Active(str, NULL, endptr);
}

TypeUtils _Type_SSM_ST_SM3_SM1_Enabled_SM2_Active_Utils = {
    SSM_ST_SM3_SM1_Enabled_SM2_Active_to_string,
    check_SSM_ST_SM3_SM1_Enabled_SM2_Active_string,
    string_to_SSM_ST_SM3_SM1_Enabled_SM2_Active,
    is_SSM_ST_SM3_SM1_Enabled_SM2_Active_double_convertion_allowed,
    SSM_ST_SM3_SM1_Enabled_SM2_Active_to_double,
    compare_SSM_ST_SM3_SM1_Enabled_SM2_Active,
    get_SSM_ST_SM3_SM1_Enabled_SM2_Active_signature,
    set_SSM_ST_SM3_SM1_Enabled_SM2_Active_default_value,
    sizeof(SSM_ST_SM3_SM1_Enabled_SM2_Active)
};

/****************************************************************
 ** SSM_TR_SM3_SM1_Enabled_SM2_Active 
 ****************************************************************/

struct SimTypeVTable* pSimSSM_TR_SM3_SM1_Enabled_SM2_ActiveVTable;

static EnumValUtils SSM_TR_SM3_SM1_Enabled_SM2_Active_values[] = {
    { "SSM_TR_SM3_SM1_Enabled_SM2_Active_no_trans", 0},
    { "SSM_TR_SM3_SM1_Enabled_SM2_Active_no_trans", 0},
    { "On:<1>", SSM_TR_On_1_SM3_SM1_Enabled_SM2_Active},
    { "On:<1>", SSM_TR_On_1_SM3_SM1_Enabled_SM2_Active},
    { "StandBy:<1>", SSM_TR_StandBy_1_SM3_SM1_Enabled_SM2_Active},
    { "StandBy:<1>", SSM_TR_StandBy_1_SM3_SM1_Enabled_SM2_Active},
};

int SSM_TR_SM3_SM1_Enabled_SM2_Active_to_string(const void* pValue, int (*pfnStrAppend)(const char *str, void *pData), void *pData)
{
    if (pSimSSM_TR_SM3_SM1_Enabled_SM2_ActiveVTable != NULL
        && pSimSSM_TR_SM3_SM1_Enabled_SM2_ActiveVTable->m_pfnGetConvInfo(SptString, SptNone) == 1) {
        return pfnStrAppend(*(char**)pSimSSM_TR_SM3_SM1_Enabled_SM2_ActiveVTable->m_pfnToType(SptString, pValue), pData);
    }
    return pConverter->m_pfnEnumToString(*(SSM_TR_SM3_SM1_Enabled_SM2_Active*)pValue, SSM_TR_SM3_SM1_Enabled_SM2_Active_values, 6, pfnStrAppend, pData);
}

int string_to_SSM_TR_SM3_SM1_Enabled_SM2_Active(const char* str, void* pValue, const char** endptr)
{
    skip_whitespace(str);
    if (pSimSSM_TR_SM3_SM1_Enabled_SM2_ActiveVTable != NULL
        && pSimSSM_TR_SM3_SM1_Enabled_SM2_ActiveVTable->m_pfnGetConvInfo(SptNone, SptString) == 1) {
        return string_to_VTable(str, pSimSSM_TR_SM3_SM1_Enabled_SM2_ActiveVTable, pValue, endptr);
    }
    {
        int nTemp = 0;
        int nRet = pConverter->m_pfnStringToEnum(str, &nTemp, SSM_TR_SM3_SM1_Enabled_SM2_Active_values, 6, endptr);
        if (pValue != NULL && nRet != 0)
            *(SSM_TR_SM3_SM1_Enabled_SM2_Active*)pValue = nTemp;
        return nRet;
    }
}

int is_SSM_TR_SM3_SM1_Enabled_SM2_Active_double_convertion_allowed()
{
    if (pSimSSM_TR_SM3_SM1_Enabled_SM2_ActiveVTable != NULL) {
        return is_VTable_double_convertion_allowed(pSimSSM_TR_SM3_SM1_Enabled_SM2_ActiveVTable);
    }
    return 1;
}

int compare_SSM_TR_SM3_SM1_Enabled_SM2_Active(int *pStatus, const void* pValue1, const void* pValue2, void *pData)
{
    if (pSimSSM_TR_SM3_SM1_Enabled_SM2_ActiveVTable != NULL
        && pSimSSM_TR_SM3_SM1_Enabled_SM2_ActiveVTable->m_version >= Scv612
        && pSimSSM_TR_SM3_SM1_Enabled_SM2_ActiveVTable->m_pfnCompare != NULL)
        return pSimSSM_TR_SM3_SM1_Enabled_SM2_ActiveVTable->m_pfnCompare(pStatus, pValue1, pValue2);
    return pConverter->m_pfnEnumComparison(pStatus, *(SSM_TR_SM3_SM1_Enabled_SM2_Active*)pValue1, *(SSM_TR_SM3_SM1_Enabled_SM2_Active*)pValue2, SSM_TR_SM3_SM1_Enabled_SM2_Active_values, 6, pData);
}

int SSM_TR_SM3_SM1_Enabled_SM2_Active_to_double(const void *pValue, double *nRetValue)
{
    if (pSimSSM_TR_SM3_SM1_Enabled_SM2_ActiveVTable != NULL) {
        return  VTable_to_double(pValue, pSimSSM_TR_SM3_SM1_Enabled_SM2_ActiveVTable, nRetValue);
    }
    *nRetValue = (double)*((SSM_TR_SM3_SM1_Enabled_SM2_Active*)pValue);
    return 1;
}

int get_SSM_TR_SM3_SM1_Enabled_SM2_Active_signature(int (*pfnStrAppend)(const char* str, void *pData), void *pData)
{
    return get_enum_signature(SSM_TR_SM3_SM1_Enabled_SM2_Active_values, 6, pfnStrAppend, pData);
}

int set_SSM_TR_SM3_SM1_Enabled_SM2_Active_default_value(void *pValue)
{
    *(SSM_TR_SM3_SM1_Enabled_SM2_Active*)pValue = 0;
    return 1;
}

int check_SSM_TR_SM3_SM1_Enabled_SM2_Active_string(const char* str, const char** endptr)
{
    return string_to_SSM_TR_SM3_SM1_Enabled_SM2_Active(str, NULL, endptr);
}

TypeUtils _Type_SSM_TR_SM3_SM1_Enabled_SM2_Active_Utils = {
    SSM_TR_SM3_SM1_Enabled_SM2_Active_to_string,
    check_SSM_TR_SM3_SM1_Enabled_SM2_Active_string,
    string_to_SSM_TR_SM3_SM1_Enabled_SM2_Active,
    is_SSM_TR_SM3_SM1_Enabled_SM2_Active_double_convertion_allowed,
    SSM_TR_SM3_SM1_Enabled_SM2_Active_to_double,
    compare_SSM_TR_SM3_SM1_Enabled_SM2_Active,
    get_SSM_TR_SM3_SM1_Enabled_SM2_Active_signature,
    set_SSM_TR_SM3_SM1_Enabled_SM2_Active_default_value,
    sizeof(SSM_TR_SM3_SM1_Enabled_SM2_Active)
};

/****************************************************************
 ** kcg_real 
 ****************************************************************/

struct SimTypeVTable* pSimDoubleVTable;

int kcg_real_to_string(const void* pValue, int (*pfnStrAppend)(const char *str, void *pData), void *pData)
{
    if (pSimDoubleVTable != NULL
        && pSimDoubleVTable->m_pfnGetConvInfo(SptString, SptNone) == 1) {
        return pfnStrAppend(*(char**)pSimDoubleVTable->m_pfnToType(SptString, pValue), pData);
    }
    return pConverter->m_pfnRealToString((double)(*(const kcg_real*)pValue), pfnStrAppend, pData);
}

int string_to_kcg_real(const char* str, void* pValue, const char** endptr)
{
    skip_whitespace(str);
    if (pSimDoubleVTable != NULL
        && pSimDoubleVTable->m_pfnGetConvInfo(SptNone, SptString) == 1) {
        return string_to_VTable(str, pSimDoubleVTable, pValue, endptr);
    }
    {
        double nTemp = 0;
        int nRet = pConverter->m_pfnStringToReal(str, &nTemp, endptr);
        if (nRet != 0 && pValue != NULL)
            *(kcg_real*)pValue = (kcg_real)nTemp;
        return nRet;
    }
}

int is_kcg_real_double_convertion_allowed()
{
    if (pSimDoubleVTable != NULL) {
        return is_VTable_double_convertion_allowed(pSimDoubleVTable);
    }
    return 1;
}

int compare_kcg_real(int *pStatus, const void* pValue1, const void* pValue2, void *pData)
{
    if (pSimDoubleVTable != NULL
        && pSimDoubleVTable->m_version >= Scv612
        && pSimDoubleVTable->m_pfnCompare != NULL)
        return pSimDoubleVTable->m_pfnCompare(pStatus, pValue1, pValue2);
    return pConverter->m_pfnRealComparison(pStatus, (double)*(kcg_real*)pValue1, (double)*(kcg_real*)pValue2, pData);
}

int kcg_real_to_double(const void *pValue, double *nRetValue)
{
    if (pSimDoubleVTable != NULL) {
        return  VTable_to_double(pValue, pSimDoubleVTable, nRetValue);
    }
    *nRetValue = (double)*((kcg_real*)pValue);
    return 1;
}

int get_kcg_real_signature(int (*pfnStrAppend)(const char* str, void *pData), void *pData)
{
    return pfnStrAppend("R", pData);
}

int set_kcg_real_default_value(void *pValue)
{
    *(kcg_real*)pValue = 0.0;
    return 1;
}

int check_kcg_real_string(const char* str, const char** endptr)
{
    return string_to_kcg_real(str, NULL, endptr);
}

TypeUtils _Type_kcg_real_Utils = {
    kcg_real_to_string,
    check_kcg_real_string,
    string_to_kcg_real,
    is_kcg_real_double_convertion_allowed,
    kcg_real_to_double,
    compare_kcg_real,
    get_kcg_real_signature,
    set_kcg_real_default_value,
    sizeof(kcg_real)
};

/****************************************************************
 ** kcg_bool 
 ****************************************************************/

struct SimTypeVTable* pSimBoolVTable;

int kcg_bool_to_string(const void* pValue, int (*pfnStrAppend)(const char *str, void *pData), void *pData)
{
    if (pSimBoolVTable != NULL
        && pSimBoolVTable->m_pfnGetConvInfo(SptString, SptNone) == 1) {
        return pfnStrAppend(*(char**)pSimBoolVTable->m_pfnToType(SptString, pValue), pData);
    }
    return pConverter->m_pfnBoolToString((*(const kcg_bool*)pValue) == kcg_true ? 1 : 0, pfnStrAppend, pData);
}

int string_to_kcg_bool(const char* str, void* pValue, const char** endptr)
{
    skip_whitespace(str);
    if (pSimBoolVTable != NULL
        && pSimBoolVTable->m_pfnGetConvInfo(SptNone, SptString) == 1) {
        return string_to_VTable(str, pSimBoolVTable, pValue, endptr);
    }
    {
        int nTemp = 0;
        int nRet = pConverter->m_pfnStringToBool(str, &nTemp, endptr);
        if (nRet != 0 && pValue != NULL)
            *(kcg_bool*)pValue = nTemp == 1 ? kcg_true : kcg_false;
        return nRet;
    }
}

int is_kcg_bool_double_convertion_allowed()
{
    if (pSimBoolVTable != NULL) {
        return is_VTable_double_convertion_allowed(pSimBoolVTable);
    }
    return 1;
}

int compare_kcg_bool(int *pStatus, const void* pValue1, const void* pValue2, void *pData)
{
    if (pSimBoolVTable != NULL
        && pSimBoolVTable->m_version >= Scv612
        && pSimBoolVTable->m_pfnCompare != NULL)
        return pSimBoolVTable->m_pfnCompare(pStatus, pValue1, pValue2);
    return pConverter->m_pfnBoolComparison(pStatus, (int)*(kcg_bool*)pValue1, (int)*(kcg_bool*)pValue2, pData);
}

int kcg_bool_to_double(const void *pValue, double *nRetValue)
{
    if (pSimBoolVTable != NULL) {
        return  VTable_to_double(pValue, pSimBoolVTable, nRetValue);
    }
    *nRetValue = *((kcg_bool*)pValue) == kcg_true ? 1.0 : 0.0;
    return 1;
}

int get_kcg_bool_signature(int (*pfnStrAppend)(const char* str, void *pData), void *pData)
{
    return pfnStrAppend("B", pData);
}

int set_kcg_bool_default_value(void *pValue)
{
    *(kcg_bool*)pValue = kcg_false;
    return 1;
}

int check_kcg_bool_string(const char* str, const char** endptr)
{
    return string_to_kcg_bool(str, NULL, endptr);
}

TypeUtils _Type_kcg_bool_Utils = {
    kcg_bool_to_string,
    check_kcg_bool_string,
    string_to_kcg_bool,
    is_kcg_bool_double_convertion_allowed,
    kcg_bool_to_double,
    compare_kcg_bool,
    get_kcg_bool_signature,
    set_kcg_bool_default_value,
    sizeof(kcg_bool)
};

/****************************************************************
 ** kcg_char 
 ****************************************************************/

struct SimTypeVTable* pSimCharVTable;

int kcg_char_to_string(const void* pValue, int (*pfnStrAppend)(const char *str, void *pData), void *pData)
{
    if (pSimCharVTable != NULL
        && pSimCharVTable->m_pfnGetConvInfo(SptString, SptNone) == 1) {
        return pfnStrAppend(*(char**)pSimCharVTable->m_pfnToType(SptString, pValue), pData);
    }
    return pConverter->m_pfnCharToString((char)(*(const kcg_char*)pValue), pfnStrAppend, pData);
}

int string_to_kcg_char(const char* str, void* pValue, const char** endptr)
{
    skip_whitespace(str);
    if (pSimCharVTable != NULL
        && pSimCharVTable->m_pfnGetConvInfo(SptNone, SptString) == 1) {
        return string_to_VTable(str, pSimCharVTable, pValue, endptr);
    }
    {
        char nTemp = 0;
        int nRet = pConverter->m_pfnStringToChar(str, &nTemp, endptr);
        if (nRet != 0 && pValue != NULL)
            *(kcg_char*)pValue = (kcg_char)nTemp;
        return nRet;
    }
}

int is_kcg_char_double_convertion_allowed()
{
    if (pSimCharVTable != NULL) {
        return is_VTable_double_convertion_allowed(pSimCharVTable);
    }
    return 1;
}

int compare_kcg_char(int *pStatus, const void* pValue1, const void* pValue2, void *pData)
{
    if (pSimCharVTable != NULL
        && pSimCharVTable->m_version >= Scv612
        && pSimCharVTable->m_pfnCompare != NULL)
        return pSimCharVTable->m_pfnCompare(pStatus, pValue1, pValue2);
    return pConverter->m_pfnCharComparison(pStatus, (char)*(kcg_char*)pValue1, (char)*(kcg_char*)pValue2, pData);
}

int kcg_char_to_double(const void *pValue, double *nRetValue)
{
    if (pSimCharVTable != NULL) {
        return  VTable_to_double(pValue, pSimCharVTable, nRetValue);
    }
    *nRetValue = (double)*((kcg_char*)pValue);
    return 1;
}

int get_kcg_char_signature(int (*pfnStrAppend)(const char* str, void *pData), void *pData)
{
    return pfnStrAppend("C", pData);
}

int set_kcg_char_default_value(void *pValue)
{
    *(kcg_char*)pValue = 0;
    return 1;
}

int check_kcg_char_string(const char* str, const char** endptr)
{
    return string_to_kcg_char(str, NULL, endptr);
}

TypeUtils _Type_kcg_char_Utils = {
    kcg_char_to_string,
    check_kcg_char_string,
    string_to_kcg_char,
    is_kcg_char_double_convertion_allowed,
    kcg_char_to_double,
    compare_kcg_char,
    get_kcg_char_signature,
    set_kcg_char_default_value,
    sizeof(kcg_char)
};

/****************************************************************
 ** kcg_int 
 ****************************************************************/

struct SimTypeVTable* pSimLongVTable;

int kcg_int_to_string(const void* pValue, int (*pfnStrAppend)(const char *str, void *pData), void *pData)
{
    if (pSimLongVTable != NULL
        && pSimLongVTable->m_pfnGetConvInfo(SptString, SptNone) == 1) {
        return pfnStrAppend(*(char**)pSimLongVTable->m_pfnToType(SptString, pValue), pData);
    }
    return pConverter->m_pfnIntToString(*(const int*)pValue, pfnStrAppend, pData);
}

int string_to_kcg_int(const char* str, void* pValue, const char** endptr)
{
    skip_whitespace(str);
    if (pSimLongVTable != NULL
        && pSimLongVTable->m_pfnGetConvInfo(SptNone, SptString) == 1) {
        return string_to_VTable(str, pSimLongVTable, pValue, endptr);
    }
    {
        int nTemp = 0;
        int nRet = pConverter->m_pfnStringToInt(str, &nTemp, endptr);
        if (nRet != 0 && pValue != NULL)
            *(kcg_int*)pValue = (kcg_int)nTemp;
        return nRet;
    }
}

int is_kcg_int_double_convertion_allowed()
{
    if (pSimLongVTable != NULL) {
        return is_VTable_double_convertion_allowed(pSimLongVTable);
    }
    return 1;
}

int compare_kcg_int(int *pStatus, const void* pValue1, const void* pValue2, void *pData)
{
    if (pSimLongVTable != NULL
        && pSimLongVTable->m_version >= Scv612
        && pSimLongVTable->m_pfnCompare != NULL)
        return pSimLongVTable->m_pfnCompare(pStatus, pValue1, pValue2);
    return pConverter->m_pfnIntComparison(pStatus, (int)*(kcg_int*)pValue1, (int)*(kcg_int*)pValue2, pData);
}

int kcg_int_to_double(const void *pValue, double *nRetValue)
{
    if (pSimLongVTable != NULL) {
        return  VTable_to_double(pValue, pSimLongVTable, nRetValue);
    }
    *nRetValue = (double)*((kcg_int*)pValue);
    return 1;
}

int get_kcg_int_signature(int (*pfnStrAppend)(const char* str, void *pData), void *pData)
{
    return pfnStrAppend("I", pData);
}

int set_kcg_int_default_value(void *pValue)
{
    *(kcg_int*)pValue = 0;
    return 1;
}

int check_kcg_int_string(const char* str, const char** endptr)
{
    return string_to_kcg_int(str, NULL, endptr);
}

TypeUtils _Type_kcg_int_Utils = {
    kcg_int_to_string,
    check_kcg_int_string,
    string_to_kcg_int,
    is_kcg_int_double_convertion_allowed,
    kcg_int_to_double,
    compare_kcg_int,
    get_kcg_int_signature,
    set_kcg_int_default_value,
    sizeof(kcg_int)
};

/****************************************************************
 ** tPercent_CarType 
 ****************************************************************/

struct SimTypeVTable* pSimtPercent_CarTypeVTable;

int tPercent_CarType_to_string(const void* pValue, int (*pfnStrAppend)(const char *str, void *pData), void *pData)
{
    if (pSimtPercent_CarTypeVTable != NULL
        && pSimtPercent_CarTypeVTable->m_pfnGetConvInfo(SptString, SptNone) == 1) {
        return pfnStrAppend(*(char**)pSimtPercent_CarTypeVTable->m_pfnToType(SptString, pValue), pData);
    }
    return kcg_real_to_string(pValue, pfnStrAppend, pData);
}

int string_to_tPercent_CarType(const char* str, void* pValue, const char** endptr)
{
    skip_whitespace(str);
    if (pSimtPercent_CarTypeVTable != NULL
        && pSimtPercent_CarTypeVTable->m_pfnGetConvInfo(SptNone, SptString) == 1) {
        return string_to_VTable(str, pSimtPercent_CarTypeVTable, pValue, endptr);
    }
    return string_to_kcg_real(str, pValue, endptr);
}

int is_tPercent_CarType_double_convertion_allowed()
{
    if (pSimtPercent_CarTypeVTable != NULL) {
        return is_VTable_double_convertion_allowed(pSimtPercent_CarTypeVTable);
    }
    return is_kcg_real_double_convertion_allowed();
}

int compare_tPercent_CarType(int *pStatus, const void* pValue1, const void* pValue2, void *pData)
{
    if (pSimtPercent_CarTypeVTable != NULL
        && pSimtPercent_CarTypeVTable->m_version >= Scv612
        && pSimtPercent_CarTypeVTable->m_pfnCompare != NULL)
        return pSimtPercent_CarTypeVTable->m_pfnCompare(pStatus, pValue1, pValue2);
    return compare_kcg_real(pStatus, pValue1, pValue2, pData);
}

int tPercent_CarType_to_double(const void *pValue, double *nRetValue)
{
    if (pSimtPercent_CarTypeVTable != NULL) {
        return  VTable_to_double(pValue, pSimtPercent_CarTypeVTable, nRetValue);
    }
    return kcg_real_to_double(pValue, nRetValue);
}

int get_tPercent_CarType_signature(int (*pfnStrAppend)(const char* str, void *pData), void *pData)
{
    return get_kcg_real_signature(pfnStrAppend, pData);
}

int set_tPercent_CarType_default_value(void *pValue)
{
    return set_kcg_real_default_value(pValue);
}

int check_tPercent_CarType_string(const char* str, const char** endptr)
{
    return string_to_tPercent_CarType(str, NULL, endptr);
}

TypeUtils _Type_tPercent_CarType_Utils = {
    tPercent_CarType_to_string,
    check_tPercent_CarType_string,
    string_to_tPercent_CarType,
    is_tPercent_CarType_double_convertion_allowed,
    tPercent_CarType_to_double,
    compare_tPercent_CarType,
    get_tPercent_CarType_signature,
    set_tPercent_CarType_default_value,
    sizeof(tPercent_CarType)
};

/****************************************************************
 ** tSpeed_CarType 
 ****************************************************************/

struct SimTypeVTable* pSimtSpeed_CarTypeVTable;

int tSpeed_CarType_to_string(const void* pValue, int (*pfnStrAppend)(const char *str, void *pData), void *pData)
{
    if (pSimtSpeed_CarTypeVTable != NULL
        && pSimtSpeed_CarTypeVTable->m_pfnGetConvInfo(SptString, SptNone) == 1) {
        return pfnStrAppend(*(char**)pSimtSpeed_CarTypeVTable->m_pfnToType(SptString, pValue), pData);
    }
    return kcg_real_to_string(pValue, pfnStrAppend, pData);
}

int string_to_tSpeed_CarType(const char* str, void* pValue, const char** endptr)
{
    skip_whitespace(str);
    if (pSimtSpeed_CarTypeVTable != NULL
        && pSimtSpeed_CarTypeVTable->m_pfnGetConvInfo(SptNone, SptString) == 1) {
        return string_to_VTable(str, pSimtSpeed_CarTypeVTable, pValue, endptr);
    }
    return string_to_kcg_real(str, pValue, endptr);
}

int is_tSpeed_CarType_double_convertion_allowed()
{
    if (pSimtSpeed_CarTypeVTable != NULL) {
        return is_VTable_double_convertion_allowed(pSimtSpeed_CarTypeVTable);
    }
    return is_kcg_real_double_convertion_allowed();
}

int compare_tSpeed_CarType(int *pStatus, const void* pValue1, const void* pValue2, void *pData)
{
    if (pSimtSpeed_CarTypeVTable != NULL
        && pSimtSpeed_CarTypeVTable->m_version >= Scv612
        && pSimtSpeed_CarTypeVTable->m_pfnCompare != NULL)
        return pSimtSpeed_CarTypeVTable->m_pfnCompare(pStatus, pValue1, pValue2);
    return compare_kcg_real(pStatus, pValue1, pValue2, pData);
}

int tSpeed_CarType_to_double(const void *pValue, double *nRetValue)
{
    if (pSimtSpeed_CarTypeVTable != NULL) {
        return  VTable_to_double(pValue, pSimtSpeed_CarTypeVTable, nRetValue);
    }
    return kcg_real_to_double(pValue, nRetValue);
}

int get_tSpeed_CarType_signature(int (*pfnStrAppend)(const char* str, void *pData), void *pData)
{
    return get_kcg_real_signature(pfnStrAppend, pData);
}

int set_tSpeed_CarType_default_value(void *pValue)
{
    return set_kcg_real_default_value(pValue);
}

int check_tSpeed_CarType_string(const char* str, const char** endptr)
{
    return string_to_tSpeed_CarType(str, NULL, endptr);
}

TypeUtils _Type_tSpeed_CarType_Utils = {
    tSpeed_CarType_to_string,
    check_tSpeed_CarType_string,
    string_to_tSpeed_CarType,
    is_tSpeed_CarType_double_convertion_allowed,
    tSpeed_CarType_to_double,
    compare_tSpeed_CarType,
    get_tSpeed_CarType_signature,
    set_tSpeed_CarType_default_value,
    sizeof(tSpeed_CarType)
};

/****************************************************************
 ** tCruiseState_CruiseControl 
 ****************************************************************/

struct SimTypeVTable* pSimtCruiseState_CruiseControlVTable;

static EnumValUtils tCruiseState_CruiseControl_values[] = {
    { "CruiseControl::OFF", OFF_CruiseControl},
    { "OFF", OFF_CruiseControl},
    { "CruiseControl::INT", INT_CruiseControl},
    { "INT", INT_CruiseControl},
    { "CruiseControl::STDBY", STDBY_CruiseControl},
    { "STDBY", STDBY_CruiseControl},
    { "CruiseControl::ON", ON_CruiseControl},
    { "ON", ON_CruiseControl},
};

int tCruiseState_CruiseControl_to_string(const void* pValue, int (*pfnStrAppend)(const char *str, void *pData), void *pData)
{
    if (pSimtCruiseState_CruiseControlVTable != NULL
        && pSimtCruiseState_CruiseControlVTable->m_pfnGetConvInfo(SptString, SptNone) == 1) {
        return pfnStrAppend(*(char**)pSimtCruiseState_CruiseControlVTable->m_pfnToType(SptString, pValue), pData);
    }
    return pConverter->m_pfnEnumToString(*(tCruiseState_CruiseControl*)pValue, tCruiseState_CruiseControl_values, 8, pfnStrAppend, pData);
}

int string_to_tCruiseState_CruiseControl(const char* str, void* pValue, const char** endptr)
{
    skip_whitespace(str);
    if (pSimtCruiseState_CruiseControlVTable != NULL
        && pSimtCruiseState_CruiseControlVTable->m_pfnGetConvInfo(SptNone, SptString) == 1) {
        return string_to_VTable(str, pSimtCruiseState_CruiseControlVTable, pValue, endptr);
    }
    {
        int nTemp = 0;
        int nRet = pConverter->m_pfnStringToEnum(str, &nTemp, tCruiseState_CruiseControl_values, 8, endptr);
        if (pValue != NULL && nRet != 0)
            *(tCruiseState_CruiseControl*)pValue = nTemp;
        return nRet;
    }
}

int is_tCruiseState_CruiseControl_double_convertion_allowed()
{
    if (pSimtCruiseState_CruiseControlVTable != NULL) {
        return is_VTable_double_convertion_allowed(pSimtCruiseState_CruiseControlVTable);
    }
    return 1;
}

int compare_tCruiseState_CruiseControl(int *pStatus, const void* pValue1, const void* pValue2, void *pData)
{
    if (pSimtCruiseState_CruiseControlVTable != NULL
        && pSimtCruiseState_CruiseControlVTable->m_version >= Scv612
        && pSimtCruiseState_CruiseControlVTable->m_pfnCompare != NULL)
        return pSimtCruiseState_CruiseControlVTable->m_pfnCompare(pStatus, pValue1, pValue2);
    return pConverter->m_pfnEnumComparison(pStatus, *(tCruiseState_CruiseControl*)pValue1, *(tCruiseState_CruiseControl*)pValue2, tCruiseState_CruiseControl_values, 8, pData);
}

int tCruiseState_CruiseControl_to_double(const void *pValue, double *nRetValue)
{
    if (pSimtCruiseState_CruiseControlVTable != NULL) {
        return  VTable_to_double(pValue, pSimtCruiseState_CruiseControlVTable, nRetValue);
    }
    *nRetValue = (double)*((tCruiseState_CruiseControl*)pValue);
    return 1;
}

int get_tCruiseState_CruiseControl_signature(int (*pfnStrAppend)(const char* str, void *pData), void *pData)
{
    return get_enum_signature(tCruiseState_CruiseControl_values, 8, pfnStrAppend, pData);
}

int set_tCruiseState_CruiseControl_default_value(void *pValue)
{
    *(tCruiseState_CruiseControl*)pValue = OFF_CruiseControl;
    return 1;
}

int check_tCruiseState_CruiseControl_string(const char* str, const char** endptr)
{
    return string_to_tCruiseState_CruiseControl(str, NULL, endptr);
}

TypeUtils _Type_tCruiseState_CruiseControl_Utils = {
    tCruiseState_CruiseControl_to_string,
    check_tCruiseState_CruiseControl_string,
    string_to_tCruiseState_CruiseControl,
    is_tCruiseState_CruiseControl_double_convertion_allowed,
    tCruiseState_CruiseControl_to_double,
    compare_tCruiseState_CruiseControl,
    get_tCruiseState_CruiseControl_signature,
    set_tCruiseState_CruiseControl_default_value,
    sizeof(tCruiseState_CruiseControl)
};

