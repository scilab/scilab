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

#ifndef __H5SOFTLINK_HXX__
#define __H5SOFTLINK_HXX__

#include "H5Link.hxx"

namespace org_modules_hdf5
{

class H5SoftLink : public H5Link
{
public:

    H5SoftLink(H5Object & _parent, const char * name) : H5Link(_parent, name) { }
    H5SoftLink(H5Object & _parent, const std::string & name) : H5Link(_parent, name) { }

    virtual ~H5SoftLink() { }

    virtual std::string toString(const unsigned int indentLevel) const;
    virtual std::string dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel = 0) const;
    virtual std::string ls() const;
    virtual void printLsInfo(std::ostringstream & os) const;

    std::string getLinkValue() const;
    virtual std::string getLinkType() const;

    virtual void getAccessibleAttribute(const std::string & name, const int pos, void * pvApiCtx) const;
};
}

#endif // __H5SOFTLINK_HXX__
