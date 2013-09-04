/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __H5SOFTLINKSLIST_HXX__
#define __H5SOFTLINKSLIST_HXX__

#include "H5NamedObjectsList.hxx"
#include "H5SoftLink.hxx"
#include "H5Group.hxx"

namespace org_modules_hdf5
{

class H5SoftLinksList : public H5NamedObjectsList<H5SoftLink>
{

public :

    H5SoftLinksList(H5Group & _parent) : H5NamedObjectsList<H5SoftLink>(_parent, H5O_TYPE_GROUP, H5L_TYPE_SOFT, "H5 Soft Link") { }

    ~H5SoftLinksList()
    {

    }
};
}

#endif // __H5SOFTLINKSLIST_HXX__
