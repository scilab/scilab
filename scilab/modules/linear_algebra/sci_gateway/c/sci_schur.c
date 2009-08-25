
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - INRIA
 * Copyright (C) 2009-2009 - DIGITEO - Bernard HUGUENEY
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *

function r=Err(x)
        r=norm(x,1)
endfunction
rand('normal')

//define tools
function A=testmat1(a,n)
        //eigen values are given by a dilation of nth roots of 1
        A=diag(a*ones(1,n-1),1)+diag((1/a)*ones(1,n-1),-1)
        A(1,n)=1/a;A(n,1)=a
endfunction

//==========================================================================
//==============================    schur     ============================== 
//==========================================================================
clear sel
function t=sel(R),t=real(R)<0 ,endfunction
A=testmat1(3,5);Ac=testmat1(3+%i,5);


schur(A,sel) : ok
[a1,a2]=schur(A,sel) pas ok, 1= +haut au lieu de 
 */

#include <string.h>
#include "doublecomplex.h"
#include "api_double.h"
#include "api_string.h"
#include <stdio.h>
#include "stack-c.h"
#include "MALLOC.h"
#include "gw_linear_algebra.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "dynamic_link.h"

#include "sas_schur.h"

/*--------------------------------------------------------------------------*/
extern int C2F(complexify)(int *num);
/*--------------------------------------------------------------------------*/


/*
 * [[VS],T] = schur(A, 'type') : R or C matrix VS orthogonal or unitary square matrix 'd'|'z' N x N, T: reuse arg 1
 * [U,dim [,T] ]=schur(A,flag) 'c' or 'd'
 * [Z,dim] = schur(A,E,flag)
 * [As,Es [,Q,Z]]=schur(A,E) : R or C matrix same dimensions reuse arg1 and arg2, create 'd'|'z' NxN for Q, Z
 * 
 * [As,Es [,Q],Z,dim] = schur(A,E,flag) : 


 * [U,dim [,T] ]=schur(A,extern1)
 *
 * [As,Es [,Q],Z,dim]= schur(A,E,extern2)
 * [Z,dim]= schur(A,E,extern2)
 *
 *

twoMatrices, selectPtr.none


 */

/* wrapper around SearchInDynLink with saner API (returns the resulting ptr as the return value NULL if not found */
static void(* getDynamicFunctionByName(char const* name))(void)
{
  int err=0;
  void (*ret) (void);
  err=SearchInDynLinks(name, &ret);
  return (err >= 0) ? ret : NULL;
}
/* makes (with MALLOC) a '\0'-terminated string from an Rhs var n°arg. */
static char* getStringArg(int arg)
{
  char* res= NULL;
  if(GetType(arg) == sci_strings)
    {
      int nbChars, unused, tmp;
      /* TODO: STACK2 api used because no stack3 api available */
      GetRhsVar(arg, STRING_DATATYPE, &nbChars, &unused, &tmp);
      res= (char*) MALLOC(nbChars+1);
      strncpy(res, cstk(tmp), nbChars);
      res[nbChars]= '\0';
    }
  return res;
}
/* get the function id from the Rhs var n°arg */
static int getSciFunctionArg(int arg)
{
  int res=0;
  if(GetType(arg) == sci_c_function)
    {
      int unusedDim;
      /* TODO: STACK2 api used because no stack3 api available */
      GetRhsVar(arg, EXTERNAL_DATATYPE, &unusedDim, &unusedDim, &res);
    }
  return res;
}
/* given the data ptr from a scilab var, returns the ptr to the header of that var. */
static int* getHeaderPtrFromDataPtr(void* dataPtr)
{
  return (((int*)dataPtr)-4);
}

/* First and last index of scilab variables allocated to store arguments when a scilab function is used as a select callback.
   As it reflects a gloabal state, there is no point in predenting otherwise by not making it a global var. */
static int beginSelectArg, endSelectArg, lastIndex ;

/* index to Scilab function, need fo the accessible to scilab function wrapper respecting the Lapack callback API,
 so cannot be passes as an argument */
