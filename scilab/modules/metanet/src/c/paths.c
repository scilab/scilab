
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "localization.h"
#include "MALLOC.h"
#include "paths.h"
#include "cerro.h"

static char str[80];

void NodesToPath(int *nodes,int **p,int *psize,int *la,int *lp,int *ls)
{
  int a,i,j,n1,n2;
  for (i = 1; i <= *psize; i++) {
    n1 = nodes[*psize-i+1]; n2 = nodes[*psize-i];
    a = 0;
    for (j = lp[n1-1]; j <= lp[n1]-1; j++) {
      if (n2 == ls[j-1]) {
	a = la[j-1];
	break;
      }
    }
    if (a == 0) {
      *psize = 0;
      return;
    }
    (*p)[i-1] = a;
  }
}

void C2F(prevn2p)(int *i,int *j,int *m,int *n,int *la,int *lp,int *ls,int *direct,int *pln,int **p,int *psize)
{
  int *nodes;
  int k,nn;
  if (*i < 0 || *i > *n) {
    sprintf(str,_("Bad internal node number %d"),*i);
    cerro(str);
    return;
  }
  if (*j < 0 || *j > *n) {
    sprintf(str,_("Bad internal node number %d"),*j);
    cerro(str);
    return;
  }
  if ((nodes = (int *)MALLOC((*m + 1) * sizeof(int))) == NULL) {
    cerro(_("Running out of memory"));
    return;
  }
  nn = 0;
  nodes[nn++] = *j;
  k = 0;
  /* the first time, the test must be always verified for dealing with
     circuits */
  while (k != *i) {
    if (nn == 1) k = *j;
    k = pln[k-1];
    nodes[nn++] = k;
    if (k <= 0 || k > *n || nn > *n + 1) {
      /* there is no path */
      *psize = 0;
      return;
    }
  }
  *psize = nn - 1;
  if ((*p = (int *)MALLOC(*psize * sizeof(int))) == NULL) {
    cerro(_("Running out of memory"));
    return;
  }
  NodesToPath(nodes,p,psize,la,lp,ls);
  FREE(nodes);
}

void C2F(ns2p)(int *nodes,int *nsize,int **p,int *psize,int *la,int *lp,int *ls,int *n)
{
  int a,i,j,n1,n2;
  *psize = *nsize - 1;
  if ((*p = (int *)MALLOC(*psize * sizeof(int))) == NULL) {
    cerro(_("Running out of memory"));
    return;
  }
  for (i = 1; i <= *psize; i++) {
    n1 = nodes[i-1];
    if ((i == 1) && (n1 < 0 || n1 > *n)) {
      sprintf(str,_("Bad internal node number %d"),n1);
      cerro(str);
      return;
    }
    n2 = nodes[i];
    if (n2 < 0 || n2 > *n) {
      sprintf(str,_("Bad internal node number %d"),n2);
      cerro(str);
      return;
    }
    a = 0;
    for (j = lp[n1-1]; j <= lp[n1]-1; j++) {
      if (n2 == ls[j-1]) {
	a = la[j-1];
	break;
      }
    }
    if (a == 0) {
      *psize = 0;
      return;
    }
    (*p)[i-1] = a;
  }
}

void C2F(p2ns)(int *p,int *psize,int **nodes,int *nsize,int *la,int *lp,int *ls,int *direct,int *m,int *n)
{
  int ma,a,i,j,k,n1,n2;
  *nsize = *psize + 1;
  if ((*nodes = (int *)MALLOC(*nsize * sizeof(int))) == NULL) {
    cerro(_("Running out of memory"));
    return;
  }
  if (*direct == 1) ma = *m; else ma = *m / 2;
  for (i = 1; i <= *psize; i++) {
    a = p[i-1];
    if (a < 0 || a > ma) {
      sprintf(str,_("Bad internal arc number %d"),a);
      cerro(str);
      return;
    }
    /* find nodes n1 and n2 of arc a */
    n1 = 0; n2 = 0;
    for (j = 1; j <= *n; j++) {
      for (k = lp[j-1]; k <= lp[j]-1; k++)
	if (la[k-1] == a) {
	  n1 = j; n2 = ls[k-1];
	  break;
	}
      if (n1 != 0) break;
    }
    if (n1 == 0) {
      *nsize = 0;
      return;
    }
    /* directed graph */
    if (*direct == 1) {
      if (i == 1) (*nodes)[i-1] = n1;
      if (i!= 1 && (*nodes)[i-1] != n1) {
	*nsize = 0;
	return;
      }
      (*nodes)[i] = n2;
    }
    /* undirected graph */
    else {
      if (i == 1) {
	(*nodes)[0] = n1; (*nodes)[1] = n2;
      }
      else if (i == 2) {
	if (n1 == (*nodes)[0]) {
	  (*nodes)[0] = (*nodes)[1];
	  (*nodes)[1] = n1;
	  (*nodes)[2] = n2;
	}
	else if (n1 == (*nodes)[1]) {
	  (*nodes)[2] = n2;
	}
	else if (n2 == (*nodes)[0]) {
	  (*nodes)[0] = (*nodes)[1];
	  (*nodes)[1] = n2;
	  (*nodes)[2] = n1;
	}
	else if (n2 == (*nodes)[1]) {
	  (*nodes)[2] = n1;
	}
	else {
	  *nsize = 0;
	  return;
	}
      }
      else {
	if (n1 == (*nodes)[i-1]) (*nodes)[i] = n2;
	else if (n2 == (*nodes)[i-1]) (*nodes)[i] = n1;
	else {
	  *nsize = 0;
	  return;
	}
      }
    }
  }
}

void C2F(edge2st)(int *n,int *alpha,int **tree,int *treesize)
{
  int i;
  *treesize = *n - 1;
  if ((*tree = (int *)MALLOC(*treesize * sizeof(int))) == NULL) {
    cerro(_("Running out of memory"));
    return;
  }
  for (i = 1; i <= *n - 1; i++) {
    if (alpha[i] < 0) {
      *treesize=0;
      return;
    }
    (*tree)[i-1] = alpha[i];
  }
}

void C2F(prevn2st)(int *n,int *nodes,int **tree,int *treesize,int *la,int *lp,int *ls)
{
  int i,in,j,nt,indic;
  *treesize = *n - 1;
  if ((*tree = (int *)MALLOC(*treesize * sizeof(int))) == NULL) {
    cerro(_("Running out of memory"));
    return;
  }
  nt = 0;
  indic=0;
  for (i = 1; i <= *n; i++) {
    in = nodes[i-1];
    if (in == 0) continue;
    /* arc (in,i) belongs to tree */
    indic=1;
    for (j = lp[in-1]; j <= lp[in]-1; j++) {
      if (ls[j-1] == i)	{
	(*tree)[nt++] = la[j-1];
	break;
      }
    }
  }
  if (indic == 0) *treesize=0;
}
