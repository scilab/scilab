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
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
* See the file ./license.txt
*/
#include "scicos_block4.h"
/*    Copyright INRIA
 *    Scicos block simulator
 *    event delay with discrete counter
 */

#if _MSC_VER
#define NULL    0
#endif

void evtdly4(scicos_block *block,int flag)
{
  double t;
  long long int *i;

  switch(flag)
  {
   /* init */
   case 4  : {/* the workspace is used to store discrete counter value */
              if ((*block->work=scicos_malloc(sizeof(long long int)))==NULL) {
                set_block_error(-16);
                return;
              }
              i=*block->work;
              (*i)=0;
              break;
             }

   /* event date computation */
   case 3  : {
              i=*block->work;
              t=get_scicos_time();
              (*i)++; /*increase counter*/
              block->evout[0]=block->rpar[1]+ \
                              (*i)*block->rpar[0]-t;
              break;
             }

   /* finish */
   case 5  : {
              scicos_free(*block->work); /*free the workspace*/
              break;
             }

   default : break;
  }
}
