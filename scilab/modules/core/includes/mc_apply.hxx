#ifndef MC_APPLY_HXX
#define MC_APPLY_HXX

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

#include <algorithm>

#include <cstdlib>
#include <omp.h>



/*
 * apply a function to an array of arguments in parallel, using threads. 
 *
 * @param in starting address of the array of arguments
 * @param in_size size in bytes of each argument(s) consumed by one function invocation
 * @param n nb of function calls to be made on the array of arguments
 *
 * @param out starting address where to return the results of the function calls
 * @param out_size size in bytes of each results computed by the function calls
 * @param f ptr to function of type void (*f)(void const*, void *) :
 *      first arg is the address of the arguments, second is the adress where to store the results
 *
 * @param nb_threads nb of requested threads to make the n calls in parallel
 * If the nb of requested threads is 0, leave the choice up to the OpenMP implementation .
 *
 */


void* mc_apply_n_threads(void const* const in, std::size_t in_size, std::size_t n, void* const out,  std::size_t out_size, void (*f)(void const*, void *), int nb_threads=0);



void* mc_apply_n_process_worker(void const* const in, std::size_t in_size, std::size_t n, void* const out,  std::size_t out_size, void (*f)(void const*, void *), int const nb_process=0);

/*
 * apply a function to an array of arguments in parallel, using processes.
 *
 * @param in starting address of the array of arguments
 * @param in_size size in bytes of each argument(s) consumed by one function invocation
 * @param n nb of function calls to be made on the array of arguments
 *
 * @param out starting address where to return the results of the function calls
 * @param out_size size in bytes of each results computed by the function calls
 * @param f ptr to function of type void (*f)(void const*, void *) :
 *      first arg is the address of the arguments, second is the adress where to store the results
 *
 * @param nb_threads nb of requested processes to make the n calls in parallel
 * If the nb of requested processes is 0, we use the nb of processors (as seen by the OpenMP implementation) .
 *
 */

template<typename In, typename Sz1, typename Sz2, typename Out, typename Sz3, typename F> Out mc_apply_n_process(In b, Sz1 in_size, Sz2 n, Out out, Sz3 out_size, F f, int nb_process){
  // _worker takes a const nb_process that can be used as an array size on the stack.
  if(nb_process == 0) { nb_process = omp_get_num_procs() ; }
  if(n<nb_process) { nb_process = n ;}
  return static_cast<Out>(mc_apply_n_process_worker(b, in_size, n, out,out_size,  reinterpret_cast<void (*)(void const*, void *)>(f), nb_process));
}

#endif
