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

#ifndef __IL_STATE_HXX__
#define __IL_STATE_HXX__

#include "types.hxx"
#include "internal.hxx"

/**
* get il_state
*/
types::InternalType* get_il_state(void);

/**
* set il_state
*/
types::InternalType* set_il_state(types::InternalType* _state);

#endif /* __IL_STATE_HXX__ */
