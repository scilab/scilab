/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Create Java object matching Scilab uicontrol 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "CreateUIControl.h"
#include "BuildObjects.h"
#include "ObjectStructure.h"
#include "PushButton.h"
#include "EditBox.h"
#include "Label.h"
#include "CheckBox.h"
#include "RadioButton.h"
#include "Slider.h"
#include "PopupMenu.h"
#include "ListBox.h"
#include "Frame.h"
#include "DestroyObjects.h" /* sciStandardDestroyOperations */

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

  /* Color property */
  /* The getter for Colors returns get the value from Java if ****groundcolor is NULL */
  pUICONTROL_FEATURE (pobj)->backgroundcolor = NULL;
  pUICONTROL_FEATURE (pobj)->foregroundcolor = NULL;

  /* Callback */
  /* The getter for Callback returns "" if Callback is NULL */
  pUICONTROL_FEATURE (pobj)->callback = NULL;
  pUICONTROL_FEATURE (pobj)->callbackType = -1; /* Disabled */

  pUICONTROL_FEATURE (pobj)->handle_visible=TRUE;

  /* Tag is set to NULL as default */
  /* The getter for tag returns "" is tag is NULL */
  pUICONTROL_FEATURE (pobj)->tag = NULL;

  /* Value is set to NULL as default */
  pUICONTROL_FEATURE (pobj)->value = NULL;
  pUICONTROL_FEATURE (pobj)->valueSize = 0;

  /* ListboxTop is set to NULL as default */
  /* The getter for ListboxTop returns [] if ListboxTop is NULL */
  pUICONTROL_FEATURE (pobj)->listboxTop = NULL;

  /* Min/Max default values */
  pUICONTROL_FEATURE (pobj)->max = 1;
  pUICONTROL_FEATURE (pobj)->min = 0;

  /* Font default */
  pUICONTROL_FEATURE(pobj)->fontWeight = NORMAL_FONT;
  pUICONTROL_FEATURE(pobj)->fontUnits = POINTS_UNITS;
  pUICONTROL_FEATURE(pobj)->fontAngle = NORMAL_FONT;

  /* Units default */
  pUICONTROL_FEATURE(pobj)->units = PIXELS_UNITS;

  /* Relief default */
  if (style == NULL || strcmp(style,"pushbutton")==0)
    {
      pUICONTROL_FEATURE(pobj)->relief = RAISED_RELIEF;
    }
  else if (strcmp(style,"edit")==0)
    {
      pUICONTROL_FEATURE(pobj)->relief = SUNKEN_RELIEF;
    }
  else if (strcmp(style,"frame")==0)
    {
      pUICONTROL_FEATURE(pobj)->relief = RIDGE_RELIEF;
    }
  else
    {
      pUICONTROL_FEATURE(pobj)->relief = FLAT_RELIEF;
    }

  /* Alignment default */
  pUICONTROL_FEATURE(pobj)->horizontalAlignment = CENTER_ALIGNMENT;
  pUICONTROL_FEATURE(pobj)->verticalAlignment = MIDDLE_ALIGNMENT;

  /* SliderStep is set to NULL as default */
  /* The getter for SliderStep returns [0.01*(max-min) 0.1*(max-min)] if SliderStep is NULL */
  /* Slider Step default */
  pUICONTROL_FEATURE(pobj)->sliderStep = NULL;

  sciStandardBuildOperations(pobj, NULL);

  /* Default Style is PushButton */
  if (style == NULL || strcmp(style,"pushbutton")==0)
    {
      createPushButton((sciPointObj *) pobj);
      pUICONTROL_FEATURE (pobj)->style = SCI_PUSHBUTTON;
    }
  else if (strcmp(style,"edit")==0)
    {
      createEditBox((sciPointObj *) pobj);
      pUICONTROL_FEATURE (pobj)->style = SCI_EDIT;
    }
  else if (strcmp(style,"text")==0)
    {
      createLabel((sciPointObj *) pobj);
      pUICONTROL_FEATURE (pobj)->style = SCI_UITEXT;
    }
  else if (strcmp(style,"checkbox")==0)
    {
      createCheckBox((sciPointObj *) pobj);
      pUICONTROL_FEATURE (pobj)->style = SCI_CHECKBOX;
    }
  else if (strcmp(style,"radiobutton")==0)
    {
      createRadioButton((sciPointObj *) pobj);
      pUICONTROL_FEATURE (pobj)->style = SCI_RADIOBUTTON;
    }
  else if (strcmp(style,"slider")==0)
    {
      createSlider((sciPointObj *) pobj);
      pUICONTROL_FEATURE (pobj)->style = SCI_SLIDER;
    }
  else if (strcmp(style,"popupmenu")==0)
    {
      createPopupMenu((sciPointObj *) pobj);
      pUICONTROL_FEATURE (pobj)->style = SCI_POPUPMENU;
    }
  else if (strcmp(style,"listbox")==0)
    {
      createListBox((sciPointObj *) pobj);
      pUICONTROL_FEATURE (pobj)->style = SCI_LISTBOX;
    }
  else if (strcmp(style,"frame")==0)
    {
      createFrame((sciPointObj *) pobj);
      pUICONTROL_FEATURE (pobj)->style = SCI_UIFRAME;
    }
  else
    {
      sciprint(_("%s: Could not create uicontrol of type %s."), "CreateUIControl", style);
      FREE(pobj);
      return (sciPointObj *) NULL;
    }
      
  return (sciPointObj *) pobj;
}
/*----------------------------------------------------------------------------*/