static int sciSelect= 0;

/* generic wrapper around scilab function calls. The returned values have boolean semantics. 
They are used by the checkXgXesSciSelectFunction() and  XgXesSciSelectFunction() (respectively).
*/

/* check that the function indexed by sciSelect, with the nbRhs variables on the stack starting @firstSelectArg,
   returns either a sci_matrix or a sci_boolean. */
static int isSciPredicateValid(int nbRhs);
/* check that the function indexed by sciSelect, with the nbRhs variables on the stack starting @firstSelectArg,
   returns either a sci_matrix or a sci_boolean. */
static int callSciPredicate(int nbRhs);



static int zggesSciSelectFunction (doublecomplex const* alpha, doublecomplex const* beta);
static zgges_select_t checkZggesSciSelectFunction(void);

static int zgeesSciSelectFunction (doublecomplex const* w);
static zgees_select_t checkZgeesSciSelectFunction(void);

static int dggesSciSelectFunction (double const* alphaReal, double const* alphaImg, double const* beta);
static dgges_select_t checkDggesSciSelectFunction(void);

static int dgeesSciSelectFunction (double const* wr, double const* wi);
static dgees_select_t checkDgeesSciSelectFunction(void);


static char const* thisFunctionName;

static schur_select_type createSelectWrapper(int isCplx, int twoMat)
{
  return isCplx
    ? (twoMat ? (schur_select_type)checkZggesSciSelectFunction() : (schur_select_type) checkZgeesSciSelectFunction())
    : (twoMat ? (schur_select_type)checkDggesSciSelectFunction() : (schur_select_type) checkDgeesSciSelectFunction());
}

static int isNameOneOf(char* const name, char* const n1, char* const n2, char* const n3)
{
  return !(strcmp(name, n1) && strcmp(name, n2) && strcmp(name, n3));
}

static schur_select_type selectFunctionByName(char* const name
				       , char* const contName, schur_select_type contFunction
				       , char* const discName, schur_select_type discFunction)
{
  return isNameOneOf(name, "c", "cont", contName) 
    ? contFunction : (isNameOneOf(name, "d", "disc", discName) ? discFunction : (schur_select_type) getDynamicFunctionByName(name));
}

static schur_select_type getFunctionByName(int isCplx, int twoMat, char* const name)
{
  return isCplx 
    ? (twoMat 
       ? selectFunctionByName(name, "zb02ow", (schur_select_type)&zb02ox,"zb02ox", (schur_select_type)&zb02ox)
       : selectFunctionByName(name, "zb02mv", (schur_select_type)&zb02mv,"zb02mw", (schur_select_type)&zb02mw))
    : (twoMat 
       ? selectFunctionByName(name, "sb02ow", (schur_select_type)&sb02ow,"sb02ox", (schur_select_type)&sb02ox)
       : selectFunctionByName(name, "sb02mv", (schur_select_type)&sb02mv,"sb02mw", (schur_select_type)&sb02mw));
}

typedef struct data_matrix_t
{
  double* data; /* is in fact a doublecomplex* if real (and img) != NULL */
  double* real;
  double* img;
  int rows;
  int cols;
} data_matrix_type;

static int getArgMatrices(int isCplx, data_matrix_type* arg1, data_matrix_type* arg2) /* arg2 == NULL -> !twoMatrices */
{
  int ret= 0;
  int i;
  for(i=0; i != (arg2 ? 2 : 1); ++i)
      {
	int iPlus1= i+1;
	data_matrix_type* arg= i ? arg2 : arg1 ;
	if(isCplx)
	  {
	    C2F(complexify)(&iPlus1);
	    GetRhsVarMatrixComplex(iPlus1, &arg->rows, &arg->cols, &arg->real, &arg->img);
	    arg->data = arg->cols ? (double*)oGetDoubleComplexFromPointer( arg->real, arg->img, arg->rows * arg->cols ) : NULL;
	  }
	else
	  {
	    GetRhsVarMatrixDouble(iPlus1, &arg->rows, &arg->cols, &arg->data);
	  }
	if(arg->rows != arg->cols)
	  {
	    Scierror(20,_("%s: Wrong type for input argument #%d: Square matrix expected.\n"), thisFunctionName, iPlus1);
	    ret= -2;
	  }
      }
  if(arg2 && (arg2->rows != arg1->rows))
    {
      Scierror(999,_("%s: Wrong size for input arguments #%d and #%d: Same dimensions expected.\n"), thisFunctionName, 1,2);
      ret= -2;
    }
  return ret;
}

