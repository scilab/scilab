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
/*--------------------------------------------------------------------------*/
#include "sci_malloc.h"
#include "scicos.h"
#include "scicos_block4.h"
#include "scicos_malloc.h"
#include "scicos_free.h"
#include "dynlib_scicos_blocks.h"

/*--------------------------------------------------------------------------*/
/*    Copyright INRIA
 *    Scicos block simulator
 *    event delay with discrete counter
 */
SCICOS_BLOCKS_IMPEXP void evtdly4(scicos_block *block, int flag)
{
    double t = 0.;
    time_counter_t** work = (time_counter_t**) block->work;
    time_counter_t* i = NULL;

    switch (flag)
    {
        /* init */
        case 4  :  /* the workspace is used to store discrete counter value */
        {
            if ((*work = (time_counter_t*) scicos_malloc(sizeof(time_counter_t))) == NULL)
            {
                set_block_error(-16);
                return;
            }
            i = *work;
            (*i) = 0;
            break;
        }

        /* event date computation */
        case 3  :
        {
            double dt;

            i = *work;
            t = get_scicos_time();
            (*i)++; /*increase counter*/
            dt = block->rpar[1] + (*i) * block->rpar[0] - t;
            /* on event enabled, use the default delay if not scheduled */
            if (block->rpar[1] >= 0 && dt < 0)
            {
                dt = block->rpar[0];
            }
            block->evout[0] = dt;
            break;
        }

        /* finish */
        case 5  :
        {
            scicos_free(*work); /*free the workspace*/
            break;
        }

        default :
            break;
    }
}
/*--------------------------------------------------------------------------*/
