/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent Couvert
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

import org.scilab.modules.gui.bridge.console.SwingScilabConsole;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Bridge for Scilab Console in GUIs
 * @author Vincent COUVERT
 */
public class ScilabConsoleBridge {

    /**
     * Constructor
     */
    protected ScilabConsoleBridge() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
    };

    /**
     * Creates a Scilab Console
     * @return the created console
     */
    public static SimpleConsole createConsole() {
        return new SwingScilabConsole();
    }

    /**
     * Displays data in the console
     * @param console the console to display the data in
     * @param dataToDisplay the data to be displayed
     * @see fr.scilab.console.HelpBrowser#display()
     */
    public static void display(Console console, String dataToDisplay) {
        console.getAsSimpleConsole().display(dataToDisplay);
    }

    /**
     * Reads input data in the console
     * @param console the console to read the data from
     * @return the data entered by the user
     * @see fr.scilab.console.HelpBrowser#readLine()
     */
    public static String readLine(Console console) {
        return console.getAsSimpleConsole().readLine();
    }

    /**
     * Draw a console
     * @param console the console to draw
     */
    public static void draw(Console console) {
        console.getAsSimpleConsole().draw();
    }

    /**
     * Gets the dimensions (width and height) of a Scilab console
     * @param console the console which we want to get the dimensions of
     * @return the size of the console
     */
    public static Size getDims(Console console) {
        return console.getAsSimpleConsole().getDims();
    }

    /**
     * Gets the position (X-coordinate and Y-coordinates) of a Scilab console
     * @param console the console which we want to get the position of
     * @return the position of the console
     */
    public static Position getPosition(Console console) {
        return console.getAsSimpleConsole().getPosition();
    }

    /**
     * Gets the visibility status of a console
     * @param console the console which we want to get the visibility status of
     * @return the visibility status of the console (true if the console is visible, false if not)
     */
    public static boolean isVisible(Console console) {
        return console.getAsSimpleConsole().isVisible();
    }

    /**
     * Sets the dimensions (width and height) of a Scilab console
     * @param console the console which we want to set the dimensions of
     * @param newSize the size we want to set to the console
     */
    public static void setDims(Console console, Size newSize) {
        console.getAsSimpleConsole().setDims(newSize);
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a Scilab console
     * @param console the console which we want to set the position of
     * @param newPosition the position we want to set to the console
     */
    public static void setPosition(Console console, Position newPosition) {
        console.getAsSimpleConsole().setPosition(newPosition);
    }

    /**
     * Sets the visibility status of a Scilab console
     * @param console the console which we want to set the visibility status of
     * @param newVisibleState the visibility status we want to set to the console
     */
    public static void setVisible(Console console, boolean newVisibleState) {
        console.getAsSimpleConsole().setVisible(newVisibleState);
    }

    /**
     * Clears the console (no more scroll history and prompt on top left corner)
     * @param console the console to be cleared
     */
    public static void clear(Console console) {
        console.getAsSimpleConsole().clear();
    }

    /**
     * Clears lines from the end of the output view
     * @param console the console to be cleared
     * @param nbLines the number of lines to be deleted
     */
    public static void clear(Console console, int nbLines) {
        console.getAsSimpleConsole().clear(nbLines);
    }

    /**
     * Puts the prompt in the top left corner of the console
     * @param console the console used
     */
    public static void toHome(Console console) {
        console.getAsSimpleConsole().toHome();
    }


    /**
     * Reads one user input char
     * @param console the console used to get the char
     * @return the data entered by the user
     * @see fr.scilab.console.HelpBrowser#getCharWithoutOutput()
     */
    public static int getCharWithoutOutput(Console console) {
        return console.getAsSimpleConsole().getCharWithoutOutput();
    }

    /**
     * Sets the prompt displayed in the console
     * @param console the console where the prompt is set
     * @param prompt the prompt to be displayed in the console
     */
    public static void setPrompt(Console console, String prompt) {
        console.getAsSimpleConsole().setPrompt(prompt);
    }

    /**
     * Updates Scilab internal variables containing the size of the console
     * These variables are used to format data before displaying it
     * @param console object we want to get the size of
     */
    public static void scilabLinesUpdate(Console console) {
        console.getAsSimpleConsole().scilabLinesUpdate();
    }

    /**
     * Get the current status of the console
     * If the prompt view is visible, Scilab is waiting for commands
     * @param console object we want to get the status of
     * @return true is Scilab is waiting for commands
     */
    public static boolean isWaitingForInput(Console console) {
        return console.getAsSimpleConsole().isWaitingForInput();
    }

    /**
     * Clear the commands history
     * @param console the console we want to clear the history of
     */
    public static void clearHistory(Console console) {
        console.getAsSimpleConsole().clearHistory();
    }

    /**
     * Select all the console contents
     * @param console the console
     */
    public static void selectAll(Console console) {
        console.getAsSimpleConsole().selectAll();
    }

    /**
     * Put the console selected text in the clipboard
     * @param console the console
     */
    public static void copyToClipboard(Console console) {
        console.getAsSimpleConsole().copyToClipboard();
    }

    /**
     * Paste clipboard contents in Console input line
     * @param console the console
     */
    public static void pasteClipboard(Console console) {
        console.getAsSimpleConsole().pasteClipboard();
    }

    /**
     * Cut selected text in the Console input line
     * @param console the console
     */
    public static void cutSelection(Console console) {
        console.getAsSimpleConsole().cutSelection();
    }

    /**
     * Set the font of the Console
     * @param console the console
     * @param font the font to set
     */
    public static void setFont(Console console, Font font) {
        console.getAsSimpleConsole().setFont(font);
    }

    /**
     * Set the font of the Console
     * @param console the console
     * @return the Console font
     */
    public static Font getFont(Console console) {
        return console.getAsSimpleConsole().getFont();
    }

    /**
     * Get the Foreground Color of the Console
     * @param console the console
     * @return the Foreground Color
     */
    public static Color getForeground(Console console) {
        return console.getAsSimpleConsole().getForeground();
    }

    /**
     * Get the Background Color of the Console
     * @param console the console
     * @return the Background Color
     */
    public static Color getBackground(Console console) {
        return console.getAsSimpleConsole().getBackground();
    }

    /**
     * Set the Foreground Color of the Console
     * @param console the console
     * @param color the Foreground Color
     */
    public static void setForeground(Console console, Color color) {
        console.getAsSimpleConsole().setForeground(color);
    }

    /**
     * Set the Background Color of the Console
     * @param console the console
     * @param color the Background Color
     */
    public static void setBackground(Console console, Color color) {
        console.getAsSimpleConsole().setBackground(color);
    }

    /**
     * Set the maximum number of lines stored in the Output
     * @param console the console
     * @param nbLines the number of lines
     */
    public static void setMaxOutputSize(Console console, int nbLines) {
        console.getAsSimpleConsole().setMaxOutputSize(nbLines);
    }

    /**
     * Set the cursor pointer over the Console
     * @param console the console
     * @param cursor the Cursor to set
     */
    public static void setCursor(Console console, Cursor cursor) {
        console.getAsSimpleConsole().setCursor(cursor);
    }

    public static void helpOnTheKeyword(ScilabConsole console) {
        console.getAsSimpleConsole().helpOnTheKeyword();
    }

    /**
     * Evaluate the selection with echo
     * @param console the console
     */
    public static void evaluateSelectionWithEcho(ScilabConsole console) {
        console.getAsSimpleConsole().evaluateSelectionWithEcho();
    }

    /**
     * Evaluate the selection with no echo
     * @param console the console
     */
    public static void evaluateSelectionWithNoEcho(ScilabConsole console) {
        console.getAsSimpleConsole().evaluateSelectionWithNoEcho();
    }
}
