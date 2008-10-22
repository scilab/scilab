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
/**
   \file scoMisc.c
   \author Benoit Bayol
   \version 1.0
   \date September 2006 - January 2007
   \brief Source Code for Misc function (not only for the sco lib)
*/

#include "scoBase.h"
#include "scoMisc.h"
#include "scoGetProperty.h"
#include "scoSetProperty.h"
#include "sciprint.h"

void scoScopeError(ScopeMemory * pScopeMemory, int code_error)
{
  extern void set_block_error(int err);
  scoGraphicalObject pScopeWindow;
  scoInteger win_id;

  switch(code_error)
    {
    case 0:
      sciprint("SCOPE ERROR : sciDrawObj cannot be performed, pShortDraw is NULL for unknown reason !\n Maybe you have destroyed the windows or any parent of curves\n");
      break;
    case 1:
      sciprint("SCOPE ERROR : Error during malloc - Check Memory\n");
      break;
    default:
      sciprint("SCOPE ERROR : Unknown code error ! Please report it to developpers :)\n");
      break;
    }

  pScopeWindow = scoGetPointerScopeWindow(pScopeMemory);
  win_id = scoGetWindowID(pScopeMemory);
  if (pScopeWindow != NULL )
    {
      //C2F(deletewin)(&win_id); Old graphics ?
	  sciDeleteWindow(win_id );
    }

  set_block_error(-16);
}

/* permut pobj_1/pobj_2 ptrs coming from
 * pparent_1/pparent_2 hdl(s)
 */
int permutobj(sciPointObj *pobj_1, sciPointObj *pparent_1,
              sciPointObj *pobj_2, sciPointObj *pparent_2)
{
 sciSons *OneSon, *OneSon2;

 OneSon=(sciGetRelationship (pparent_1)->psons);
 while (OneSon->pointobj!=pobj_1)
   OneSon=(sciSons *)OneSon->pnext;
 OneSon2=(sciGetRelationship (pparent_2)->psons);
 while (OneSon2->pointobj!=pobj_2)
   OneSon2=(sciSons *)OneSon2->pnext;
 OneSon->pointobj=pobj_2;
 OneSon2->pointobj=pobj_1;

 return 0;
}
