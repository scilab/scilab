/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __H5ATTRIBUTES_HXX__
#define __H5ATTRIBUTES_HXX__

#include "H5Object.hxx"
#include "H5Type.hxx"
#include "H5Data.hxx"
#include "H5Dataspace.hxx"
#include "H5File.hxx"

namespace org_modules_hdf5
{
class H5Attribute : public H5Object
{
    hid_t attr;
    std::string name;

public :

    H5Attribute(H5Object & _parent, const unsigned int pos);
    H5Attribute(H5Object & _parent, const std::string & name);
    H5Attribute(H5Object & _parent, hid_t _attr, const char * _name);
    H5Attribute(H5Object & _parent, hid_t _attr, const std::string & _name);

    ~H5Attribute();

    hid_t getH5Id() const
    {
        return attr;
    }

    virtual const std::string & getName() const
    {
        return name;
    }

    H5Data & getData();
    H5Type & getDataType();
    H5Dataspace & getSpace();
    virtual void getAccessibleAttribute(const std::string & _name, const int pos, void * pvApiCtx) const;

    virtual std::string dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel) const;
    virtual std::string toString(const unsigned int indentLevel) const;

    static hid_t create(H5Object & loc, const std::string & name, hid_t type, hid_t targettype, hid_t space, void * data);
};
}

#endif // __H5ATTRIBUTE_HXX__
