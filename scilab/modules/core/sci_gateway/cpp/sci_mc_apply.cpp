/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bernard HUGUENEY
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
extern "C" {
#include <stdio.h>

#include "stack-c.h"
#include "gw_core.h"

#include "dynamic_link.h"
#include "api_common.h"
#include "api_double.h"

#include "MALLOC.h"

#include "stack1.h"
#include "api_double.h"
#include "api_int.h"
#include "api_list.h"
#include "api_string.h"
#include "api_common.h"
#include "stack3.h"
#include "stack2.h"
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"
#include "sci_mem_alloc.h"

#include "stack-def.h" //#define nlgh nsiz*4  
#include "stack-c.h"  // #define Nbvars C2F(intersci).nbvars, Top & cie

}

#include <cstdlib>
#include <cstring>
#include "mc_apply.hxx"

#include <iostream>


  /*
   *
   fun_name='test_fun';
   c_prog=['#include  <math.h>'
   'void '+fun_name+'(double* arg, double* res) {'
   '*res= 2.*(*arg);'
   '}'];
   disp(c_prog);
   mputl(c_prog,fun_name+'.c');
   ilib_for_link(fun_name,fun_name+'.c',[],"c");
   exec loader.sce;
   mc_apply([1,2],fun_name,1)

   function res= f(arg); res=2*arg; endfunction;

   mc_apply([1, 2], "f", 1);


bug de scifunction : les macros doivent être appelées par leur nom :(
pour la vérification qu'un nom est bien celui d'une macro :
getmacroslist
	et dans core/src/c/getvariablesname.c puis vérifier que le type est bien sci_XXX :( :( :(


	[R1,...Rm]= mc_apply(Arg1,...,Argk, function, [type1,...,typem[,ncols1,...,ncolsm,[
pos= find_arg_if(Test[, start_pos])

penser à allouer plus de Rhs (y compris dummy) que de Lhs.

   *
   */









extern "C" {
  int  C2F(sci_mc_apply)(char *fname,unsigned long fname_len);
}


namespace {
 char* getStringArg(int* arg_addr)
  {
    char* res= 0;
    int type;
    SciErr err= getVarType(pvApiCtx, arg_addr, &type);
    if(type == sci_strings)
      {
	int rows, cols;
	err= getMatrixOfString(pvApiCtx, arg_addr, &rows, &cols, NULL, NULL);
	if( (!err.iErr) && (rows == 1) && (cols == 1)){
	  int nChars;
	  err= getMatrixOfString(pvApiCtx, arg_addr, &rows, &cols, &nChars, NULL);
	  if(!err.iErr) {
	    res= ((char*)( MALLOC(nChars+1)));
	    if( res ) {
	      char* data;
	      err = getMatrixOfString(pvApiCtx, arg_addr, &rows, &cols, &nChars, &res);
	      if( err.iErr ) {
		FREE(res);
		res= 0;
	      }
	    }
	  }
	}
      }
    return res;
  }


static int currentTop;
static int n, k, m;

static char* scilab_function_name=0;
static unsigned long scilab_function_name_length=0;

static int scilab_function=0;

template<bool by_name>
static void wrapper(double const* args, double * res) {
  
  SciErr err;
  {
    double* data;
    err= allocMatrixOfDouble(pvApiCtx, ++currentTop, 1, k, &data);
    memcpy(data, args, k*sizeof(double));
  }
  int sci_arg_pos = currentTop;
  int  sci_rhs = 1;
  int  sci_lhs = 1;
  //  fprintf(stderr, " Nbvars = %d\n", 	Nbvars);
  int saveNbvars= Nbvars;
  Nbvars = 6; // sinon MALLOC incorrect Size Error File src/c/stack2.c Line 3311
    // r = scilabfoo(x)	
    // C2F(scifunction) call a scilab function
  if(by_name){
    C2F(scistring)(&sci_arg_pos, scilab_function_name, &sci_lhs, &sci_rhs, scilab_function_name_length);
  } else {
    C2F(scifunction)(&sci_arg_pos, &scilab_function, &sci_lhs, &sci_rhs);
  }
  //    fprintf(stderr, " Nbvars = %d\n", 	Nbvars);
    // result r is now on first position on stack
    {
      double* tmp;
      int* addr;
      int rm1, rn1;
      Nbvars = 6;
      currentTop= 5; // à modifier ?
      err= getVarAddressFromPosition(pvApiCtx, currentTop, &addr);
      err = getMatrixOfDouble(pvApiCtx, addr, &rm1, &rn1, &tmp);// assert rm1 and rn1
      //    fprintf(stderr," rm1= %d, rn1=%d  r=%g\n" , rm1, rn1,  *tmp);
      memcpy(res, tmp, m*sizeof(double));
      
      //    fprintf(stderr," currentTop= %d, *tmp=%g  r=%g\n" , currentTop,*tmp,  *res);
      Nbvars= saveNbvars;
      --currentTop;
    }
}
}


