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
#if _MSC_VER
#include <Windows.h>
#endif

#include "../machine.h"
#include "../stack-c.h"
#include "../version.h"

void realmain(int nowin,int no_startup_flag,char *initial_script,int initial_script_type,int memory);
int Get_no_startup_flag(void);
void Set_no_startup_flag(int start);

#ifndef _MSC_VER
char ** create_argv(int *argc);
#endif

#endif /* __REALMAIN__ */
/*-----------------------------------------------------------------------------------*/