/* cannot return error code because CheckXhs returns :( */
static void checkRhsLhs(int twoMat)
{
  if(twoMat)
    {
      //     CheckLhs(2, 5);
    }
  else
    {
      //  CheckLhs(1, 3);
    }
}

static int createLhsOpts(int isCplx, int cols, data_matrix_type* lhsOpt1, int* vsIndexPtr, data_matrix_type* lhsOpt2, int* vsRIndexPtr)
{
  int ret=0;
  int i;
  int const nbVs=lhsOpt2 ?  2 : (lhsOpt1 ? 1 : 0);
  for(i=0; i!= nbVs; ++i)
    {
      int index= (i==0) ? (*vsIndexPtr= lastIndex++) : (*vsRIndexPtr= lastIndex++); /* assign index as needed for vs and vsR */
      data_matrix_type* lhsOptPtr= i ? lhsOpt2 : lhsOpt1;
      lhsOptPtr->cols= lhsOptPtr->rows= cols;
      {
	int const one = 1;
	int dataReal, dataImg;
	if(isCplx)
	  {
	    lhsOptPtr->data= cols ? (double*)MALLOC( cols * cols * sizeof(doublecomplex)) : NULL;
	    ret= C2F(createcvar)(&index, MATRIX_OF_DOUBLE_DATATYPE, &one, &cols, &cols, &dataReal, &dataImg, 1L);
	    lhsOptPtr->real= stk(dataReal);
	    lhsOptPtr->img= stk(dataImg);
	  }
	else
	  {
	    ret= C2F(createvar)(&index, MATRIX_OF_DOUBLE_DATATYPE, &cols, &cols, &dataReal, 1L);
	    lhsOptPtr->data= stk(dataReal);
	  }
      }
    }
  return ret;
}

static int nbOfVSToCompute(int twoMat, int externalFunction)
{
  /*
[U,T] = schur(A)
[U,dim [,T] ]=schur(A,flag)
[U,dim [,T] ]=schur(A,extern1)

[As,Es [,Q,Z]]=schur(A,E)
[As,Es [,Q],Z,dim] = schur(A,E,flag)
[Z,dim] = schur(A,E,flag)
[As,Es [,Q],Z,dim]= schur(A,E,extern2)
[Z,dim]= schur(A,E,extern2)

    externalFunction=0, twoMat=0 nbLhs=1 -> 0 2 -> 1

    externalFunction=1 twoMat=0, nbLhs=1 ->0 2-> 1 (on ne retourne pas arg1 mais vs en LhsVar(1)) 3 ->1

    externalfunction=0 twoMat=1 , nbLhs=2 ->0, 4 -> 2

    externalFunction=1 twoMat=1 nbLhs: 1 ->0 2-> 1 3->2 4->1 5 ->2

   */
return externalFunction 
    ?( twoMat ?  (Lhs > 1 ? (1+ (Lhs & 1L)) : 0) : (Lhs>1 ? 1 : 0))
    :( twoMat ? Lhs-2 : Lhs-1);

}

static int createDimVar(int * dimIndexPtr, double** ptrDimPtr)
{
  int ret= 0;
  *dimIndexPtr= lastIndex++;
  {
    int data;
    int const one=1;
    ret= C2F(createvar)(dimIndexPtr, MATRIX_OF_DOUBLE_DATATYPE, &one, &one, &data, 1L);
    *ptrDimPtr= stk(data);
  }
  **ptrDimPtr= 0.;
  return ret;
}


