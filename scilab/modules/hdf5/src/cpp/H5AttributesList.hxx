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
    virtual const unsigned int getSize() const;

    virtual std::string dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel) const;
    virtual std::string toString(const unsigned int indentLevel) const;

private:
    H5Attribute & getObject(const int pos, const bool checkPos);
};
}

#endif // __H5ATTRIBUTESLIST_HXX__
