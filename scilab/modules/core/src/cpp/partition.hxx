/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Bernard HUGUENEY
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*
*/
/*-----------------------------------------------------------------------------------*/
#ifndef PARTITION_HXX
#define PARTITION_HXX

#include <iterator>

namespace scilab
{
namespace core
{
/* generic dichotomy function used to search in the sorted vector
precondition :  [f,f+n) is p-partitioned : for any i in [f,f+n)
(p(*i) == true) => (p(*j) == true) for every j in [i,f+n]
returns the first iterator i in [f,f+n) for which p(*i) is true, or f+n .
*/
template<typename I, typename P>
I partition_point_n(I f, typename std::iterator_traits<I>::difference_type n, P p)
{
    while (n)  // when n==0, we have narrowed the interval to the singleton result
    {
        typename std::iterator_traits<I>::difference_type h = n >> 1; // n>0 -> n>>1 == n/2
        I m = f + h; // we test at the middle of the interval
        if (p(*m)) // partition point is amongst the [f,m]
        {
            n = h;
        }
        else     // partition point is amongst the ]m,f+n)
        {
            n = n - (h + 1);
            f = m + 1;
        }
    }
    return f;
}
}
}
#endif
/*-----------------------------------------------------------------------------------*/
