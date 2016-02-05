/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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

#ifndef __CONTAINER_HXX__
#define __CONTAINER_HXX__

#include "types.hxx"

namespace types
{
class EXTERN_AST Container : public GenericType
{

public :
    Container() : GenericType() {}
    virtual                 ~Container() {}

    virtual void            whoAmI(void)
    {
        std::cout << "types::Container";
    }

    virtual int             getSize() = 0 ;

    virtual bool            isAssignable(void)
    {
        return true;
    }

    virtual ScilabType      getType(void)
    {
        return ScilabContainer;
    }
    virtual bool            isContainer(void)
    {
        return true;
    }

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring    getTypeStr() = 0;
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring    getShortTypeStr() = 0;
};
}

#endif /* !__CONTAINER_HXX__ */
