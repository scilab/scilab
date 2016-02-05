/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2010 - Calixte DENIZET
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
import java.awt.event.ActionEvent;
import java.util.Iterator;
import java.util.List;

import com.artenum.rosetta.core.action.AbstractConsoleAction;
import com.artenum.rosetta.interfaces.core.CompletionItem;

import org.scilab.modules.completion.AbstractSciCompletionWindow;
import org.scilab.modules.completion.Completion;

/**
 * Class used when Scilab user asks for completion on the current edited line
 * @author Vincent COUVERT
 * @author Allan CORNET
 */
public class CompletionAction extends AbstractConsoleAction {
    private static final long serialVersionUID = 1L;

    /**
     * Constructor
     */
    public CompletionAction() {
        super();
    }

    /**
     * Threats the event
     * @param e the action event that occurred
     * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
     */
    public void actionPerformed(ActionEvent e) {
        int caretPosition = configuration.getInputParsingManager().getCaretPosition();
        if (caretPosition == 0) {
            return;
        }

        List<CompletionItem> completionItems = configuration.getCompletionManager().getCompletionItems();

        if ((completionItems != null) && (completionItems.size() > 0)) {
            AbstractSciCompletionWindow win = (AbstractSciCompletionWindow) configuration.getCompletionWindow();

            if (completionItems.size() == 1) {
                Point location = configuration.getInputParsingManager().getWindowCompletionLocation();
                /* Only one item returned, autoselected and appended to command line */
                configuration.getCompletionWindow().show(completionItems, location);
                win.addCompletedWord(caretPosition);

                ((AbstractSciCompletionWindow) configuration.getCompletionWindow()).setVisible(false);
            } else {
                String [] completionArray = new String [completionItems.size()];

                int i = 0;
                Iterator < CompletionItem >  it = completionItems.iterator();
                while  (it.hasNext()) {
                    CompletionItem currentItem = it.next();
                    completionArray[i] = currentItem.getReturnValue();
                    i++;
                }

                // First autocmplete by the common prefix of suggested words, if exist
                java.util.Arrays.sort(completionArray);
                String commonPartOfWord = Completion.getCommonPart(completionArray, completionItems.size());

                caretPosition = configuration.getInputParsingManager().getCaretPosition();
                String currentLine = configuration.getInputParsingManager().getCommandLine();

                if ((commonPartOfWord.length() != 0) && (caretPosition == currentLine.length())) {
                    if (configuration.getInputParsingManager().getPartLevel(0).length() != 0) {
                        win.addCompletedWord(commonPartOfWord, caretPosition);
                    }
                }

                // Display suggested word window at caret position
                Point location = configuration.getInputParsingManager().getWindowCompletionLocation();
                configuration.getCompletionWindow().show(completionItems, location);
            }
        }
    }
}
