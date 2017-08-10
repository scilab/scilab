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
