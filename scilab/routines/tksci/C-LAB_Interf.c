/* Library to add C-Functions to Scilab And/Or Matlab */
/* Bertrand Guiheneuf 1996 */

/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
/* Changes                                                     */
/* ----------------------------------------------------------- */
/* 6 March 1997 (Bertrand Guiheneuf):                          */
/* Added string management (no string matrix management!!!!)   */
/* ----------------------------------------------------------- */
/*                                                             */
/* ----------------------------------------------------------- */
/*                                                             */
/* 02/20/1998 (Bertrand Guiheneuf):                            */
/* Added list management                                       */
/* ----------------------------------------------------------- */
/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

/* 	$Id: C-LAB_Interf.c,v 1.5 2002/10/14 09:26:42 chanceli Exp $	 */

/*
  #ifndef lint
  static char vcid[] = "$Id: C-LAB_Interf.c,v 1.5 2002/10/14 09:26:42 chanceli Exp $";
  #endif 
*/

#include "C-LAB_Interf.h"
#include "../intersci/libinter.h"

extern int C2F(cvstr)  __PARAMS((integer *n, integer *line, char *str, integer *job, unsigned long str_len));

/****************************************************************/
/*       ERRORS MANAGEMENT                                      */

/********** InterfError **********/
/* errare humanum est */

void InterfError( mesg )
     char *mesg;
{
  /* Cout("ERROR IN INTERFACE ROUTINE");*/
  cerro(mesg);
  /*  cout( mesg );*/
}


/****************************************************************/
/*       MATRIX MANAGEMENT                                      */

/********** MatrixMemSize **********/
/* Returns the size of a matrix in bytes */

int MatrixMemSize( m )
     Matrix *m;
{
  Matrix *base;
  integer *prefix;
  int error=0;
  int mem_siz=0;
  int type;
  int w,h; 
  int n,i;
  Matrix *cur;

  prefix = (integer *)m; /* virtual variable to read the matrix properties */

  error=0; /* is there in error in the source matrix? for the moment, no */

  type = prefix[0]; /* matrix type */
  h = prefix[1];    /* matrix height */
  w = prefix[2];    /* matrix width */

  switch (type) 
    {
    case 1 :/* it is a numeric matrix */
      {
	if ( prefix[3]==0 ) /* and it's real */
	  mem_siz = 4 * sizeof(integer) +w*h * sizeof(Matrix);
	else
	  mem_siz = 4 * sizeof(integer) + 2*w*h * sizeof(Matrix);
      }
      break;
      
    case 10 :/* it is a string matrix */ 	
      {
	mem_siz = (4 + w*h + prefix[4 + w*h]);
	mem_siz = ((mem_siz % 2) + mem_siz) * sizeof(integer);
      }
      break;
    case 15 :/* it is a list */  

      {
	n=h;
	mem_siz= (3 + n +1)/2 * sizeof(Matrix); /* the prefix and the pointers */
	/* now we have to count the size of the matrix contained in */
	/* the list */
	  
	base = m + (3+n +1)/2 -1;
	for (i=0; i<n; i++)
	  {
	    cur = base + prefix[2+i]; 
	    mem_siz += MatrixMemSize( cur );
	  }
      }
      break;
    default :
      { error=1; }

    }



  if (error==1) 
    {
      InterfError("MatrixMemSize: matrix tupe is not valid\n" ) ;
      return(-1);
    }
  else
    {
	
      return(mem_siz);
    }


}


/********** MatrixCreate **********/
/* Create a Matrix with the good dimensions and the good type */
/* Be careful when used with a string type !!!!! */
/* type="real"    --> double (real part only )           */
/* type="complex" --> double (real and imaginery part )  */
/* type="sys-string" --> string */
/* NOT DESIGNED FOR SCILAB/MATLAB PROGRAMMER USE                      */
/* INTERNAL USE ONLY !!!!!!!!!!!!                                     */
/*      WARNING: The string is contained in a (1x1) string matrix     */
/*      In this case, on of the dimensions (h or w) must be 1 and the */
/*      other one represents the size of the string !!!!!!!!!!!!!!    */