static void dataToRealImgIfNeeded(data_matrix_type* m)
{
  if(m->real)
    {
      vGetPointerFromDoubleComplex((doublecomplex*)m->data, m->cols * m->cols, m->real, m->img);
      FREE(m->data);
    }
  return;
}


static void errorMessages(int errorCode, int twoMat, int cols)
{
  if(errorCode == -1)
    {
      Scierror(999,_("%s: Cannot allocate more memory.\n"),thisFunctionName);
    }
  else if( errorCode > 0)
    {
      if(twoMat)
	{
	  if(errorCode <= cols)
	    {
	      sciprint(_("Warning :\n"));
	      sciprint(_("Non convergence in the QZ algorithm.\n"));
	      sciprint(_("The top %d  x %d blocks may not be in generalized Schur form.\n"), errorCode, errorCode);
	    }
	  else
	    {
	      Scierror(999,_("%s: Convergence error.\n"), thisFunctionName);
	    }
	}
      else
	{
	  sciprint(_("Warning :\n"));
	  sciprint(_("%s: Non convergence in QR steps.\n"), thisFunctionName);
	  sciprint(_("The top %d x %d block may not be in Schur form.\n"), errorCode, errorCode);
	}
    }
  return;
}




static void setLhsVars(int twoMat, int externalFunction, int vs, int vsR, int dim)
{
  fprintf(stderr, "Top=%d Rhs=%d Nbvars=%d vs=%d dim=%d\n",Top, Rhs, Nbvars, vs,dim);
#define HANDLE_LHS
#ifdef HANDLE_LHS
    if(!externalFunction)
      {
	if( (Lhs==2) && !twoMat)/* special case */
	  {
	    LhsVar(2)= 1;
	    LhsVar(1)= Rhs+1;
	  }
	else
	  {
	    switch(Lhs)
	      {
	      case 4: LhsVar(4)= Rhs+2; /* no break */
	      case 3: LhsVar(3)= Rhs+1; /* no break */
	      case 2: LhsVar(2)= twoMat ? 2 : Rhs+1 ;/* no break */
	      case 1: LhsVar(1)= 1;
	      }
	  }
      }
    else
      {
	if(twoMat)
	  {
	    if(Lhs >= 4)
	      {
		LhsVar(1)= 1;
		LhsVar(2)= 2;
	      }
	    if((Lhs== 3) || (Lhs==5))
	      {
		LhsVar(Lhs-2)= vs;
	      }

	    if(Lhs == 2)
	      {
		fprintf(stderr,"twoMaty && lhs==2\n vsR=%d dim=%d\n",vsR, dim);
		LhsVar(1)= vs;
		LhsVar(2)= dim;
	      }
	    else
	      {
	    switch(Lhs)
	      {
	      default:
	      case 2: LhsVar(Lhs-1)= vsR; /* nobreak */
	      case 1: LhsVar(Lhs)= dim; /* no break */
	      }
	      }
	  }
	else
	  {
	    if( Lhs > 1) /* == 2 || == 3 */
	      {
		Nbvars= 6;
		LhsVar(1)= vs;
		LhsVar(2)= dim;
	      }
	    if( Lhs & 1)/* == 1 || == 3 */
	      {
		LhsVar(Lhs)= 1;
	      }
	  }
      }
#endif
    return;
}

