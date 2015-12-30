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

#ifndef __LISTINSERT_HXX__
#define __LISTINSERT_HXX__

#include "types.hxx"
#include "listoperation.hxx"

namespace types
{
class EXTERN_AST ListInsert : public ListOperation
{
public :
    ListInsert(InternalType* _pIT);
    virtual                 ~ListInsert();

    ListInsert*             clone();

    inline ScilabType       getType(void)
    {
        return ScilabListInsertOperation;
    }
    inline ScilabId         getId(void)
    {
        return IdListInsertOperation;
    }


    bool                    isListInsert()
    {
        return true;
    }

    void                    whoAmI();

    bool                    toString(std::ostringstream& ostr) override;
    InternalType*           getInsert();

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::string    getTypeStr()
    {
        return "listinsert";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::string    getShortTypeStr()
    {
        return "";
    }

private :
    InternalType*           m_pInsert;
};
}


#endif /* !__LISTINSERT_HXX__ */
