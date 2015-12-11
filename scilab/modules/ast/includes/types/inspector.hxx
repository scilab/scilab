/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