Matrix *MatrixCreate( h,w,type )
     int h; /* number of lines */
     int w; /* number of columns */
     char *type; /* type of the matrix */
{
  int mat_size;
  Matrix *output = 0;
  integer *prefix;
  char Error[200];

  if (!strcmp(type,"real"))
    {
      mat_size = 4 * sizeof(integer) + w*h * sizeof(double);
      output = (Matrix *)malloc((size_t)mat_size); /* the final matrix */
      prefix = (integer *)output; /* to access the matrix properties */
      
      prefix[0] = 1;  /* it's a full numerical matrix */
      prefix[1] = h;  /* coding the dimensions ... */
      prefix[2] = w;  /* see Guide for Developpers in the SCILAB doc */
      prefix[3] = 0;  /* no imaginery part */
    }
  else
    {
      if (!strcmp("complex",type))
	{
	  mat_size = 4 * sizeof(integer) + 2*w*h * sizeof(double);
	  output = (Matrix *)malloc(mat_size); /* the final matrix */
	  prefix = (integer *)output; /* to access the matrix properties */
	  
	  prefix[0] = 1;  /* it's a full numeric matrix */
	  prefix[1] = h;  /* coding the dimensions ... */
	  prefix[2] = w;  /* see Guide for Developpers in the SCILAB doc */
	  prefix[3] = 1;  /* it's complex! */
	}
      else
	{
	  if (!strcmp("sys-string",type))
	    {
	      /* if ( ( (MIN(w,h)) == 1) ) */
	      {
		mat_size = (4 + 2  + w*h);
		mat_size = (mat_size + (mat_size %2))* sizeof(integer);
		output = (Matrix *)malloc((unsigned)mat_size); /* the final matrix */
		prefix = (integer *)output; /* to access the matrix properties */
	  
		prefix[0] = 10; /* it's a string matrix */
		prefix[1] = 1;  /* dimensions */
		prefix[2] = 1;  /* multiple strings matrices are not allowed for the moment */
		prefix[3] = 0;  /* see Guide for Developpers in the SCILAB doc */

		prefix[4] = 1;  /* pointer on the string */
		prefix[5] = 1+w*h; /* pointer on the first char after the string */

	      }
	      
	    }
	  else 
	    {
	      sprintf(Error,"%s is not an implemented type",type);
	      InterfError(Error);
	    }
	}
    }
  return(output);
}


/********** MatrixGetType **********/
/* returns the type of a matrix */
/* the possible return values are:        */
/* "real" for a real numeric matrix       */
/* "complex" for a complex numeric matrix */

void MatrixGetType( m,type )
     Matrix *m;  /* the matrix which we want to know the type */
     char *type; /* the return value */
{
  integer *prefix;    /* virtual variable to read the matrix properties */
  prefix = (integer *)m; /* we want to read the first four ints of m */
  switch (prefix[0]) /* the matrix is numeric */
    {
    case 1 :
      {
	if (prefix[3] == 0) /* is it complex ??? */
	  strcpy(type,"real"); 
	else
	  strcpy(type,"complex");
      }
      break;
    case 10 :
      {
	strcpy(type,"string");
      }
      break;
    case 15 :
      {
	strcpy(type,"list");
      }
      break;
    default :
      {
	strcpy(type,"unknown");
      }
    }
}


/********** MatrixIsNumeric **********/
/* tests if a matrix is numeric ( real or complex ) */

int MatrixIsNumeric( m )
     Matrix *m; /* the matrix we want to test the type */
{
  integer *prefix;    /* virtual variable to read the matrix properties */
  prefix = (integer *)m;
  return( ( prefix[0] == 1 ) ); /* easy, no? */
}


/********** MatrixIsReal  **********/
/* tests if a matrix is real */

