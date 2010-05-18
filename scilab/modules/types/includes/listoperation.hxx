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
    class ListOperation : public InternalType
    {
    public :
                                ListOperation(): InternalType(){};
        virtual                 ~ListOperation();

        ListOperation*          clone();

        ListOperation*          getAsListOperation(void);
        RealType                getType(void);

        void                    whoAmI();

        std::string             toString(int _iPrecision, int _iLineLen);
        virtual bool            isAssignable(void) { return false; }

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::string     getTypeStr() {return string("listoperation");}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::string     getShortTypeStr() {return string("");}
    };
}


#endif /* !__LISTOPERATION_HXX__ */
