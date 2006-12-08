/*-----------------------------------------------------------------------------------*/
/* Scilab */
/*-----------------------------------------------------------------------------------*/
#include "machine.h"
#include "stack-def.h"
/*-----------------------------------------------------------------------------------*/
#ifdef FAILED
	#undef FAILED
#endif
#define FAILED 0
#define OK 1
#define MAXLENGHTFUNCTIONNAME 256 /* 24 in fact in scilab */
/*-----------------------------------------------------------------------------------*/
typedef struct entry 
{
	int key[nsiz];
	int data;
	char namefunction[MAXLENGHTFUNCTIONNAME];
} ENTRY;
/*-----------------------------------------------------------------------------------*/
typedef struct 
{ 
	unsigned int   used;
	ENTRY entry;
} _ENTRY;
/*-----------------------------------------------------------------------------------*/
typedef enum 
{
	SCI_HFUNCTIONS_FIND, SCI_HFUNCTIONS_ENTER,SCI_HFUNCTIONS_DELETE,SCI_HFUNCTIONS_BACKSEARCH
} SCI_HFUNCTIONS_ACTION;
/*-----------------------------------------------------------------------------------*/
/* maximum number of entries in the htable */
/* in fact create_hashtable_scilab_functions used a prime > MAXTAB */
/* WARNING : MAXTAB must be chosen > 2* the number of scilab functions */
/* for good efficiency of the hash code */
#define MAXTAB 1536
/*-----------------------------------------------------------------------------------*/
int	create_hashtable_scilab_functions(unsigned int nel);
void destroy_hashtable_scilab_functions();
int action_hashtable_scilab_functions(int *key,char *name, int *data, SCI_HFUNCTIONS_ACTION action);
char **GetFunctionsList(int *sizeList);
BOOL ExistFunction(char *name);
/*-----------------------------------------------------------------------------------*/
