
#include <stdlib.h>
#include "SmuTypes.h"
#include "kcg_types.h"
#include "CruiseControl_CruiseControl_type.h"
#include "CruiseControl_CruiseControl_mapping.h"

extern ScSimulator * pSimulator;

/****************************************************************
 ** Boolean entity activation
 ****************************************************************/
int _SCSIM_BoolEntity_is_active(void* pValue) {
	return *((kcg_bool*)pValue) == kcg_true ? 1 : 0;
}

/****************************************************************
 ** Type utils declarations
 ****************************************************************/
TypeUtils _SCSIM_kcg_real_Utils = {kcg_real_to_string,
	check_kcg_real_string,
	string_to_kcg_real,
	is_kcg_real_allow_double_convertion,
	kcg_real_to_double,
	compare_kcg_real_type,
	get_kcg_real_signature,
	get_kcg_real_filter_utils,
	kcg_real_filter_size,
	kcg_real_filter_values};
TypeUtils _SCSIM_kcg_bool_Utils = {kcg_bool_to_string,
	check_kcg_bool_string,
	string_to_kcg_bool,
	is_kcg_bool_allow_double_convertion,
	kcg_bool_to_double,
	compare_kcg_bool_type,
	get_kcg_bool_signature,
	get_kcg_bool_filter_utils,
	kcg_bool_filter_size,
	kcg_bool_filter_values};
TypeUtils _SCSIM_kcg_char_Utils = {kcg_char_to_string,
	check_kcg_char_string,
	string_to_kcg_char,
	is_kcg_char_allow_double_convertion,
	kcg_char_to_double,
	compare_kcg_char_type,
	get_kcg_char_signature,
	get_kcg_char_filter_utils,
	kcg_char_filter_size,
	kcg_char_filter_values};
TypeUtils _SCSIM_kcg_int_Utils = {kcg_int_to_string,
	check_kcg_int_string,
	string_to_kcg_int,
	is_kcg_int_allow_double_convertion,
	kcg_int_to_double,
	compare_kcg_int_type,
	get_kcg_int_signature,
	get_kcg_int_filter_utils,
	kcg_int_filter_size,
	kcg_int_filter_values};
TypeUtils _SCSIM_tPercent_CarType_Utils = {tPercent_CarType_to_string,
	check_tPercent_CarType_string,
	string_to_tPercent_CarType,
	is_tPercent_CarType_allow_double_convertion,
	tPercent_CarType_to_double,
	compare_tPercent_CarType_type,
	get_tPercent_CarType_signature,
	get_tPercent_CarType_filter_utils,
	tPercent_CarType_filter_size,
	tPercent_CarType_filter_values};
TypeUtils _SCSIM_tSpeed_CarType_Utils = {tSpeed_CarType_to_string,
	check_tSpeed_CarType_string,
	string_to_tSpeed_CarType,
	is_tSpeed_CarType_allow_double_convertion,
	tSpeed_CarType_to_double,
	compare_tSpeed_CarType_type,
	get_tSpeed_CarType_signature,
	get_tSpeed_CarType_filter_utils,
	tSpeed_CarType_filter_size,
	tSpeed_CarType_filter_values};
TypeUtils _SCSIM_tCruiseState_CruiseControl_Utils = {tCruiseState_CruiseControl_to_string,
	check_tCruiseState_CruiseControl_string,
	string_to_tCruiseState_CruiseControl,
	is_tCruiseState_CruiseControl_allow_double_convertion,
	tCruiseState_CruiseControl_to_double,
	compare_tCruiseState_CruiseControl_type,
	get_tCruiseState_CruiseControl_signature,
	get_tCruiseState_CruiseControl_filter_utils,
	tCruiseState_CruiseControl_filter_size,
	tCruiseState_CruiseControl_filter_values};

/****************************************************************
 ** kcg_real
 ****************************************************************/
struct SimTypeVTable* pSimDoubleVTable;
const char * kcg_real_to_string(const void* pValue) {
	if (pSimDoubleVTable != 0 && pSimDoubleVTable->m_pfnGetConvInfo(SptString, SptNone) == 1) {
		double value = (double)(*(const kcg_real*)pValue);
		return *(char**)pSimDoubleVTable->m_pfnToType(SptString, &value);
	}
	return pSimulator->m_pfnRealToString((double)(*(const kcg_real*)pValue));
}

