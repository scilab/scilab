/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
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

package org.scilab.modules.console;

import java.awt.Point;

import javax.swing.JPanel;

import com.artenum.rosetta.interfaces.core.InputParsingManager;
import com.artenum.rosetta.interfaces.ui.InputCommandView;
import com.artenum.rosetta.interfaces.ui.PromptView;
import com.artenum.rosetta.util.StringConstants;

import org.scilab.modules.completion.Completion;

/**
 * @author Vincent COUVERT
 *
 */
public class SciInputParsingManager implements InputParsingManager {

    private InputCommandView inputCommandView;
    private PromptView promptView;
    private Point windowCompletionPosition;

    /**
     * Default constructor
     */
    public SciInputParsingManager() {
        windowCompletionPosition = new Point(0, 0);
    }

    /**
     * Append a string to the console input
     * @param content the string to append
     * @see com.artenum.rosetta.interfaces.core.InputParsingManager#append(java.lang.String)
     */
    public void append(String content) {
        inputCommandView.append(content);
    }

    /**
     * Get the position of the caret in the console input
     * @return the position as an int value
     * @see com.artenum.rosetta.interfaces.core.InputParsingManager#getCaretPosition()
     */
    public int getCaretPosition() {
        return inputCommandView.getCaretPosition();
    }

    /**
     * Read the text entered (by the user) in the console
     * @return the text entered
     * @see com.artenum.rosetta.interfaces.core.InputParsingManager#getCommandLine()
     */
    public String getCommandLine() {
        return inputCommandView.getText();
    }

    /**
     * Get the level to use for completion (which keywork we should use for completion ?)
     * @return the level
     * @see com.artenum.rosetta.interfaces.core.InputParsingManager#getCompletionLevel()
     */
    public int getCompletionLevel() {
        // Always zero for first implementation in Scilab
        return 0;
    }

    /**
     * Get the number of lines in the console input
     * @return the number of lines
     * @see com.artenum.rosetta.interfaces.core.InputParsingManager#getNumberOfLines()
     */
    public int getNumberOfLines() {
        int result = inputCommandView.getText().split(StringConstants.NEW_LINE).length;
        if (inputCommandView.getText().lastIndexOf(StringConstants.NEW_LINE) != -1) {
            result++;
        }
        return result;
    }

    /**
     * Get the string to use for completion (see completion level)
     * @param level the level used for completion
     * @return the string to use as a keyword for completion
     * @see com.artenum.rosetta.interfaces.core.InputParsingManager#getPartLevel(int)
     */
    public String getPartLevel(int level) {
        String returnLine = "";
        String wholeLine = getCommandLine();
        int caretPos = getCaretPosition();

        String lineToParse = wholeLine.substring(0, caretPos);
        if (lineToParse.length() > 0) {
            returnLine = Completion.getPartLevel(lineToParse);
        }
        return returnLine;
    }

    /**
     * Get the string to use for completion (see completion level)
     * @param level the level used for completion
     * @return the string to use as a keyword for completion
     * @see com.artenum.rosetta.interfaces.core.InputParsingManager#getPartLevel(int)
     */
    public String getFilePartLevel(int level) {
        String returnLine = null;
        String wholeLine = getCommandLine();
        int caretPos = getCaretPosition();

        String lineToParse = wholeLine.substring(0, caretPos);
        if (lineToParse.length() > 0) {
            returnLine = Completion.getFilePartLevel(lineToParse);
        }
        return returnLine;
    }

    /**
     * Get the position where the completion window has to be displayed
     * @return the position
     * @see com.artenum.rosetta.interfaces.core.InputParsingManager#getWindowCompletionLocation()
     */
    public Point getWindowCompletionLocation() {
        windowCompletionPosition.setLocation(inputCommandView.getCaretLocation());
        windowCompletionPosition.translate(-((JPanel) promptView).getWidth(), 0);
        return windowCompletionPosition;
    }

    /**
     * Is a block currently edited in the console input ?
     * @return true if a block is currently edited (false else)
     * @see com.artenum.rosetta.interfaces.core.InputParsingManager#isBlockEditing()
     */
    public boolean isBlockEditing() {
        // FIXME : No block editing by default
        return false;
    }

    /**
     * Reset the input of the console (delete all entries)
     * @see com.artenum.rosetta.interfaces.core.InputParsingManager#reset()
     */
    public void reset() {
        inputCommandView.reset();
    }

    /**
     * Set the input command view associated to this parsing manager
     * @param inputCommandView the command view object to associate
     * @see com.artenum.rosetta.interfaces.core.InputParsingManager#setInputCommandView(com.artenum.rosetta.interfaces.ui.InputCommandView)
     */
    public void setInputCommandView(InputCommandView inputCommandView) {
        this.inputCommandView = inputCommandView;
    }

    /**
     * Append the text selected in the completioon window to the console input
     * @param completionResult the string to append
     * @see com.artenum.rosetta.interfaces.core.InputParsingManager#writeCompletionPart(java.lang.String)
     */
    public void writeCompletionPart(String completionResult) {
        // Add to the input view the end of the selected item
        inputCommandView.append(completionResult.substring(getPartLevel(getCompletionLevel()).length()));
    }

    /**
     * Delete the character just before the caret position
     * @see com.artenum.rosetta.interfaces.core.InputParsingManager#backspace()
     */
    public void backspace() {
        inputCommandView.backspace();
    }

    /**
     * Get the prompt view associated to the parsing manager
     * @return the promptView to associate
     */
    public PromptView getPromptView() {
        return promptView;
    }

    /**
     * Set the prompt view associated to the parsing manager
     * @param promptView the promptView to set
     */
    public void setPromptView(PromptView promptView) {
        this.promptView = promptView;
    }

}
