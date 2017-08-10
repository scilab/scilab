/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2009-2009 - DIGITEO - Antoine ELIAS
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

#include <sstream>
#include <cstdio>

#include "listinsert.hxx"
#include "localization.h"

namespace types
{

ListInsert::ListInsert(InternalType* _pIT)
{
    _pIT->IncreaseRef();
    m_pInsert = _pIT;
}

ListInsert::~ListInsert()
{
    if (m_pInsert != NULL)
    {
        m_pInsert->DecreaseRef();
        if (m_pInsert->isDeletable())
        {
            delete m_pInsert;
            m_pInsert = NULL;
        }
    }
}

void ListInsert::whoAmI()
{
    std::cout << "types::ListInsert";
}

ListInsert* ListInsert::clone(void)
{
    return new ListInsert(m_pInsert);
}

bool ListInsert::toString(std::wostringstream& ostr)
{
    ostr << L"FIXME : Implement ListInsert::toString" << std::endl;
    return true;
}

InternalType* ListInsert::getInsert()
{
    return m_pInsert;
}
}
