/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
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

#ifndef __MLIST_HH__
#define __MLIST_HH__

#include <vector>
#include "tlist.hxx"

namespace types
{
class EXTERN_AST MList : public TList
{
public :
    MList() : TList() {}
    //Destructor uses ~TList()

    virtual MList*                  clone()
    {
        return new MList(this);
    }
    void                            whoAmI(void)
    {
        std::cout << "types::MList";
    };

    ScilabType                      getType(void)
    {
        return ScilabMList;
    }
    bool                            isMList()
    {
        return true;
    }
    bool                            isTList()
    {
        return false;
    }

    virtual bool invoke(typed_list & in, optional_list & opt, int _iRetCount, typed_list & out, const ast::Exp & e) override;

protected :
    MList(MList *_oMListCopyMe) : TList(_oMListCopyMe) {}
};
}

#endif /* __MLIST_HH__ */
