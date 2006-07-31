/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2000 Enpc/Jean-Philippe Chancelier
 *    jpc@cereve.enpc.fr 
 --------------------------------------------------------------------------*/
#ifndef __SCI_GSORT_GRAPH__
#define __SCI_GSORT_GRAPH__

static void CNAME(ColSort,TYPE)();
static void CNAME(RowSort,TYPE)();
static void CNAME(GlobalSort,TYPE)();
static void CNAME(LexiRow,TYPE)();
static void CNAME(LexiCol,TYPE)();
static void CNAME(inita,TYPE)();
static void CNAME(afficher,TYPE)();
static void CNAME(sorttest,TYPE)();


/******************************************************
 * Generic code for Sorting Matrices a[i+n*j] 
 * This code is inserted in gsort.c 
 * with TYPE == double or type = int 
 ******************************************************/

static int CNAME(swapcode,TYPE)(parmi, parmj, n) 
     char *parmi,*parmj;
     int n;
{ 		
  int i = n;
  register TYPE *pi = (TYPE *) (parmi); 		
  register TYPE *pj = (TYPE *) (parmj); 
  do { 						
    register TYPE t = *pi;		
    *pi++ = *pj;				
    *pj++ = t;				
  } while (--i > 0);				
  return(0);
}

static int CNAME(compareC,TYPE)(i,j)
     char *i,*j;
{
  if ( *((TYPE *)i) > *((TYPE *)j))
    return (1);
  if ( *((TYPE *)i) < *((TYPE *)j))
    return (-1);
  return (0);
}

static int CNAME(compareD,TYPE)(i,j)
     char *i,*j;
{
  if ( *((TYPE *)i) < *((TYPE *)j))
    return (1);
  if ( *((TYPE *)i) > *((TYPE *)j))
    return (-1);
  return (0);
}

/******************************************************
 * Column sort of a matrix 
 ******************************************************/

static void CNAME(ColSort,TYPE)(a,ind,flag,n,p,dir)
     TYPE *a;
     int *ind;
     int flag,n,p;
     char dir;
{
  int i,j;
  if ( flag == 1) 
    {
      for ( j= 0 ; j < p ; j++ ) 
	{
	  for ( i = 0 ; i < n ; i++) 
	    ind[i+n*j]= i+1;
	}
    }
  for ( j= 0 ; j < p ; j++ ) 
    {
      sciqsort((char *) (a+n*j),(char *) (ind+n*j),flag, n, 
	       sizeof(TYPE),sizeof(int), 
	       (dir == 'c' ) ? CNAME(compareC,TYPE) : CNAME(compareD,TYPE),
	       CNAME(swapcode,TYPE),swapcodeind);
    }
}

/******************************************************
 * Row sort of a matrix 
 ******************************************************/

static void CNAME(RowSort,TYPE)(a,ind,flag,n,p,dir)
     TYPE *a;
     int *ind;
     int n,p,flag;
     char dir;
{  
  int i,j;
  if ( flag == 1) 
    {
      for ( i = 0 ; i < n ; i++) 
	{
	  for ( j= 0 ; j < p ; j++ ) 
	    {
	      ind[i+n*j]= j+1;
	    }
	}
    }
  for ( i = 0 ; i < n ; i++) 
    {
      sciqsort((char *) (a+i),(char *) (ind+i),flag, p, 
	       n*sizeof(TYPE),n*sizeof(int), 
	       (dir == 'c' ) ? CNAME(compareC,TYPE):CNAME(compareD,TYPE),
	       CNAME(swapcode,TYPE),swapcodeind);
    }
}


/******************************************************
 * Global sort of a Matrix
 ******************************************************/

static void CNAME(GlobalSort,TYPE)(a,ind,flag,n,p,dir)
     TYPE *a;
     int *ind;
     int n,p,flag;
     char dir;
{  
  int i,j;
  if ( flag == 1) 
    {
      for ( i = 0 ; i < n*p ; i++) 
	ind[i]= i+1;
    }
  sciqsort((char *) (a),(char *) (ind),flag, n*p, 
	   sizeof(TYPE),sizeof(int), 
	   (dir == 'c' ) ? CNAME(compareC,TYPE):CNAME(compareD,TYPE),
	   CNAME(swapcode,TYPE),swapcodeind);
}

/*******************************************************
 *  lexicographic order with Rows ind is of size n
 *  ind gives the permutation of the rows which is applied 
 *  to sort them 
 ******************************************************/

static int CNAME(lexicols,TYPE) =1;
static int CNAME(lexirows,TYPE) =1;

static int CNAME(setLexiSize,TYPE)(n,p) 
     int p,n;
{
  CNAME(lexicols,TYPE) = p;
  CNAME(lexirows,TYPE) = n;
}

