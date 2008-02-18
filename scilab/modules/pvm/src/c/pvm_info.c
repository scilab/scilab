/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1997-1999 - INRIA - Eric FLEURY
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/* Copyright (c) 1997 by Inria Lorraine.  All Rights Reserved */

/***
   NAME
     pvm_info
   PURPOSE
     
   NOTES
     PVM task information
   HISTORY
     fleury - Nov 18, 1997: Created.
     $Id: pvm_info.c,v 1.8 2005/10/22 18:53:10 cornet Exp $
***/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "machine.h" 
#include "stack-c.h" 
#include "sci_pvm.h"
#include "localization.h"

#include "MALLOC.h"
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
    Scierror(999,_("%s: An error occurred: %s\n"), "pvm_config", scipvm_error_msg(*info));
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
    strcpy((*name)[i], hostp[i].hi_name);
    
    if (((*arch)[i] = (char *) MALLOC((1+strlen(hostp[i].hi_arch)) * 
				      sizeof(char*))) == NULL) {
      *info = PvmNoMem;
      return;
    }
    strcpy((*arch)[i], hostp[i].hi_arch);
    
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
    Scierror(999,_("%s: An error occurred: %s\n"), "pvm_tasks", scipvm_error_msg(*info));
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
    strcpy((*name)[i], taskp[i].ti_a_out);
  }
}


void C2F(scipvmbufinfo)(int *bufid, int *bytes, int *msgtag, int * tid, int *res)
{
  *bytes = -1;
  *msgtag = -1;
  *tid = -1;
  *res = pvm_bufinfo(*bufid, bytes, msgtag, tid);
}