int C2F(intschur)(char *fname,unsigned long fname_len)
{
  int ret= 0;
  thisFunctionName= fname;
  if( (Rhs >= 1) && (GetType(1) != sci_matrix))
    {
      OverLoad(1);
      return 0;
    }
  if( (Rhs >= 3) && (GetType(2) != sci_matrix))
    {
      OverLoad(2);
      return 0;
    }
  CheckRhs(1, 3);
  CheckLhs(1, 5); /* doc says min Lhs == 2, but unit tests disagree :( */
  {
    schur_select_type selectPtr;
    int twoMatrices= (Rhs >= 2) && (GetType(2)==sci_matrix) ;
    int optionPlace= (Rhs==3) ? 3 : ( ( (Rhs==2) && !twoMatrices) ? 2 : 0 /* no option arg */) ;

    /* must be set here, before the switch/case because complexArgs is needed for function selection */
    char* const strArg= optionPlace ? getStringArg(optionPlace) : NULL;
    int isRealStr= strArg && ((strcmp(strArg, "r")==0) || (strcmp(strArg, "real")==0));
    int isCplxStr= strArg && ((strcmp(strArg, "comp")==0) || (strcmp(strArg, "complex")==0));
    int complexArgs= iIsComplex(1) || ( twoMatrices && iIsComplex(2)) || isCplxStr ;

    /* Lhs variables handling is so messy^Wtricky that we use variable to store variables index in stack.
       The allocation layout is :
       [1,Rhs] : stack allocated variables to retrieve arguments
       [beginSelectArg, endSelectArg[ : stack allocated variables to store arguments for a scilab select callback from Lapack
       vsId, vsRId, dimId as needed
    */
    int vsIndex=0, vsRIndex=0, dimIndex=0;
    selectPtr.none= NULL;

    beginSelectArg= endSelectArg= Rhs + 1;

    if( optionPlace )
      {
	switch( GetType(optionPlace) )
	  {
	  case sci_c_function :
	    {
	      sciSelect= getSciFunctionArg(optionPlace);
	      selectPtr = createSelectWrapper( complexArgs, twoMatrices);
	      ret= (selectPtr.none == NULL);
	      if( ret )
		{
		  Scierror(268,_("Invalid return value for function passed in arg %d.\n"), optionPlace);
		}
	      break;
	    }
	  case sci_strings :
	    { /* strArg and isRealStr, isCplxStr are already set */
	      if(!(isRealStr || isCplxStr))
		{
		  selectPtr= getFunctionByName(complexArgs, twoMatrices, strArg);
		  ret= (selectPtr.none == NULL);
		  if( ret )
		    {
		       Scierror(999,_("subroutine not found: %s\n"),strArg);
		    }
		}
	      break;
	    }
	  default :
	    {
	      Scierror(999,_( "%s: Wrong type for input argument #%d: A function expected.\n"), thisFunctionName, optionPlace);
	      ret= 1;
	      break;
	    } 
	  }
	if(strArg) /* FREE does not like NULL ptr :( */
	  {
	    FREE(strArg);
	  }
      }

    if(!ret)
    {
      data_matrix_type args[2]= {{NULL, NULL, NULL}, {NULL, NULL, NULL}};

      ret= getArgMatrices(complexArgs, &args[0], twoMatrices ? &args[1] : NULL);
      if(!ret)
	{
	  checkRhsLhs(twoMatrices);
	  /*
	    check for invalid args.
	  */
	  if(isRealStr && complexArgs)
	    {
	      Scierror(202,_("%s: Wrong type for input argument #%d: Real matrix expected.\n"),thisFunctionName, 1);
	      ret= 1;
	    }
	  else
	    {
	      double* ptrDim=NULL;
	      data_matrix_type lhsOpt[2]= {{NULL, NULL, NULL}, {NULL, NULL, NULL}};
	      int n= nbOfVSToCompute(twoMatrices, selectPtr.none != NULL);
	      fprintf(stderr,"n=%d\n",n);

	      lhsOpt[0].data= lhsOpt[1].data= NULL;


	      lastIndex= endSelectArg;
	      ret= createLhsOpts(complexArgs, args[0].cols
				 , (n>0) ? &lhsOpt[0] : NULL, &vsIndex
				 , (n>1) ? &lhsOpt[1] : NULL, &vsRIndex);
	      
	      fprintf(stderr, "vsIndex:%d, vsRIndex:%d\n",(int)vsIndex, (int) vsRIndex);
	      ret= (selectPtr.none && !( (Lhs == 1) && !twoMatrices)) ? createDimVar(&dimIndex, &ptrDim) : 0;
	      {
		int placeholderInt=0;
		fprintf(stderr,"nbvars=%d, lastIndex=%d, rows=%d cols=%d dim=%d\n",intersci_.nbvars, lastIndex, args[0].rows, args[0].cols, ptrDim ? *ptrDim : -1);
		ret= args[0].cols 
		  ? iSchurM(args[0].data, args[1].data, args[0].cols, complexArgs
			    , lhsOpt[0].data, lhsOpt[1].data, selectPtr, ptrDim? &placeholderInt : NULL) 
		  : 0;
		/*
		  if(ptrDebug)
		  {
		  int* tmpPtr=getHeaderPtrFromDataPtr(ptrDebug);
		  tmpPtr[0]= sci_matrix; tmpPtr[1]= tmpPtr[2]= 1; tmpPtr[3]=0;
		  }
		*/
		if (ptrDim)
		  {
		    *ptrDim=(double)placeholderInt;
		  }
	      }
	      errorMessages(ret, twoMatrices, args[0].cols);
	      {
		int i;
		for(i=0; i!=2; ++i)
		  {
		    dataToRealImgIfNeeded(&args[i]);
		    dataToRealImgIfNeeded(&lhsOpt[i]);
		  }
	      }
	    }
	}
    }
    setLhsVars(twoMatrices,(selectPtr.none != NULL), vsIndex, vsRIndex, dimIndex);
  }
  return ret;
}