int  C2F(sci_mc_apply)(char *fname,unsigned long fname_len) 
{
 
  double* args_data;
  double* res_data;
  double* tmp;
  
  CheckRhs(3,3);// args, fun, res_size


  int type;
  SciErr err;
  int* addr;

  typedef void (*function_to_call_t)(void const*, void*);
  typedef void (*wrapper_function_t)(double const*, double*);
  typedef void (*loaded_function_t)();
  union{
    function_to_call_t to_call;
    loaded_function_t to_load;
    wrapper_function_t wrapper;
  } function;
  err= getVarAddressFromPosition(pvApiCtx, 1, &addr);
  err= getVarType(pvApiCtx, addr, &type);
  if (type != sci_matrix)
    {
      Scierror(999,"%s: Wrong type for input argument #%d: A real expected.\n",fname,1);
      return 0;
    }
    
  err = getMatrixOfDouble(pvApiCtx, addr, &k, &n, &args_data);


    currentTop = Rhs;

  // we want to have to result var at hand before calling the scilab macro so we must create it now before the args

    err= allocMatrixOfDouble(pvApiCtx, ++currentTop, m, n,  &res_data);
    int res_pos= currentTop;


   err= getVarAddressFromPosition(pvApiCtx, 2, &addr);
  err= getVarType(pvApiCtx, addr, &type);
  switch(type){
    case sci_strings: { // native function name
      char* funName = getStringArg(addr);
      fprintf(stderr,"name: %s\n",funName);
    int found;
    found=SearchInDynLinks(funName, &function.to_load);
    if(found == -1) 
      { 
	fprintf(stderr," not dynamicLink function : %s should be a macro name \n",funName);
	scilab_function_name= funName;
	scilab_function_name_length= std::strlen(scilab_function_name);
    function.wrapper = wrapper<true>;
	//SciError( a pa trouvé);

      }
    scilab_function= 0;
    break;
  }
  case sci_c_function: { // scilab macro
  // get pointer on external function
    int m_fun, n_fun;
    GetRhsVar(2, EXTERNAL_DATATYPE, &m_fun, &n_fun, &scilab_function);
    function.wrapper = wrapper<false>;
    break;
  }
  default: {
    Scierror(999,"%s: Wrong type for input argument #%d: A scilab function expected.\n",fname,3);
    return 0;
  }
  }
    
  err= getVarAddressFromPosition(pvApiCtx, 3, &addr);
  err= getVarType(pvApiCtx, addr, &type);
  if (type != sci_matrix)
    {
      Scierror(999,"%s: Wrong type for input argument #%d: A real expected.\n",fname,2);
      return 0;
    }
  {
    double*tmp;
    int m3, n3;
    err = getMatrixOfDouble(pvApiCtx, addr, &m3, &n3, &tmp);
	  
    if ( (m3 == n3) && (n3 == 1) )
      {
	m = (int) *tmp;
      }
    else
      {
	Scierror(999,"%s: Wrong size for input argument #%d: A scalar expected.\n",fname,2);
	return 0;
      }
  }	
  if(scilab_function || scilab_function_name) { // we have a macro
    mc_apply_n_process(args_data, k*sizeof(double), n, res_data,  m*sizeof(double), function.to_call);
  }else {
    mc_apply_n_threads(args_data, k*sizeof(double), n, res_data,  m*sizeof(double), function.to_call);
  }
  LhsVar(1) = res_pos;
  PutLhsVar();
  return 0;
}

