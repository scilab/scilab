/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Vincent COUVERT
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
