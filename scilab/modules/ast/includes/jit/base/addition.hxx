/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __BASE_ADDITIONS_HXX__
#define __BASE_ADDITIONS_HXX__

#include <complex>

#include "Cast.hxx"


namespace jit
{
    namespace op
    {

	template<typename T, typename U, typename V>
	struct Add
	{

	    inline V operator()(T x, U y)
		{
		    return (V)x + (V)y;
		}
	};
	
	template<typename U>
	struct Add<const std::complex<double> &, U, std::complex<double>>
	{

	    inline std::complex<double> operator()(const std::complex<double> & x, U y)
		{
		    return x + (double)y;
		}
	};

	template<typename T>
	struct Add<T, const std::complex<double> &, std::complex<double>>
	{

	    inline std::complex<double> operator()(T x, const std::complex<double> & y)
		{
		    return (double)x + y;
		}
	};

	template<>
	struct Add<const std::complex<double> &, const std::complex<double> &, std::complex<double>>
	{

	    inline std::complex<double> operator()(const std::complex<double> & x, const std::complex<double> & y)
		{
		    return x + y;
		}
	};

    } // namespace op

} // namespace jit

#endif // __BASE_ADDITIONS_HXX__
