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

#ifndef __H5LINK_HXX__
#define __H5LINK_HXX__

#include "H5Object.hxx"
#include "H5File.hxx"

namespace org_modules_hdf5
{

class H5Link : public H5Object
{

protected:

public:

    H5Link(H5Object & _parent, const std::string & name);

    virtual ~H5Link();

    virtual std::string dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel = 0) const
    {
        return "";
    };

    virtual std::string getLinkType() const
    {
        return "";
    }

    static H5Link & getLink(H5Object & _parent, const char * _name);
    static H5Link & getLink(H5Object & _parent, const std::string & _name);
};
}

#endif // __H5LINK_HXX__
