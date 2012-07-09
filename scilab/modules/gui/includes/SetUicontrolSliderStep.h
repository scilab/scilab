/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SET_UICONTROL_SLIDERSTEP_H__
#define __SET_UICONTROL_SLIDERSTEP_H__

#include <stddef.h>

#include "dynlib_gui.h"

/**
 * Set the slider step property of a uicontrol (slider)
 *
 * @param pobjUID Scilab object corresponding to the uicontrol
 * @param stackPointer position on the stack of the slider step to set
 * @param valueType type of the value on stack
 * @param nbRow number of rows of the value on stack
 * @param nbCol number of columns of the value on stack
 * @return true if the slider step property set has been correcty done
 */
GUI_IMPEXP int SetUicontrolSliderStep(void* _pvCtx, char* pobjUID, size_t stackPointer, int valueType, int nbRow, int nbCol);

#endif /* __SET_UICONTROL_SLIDERSTEP_H__ */
