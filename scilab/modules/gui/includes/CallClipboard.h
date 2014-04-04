#ifndef __CALLCLIPBOARD_H__
#define __CALLCLIPBOARD_H__

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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

