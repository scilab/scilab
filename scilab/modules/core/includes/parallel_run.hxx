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
#include <iterator>
#include <iostream>

#include <cstdlib>
#include <unistd.h>
#include <omp.h>


namespace {


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


  template<typename F>
  struct parallel_wrapper {
    parallel_wrapper(void const* const* const a, std::size_t const* a_s, std::size_t const* a_n, std::size_t const the_rhs, std::size_t nb_tasks, void * const* const r, std::size_t const* r_s, std::size_t const the_lhs, F the_f)
      :args(a), args_size(a_s), args_nb(a_n), rhs(the_rhs), n(nb_tasks),res(r), res_size(r_s), lhs(the_lhs), f(the_f){}

    // we define a functor. Calling it lanches the parallel processing of args, either with threads of processes(default).
    // the nb of wokers (threads or processes) can also be specified (default is implementation défined usually nb of cores).
    // TODO : enable specification of near / far / all (must, must not, can share L2 cache), at least for threads.
    // so first arge might not stay boolean (we can add an overload)
    F operator()( bool with_threads=false, std::size_t nb_workers=0){
      return with_threads ? apply_with_threads(nb_workers) : apply_with_processes(nb_workers);
    }
  private:
    F apply_with_threads(std::size_t nb_threads) {
      std::size_t i;
      if(n <  nb_threads) { nb_threads = n;}
      if(nb_threads) {
	omp_set_num_threads(nb_threads);
      }
#pragma omp parallel for private(i)
      for(i=0; i < n; ++i)
	{
	  call_f(i);
	}
      return f;
    }

    F apply_with_processes(std::size_t nb_process) {
      if(nb_process == 0) { nb_process = omp_get_num_procs() ; }
      if(n<nb_process) { nb_process = n ;}

      std::size_t shares[nb_process];
      share_work(n, nb_process, &shares[0]);
      for( int i(1); i != nb_process; ++i) {
	shares[i]+= shares[i-1];
      }
      
      pid_t my_id=-1;// if no child is forked because nb_process=1, we are the master
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
	  call_f(i);
	}
	// copy results from children
	// select_res and read for each res
	for(std::size_t child(1); child != nb_process; ++child) { // reading time is considered negligible, otherwise we would asynch read
	  read_and_close(pipes_id[child-1][0], shares[child-1], shares[child]);
	}
      } else { // child
	for(std::size_t i(shares[p-1]); i != shares[p]; ++i) {
	  //      std::cerr<<"processing:"<<*reinterpret_cast<double const*const>(static_cast<char const*const>(b)+i*in_size)<<" ";
	  call_f(i);
	}
	write_and_close(shares[p-1], shares[p], pipes_id[p-1][1]);
	exit(0);
      }
      return f;
    }

    void call_f(std::size_t const i){
      void const * local_args[rhs];
      for(std::size_t j(0); j!= rhs; ++j) {
	local_args[j]= args[j]+ (i%(*(args_nb+j))) *(*(args_size+j));//(i%(*(args_nb+j))) because all args arer not required to have same nb of elts
      }
      void * local_res[lhs];
      for(std::size_t j(0); j!= lhs; ++j) {
	local_res[j]= res[j]+ i*(*(res_size+j));// all res are required to have n elts
      }
      //      std::cerr<<"local_args "<<*(double*)*local_args<<" local_res "<<*(double*)*local_res<<std::endl;
      f(local_args, local_res);
    }

    void write_and_close(std::size_t const i_begin, std::size_t const i_end, int fd){
      for(std::size_t j(0); j!= lhs; ++j){
	write(fd, res[j]+res_size[j]*i_begin, (i_end - i_begin)*res_size[j]);
      }
      close(fd);
    }
    // read each lhs from pipe
    void read_and_close(int fd, std::size_t const i_begin, std::size_t const i_end){
      for(std::size_t j(0); j!= lhs; ++j){
	read(fd, res[j]+res_size[j]* i_begin, (i_end - i_begin)*res_size[j]);
      }
      close(fd);
    }

    void const* const* const args;
    std::size_t const* args_size;
    std::size_t const* args_nb;
    std::size_t const rhs;
    std::size_t n;
    void * const* const res;
    std::size_t const* res_size;
    std::size_t const lhs;
    F& f;
  };

}


/*
 * make a parallel wrapper, just calls the constructor : is used mainly not to have to type the complete templated name of the wrapper type.
 * We can then directly call the parallel_wrapper operator() to lanch the parallel processing.
 */
template<typename ArgsItIt, typename ArgsSizeIt, typename ArgsNbIt, typename Sz1, typename Sz2, typename ResItIt, typename ResSizeIt, typename Sz3, typename F>   parallel_wrapper<F>
make_parallel_wrapper(ArgsItIt args, ArgsSizeIt args_size, ArgsNbIt args_nb, Sz1 rhs, Sz2 n, ResItIt res, ResSizeIt res_size, Sz3 lhs, F f){
  return parallel_wrapper<F>(args, args_size, args_nb, rhs, n, res, res_size, lhs, f); 
}





#endif
