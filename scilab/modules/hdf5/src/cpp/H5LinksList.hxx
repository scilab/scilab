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
