/* Copyright (c) 1997 by Inria Lorraine.  All Rights Reserved */

/***
   NAME
     pvm_recv
   PURPOSE
     
   NOTES
     
   HISTORY
     fleury - Nov 19, 1997: Created.

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
#include "../calelm/sci_tools.h"
#include "sci_pvm.h"

#ifdef WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif

void C2F(scipvmrecv)(double *beginvar, int *maxsize, int *size,
		     int *tid,  int *tag, int *res)
{
  int bufid;
  int info;
  int msgbyte, msgtag, msgtid, buffsize;
  int i;
  int n;
  int *pack;
  int *ptr_int;
  double *ptr_double;

#ifdef DEBUG
  printf("pvm_recv: %f:%d:%d:%d\n", beginvar[0], *maxsize, *tid, *tag);  
#endif /* DEBUG */

  bufid = pvm_recv(*tid, *tag);
  if (bufid < 0) {
    (void) fprintf(stderr, "Error pvm_recv: %d\n", bufid);
    *res = bufid;
    return;
  }
  info = pvm_bufinfo(bufid, &msgbyte, &msgtag, &msgtid);
  if (info < 0) {
    (void) fprintf(stderr, "Error pvm_recv: -bufinfo- %d\n", info);
    pvm_freebuf(bufid);
    *res = info;
    return;
  }

#ifdef DEBUG
  (void) fprintf(stderr, "RECV: %d:%d:%d\n", msgbyte, msgtag, msgtid);
#endif /* DEBUG */

  /* unpack the size of the packing vector */
  info = pvm_upkint(&n, 1, 1);
  if (info < 0) {
    (void) fprintf(stderr, "Error pvm_recv: -pack- %d\n", info);
    pvm_freebuf(bufid);
    *res = info;
    return;
  }

#ifdef DEBUG
  (void) fprintf(stderr, "n=%d\n", n);
#endif /* DEBUG */

  /* unpack the packing vector */
  if ((pack = (int *) MALLOC(n * sizeof(int))) == NULL) {
    (void) fprintf(stderr, "Error malloc in pvm_recv\n");
    pvm_freebuf(bufid);
    *res = PvmNoMem;
    return;
  }
  info = pvm_upkint(pack, n, 1);
  if (info < 0) {
    (void) fprintf(stderr, "Error pvm_recv: -pack- %d\n", info);
    pvm_freebuf(bufid);
    *res = info;
    return;
  }

#ifdef DEBUG
  (void) fprintf(stderr, "RECV:");
  for (i = 0; i < n; ++i) {
    (void) fprintf(stderr, "%3d:", pack[i]);
  }
  (void) fprintf(stderr, "\n");
#endif /* DEBUG */

  msgbyte -= (n + 1)  * sizeof(int); /* on ne tient pas compte de la taille
					du vecteur de pack et de sa
					taille */
  if (msgbyte % sizeof(double)) {
    (void) fprintf(stderr, "Error pvm_recv: not double\n");
    pvm_freebuf(bufid);
    *res = PvmBadMsg;
    return;
  }
  
  buffsize = msgbyte/sizeof(double);

  *size = buffsize;
  if (*size > *maxsize) {
    (void) fprintf(stderr, "Error pvm_recv: Not enough memory: available=%d:needed=%d\n", *maxsize, *size);
    pvm_freebuf(bufid);
    *res = PvmNoMem;
    return;
  }

  /* UnPack the msg using the pack vect info */
  ptr_double = beginvar;
  ptr_int = (int*) beginvar;
  for (i = 0; i < n; i+=2) {
    if (pack[i] > 0) {		/* have to unpack some int */
      info = pvm_upkint(ptr_int, pack[i], 1);
      if (info < 0) {
	(void) fprintf(stderr, "Error pvm_send: -pack- %d\n", info);
	pvm_freebuf(bufid);
	*res = info;
	return;
      }
      ptr_int += pack[i] + (pack[i] % 2);
      ptr_double += ((pack[i]-1)/2 + 1);
    }
    if (pack[i+1] > 0) {	/* have to pack some double */
      info = pvm_upkdouble(ptr_double, pack[i+1], 1);
      if (info < 0) {
	(void) fprintf(stderr, "Error pvm_send: -pack- %d\n", info);
	pvm_freebuf(bufid);
	*res = info;
	return;
      }
      ptr_int += (pack[i+1]*2);
      ptr_double += pack[i+1];
    }
  }
  *res = info;
  *tag = msgtag;
  *tid = msgtid;
  FREE(pack);
} /* scipvmrecv */


void 
#ifdef __STDC__
C2F(scipvmrecvvar)(int *tid,  int *tag, char *buff, int *res)
#else
C2F(scipvmrecvvar)(tid, tag, buff, res)
  int *tid; 
  int *tag;
  char *buff;
  int *res;
#endif 
{
  int bufid;
  int info;
  int msgbyte, msgtag, msgtid;
  int type;
  int mx, nx, type_x, ptr_x, size_x;
  int m, n;

  F2C(mycmatptr)(buff, &mx, &nx, &type_x, &ptr_x);
  
  switch (type_x) 
    {
    case TYPE_DOUBLE :
      size_x = mx * nx * sizeof(double);
      break;
    case TYPE_COMPLEX :
      size_x = mx * nx * sizeof(complex16);
    break;
    default :
      size_x = -1;
      (void) fprintf(stderr, "Error pvm_recv_var: Not scalar type\n");
      *res = PvmBadMsg;
      return;
    }
  bufid = pvm_recv(*tid, *tag);
  if (bufid < 0) {
    (void) fprintf(stderr, "Error pvm_recv: %d\n", bufid);
    *res = bufid;
    return;
  }
  info = pvm_bufinfo(bufid, &msgbyte, &msgtag, &msgtid);
  if (info < 0) {
    (void) fprintf(stderr, "Error pvm_recv: %d\n", info);
    *res = info;
    return;
  }

  msgbyte -= 3 * sizeof(int);   /* on ne tient pas compte de m,n,type */
  
  if (msgbyte != size_x) {
    (void) fprintf(stderr, "Error pvm_recv: size of %s != size of msg\n", 
		   buff);
    pvm_freebuf(bufid);
    *res = PvmNoMem;
    return;
  }
  *res = pvm_upkint(&m, 1, 1);
  *res = pvm_upkint(&n, 1, 1);
  *res = pvm_upkint(&type, 1, 1);
  SET_NB_ROW(stk(ptr_x),m);
  SET_NB_COL(stk(ptr_x),n);
  switch (type) 
    {
    case TYPE_DOUBLE :
      SET_TYPE_DOUBLE(stk(ptr_x));
      *res = pvm_upkdouble(stk(ptr_x), m*n, 1);
      break;
    case TYPE_COMPLEX :
      SET_TYPE_COMPLEX(stk(ptr_x));
      *res = pvm_upkdcplx(stk(ptr_x), m*n, 1);
      break;
    default :
      (void) fprintf(stderr, "Error pvm_recv_var: Not scalar type\n");
      *res = PvmBadMsg;
      return;
    }
} /* scipvmrecvvar */

