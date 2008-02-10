/*------------------------------------------------------------------------*/
/* file: DestroyUIControl.c                                               */
/* Copyright INRIA 2008                                                   */
/* Authors : Vincent COUVERT                                              */
/* Destroy Java object matching Scilab uicontrol                          */
/*------------------------------------------------------------------------*/

#include "DestroyUIControl.hxx"

using namespace org_scilab_modules_gui_bridge;

void DestroyUIControl(sciPointObj * sciObj)
{
  if (sciGetEntityType( sciObj ) == SCI_UICONTROL)
    {
      if (pUICONTROL_FEATURE(sciObj)->style == SCI_UIFRAME) /* Frame style uicontrol */
        {
          CallScilabBridge::destroyFrame(getScilabJavaVM(),
                                         pUICONTROL_FEATURE(sciObj)->hashMapIndex);
        }
      else
        {
          CallScilabBridge::destroyWidget(getScilabJavaVM(),
                                         pUICONTROL_FEATURE(sciObj)->hashMapIndex);
        }
    }
  else
    {
      sciprint(_("Could not destroy this object.\n"));
    }
}
