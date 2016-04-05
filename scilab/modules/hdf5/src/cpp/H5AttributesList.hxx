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

#ifndef __H5ATTRIBUTESLIST_HXX__
#define __H5ATTRIBUTESLIST_HXX__

#include "H5ListObject.hxx"

namespace org_modules_hdf5
{
class H5Attribute;

class H5AttributesList : public H5ListObject<H5Attribute>
{

public :

    H5AttributesList(H5Object & _parent);
    H5AttributesList(H5Object & _parent, const unsigned int size, const unsigned int * index);

    ~H5AttributesList();

    virtual void setObject(const unsigned int pos, H5Attribute & attribute);
    virtual H5Attribute & getObject(const int pos);
    virtual H5Attribute & getObject(const std::string & name);
    virtual unsigned int getSize() const;

    virtual std::string dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel) const;
    virtual std::string toString(const unsigned int indentLevel) const;

private:
    H5Attribute & getObject(const int pos, const bool checkPos);
};
}

#endif // __H5ATTRIBUTESLIST_HXX__
