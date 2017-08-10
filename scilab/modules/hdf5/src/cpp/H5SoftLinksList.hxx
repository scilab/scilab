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
