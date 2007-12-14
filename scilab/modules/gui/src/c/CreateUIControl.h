/*------------------------------------------------------------------------*/
/* file: CreateUIControl.h                                               */
/* Copyright INRIA 2007                                                   */
/* Authors : Vincent COUVERT                                              */
/*------------------------------------------------------------------------*/

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
