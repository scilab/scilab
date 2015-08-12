/*  Scicos
 *
 *  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * See the file ./license.txt
 */

/**
 * INRIA 2008
 * Bruno JOFRET
 * Simone MANNORI
 */
#include "gw_scicos.h"
#include "stack-c.h"

/*--------------------------------------------------------------------------*/
typedef struct
{
    int halt;
}  COSHLT_struct;
extern COSHLT_struct  C2F(coshlt);
/*--------------------------------------------------------------------------*/
int sci_haltscicos(char *fname, unsigned long fname_len)
{

    CheckLhs(0, 1);
    CheckRhs(0, 0);

    // MAGIC VALUE: 0 is used to continue the simulation
    // MAGIC VALUE: 1 is used to halt the simulator
    // MAGIC VALUE: 2 is used to switch to the final time
    //                        then halt the simulator
    C2F(coshlt).halt = 2;

    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
