/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2010 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __LISTDELETE_HXX__
#define __LISTDELETE_HXX__

#include "types.hxx"
#include "listoperation.hxx"

namespace types
{
class EXTERN_AST ListDelete : public ListOperation
{
public :
    ListDelete(): ListOperation() {};
    virtual                 ~ListDelete();

    // FIXME : Should not return NULL;
    ListDelete*             clone();

    inline ScilabType       getType(void)
    {
        return ScilabListDeleteOperation;
    }
    inline ScilabId         getId(void)
    {
        return IdListDeleteOperation;
    }

    bool                    isListDelete()
    {
        return true;
    }

    void                    whoAmI();

    bool                    toString(std::ostringstream& ostr) override;

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::string    getTypeStr()
    {
        return "listdelete";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::string    getShortTypeStr()
    {
        return "0";
    }
};
}


#endif /* !__LISTDELETE_HXX__ */
