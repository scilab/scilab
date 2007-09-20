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
      C2F(deletewin)(&win_id); //Old graphics ?
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
