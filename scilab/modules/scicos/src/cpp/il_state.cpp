/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
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