int MatrixIsReal( m )
     Matrix *m; /* the matrix we want to test the type */

{
  integer *prefix;    /* virtual variable to read the matrix properties */
  prefix = (integer *)m;
  /* type verification will come later */
  return( ( prefix[3] == 0 ) && ( prefix[0] == 1 )  ); /* easy again */
}


/********** MatrixIsComplex  **********/
/* tests if a matrix is complex */

int MatrixIsComplex( m )
     Matrix *m; /* the matrix we want to test the type */
{
  integer *prefix;    /* virtual variable to read the matrix properties */
  prefix = (integer *)m;
  /* type verification will come later */
  return( ( prefix[3] == 1 ) && ( prefix[0] == 1 )  );
}


/********** MatrixIsScalar  **********/
/* tests if a matrix is a real scalar */

int MatrixIsScalar( m )
     Matrix *m; /* the matrix we want to test the type */
{
  integer *prefix;    /* virtual variable to read the matrix properties */
  prefix = (integer *)m;
  return( ((prefix[0]==1) && (prefix[2]==1) && (prefix[2]==1) && (prefix[3]==0)) );
}

/********** MatrixIsString  **********/
/* tests if a matrix is (1x1) string matrix */

int MatrixIsString( m )
     Matrix *m; /* the matrix we want to test the type */
{
  integer *prefix;    /* virtual variable to read the matrix properties */
  prefix = (integer *)m;
  return( (prefix[0]==10) && (prefix[1]==1) && (prefix[2]==1) );
}

/********** MatrixIsList  **********/
/* tests if a matrix is List  */

int MatrixIsList( m )
     Matrix *m; /* the matrix we want to test the type */
{
  integer *prefix;    /* virtual variable to read the matrix properties */
  prefix = (integer *)m;
  return( (prefix[0]==15));
}


/********** MatrixReadString  **********/
/* Reads the string contained in a string matrix */
/* Necessary because of scilab strange management of strings */
/* The returned pointer will need to be freed by the user !!!!!! */

char *MatrixReadString( m )
     Matrix *m; /* the matrix in which the string is stored */
{
  char *outstr;
  integer *prefix;    /* virtual variable to read the matrix properties */
  int strsiz;
  int op; /* to code the operarion when calling the fortran routine cvstr */

  prefix = (integer *)m;
  strsiz = prefix[5]-1;
  outstr = (char *)malloc((strsiz+1) * sizeof(char));
  op = 1; /* scilab code to ascii */
  F2C(cvstr)(&strsiz, prefix + 6, outstr, &op ,strsiz+1);
  outstr[strsiz]='\0';
  return(outstr);
}



/********** MatrixCreateString  **********/
/* Reads the string contained in a ascii  string and put it  */
/* into a (1x1) Scilab string Matrix                         */

Matrix *MatrixCreateString( s )
     char *s;   /* the string to write */
{
  Matrix *m; /* the matrix where we want to put the string */
  integer *prefix;    /* virtual variable to read the matrix properties */
  int strsiz;
  int op; /* to code the operartion when calling the fortran routine cvstr */
 
  strsiz = strlen(s);
  m = MatrixCreate(1, strsiz, "sys-string");
  
  if (strsiz>0) {
    prefix = (integer *)m;
    op = 0; /* ascii to scilab code */
    F2C(cvstr)(&strsiz, prefix + 6, s, &op, strsiz);
  }
  return(m); 
}


/***************************/
/*  Lists management       */
/***************************/
/* provided the way lists, and objects in general, are treated in */
/* scilab [The stack :-((  ], this list management set is a bit   */
/* tricky and non-efficient. It's not my fault. Sorry.            */

/*********** ListCreate **********/
/* Create a void list. */
/* The list must later filled with appendList. */
/* BE CAREFULL : AppendList does not really append */
/* the existing list. It creates a new list instead !!! */

