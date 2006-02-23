/* Copyright (c) 1997 by Inria Lorraine.  All Rights Reserved */

/***
   NAME
     pvm_info
   PURPOSE
     
   NOTES
     PVM task information
   HISTORY
     fleury - Nov 18, 1997: Created.

***/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN32
  #include "../../pvm3/include/pvm3.h"
#else
  #include "pvm3.h"
#endif
#include "../machine.h" 
#include "../stack-c.h" 
#include "sci_pvm.h"

#ifdef WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif
/*---------------------------------------------------
 * get configuration informations 
 *---------------------------------------------------*/

void C2F(scipvmconfig)(int *nhost, int *narch, int **dtid, 
		       char ***name, char ***arch, int **speed, 
		       int *n, int *info)
{
  int i;
  struct pvmhostinfo *hostp;
  
  *info = pvm_config(nhost, narch, &hostp);
  *n = *nhost;
  
  if (*info) {
    Scierror(999,"pvm_config: Error  %d\n", *info);
    *n = 0;
    *name = NULL;
    *arch = NULL;
    *nhost = 0;
    *narch = 0;
    return;
  }
  if (((*name) = (char **) MALLOC((*nhost+1) * sizeof(char**))) == NULL) {
    *info = PvmNoMem;
    return;
  }
  (*name)[*nhost]=NULL;
  if (((*arch) = (char **) MALLOC((*nhost+1) * sizeof(char**))) == NULL) {
    *info = PvmNoMem;
    return;
  }
  (*arch)[*nhost]=NULL;
  if ((*dtid = (int *) MALLOC(*nhost * sizeof(int))) == NULL) {
    *info = PvmNoMem;
    return;
  }
  if ((*speed = (int *) MALLOC(*nhost * sizeof(int))) == NULL) {
    *info = PvmNoMem;
    return;
  }
  
  for (i = 0; i < *nhost; ++i) {
    if (((*name)[i] = (char *) MALLOC((1+strlen(hostp[i].hi_name)) * 
				      sizeof(char*))) == NULL) {
      *info = PvmNoMem;
      return;
    }
    (void) sprintf((*name)[i], "%s", hostp[i].hi_name);
    
    if (((*arch)[i] = (char *) MALLOC((1+strlen(hostp[i].hi_arch)) * 
				      sizeof(char*))) == NULL) {
      *info = PvmNoMem;
      return;
    }
    (void) sprintf((*arch)[i], "%s", hostp[i].hi_arch);
    
    (*dtid)[i] =hostp[i].hi_tid;
    (*speed)[i] =hostp[i].hi_speed;
  }
}


/*-------------------------------------------------
 * informations about tasks 
 *-------------------------------------------------*/ 

void C2F(scipvmtasks)(int *where, int *ntask, 
		      int **tid, int **ptid, int **dtid, int **flag,
		      char ***name, int *n, int *info)
{
  int i;
  struct pvmtaskinfo *taskp;
  
  *info = pvm_tasks(*where, ntask, &taskp);
  *n = *ntask;

  if (*info) {
    Scierror(999,"pvm_tasks: Error in C routine pvm_tasks  %d\n", *info);
    *n = 0;
    *ntask = 0;
    *name = NULL;
    return;
  }
  if ((*tid = (int *) MALLOC(*ntask * sizeof(int))) == NULL) {
    *info = PvmNoMem;
    return;
  }
  if ((*ptid = (int *) MALLOC(*ntask * sizeof(int))) == NULL) {
    *info = PvmNoMem;
    return;
  }
  if ((*dtid = (int *) MALLOC(*ntask * sizeof(int))) == NULL) {
    *info = PvmNoMem;
    return;
  }
  if ((*flag = (int *) MALLOC(*ntask * sizeof(int))) == NULL) {
    *info = PvmNoMem;
    return;
  }
  if (((*name) = (char* *) MALLOC((*ntask+1) * sizeof(char**))) == NULL) {
    *info = PvmNoMem;
    return;
  }
  (*name)[*ntask]=NULL; 
  
  for (i = 0; i < *ntask; ++i) {
    (*tid)[i] =  taskp[i].ti_tid;
    (*ptid)[i] =  taskp[i].ti_ptid;
    (*dtid)[i] =  taskp[i].ti_host;
    (*flag)[i] =  taskp[i].ti_flag;
    if (((*name)[i] = (char *) MALLOC((1+strlen(taskp[i].ti_a_out)) * 
				      sizeof(char*))) == NULL) {
      *info = PvmNoMem;
      return;
    }
    (void) sprintf((*name)[i], "%s", taskp[i].ti_a_out);
  }
}


void C2F(scipvmbufinfo)(int *bufid, int *bytes, int *msgtag, int * tid, int *res)
{
  *bytes = -1;
  *msgtag = -1;
  *tid = -1;
  *res = pvm_bufinfo(*bufid, bytes, msgtag, tid);
}


