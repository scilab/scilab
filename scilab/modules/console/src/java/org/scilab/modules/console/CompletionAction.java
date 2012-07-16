/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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

        Point location = configuration.getInputParsingManager().getWindowCompletionLocation();
        List<CompletionItem> completionItems = configuration.getCompletionManager().getCompletionItems();
        AbstractSciCompletionWindow win = (AbstractSciCompletionWindow) configuration.getCompletionWindow();

        if (completionItems != null && completionItems.size() == 1) {
            /* Only one item returned, autoselected and appended to command line */
            configuration.getCompletionWindow().show(completionItems, location);
            win.addCompletedWord(caretPosition);

            ((AbstractSciCompletionWindow) configuration.getCompletionWindow()).setVisible(false);
        } else if (completionItems != null && completionItems.size() != 0) {
            String [] completionArray = new String [completionItems.size()];

            int i = 0;
            Iterator < CompletionItem >  it = completionItems.iterator();
            while  (it.hasNext()) {
                CompletionItem currentItem = it.next();
                completionArray[i] = currentItem.getReturnValue();
                i++;
            }

            java.util.Arrays.sort(completionArray);
            String commonPartOfWord = Completion.getCommonPart(completionArray, completionItems.size());

            caretPosition = configuration.getInputParsingManager().getCaretPosition();
            String currentLine = configuration.getInputParsingManager().getCommandLine();

            if ((commonPartOfWord.length() != 0) && (caretPosition == currentLine.length())) {
                if (configuration.getInputParsingManager().getPartLevel(0).length() != 0) {
                    win.addCompletedWord(commonPartOfWord, caretPosition);
                }
            }
            configuration.getCompletionWindow().show(completionItems, location);
        }
    }
}
