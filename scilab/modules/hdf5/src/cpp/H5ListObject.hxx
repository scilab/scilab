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

#ifndef __H5LISTOBJECT_HXX__
#define __H5LISTOBJECT_HXX__

#include "H5Object.hxx"

namespace org_modules_hdf5
{

    template<typename T>
    class H5ListObject : public H5Object
    {

    public :

	H5ListObject(H5Object & _parent) : H5Object(_parent) { }

        virtual void setObject(const unsigned int pos, T & object) = 0;
        virtual T & getObject(const int pos) = 0;
        virtual const unsigned int getSize() const = 0;
    };
}

#endif // __H5FILE_HXX__
