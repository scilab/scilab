/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "gw_gui.h"
/*--------------------------------------------------------------------------*/
#include "CreateUIControl.h"
#include "MALLOC.h" /* MALLOC */
#include "localization.h"
#include "stricmp.h"
#include "stack-c.h"
#include "sciprint.h"
#include "ObjectStructure.h"
#include "SetPropertyStatus.h"
#include "SetHashTable.h"
#include "PushButton.h" /* setCurentFigureAsPushButtonParent */
#include "EditBox.h" /* setCurentFigureAsEditBoxParent */
#include "Label.h" /* setCurentFigureAsLabelParent */
#include "CheckBox.h" /* setCurentFigureAsCheckBoxParent */
#include "RadioButton.h" /* setCurentFigureAsRadioButtonParent */
#include "Slider.h" /* setCurentFigureAsSliderParent */
#include "PopupMenu.h" /* setCurentFigureAsPopupMenuParent */
#include "ListBox.h" /* setCurentFigureAsListBoxParent */
#include "Frame.h" /* setCurentFigureAsFrameParent */
#include "Scierror.h"
#include "WindowList.h" /* getFigureFromIndex */
#include "Widget.h" /* requestWidgetFocus */
#include "SetUicontrolPosition.h"
/*--------------------------------------------------------------------------*/
#define NBPROPERTIES 26
#define MAXPROPERTYNAMELENGTH 20
/*--------------------------------------------------------------------------*/
int sci_uicontrol(char *fname, unsigned long fname_len)
{
  int nbRow = 0, nbCol = 0, stkAdr = 0, k = 0;
  char **stkAdrForStrings = NULL;
  
  int setStatus = SET_PROPERTY_SUCCEED;

  int NOT_FOUND = -1;

  int inputIndex = 0, beginIndex = 0;

  char *propertyName=NULL;
  char *styleProperty=NULL;

  sciPointObj *pParent=NULL;
  sciPointObj *pUicontrol=NULL;

  unsigned long GraphicHandle = 0;

  int found = 0; /* Does the property exists ? */

  /* @TODO remove this crappy initialization */
  /* DO NOT CHANGE ORDER !! */
  char propertiesNames[NBPROPERTIES][MAXPROPERTYNAMELENGTH] = {"style", "parent", "backgroundcolor", "foregroundcolor","string", "units", "fontweight", "min", "max", "tag", "position", "relief", "horizontalalignment", "verticalalignment", "sliderstep", "fontname", "callback", "fontangle", "fontunits", "fontsize", "listboxtop", "user_data", "value", "userdata", "visible", "enable"};
  int *propertiesValuesIndices = NULL;

  char *propertyPart = NULL;

  //CheckRhs(2,2);
  CheckLhs(0,1);

  if (Rhs==0)
    {
      /* Create a pushbutton in current figure */

      /* Create a new pushbutton */
      GraphicHandle=sciGetHandle(CreateUIControl(NULL));

      /* Set the parent */
      setCurentFigureAsPushButtonParent((sciPointObj*) GraphicHandle);
    }
  else if (Rhs==1)
    {
      /* Create a pushbutton in figure given as parameter */
      /* Or give focus to the uicontrol given as parameter */

      if (VarType(1) != sci_handles)
        {
          Scierror(999,_("%s: Wrong type for input argument #%d: A graphic handle expected.\n"),fname, 1);
          return FALSE;
        }
      else /* Get parent ID */
        {
          GetRhsVar(1,GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);

          if (nbRow*nbCol != 1)
            {
              Scierror(999,_("%s: Wrong size for input argument #%d: A graphic handle expected.\n"),fname, 1);
              return FALSE;
            }
          pParent=sciGetPointerFromHandle((long)*hstk(stkAdr));

          if (sciGetEntityType (pParent) == SCI_UICONTROL) /* Focus management */
            {
              GraphicHandle = (long)*hstk(stkAdr);
              if (pUICONTROL_FEATURE(pParent)->style == SCI_UIFRAME) /* Frame style uicontrol */
                {
                  requestFrameFocus(pParent);
                }
              else
                {
                  requestWidgetFocus(pParent);
                }
            }
          else if ( (sciGetEntityType (pParent) == SCI_FIGURE) || (sciGetEntityType (pParent) == SCI_UIMENU) ) /* PushButton creation */
            {
              /* Create a new pushbutton */
              GraphicHandle=sciGetHandle(CreateUIControl(NULL));
              
              /* First parameter is the parent */
              setStatus = callSetProperty((sciPointObj*) GraphicHandle, stkAdr, sci_handles, nbRow, nbCol, (char*)propertiesNames[1]);
              if (setStatus == SET_PROPERTY_ERROR)
                {
                  Scierror(999, _("%s: Could not set property '%s'.\n"), fname, propertyName);
                  return FALSE;
                }
            }
          else
            {
              Scierror(999,_("%s: Wrong type for input argument #%d: A '%s', '%s' or '%s' handle expected.\n"),fname,1,"Uicontrol","Figure","Uimenu");
              return FALSE;
            }
        }
    }
  else
    {
      /* Allocate memory to store the position of properties in uicontrol call */
      if((propertiesValuesIndices = (int*)MALLOC(sizeof(int)*NBPROPERTIES))==NULL)
        {
          Scierror(999,_("%s: No more memory.\n"), fname);
          return FALSE;
        }

      /* Init all positions to NOT_FOUND */
      for(inputIndex=0; inputIndex<NBPROPERTIES; inputIndex++)
        {
          propertiesValuesIndices[inputIndex] = NOT_FOUND; /* Property initialized as not found */
        }

      /**
       * Odd number of input arguments
       * First input is the parent ID
       * All event inputs are property names
       * All odd (except first) inputs are property values
       */
      if (Rhs%2==1)
        {
          if (VarType(1) != sci_handles)
            {
              if (VarType(1) == sci_matrix)
                {
                  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &stkAdr);
                  if (nbRow*nbCol == 1)
                    {
                      pParent = getFigureFromIndex((int)(*stk(stkAdr)));
                      
                      if ( (sciGetEntityType (pParent) != SCI_FIGURE) && !((sciGetEntityType (pParent) == SCI_UICONTROL) && (pUICONTROL_FEATURE(pParent)->style == SCI_UIFRAME)))
                        {
                          Scierror(999,_("%s: Wrong type for input argument #%d: A '%s' or a '%s' handle expected.\n"),fname,1,"Figure", "Frame uicontrol");
                          return FALSE;
                        }
                      /* First parameter is the parent */
                      propertiesValuesIndices[1] = 1;
                    }
                  else
                    {
                      Scierror(999,_("%s: Wrong size for input argument #%d: A graphic handle expected.\n"),fname, 1);
                      return FALSE;
                    }
                }
              else
                {
                  Scierror(999,_("%s: Wrong type for input argument #%d: A '%s' or a '%s' handle expected.\n"),fname, 1,"Figure", "Frame uicontrol");
                  return FALSE;
                }
            }
          else /* Get parent ID */
            {
              GetRhsVar(1,GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);

              if (nbRow*nbCol != 1)
                {
                  Scierror(999,_("%s: Wrong size for input argument #%d: A '%s' or a '%s' handle expected.\n"),fname, 1, "Figure", "Frame uicontrol");
                  return FALSE;
                }
              pParent=sciGetPointerFromHandle((long)*hstk(stkAdr));
              if ( (sciGetEntityType (pParent) != SCI_FIGURE) && !((sciGetEntityType (pParent) == SCI_UICONTROL) && (pUICONTROL_FEATURE(pParent)->style == SCI_UIFRAME)))
                {
                  Scierror(999,_("%s: Wrong type for input argument #%d: A '%s' or a '%s' handle expected.\n"),fname, 1, "Figure", "Frame uicontrol");
                  return FALSE;
                }
              /* First parameter is the parent */
              propertiesValuesIndices[1] = 1;
            }

          // First input parameter which is a property name
          beginIndex = 2;
        }
      /**
       * Even number of input arguments
       * All odd inputs are property names
       * All even inputs are property values
       */
      else
        {
          // First input parameter which is a property name
          beginIndex = 1;
        }

      /* Get all properties positions */
      for(inputIndex = beginIndex; inputIndex<Rhs; inputIndex = inputIndex+2)
        {
          /* Read property name */
          if (VarType(inputIndex) != sci_strings)
            {
              Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, inputIndex);
              return FALSE;
            }
          else
            {
              GetRhsVar(inputIndex, STRING_DATATYPE, &nbRow, &nbCol, &stkAdr);
              propertyName = cstk(stkAdr);

              /* Bug 3031 */
              /* We only want to compare propertyName along its length */
              /* 'posi' must be matched to 'position' */
              propertyPart = (char*)MALLOC(sizeof(char)*(MAXPROPERTYNAMELENGTH+1));
              if (propertyPart==NULL)
                {
                  Scierror(999, _("%s: No more memory.\n"), fname);
                  return FALSE;
                }

              found = 0;
              for(k=0; k<NBPROPERTIES; k++)
                {
                  if (strlen(propertyName) <= MAXPROPERTYNAMELENGTH)
                    {
                      strncpy(propertyPart, propertiesNames[k], strlen(propertyName));
                      propertyPart[strlen(propertyName)] = '\0';
 
                      if (stricmp(propertyName, propertyPart) == 0)
                        {
                          propertiesValuesIndices[k] = inputIndex+1; /* Position of value for property */
                          found = 1;
                        }
                    }
                }

              FREE(propertyPart);

              if (found == 0)
                {
                  Scierror(999, _("%s: Unknown property: %s for '%s' handles.\n"), fname, propertyName, "Uicontrol");
                  return FALSE;
                }
            }
        }

      if (propertiesValuesIndices[0] != NOT_FOUND) /* Style found */
        {
          if (VarType(propertiesValuesIndices[0]) == sci_strings)
            {
              GetRhsVar(propertiesValuesIndices[0], STRING_DATATYPE, &nbRow, &nbCol, &stkAdr);
              styleProperty = cstk(stkAdr);
            }
          else
            {
              Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, propertiesValuesIndices[0]);
              return FALSE;
            }
        }

      /* Create a new uicontrol */
      pUicontrol = CreateUIControl(styleProperty);
      if (pUicontrol == NULL) /* Error in creation */
        {
          Scierror(999, _("%s: Could not create 'Uicontrol' handle.\n"), fname);
          return FALSE;
        }
      GraphicHandle=sciGetHandle(pUicontrol);

      /* If no parent given then the current figure is the parent */
      if(propertiesValuesIndices[1]==NOT_FOUND)
        {
          /* Set the parent */
           switch(pUICONTROL_FEATURE( ((sciPointObj*) GraphicHandle) )->style)
            {
            case SCI_PUSHBUTTON:
              setCurentFigureAsPushButtonParent((sciPointObj*) GraphicHandle);
              break;
            case SCI_EDIT:
              setCurentFigureAsEditBoxParent((sciPointObj*) GraphicHandle);
              break;
            case SCI_UITEXT:
              setCurentFigureAsLabelParent((sciPointObj*) GraphicHandle);
              break;
            case SCI_CHECKBOX:
              setCurentFigureAsCheckBoxParent((sciPointObj*) GraphicHandle);
              break;
            case SCI_RADIOBUTTON:
              setCurentFigureAsRadioButtonParent((sciPointObj*) GraphicHandle);
              break;
            case SCI_SLIDER:
              setCurentFigureAsSliderParent((sciPointObj*) GraphicHandle);
              break;
            case SCI_POPUPMENU:
              setCurentFigureAsPopupMenuParent((sciPointObj*) GraphicHandle);
              break;
            case SCI_LISTBOX:
              setCurentFigureAsListBoxParent((sciPointObj*) GraphicHandle);
              break;
            case SCI_UIFRAME:
              setCurentFigureAsFrameParent((sciPointObj*) GraphicHandle);
              break;
           default:
              break;
            }
        }

      /* If no position given then set the default position */
      if(propertiesValuesIndices[10]==NOT_FOUND)
        {
          /* See SetUicontrolPosition for the use of -1 as stackPointer */
          SetUicontrolPosition((sciPointObj*) GraphicHandle, -1, 0, 0, 0);
        }

      /* Read and set all properties */
      for(inputIndex = 1; inputIndex<NBPROPERTIES; inputIndex++) /* Style has already been set */
        {
           if(propertiesValuesIndices[inputIndex] != NOT_FOUND)
            {
              if (inputIndex==21 || inputIndex==23) /* User data settings */
                {
                  stkAdr = propertiesValuesIndices[inputIndex]; /* Special management */
                  nbRow = -1; 
                  nbCol = -1;
                  setStatus = callSetProperty((sciPointObj*) GraphicHandle, stkAdr, VarType(propertiesValuesIndices[inputIndex]), nbRow, nbCol, (char*)propertiesNames[inputIndex]);
                }
              else /* All other properties */
                {
                  /* Read property value */
                  switch (VarType(propertiesValuesIndices[inputIndex]))
                    {
                    case sci_matrix:
                      GetRhsVar(propertiesValuesIndices[inputIndex],MATRIX_OF_DOUBLE_DATATYPE,&nbRow,&nbCol,&stkAdr);
                      setStatus = callSetProperty((sciPointObj*) GraphicHandle, stkAdr, sci_matrix, nbRow, nbCol, (char*)propertiesNames[inputIndex]);
                      break;
                    case sci_strings:
                      if (inputIndex == 4) /* Index for String property: Can be mon than one character string */
                        {
                          GetRhsVar(propertiesValuesIndices[inputIndex],MATRIX_OF_STRING_DATATYPE,&nbRow,&nbCol,&stkAdrForStrings);
                          setStatus = callSetProperty((sciPointObj*) GraphicHandle, (size_t)stkAdrForStrings, sci_strings, nbRow, nbCol, (char*)propertiesNames[inputIndex]);
                        }
                      else
                        {
                          GetRhsVar(propertiesValuesIndices[inputIndex],STRING_DATATYPE,&nbRow,&nbCol,&stkAdr);
                          setStatus = callSetProperty((sciPointObj*) GraphicHandle, stkAdr, sci_strings, nbRow, nbCol, (char*)propertiesNames[inputIndex]);
                        }
                      break;
                    case sci_handles:
                      GetRhsVar(propertiesValuesIndices[inputIndex],GRAPHICAL_HANDLE_DATATYPE,&nbRow,&nbCol,&stkAdr);
                      setStatus = callSetProperty((sciPointObj*) GraphicHandle, stkAdr, sci_handles, nbRow, nbCol, (char*)propertiesNames[inputIndex]);
                      break;
                    default:
                      setStatus = SET_PROPERTY_ERROR;
                      break;
                    }
                }
              if (setStatus == SET_PROPERTY_ERROR)
                {
                  Scierror(999, _("%s: Could not set property '%s'.\n"), fname, (char*)propertiesNames[inputIndex]);
                  return FALSE;
                }
            }
        }
    }

  FREE(propertiesValuesIndices);

  /* Create return variable */
  nbRow = 1;
  nbCol = 1;
  CreateVar(Rhs+1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);
  *hstk(stkAdr) = GraphicHandle;

  LhsVar(1)=Rhs+1;

  C2F(putlhsvar)();

  return TRUE;
}
/*--------------------------------------------------------------------------*/