static int callSciPredicate(int const nbRhs)
{
  int res= 0;
  int const nbLhs= 1;
  int i;
  /*  fprintf(stderr, "in callSciPredicate with arg of type:%d\n",*getHeaderPtrFromDataPtr(pStaticDouble[0])); */
  C2F(scifunction)(&beginSelectArg, &sciSelect, &nbLhs, &nbRhs);
    {
      int iRows, iCols;
      switch(GetType(beginSelectArg)){
      case sci_matrix:
	{
	  double* pRes;
	  GetRhsVarMatrixDouble(beginSelectArg, &iRows, &iCols, &pRes);
	  res= *pRes != 0.; /* original Fortran code does the strict fp cmp */
	  break;
	}
      case sci_boolean:
	{
	    int boolRes; /* /!\ TODO get some stack3 real GetRhsVarMatrixBoolean */
	    GetRhsVar(beginSelectArg, MATRIX_OF_BOOLEAN_DATATYPE, &iRows, &iCols, &boolRes);
	    res= *istk(boolRes);
	}
      }
    }
  return res;
}
static int isSciPredicateValid(int const nbRhs)
{
  int res= 0;
  int const nbLhs=1;
  //  fprintf(stderr,"Rhs before:%d\n",Rhs);
  C2F(scifunction)(&beginSelectArg, &sciSelect, &nbLhs, &nbRhs);
  //  fprintf(stderr,"Rhs after:%d\n",Rhs);
  {
      int iRows, iCols;
      switch(GetType(beginSelectArg))
	{
	case sci_matrix:
	  {
	    double* pRes;
	    GetRhsVarMatrixDouble(beginSelectArg, &iRows, &iCols, &pRes);
	    break;
	  }
	case sci_boolean:
	  {
	    int boolRes; /* /!\ TODO get some stack3 real GetRhsVarMatrixBoolean */
	    GetRhsVar(beginSelectArg, MATRIX_OF_BOOLEAN_DATATYPE, &iRows, &iCols, &boolRes);
	  }
	  if(iRows * iCols == 1)
	    {
	      res= 1;
	    }
	}
  }
  return res;
}


static int zggesSciSelectFunction (doublecomplex const* alpha, doublecomplex const* beta)
{
  int i;
  int const one= 1;
  for (i=beginSelectArg; i!=endSelectArg; ++i)
    {
      int dataReal, dataImg;
      doublecomplex const* arg= i? beta : alpha;
      C2F(createcvar)( &i, MATRIX_OF_DOUBLE_DATATYPE, &one, &one, &one, &dataReal, &dataImg, 1L);
      *stk(dataReal)= arg->r;
      *stk(dataImg)= arg->i;
    }
  return callSciPredicate(2);
}

