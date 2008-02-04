/*------------------------------------------------------------------------*/
/* file: get_callback_type_property.c                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the callback_type field of       */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_callback_type_property( sciPointObj * pobj )
{
  int type = 0;

  if (sciGetEntityType (pobj) == SCI_UIMENU || sciGetEntityType (pobj) == SCI_UICONTROL)
    {
      if(sciGetEntityType (pobj) == SCI_UIMENU)
        {
          type = pUIMENU_FEATURE(pobj)->callbackType;
        }
      else
        {
          type = pUICONTROL_FEATURE(pobj)->callbackType;
        }
      
      return sciReturnDouble(type);
    }
  else
    {
      sciprint(_("%s property does not exist for this handle.\n"),"callback_type");
      return -1;
    }
  
}
/*------------------------------------------------------------------------*/
