/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
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