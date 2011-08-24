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

#ifndef __VARIABLESCOPE_HXX__
#define __VARIABLESCOPE_HXX__

#include <map>
#include <stack>

extern "C"
{
#include <libxml/xmlmemory.h>
}

namespace org_modules_xml
{
    class XMLObject;

    /**
     * @file
     * @author Calixte DENIZET <calixte.denizet@scilab.org>
     *
     * Class to handle the mapping between XMLObjects and their id
     */
    class VariableScope
    {

        XMLObject ** scope;
        int position;
        int initialSize;
        std::stack<int> * freePlaces;
        static std::map<void *, XMLObject *> * mapLibXMLToXMLObject;
        static xmlFreeFunc XMLFreeFunc;

    public :

        /**
         * Registers a pointer and its associated object.
         * The aim of this mapping is to delete an existing object when a pointer
         * in the xml tree is freed.
         * @param libxml a pointer in the xml tree
         * @param obj the corresponding object
         */
        static void registerPointers(void * libxml, XMLObject * obj);

        /**
         * Unregisters a pointer. It can be used when a pointer in the tree is freed or
         * locally to avoid cyclic dependencies on removal.
         * @param libxml a pointer in the xml tree
         */
        static void unregisterPointer(void * libxml);

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
         * Gets the variable id from the objec
         * @param obj the object
         * @return the corresponding id
         */
        int getVariableId(const XMLObject & obj);

        /**
         * Gets the object from the id
         * @param id the object id
         * @return the object pointer or 0 if not found
         */
        XMLObject * getVariableFromId(int id);

        /**
         * Remove an id from the scope
         * @param id the id
         */
        template <class T> void removeId(int id)
            {
                if (id >= 0 && id < initialSize && scope[id])
                {
                    removeDependencies<T>(scope[id]);
                    scope[id] = static_cast<XMLObject *>(0);
                    freePlaces->push(id);
                }
            }

    private :
        static void _xmlFreeFunc(void * mem);
        static void initXMLMemory();
        static xmlFreeFunc getFreeFunc(xmlFreeFunc freeFunc);

        /**
         * Remove the object dependencies if they exist
         * @param obj the object
         */
        template <class T> void removeDependencies(XMLObject * obj)
            {
                for (int i = 0; i <= position; i++)
                {
                    if (scope[i] && reinterpret_cast<T *>(scope[i])->getXMLObjectParent() == obj)
                    {
                        delete reinterpret_cast<T *>(scope[i]);
                    }
                }
            }
    };
}

#endif
