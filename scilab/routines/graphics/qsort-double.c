/*-----------------------------------------------------------------------------------*/
/* INRIA Scilab */
/*-----------------------------------------------------------------------------------*/
static int CNAME(swapcode,double)(char *parmi,char* parmj,int n, int inc) 
{
	int i = n;
	register double *pi = (double *) (parmi); 		
	register double *pj = (double *) (parmj); 
	register int inc1 = inc/sizeof(double);
	do { 						
		register double t = *pi;		
		*pi = *pj;				
		*pj = t;				
		pi += inc1;
		pj += inc1;
	} while (--i > 0);				
	return(0);
  /*int i = n;
  register double *pi = (double *) (parmi); 		
  register double *pj = (double *) (parmj); 
  do { 						
    register double t = *pi;		
    *pi++ = *pj;				
    *pj++ = t;				
  } while (--i > 0);				
  return(0);*/


}
/*-----------------------------------------------------------------------------------*/
static int CNAME(compareC,double)(char *i,char *j)
{
  if ( *((double *)i) > *((double *)j))
    return (1);
  if ( *((double *)i) < *((double *)j))
    return (-1);
  return (0);
}
/*-----------------------------------------------------------------------------------*/
static int CNAME(compareD,double)(char *i,char *j)
{
  if ( *((double *)i) < *((double *)j))
    return (1);
  if ( *((double *)i) > *((double *)j))
    return (-1);
  return (0);
}
/*-----------------------------------------------------------------------------------*/
/******************************************************
 * Column sort of a matrix 
 ******************************************************/
void CNAME(ColSort,double)(double *a,int *ind,int flag,int n,int p,char dir)
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
		sizeof(double),sizeof(int), 
		(dir == 'i' ) ? CNAME(compareC,double) : CNAME(compareD,double),
		CNAME(swapcode,double),swapcodeind);
    }
}
/*-----------------------------------------------------------------------------------*/
/******************************************************
 * Row sort of a matrix 
 ******************************************************/
void CNAME(RowSort,double)(double *a,int *ind,int flag,int n,int p,char dir)
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
		n*sizeof(double),n*sizeof(int), 
		(dir == 'i' ) ? CNAME(compareC,double):CNAME(compareD,double),
		CNAME(swapcode,double),swapcodeind);
    }
}
/*-----------------------------------------------------------------------------------*/
/******************************************************
 * Global sort of a Matrix
 ******************************************************/
void CNAME(GlobalSort,double)(double *a,int *ind,int flag,int n,int p,char dir)
{  
  int i;
  if ( flag == 1) 
    {
      for ( i = 0 ; i < n*p ; i++) 
	ind[i]= i+1;
    }
  sciqsort((char *) (a),(char *) (ind),flag, n*p, 
	    sizeof(double),sizeof(int), 
	    (dir == 'i' ) ? CNAME(compareC,double):CNAME(compareD,double),
	    CNAME(swapcode,double),swapcodeind);
}
/*-----------------------------------------------------------------------------------*/
/*******************************************************
 *  lexicographic order with Rows ind is of size n
 *  ind gives the permutation of the rows which is applied 
 *  to sort them 
 ******************************************************/
static int CNAME(lexicols,double) =1;
static int CNAME(lexirows,double) =1;