int string_to_kcg_real(const char* strValue, void* pValue) {
	double nTemp = 0;
	static double rTemp;
	int nResult;
	if (pSimDoubleVTable != 0 && pSimDoubleVTable->m_pfnGetConvInfo(SptNone, SptString) == 1) {
		nResult = pSimDoubleVTable->m_pfnFromType(SptString, (const void*)&strValue, &rTemp);
		if (nResult == 1)
			*(kcg_real*)pValue = (kcg_real)rTemp;
		return nResult;
	}
	nResult = pSimulator->m_pfnStringToReal(strValue, &nTemp);
	if (nResult == 1)
		*(kcg_real*)pValue = (kcg_real)nTemp;
	return nResult;
}

int compare_kcg_real_type(int* pResult, const char* toCompare, const void* pValue) {
	static kcg_real rTemp;
	const kcg_real* pCurrent = (const kcg_real*)pValue;
	if (string_to_kcg_real(toCompare, &rTemp) == 0)
		return 0;
	if (*pCurrent > rTemp)
		*pResult = 1;
	else if (*pCurrent < rTemp)
		*pResult = -1;
	else
		*pResult = 0;
	return 1;
}

int is_kcg_real_allow_double_convertion() {
	if (pSimDoubleVTable != 0) {
		int nConvertionAllowed = 0;
		nConvertionAllowed |= pSimDoubleVTable->m_pfnGetConvInfo(SptNone, SptLong) == 1;
		nConvertionAllowed |= pSimDoubleVTable->m_pfnGetConvInfo(SptNone, SptShort) == 1;
		nConvertionAllowed |= pSimDoubleVTable->m_pfnGetConvInfo(SptNone, SptDouble) == 1;
		nConvertionAllowed |= pSimDoubleVTable->m_pfnGetConvInfo(SptNone, SptFloat) == 1;
		return nConvertionAllowed;
	}
	return 1;
}

int kcg_real_to_double(double * nValue, const void* pValue) {
	if (pSimDoubleVTable != 0) {
		double value = (double)(*(const kcg_real*)pValue);
		if (pSimDoubleVTable->m_pfnGetConvInfo(SptNone, SptDouble) == 1)
			*nValue = (*(double*)pSimDoubleVTable->m_pfnToType(SptDouble, &value));
		else if (pSimDoubleVTable->m_pfnGetConvInfo(SptNone, SptFloat) == 1)
			*nValue = (double)(*(float*)pSimDoubleVTable->m_pfnToType(SptFloat, &value));
		else if (pSimDoubleVTable->m_pfnGetConvInfo(SptNone, SptLong) == 1)
			*nValue = (double)(*(long*)pSimDoubleVTable->m_pfnToType(SptLong, &value));
		else if (pSimDoubleVTable->m_pfnGetConvInfo(SptNone, SptShort) == 1)
			*nValue = (double)(*(int*)pSimDoubleVTable->m_pfnToType(SptShort, &value));
		else
			return 0;
		return 1;
	}
	*nValue = (double)*((const kcg_real*)pValue);
	return 1;
}

const char * get_kcg_real_signature() {
	return "R";
}

int check_kcg_real_string(const char* strValue) {
	static kcg_real rTemp;
	return string_to_kcg_real(strValue, &rTemp);
}


/****************************************************************
 ** kcg_bool
 ****************************************************************/
struct SimTypeVTable* pSimBoolVTable;
const char * kcg_bool_to_string(const void* pValue) {
	if (pSimBoolVTable != 0 && pSimBoolVTable->m_pfnGetConvInfo(SptString, SptNone) == 1) {
		SimBool value = (*((const kcg_bool*)pValue) == kcg_true)? SbTrue : SbFalse;
		return *(char**)pSimBoolVTable->m_pfnToType(SptString, &value);
	}
	return pSimulator->m_pfnBoolToString((*(const kcg_bool*)pValue) == kcg_true ? 1 : 0);
}

