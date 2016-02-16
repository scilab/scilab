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

#ifndef __H5LISTOBJECT_HXX__
#define __H5LISTOBJECT_HXX__

#include "H5Object.hxx"
#include "H5Attribute.hxx"

namespace org_modules_hdf5
{

template<typename T>
class H5ListObject : public H5Object
{

public :

    H5ListObject(H5Object & _parent) : H5Object(_parent), indexSize(0), indexList(0) { }
    H5ListObject(H5Object & _parent, const unsigned int size, const unsigned int * index) : H5Object(_parent), indexSize(size), indexList(index) { }

    virtual ~H5ListObject()
    {
        if (indexList)
        {
            delete indexList;
        }
    }

    virtual bool isList() const
    {
        return true;
    }

    virtual void setObject(const unsigned int pos, T & object) = 0;
    virtual T & getObject(const int pos) = 0;
    virtual T & getObject(const std::string & name)
    {
        throw H5Exception(__LINE__, __FILE__, _("Invalid operation"));
    }

    virtual unsigned int getSize() const = 0;

    virtual void getAccessibleAttribute(const double index, const int pos, void * pvApiCtx) const
    {
        T & obj = const_cast<H5ListObject<T> *>(this)->getObject((int)index);
        obj.createOnScilabStack(pos, pvApiCtx);
    }

    virtual void getAccessibleAttribute(const std::string & name, const int pos, void * pvApiCtx) const
    {
        T & obj = const_cast<H5ListObject *>(this)->getObject(name);
        obj.createOnScilabStack(pos, pvApiCtx);
    }

protected :

    const unsigned int indexSize;
    const unsigned int * indexList;

};
}

#endif // __H5FILE_HXX__
