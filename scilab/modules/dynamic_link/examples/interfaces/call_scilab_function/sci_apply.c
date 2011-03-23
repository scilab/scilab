//-------------------------------------------------------------------------------------
/*
 * Scilab ( http://www.scilab.org/ )
 * Copyright (C) DIGITEO - Bernard HUGUENEY - 2010
 * 
 */


#include "stack-c.h"
#include "Scierror.h"
#include "api_common.h"
#include "api_double.h"
#include "api_string.h"
#include "MALLOC.h"
#include <string.h> // memcpy

#include <stdio.h>

#ifndef  _MSC_VER
#include <unistd.h>
#endif

#include <stdlib.h>

#include <omp.h>


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

static size_t* share_work(size_t s, size_t n, size_t* o){
  while(n) {
    if(n==1) {
      *o= s;
    } else {
      size_t const r=s/n;
      *o= r;
      s-= r;
    }
    --n;
    ++o;
  }
  return o;
}


static void* mc_apply_n_process_worker(void const* const b, size_t in_size, size_t n, void* const out,  size_t out_size, void (*f)(void const*, void *), int const nb_process){

  size_t shares[nb_process];
  share_work(n, nb_process, &shares[0]);
  int i;
  for( i=1; i != nb_process; ++i) {
    shares[i]+= shares[i-1];
  }

  pid_t my_id;
  int p;
  int pipes_id[nb_process][2];
  for(p= 1; p != nb_process; ++p) {
    pipe(pipes_id[p-1]);
    fprintf(stderr,"forking one child\n");
    my_id= fork();
    if( my_id == 0 ) { // child process
      close(pipes_id[p-1][0]);
      break;
    }// only parent process after this line
    close(pipes_id[p-1][1]);
  }
  
  if( my_id ) { // parent
    // does own work
    fprintf(stderr,"master process working\n");
    size_t i; // reused for the two loops : we will read after the last result that we computed in the master process
    for(i=0; i != shares[0]; ++i) {
      f(((char const*const)(b))+i*in_size, ((char *const)(out))+i*out_size);
    }
    // copy results from children
    i *= out_size;
    size_t child;
    for(child=1; child != nb_process; ++child) { // reading time is considered negligible, otherwise we would asynch read
      size_t const chunk_size=(shares[child]-shares[child-1])*out_size;
      read(pipes_id[child-1][0], ((char *const)(out))+i, chunk_size);
      i+=chunk_size;
      close(pipes_id[child-1][0]);
    }
  } else { // child
    fprintf(stderr,"slave process %d working\n", p);
    size_t i;
    for(i= shares[p-1]; i != shares[p]; ++i) {
      f( ((char const*const)b)+i*in_size, ((char *)out)+i*out_size);
    }
    write(pipes_id[p-1][1], ((char *const)out)+shares[p-1]*out_size, (shares[p]-shares[p-1])*out_size);
    close(pipes_id[p-1][1]);
    exit(0);
  }
  return out;
}




static void* mc_apply_n_process(void const* const b, size_t in_size, size_t n, void* const out,  size_t out_size, void (*f)(void const*, void *), int nb_process){
  // _worker takes a const nb_process that can be used as an array size on the stack.
  if(nb_process == 0) { nb_process = omp_get_num_procs() ; }
  if(n<nb_process) { nb_process = n ;}
  return mc_apply_n_process_worker(b, in_size, n, out,out_size,  f, nb_process);
}

static int currentTop;
static int n, k, m;
static int scilab_function=0;


static void wrapper(double const* args, double * res) {
  
  SciErr err;
  {
    double* data;
    err= allocMatrixOfDouble(pvApiCtx, ++currentTop, 1, k, &data);
    memcpy(data, args, k*sizeof(double));
  }
  int const sci_arg_pos = currentTop;
  int const sci_rhs = 1;
  int const sci_lhs = 1;
  //  fprintf(stderr, " Nbvars = %d\n", 	Nbvars);
  int saveNbvars= Nbvars;
    // r = scilabfoo(x)	
    // C2F(scifunction) call a scilab function

  C2F(scifunction)(&sci_arg_pos, &scilab_function, &sci_lhs, &sci_rhs);
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





void* mc_apply_n_threads(void const* const b,  size_t in_size, size_t n, void* const out,  size_t out_size, void (*f)(void const*, void *), int nb_threads){
  size_t i;
  if(n <  nb_threads) { nb_threads = n;}
  if(nb_threads) {
    omp_set_num_threads(nb_threads);
  }
#pragma omp parallel for private(i)
  for(i=0; i < n; ++i)
  {
    (*f)(((char const*const)b)+i*in_size, ((char *const)out)+i*out_size);
  }
return out;
}

int sci_apply(char *fname) 
{
 
  double* args_data;
  double* res_data;
  double*tmp;
  
  CheckRhs(3,3);// args, fun, res_size
  CheckLhs(1,1);// res

  int type;
  SciErr err;
  int* addr;

  void (*function)(void const*, void*);

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
      fprintf(stderr,"funName: %s\n",funName);
    int found;
    found=SearchInDynLinks(funName, &function);
    if(found == -1) 
      { 
	//fprintf(stderr," pas trouvé function:%s \n",funName);
	//SciError( a pa trouvé);
	return 1;
      }
    scilab_function= 0;
    break;
  }
  case sci_c_function: { // scilab macro
  // get pointer on external function
    int m_fun, n_fun;
    GetRhsVar(2, EXTERNAL_DATATYPE, &m_fun, &n_fun, &scilab_function);
    function = wrapper;
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
  if(scilab_function) { // we have a macro
    mc_apply_n_process(args_data, k*sizeof(double), n, res_data,  m*sizeof(double), function, 0);
  }else {
    mc_apply_n_threads(args_data, k*sizeof(double), n, res_data,  m*sizeof(double), function, 0);
  }
  LhsVar(1) = res_pos;
	
  return 0;
}