int string_to_kcg_bool(const char* strValue, void* pValue) {
	int nTemp = 0;
	static SimBool rTemp;
	int nResult;
	if (pSimBoolVTable != 0 && pSimBoolVTable->m_pfnGetConvInfo(SptNone, SptString) == 1) {
		nResult = pSimBoolVTable->m_pfnFromType(SptString, (const void*)&strValue, &rTemp);
		if (nResult == 1)
			*((kcg_bool*)pValue) = (rTemp == SbTrue)? kcg_true : kcg_false;
		return nResult;
	}
	nResult = pSimulator->m_pfnStringToBool(strValue, &nTemp);
	if (nResult == 1)
		*(kcg_bool*)pValue = nTemp == 1 ? kcg_true : kcg_false;
	return nResult;
}

int compare_kcg_bool_type(int* pResult, const char* toCompare, const void* pValue) {
	static kcg_bool rTemp;
	const kcg_bool* pCurrent = (const kcg_bool*)pValue;
	if (string_to_kcg_bool(toCompare, &rTemp) == 0)
		return 0;
	if (*pCurrent > rTemp)
		*pResult = 1;
	else if (*pCurrent < rTemp)
		*pResult = -1;
	else
		*pResult = 0;
	return 1;
}

int is_kcg_bool_allow_double_convertion() {
	if (pSimBoolVTable != 0) {
		int nConvertionAllowed = 0;
		nConvertionAllowed |= pSimBoolVTable->m_pfnGetConvInfo(SptNone, SptLong) == 1;
		nConvertionAllowed |= pSimBoolVTable->m_pfnGetConvInfo(SptNone, SptShort) == 1;
		nConvertionAllowed |= pSimBoolVTable->m_pfnGetConvInfo(SptNone, SptDouble) == 1;
		nConvertionAllowed |= pSimBoolVTable->m_pfnGetConvInfo(SptNone, SptFloat) == 1;
		return nConvertionAllowed;
	}
	return 1;
}

int kcg_bool_to_double(double * nValue, const void* pValue) {
	if (pSimBoolVTable != 0) {
		SimBool value = (*(const kcg_bool*)pValue == kcg_true)? SbTrue : SbFalse;
		if (pSimBoolVTable->m_pfnGetConvInfo(SptNone, SptDouble) == 1)
			*nValue = (*(double*)pSimBoolVTable->m_pfnToType(SptDouble, &value));
		else if (pSimBoolVTable->m_pfnGetConvInfo(SptNone, SptFloat) == 1)
			*nValue = (double)(*(float*)pSimBoolVTable->m_pfnToType(SptFloat, &value));
		else if (pSimBoolVTable->m_pfnGetConvInfo(SptNone, SptLong) == 1)
			*nValue = (double)(*(long*)pSimBoolVTable->m_pfnToType(SptLong, &value));
		else if (pSimBoolVTable->m_pfnGetConvInfo(SptNone, SptShort) == 1)
			*nValue = (double)(*(int*)pSimBoolVTable->m_pfnToType(SptShort, &value));
		else
			return 0;
		return 1;
	}
	*nValue = *((const kcg_bool*)pValue) == kcg_true ? 1.0 : 0.0;
	return 1;
}

const char * get_kcg_bool_signature() {
	return "B";
}

int check_kcg_bool_string(const char* strValue) {
	static kcg_bool rTemp;
	return string_to_kcg_bool(strValue, &rTemp);
}


/****************************************************************
 ** kcg_char
 ****************************************************************/
struct SimTypeVTable* pSimCharVTable;
const char * kcg_char_to_string(const void* pValue) {
	if (pSimCharVTable != 0 && pSimCharVTable->m_pfnGetConvInfo(SptString, SptNone) == 1) {
		char value = (char)(*(const kcg_char*)pValue);
		return *(char**)pSimCharVTable->m_pfnToType(SptString, &value);
	}
	return pSimulator->m_pfnCharToString((char)(*(const kcg_char*)pValue));
}

int string_to_kcg_char(const char* strValue, void* pValue) {
	char nTemp = 0;
	static char rTemp;
	int nResult;
	if (pSimCharVTable != 0 && pSimCharVTable->m_pfnGetConvInfo(SptNone, SptString) == 1) {
		nResult = pSimCharVTable->m_pfnFromType(SptString, (const void*)&strValue, &rTemp);
		if (nResult == 1)
			*(kcg_char*)pValue = (kcg_char)rTemp;
		return nResult;
	}
	nResult = pSimulator->m_pfnStringToChar(strValue, &nTemp);
	if (nResult == 1)
		*(kcg_char*)pValue = (kcg_char)nTemp;
	return nResult;
}

