/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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
