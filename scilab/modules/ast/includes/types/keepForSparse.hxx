/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __KEEPFORSPARSE__
#define __KEEPFORSPARSE__

/*
 * used by Eigen to prune sparse matrices.
 * Scilab <6 keeps values <%eps, so we prune only 0. but it should really be a property of the sparse matrix.
 */
template<typename T>
bool keepForSparse(std::size_t /* unused */, std::size_t /* unused */, T const& v)
{
    static T const zero(0);
    return v != zero;
}
#endif // __KEEPFORSPARSE__