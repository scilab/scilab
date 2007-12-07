/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/* Vincent COUVERT 2007 - Java version */
/*--------------------------------------------------------------------------*/ 
#include "gw_gui.h"
#include "machine.h"
#include "stack-c.h"
#include "CallFileChooser.h"
#include "MALLOC.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_xgetfile) _PARAMS((char *fname,unsigned long fname_len))
{
  int fileChooserID = 0;
  
  int nbRow = 0, nbCol = 0;

  char **userSelection = NULL;
  int voidSelectionAdr = 0;

  char *optName = NULL;

  int titleAdr = 0;
  int initialDirectoryAdr = 0;
  int fileMaskAdr = 0;

  char *title = NULL, *fileMask = NULL, *initialDirectory = NULL;

  CheckRhs(0,3);
  CheckLhs(1,1);

  if ((optName = (char*)MALLOC(sizeof(char*)*(strlen("title")+1))) == NULL)
    {
      Scierror(999, _("No more momory."));
      return FALSE;
    }
  
  
  if (Rhs==1)
    {
      /* Argument is file_mask or optional title */
      if (IsOpt(1, optName))
        {
          if (strcmp(optName,"title")==0)
            {
              GetRhsVar(1, STRING_DATATYPE, &nbRow, &nbCol, &titleAdr);
              if (nbCol !=1)
                {
                  Scierror(999, _("Wrong type for first parameter, should be a single string."));
                  FREE(optName);
                  return FALSE;
                }
              title = cstk(titleAdr);
            }
          else
            {
              Scierror(999, _("Optional parameter must be 'title='."));
              FREE(optName);
              return FALSE;
            }
        }
      else if (VarType(1) == sci_strings)
        {
          GetRhsVar(1, STRING_DATATYPE, &nbRow, &nbCol, &fileMaskAdr);
          if (nbCol !=1)
            {
              Scierror(999, _("Wrong type for first parameter, should be a single string."));
              FREE(optName);
              return FALSE;
            }
          fileMask = cstk(fileMaskAdr);
       }
    }
  else if (Rhs==2)
    {
      /* First argument is file_mask */
      if (VarType(1) == sci_strings)
        {
          GetRhsVar(1, STRING_DATATYPE, &nbRow, &nbCol, &fileMaskAdr);
          if (nbCol !=1)
            {
              Scierror(999, _("Wrong type for first parameter, should be a single string."));
              FREE(optName);
              return FALSE;
            }
          fileMask = cstk(fileMaskAdr);
        }
      else
        {
          Scierror(999, _("Wrong type for first parameter, should be a single string."));
          FREE(optName);
          return FALSE;
        }

      /* Second argument is initial directory or optional title */
      if (IsOpt(2, optName))
        {
          if (strcmp(optName,"title")==0)
            {
              GetRhsVar(2, STRING_DATATYPE, &nbRow, &nbCol, &titleAdr);
              if (nbCol !=1)
                {
                  Scierror(999, _("Wrong type for first parameter, should be a single string."));
                  FREE(optName);
                  return FALSE;
                }
              title = cstk(titleAdr);
            }
          else
            {
              Scierror(999, _("Optional parameter must be 'title='."));
              FREE(optName);
              return FALSE;
            }
        }
      else if (VarType(2) == sci_strings)
        {
          GetRhsVar(2, STRING_DATATYPE, &nbRow, &nbCol, &initialDirectoryAdr);
          if (nbCol !=1)
            {
              Scierror(999, _("Wrong type for second parameter, should be a single string."));
              FREE(optName);
              return FALSE;
            }
              initialDirectory = cstk(initialDirectoryAdr);
        }
    }
  else if (Rhs==3)
    {
      /* First argument is file_mask */
      if (VarType(1) == sci_strings)
        {
          GetRhsVar(1, STRING_DATATYPE, &nbRow, &nbCol, &fileMaskAdr);
          if (nbCol !=1)
            {
              Scierror(999, _("Wrong size for first parameter, should be a single string."));
               FREE(optName);
             return FALSE;
            }
          fileMask = cstk(fileMaskAdr);
       }
      else
        {
          Scierror(999, _("Wrong type for first parameter, should be a single string."));
          return FALSE;
        }

      /* Second argument is initial directory */
       if (VarType(2) == sci_strings)
        {
          GetRhsVar(2, STRING_DATATYPE, &nbRow, &nbCol, &initialDirectoryAdr);
          if (nbCol !=1)
            {
              Scierror(999, _("Wrong size for second parameter, should be a single string."));
              FREE(optName);
              return FALSE;
            }
              initialDirectory = cstk(initialDirectoryAdr);
        }
       else
        {
          Scierror(999, _("Wrong type for second parameter, should be a single string."));
              FREE(optName);
          return FALSE;
        }
     
      /* Third argument is title or optional title */
      if (IsOpt(3, optName))
        {
          if (strcmp(optName,"title")==0)
            {
              GetRhsVar(3, STRING_DATATYPE, &nbRow, &nbCol, &titleAdr);
              if (nbCol !=1)
                {
                  Scierror(999, _("Wrong type for first parameter, should be a single string."));
                  FREE(optName);
                  return FALSE;
                }
              title = cstk(titleAdr);
            }
          else
            {
              Scierror(999, _("Optional parameter must be 'title='."));
              FREE(optName);
              return FALSE;
            }
        }
      else if (VarType(3) == sci_strings)
        {
          GetRhsVar(3, STRING_DATATYPE, &nbRow, &nbCol, &titleAdr);
          if (nbCol !=1)
            {
              Scierror(999, _("Wrong size for third parameter, should be a single string."));
              FREE(optName);
              return FALSE;
            }
          title = cstk(titleAdr);
        }
    }

  /* Create the Java Object */
  fileChooserID = createFileChooser();
  
  /* Set options */
  if (title != NULL)
    {
      setFileChooserTitle(fileChooserID, title);
    }

  if (initialDirectory != NULL)
    {
      setFileChooserInitialDirectory(fileChooserID, initialDirectory);
    }

  if (fileMask != NULL)
    {
      setFileChooserMask(fileChooserID, fileMask);
    }

  /* Display it and wait for a user input */
  fileChooserDisplayAndWait(fileChooserID);

  /* Read the size of the selection, if 0 then no file selected */
  nbRow = getFileChooserSelectionSize(fileChooserID);

  if (nbRow !=0 ) 
    {
      /* The user selected a file --> returns the files names */
      nbCol = 1;
      
      userSelection = getFileChooserSelection(fileChooserID);

      CreateVarFromPtr(Rhs+1, MATRIX_OF_STRING_DATATYPE, &nbRow, &nbCol, userSelection);
    }
  else
    {
      /* The user canceled the selection --> returns an empty matrix */
      nbRow = 0;
      nbCol = 0;

      CreateVar(Rhs+1, STRING_DATATYPE, &nbRow, &nbCol, &voidSelectionAdr);
    }
  
  LhsVar(1)=Rhs+1;
  C2F(putlhsvar)();

  FREE(optName);

  return TRUE;
}
/*--------------------------------------------------------------------------*/
