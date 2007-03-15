/**
 * Skeleton for the gateway header file (declaration of the "links" 
 * between Scilab and the "library"
 * @author Sylvestre LEDRU INRIA 2007
 */

#ifndef __GW_SKELETON__
#define __GW_SKELETON__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include <string.h>
#include "machine.h"
#include "stack-c.h"

int C2F(gw_skeleton)(void);

typedef int (*Skeleton_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct
{
  Skeleton_Interf f;    /** function **/
  char *name;      /** its name **/
} SkeletonTable;

// Declaration of all the profile function declared and 
// used in sci_gateway/*/
int C2F(sci_skeleton_c) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_skeleton_f) _PARAMS((char *fname,unsigned long fname_len));


#endif /*  __GW_SKELETON__ */

