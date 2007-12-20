/*------------------------------------------------------------------------*/
/* file: sci_uicontrol.c                                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Vincent COUVERT                                              */
/*------------------------------------------------------------------------*/
#include "gw_gui.h"
/*--------------------------------------------------------------------------*/
#include "CreateUIControl.h"
#include "MALLOC.h" /* MALLOC */
#include "localization.h"
#include "stack-c.h"
#include "sciprint.h"
#include "SetPropertyStatus.h"
#include "SetHashTable.h"
#include "PushButton.h" /* setCurentFigureAsPushButtonParent */
/*--------------------------------------------------------------------------*/
#define NBPROPERTIES 10 
/*--------------------------------------------------------------------------*/
int sci_uicontrol(char *fname, unsigned long fname_len)
{
  int nbRow = 0, nbCol = 0, stkAdr = 0, k = 0;

  int setStatus = SET_PROPERTY_SUCCEED;

  int NOT_FOUND = -1;

  int inputIndex = 0, beginIndex = 0;

  char *propertyName=NULL;
  char *styleProperty=NULL;

  sciPointObj *pParent=NULL;

  unsigned long GraphicHandle = 0;

  /* @TODO remove this crappy initialization */
  /* DO NOT CHANGE ORDER !! */
  char propertiesNames[NBPROPERTIES][16] = {"style", "parent", "backgroundcolor", "foregroundcolor","label", "position", "fontweight", "min", "max", "tag", "units"};
  int *propertiesValuesIndices = NULL;

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

      /* Create a new pushbutton */
      GraphicHandle=sciGetHandle(CreateUIControl(NULL));

      if (VarType(1) != sci_handles)
        {
          Scierror(999,_("First input argument must be a graphic handle."));
          return FALSE;
        }
      else /* Get parent ID */
        {
          GetRhsVar(1,GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);
          
          if (nbRow*nbCol != 1)
            {
              Scierror(999,_("First input argument must be a single handle."));
              return FALSE;
            }
          pParent=sciGetPointerFromHandle((long)*hstk(stkAdr));
          if ( (sciGetEntityType (pParent) != SCI_FIGURE) && (sciGetEntityType (pParent) != SCI_UIMENU) )
            {
              Scierror(999,_("Parent must be a figure or a uimenu."));
              return FALSE;
            }
          /* First parameter is the parent */
          setStatus = callSetProperty((sciPointObj*) GraphicHandle, stkAdr, sci_handles, nbRow, nbCol, (char*)propertiesNames[1]);
          if (setStatus == SET_PROPERTY_ERROR)
            {
              Scierror(999, _("Could not set property %s.\n"), propertyName);
              return FALSE;
            }
        }
    }
  else
    {
      /* Allocate memory to store the position of properties in uicontrol call */
      if((propertiesValuesIndices = (int*)MALLOC(sizeof(int)*NBPROPERTIES))==NULL)
        {
          Scierror(999,_("No more memory.\n"));
          return FALSE_;
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
              Scierror(999,_("First input argument must be a graphic handle."));
              return FALSE;
            }
          else /* Get parent ID */
            {
              GetRhsVar(1,GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stkAdr);

              if (nbRow*nbCol != 1)
                {
                  Scierror(999,_("First input argument must be a single handle."));
                  return FALSE;
                }
              pParent=sciGetPointerFromHandle((long)*hstk(stkAdr));
              if ( (sciGetEntityType (pParent) != SCI_FIGURE) && (sciGetEntityType (pParent) != SCI_UIMENU) )
                {
                  Scierror(999,_("Parent must be a figure or a uimenu."));
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
              Scierror(999, _("Property name must be a character string."));
              return FALSE;
            }
          else
            {
              GetRhsVar(inputIndex, STRING_DATATYPE, &nbRow, &nbCol, &stkAdr);
              propertyName = cstk(stkAdr);

              for(k=0; k<NBPROPERTIES; k++)
                {
                  if (strcmp(propertyName, propertiesNames[k]) == 0)
                    {
                      propertiesValuesIndices[k] = inputIndex+1; /* Position of value for property */
                    }
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
              Scierror(999, _("Style property name must be a character string."));
              return FALSE;
            }
        }
      
      /* Create a new uicontrol */
      GraphicHandle=sciGetHandle(CreateUIControl(styleProperty));

      /* Read and set all properties */
      for(inputIndex = 1; inputIndex<NBPROPERTIES; inputIndex++) /* Style has already been set */
        {
           if(propertiesValuesIndices[inputIndex] != NOT_FOUND)  
            {
              /* Read property value */
              switch (VarType(propertiesValuesIndices[inputIndex]))
                {
                case sci_matrix:
                  GetRhsVar(propertiesValuesIndices[inputIndex],MATRIX_OF_DOUBLE_DATATYPE,&nbRow,&nbCol,&stkAdr);
                  setStatus = callSetProperty((sciPointObj*) GraphicHandle, stkAdr, sci_matrix, nbRow, nbCol, (char*)propertiesNames[inputIndex]);
                  break;
                case sci_strings:
                  GetRhsVar(propertiesValuesIndices[inputIndex],STRING_DATATYPE,&nbRow,&nbCol,&stkAdr);
                  setStatus = callSetProperty((sciPointObj*) GraphicHandle, stkAdr, sci_strings, nbRow, nbCol, (char*)propertiesNames[inputIndex]);
                  break;
                case sci_handles:
                  GetRhsVar(propertiesValuesIndices[inputIndex],GRAPHICAL_HANDLE_DATATYPE,&nbRow,&nbCol,&stkAdr);
                  setStatus = callSetProperty((sciPointObj*) GraphicHandle, stkAdr, sci_handles, nbRow, nbCol, (char*)propertiesNames[inputIndex]);
                  break;
                default:
                  setStatus = SET_PROPERTY_ERROR;
                  break;
                }
              if (setStatus == SET_PROPERTY_ERROR)
                {
                  Scierror(999, _("Could not set property %s.\n"), propertyName);
                  return FALSE;
                }
            }
        }

      /* If no parent given then the current figure is the parent */
      if(propertiesValuesIndices[1]==NOT_FOUND)
        {
          /* Set the parent */
          setCurentFigureAsPushButtonParent((sciPointObj*) GraphicHandle);
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
