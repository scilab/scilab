/*-----------------------------------------------------------------------------------*/
/* Scilab */
/*-----------------------------------------------------------------------------------*/
#ifndef __HASHTABLE_CORE_H__
#define __HASHTABLE_CORE_H__
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
	SCI_HFUNCTIONS_FIND = 1,SCI_HFUNCTIONS_BACKSEARCH = 2, SCI_HFUNCTIONS_ENTER = 3,SCI_HFUNCTIONS_DELETE = 4
} SCI_HFUNCTIONS_ACTION;
/*-----------------------------------------------------------------------------------*/
/* maximum number of entries in the htable */
/* in fact create_hashtable_scilab_functions used a prime > MAXTAB */
/* WARNING : MAXTAB must be chosen > 2* the number of scilab functions */
/* for good efficiency of the hash code */
#define MAXELEMENTFUNCTIONLIST 1536
/*-----------------------------------------------------------------------------------*/
/** 
 * Create the hashtable of Scilab functions
 * @param nel TODO
 */
int	create_hashtable_scilab_functions(unsigned int nel);

/** 
 * Destroy the hashtable of Scilab functions
 */
void destroy_hashtable_scilab_functions(void);

/** 
 * Perform an action on the hashtable of Scilab functions
 * @param key 
 * @param name
 * @param data
 * @param action SCI_HFUNCTIONS_ACTION 
 * @return result of the operation
 */
int action_hashtable_scilab_functions(int *key,char *name, int *data, SCI_HFUNCTIONS_ACTION action);

/** 
 * Check if a function exists in the list
 * @param[in] name the name of the function
 * @return if there is this function or not
 */
BOOL ExistFunction(char *name);
/*-----------------------------------------------------------------------------------*/
#endif /* __HASHTABLE_CORE_H__ */
/*-----------------------------------------------------------------------------------*/
