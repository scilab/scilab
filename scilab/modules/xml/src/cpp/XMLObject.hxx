/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string>
#include <sstream>

extern "C" {
#include "xml_mlist.h"
}

namespace org_modules_xml
{
    class VariableScope;

    class XMLObject
    {
	
    public :
        XMLObject();
        virtual ~XMLObject() { }

        virtual XMLObject * getXMLObjectParent() = 0;
	virtual std::string * toString() { }
	virtual std::string * dump() { return new std::string(""); }

	int getId() { return id; }
	int createOnStack(int pos);

        template <class T>
        static T* getFromId(int id) { return static_cast<T*>(getVariableFromId(id)); }
	
	static std::string intToStr(int n)
	    {
		std::stringstream oss;
		oss << n;
		return oss.str();
	    }

    protected :
        int id;
	int scilabType;
	
        static VariableScope & scope;

    private :
        static XMLObject * getVariableFromId(int id);

    };
}
