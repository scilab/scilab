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

#ifndef __VARIABLESCOPE_HXX__
#define __VARIABLESCOPE_HXX__

#include <map>
#include <stack>
#include <vector>
#include <libxml/xmlmemory.h>

namespace org_modules_xml
{
class XMLObject;
class XMLNodeList;

/**
 * @file
 * @author Calixte DENIZET <calixte.denizet@scilab.org>
 *
 * Class to handle the mapping between XMLObjects and their id
 */
class VariableScope
{

    std::vector < XMLObject * >*scope;
    int position;
    int initialSize;
    std::stack < int >*freePlaces;

    static std::map < const XMLObject *, std::map < const XMLObject *, bool>*> parentToChildren;
    static std::map < void *, XMLObject * > mapLibXMLToXMLObject;
    static std::map < void *, XMLNodeList * > mapLibXMLToXMLNodeList;
    static xmlFreeFunc XMLFreeFunc;

public:
    /**
     * Registers a pointer and its associated object.
     * The aim of this mapping is to delete an existing object when a pointer
     * in the xml tree is freed.
     * @param libxml a pointer in the xml tree
     * @param obj the corresponding object
     */
    static void registerPointers(void *libxml, XMLObject * obj);

    /**
     * Unregisters a pointer. It can be used when a pointer in the tree is freed or
     * locally to avoid cyclic dependencies on removal.
     * @param libxml a pointer in the xml tree
     */
    static void unregisterPointer(void *libxml);

    /**
     * Unregisters a pointer. It can be used when a pointer in the tree is freed or
     * locally to avoid cyclic dependencies on removal.
     * @param libxml a pointer in the xml tree
     */
    static void unregisterNodeListPointer(void *libxml);

    /**
     * Registers a pointer and its associated object.
     * The aim of this mapping is to delete an existing object when a pointer
     * in the xml tree is freed.
     * @param libxml a pointer in the xml tree
     * @param nodeList the corresponding nodeList
     */
    static void registerPointers(void *libxml, XMLNodeList * nodeList);

    /**
     * Default constructor
     * @param initialSize the default size of the scope
     */
    VariableScope(int initialSize);

    /**
     * Destructor
     */
    ~VariableScope();

    /**
     * Gets the variable id from the object
     * @param obj the object
     * @return the corresponding id
     */
    int getVariableId(const XMLObject & obj);

    /**
     * Gets the object from the id
     * @param id the object id
     * @return the object pointer or 0 if not found
     */
    XMLObject *getVariableFromId(int id);

    /**
     * Removes an id from the scope
     * @param id the id
     */
    void removeId(int id);

    /**
     * Gets the XMLObject associated with a libxml pointer
     * @param libxml the libxml pointer
     * @return the XMLObject pointer
     */
    XMLObject *getXMLObjectFromLibXMLPtr(void *libxml) const;

    /**
     * Gets the XMLNodeList associated with a libxml pointer
     * @param libxml the libxml pointer
     * @return the XMLNodeList pointer
     */
    XMLNodeList *getXMLNodeListFromLibXMLPtr(void *libxml) const;

private:
    static void _xmlFreeFunc(void *mem);
    static void initXMLMemory();
    static xmlFreeFunc getFreeFunc(xmlFreeFunc freeFunc);

    static void removeChildFromParent(const XMLObject * child);

    /**
     * Removes the object dependencies if they exist
     * @param obj the object
     */
    void removeDependencies(XMLObject * obj);
};
}

#endif
