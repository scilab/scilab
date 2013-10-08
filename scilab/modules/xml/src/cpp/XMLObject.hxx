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

#ifndef __XMLOBJECTS_HXX__
#define __XMLOBJECTS_HXX__

#include <string>
#include <sstream>
#include <typeinfo>

extern "C"
{
#include "xml_mlist.h"
#include "dynlib_xml_scilab.h"
}

namespace org_modules_xml
{
class VariableScope;

/**
 * @file
 * @author Calixte DENIZET <calixte.denizet@scilab.org>
 *
 * Base class for the XML objects.
 */
class XML_SCILAB_IMPEXP XMLObject
{

public:
    /**
     * Default constructor
     */
    XMLObject();

    /**
     * Destructor
     */
    virtual ~ XMLObject()
    {
    }

    /**
     * Get the libxml2 pointer
     * @return the pointer
     */
    virtual void *getRealXMLPointer() const
    {
        return 0;
    }

    /**
     * Gets a XML parent object. A set of dependencies is created between the objects
     * to be sure that all the XML objects will be freed when a document will be destroyed.
     * @return the parent XMLObject
     */
    virtual const XMLObject *getXMLObjectParent() const
    {
        return 0;
    }

    /**
     * Sets the attribute value.
     * @param name the attribute names
     * @param value the attribute values
     * @param size the number of names
     */
    virtual void setAttributeValue(const char **name, const char **value, int size) const
    {
        return;
    }

    /**
     * Sets the attribute value with a prefix namespace.
     * @param prefix the namespace prefix or the namespace itself
     * @param name the attribute names
     * @param value the attribute values
     * @param size the number of names
     */
    virtual void setAttributeValue(const char **prefix, const char **name, const char **value, int size) const
    {
        return;
    }

    /**
     * @return the string representation of this object
     */
    virtual const std::string toString() const
    {
        return std::string("");
    }

    /**
     * @return a dump of this object
     */
    virtual const std::string dump(bool indent) const
    {
        return std::string("");
    }

    /**
     * @return the object id
     */
    int getId() const
    {
        return id;
    }

    /**
     * Creates the Scilab's variable corresponding to this object
     * @param pos the stack position
     * @return 1 if all is ok, else 0
     */
    int createOnStack(int pos, void *pvApiCtx) const;

    /**
     * @param id the object id
     * @return the object which has the corresponding id or 0 if not found
     */
    template < class T > static T *getFromId(int id)
    {
        return static_cast < T * >(getVariableFromId(id));
    }

protected:
    int id;
    int scilabType;

    static VariableScope *scope;

    /**
     * Reset the scope
     */
    static void resetScope();

private:

    /**
     * @param id the id
     * @return XMLObject corresponding to the id or 0 if not found
     */
    static XMLObject *getVariableFromId(int id);
};
}

#endif
