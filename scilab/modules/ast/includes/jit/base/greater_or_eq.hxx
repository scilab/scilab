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

#ifndef __BASE_GREATER_OR_EQ_HXX__
#define __BASE_GREATER_OR_EQ_HXX__

#include "Cast.hxx"

namespace jit
{
    namespace op
    {

	template<typename T, typename U, typename V>
	struct Ge
	{
	    typedef typename ScilabType<T, U>::type __type;
	    
	    inline V operator()(T x, U y)
		{
		    return (__type)x >= (__type)y;
		}
	};

    } // namespace op

} // namespace jit

#endif // __BASE_GREATER_OR_EQ_HXX__
