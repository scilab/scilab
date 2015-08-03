/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include "il_state.hxx"

#include "types.hxx"
#include "internal.hxx"

/*--------------------------------------------------------------------------*/
static types::InternalType* il_state_save = nullptr;
/*--------------------------------------------------------------------------*/
types::InternalType* get_il_state(void)
{
    return il_state_save;
}
/*--------------------------------------------------------------------------*/
types::InternalType* set_il_state(types::InternalType* _state)
{
    il_state_save = _state;
    return il_state_save;
}
/*--------------------------------------------------------------------------*/
