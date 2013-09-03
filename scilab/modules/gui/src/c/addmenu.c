/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/****************************************************
 * Scilab interface for the AddMenu function
 *  Add a menu in  window  number win_num or in Main window
 *
 *  win_num     : graphic window number or -1 for main scilab window
 *  button_name : label of button
 *  entries     : labels of submenus if any (in scilab code)
 *  ptrentries  : table of pointers on each entries
 *  ne          : number of submenus
 *  typ         : Action mode
 *                typ==0 : interpreted (execution of scilab instruction
 *                typ!=0 : hard coded a routine is called
 *  fname;      : name of the action function
 *******************************************************/
#include "machine.h"

int C2F(addmen)(int *win_num,
                char *button_name,
                int *entries,
                int *ptrentries,
                int *ne
                , int *typ,
                char *fname,
                int *ierr)
{
    return 0;
}
