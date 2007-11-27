/*--------------------------------------------------------------------------*/
/* Allan CORNET INRIA 2006 */
/* Vincent COUVERT INRIA 2007 */
/*--------------------------------------------------------------------------*/ 
#include "gw_gui.h"
#include "machine.h"
#include "machine.h"
#include "stack-c.h"
#include "localization.h"
#include "Scierror.h"
#include "InitUIMenu.h"
/*--------------------------------------------------------------------------*/
int sci_unsetmenu(char *fname,unsigned long fname_len)
{
  int nbRow=0, nbCol=0, stkAdr=0;
  int nbRow1=0, nbCol1=0, stkAdr1=0;

  // Check parameter number
  CheckRhs(1,2);
  CheckLhs(1,1);

  if (Rhs == 1)
    {
      // Unset a Menu of Scilab Main Window
      if (VarType(1) != sci_strings)
        {
          Scierror(999, _("Input parameter must be a character string.\n"));
          return FALSE;
        }

      GetRhsVar(1, STRING_DATATYPE, &nbRow, &nbCol, &stkAdr);

      if (nbCol != 1)
        {
          Scierror(999, _("Input parameter must be a single character string.\n"));
          return FALSE;
        }
     EnableRootMenu(cstk(stkAdr), FALSE);
    }
  else
    {
      // Unset a Menu a Scilab Graphic Window
      if (VarType(1) != sci_matrix)
        {
          Scierror(999, _("First input parameter must be a double value.\n"));
          return FALSE;
        }
      GetRhsVar(1, STRING_DATATYPE, &nbRow, &nbCol, &stkAdr);
      
      if (nbRow*nbCol != 1)
        {
          Scierror(999, _("First input parameter must be a scalar.\n"));
          return FALSE;
        }

      if (VarType(2) != sci_strings)
        {
          Scierror(999, _("Second input parameter must be a character string.\n"));
          return FALSE;
        }
      GetRhsVar(2, STRING_DATATYPE, &nbRow1, &nbCol1, &stkAdr1);

      EnableFigureMenu((int)stk(stkAdr), cstk(stkAdr), FALSE);
    }
  
  LhsVar(1)=0;
  C2F(putlhsvar)();
  return 0;
}
/*--------------------------------------------------------------------------*/
