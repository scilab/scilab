/* Copyright (c) 1997 by Inria Lorraine.  All Rights Reserved */

/***
   NAME
     pvm_grp
   PURPOSE
     
   NOTES
     
   HISTORY
     fleury - Nov 25, 1997: Created.
     $Id: pvm_grp.c,v 1.1 2001/04/26 07:49:01 scilab Exp $
***/
#include <stdio.h>
#include <string.h>
#include "pvm3.h"
#include "../machine.h"

#include "../calelm/sci_tools.h"

#ifdef __STDC__ 
void 
C2F(scipvmjoingroup)(char *group, int *l, int *inum)
#else
void 
C2F(scipvmjoingroup)(group, l, inum)
  char *group;
  int *l; 
  int *inum;
#endif
{
  *inum = pvm_joingroup(group);
} /* scipvmjoingroup */



#ifdef __STDC__ 
void 
C2F(scipvmlvgroup)(char *group, int *l, int *res)
#else
void 
C2F(scipvmlvgroup)(group, l, res)
  char *group;
  int *l;
  int *res;
#endif
{
  *res = pvm_lvgroup(group);
} /* scipvmlvgroup */


#ifdef __STDC__ 
void 
C2F(scipvmgsize)(char *group, int *l, int *size)
#else
void 
C2F(scipvmgsize)(group, l, size)
  char *group;
  int *l;
  int *size;
#endif
{
  *size = pvm_gsize(group);
} /* scipvmgsize */


#ifdef __STDC__ 
void 
C2F(scipvmgettid)(char *group, int *l, int *inum, int *tid)
#else
C2F(scipvmgettid)(group, l, inum, tid)
  char *group;
  int *l; 
  int *inum;
  int *tid;
#endif
{
  *tid = pvm_gettid(group, *inum);
} /* scipvmgettid */


#ifdef __STDC__
void 
C2F(scipvmgetinst)(char *group, int *l, int *tid, int *inum)
#else
void 
C2F(scipvmgetinst)(group, l, tid, inum)
  char *group;
  int *l;
  int *tid; 
  int *inum;
#endif 
{
  *inum = pvm_getinst(group, *tid);
} /* scipvmgetinst */


#ifdef __STDC__
void 
C2F(scipvmbarrier)(char *group, int *l, int *count, int *res)
#else
void 
C2F(scipvmbarrier)(group, l, count, res)
  char *group;
  int *l;
  int *count; 
   int *res;
#endif 

{
  *res = pvm_barrier(group, *count);
} /* scipvmbarrier */


#ifdef __STDC__
void 
C2F(scipvmbcast)(char *group, int *l, 
		   int *pack, int *n, 
		   double *buff,
		   int *msgtag, int *res)
#else
void 
C2F(scipvmbcast)(group, l, pack, n, buff, msgtag, res)
  char *group;
  int *l;
  int *pack;
  int *n;
  double *buff;
  int *msgtag;
  int *res;
#endif 
{
  int info, bufid;
  double *ptr_double;
  int *ptr_int;
  int i;
  
  /*  (void) fprintf(stderr, "BCAST: %s,%d:%d:%d\n", group, *n, pack[0], pack[1]);*/

  bufid = pvm_initsend(PvmDataDefault);
  if (bufid < 0) {
    (void) fprintf(stderr, "Error pvm_bcast: -init- %d\n", bufid);
    *res = bufid;
    return;
  }

  /* Pack the size of the packing vector */
  info = pvm_pkint(n, 1, 1);
  if (info < 0) {
    (void) fprintf(stderr, "Error pvm_bcast: -pack- %d\n", info);
    pvm_freebuf(bufid);
    *res = info;
    return;
  }
  /* Pack the packing vector */
  info = pvm_pkint(pack, *n, 1);
  if (info < 0) {
    (void) fprintf(stderr, "Error pvm_bcast: -pack- %d\n", info);
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
	(void) fprintf(stderr, "Error pvm_bcast: -pack- %d\n", info);
	pvm_freebuf(bufid);
	*res = info;
	return;
      }
      ptr_int += pack[i] + (pack[i] % 2);
      ptr_double += ((pack[i]-1)/2 + 1);
    }
    if (pack[i+1] > 0) {	/* have to pack some double */
      info = pvm_pkdouble(ptr_double, pack[i+1], 1);
      if (info < 0) {
	(void) fprintf(stderr, "Error pvm_bcast: -pack- %d\n", info);
	pvm_freebuf(bufid);
	*res = info;
	return;
      }
      ptr_int += (pack[i+1]*2);
      ptr_double += pack[i+1];
    }
  }
  
  *res = pvm_bcast(group, *msgtag);
} /* scipvmbcast */


#ifdef __STDC__
void 
C2F(scipvmreduce)(char *func, int *l1,
		  double *buff, int *m, int *n, int *msgtag, 
		  char *group, int *l2, int *rootginst, int *res)
#else
void 
C2F(scipvmreduce)(func, l1, buff, m, n, msgtag, group, l2, rootginst, res)
  char *func;
  int *l1;
  double *buff;
  int *m;
  int *n;
  int *msgtag; 
  char *group;
  int *l2;
  int *rootginst;
  int *res;
#endif 
{
  int type, datatype;
  int size;
#ifdef __STDC__
  void (*op)(int *, void *, void *, int *, int *);
#else
  void (*op)();
#endif
  /* Check the type of the buff */
  type = TYPE(buff);
  size = *m * *n;
  switch (type)    
    {
    case TYPE_DOUBLE :
      datatype = PVM_DOUBLE;
      break;
    case TYPE_COMPLEX :
      datatype = PVM_DCPLX;
      SciToF77(buff, size, size);
      break;
    default :
      (void) fprintf(stderr, "Error pvm_reduce: Not scalar type\n");
      *res = PvmBadMsg;
      return;
    }
  /* Check the reduction operation */
  if (!strcmp(func, "Max")) {
    op = PvmMax;
  }
  else 
    if (!strcmp(func, "Min")) {
      op = PvmMin;
    } 
    else 
      if (!strcmp(func, "Sum")) {
	op = PvmSum;
      } 
      else 
	if (!strcmp(func, "Pro")) {
	  op = PvmProduct;
	} 
	else {
	  (void) fprintf(stderr, "Error pvm_reduce: Unknow reduction operation %s\n", func);
	  *res = PvmBadMsg;
	  return;
	}
  *res = pvm_reduce(op, buff, size, datatype, *msgtag, group, *rootginst);
  if (type == TYPE_COMPLEX)
    F77ToSci(buff, size, size);
  return;
} /* scipvmreduce */
