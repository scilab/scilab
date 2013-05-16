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
#pragma warning (disable : 4251)

#include <vector>
#include <string>

extern "C"
{
#include "dynlib_types.h"
}

#include "internal.hxx"

namespace types
{
#ifndef NDEBUG
class TYPES_IMPEXP Inspector
{
protected :
    static std::vector<InternalType*> m_vIT;

public :
    static int getItemCount();
    static int getUnreferencedItemCount();

    static void addItem(InternalType* _pIT);
    static void removeItem(InternalType* _pIT);

    static InternalType* getItem(int _iPos);
    static InternalType* getUnreferencedItem(int _iPos);

    static std::wstring showItem(int _iPos);
    static std::wstring showUnreferencedItem(int _iPos);
    static void deleteItems();
};
#endif
}
#endif /* !__INSPECTOR_HXX__ */