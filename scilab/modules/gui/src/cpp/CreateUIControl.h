/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __CREATE_UICONTROL_H__
#define __CREATE_UICONTROL_H__

/**CreateUIControl
 * This function creates Uicontrol structure.
 * @param char style[]: the type of the uicontrol object
 * @return  : object UID if ok , NULL if not
 */
int CreateUIControl(char *style);

#endif /* !__CREATE_UICONTROL_H__ */
