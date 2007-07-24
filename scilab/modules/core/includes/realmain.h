/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifndef __REALMAIN__
#define __REALMAIN__
/*-----------------------------------------------------------------------------------*/
#include <signal.h>
#include <stdio.h>
#include <string.h>
#ifdef _MSC_VER
#include <Windows.h>
#endif

#include "machine.h"
#include "stack-c.h"
#include "version.h"

/**
* The real main of Scilab
*
* @param no_startup_flag_l  
* @param initial_script 
* @param initial_script_type    
* @param memory 
*/
void realmain(int no_startup_flag_l,char *initial_script,int initial_script_type,int memory);

/**
 * @TODO add comment
 *
 * @param void  
 * @return <ReturnValue>
 */
int Get_no_startup_flag(void);

/**
 * @TODO add comment
 *
 * @param start 
 */
void Set_no_startup_flag(int start);

#ifndef _MSC_VER
char ** create_argv(int *argc);
#endif

#endif /* __REALMAIN__ */
/*-----------------------------------------------------------------------------------*/