int compare_kcg_char_type(int* pResult, const char* toCompare, const void* pValue) {
	static kcg_char rTemp;
	const kcg_char* pCurrent = (const kcg_char*)pValue;
	if (string_to_kcg_char(toCompare, &rTemp) == 0)
		return 0;
	if (*pCurrent > rTemp)
		*pResult = 1;
	else if (*pCurrent < rTemp)
		*pResult = -1;
	else
		*pResult = 0;
	return 1;
}

int is_kcg_char_allow_double_convertion() {
	if (pSimCharVTable != 0) {
		int nConvertionAllowed = 0;
		nConvertionAllowed |= pSimCharVTable->m_pfnGetConvInfo(SptNone, SptLong) == 1;
		nConvertionAllowed |= pSimCharVTable->m_pfnGetConvInfo(SptNone, SptShort) == 1;
		nConvertionAllowed |= pSimCharVTable->m_pfnGetConvInfo(SptNone, SptDouble) == 1;
		nConvertionAllowed |= pSimCharVTable->m_pfnGetConvInfo(SptNone, SptFloat) == 1;
		return nConvertionAllowed;
	}
	return 1;
}

int kcg_char_to_double(double * nValue, const void* pValue) {
	if (pSimCharVTable != 0) {
		char value = (char)(*(const kcg_char*)pValue);
		if (pSimCharVTable->m_pfnGetConvInfo(SptNone, SptDouble) == 1)
			*nValue = (*(double*)pSimCharVTable->m_pfnToType(SptDouble, &value));
		else if (pSimCharVTable->m_pfnGetConvInfo(SptNone, SptFloat) == 1)
			*nValue = (double)(*(float*)pSimCharVTable->m_pfnToType(SptFloat, &value));
		else if (pSimCharVTable->m_pfnGetConvInfo(SptNone, SptLong) == 1)
			*nValue = (double)(*(long*)pSimCharVTable->m_pfnToType(SptLong, &value));
		else if (pSimCharVTable->m_pfnGetConvInfo(SptNone, SptShort) == 1)
			*nValue = (double)(*(int*)pSimCharVTable->m_pfnToType(SptShort, &value));
		else
			return 0;
		return 1;
	}
	*nValue = (double)*((const kcg_char*)pValue);
	return 1;
}

const char * get_kcg_char_signature() {
	return "C";
}

int check_kcg_char_string(const char* strValue) {
	static kcg_char rTemp;
	return string_to_kcg_char(strValue, &rTemp);
}


/****************************************************************
 ** kcg_int
 ****************************************************************/
struct SimTypeVTable* pSimLongVTable;
const char * kcg_int_to_string(const void* pValue) {
	if (pSimLongVTable != 0 && pSimLongVTable->m_pfnGetConvInfo(SptString, SptNone) == 1) {
		long value = (long)(*(const kcg_int*)pValue);
		return *(char**)pSimLongVTable->m_pfnToType(SptString, &value);
	}
	return pSimulator->m_pfnIntToString(*(const int*)pValue);
}

int string_to_kcg_int(const char* strValue, void* pValue) {
	int nTemp = 0;
	static long rTemp;
	int nResult;
	if (pSimLongVTable != 0 && pSimLongVTable->m_pfnGetConvInfo(SptNone, SptString) == 1) {
		nResult = pSimLongVTable->m_pfnFromType(SptString, (const void*)&strValue, &rTemp);
		if (nResult == 1)
			*(kcg_int*)pValue = (kcg_int)rTemp;
		return nResult;
	}
	nResult = pSimulator->m_pfnStringToInt(strValue, &nTemp);
	if (nResult == 1)
		*(kcg_int*)pValue = (kcg_int)nTemp;
	return nResult;
}

int compare_kcg_int_type(int* pResult, const char* toCompare, const void* pValue) {
	static kcg_int rTemp;
	const kcg_int* pCurrent = (const kcg_int*)pValue;
	if (string_to_kcg_int(toCompare, &rTemp) == 0)
		return 0;
	if (*pCurrent > rTemp)
		*pResult = 1;
	else if (*pCurrent < rTemp)
		*pResult = -1;
	else
		*pResult = 0;
	return 1;
}

