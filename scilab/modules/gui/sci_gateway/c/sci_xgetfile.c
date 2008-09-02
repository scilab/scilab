/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2007 - INRIA - Vincent COUVERT (java version)
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
#include "gw_gui.h"
#include "machine.h"
#include "stack-c.h"
#include "CallFileChooser.h"
#include "MALLOC.h"
#include "localization.h"
#include "Scierror.h"
#include "cluni0.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_xgetfile)(char *fname,unsigned long fname_len)
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
      Scierror(999, _("%s: No more memory.\n"),fname);
      return FALSE;
    }
  
  
  if (Rhs==1)
    {
      /* Argument is file_mask or optional title */
      if (VarType(1) == sci_strings) /* Check type */
        {
          if (IsOpt(1, optName)) /* Check if optional title */
            {
              if (strcmp(optName,"title")==0) /* Check if optional parameter is title */
                {
                  GetRhsVar(1, STRING_DATATYPE, &nbRow, &nbCol, &titleAdr);
                  if (nbCol !=1)
                    {
                      Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"),fname, 1);
                      FREE(optName);
                      return FALSE;
                    }
                  title = cstk(titleAdr);
                }
              else
                {
                  Scierror(999, _("%s: Wrong name for optional input argument #%d: '%s' expected.\n"),fname, 1, "title");
                  FREE(optName);
                  return FALSE;
                }
            }
          else
            {
              GetRhsVar(1, STRING_DATATYPE, &nbRow, &nbCol, &fileMaskAdr);
              if (nbCol !=1)
                {
                  Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"),fname, 1);
                  FREE(optName);
                  return FALSE;
                }
              fileMask = cstk(fileMaskAdr);
            }
        }
      else
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"),fname, 1);
          FREE(optName);
          return FALSE;
        }
    }
  else if (Rhs==2)
    {
      /* First argument is file_mask */
      if (VarType(1) == sci_strings) /* Check type */
        {
          GetRhsVar(1, STRING_DATATYPE, &nbRow, &nbCol, &fileMaskAdr);
          if (nbCol !=1)
            {
              Scierror(999,  _("%s: Wrong size for input argument #%d: A string expected.\n"),fname,1);
              FREE(optName);
              return FALSE;
            }
          fileMask = cstk(fileMaskAdr);
        }
      else
        {
          Scierror(999,  _("%s: Wrong type for input argument #%d: A string expected.\n"),fname, 1);
          FREE(optName);
          return FALSE;
        }

      /* Second argument is initial directory or optional title */
      if (VarType(2) == sci_strings) /* Check type */
        {
          if (IsOpt(2, optName)) /* Check if optional title */
            {
              if (strcmp(optName,"title")==0) /* Check if optional parameter is title */
                {
                  GetRhsVar(2, STRING_DATATYPE, &nbRow, &nbCol, &titleAdr);
                  if (nbCol !=1)
                    {
                      Scierror(999,  _("%s: Wrong size for input argument #%d: A string expected.\n"),fname, 2);
                      FREE(optName);
                      return FALSE;
                    }
                  title = cstk(titleAdr);
                }
              else
                {
                  Scierror(999, _("%s: Wrong name for optional input argument #%d: '%s' expected.\n"),fname, 2, "title");
                  FREE(optName);
                  return FALSE;
                }
            }
          else
            {
              GetRhsVar(2, STRING_DATATYPE, &nbRow, &nbCol, &initialDirectoryAdr);
              if (nbCol !=1)
                {
                  Scierror(999,  _("%s: Wrong size for input argument #%d: A string expected.\n"),fname, 2);
                  FREE(optName);
                  return FALSE;
                }
              initialDirectory = cstk(initialDirectoryAdr);
            }
        }
      else
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"),fname, 2);
          FREE(optName);
          return FALSE;
        }
    }
  else if (Rhs==3)
    {
      /* First argument is file_mask */
      if (VarType(1) == sci_strings) /* Check type */
        {
          GetRhsVar(1, STRING_DATATYPE, &nbRow, &nbCol, &fileMaskAdr);
          if (nbCol !=1)
            {
              Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"),fname, 1);
              FREE(optName);
              return FALSE;
            }
          fileMask = cstk(fileMaskAdr);
        }
      else
        {
          Scierror(999,  _("%s: Wrong type for input argument #%d: A string expected.\n"),fname, 1);
          return FALSE;
        }

      /* Second argument is initial directory */
      if (VarType(2) == sci_strings) /* Check type */
        {
          GetRhsVar(2, STRING_DATATYPE, &nbRow, &nbCol, &initialDirectoryAdr);
          if (nbCol !=1)
            {
              Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"),fname, 2);
              FREE(optName);
              return FALSE;
            }
          initialDirectory = cstk(initialDirectoryAdr);
        }
      else
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"),fname, 2);
          FREE(optName);
          return FALSE;
        }
     
      /* Third argument is title or optional title */
      if (VarType(3) == sci_strings)  /* Check type */
        {
          if (IsOpt(3, optName)) /* Check if optional title */
            {
              if (strcmp(optName,"title")==0) /* Check if optional parameter is title */
                {
                  GetRhsVar(3, STRING_DATATYPE, &nbRow, &nbCol, &titleAdr);
                  if (nbCol !=1)
                    {
                      Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"),fname, 3);
                      FREE(optName);
                      return FALSE;
                    }
                  title = cstk(titleAdr);
                }
              else
                {
                  Scierror(999, _("%s: Wrong name for optional input argument #%d: '%s' expected.\n"),fname, 3, "title");
                  FREE(optName);
                  return FALSE;
                }
            }
          else
            {
              GetRhsVar(3, STRING_DATATYPE, &nbRow, &nbCol, &titleAdr);
              if (nbCol !=1)
                {
                  Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"),fname, 3);
                  FREE(optName);
                  return FALSE;
                }
              title = cstk(titleAdr);
            }
        }
      else
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"),fname, 3);
          FREE(optName);
          return FALSE;
        }
    }

  /* Create the Java Object */
  fileChooserID = createFileChooser();

  /* Do not allow to select a directory */
  setFileChooserFileSelectionOnly(fileChooserID);
  
  /* Set options */
  if (title != NULL)
    {
      setFileChooserTitle(fileChooserID, title);
    }

  if (initialDirectory != NULL)
    {
		char expandedpath[PATH_MAX+1];
		int out_n = 0;
		C2F(cluni0)(initialDirectory ,expandedpath, &out_n,(int)strlen(initialDirectory),PATH_MAX);
		setFileChooserInitialDirectory(fileChooserID, expandedpath);
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
