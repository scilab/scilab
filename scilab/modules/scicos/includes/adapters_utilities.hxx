/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef ADAPTERS_UTILITIES_HXX_
#define ADAPTERS_UTILITIES_HXX_

enum startOrEnd
{
    Start = 0,
    End = 1
};

struct link_t
{
    size_t block;
    size_t port;
    enum startOrEnd kind;
};

#endif /* ADAPTERS_UTILITIES_HXX_ */
