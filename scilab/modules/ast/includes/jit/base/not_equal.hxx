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

#ifndef __BASE_NOT_EQUAL_HXX__
#define __BASE_NOT_EQUAL_HXX__

#include <complex>

#include "Cast.hxx"


namespace jit
{
    namespace op
    {

	template<typename T, typename U, typename V>
	struct Neq
	{
	    typedef typename ScilabType<T, U>::type __type;
	    
	    inline V operator()(T x, U y)
		{
		    return (__type)x != (__type)y;
		}
	};

	template<typename U, typename V>
	struct Neq<const std::complex<double> &, U, V>
	{

	    inline V operator()(const std::complex<double> & x, U y)
		{
		    return x != (double)y;
		}
	};

	template<typename T, typename V>
	struct Neq<T, const std::complex<double> &, V>
	{

	    inline V operator()(T x, const std::complex<double> & y)
		{
		    return (double)x != y;
		}
	};

	template<typename V>
	struct Neq<const std::complex<double> &, const std::complex<double> &, V>
	{

	    inline V operator()(const std::complex<double> & x, const std::complex<double> & y)
		{
		    return x != y;
		}
	};
		
    } // namespace op

} // namespace jit

#endif // __BASE_NOT_EQUAL_HXX__