int is_kcg_int_allow_double_convertion() {
	if (pSimLongVTable != 0) {
		int nConvertionAllowed = 0;
		nConvertionAllowed |= pSimLongVTable->m_pfnGetConvInfo(SptNone, SptLong) == 1;
		nConvertionAllowed |= pSimLongVTable->m_pfnGetConvInfo(SptNone, SptShort) == 1;
		nConvertionAllowed |= pSimLongVTable->m_pfnGetConvInfo(SptNone, SptDouble) == 1;
		nConvertionAllowed |= pSimLongVTable->m_pfnGetConvInfo(SptNone, SptFloat) == 1;
		return nConvertionAllowed;
	}
	return 1;
}

int kcg_int_to_double(double * nValue, const void* pValue) {
	if (pSimLongVTable != 0) {
		long value = (long)(*(const kcg_int*)pValue);
		if (pSimLongVTable->m_pfnGetConvInfo(SptNone, SptDouble) == 1)
			*nValue = (*(double*)pSimLongVTable->m_pfnToType(SptDouble, &value));
		else if (pSimLongVTable->m_pfnGetConvInfo(SptNone, SptFloat) == 1)
			*nValue = (double)(*(float*)pSimLongVTable->m_pfnToType(SptFloat, &value));
		else if (pSimLongVTable->m_pfnGetConvInfo(SptNone, SptLong) == 1)
			*nValue = (double)(*(long*)pSimLongVTable->m_pfnToType(SptLong, &value));
		else if (pSimLongVTable->m_pfnGetConvInfo(SptNone, SptShort) == 1)
			*nValue = (double)(*(int*)pSimLongVTable->m_pfnToType(SptShort, &value));
		else
			return 0;
		return 1;
	}
	*nValue = (double)*((const kcg_int*)pValue);
	return 1;
}

const char * get_kcg_int_signature() {
	return "I";
}

int check_kcg_int_string(const char* strValue) {
	static kcg_int rTemp;
	return string_to_kcg_int(strValue, &rTemp);
}


/****************************************************************
 ** tPercent_CarType
 ****************************************************************/
struct SimTypeVTable* pSimtPercent_CarTypeVTable;
const char * tPercent_CarType_to_string(const void* pValue) {
	if (pSimtPercent_CarTypeVTable != 0 && pSimtPercent_CarTypeVTable->m_pfnGetConvInfo(SptString, SptNone) == 1)
		return *(char**)pSimtPercent_CarTypeVTable->m_pfnToType(SptString, pValue);
	return kcg_real_to_string(pValue);
}

int string_to_tPercent_CarType(const char* strValue, void* pValue) {
	if (pSimtPercent_CarTypeVTable != 0 && pSimtPercent_CarTypeVTable->m_pfnGetConvInfo(SptNone, SptString) == 1) {
		static tPercent_CarType rTemp;
		int nResult = pSimtPercent_CarTypeVTable->m_pfnFromType(SptString, (const void*)&strValue, &rTemp);
		if (nResult == 1)
			*((tPercent_CarType*)pValue) = rTemp;
		return nResult;
	}
	return string_to_kcg_real(strValue, pValue);
}

int is_tPercent_CarType_allow_double_convertion() {
	if (pSimtPercent_CarTypeVTable != 0) {
		int nConvertionAllowed = 0;
		nConvertionAllowed |= pSimtPercent_CarTypeVTable->m_pfnGetConvInfo(SptNone, SptLong) == 1;
		nConvertionAllowed |= pSimtPercent_CarTypeVTable->m_pfnGetConvInfo(SptNone, SptShort) == 1;
		nConvertionAllowed |= pSimtPercent_CarTypeVTable->m_pfnGetConvInfo(SptNone, SptDouble) == 1;
		nConvertionAllowed |= pSimtPercent_CarTypeVTable->m_pfnGetConvInfo(SptNone, SptFloat) == 1;
		return nConvertionAllowed;
	}
	return is_kcg_real_allow_double_convertion();
}

