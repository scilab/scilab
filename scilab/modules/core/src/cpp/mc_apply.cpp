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
#include "mc_apply.hxx"

#include <omp.h>
#include <iterator>

extern "C" {
#include <unistd.h>

#include <stdlib.h>
}

#include <iostream>
#include <algorithm>

void* mc_apply_n_threads(void const* const b,  std::size_t in_size, std::size_t n, void* const out,  std::size_t out_size, void (*f)(void const*, void *), int nb_threads){
  std::size_t i;
  if(n <  nb_threads) { nb_threads = n;}
  if(nb_threads) {
    omp_set_num_threads(nb_threads);
  }
#pragma omp parallel for private(i)
  for(i=0; i < n; ++i)
  {
    f(static_cast<char const*const>(b)+i*in_size, static_cast<char *const>(out)+i*out_size);
  }
return out;
}

/*
 * "divides" s in n integers. Used to share s tasks amongst n workers.
 */
template<typename  Sz, typename Nb, typename Out> 
Out share_work(Sz s, Nb n, Out o){
  while(n) {
    if(n==1) {
      *o= s;
    } else {
      Sz const r(s/n);
      *o= r;
      s-= r;
    }
    --n;
    ++o;
  }
  return o;
}



void* mc_apply_n_process_worker(void const* const b, std::size_t in_size, std::size_t n, void* const out,  std::size_t out_size, void (*f)(void const*, void *), int const nb_process){

  //  std::cerr<<"using "<<nb_process<<" for "<<n<<" tasks.\n";

  std::size_t shares[nb_process];
  share_work(n, nb_process, &shares[0]);
  for( int i(1); i != nb_process; ++i) {
    shares[i]+= shares[i-1];
  }

  pid_t my_id;
  int p;
  int pipes_id[nb_process][2];
  for(p= 1; p != nb_process; ++p) {
    pipe(pipes_id[p-1]);
    my_id= fork();
    if( my_id == 0 ) { // child process
      close(pipes_id[p-1][0]);
      break;
    }// only parent process after this line
    close(pipes_id[p-1][1]);
  }
  
  if( my_id ) { // parent
    // does own work
    std::size_t i; // reused for the two loops : we will read after the last result that we computed in the master process
    for(i=0; i != shares[0]; ++i) {
      //      std::cerr<<"processing:"<<*reinterpret_cast<double const*const>(static_cast<char const*const>(b)+i*in_size)<<" ";
      f(static_cast<char const*const>(b)+i*in_size, static_cast<char *const>(out)+i*out_size);
    }
    // copy results from children
    i *= out_size;
    for(std::size_t child(1); child != nb_process; ++child) { // reading time is considered negligible, otherwise we would asynch read
      std::size_t const chunk_size((shares[child]-shares[child-1])*out_size);
      read(pipes_id[child-1][0], static_cast<char *const>(out)+i, chunk_size);
      i+=chunk_size;
      close(pipes_id[child-1][0]);
    }
  } else { // child
    for(std::size_t i(shares[p-1]); i != shares[p]; ++i) {
      //      std::cerr<<"processing:"<<*reinterpret_cast<double const*const>(static_cast<char const*const>(b)+i*in_size)<<" ";
      f(static_cast<char const*const>(b)+i*in_size, static_cast<char *>(out)+i*out_size);
    }
    write(pipes_id[p-1][1], static_cast<char *const>(out)+shares[p-1]*out_size, (shares[p]-shares[p-1])*out_size);
    close(pipes_id[p-1][1]);
    exit(0);
  }
  return out;
}


