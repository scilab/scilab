/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT (java version)
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
