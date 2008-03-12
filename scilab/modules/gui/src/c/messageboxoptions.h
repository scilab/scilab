/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT (java version)
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __MESSAGEBOXOPTIONS_H__
#define __MESSAGEBOXOPTIONS_H__

/**
 * Verify the argument string is "modal" or "non-modal"
 *
 * @param option the string to test
 * @return 1 is the string matches "modal" or "non-modal"
 */
int isModalOption(char *option);

/**
 * Verify the argument string is an icon name
 * Scilab icon names are: "scilab" "error" "hourglass" "info" "passwd" "question" "warning"
 *
 * @param option the string to test
 * @return 1 is the string matches an icon name
 */
int isIconName(char *name);

#endif /* ! __MESSAGEBOXOPTIONS_H__ */
