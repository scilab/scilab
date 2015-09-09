/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __BASE_OR_HXX__
#define __BASE_OR_HXX__

#include <complex>

#include "Cast.hxx"

namespace jit
{
    namespace op
    {

	template<bool are_integral, typename T, typename U, typename V>
	struct __Or_helper;

	template<typename T, typename U, typename V>
	struct __Or_helper<true, T, U, V>
	{
	    inline V operator()(T x, U y)
		{
		    return (V)x | (V)y;
		}
	};

	template<typename T, typename U, typename V>
	struct __Or_helper<false, T, U, V>
	{
	    inline V operator()(T x, U y)
		{
		    return (x != 0) || (y != 0);
		}
	};

	template<typename U, typename V>
	struct __Or_helper<false, const std::complex<double> &, U, V>
	{

	    inline V operator()(const std::complex<double> & x, U y)
		{
		    return (x != 0.) || ((double)y != 0);
		}
	};

	template<typename T, typename V>
	struct __Or_helper<false, T, const std::complex<double> &, V>
	{

	    inline V operator()(T x, const std::complex<double> & y)
		{
		    return ((double)x != 0.) || (y != 0.);
		}
	};

	template<typename V>
	struct __Or_helper<false, const std::complex<double> &, const std::complex<double> &, V>
	{

	    inline V operator()(const std::complex<double> & x, const std::complex<double> & y)
		{
		    return (x != 0.) || (y != 0.);
		}
	};

	template<typename T, typename U, typename V>
	struct Or
	{
	    inline V operator()(T x, U y)
		{
		    return __Or_helper<jit::is_pure_integral<T>::value && jit::is_pure_integral<U>::value, T, U, V>()(x, y);
		}
	};

    } // namespace op

} // namespace jit

#endif // __BASE_OR_HXX__
