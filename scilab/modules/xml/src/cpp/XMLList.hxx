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

#ifndef __XMLLIST_HXX__
#define __XMLLIST_HXX__

#include <string>
#include "XMLRemovable.hxx"

#include "xml.h"

namespace org_modules_xml
{
class XMLObject;

/**
 * @file
 * @author Calixte DENIZET <calixte.denizet@scilab.org>
 *
 * Virtual class to handle a list of XMLObjects
 */
class XMLList: public XMLObject, public XMLRemovable
{

public:
    /**
     * Gets the element with the given index.
     * @param index the element index
     * @return the corresponding object
     */
    virtual const XMLObject *getListElement(int index) = 0;

    /**
     * Default constructor
     */
    XMLList();

    /**
     * @return the list size
     */
    int getSize() const
    {
        return size;
    }

    void incrementSize()
    {
        size++;
    }

    /**
     * Get the content of each node of the list
     * @return an array of strings
     */
    virtual const char **getContentFromList() const = 0;

    /**
     * Get the name of each node of the list
     * @return an array of strings
     */
    virtual const char **getNameFromList() const = 0;

    const std::string toString() const;

protected:
    int size;

    /**
     * Gets an element in a linked list with a given index.
     * The element is reached from a previous element which has an index.
     * This way to search the element is faster in a for loop where the indexes are
     * consecutives.
     * @param index the searched index
     * @param max the max
     * @param prev a pointer on the previous index (*prev is modified by this function)
     * @param prevElem a pointer on the previous element (*prevElem is modified by this function)
     * @return the found element
     */
    template < typename T > static T *getListElement(int index, int max, int *prev, T ** prevElem)
    {
        if (index >= 1 && index <= max)
        {
            if (index != *prev)
            {
                if (index < *prev)
                {
                    for (int i = *prev; i > index; i--, *prevElem = (*prevElem)->prev)
                    {
                        ;
                    }
                }
                else
                {
                    for (int i = *prev; i < index; i++, *prevElem = (*prevElem)->next)
                    {
                        ;
                    }
                }
                *prev = index;
            }

            return *prevElem;
        }

        return 0;
    }
};
}

#endif
