/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <string.h>
#include "cerro.h"
#include "erro.h"
/*--------------------------------------------------------------------------*/
void cerro(char *str)
{
    int l = 0;
    l = (int)strlen(str) + 1;
    C2F(erro)(str, l);
}
/*--------------------------------------------------------------------------*/