static zgges_select_t checkZggesSciSelectFunction(void)
{  
  int i;
  for(i=0; i!=2; ++i)
    {
      int const one = 1;
      int dataReal, dataImg;
      C2F(createcvar)( &endSelectArg, MATRIX_OF_DOUBLE_DATATYPE, &one, &one, &one, &dataReal, &dataImg, 1L);
      ++endSelectArg;
      *stk(dataReal)= *stk(dataImg)= 1.0;
    }
  return isSciPredicateValid(2) ? &zggesSciSelectFunction : NULL ;
}

static int zgeesSciSelectFunction (doublecomplex const* w)
{
  int const one = 1;
  int dataReal, dataImg;
  C2F(createcvar)(&beginSelectArg, MATRIX_OF_DOUBLE_DATATYPE, &one, &one, &one, &dataReal, &dataImg, 1L);
  ++endSelectArg;
  *stk(dataReal)= w->r;
  *stk(dataImg)= w->i;

  return callSciPredicate(1);
}

static zgees_select_t checkZgeesSciSelectFunction(void)
{
  int const one = 1;
  int dataReal, dataImg;

  C2F(createcvar)(&endSelectArg, MATRIX_OF_DOUBLE_DATATYPE, &one, &one, &one, &dataReal, &dataImg, 1L);
  ++endSelectArg;
  *stk(dataReal)= *stk(dataImg)= 1.0;

  return isSciPredicateValid(1)? &zgeesSciSelectFunction : NULL;
}

/* /!\ Lapack uses 2 double args for alphaReal and alphaImg, but Scilab uses a complex alpha */
static int dggesSciSelectFunction (double const* alphaReal, double const* alphaImg, double const* beta)
{
  int const one = 1;
  int dataReal, dataImg;

  C2F(createcvar)(&beginSelectArg, MATRIX_OF_DOUBLE_DATATYPE, &one, &one, &one, &dataReal, &dataImg, 1L);
  *stk(dataReal)= *alphaReal;
  *stk(dataImg)= *alphaImg;

  {
    int const secondArg= beginSelectArg+1;
    C2F(createvar)(&secondArg, MATRIX_OF_DOUBLE_DATATYPE, &one, &one, &dataReal, 1L);
    *stk(dataReal)= *beta;
  }

  return callSciPredicate(2);
}

static dgges_select_t checkDggesSciSelectFunction(void)
{
  int const one = 1;
  int dataReal, dataImg;

  C2F(createcvar)(&endSelectArg, MATRIX_OF_DOUBLE_DATATYPE, &one, &one, &one, &dataReal, &dataImg, 1L);
  ++endSelectArg;
  *stk(dataReal)= *stk(dataImg)= 1.0;

  C2F(createvar)(&endSelectArg, MATRIX_OF_DOUBLE_DATATYPE, &one, &one, &dataReal, 1L);
  ++endSelectArg;
  *stk(dataReal)= 1.0;

  return isSciPredicateValid(2)? &dggesSciSelectFunction : NULL;
}

static int dgeesSciSelectFunction (double const* wr, double const* wi)
{
  int const one= 1;
  int dataReal, dataImg;
  C2F(createcvar)(&beginSelectArg, MATRIX_OF_DOUBLE_DATATYPE, &one, &one, &one, &dataReal, &dataImg, 1L);
  *stk(dataReal)= *wr;
  *stk(dataImg)= *wi;

  return callSciPredicate(1);
}

static dgees_select_t checkDgeesSciSelectFunction(void)
{
  int const one = 1;
  int dataReal, dataImg;

  C2F(createcvar)(&endSelectArg, MATRIX_OF_DOUBLE_DATATYPE, &one, &one, &one, &dataReal, &dataImg, 1L);
  ++endSelectArg;
  *stk(dataReal)= *stk(dataImg)= 1.0;

  return isSciPredicateValid(1)? &dgeesSciSelectFunction : NULL;
}
