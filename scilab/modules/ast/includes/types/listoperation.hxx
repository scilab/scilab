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

#ifndef __LISTOPERATION_HXX__
#define __LISTOPERATION_HXX__

#include <list>
#include "types.hxx"

namespace types
{
class EXTERN_AST ListOperation : public InternalType
{
public :
    ListOperation(): InternalType() {};
    virtual                 ~ListOperation();

    ListOperation*          clone();

    inline ScilabType       getType(void)
    {
        return ScilabListOperation;
    }
    inline ScilabId         getId(void)
    {
        return IdListOperation;
    }

    bool                    isListOperation()
    {
        return true;
    }

    void                    whoAmI();

    bool                    toString(std::ostringstream& ostr) override;
    virtual bool            isAssignable(void)
    {
        return false;
    }

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::string    getTypeStr()
    {
        return "listoperation";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::string    getShortTypeStr()
    {
        return "";
    }
};
}


#endif /* !__LISTOPERATION_HXX__ */
