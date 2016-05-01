#ifndef __CALLHELPBROWSER_H__
#define __CALLHELPBROWSER_H__

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
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

#include "BOOL.h"
#include "dynlib_gui.h"
/**
 * Launch a Scilab Help Browser
 *
 * @param helps help chapters and directories
 * @param helpsSize number of entries in helps
 * @param language Scilab current locale
 */
GUI_IMPEXP int launchHelpBrowser(char **helps, int helpsSize, char *language);

/**
 * Search a keyword in Scilab Help Browser
 *
 * @param helps help chapters and directories
 * @param helpsSize number of entries in helps
 * @param keyword the keyword to search
 * @param language Scilab current locale
 * @param fullText true to execute a full-text search
 */
GUI_IMPEXP int searchKeyword(char **helps, int helpsSize, char *keyword, char *language, BOOL fullText);

#endif /* !__CALLHELPBROWSER_H__ */

