/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2000 Enpc/Jean-Philippe Chancelier
 *    jpc@cereve.enpc.fr 
 --------------------------------------------------------------------------*/

static void CNAME(ColSort,char)();
static void CNAME(RowSort,char)();
static void CNAME(GlobalSort,char)();
static void CNAME(LexiRow,char)();
static void CNAME(LexiCol,char)();

/******************************************************
 * code for Sorting Matrices a[i+n*j] of strings 
 ******************************************************/

static int CNAME(swapcode,char)(parmi, parmj, n) 
     char **parmi,**parmj;
     int n;
{ 		
  int i = n;
  register char **pi = (char **) (parmi); 		
  register char **pj = (char **) (parmj); 
  do { 		
    register char *t = *pi;
    *pi++ = *pj;				
    *pj++ = t;				
  } while (--i > 0);				
  return(0);
}

static int CNAME(compareC,char)(i,j)
     char **i,**j;
{
  return(strcmp(*i,*j));
}

static int CNAME(compareD,char)(i,j)
     char **i,**j;
{
  return(- strcmp(*i,*j));
}

/******************************************************
 * Column sort of a matrix 
 ******************************************************/

static void CNAME(ColSort,char)(a,ind,flag,n,p,dir)
     char **a;
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
	       sizeof(char *),sizeof(int), 
	       (dir == 'i' ) ? CNAME(compareC,char) : CNAME(compareD,char),
	       CNAME(swapcode,char),swapcodeind);
    }
}

/******************************************************
 * Row sort of a matrix 
 ******************************************************/

static void CNAME(RowSort,char)(a,ind,flag,n,p,dir)
     char **a;
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
	       n*sizeof(char *),n*sizeof(int), 
	       (dir == 'i' ) ? CNAME(compareC,char):CNAME(compareD,char),
	       CNAME(swapcode,char),swapcodeind);
    }
}


/******************************************************
 * Global sort of a Matrix
 ******************************************************/

static void CNAME(GlobalSort,char)(a,ind,flag,n,p,dir)
     char **a;
     int *ind;
     int n,p,flag;
     char dir;
{  
  int i;
  if ( flag == 1) 
    {
      for ( i = 0 ; i < n*p ; i++) 
	ind[i]= i+1;
    }
  sciqsort((char *) (a),(char *) (ind),flag, n*p, 
	   sizeof(char *),sizeof(int), 
	   (dir == 'i' ) ? CNAME(compareC,char):CNAME(compareD,char),
	   CNAME(swapcode,char),swapcodeind);
}

/*******************************************************
 *  lexicographic order with Rows ind is of size n
 *  ind gives the permutation of the rows which is applied 
 *  to sort them 
 ******************************************************/

static int CNAME(lexicols,char) =1;
static int CNAME(lexirows,char) =1;

static void CNAME(setLexiSize,char)(n,p) 
     int p,n;
{
  CNAME(lexicols,char) = p;
  CNAME(lexirows,char) = n;
}

static  int CNAME(LexiRowcompareC,char)(i,j)
     char **i, **j;
{
  int jc;
  for ( jc = 0 ; jc < CNAME(lexicols,char) ; jc++) 
    {
      int k = strcmp(*i,*j);
      if ( k != 0) return(k);
      i += CNAME(lexirows,char);
      j += CNAME(lexirows,char);
    }
  return (0);
}
static  int CNAME(LexiRowcompareD,char)(i,j)
     char **i, **j;
{
  int jc;
  for ( jc = 0 ; jc < CNAME(lexicols,char) ; jc++) 
    {
      int k = strcmp(*i,*j);
      if ( k != 0) return(-k);
      i += CNAME(lexirows,char);
      j += CNAME(lexirows,char);
    }
  return (0);
}

static int CNAME(LexiRowswapcode,char)(parmi, parmj, n) 
     char **parmi,**parmj;
     int n;
{ 		
  int i = n,j;
  register char **pi = (char **) (parmi); 		
  register char **pj = (char **) (parmj); 
  /** if ( n!= 1) printf(" swapcode avec n != 1\n"); **/
  do { 
    for ( j = 0 ; j < CNAME(lexicols,char) ; j++) 
      {
	register char *t = *(pi +CNAME(lexirows,char)*j);		
	*(pi + CNAME(lexirows,char)*j) = *(pj+CNAME(lexirows,char)*j);
	*(pj + CNAME(lexirows,char)*j) = t;	
      }
    pi++;
    pj++;
  } while (--i > 0);				
  return(0);
}


static void CNAME(LexiRow,char)(a,ind,flag,n,p,dir)
     int **a,*ind;
     int n,p;
     char dir;
{
  int i;
  CNAME(setLexiSize,char)(n,p);
  if ( flag == 1) 
    {
      for ( i = 0 ; i < n ; i++) 
	  ind[i]= i+1;
    }
  sciqsort((char *) (a),(char *) (ind),flag, n, 
	   sizeof(char*),sizeof(int), 
	   (dir == 'i' ) ? CNAME(LexiRowcompareC,char):CNAME(LexiRowcompareD,char),
	   CNAME(LexiRowswapcode,char),swapcodeind);
}

