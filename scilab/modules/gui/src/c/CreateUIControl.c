/*------------------------------------------------------------------------*/
/* file: CreateUicontrol.c                                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Vincent COUVERT                                              */
/* Create Java object matching Scilab uicontrol                           */
/*------------------------------------------------------------------------*/

#include "CreateUIControl.h"
#include "BuildObjects.h"
#include "ObjectStructure.h"
#include "PushButton.h"

/**CreateUIControl
 * This function creates Uicontrol structure.
 * @param char style[]: the type of the uicontrol object
 * @return  : pointer sciPointObj if ok , NULL if not
 */
sciPointObj * CreateUIControl(char *style)
{
  sciPointObj *pobj = (sciPointObj *) NULL;

  if ((pobj = MALLOC (sizeof (sciPointObj))) == NULL)	return (sciPointObj *) NULL;

  sciSetEntityType (pobj, SCI_UICONTROL);

  if ((pobj->pfeatures = MALLOC ((sizeof (sciUicontrol)))) == NULL)
    {
      FREE(pobj);
      return (sciPointObj *) NULL;
    }

  if ((pUICONTROL_FEATURE (pobj)->callback = CALLOC(1,sizeof(char))) == NULL )
    {
      sciprint(_("No more place to allocates text string, try a shorter string"));
      return (sciPointObj *) NULL;
    }

  strcpy(pUICONTROL_FEATURE (pobj)->callback,"");
  pUICONTROL_FEATURE (pobj)->callbacklen = 0;
  pUICONTROL_FEATURE (pobj)->CallbackType = 0;
  pUICONTROL_FEATURE (pobj)->Enable = TRUE;

  /* Tag is set to NULL as default */
  /* The getter for tag returns "" is tag is NULL */
  pUICONTROL_FEATURE (pobj)->tag = NULL;

  /* Value is set to NULL as default */
  pUICONTROL_FEATURE (pobj)->value = NULL;
  pUICONTROL_FEATURE (pobj)->valueSize = 0;

  /* ListboxTop is set to NULL as default */
  /* The getter for ListboxTop returns [] is ListboxTop is NULL */
  pUICONTROL_FEATURE (pobj)->listboxTop = NULL;

  /* Min/Max default values */
  pUICONTROL_FEATURE (pobj)->max = 1;
  pUICONTROL_FEATURE (pobj)->min = 0;

  /* Font default */
  pUICONTROL_FEATURE(pobj)->fontWeight = NORMAL_FONT;
  pUICONTROL_FEATURE(pobj)->fontUnits = POINTS_UNITS;
  pUICONTROL_FEATURE(pobj)->fontAngle = NORMAL_FONT;

  /* Units default */
  pUICONTROL_FEATURE(pobj)->units = POINTS_UNITS;

  sciStandardBuildOperations(pobj, NULL);
      
  /* Default Style is PushButton */
  if (style == NULL || strcmp(style,"pushbutton")==0)
    {
      createPushButton((sciPointObj *) pobj);
      pUICONTROL_FEATURE (pobj)->style = SCI_PUSHBUTTON;
    }
  else
    {
      sciprint(_("Could not create uicontrol of type %s."), style);
    }
      
  return (sciPointObj *) pobj;
}
/*----------------------------------------------------------------------------*/
