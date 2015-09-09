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

#ifndef __BASE_LDIVISIONS_HXX__
#define __BASE_LDIVISIONS_HXX__

#include <complex>

#include "Cast.hxx"


namespace jit
{
    namespace op
    {

	template<typename T, typename U, typename V>
	struct Ldiv
	{

	    inline V operator()(T x, U y)
		{
		    return (V)y / (V)x;
		}
	};
	
	template<typename U>
	struct Ldiv<const std::complex<double> &, U, std::complex<double>>
	{

	    inline std::complex<double> operator()(const std::complex<double> & x, U y)
		{
		    return (double)y / x;
		}
	};

	template<typename T>
	struct Ldiv<T, const std::complex<double> &, std::complex<double>>
	{

	    inline std::complex<double> operator()(T x, const std::complex<double> & y)
		{
		    return y / (double)x;
		}
	};

	template<>
	struct Ldiv<const std::complex<double> &, const std::complex<double> &, std::complex<double>>
	{

	    inline std::complex<double> operator()(const std::complex<double> & x, const std::complex<double> & y)
		{
		    return y / x;
		}
	};

    } // namespace op

} // namespace jit

#endif // __BASELDIVISIONS_HXX__
