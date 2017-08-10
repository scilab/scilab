/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
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

package org.scilab.modules.gui.console;

import java.awt.Color;
import java.awt.Cursor;
import java.awt.Font;

import org.scilab.modules.gui.dockable.Dockable;

/**
 * Interface for Scilab Console in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public interface Console extends Dockable {

    /**
     * Gets this Bridge component object
     * @return this Bridge component object
     */
    SimpleConsole getAsSimpleConsole();

    /**
     * Reads input data in console
     * @return the string entered by user
     */
    String readLine();

    /**
     * Displays data in Console
     * @param dataToDisplay the data to be displayed
     */
    void display(String dataToDisplay);

    /**
     * Clears the console (all output view is cleared)
     */
    void clear();

    /**
     * Clears lines from the end of the output view
     * @param nbLines the number of lines to be deleted
     */
    void clear(int nbLines);

    /**
     * Puts the prompt in the top left corner of the console
     */
    void toHome();

    /**
     * Sets the prompt displayed in the console
     * @param prompt the prompt to be displayed in the console
     */
    void setPrompt(String prompt);

    /**
     * Updates Scilab internal variables containing the size of the console
     * These variables are used to format data before displaying it
     */
    void scilabLinesUpdate();

    /**
     * Get the current status of the console
     * If the prompt view is visible, Scilab is waiting for commands
     * @return true is Scilab is waiting for commands
     */
    boolean isWaitingForInput();

    /**
     * Clear the commands history
     */
    void clearHistory();

    /**
     * Select all the console contents
     */
    void selectAll();

    /**
     * Put the console selected text in the clipboard
     */
    void copyToClipboard();

    /**
     * Paste clipboard contents in Console input line
     */
    void pasteClipboard();

    /**
     * Cut selected text in the Console input line
     */
    void cutSelection();

    /**
     * Set the font of the Console
     * @param font the font to set
     */
    void setFont(Font font);

    /**
     * Get the font of the Console
     * @return the font
     */
    Font getFont();

    /**
     * Get the Foreground Color of the Console
     * @return the Foreground Color
     */
    Color getForeground();

    /**
     * Get the Background Color of the Console
     * @return the Background Color
     */
    Color getBackground();

    /**
     * Set the Foreground Color of the Console
     * @param color the Foreground Color
     */
    void setForeground(Color color);

    /**
     * Set the Background Color of the Console
     * @param color the Background Color
     */
    void setBackground(Color color);

    /**
     * Set the maximum number of lines stored in the Output
     * @param nbLines the number of lines
     */
    void setMaxOutputSize(int nbLines);

    /**
     * Set the cursor pointer over the Console
     * @param cursor the Cursor to set
     */
    void setCursor(Cursor cursor);

    void helpOnTheKeyword();

    /**
     * Evaluate the selection with echo
     */
    void evaluateSelectionWithEcho();

    /**
     * Evaluate the selection with no echo
     */
    void evaluateSelectionWithNoEcho();
}
