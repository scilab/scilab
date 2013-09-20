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

#ifndef __H5TYPESLIST_HXX__
#define __H5TYPESLIST_HXX__

#include "H5NamedObjectsList.hxx"
#include "H5Type.hxx"

namespace org_modules_hdf5
{

class H5Group;

class H5TypesList : public H5NamedObjectsList<H5Type>
{

public :

    H5TypesList(H5Group & _parent) : H5NamedObjectsList<H5Type>(_parent, H5O_TYPE_NAMED_DATATYPE, -1, "H5 Type") { }

    ~H5TypesList()
    {

    }
};
}

#endif // __H5TYPESLIST_HXX__
