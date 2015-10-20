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

#ifndef __BASE_OPPOSITES_HXX__
#define __BASE_OPPOSITES_HXX__

#include <complex>

namespace jit
{
    namespace op
    {

	template<typename T, typename U>
	struct Opposite
	{

	    inline U operator()(T x)
		{
		    return (U)(-x);
		}
	};

    } // namespace op

} // namespace jit

#endif // __BASE_OPPOSITES_HXX__
