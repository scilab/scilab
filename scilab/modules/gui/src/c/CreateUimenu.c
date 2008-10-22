/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * Create Java object matching Scilab uicontrol 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "CreateUimenu.h"

/**
 * CreateUimenu
 * This function creates Uimenu structure.
 * @return pointer sciPointObj if ok , NULL if not
 */
sciPointObj * CreateUimenu (void)
{
  sciPointObj *pobj = (sciPointObj *) NULL;

  if ((pobj = MALLOC (sizeof (sciPointObj))) == NULL)	return (sciPointObj *) NULL;

  sciSetEntityType (pobj, SCI_UIMENU);

  if ((pobj->pfeatures = MALLOC ((sizeof (sciUimenu)))) == NULL)
    {
      FREE(pobj);
      return (sciPointObj *) NULL;
    }

  /* Color property */
  pUIMENU_FEATURE (pobj)->foregroundcolor = NULL;

  /* Callback property */
  /* Callback getter returns "" if callback is NULL */
  pUIMENU_FEATURE (pobj)->callback = NULL;
  pUIMENU_FEATURE (pobj)->callbackType=0;

  pUIMENU_FEATURE (pobj)->handle_visible=TRUE;

  /* Initialize the tag */
  /* NULL value is displayed as "" in tag getter */
  pUIMENU_FEATURE (pobj)->tag = NULL;

  sciStandardBuildOperations(pobj, NULL);

  sciInitSelectedSons(pobj);
    
  InitUIMenu((sciPointObj *) pobj);
      
  return (sciPointObj *) pobj;
}
