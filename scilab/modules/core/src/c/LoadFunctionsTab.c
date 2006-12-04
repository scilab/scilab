/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include "machine.h"
#include "hashtable_core.h"
/*-----------------------------------------------------------------------------------*/  
static int firstentry = 0;
/*-----------------------------------------------------------------------------------*/  
typedef struct 
{
	char *name;
	int codeI;
	int code;
	int level;
} Funcs ;
extern Funcs SciFuncs[];
/*-----------------------------------------------------------------------------------*/  
extern  int C2F(cvname) __PARAMS((integer *,char *,integer *, unsigned long int));
extern void sciprint __PARAMS((char *fmt,...));
/*-----------------------------------------------------------------------------------*/  
static int Add_a_Scilab_primitive_in_hashtable(char *str, int *dataI, int *data, int *level);
/*-----------------------------------------------------------------------------------*/  
void LoadFunctionsTab(void)
{
	static int firstentry = 0;

	int j=0;

	if ( firstentry != 0 ) return;

	while ( SciFuncs[j].name != (char *) 0 )
	{
		if ( Add_a_Scilab_primitive_in_hashtable(SciFuncs[j].name,&SciFuncs[j].codeI,&SciFuncs[j].code,&SciFuncs[j].level) == FAILED)
		{
			sciprint("Fatal Error : Table for scilab functions is too small \n");
			exit(1);
		}
		j++;
	}

	firstentry = 1;

}
/*-----------------------------------------------------------------------------------*/
static int Add_a_Scilab_primitive_in_hashtable(char *str, int *dataI, int *data, int *level)
{
	int ldata;
	int id[NAMECODE];
	int zero=0;
	C2F(cvname)(id,str,&zero,strlen(str));
	ldata= (*dataI)*100+*data;
	return( action_hashtable_scilab_functions(id,&ldata,level,SCI_HFUNCTIONS_ENTER));
}
/*-----------------------------------------------------------------------------------*/
