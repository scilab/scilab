/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ObjectStructure.h" /* sciPointObj */
#include "MALLOC.h" /* MALLOC */
#include "sciprint.h"
#include "localization.h"
#include "ObjectSelection.h" /* sciInitSelectedSons... */
#include "HandleManagement.h" /* sciGetRelationship... */
#include "SetProperty.h" /* sciInitVisibility... */
#include "GetProperty.h" /* sciGetPointerToUserData */

/**CreateUIControl
 * This function creates Uicontrol structure.
 * @param char style[]: the type of the uicontrol object
 * @return  : pointer sciPointObj if ok , NULL if not
 */
sciPointObj * CreateUIControl(char *style);
