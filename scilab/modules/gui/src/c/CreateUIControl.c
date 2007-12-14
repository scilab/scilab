/*------------------------------------------------------------------------*/
/* file: CreateUicontrol.c                                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Vincent COUVERT                                              */
/* Create Java object matching Scilab uicontrol                           */
/*------------------------------------------------------------------------*/

#include "CreateUIControl.h"
#include "ObjectStructure.h"
#include "CreateUIControl.h"
#include "PushButton.h"
/**CreateUIControl
 * This function creates Uicontrol structure.
 * @param char style[]: the type of the uicontrol object
 * @return  : pointer sciPointObj if ok , NULL if not
 */
sciPointObj * CreateUIControl(char *style)
{
  int ** userData = NULL ;
  int *  udSize   = NULL ;
  char *callback = NULL;

  sciPointObj *pobj = (sciPointObj *) NULL;
  sciUicontrol *ppobj=NULL;

  if ((pobj = MALLOC (sizeof (sciPointObj))) == NULL)	return (sciPointObj *) NULL;

  sciSetEntityType (pobj, SCI_UICONTROL);

  if ((pobj->pfeatures = MALLOC ((sizeof (sciUicontrol)))) == NULL)
    {
      FREE(pobj);
      return (sciPointObj *) NULL;
    }
  ppobj=pUICONTROL_FEATURE (pobj);

  if ((pUICONTROL_FEATURE (pobj)->callback = CALLOC(1,sizeof(char))) == NULL )
    {
      sciprint(_("No more place to allocates text string, try a shorter string"));
      return (sciPointObj *) NULL;
    }

  strcpy(pUICONTROL_FEATURE (pobj)->callback,"");
  pUICONTROL_FEATURE (pobj)->callbacklen = 0;
  pUICONTROL_FEATURE (pobj)->MenuPosition=0;
  pUICONTROL_FEATURE (pobj)->CallbackType=0;
  pUICONTROL_FEATURE (pobj)->Enable=TRUE;


  /* add the handle in the handle list */
  if ( sciAddNewHandle(pobj) == -1 )
    {
      FREE( pobj->pfeatures ) ;
      FREE( pobj ) ;
      return NULL ;
    }

  /* no sons for now */
  sciInitSelectedSons( pobj ) ;
      
  sciGetRelationship(pobj)->psons        = NULL ;
  sciGetRelationship(pobj)->plastsons    = NULL ;
  sciGetRelationship(pobj)->pSelectedSon = NULL ;
      
  sciInitVisibility( pobj, TRUE ) ;
      
  sciGetPointerToUserData( pobj, &userData, &udSize ) ;
  *userData = NULL ;
  *udSize   = 0    ;
      
      
  //pobj->pObservers = DoublyLinkedList_new() ;
  //createDrawingObserver( pobj ) ;
      
      
  pobj->pDrawer = NULL ;
      
  /* Default Style is PushButton */
  if (style == NULL || strcmp(style,"pushbutton")==0)
    {
      createPushButton((sciPointObj *) pobj);
      if ((pUICONTROL_FEATURE (pobj)->style = (char*)MALLOC(sizeof("pushbutton")+1))==NULL)
        {
          Scierror(999, _("No more memory.\n"));
          return NULL;
        }
      else
        {
          strcpy(pUICONTROL_FEATURE (pobj)->style, "pushbutton");
        }
    }
  else
    {
      sciprint(_("Could not create uicontrol of type %s."), style);
    }
      
  return (sciPointObj *) pobj;
}
/*----------------------------------------------------------------------------*/
