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
