/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _SCI_UICONTROL_H_
#define _SCI_UICONTROL_H_

/**
* interface function for the uicontrol routine.
*
* @param[in] fname     name of the routine (ie uicontrol)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_uicontrol(char * fname, unsigned long fname_len) ;

#endif /* _SCI_UICONTROL_H_ */