Matrix *ListCreate()
{
  Matrix *m;
  integer *prefix;
  m = (Matrix *)malloc( 3 * sizeof(integer));
  prefix=(integer *)m;
  prefix[0]=15; /* list type */
  prefix[1]=0;
  prefix[2]=1;
  return(m);
}
  
  
/******** AppendList *********/
/* Append any scilab object to a list. */
/* It's not really an append operation because  */
/* a new list is created and returned by the routine. */
/* This new list IS the real appended list  */

Matrix *AppendList(list, object)
     Matrix *list;
     Matrix *object;
{
  int lsz, osz;
  integer *old_pfx, *new_pfx;
  Matrix *m;
  
  int n;
  Matrix *old_base, *new_base;
  int old_nbelem;

  lsz = MatrixMemSize( list );
  
  osz = MatrixMemSize( object );
  
  m = (Matrix *)malloc( lsz + osz + 2*sizeof(integer));
  /* the two object sizes and a new pointer in the created list */
  
  old_pfx = (integer *)list;
  new_pfx = (integer *)m;
  
  n=old_pfx[1];
  memcpy( m,list, (3+n) * sizeof(integer));
  new_pfx[1]++;
  
  old_base = list + (3+n+1)/2 -1;
  new_base = m + (3+n+2)/2 -1;
  
  old_nbelem = old_pfx[2+n]-old_pfx[2];
  memcpy(new_base+1,old_base+1, old_nbelem*sizeof(Matrix) );
  
  memcpy(new_base+new_pfx[2+n], object, osz);
  new_pfx[2+n+1]=new_pfx[2+n]+osz/sizeof(Matrix);
  free(list);
  return(m);
}


/****** ListGetSize ******/
/* returns the number of cells in a list */
int ListGetSize(l)
     Matrix *l;
{
  integer *prefix;
  if (!MatrixIsList(l)) InterfError("The input parameter is not a list");
  prefix = (integer *)l;
  return(prefix[1]);
}


Matrix *ListGetCell(n, l)
     int n;
     Matrix *l;
     /* return the Matrix numbered n in the lits*/
     /* n starts to 0 !!! */
{
  integer *prefix;
  int sz;
  Matrix *m;

  if (!MatrixIsList(l)) InterfError("The input parameter is not a list");
  prefix = (integer *)l;
  sz=prefix[1];
  if ((n<0) || (n>=sz)) InterfError("Cell index out of range");
  
  m=l + (3 +sz +1)/2 + prefix[2+n] -1;
  return(m);
}
  
/********** MatrixFree **********/
/* Free a Matrix. For later compatibility */

void MatrixFree( m )
     Matrix *m; /* matrix to be freed */
{
  free((void *) m);
}


/********** MatrixGetPr **********/
/* Returns a pointer on the real part of a numeric matrix */

double *MatrixGetPr( m )
     Matrix *m; /* matrix that we want to get the real part */
{
  if ( !MatrixIsNumeric(m) ) 
    {
      InterfError("MatrixGetPr : Matrix is not numeric\n");
      return(0); /* returns a NULL pointer */
    }
  else /* good boy */
    {
      return( &(m[2]) );
    }
}

/********** MatrixGetPi **********/
/* Returns a pointer on the imaginery part of a numeric complex  matrix */

double *MatrixGetPi( m )
     Matrix *m; /* matrix that we want to get the real part */
{
  integer *prefix;  /* virtual variable to read the matrix properties */
  prefix = (integer *)m;

  if ( !MatrixIsNumeric(m) ) 
    {
      InterfError("MatrixGetPi : Matrix is not numeric\n");
      return(0); /* returns a NULL pointer */
    }
  else 
    {
      if ( !MatrixIsComplex(m) ) /* come on boy, don't try it with me .. */
	{
	  InterfError("MatrixGetPi : Matrix is must be complex\n");
	  return(0); /* returns a NULL pointer */
	}
      else 
	{
	  return( &(m[2 + prefix[1]*prefix[2]]) ); /* refix[1/2] are the matrix dimensions */
	}
    }
}