static  int CNAME(LexiRowcompareC,TYPE)(int *i, int *j)
{
  int jc;
  for ( jc = 0 ; jc < CNAME(lexicols,TYPE) ; jc++) 
    {
      if (*i > *j)
	return (1);
      if (*i < *j)
	return (-1);
      i += CNAME(lexirows,TYPE);
      j += CNAME(lexirows,TYPE);
    }
  return (0);
}
static  int CNAME(LexiRowcompareD,TYPE)(int *i, int *j)
{
  int jc;
  for ( jc = 0 ; jc < CNAME(lexicols,TYPE) ; jc++) 
    {
      if (*i < *j)
	return (1);
      if (*i > *j)
	return (-1);
      i += CNAME(lexirows,TYPE);
      j += CNAME(lexirows,TYPE);
    }
  return (0);
}

static int CNAME(LexiRowswapcode,TYPE)(parmi, parmj, n) 
     char *parmi,*parmj;
     int n;
{ 		
  int i = n,j;
  register TYPE *pi = (TYPE *) (parmi); 		
  register TYPE *pj = (TYPE *) (parmj); 
  /** if ( n!= 1) printf(" swapcode avec n != 1\n"); **/
  do { 
    for ( j = 0 ; j < CNAME(lexicols,TYPE) ; j++) 
      {
	register TYPE t = *(pi +CNAME(lexirows,TYPE)*j);		
	*(pi + CNAME(lexirows,TYPE)*j) = *(pj+CNAME(lexirows,TYPE)*j);				
	*(pj + CNAME(lexirows,TYPE)*j) = t;	
      }
    pi++;
    pj++;
  } while (--i > 0);				
  return(0);
}


static void CNAME(LexiRow,TYPE)(a,ind,flag,n,p,dir)
     int *a,*ind;
     int n,p;
     char dir;
{
  int i,j;
  CNAME(setLexiSize,TYPE)(n,p);
  if ( flag == 1) 
    {
      for ( i = 0 ; i < n ; i++) 
	  ind[i]= i+1;
    }
  sciqsort((char *) (a),(char *) (ind),flag, n, 
	   sizeof(TYPE),sizeof(int), 
	   (dir == 'c' ) ? CNAME(LexiRowcompareC,TYPE):CNAME(LexiRowcompareD,TYPE),
	   CNAME(LexiRowswapcode,TYPE),swapcodeind);
}

/******************************************************
 *  lexicographic order with Cols ind is of size p
 *  ind gives the permutation of the column which is applied 
 *  to sort them 
 ******************************************************/

static  int CNAME(LexiColcompareC,TYPE)(i,j)
     TYPE *i,*j;
{
  int ic;
  for ( ic = 0 ; ic < CNAME(lexirows,TYPE) ; ic++) 
    {
      if (*i > *j)
	return (1);
      if (*i < *j)
	return (-1);
      i++;
      j++;
    }
  return (0);
}
static  int CNAME(LexiColcompareD,TYPE)(i,j)
     TYPE *i,*j;
{
  int ic;
  for ( ic = 0 ; ic < CNAME(lexirows,TYPE) ; ic++) 
    {
      if (*i < *j)
	return (1);
      if (*i > *j)
	return (-1);
      i++;
      j++;
    }
  return (0);
}

static int CNAME(LexiColswapcode,TYPE)(parmi, parmj, n) 
     char *parmi,*parmj;
     int n;
{ 		
  int i = n,ir;
  register TYPE *pi = (TYPE *) (parmi); 		
  register TYPE *pj = (TYPE *) (parmj); 
  /** if ( n!= 1) printf(" swapcode avec n != 1\n"); **/
  do { 
    for ( ir = 0 ; ir < CNAME(lexirows,TYPE) ; ir++) 
      {
	register TYPE t = *(pi +ir);		
	*(pi +ir) = *(pj+ir);				
	*(pj +ir) = t;	
      }
    pi += CNAME(lexirows,TYPE) ;
    pj += CNAME(lexirows,TYPE) ;
  } while (--i > 0);				
  return(0);
}


static void CNAME(LexiCol,TYPE)(a,ind,flag,n,p,dir)
     TYPE *a;
     int *ind;
     int n,p;
     char dir;
{
  int i,j;
  CNAME(setLexiSize,TYPE)(n,p);
  if ( flag == 1) 
    {
      for ( i = 0 ; i < p ; i++) 
	  ind[i]= i+1;
    }
  sciqsort((char *) (a),(char *) (ind),flag, p, 
	   n*sizeof(TYPE),sizeof(int), 
	   (dir == 'c' ) ? CNAME(LexiColcompareC,TYPE):CNAME(LexiColcompareD,TYPE),
	   CNAME(LexiColswapcode,TYPE),
	   swapcodeind);
}

#endif /*__SCI_GSORT_GRAPH__*/


