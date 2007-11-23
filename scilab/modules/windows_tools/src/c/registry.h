/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#ifndef __REGISTY_H__
#define __REGISTY_H__

#include <Windows.h>
#include "machine.h" /* BOOL */

/**
* WindowsQueryRegistry
* Query a value in a registry key
* @param[in] ParamIn1 (HKEY)
* @param[in] ParamIn2 (SUBKEY)
* @param[in] ParamIn3 (value name)
* @param[out] ParamOut1 (value REG_SZ format)
* @param[out] ParamOut2 (value int)
* @param[out] OuputIsREG_SZ
* @return TRUE or FALSE
*/
BOOL WindowsQueryRegistry(char *ParamIn1,char *ParamIn2,char *ParamIn3,char *ParamOut1,int *ParamOut2,BOOL *OuputIsREG_SZ);

/**
* WindowsQueryRegistryList
* Query a list of values in a registry key
* @param[in] ParamIn1 (HKEY)
* @param[in] ParamIn2 (SUBKEY)
* @param[in] dimMax 
* @param[out] ListKeys (values)
* @return TRUE or FALSE
*/
BOOL WindowsQueryRegistryList(char *ParamIn1,char *ParamIn2,int dimMax,char **ListKeys);

/**
* WindowsQueryRegistryNumberOfElementsInList
* get numbers of elements in a list
* @param[in] ParamIn1 (HKEY)
* @param[in] ParamIn2 (SUBKEY)
* @param[out] Number
* @return TRUE or FALSE
*/
BOOL WindowsQueryRegistryNumberOfElementsInList(char *ParamIn1,char *ParamIn2,int *Number);

/**
* GetHkeyrootFromString
* convert string value to HKEY
* @param[in] string example "HKEY_CLASSES_ROOT"
* @return HKEY
*/
HKEY GetHkeyrootFromString(char *string);


#endif /* __REGISTY_H__ */
/*--------------------------------------------------------------------------*/
