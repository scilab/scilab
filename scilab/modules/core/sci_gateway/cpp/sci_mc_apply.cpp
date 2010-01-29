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

extern "C" {
  int C2F(sci_mc_apply)(char *fname,unsigned long fname_len);
 }

namespace {
/* makes (with MALLOC) a '\0'-terminated string from an Rhs var n<B0>arg. */
 char* getStringArg(int arg)
{
  char* res= 0;
  if(GetType(arg) == sci_strings)
    {
      int nbChars, unused, tmp;
      /* TODO: STACK2 api used because no stack3 api available */
      GetRhsVar(arg, STRING_DATATYPE, &nbChars, &unused, &tmp);
      res= static_cast<char*>( MALLOC(nbChars+1));
      strncpy(res, cstk(tmp), nbChars);
      res[nbChars]= '\0';
    }
  return res;
}
static double getDoubleArg(int arg)
{
  int err;
  double res;
  int* addr;
  getVarAddressFromPosition(arg, &addr);
  if(getVarType(addr) == sci_matrix)
    {
      int rows, cols;
      double* val;
      err = getMatrixOfDouble(addr, &rows, &cols, &val);
      if(rows != 1 || cols != 1)
        {
          // err
        }
      res= *val;
    }
  return res;
}

}


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
 *
 */












int C2F(sci_mc_apply)(char *fname,unsigned long fname_len)
{

  // Rhs1 = MatrixOfDouble(n x k)
  // Rhs2 = MatrixOfString(1 x 1)
  // Rhs3 = MatrixOfDouble(1 x 1) = m
  // Lhs1 = MatrixOfDouble(n x m)
  // fun_name= MatrixOfString(Rhs2)



  double* args;
  double* results;
  int n, k, m; // n x k args and n x m results
  
  char* funName;
  void (*function)(double*, double*);
  int err;
  {
    int* argVar;
    int type;
    getVarAddressFromPosition(1, &argVar);
    type= getVarType(argVar);
 getMatrixOfDouble( argVar, &k, &n, &args);
  }
  //  fprintf(stderr,"n:%d k:%d\n",n,k);
  std::cerr<<"n:"<<n<<" k:"<<k; 
funName = getStringArg(2);
  //  fprintf(stderr,"function:%s \n",funName);
  if(funName == NULL)
    {
    }
  {
    int found;
    found=SearchInDynLinks(funName, (reinterpret_cast< void (**)()>(&function)));
    if(found == -1) 
      { 
	//fprintf(stderr," pas trouvé function:%s \n",funName);
	//SciError( a pa trouvé);
	return 1;
      }
  }
  m= (int) getDoubleArg(3);
  //  fprintf(stderr,"m:%d \n",m);
  std::cerr<<"m:"<<m<<std::endl;
  allocMatrixOfDouble(Rhs+1, m, n, &results);
   mc_apply_n_process(args, k*sizeof(double), n, results, m*sizeof(double),  reinterpret_cast<void(*)(void const*, void*)>(function),0);
   // mc_apply_n_threads(args, k*sizeof(double), n, results, m*sizeof(double), reinterpret_cast<void(*)(void const*, void*)>(function), 0);

  LhsVar(1)= Rhs+1;
  PutLhsVar();
  return 0;
}
