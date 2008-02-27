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

#ifndef __SCI_UICONTEXTMENU_H__
#define __SCI_UICONTEXTMENU_H__

/**
 * Interface function for the uicontextmenu routine.
 *
 * @param[in] fname     name of the routine (ie uicontextmenu)
 * @param[in] fname_len length of fname
 * @return 0 if success, -1 otherwise
 */
int sci_uicontextmenu(char * fname, unsigned long fname_len) ;

#endif /* !__SCI_UICONTEXTMENU_H__ */
