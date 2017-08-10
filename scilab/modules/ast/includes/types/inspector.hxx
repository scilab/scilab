/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
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

#ifndef __INSPECTOR_HXX__
#define __INSPECTOR_HXX__

//disable warnings about exports STL items
#ifdef _MSC_VER
#pragma warning (disable : 4251)
#endif

#include <unordered_set>
#include <string>
#include "dynlib_ast.h"

namespace types
{
class InternalType;

#ifndef NDEBUG
class EXTERN_AST Inspector
{
protected :
    static std::unordered_set<InternalType*> m_vIT;

public :
    static size_t getItemCount();
    static size_t getUnreferencedItemCount();

    static void addItem(InternalType* _pIT);
    static void removeItem(InternalType* _pIT);

    static InternalType* getItem(size_t _iPos);
    static InternalType* getUnreferencedItem(size_t _iPos);

    static std::wstring showItem(size_t _iPos);
    static std::wstring showUnreferencedItem(size_t _iPos);
    static void deleteItems();
    static void displayMemleak();
};
#endif
}
#endif /* !__INSPECTOR_HXX__ */
