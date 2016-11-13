#ifndef __CALLCLIPBOARD_H__
#define __CALLCLIPBOARD_H__

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

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Get the clipboard contents
 *
 * @param void
 * @return the contents of the clipboard or NULL if the clipboard data are not chars
 */
char *getClipboardContents(void);

/**
 * Paste the clipboard contents into the Console
 */
void pasteClipboardIntoConsole(void);

/**
 * Copy the Console selected text into the clipboard
 */
void copyConsoleSelection(void);

/**
 * Empty the clipboard contents
 */
void emptyClipboard(void);

/**
 * Set the clipboard contents
 *
 * @param text the contents to set
 */
void setClipboardContents(char *text);

/**
 * Set the clipboard contents with Scilab figure contents
 *
 * @param figID the source figure
 */
void copyFigureToClipBoard(int iFigID);

#ifdef __cplusplus
}
#endif

#endif /* !__CALLCLIPBOARD_H__ */