/********** MatrixGetWidth **********/
/* returns the number of columns of the matrix */

int MatrixGetWidth( m )
     Matrix *m; /* The matrix ... */
{
  integer *prefix;  /* virtual variable to read the matrix properties */
  prefix = (integer *)m;
  return ( prefix[2] ); /* see Guide for Developpers in the SCILAB doc */
}

/********** MatrixGetHeight **********/
/* returns the number of lines of the matrix */

int MatrixGetHeight( m )
     Matrix *m; /* The matrix ... */
{
  integer *prefix;  /* virtual variable to read the matrix properties */
  prefix = (integer *)m;
  return ( prefix[1] ); /* see Guide for Developpers in the SCILAB doc */
}

/********** MatrixGetScalar **********/
/* return the real part of the (1,1) element of a matrix */
/* if offers a way to get the real parameters  without   */
/* handling all the matrix treatment */

double MatrixGetScalar( m )
     Matrix *m; /* the matrix containing the scalar */
{
  if ( !MatrixIsNumeric(m) ) /* It is not a numeric matrix !! */
    {
      InterfError("MatrixGetScalar : matrix must be numeric\n");
      return(0); /* returns a NULL pointer */
    }
  else /* OK */
    {
      return( m[2] );
    }
}

/********** MatrixTranspose **********/
/* Transpose a matrix but without conjugating the complexes */
/* This routine is SLOW but permits to interface C programs */
/* without much efforts. Remember the elements of a matrix  */
/* are written line per line in C whereas they are writen   */
/* column after column in Fortran.                          */
/* Unfortunately ( for us C-coder ), Scilab codes the       */
/* in the Fortran-like style                                */   
/* Why doesn't everybody code in JAVA ?????                 */
/* anyway IT IS STRONGLY RECOMMENDED TO HANDLE THE PROBLEM  */
/* IN YOUR C-ROUTINES                                       */

void MatrixTranspose( m )
     Matrix *m; /* the matrix to transpose */
{
  double *buffer; /* the buffer used to transpose the matrix */
  /* LATER, this routine will be optimized */
  /* TO DO : Use the temporary memory of scilab when avalaible    */
  /*         When the matrix is squared, do not use any buffer(?) */

  integer *prefix; /* to get the matrix properties */
  int mat_w, mat_h, mat_size;
  int i,j;

  prefix = (integer *)m;
  mat_h = prefix[1];
  mat_w = prefix[2];
  mat_size = mat_h*mat_w;

  if ( !prefix[0]==1 ) /* the matrix is not numeric */
    InterfError("MatrixTranspose : Unfortunately, the matrix must be numeric\n");
  else 
    {
      buffer = (double *)malloc( mat_size * sizeof(double) );
      if (buffer == 0) /* not enough memory, sorry */
	InterfError("MatrixTranspose : Not enough memory to allocate the buffer\nTry to quit applications\n");
      else
	{
	  m+=2; /* to be on the real part */
	  for (i=0; i<mat_h; i++) /* transpose in the buffer */
	    for (j=0; j<mat_w; j++)
	      buffer[i*mat_w+j] = m[j*mat_h+i];
	  
	  for (i=0; i<mat_h; i++) /* copy the buffer in the matrix */
	    for (j=0; j<mat_w; j++)
	      m[i*mat_w+j] = buffer[i*mat_w+j];
	  
	  prefix[1] = mat_w;
	  prefix[2] = mat_h;

	  if (prefix[3] == 1) /* is it complex ? */
	    {
	      m+=mat_size;
	      for (i=0; i<mat_h; i++) /* transpose in the buffer */
		for (j=0; j<mat_w; j++)
		  buffer[i*mat_w+j] = m[j*mat_h+i];
	      
	      for (i=0; i<mat_h; i++) /* copy the buffer in the matrix */
		for (j=0; j<mat_w; j++)
		  m[i*mat_w+j] = buffer[i*mat_w+j];
	      
	    }
	 
	  free(buffer);
	}
    }
}


