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

#include "VariableScope.hxx"
#include "XMLObject.hxx"

namespace org_modules_xml
{
    VariableScope::VariableScope(int initialSize_)
    {
        position = -1;
        this->initialSize = initialSize_;
        this->scope = new XMLObject*[initialSize_];
	std::fill(this->scope, this->scope + initialSize_, static_cast<XMLObject *>(0));
	this->freePlaces = new std::stack<int>();
    }

    VariableScope::~VariableScope()
    {
        for (int i = 0; i <= position; i++)
        {
            if (scope[i])
            {
                delete scope[i];
            }
        }
        delete[] scope;
        delete freePlaces;
    }

    int VariableScope::getVariableId(const XMLObject & obj)
    {
        int returnValue;

        if (freePlaces->size() != 0)
        {
            returnValue = freePlaces->top();
            freePlaces->pop();
        }
        else
        {
            if (position == initialSize - 1)
            {
                int newSize = initialSize * 2;
                XMLObject ** newScope = new XMLObject*[newSize];
		std::fill(this->scope, this->scope + initialSize, static_cast<XMLObject *>(0));
	        for (int i = 0; i < initialSize; i++)
                {
                    newScope[i] = scope[i];
                }
                delete[] scope;
                initialSize = newSize;
                scope = newScope;
            }
            returnValue = ++position;
        }
        scope[returnValue] = const_cast<XMLObject*>(&obj);

        return returnValue;
    }

    XMLObject * VariableScope::getVariableFromId(int id)
    {
        if (id >= 0 && id < initialSize)
        {
            return scope[id];
        }

        return 0;
    }
}