int tPercent_CarType_to_double(double * nValue, const void* pValue) {
	if (pSimtPercent_CarTypeVTable != 0) {
		if (pSimtPercent_CarTypeVTable->m_pfnGetConvInfo(SptNone, SptLong) == 1)
			*nValue = (double)(*(long*)pSimtPercent_CarTypeVTable->m_pfnToType(SptLong, pValue));
		else if (pSimtPercent_CarTypeVTable->m_pfnGetConvInfo(SptNone, SptShort) == 1)
			*nValue = (double)(*(int*)pSimtPercent_CarTypeVTable->m_pfnToType(SptShort, pValue));
		else if (pSimtPercent_CarTypeVTable->m_pfnGetConvInfo(SptNone, SptDouble) == 1)
			*nValue = (*(double*)pSimtPercent_CarTypeVTable->m_pfnToType(SptDouble, pValue));
		else if (pSimtPercent_CarTypeVTable->m_pfnGetConvInfo(SptNone, SptFloat) == 1)
			*nValue = (double)(*(float*)pSimtPercent_CarTypeVTable->m_pfnToType(SptFloat, pValue));
		else
			return 0;
		return 1;
	}
	if (_SCSIM_kcg_real_Utils.m_pfnTypeToDouble != 0)
		return _SCSIM_kcg_real_Utils.m_pfnTypeToDouble(nValue, pValue);
	return 0;
}

int check_tPercent_CarType_string(const char* strValue) {
	static tPercent_CarType rTemp;
	return string_to_tPercent_CarType(strValue, &rTemp);
}


/****************************************************************
 ** tSpeed_CarType
 ****************************************************************/
struct SimTypeVTable* pSimtSpeed_CarTypeVTable;
const char * tSpeed_CarType_to_string(const void* pValue) {
	if (pSimtSpeed_CarTypeVTable != 0 && pSimtSpeed_CarTypeVTable->m_pfnGetConvInfo(SptString, SptNone) == 1)
		return *(char**)pSimtSpeed_CarTypeVTable->m_pfnToType(SptString, pValue);
	return kcg_real_to_string(pValue);
}

int string_to_tSpeed_CarType(const char* strValue, void* pValue) {
	if (pSimtSpeed_CarTypeVTable != 0 && pSimtSpeed_CarTypeVTable->m_pfnGetConvInfo(SptNone, SptString) == 1) {
		static tSpeed_CarType rTemp;
		int nResult = pSimtSpeed_CarTypeVTable->m_pfnFromType(SptString, (const void*)&strValue, &rTemp);
		if (nResult == 1)
			*((tSpeed_CarType*)pValue) = rTemp;
		return nResult;
	}
	return string_to_kcg_real(strValue, pValue);
}

int is_tSpeed_CarType_allow_double_convertion() {
	if (pSimtSpeed_CarTypeVTable != 0) {
		int nConvertionAllowed = 0;
		nConvertionAllowed |= pSimtSpeed_CarTypeVTable->m_pfnGetConvInfo(SptNone, SptLong) == 1;
		nConvertionAllowed |= pSimtSpeed_CarTypeVTable->m_pfnGetConvInfo(SptNone, SptShort) == 1;
		nConvertionAllowed |= pSimtSpeed_CarTypeVTable->m_pfnGetConvInfo(SptNone, SptDouble) == 1;
		nConvertionAllowed |= pSimtSpeed_CarTypeVTable->m_pfnGetConvInfo(SptNone, SptFloat) == 1;
		return nConvertionAllowed;
	}
	return is_kcg_real_allow_double_convertion();
}

int tSpeed_CarType_to_double(double * nValue, const void* pValue) {
	if (pSimtSpeed_CarTypeVTable != 0) {
		if (pSimtSpeed_CarTypeVTable->m_pfnGetConvInfo(SptNone, SptLong) == 1)
			*nValue = (double)(*(long*)pSimtSpeed_CarTypeVTable->m_pfnToType(SptLong, pValue));
		else if (pSimtSpeed_CarTypeVTable->m_pfnGetConvInfo(SptNone, SptShort) == 1)
			*nValue = (double)(*(int*)pSimtSpeed_CarTypeVTable->m_pfnToType(SptShort, pValue));
		else if (pSimtSpeed_CarTypeVTable->m_pfnGetConvInfo(SptNone, SptDouble) == 1)
			*nValue = (*(double*)pSimtSpeed_CarTypeVTable->m_pfnToType(SptDouble, pValue));
		else if (pSimtSpeed_CarTypeVTable->m_pfnGetConvInfo(SptNone, SptFloat) == 1)
			*nValue = (double)(*(float*)pSimtSpeed_CarTypeVTable->m_pfnToType(SptFloat, pValue));
		else
			return 0;
		return 1;
	}
	if (_SCSIM_kcg_real_Utils.m_pfnTypeToDouble != 0)
		return _SCSIM_kcg_real_Utils.m_pfnTypeToDouble(nValue, pValue);
	return 0;
}

