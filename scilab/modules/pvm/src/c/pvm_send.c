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
     pvm_send
   PURPOSE
     pvm_send send a scilab variable 
   NOTES
     For each variable, we send:
        - n: size of the pack array
	- p: pack array vect: (real,int)+
        - v: the variable.
   HISTORY
     fleury - Nov 19, 1997: Created.
     $Id: pvm_send.c,v 1.5 2005/01/07 20:49:25 cornet Exp $
***/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sci_pvm.h"
#include "sci_tools.h"
#include "stack-c.h"


void C2F(scipvmsend)(int *taskId, int *p, 
		     int *pack, int *n, 
		     double *buff,
		     int *msgtag, int *res)
{
  int info, bufid;
  double *ptr_double;
  int *ptr_int;
  int i;

#ifdef DEBUG
  (void) fprintf(stdout, "SEND: %d:%d:%d:%d:%d:%d:%p:%f:%f:%f:%f:%f:%f:%f\n", 
                 *taskId, *p, *n, *msgtag, pack[0], pack[1], buff, 
                 buff[0], buff[1], buff[2], buff[3], buff[4], buff[5], buff[6]);
  
  (void) fprintf(stderr, "SEND:");
  for (i = 0; i < *n; ++i) {
    (void) fprintf(stderr, "%3d:", pack[i]);
  }
  (void) fprintf(stderr, "\n");
#endif /* DEBUG */

  bufid = pvm_initsend(PvmDataDefault);
  if (bufid < 0) {
    (void) fprintf(stderr, "Error pvm_send - init: %d\n", bufid);
    *res = bufid;
    return;
  }
  /* Pack the size of the packing vector */
  info = pvm_pkint(n, 1, 1);
  if (info < 0) {
    (void) fprintf(stderr, "Error pvm_send: -pack- %d\n", info);
    pvm_freebuf(bufid);
    *res = info;
    return;
  }
  /* Pack the packing vector */
  info = pvm_pkint(pack, *n, 1);
  if (info < 0) {
    (void) fprintf(stderr, "Error pvm_send: -pack- %d\n", info);
    pvm_freebuf(bufid);
    *res = info;
    return;
  }
  
  /* Pack the msg using the packing vector info */
  ptr_double = buff;
  ptr_int = (int*) buff;

  for (i = 0; i < *n; i+=2) {
    if (pack[i] > 0) {		/* have to pack some int */
      info = pvm_pkint(ptr_int, pack[i], 1);
      if (info < 0) {
	(void) fprintf(stderr, "Error pvm_send: -pack- %d\n", info);
	pvm_freebuf(bufid);
	*res = info;
	return;
      }

#ifdef DEBUG
      {
	int tmp_i;
	for (tmp_i = 0; tmp_i < pack[i]; ++tmp_i) {
	  (void) fprintf(stderr, "%d:", ptr_int[tmp_i]);
	}
      }
#endif /* DEBUG */

      ptr_int += pack[i] + (pack[i] % 2);
      ptr_double += ((pack[i]-1)/2 + 1);
    }
    if (pack[i+1] > 0) {	/* have to pack some double */
      info = pvm_pkdouble(ptr_double, pack[i+1], 1);
      if (info < 0) {
	(void) fprintf(stderr, "Error pvm_send: -pack- %d\n", info);
	pvm_freebuf(bufid);
	*res = info;
	return;
      }

#ifdef DEBUG
      {
	int tmp_i;
	for (tmp_i = 0; tmp_i < pack[i+1]; ++tmp_i) {
	  (void) fprintf(stderr, "%f:", ptr_double[tmp_i]);
	}
      }
#endif /* DEBUG */

      ptr_int += (pack[i+1]*2);
      ptr_double += pack[i+1];
    }
  }

#ifdef DEBUG
  (void) fprintf(stderr, "\n");
#endif /* DEBUG */

  if (*p == 1) {
    *res = pvm_send(taskId[0], *msgtag);
  }
  else
    *res = pvm_mcast(taskId, *p, *msgtag);
} /* scipvmsend */



void C2F(scipvmsendvar)(int *taskId, int *p, char *buff, int *msgtag, int *res)
{
  int info, bufid, type;
  int mx, nx, type_x, ptr_x;


  F2C(mycmatptr)(buff, &mx, &nx, &type_x, &ptr_x);

  bufid = pvm_initsend(PvmDataDefault);
  if (bufid < 0) {
    (void) fprintf(stderr, "Error pvm_send_var - init: %d\n", bufid);
    pvm_freebuf(bufid);
    *res = bufid;
    return;
  }
  info = pvm_pkint(&mx, 1, 1);
  if (info < 0) {
    (void) fprintf(stderr, "Error pvm_send_var: -pack- %d\n", info);
    pvm_freebuf(bufid);
    *res = info;
    return;
  }
  info = pvm_pkint(&nx, 1, 1);
  if (info < 0) {
    (void) fprintf(stderr, "Error pvm_send_var: -pack- %d\n", info);
    pvm_freebuf(bufid);
    *res = info;
    return;
  }
  if (type_x == TYPE_COMPLEX) {
    type = TYPE_COMPLEX;
    info = pvm_pkint(&type, 1, 1);
    if (info < 0) {
      (void) fprintf(stderr, "Error pvm_send_var: -pack- %d\n", info);
      pvm_freebuf(bufid);
      *res = info;
      return;
    }
    info = pvm_pkdcplx(stk(ptr_x), mx * nx, 1);
    if (info < 0) {
      (void) fprintf(stderr, "Error pvm_send_var: -pack- %d\n", info);
      pvm_freebuf(bufid);
      *res = info;
      return;
    }
  } else {    
    type = TYPE_DOUBLE;
    info = pvm_pkint(&type, 1, 1);
    if (info < 0) {
      (void) fprintf(stderr, "Error pvm_send_var: -pack- %d\n", info);
      pvm_freebuf(bufid);
      *res = info;
      return;
    }
    info = pvm_pkdouble(stk(ptr_x), mx * nx, 1);
    if (info < 0) {
      (void) fprintf(stderr, "Error pvm_send_var: -pack- %d\n", info);
      pvm_freebuf(bufid);
      *res = info;
      return;
    }
  }
  
  if (*p == 1)
    *res = pvm_send(taskId[0], *msgtag);
  else
    *res = pvm_mcast(taskId, *p, *msgtag);
} /* scipvmsendvar */