static void CNAME(setLexiSize,double)(int n,int p) 
{
  CNAME(lexicols,double) = p;
  CNAME(lexirows,double) = n;
}
/*-----------------------------------------------------------------------------------*/
static  int CNAME(LexiRowcompareC,double)(double *i,double *j)
{
  int jc;
  for ( jc = 0 ; jc < CNAME(lexicols,double) ; jc++) 
    {
      if (*i > *j)
	return (1);
      if (*i < *j)
	return (-1);
      i += CNAME(lexirows,double);
      j += CNAME(lexirows,double);
    }
  return (0);
}
/*-----------------------------------------------------------------------------------*/
static  int CNAME(LexiRowcompareD,double)(double *i, double*j)
{
  int jc;
  for ( jc = 0 ; jc < CNAME(lexicols,double) ; jc++) 
    {
      if (*i < *j)
	return (1);
      if (*i > *j)
	return (-1);
      i += CNAME(lexirows,double);
      j += CNAME(lexirows,double);
    }
  return (0);
}
/*-----------------------------------------------------------------------------------*/
static int CNAME(LexiRowswapcode,double)(char *parmi,char * parmj,int n) 
{ 		
  int i = n,j;
  register double *pi = (double *) (parmi); 		
  register double *pj = (double *) (parmj); 
  /* if ( n!= 1) printf(" swapcode avec n != 1\n"); */
  do { 
    for ( j = 0 ; j < CNAME(lexicols,double) ; j++) 
      {
	register double t = *(pi +CNAME(lexirows,double)*j);		
	*(pi + CNAME(lexirows,double)*j) = *(pj+CNAME(lexirows,double)*j);				
	*(pj + CNAME(lexirows,double)*j) = t;	
      }
    pi++;
    pj++;
  } while (--i > 0);				
  return(0);
}
/*-----------------------------------------------------------------------------------*/
void CNAME(LexiRow,double)(double *a,int *ind,int flag,int n,int p,char dir)
{
  int i;
  CNAME(setLexiSize,double)(n,p);
  if ( flag == 1) 
    {
      for ( i = 0 ; i < n ; i++) 
	ind[i]= i+1;
    }
  sciqsort((char *) (a),(char *) (ind),flag, n, 
	    sizeof(double),sizeof(int), 
	    (dir == 'i' ) ? CNAME(LexiRowcompareC,double):CNAME(LexiRowcompareD,double),
	    CNAME(LexiRowswapcode,double),swapcodeind);
}
/*-----------------------------------------------------------------------------------*/
/******************************************************
 *  lexicographic order with Cols ind is of size p
 *  ind gives the permutation of the column which is applied 
 *  to sort them 
 ******************************************************/
static  int CNAME(LexiColcompareC,double)(double *i,double *j)
{
  int ic;
  for ( ic = 0 ; ic < CNAME(lexirows,double) ; ic++) 
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
/*-----------------------------------------------------------------------------------*/
static  int CNAME(LexiColcompareD,double)(double *i,double *j)
{
  int ic;
  for ( ic = 0 ; ic < CNAME(lexirows,double) ; ic++) 
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
/*-----------------------------------------------------------------------------------*/
static int CNAME(LexiColswapcode,double)(char *parmi,char* parmj,int n) 
{ 		
  int i = n,ir;
  register double *pi = (double *) (parmi); 		
  register double *pj = (double *) (parmj); 
  /* if ( n!= 1) printf(" swapcode avec n != 1\n"); */
  do { 
    for ( ir = 0 ; ir < CNAME(lexirows,double) ; ir++) 
      {
	register double t = *(pi +ir);		
	*(pi +ir) = *(pj+ir);				
	*(pj +ir) = t;	
      }
    pi += CNAME(lexirows,double) ;
    pj += CNAME(lexirows,double) ;
  } while (--i > 0);				
  return(0);
}
/*-----------------------------------------------------------------------------------*/
void CNAME(LexiCol,double)(double *a,int *ind,int flag,int n,int p,char dir)
{
  int i;
  CNAME(setLexiSize,double)(n,p);
  if ( flag == 1) 
    {
      for ( i = 0 ; i < p ; i++) 
	ind[i]= i+1;
    }
  sciqsort((char *) (a),(char *) (ind),flag, p, 
	    n*sizeof(double),sizeof(int), 
	    (dir == 'i' ) ? CNAME(LexiColcompareC,double):CNAME(LexiColcompareD,double),
	    CNAME(LexiColswapcode,double),
	    swapcodeind);
}
/*-----------------------------------------------------------------------------------*/
