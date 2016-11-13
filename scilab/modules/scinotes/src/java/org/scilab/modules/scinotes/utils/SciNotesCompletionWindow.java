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

package org.scilab.modules.scinotes.utils;

import java.awt.Component;
import java.awt.Point;
import java.util.List;

import javax.swing.JComponent;
import javax.swing.text.JTextComponent;
import javax.swing.text.BadLocationException;

import com.artenum.rosetta.interfaces.core.CompletionItem;
import com.artenum.rosetta.interfaces.core.CompletionManager;

import org.scilab.modules.completion.AbstractSciCompletionWindow;
import org.scilab.modules.completion.Completion;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabEditorPane;

/**
 * Scilab completion window main class
 * @author Vincent COUVERT
 * @author Calixte DENIZET
 */
public class SciNotesCompletionWindow extends AbstractSciCompletionWindow {

    private SciNotes editor;
    private CompletionManager completionManager;

    /**
     * Default constructor
     * @param editor the editor
     * @param completionManager the CompletionManager
     */
    public SciNotesCompletionWindow(SciNotes editor, CompletionManager completionManager) {
        this.editor = editor;
        this.completionManager = completionManager;
    }

    /**
     * @return the JTextComponent where the CompletionWindow will be drawn
     */
    public JTextComponent getTextComponent() {
        return (JTextComponent) editor.getTextPane();
    }

    /**
     * @return the list of the possibilities to complete
     */
    public List<CompletionItem> getCompletionItems() {
        return completionManager.getCompletionItems();
    }

    /**
     * Set the parent component for the completion window
     * @param component the parent component
     * Caution, the component shouldn't be null otherwise the completion window
     * will never get the focus
     */
    public void setGraphicalContext(Component component) {
        setFocusOut((JComponent) component);
        super.setGraphicalContext(component);
    }

    /**
     * Make the completion window visible and update its content
     * @param list list of items matching completion
     * @param location position of the top left corner of the window
     * @see com.artenum.rosetta.interfaces.ui.CompletionWindow#show(java.util.List, java.awt.Point)
     */
    public void show(List<CompletionItem> list, Point location) {
        currentCaretPosition = inputParsingManager.getCaretPosition();
        /* Display only in completion items list is not empty */
        if (list != null) {
            /* If completion window size bigger than input command view size
               put the window on the top of the current line */
            ScilabEditorPane first = editor.getTextPane();
            int height;
            int value;
            value = first.getScrollPane().getVerticalScrollBar().getValue();
            height = first.getScrollPane().getHeight();
            if (first.getOtherPaneInSplit() != null) {
                getTextComponent().add(window);
            }
            if (window.getHeight() + location.y > value + height) {
                try {
                    int ypos = first.modelToView(currentCaretPosition).height;
                    location.y = location.y - window.getHeight() - ypos;
                } catch (BadLocationException e) { }
            }

            model.updateData(list);

            /* Display completion window */
            window.setLocation(location);
            window.setVisible(true);

            scrollPane.getViewport().setViewPosition(new Point(0, 0));

            if (model.getSize() > 0) {
                listUI.setSelectedIndex(0);
            }
            listUI.grabFocus();
        }
    }

    /**
     * Set the visibility status of the completion window
     * @param status true if the window is visible (false else)
     */
    public void setVisible(boolean status) {
        window.setVisible(status);
        if (!status) {
            editor.getTextPane().grabFocus();
        }
    }

    /**
     * Add the completed word in the TextComponent
     * @param position where to append the completed word
     */
    public void addCompletedWord(int position) {
        addCompletedWord(((CompletionItem) listUI.getSelectedValue()).getReturnValue(), position);
    }

    /**
     * Add the completed word in the TextComponent
     * @param str the string to add
     * @param position where to append the completed word
     */
    public void addCompletedWord(String str, int position) {
        String currentLine = inputParsingManager.getCommandLine();

        String newLine = currentLine.substring(0, currentLine.length() - Completion.getPartLevel(currentLine).length());
        newLine += str;
        inputParsingManager.reset();
        inputParsingManager.append(newLine);
    }
}
