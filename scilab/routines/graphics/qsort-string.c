/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
static int swapcodechar( char ** parmi, char ** parmj, int n,int incr) 
	{ 		
	int i = n;
	register char **pi = (char **) (parmi); 		
	register char **pj = (char **) (parmj); 
	register int inc1 = incr/sizeof(char *);
	do { 		
		register char *t = *pi;
		*pi = *pj;				
		*pj = t;				
		pi += inc1;
		pj += inc1;
		} while (--i > 0);				
	return(0);
	}
/*-----------------------------------------------------------------------------------*/ 
static int compareCchar(char *i,char *j)
{
  return(strcmp(*((char * *) i),*((char **) j)));
}
/*-----------------------------------------------------------------------------------*/ 
static int compareDchar(char *i,char *j)
{
  return(- strcmp(*((char * *) i),*((char **) j)));
}
/*-----------------------------------------------------------------------------------*/ 
/******************************************************
 * Column sort of a matrix 
 ******************************************************/
void ColSortchar(char * *a,int *ind,int flag,int n,int p,char dir)
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
		sizeof(char *),sizeof(int), 
		(dir == 'i' ) ? compareCchar : compareDchar,
		swapcodechar,swapcodeind);
    }
}
/*-----------------------------------------------------------------------------------*/ 
/******************************************************
 * Row sort of a matrix 
 ******************************************************/
void RowSortchar(char * *a,int *ind,int flag,int n,int p,char dir)
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
		n*sizeof(char *),n*sizeof(int), 
		(dir == 'i' ) ? compareCchar:compareDchar,
		swapcodechar,swapcodeind);
    }
}
/*-----------------------------------------------------------------------------------*/ 
/******************************************************
 * Global sort of a Matrix
 ******************************************************/
void GlobalSortchar(char * *a,int *ind,int flag,int n,int p,char dir)
{  
  int i;
  if ( flag == 1) 
    {
      for ( i = 0 ; i < n*p ; i++) 
	ind[i]= i+1;
    }
  sciqsort((char *) (a),(char *) (ind),flag, n*p, 
	    sizeof(char *),sizeof(int), 
	    (dir == 'i' ) ? compareCchar:compareDchar,
	    swapcodechar,swapcodeind);
}
/*-----------------------------------------------------------------------------------*/ 
/*******************************************************
 *  lexicographic order with Rows ind is of size n
 *  ind gives the permutation of the rows which is applied 
 *  to sort them 
 ******************************************************/
static int lexicolschar =1;
static int lexirowschar =1;
/*-----------------------------------------------------------------------------------*/ 
static void setLexiSizechar(int n,int p) 
{
  lexicolschar = p;
  lexirowschar = n;
}
/*-----------------------------------------------------------------------------------*/ 
static  int LexiRowcompareCchar(char * *i,char * *j)
{
  int jc;
  for ( jc = 0 ; jc < lexicolschar ; jc++) 
    {
      int k = strcmp(*i,*j);
      if ( k != 0) return(k);
      i += lexirowschar;
      j += lexirowschar;
    }
  return (0);
}
/*-----------------------------------------------------------------------------------*/ 
static  int LexiRowcompareDchar(char * *i,char * *j)
{
  int jc;
  for ( jc = 0 ; jc < lexicolschar ; jc++) 
    {
      int k = strcmp(*i,*j);
      if ( k != 0) return(-k);
      i += lexirowschar;
      j += lexirowschar;
    }
  return (0);
}
/*-----------------------------------------------------------------------------------*/ 
static int LexiRowswapcodechar(char *parmi,char * parmj,int n) 
{ 		
  int i = n,j;
  register char * *pi = (char * *) (parmi); 		
  register char * *pj = (char * *) (parmj); 
  /* if ( n!= 1) printf(" swapcode avec n != 1\n"); */
  do { 
    for ( j = 0 ; j < lexicolschar ; j++) 
      {
	register char * t = *(pi +lexirowschar*j);		
	*(pi + lexirowschar*j) = *(pj+lexirowschar*j);				
	*(pj + lexirowschar*j) = t;	
      }
    pi++;
    pj++;
  } while (--i > 0);				
  return(0);
}
/*-----------------------------------------------------------------------------------*/ 
void LexiRowchar(char * *a,int *ind,int flag,int n,int p,char dir)
{
  int i;
  setLexiSizechar(n,p);
  if ( flag == 1) 
    {
      for ( i = 0 ; i < n ; i++) 
	ind[i]= i+1;
    }
  sciqsort((char *) (a),(char *) (ind),flag, n, 
	    sizeof(char *),sizeof(int), 
	    (dir == 'i' ) ? LexiRowcompareCchar:LexiRowcompareDchar,
	    LexiRowswapcodechar,swapcodeind);
}
/*-----------------------------------------------------------------------------------*/ 
/******************************************************
*  lexicographic order with Cols ind is of size p
*  ind gives the permutation of the column which is applied 
*  to sort them 
******************************************************/
static  int LexiColcompareCchar(char * *i,char * *j)
	{
	int ic;
	for ( ic = 0 ; ic < lexirowschar ; ic++) 
		{
		int k = strcmp(*i,*j);
		if ( k != 0) return(k);
		i++;
		j++;
		}
	return (0);
	}
/*-----------------------------------------------------------------------------------*/ 
static  int LexiColcompareDchar(char * *i,char * *j)
	{
	int ic;
	for ( ic = 0 ; ic < lexirowschar ; ic++) 
		{
		int k = strcmp(*i,*j);
		if ( k != 0) return(-k);
		i++;
		j++;
		}
	return (0);
	}
/*-----------------------------------------------------------------------------------*/ 
static int LexiColswapcodechar(char *parmi,char* parmj,int n) 
	{ 		
	int i = n,ir;
	register char * *pi = (char * *) (parmi); 		
	register char * *pj = (char * *) (parmj); 
	/* if ( n!= 1) printf(" swapcode avec n != 1\n"); */
	do { 
		for ( ir = 0 ; ir < lexirowschar ; ir++) 
			{
			register char * t = *(pi +ir);		
			*(pi +ir) = *(pj+ir);				
			*(pj +ir) = t;	
			}
		pi += lexirowschar ;
		pj += lexirowschar ;
		} while (--i > 0);				
	return(0);
	}
/*-----------------------------------------------------------------------------------*/ 
void LexiColchar(char * *a,int *ind,int flag,int n,int p,char dir)
	{
	int i;
	setLexiSizechar(n,p);
	if ( flag == 1) 
		{
		for ( i = 0 ; i < p ; i++) 
			ind[i]= i+1;
		}
	sciqsort((char *) (a),(char *) (ind),flag, p, 
		n*sizeof(char *),sizeof(int), 
		(dir == 'i' ) ? LexiColcompareCchar:LexiColcompareDchar,
		LexiColswapcodechar,swapcodeind);
	}
/*-----------------------------------------------------------------------------------*/ 
