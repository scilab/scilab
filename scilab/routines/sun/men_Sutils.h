#ifndef MEN_SUTIL_PROTO
#define MEN_SUTIL_PROTO

/** men_Sutils.c **/
/* Copyright Inria/Enpc */

#include "../machine.h"
  
extern void strwidth __PARAMS((char *, int *, int *));  
extern void ScilabStr2C __PARAMS((int *n, int *, char **, int *));  
extern void ScilabMStr2CM __PARAMS((int *, int *, int *, char ***, int *));  
extern void ScilabMStr2C __PARAMS((int *, int *, int *, char **, int *));  
extern void ScilabC2MStr2 __PARAMS((int*,int *,int *,char *,int *, int,int)); 
extern void ScilabCM2MStr __PARAMS((char **,int,int *,int *,int ,int *));  

#endif /*  MEN_SUTIL_PROTO */