int check_tSpeed_CarType_string(const char* strValue) {
	static tSpeed_CarType rTemp;
	return string_to_tSpeed_CarType(strValue, &rTemp);
}


/****************************************************************
 ** tCruiseState_CruiseControl
 ****************************************************************/
struct SimTypeVTable* pSimtCruiseState_CruiseControlVTable;
const char * tCruiseState_CruiseControl_to_string(const void* pValue) {
	if (pSimtCruiseState_CruiseControlVTable != 0 && pSimtCruiseState_CruiseControlVTable->m_pfnGetConvInfo(SptString, SptNone) == 1)
		return *(char**)pSimtCruiseState_CruiseControlVTable->m_pfnToType(SptString, pValue);
	switch (*((tCruiseState_CruiseControl*)pValue)) {
	case OFF_CruiseControl:
		return "CruiseControl::OFF";
	case INT_CruiseControl:
		return "CruiseControl::INT";
	case STDBY_CruiseControl:
		return "CruiseControl::STDBY";
	case ON_CruiseControl:
		return "CruiseControl::ON";
	default:
		return "?";
	}
}

int string_to_tCruiseState_CruiseControl(const char* strValue, void* pValue) {
	if (pSimtCruiseState_CruiseControlVTable != 0 && pSimtCruiseState_CruiseControlVTable->m_pfnGetConvInfo(SptNone, SptString) == 1) {
		tCruiseState_CruiseControl rTemp;		int nResult = pSimtCruiseState_CruiseControlVTable->m_pfnFromType(SptString, (const void*)&strValue, &rTemp);
		if (nResult == 1)
			*((tCruiseState_CruiseControl*)pValue) = rTemp;
		return nResult;
	}
	if(strcmp(strValue, "OFF") == 0 || strcmp(strValue, "CruiseControl::OFF") == 0)
		*((tCruiseState_CruiseControl*)pValue) = OFF_CruiseControl;
	else if(strcmp(strValue, "INT") == 0 || strcmp(strValue, "CruiseControl::INT") == 0)
		*((tCruiseState_CruiseControl*)pValue) = INT_CruiseControl;
	else if(strcmp(strValue, "STDBY") == 0 || strcmp(strValue, "CruiseControl::STDBY") == 0)
		*((tCruiseState_CruiseControl*)pValue) = STDBY_CruiseControl;
	else if(strcmp(strValue, "ON") == 0 || strcmp(strValue, "CruiseControl::ON") == 0)
		*((tCruiseState_CruiseControl*)pValue) = ON_CruiseControl;
	else 
		return 0;
	return 1;
}

int is_tCruiseState_CruiseControl_allow_double_convertion() {
	return 1;
}


int tCruiseState_CruiseControl_to_double(double * nValue, const void* pValue) {
	switch (*((tCruiseState_CruiseControl*)pValue)) {
	case OFF_CruiseControl:
		*nValue = 0.0;
		break;
	case INT_CruiseControl:
		*nValue = 1.0;
		break;
	case STDBY_CruiseControl:
		*nValue = 2.0;
		break;
	case ON_CruiseControl:
		*nValue = 3.0;
		break;
	default:
		return 0;
	}
	return 1;
}


int compare_tCruiseState_CruiseControl_type(int* pResult, const char* toCompare, const void* pValue) {
	static tCruiseState_CruiseControl rTemp;
	const tCruiseState_CruiseControl* pCurrent = (const tCruiseState_CruiseControl*)pValue;
	if (string_to_tCruiseState_CruiseControl(toCompare, &rTemp) == 0)
		return 0;
	if (*pCurrent > rTemp)
		*pResult = 1;
	else if (*pCurrent < rTemp)
		*pResult = -1;
	else
		*pResult = 0;
	return 1;
}

const char * get_tCruiseState_CruiseControl_signature() {
	return "E"
		"|CruiseControl::OFF"
		"|CruiseControl::INT"
		"|CruiseControl::STDBY"
		"|CruiseControl::ON"
		;
}

int check_tCruiseState_CruiseControl_string(const char* strValue) {
	static tCruiseState_CruiseControl rTemp;
	return string_to_tCruiseState_CruiseControl(strValue, &rTemp);
}


