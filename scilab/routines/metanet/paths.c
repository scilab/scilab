#include <stdio.h>
#include <string.h>

#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif


#include "../machine.h"
#ifdef WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif


extern void cerro();

static char str[80];

/* NodesToPath converts a vector with node numbers into a path:
   NODES = [Nn,...,N2,N1] =>
   PATH = [A1,A2,...,An-1] with Ai = (Ni,Ni+1)
   n = *psize + 1 
*/
void NodesToPath(nodes,p,psize,la,lp,ls)
int *nodes,*psize,*la,*lp,*ls;
int **p;
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

/* prevn2p_ computes a path from node i to node j from a
   vector describing the previous nodes in path:
   node i belongs to path if pln[i] > 0 
   pln[i] is then the previous node in the sequence 
*/
void C2F(prevn2p)(i,j,m,n,la,lp,ls,direct,pln,p,psize)
int *i,*j,*m,*n,*la,*lp,*ls,*pln,*psize;
int **p;
{
  int *nodes;
  int k,nn;
  if (*i < 0 || *i > *n) {
    sprintf(str,"Bad internal node number %d",*i);
    cerro(str);
    return;
  }
  if (*j < 0 || *j > *n) {
    sprintf(str,"Bad internal node number %d",*j);
    cerro(str);
    return;
  }
  if ((nodes = (int *)MALLOC((*m + 1) * sizeof(int))) == NULL) {
    cerro("Running out of memory");
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
    cerro("Running out of memory");
    return;
  }
  NodesToPath(nodes,p,psize,la,lp,ls);
  FREE(nodes);
}

/* ns2p_ converts a node set into a path:
   NODES = [N1,N2,...,Nn] =>
   PATH = [A1,A2,...,An-1] with Ai = (Ni,Ni+1)
   *psize = *nsize - 1 
*/
void C2F(ns2p)(nodes,nsize,p,psize,la,lp,ls,n)
int *nodes,*nsize,*psize,*la,*lp,*ls,*n;
int **p;
{
  int a,i,j,n1,n2;
  *psize = *nsize - 1;
  if ((*p = (int *)MALLOC(*psize * sizeof(int))) == NULL) {
    cerro("Running out of memory");
    return;
  }
  for (i = 1; i <= *psize; i++) {
    n1 = nodes[i-1];
    if ((i == 1) && (n1 < 0 || n1 > *n)) {
      sprintf(str,"Bad internal node number %d",n1);
      cerro(str);
      return;
    }
    n2 = nodes[i];
    if (n2 < 0 || n2 > *n) {
      sprintf(str,"Bad internal node number %d",n2);
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

/* p2ns_ converts a path into a node set:
   PATH = [A1,A2,...,An] =>
   NODES = [N1,N2,...,Nn+1] => with Ai = (Ni,Ni+1)
   with *nsize = *psize + 1 
*/
void C2F(p2ns)(p,psize,nodes,nsize,la,lp,ls,direct,m,n)
int *p,*psize,*nsize,*la,*lp,*ls,*direct,*m,*n;
int **nodes;
{
  int ma,a,i,j,k,n1,n2;
  *nsize = *psize + 1;
  if ((*nodes = (int *)MALLOC(*nsize * sizeof(int))) == NULL) {
    cerro("Running out of memory");
    return;
  }
  if (*direct == 1) ma = *m; else ma = *m / 2;
  for (i = 1; i <= *psize; i++) {
    a = p[i-1];
    if (a < 0 || a > ma) {
      sprintf(str,"Bad internal arc number %d",a);
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

/* edge2st_ computes a spanning tree (root = node 1) from 
   an array alpha of connecting edge numbers */

void C2F(edge2st)(n,alpha,tree,treesize)
int *n,*alpha,*treesize;
int **tree;
{
  int i;
  *treesize = *n - 1;
  if ((*tree = (int *)MALLOC(*treesize * sizeof(int))) == NULL) {
    cerro("Running out of memory");
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

/* prevn2st_ computes a spanning tree (root = node i0) from
   a vector nodes describing the previous nodes in tree */

void C2F(prevn2st)(n,nodes,tree,treesize,la,lp,ls)
int *n,*nodes,*treesize,*la,*lp,*ls;
int **tree;
{
  int i,in,j,nt,indic;
  *treesize = *n - 1;
  if ((*tree = (int *)MALLOC(*treesize * sizeof(int))) == NULL) {
    cerro("Running out of memory");
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
