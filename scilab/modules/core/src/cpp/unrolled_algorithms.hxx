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
/*-----------------------------------------------------------------------------------*/
#ifndef UNROLLED_ALGORITHMS_HXX
#define UNROLLED_ALGORITHMS_HXX

namespace {
	/*
	 * generic template unrolling for small vectors
	 */

/**
 * @TODO add comment
 *
 * @param   
 * @return <ReturnValue>
 */
	template<typename It1, typename It2, int N> struct eq_n_t 
	{
		bool operator()(It1 i1, It2 i2) const 
			{
				return (*i1 == *i2) && eq_n_t<It1, It2, N-1>()(++i1, ++i2) ;
			}
	};

/**
 * @TODO add comment
 *
 * @param   
 * @return <ReturnValue>
 */
	template<typename It1, typename It2> struct eq_n_t<It1, It2, 0> {
		bool operator()(It1 i1, It2 i2) const {
			return true ;
		}
	};

/**
 * @TODO add comment
 *
 * @param i1    
 * @param i2    
 * @return <ReturnValue>
 */
	template<int N, typename It1, typename It2> bool eq_n(It1 i1, It2 i2) {
		return eq_n_t<It1, It2, N>()(i1, i2);
	}

/**
 * ensure most efficient inlining irrespectively of optimization compiler options for ids of nsiz=6 ints
 *
 * @param id1
 * @param id2
 */
	template<> bool eq_n<6,int const*, int const*>(int const* id1, int const* id2) {
		return (id1[0]==id2[0]) && (id1[1]==id2[1]) && (id1[2]==id2[2]) && (id1[3]==id2[3]) && (id1[4]==id2[4]) && (id1[5]==id2[5]);
	}

/**
 * @TODO add comment
 *
 * @param id1
 * @param id2
 */
    template<> bool eq_n<6,int const*, int*>(int const* id1, int* id2)
	{
		return (id1[0]==id2[0]) && (id1[1]==id2[1]) && (id1[2]==id2[2]) && (id1[3]==id2[3]) && (id1[4]==id2[4]) && (id1[5]==id2[5]);
    }

/**
 * @TODO add comment
 *
 * @param id1
 * @param id2
 */
    template<> bool eq_n<6,int*, int const*>(int* id1, int const* id2)
	{
		return (id1[0]==id2[0]) && (id1[1]==id2[1]) && (id1[2]==id2[2]) && (id1[3]==id2[3]) && (id1[4]==id2[4]) && (id1[5]==id2[5]);
    }


/**
 * @TODO add comment
 *
 * @param id1
 * @param id2
 */
    template<> bool eq_n<6,int*, int*>(int* id1, int* id2)
	{
		return (id1[0]==id2[0]) && (id1[1]==id2[1]) && (id1[2]==id2[2]) && (id1[3]==id2[3]) && (id1[4]==id2[4]) && (id1[5]==id2[5]);
    }

/**
 * @TODO add comment
 *
 * @param i1    
 * @param i2    
 * @return <ReturnValue>
 */
	template<typename It1, typename It2, int N> struct less_n_t 
	{
		bool operator()(It1 i1, It2 i2) const 
			{
				return (*i1<*i2) ? true : ( (*i1>*i2) ? false : less_n_t<It1, It2, N-1>()(++i1, ++i2));
			}
	};

/**
 * @TODO add comment
 *
 * @param i1    
 * @param i2    
 * @return <ReturnValue>
 */
	template<typename It1, typename It2> struct less_n_t<It1, It2, 0> 
	{
		bool operator()(It1 i1, It2 i2) const 
			{
				return false ;
			}
	};

/**
 * @TODO add comment
 *
 * @param i1    
 * @param i2    
 * @return <ReturnValue>
 */
	template<int N, typename It1, typename It2> bool less_n(It1 i1, It2 i2)
	{
		return less_n_t<It1, It2, N>()(i1, i2);
	}

/**
 * @TODO add comment
 *
 * @param in
 * @param out    
 * @return <ReturnValue>
 */
	template<typename In, typename Out, int N> struct copy_n_t
	{
		Out operator()(In in, Out out) const
			{
				*out= *in;
				return copy_n_t<In, Out, N-1>()(++in, ++out);
			}
	};

/**
 * @TODO add comment
 *
 * @param in
 * @param out    
 * @return <ReturnValue>
 */
	template<typename In, typename Out> struct copy_n_t<In, Out, 0>
	{
		Out operator()(In, Out out) const
			{
				return out;
			}
	};

/**
 * @TODO add comment
 *
 * @param in
 * @param out    
 * @return <ReturnValue>
 */
	template<int N, typename In, typename Out> Out copy_n(In in, Out out)
	{
		return copy_n_t<In, Out, N>()(in, out);
	}

}
#endif
/*-----------------------------------------------------------------------------------*/
