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

namespace scilab {
	namespace core {
		/*
		* generic template unrolling for small vectors
		*/

		template<typename It1, typename It2, int N> struct eq_n_t {
			bool operator()(It1 i1, It2 i2) const {
				return (*i1 == *i2) && eq_n_t<It1, It2, N-1>()(++i1, ++i2) ;
			}
		};
		template<typename It1, typename It2> struct eq_n_t<It1, It2, 0> {
			bool operator()(It1 i1, It2 i2) const { 
				return true ;
			}
		};
		template<int N, typename It1, typename It2> bool eq_n(It1 i1, It2 i2) {
			return eq_n_t<It1, It2, N>()(i1, i2);
		}

		template<typename It1, typename It2, int N> struct less_n_t {
			bool operator()(It1 i1, It2 i2) const {
				return (*i1<*i2) ? true : ( (*i1>*i2) ? false : less_n_t<It1, It2, N-1>()(++i1, ++i2));
			}
		};
		template<typename It1, typename It2> struct less_n_t<It1, It2, 0> {
			bool operator()(It1 i1, It2 i2) const {
				return false ;
			}
		};

		template<int N, typename It1, typename It2> bool less_n(It1 i1, It2 i2) {
			return less_n_t<It1, It2, N>()(i1, i2);
		}

		template<typename In, typename Out, int N> struct copy_n_t {
			Out operator()(In in, Out out) const {
				*out= *in;
				return copy_n_t<In, Out, N-1>()(++in, ++out);
			}
		};
		template<typename In, typename Out> struct copy_n_t<In, Out, 0> {
			Out operator()(In, Out out) const {
				return out;
			}
		};
		template<int N, typename In, typename Out> Out copy_n(In in, Out out) {
			return copy_n_t<In, Out, N>()(in, out);
		}

	}
}
#endif
/*-----------------------------------------------------------------------------------*/
