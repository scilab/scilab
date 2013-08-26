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

#ifndef __H5LINKSLIST_HXX__
#define __H5LINKSLIST_HXX__

#include "H5Object.hxx"
#include "H5ListObject.hxx"
#include "H5Group.hxx"
#include "H5Dataset.hxx"
#include "H5Type.hxx"
#include "H5Link.hxx"

namespace org_modules_hdf5
{
class H5Attribute;

class H5LinksList : public H5ListObject<H5Object>
{

public :

    H5LinksList(H5Object & _parent);

    ~H5LinksList();

    void setObject(const unsigned int pos, H5Object & obj);
    H5Object & getObject(const int pos);
    const unsigned int getSize() const;

    virtual std::string dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel) const;
    virtual std::string toString(const unsigned int indentLevel) const;

private:

    H5Object & getObject(const int pos, const bool checkPos);
};
}

#endif // __H5LINKSLIST_HXX__
