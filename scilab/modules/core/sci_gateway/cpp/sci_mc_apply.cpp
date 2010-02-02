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

}

#include <cstdlib>
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

   mc_apply([1, 2], f, 1);

   *
   */









extern "C" {
  int C2F(sci_mc_apply)(char *fname,unsigned long fname_len);
}

namespace {

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
  /* makes (with MALLOC) a '\0'-terminated string from an Rhs var n<B0>arg. */
  char* getStringArg(int* arg_addr)
  {
    char* res= 0;
    int type;
    SciErr err= getVarType(pvApiCtx, arg_addr, &type);
    if(type == sci_strings)
      {
	int rows, cols;
	err = getMatrixOfString(pvApiCtx, arg_addr, &rows, &cols, NULL, NULL);
	if( (!err.iErr) && (rows == 1) && (cols == 1)){
	  int nChars;
	  err = getMatrixOfString(pvApiCtx, arg_addr, &rows, &cols, &nChars, NULL);
	  if(!err.iErr) {
	    res= static_cast<char*>( MALLOC(nChars+1));
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
  static double getDoubleArg(int arg)
  {
    double res;
    int* addr;
    SciErr err=  getVarAddressFromPosition(pvApiCtx, arg, &addr);
    int type;
    err= getVarType(pvApiCtx, addr, &type);
    if(type == sci_matrix)
      {
	int rows, cols;
	double* val;
	err = getMatrixOfDouble(pvApiCtx, addr, &rows, &cols, &val);
	if(rows != 1 || cols != 1)
	  {
	    // err
	  }
	res= *val;
      }
    return res;
  }

  static int scilab_function=0;
  static double* scilab_arg_data=0;
  static int currentTop;
  static int n, k, m;

  int wrapper(double const * args, double* res) {
    memcpy(scilab_arg_data, args, sizeof(double)*k);
    int  nb_lhs= 1, nb_rhs= 1;
    std::cerr<<"in wrapper\n";
    C2F(scifunction)(&currentTop, &scilab_function, &nb_lhs, &nb_rhs);
    { // should be the same every time we are called.
      int rows, cols;
      int* scilab_res_var;
      double* scilab_res_data;
      getVarAddressFromPosition(pvApiCtx, 1, &scilab_res_var);
      getMatrixOfDouble(pvApiCtx, scilab_res_var, &rows, &cols, &scilab_res_data);
      memcpy(res, scilab_res_data, sizeof(double)*m);
    }
    return 0;
  }


}


int C2F(sci_mc_apply)(char *fname,unsigned long fname_len)
{

  // Rhs1 = MatrixOfDouble(n x k)
  // Rhs2 = MatrixOfString(1 x 1)
  // Rhs3 = MatrixOfDouble(1 x 1) = m
  // Lhs1 = MatrixOfDouble(n x m)
  // fun_name= MatrixOfString(Rhs2)



  double* args;
  double* results;
  
  char* funName;
  int (*function)(double const *, double*);

  
  int* argVar;
  int type;
  SciErr err;

  int debug = getSciFunctionArg(1);

  err= getVarAddressFromPosition(pvApiCtx, 1, &argVar);
  err= getVarType(pvApiCtx, argVar, &type);
  err= getMatrixOfDouble(pvApiCtx, argVar, &k, &n, &args);
  
  //  fprintf(stderr,"n:%d k:%d\n",n,k);
  std::cerr<<"n:"<<n<<" k:"<<k; 
  int* funVar;



  err= getVarAddressFromPosition(pvApiCtx, 2, &funVar);
  err= getVarType(pvApiCtx, funVar, &type);

  currentTop= Rhs;
  m= (int) getDoubleArg(3);
  //  fprintf(stderr,"m:%d \n",m);
  std::cerr<<"m:"<<m<<std::endl;
  allocMatrixOfDouble(pvApiCtx, ++currentTop, m, n, &results);

  switch(type){
  case sci_strings: { // native function name
    funName = getStringArg(funVar);
    int found;
    found=SearchInDynLinks(funName, (reinterpret_cast< void (**)()>(&function)));
    if(found == -1) 
      { 
	//fprintf(stderr," pas trouvé function:%s \n",funName);
	//SciError( a pa trouvé);
	return 1;
      }
    break;
  }
  case sci_c_function: { // scilab macro
    int lhs, rhs;
    GetRhsVar(2, EXTERNAL_DATATYPE, &lhs, &rhs, &scilab_function);
    function= &wrapper;
    err= allocMatrixOfDouble(pvApiCtx, ++currentTop, k, 1, &scilab_arg_data);
    break;
  }
  default:{/* SciError*/}
  }



  std::cerr<<"m:"<<m<<std::endl;
  allocMatrixOfDouble(pvApiCtx, Rhs+1, m, n, &results);
  mc_apply_n_process(args, k*sizeof(double), n, results, m*sizeof(double),  reinterpret_cast<void(*)(void const*, void*) >(function),0);
  // mc_apply_n_threads(args, k*sizeof(double), n, results, m*sizeof(double), reinterpret_cast<void(*)(void const*, void*)>(function), 0);

  LhsVar(1)= Rhs+1;
  PutLhsVar();
  return 0;
}

