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

    virtual const unsigned int getSize() const = 0;

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
