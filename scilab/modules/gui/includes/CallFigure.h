#ifndef __CALLFIGURE_H__
#define __CALLFIGURE_H__

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "BOOL.h"

/**
 * Set the event handler of a figure
 *
 * @param ID the ID of the figure 
 * @param command the name of the event handling function
 */
void setFigureEventHandler(int ID, char * command);

/**
 * Enable or Disable the event handler of a figure
 *
 * @param ID the ID of the figure
 * @param status true to set the event handler enabled
 */
void setFigureEventHandlerEnabled(int ID, BOOL status);

#endif /* !__CALLFIGURE_H__ */