/********** MatrixCopy **********/
/* Copies a matrix from one place to another */

void MatrixCopy( m_source, m_dest )
     Matrix *m_source;
     Matrix *m_dest;
{
  int mem_siz;
  mem_siz = MatrixMemSize(m_source);
  if (mem_siz == -1) 
    {
      InterfError("MatrixCopy : Cannot determinate the matrix memory size\n");
      return;
    }
  else
    {
      memcpy(m_dest, m_source, mem_siz);
    }
}


/****************************************************************/
/*       STACK MANAGEMENT                                       */

/********** StkAvail **********/
/* Returns the Available memory in the stack   */
/* the answer is in BYTE, not in "stck indexes" */

int StkAvail()
{
  return( ((C2F(vstk).Lstk)[(C2F(vstk).bot)-1]-(C2F(vstk).Lstk)[(C2F(vstk).top)]-1) * sizeof(double) );
}


/********** InterfInit **********/
/* Allocate the memory to the Param array */
/* Puts the input parameters in the Param array   */
/* also computes the number of input and output   */
/* parameters. They are returned in the Interf.NbParamIn */
/* and Interf.NbParamOut */
/* Also puts the index of the called function */
/* within the interface in Interf.FuncIndex */

void InterfInit()
{
  int i;
  Interf.NbParamIn = (C2F(com).rhs>0 ? C2F(com).rhs : 0) ;  
  Interf.NbParamOut = (C2F(com).lhs>0 ? C2F(com).lhs : 0);
  Interf.FuncIndex = C2F(com).fin;
  Interf.ReturnCounter = 0;
  
  Interf.ReturnIndex = C2F(vstk).top+1 - Interf.NbParamIn;
  /* The first free index minus the number of Input Parameters */

  if (Interf.NbParamIn == 0 ) Interf.Param=0;
  else
    {
      Interf.Param = (Matrix **)malloc(Interf.NbParamIn * sizeof(Matrix *));
      
      for (i=0; i<Interf.NbParamIn; i++) /* be carefull stack_.stk became stack_Stk in stack-c.h */
	(Interf.Param)[i] = (Matrix *)( &( (C2F(stack).Stk)[ ( (C2F(vstk).Lstk)[C2F(vstk).top-C2F(com).rhs+i] ) -1] ) ); 
      /* in fact, it's stk(top-1 -rhs +1) to correct the C/FORTRAN problem */
    }
}


/********** ReturnParam **********/
/* Push a parameter (matrix) on the return stack */

void ReturnParam(m)
     Matrix *m;
{
  int CurrIndex;
  if (Interf.ReturnCounter < Interf.NbParamOut)
    {
      CurrIndex =(C2F(vstk).Lstk)[Interf.ReturnIndex-1];
      /* Gets the index of the first free cell in the return stack */
      if ( MatrixMemSize(m)>StkAvail() )
	{
	  free(m);
	  InterfError("Sorry, No more Stack free");
	  return;
	}

      MatrixCopy( m , &((C2F(stack).Stk)[CurrIndex-1]) );
      /* Copies the matrix there */
      
      Interf.ReturnIndex++;
      
      (C2F(vstk).Lstk)[Interf.ReturnIndex-1] = CurrIndex + MatrixMemSize( m ) / sizeof(Matrix);
      /* We must divide by sizeof(Matrix) because we work in bytes */
      /* not in "stack elements" */
      /* it's far from neatness */
      /* Anyway, doesn't it work??? */
      Interf.ReturnCounter++;
      free(m);
      /* and then free the parameter */
    }
}

/********** InterfDone **********/
/* Frees the memory used by Interf.Param */

void InterfDone()
{
  C2F(vstk).top += Interf.NbParamOut - Interf.NbParamIn;
  if (Interf.NbParamIn > 0) free(Interf.Param);
}












