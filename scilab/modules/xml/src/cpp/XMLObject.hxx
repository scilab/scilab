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

#ifndef __XMLOBJECTS_HXX__
#define __XMLOBJECTS_HXX__

#include <iostream>
#include <string>
#include <sstream>
#include <typeinfo>
#include <set>

extern "C"
{
#include "xml_mlist.h"
#include "dynlib_xml_scilab.h"
}

//#define SCILAB_DEBUG_XML

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

#ifdef SCILAB_DEBUG_XML
    static std::set<XMLObject *> pointers;
#endif

    /**
     * Default constructor
     */
    XMLObject();

    /**
     * Destructor
     */
    virtual ~ XMLObject()
    {
#ifdef SCILAB_DEBUG_XML
        //std::cout << "Delete = " << (void*)this << std::endl;
        pointers.erase(this);
#endif
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
    inline int getId() const
    {
        return id;
    }

    inline bool isValid() const
    {
        return valid;
    }

    inline void invalid()
    {
        valid = false;
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
    bool valid;

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
