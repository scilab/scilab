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

#ifndef __H5GROUPSLIST_HXX__
#define __H5GROUPSLIST_HXX__

#include "H5NamedObjectsList.hxx"
#include "H5Group.hxx"

namespace org_modules_hdf5
{

class H5GroupsList : public H5NamedObjectsList<H5Group>
{

public :

    H5GroupsList(H5Group & _parent) : H5NamedObjectsList<H5Group>(_parent, H5O_TYPE_GROUP, -1, "H5 Group") { }

    ~H5GroupsList()
    {

    }
};
}

#endif // __H5GROUPSLIST_HXX__