/******************************************************
 *  lexicographic order with Cols ind is of size p
 *  ind gives the permutation of the column which is applied 
 *  to sort them 
 ******************************************************/

static  int CNAME(LexiColcompareC,char)(i,j)
     char **i,**j;
{
  int ic;
  for ( ic = 0 ; ic < CNAME(lexirows,char) ; ic++) 
    {
      int k = strcmp(*i,*j);
      if ( k != 0) return(k);
      i++;
      j++;
    }
  return (0);
}
static  int CNAME(LexiColcompareD,char)(i,j)
     char **i,**j;
{
  int ic;
  for ( ic = 0 ; ic < CNAME(lexirows,char) ; ic++) 
    {
      int k = strcmp(*i,*j);
      if ( k != 0) return(-k);
      i++;
      j++;
    }
  return (0);
}


static int CNAME(LexiColswapcode,char)(parmi, parmj, n) 
     char **parmi,**parmj;
     int n;
{ 		
  int i = n,ir;
  register char **pi = (char **) (parmi); 		
  register char **pj = (char **) (parmj); 
  /* if ( n!= 1) printf(" swapcode avec n != 1\n"); */
  do { 
    for ( ir = 0 ; ir < CNAME(lexirows,char) ; ir++) 
      {
	register char *t = *(pi +ir);		
	*(pi +ir) = *(pj+ir);				
	*(pj +ir) = t;	
      }
    pi += CNAME(lexirows,char) ;
    pj += CNAME(lexirows,char) ;
  } while (--i > 0);				
  return(0);
}


static void CNAME(LexiCol,char)(a,ind,flag,n,p,dir)
     char **a;
     int *ind;
     int n,p;
     char dir;
{
  int i;
  CNAME(setLexiSize,char)(n,p);
  if ( flag == 1) 
    {
      for ( i = 0 ; i < p ; i++) 
	  ind[i]= i+1;
    }
  sciqsort((char *) (a),(char *) (ind),flag, p, 
	   n*sizeof(char*),sizeof(int), 
	   (dir == 'i' ) ? CNAME(LexiColcompareC,char):CNAME(LexiColcompareD,char),
	   CNAME(LexiColswapcode,char),
	   swapcodeind);
}



#ifdef TEST 

static void CNAME(inita,char)();
static void CNAME(afficher,char)();
static void CNAME(sorttest,char)();



typedef char string[10]; 
static char * CNAME(aa,char)[4*4] = {"a","d","c","z","e","i","z","b","c","a","t","z","o","w","i","s"};

static void CNAME(inita,char)(a,n,p)
     char **a;
     int n,p;
{
  int i;
  if ( n*p <= 4*4 )
    for (i=0; i < n*p; i++) 
      { 
	a[i] = (char *) malloc( (strlen(CNAME(aa,char)[i])+1)*sizeof(char));
	strcpy(a[i],CNAME(aa,char)[i]);
      }
  else 
    for (i=0; i < n*p; i++) 
      { 
	a[i] = (char *)  malloc( 2*sizeof(char));
	a[i][0]= 38+ n*p-i;a[i][1]= '\0';
      }
  CNAME(afficher,char)(a,"a",n,p,sizeof(char));
}


static void CNAME(afficher,char)(a,name,n,p)
     char *name;
     char **a;
     int n,p;
{
  int i,j;
  printf("%s=\n",name);
  for ( i = 0 ; i < n ; i++) 
    {
      for ( j= 0 ; j < p ; j++ ) 
	{
	  printf("%s ", a[i+n*j]);
	}
      printf("\n");
    }
}


#define Nstr 2
#define Pstr 3 

static void CNAME(sorttest,char)()
{
  char *a[Nstr*Pstr];
  char *b[Nstr*Pstr];
  int ind[Nstr*Pstr];
  int i,flag,j;
  int n=Nstr,p=Pstr;
  flag=1;

  /** Global sort example **/
  CNAME(inita,char)(a,n,p) ;
  CNAME(GlobalSort,char)(a,ind,flag,n,p,'c');
  CNAME(afficher,char)(a,"glob a",n,p);
  afficherint(ind,"glob ind",n,p);

  /** Column sort example **/
  CNAME(inita,char)(a,n,p) ;
  CNAME(ColSort,char)(a,ind,flag,n,p,'c');
  CNAME(afficher,char)(a,"col a",n,p);
  afficherint(ind,"col ind",n,p);

  /** Row sort example **/
  CNAME(inita,char)(a,n,p) ;
  CNAME(RowSort,char)(a,ind,flag,n,p,'c');
  CNAME(afficher,char)(a,"row a",n,p);
  afficherint(ind,"row ind",n,p);

  /** Lexicographic Col sort **/
  CNAME(inita,char)(a,n,p) ;
  CNAME(LexiCol,char)(a,ind,flag,n,p,'c');
  CNAME(afficher,char)(a,"lexico col a",n,p);
  afficherint(ind,"lexico col ind",1,p);

  /** Lexicographic Row sort **/
  CNAME(inita,char)(a,n,p) ;
  CNAME(LexiRow,char)(a,ind,flag,n,p,'c');
  CNAME(afficher,char)(a,"lexico Row a",n,p);
  afficherint(ind,"lexico Row ind",n,1);
}

#endif





