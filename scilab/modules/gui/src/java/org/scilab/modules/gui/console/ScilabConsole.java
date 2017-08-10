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
import java.io.PrintStream;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.dockable.ScilabDockable;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.utils.ScilabPrintStream;

/**
 * Class for Scilab Console in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class ScilabConsole extends ScilabDockable implements Console {

    private static Console instance;

    private SimpleConsole component;

    /**
     * Constructor
     */
    protected ScilabConsole() {
        component = ScilabBridge.createConsole();
    }

    /**
     * Creates a Scilab Console
     * @return the created console
     */
    public static Console createConsole() {
        if (instance == null) {
            instance = new ScilabConsole();
            if (ScilabPrintStream.isAvailable()) {
                /* Get usual stderr */
                PrintStream err = System.err;

                /* Now stderr is the console */
                System.setErr(ScilabPrintStream.getInstance());

                /* Bug 8748: Xcos needs to have the usual stderr too */
                ScilabPrintStream.setRedirect(err);
            }
        }
        return instance;
    }

    /**
     * get a Scilab Console
     * @return the console
     */
    public static Console getConsole() {
        if (instance == null) {
            return new ScilabConsole();
        }
        return instance;
    }

    /**
     * @return true if the console is existing
     */
    public static boolean isExistingConsole() {
        if (instance == null) {
            return false;
        } else {
            return true;
        }
    }

    /**
     * Gets this Bridge component object
     * @return this Bridge component object
     */
    public SimpleConsole getAsSimpleConsole() {
        return component;
    }

    /**
     * Displays data in the console
     * @param dataToDisplay the data to be displayed
     * @see fr.scilab.console.HelpBrowser#display()
     */
    public void display(String dataToDisplay) {
        ScilabBridge.display(this, dataToDisplay);
    }

    /**
     * Reads input data in the console
     * @return the data entered by the user
     */
    public String readLine() {
        return ScilabBridge.readLine(this);
    }

    /**
     * Draw a console
     * @see org.scilab.modules.gui.UIElement#draw()
     */
    public void draw() {
        ScilabBridge.draw(this);
    }

    /**
     * Gets the dimensions (width and height) of a Scilab console
     * @return the size of the console
     * @see org.scilab.modules.gui.UIElement#getDims()
     */
    public Size getDims() {
        return ScilabBridge.getDims(this);
    }

    /**
     * Gets the position (X-coordinate and Y-coordinates) of a Scilab console
     * @return the position of the console
     * @see org.scilab.modules.gui.UIElement#getPosition()
     */
    public Position getPosition() {
        return ScilabBridge.getPosition(this);
    }

    /**
     * Gets the visibility status of a console
     * @return the visibility status of the console (true if the console is visible, false if not)
     * @see org.scilab.modules.gui.UIElement#isVisible()
     */
    public boolean isVisible() {
        return ScilabBridge.isVisible(this);
    }

    /**
     * Sets the dimensions (width and height) of a Scilab console
     * @param newSize the size we want to set to the console
     * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
     */
    public void setDims(Size newSize) {
        ScilabBridge.setDims(this, newSize);
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a Scilab console
     * @param newPosition the position we want to set to the console
     * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
     */
    public void setPosition(Position newPosition) {
        ScilabBridge.setPosition(this, newPosition);
    }

    /**
     * Sets the visibility status of a Scilab console
     * @param newVisibleState the visibility status we want to set to the console
     * @see org.scilab.modules.gui.UIElement#setVisible(boolean)
     */
    public void setVisible(boolean newVisibleState) {
        ScilabBridge.setVisible(this, newVisibleState);
    }

    /**
     * Clears the console (no more scroll history and prompt on top left corner)
     */
    public void clear() {
        ScilabBridge.clear(this);
    }

    /**
     * Clears lines from the end of the output view
     * @param nbLines the number of lines to be deleted
     */
    public void clear(int nbLines) {
        ScilabBridge.clear(this, nbLines);
    }

    /**
     * Puts the prompt in the top left corner of the console
     */
    public void toHome() {
        ScilabBridge.toHome(this);
    }

    /**
     * Reads one user input char
     * @return the data entered by the user
     * @see fr.scilab.console.HelpBrowser#getCharWithoutOutput()
     */
    public int getCharWithoutOutput() {
        return ScilabBridge.getCharWithoutOutput(this);
    }

    /**
     * Sets the prompt displayed in the console
     * @param prompt the prompt to be displayed in the console
     */
    public void setPrompt(String prompt) {
        ScilabBridge.setPrompt(this, prompt);
    }

    /**
     * Updates Scilab internal variables containing the size of the console
     * These variables are used to format data before displaying it
     */
    public void scilabLinesUpdate() {
        ScilabBridge.scilabLinesUpdate(this);
    }

    /**
     * Get the current status of the console
     * If the prompt view is visible, Scilab is waiting for commands
     * @return true is Scilab is waiting for commands
     */
    public boolean isWaitingForInput() {
        return ScilabBridge.isWaitingForInput(this);
    }

    /**
     * Clear the commands history
     */
    public void clearHistory() {
        ScilabBridge.clearHistory(this);
    }

    /**
     * Select all the console contents
     */
    public void selectAll() {
        ScilabBridge.selectAll(this);

    }

    /**
     * Put the console selected text in the clipboard
     */
    public void copyToClipboard() {
        ScilabBridge.copyToClipboard(this);
    }

    /**
     * Paste clipboard contents in Console input line
     */
    public void pasteClipboard() {
        ScilabBridge.pasteClipboard(this);
    }

    /**
     * Cut selected text in the Console input line
     */
    public void cutSelection() {
        ScilabBridge.cutSelection(this);
    }

    /**
     * Set the font of the Console
     * @param font the font to set
     */
    public void setFont(Font font) {
        ScilabBridge.setFont(this, font);
    }

    /**
     * Get the font of the Console
     * @return the font
     */
    public Font getFont() {
        return ScilabBridge.getFont(this);
    }

    /**
     * Get the Foreground Color of the Console
     * @return the Foreground Color
     */
    public Color getForeground() {
        return ScilabBridge.getForeground(this);
    }

    /**
     * Get the Background Color of the Console
     * @return the Background Color
     */
    public Color getBackground() {
        return ScilabBridge.getBackground(this);
    }

    /**
     * Set the Foreground Color of the Console
     * @param color the Foreground Color
     */
    public void setForeground(Color color) {
        ScilabBridge.setForeground(this, color);
    }

    /**
     * Set the Background Color of the Console
     * @param color the Background Color
     */
    public void setBackground(Color color) {
        ScilabBridge.setBackground(this, color);
    }

    /**
     * Set the maximum number of lines stored in the Output
     * @param nbLines the number of lines
     */
    public void setMaxOutputSize(int nbLines) {
        ScilabBridge.setMaxOutputSize(this, nbLines);
    }

    /**
     * Set the cursor pointer over the Console
     * @param cursor the Cursor to set
     */
    public void setCursor(Cursor cursor) {
        ScilabBridge.setCursor(this, cursor);
    }

    public void helpOnTheKeyword() {
        ScilabBridge.helpOnTheKeyword(this);
    }

    /**
     * Evaluate the selection with echo
     */
    public void evaluateSelectionWithEcho() {
        ScilabBridge.evaluateSelectionWithEcho(this);
    }

    /**
     * Evaluate the selection with no echo
     */
    public void evaluateSelectionWithNoEcho() {
        ScilabBridge.evaluateSelectionWithNoEcho(this);
    }
}
