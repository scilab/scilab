#ifndef __CALLWAITBAR_H__
#define __CALLWAITBAR_H__

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
 * Create a new WaitBar
 *
 * @param void  
 * @return the ID of the created WaitBar
 */
int createWaitBar(void);

/**
 * Set the current value of the WaitBar
 *
 * @param ID the id of the WaitBar
 * @param value the value to set
 */
void setWaitBarValue(int ID, int value);

/**
 * Set the message of the WaitBar
 *
 * @param ID the id of the WaitBar
 * @param message the message to set
 * @param nbLines the number of entries in the message
 */
void setWaitBarMessage(int ID, char **message, int nbLines);

/**
 * Indicates if the total execution time is known
 *
 * @param ID the id of the WaitBar
 * @param status true if the total progress time in unknown
 */
void setWaitBarIndeterminateMode(int ID, BOOL status);

#endif /* !__CALLWAITBAR_H__ */

