/*------------------------------------------------------------------------*/
/* file: CreateUIContextMenu.h                                            */
/* Copyright INRIA 2008                                                   */
/* Authors : Vincent COUVERT                                              */
/*------------------------------------------------------------------------*/

#ifndef __CREATE_UICONTEXTMNEU_H__
#define __CREATE_UICONTEXTMNEU_H__

#include "ObjectStructure.h" /* sciPointObj */
#include "MALLOC.h" /* MALLOC */
#include "sciprint.h"
#include "localization.h"
#include "ObjectSelection.h" /* sciInitSelectedSons... */
#include "HandleManagement.h" /* sciGetRelationship... */
#include "SetProperty.h" /* sciInitVisibility... */
#include "GetProperty.h" /* sciGetPointerToUserData */

/**
 * CreateUIContextMenu
 * This function creates UicontextMenu structure.
 * @return  : pointer sciPointObj if ok , NULL if not
 */
sciPointObj * CreateUIContextMenu(void);

#endif /* !__CREATE_UICONTEXTMNEU_H__ */
