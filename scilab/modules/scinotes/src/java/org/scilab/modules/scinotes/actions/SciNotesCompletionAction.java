/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

package org.scilab.modules.scinotes.actions;

import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.List;

import javax.swing.JComponent;
import javax.swing.KeyStroke;
import javax.swing.ActionMap;
import javax.swing.InputMap;
import javax.swing.text.Element;
import javax.swing.text.StyledDocument;
import javax.swing.text.BadLocationException;

import com.artenum.rosetta.core.CompletionItemImpl;
import com.artenum.rosetta.interfaces.core.CompletionItem;
import com.artenum.rosetta.interfaces.core.InputParsingManager;
import com.artenum.rosetta.interfaces.core.HistoryManager;
import com.artenum.rosetta.interfaces.core.GenericInterpreter;
import com.artenum.rosetta.interfaces.core.ConsoleConfiguration;
import com.artenum.rosetta.interfaces.core.CompletionManager;
import com.artenum.rosetta.interfaces.ui.CompletionWindow;
import com.artenum.rosetta.interfaces.ui.OutputView;
import com.artenum.rosetta.interfaces.ui.PromptView;
import com.artenum.rosetta.interfaces.ui.InputCommandView;

import org.scilab.modules.completion.Completion;
import org.scilab.modules.console.CompletionAction;
import org.scilab.modules.console.SciCompletionManager;
import org.scilab.modules.console.SciInputParsingManager;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;

import org.scilab.modules.localization.Messages;

import org.scilab.modules.scinotes.KeywordEvent;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.ScilabLexerConstants;
import org.scilab.modules.scinotes.utils.SciNotesCompletionWindow;

/**
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public final class SciNotesCompletionAction extends CompletionAction {

    private SciNotes editor;

    /**
     * Constructor
     * @param textPane the pane associated
     * @param editor SciNotes
     */
    public SciNotesCompletionAction(JComponent textPane, SciNotes editor) {
        this.editor = editor;
        configuration = new SciNotesCompletionConfiguration();
    }

    /**
     * Constructor
     * @param editor SciNotes
     */
    public SciNotesCompletionAction(SciNotes editor) {
        this.editor = editor;
    }

    /**
     * Constructor
     * @param name unused parameter
     * @param editor SciNotes
     */
    public SciNotesCompletionAction(String name, SciNotes editor) {
        this.editor = editor;
    }

    /**
     * {@inheritDoc}
     */
    public void actionPerformed(ActionEvent actionEvent) {
        if (configuration == null) {
            configuration = new SciNotesCompletionConfiguration();
        }
        configuration.getCompletionWindow().setGraphicalContext(editor.getTextPane());
        super.actionPerformed(actionEvent);
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, final SciNotes editor, KeyStroke key) {
        MenuItem menu = ScilabMenuItem.createMenuItem();
        menu.setText(label);
        ((SwingScilabMenuItem) menu.getAsSimpleMenuItem()).addActionListener(new ActionListener() {
            private SciNotesCompletionAction action;
            public void actionPerformed(ActionEvent actionEvent) {
                if (action == null) {
                    action = new SciNotesCompletionAction(editor);
                }
                action.actionPerformed(actionEvent);
            }
        });

        if (key != null) {
            ((SwingScilabMenuItem) menu.getAsSimpleMenuItem()).setAccelerator(key);
        }

        return menu;
    }

    /**
     * Inner class to manage the completion
     */
    class SciNotesInputParsingManager extends SciInputParsingManager {

        /**
         * Reset
         */
        public void reset() {
            try {
                ((ScilabDocument) editor.getTextPane().getDocument()).setFocused(true);
                Element root = editor.getTextPane().getDocument().getDefaultRootElement();
                int pos = editor.getTextPane().getCaretPosition();
                int line = root.getElementIndex(pos);
                int start = root.getElement(line).getStartOffset();
                editor.getTextPane().getDocument().remove(start, pos - start);
                editor.getTextPane().setCaretPosition(start);
            } catch (BadLocationException e) { }
        }

        /**
         * @return the caret position
         */
        public int getCaretPosition() {
            Element root = editor.getTextPane().getDocument().getDefaultRootElement();
            int pos = editor.getTextPane().getCaretPosition();
            int line = root.getElementIndex(pos);
            int start = root.getElement(line).getStartOffset();
            return pos - start;
        }

        /**
         * @return the commandLine
         */
        public String getCommandLine() {
            try {
                Element root = editor.getTextPane().getDocument().getDefaultRootElement();
                int pos = editor.getTextPane().getCaretPosition();
                int line = root.getElementIndex(pos);
                int start = root.getElement(line).getStartOffset();
                KeywordEvent ke = editor.getTextPane().getKeywordEvent();
                if (ke.getType() == ScilabLexerConstants.FIELD) {
                    ke = editor.getTextPane().getKeywordEvent(ke.getStart() - 1);
                    if (ke.getType() == ScilabLexerConstants.ID) {
                        // Here we try to complete a field of an unknown variable so
                        // we suppose that it will be a handle
                        return "";
                    }
                }
                return editor.getTextPane().getDocument().getText(start, pos - start);
            } catch (BadLocationException e) { }
            return "";
        }

        /**
         * @param content to append
         */
        public void append(String content) {
            try {
                ScilabDocument doc = (ScilabDocument) editor.getTextPane().getDocument();
                doc.setFocused(true);
                Element root = editor.getTextPane().getDocument().getDefaultRootElement();
                int pos = editor.getTextPane().getCaretPosition();
                doc.mergeEditsBegin();
                editor.getTextPane().getDocument().insertString(pos, content, null);
                doc.mergeEditsEnd();
            } catch (BadLocationException e) { }
        }

        /**
         * @return a Point where to put the completion window
         */
        public Point getWindowCompletionLocation() {
            try {
                int pos = editor.getTextPane().getCaretPosition();
                Rectangle r = editor.getTextPane().modelToView(pos);
                return new Point(r.x, r.y + r.height);
            } catch (BadLocationException e) { }
            return new Point(0, 0);
        }

        /**
         * @return an int
         */
        public int getCompletionLevel() {
            return 0;
        }

        /**
         * @param level an int
         * @return a String to be completed
         */
        public String getPartLevel(int level) {
            return Completion.getPartLevel(getCommandLine());
        }

        /**
         * Return an empty string to be sure that word won't be completed
         * into filename.
         * @param level an unused int
         * @return a String
         */
        public String getFilePartLevel(int level) {
            return "";
        }

        /**
         * @param completionResult a String
         */
        public void writeCompletionPart(String completionResult) { }

        /**
         * @return an int
         */
        public int getNumberOfLines() {
            return 0;
        }

        /**
         * @return a boolean
         */
        public boolean isBlockEditing() {
            return true;
        }

        /**
         * @param inputCommandView an InputCommandView
         */
        public void setInputCommandView(InputCommandView inputCommandView) { }

        /**
         * Put a backspace
         */
        public void backspace() {
            try {
                Element root = editor.getTextPane().getDocument().getDefaultRootElement();
                int pos = editor.getTextPane().getCaretPosition();
                editor.getTextPane().getDocument().remove(pos - 1, 1);
                editor.getTextPane().setCaretPosition(pos - 1);
            } catch (BadLocationException e) { }
        }
    }

    /**
     * Inner class to handle special completion case in SciNotes.
     * When a field name is met, we can suppose that is a handle.
     */
    class SciNotesCompletionManager extends SciCompletionManager {

        /**
         * {@inheritDoc}
         */
        public SciNotesCompletionManager() {
            super();
        }

        /**
         * {@inheritDoc}
         */
        public List<CompletionItem> getCompletionItems() {
            List<CompletionItem> list = super.getCompletionItems();
            if (list.isEmpty()) {
                ScilabEditorPane sep = editor.getTextPane();
                KeywordEvent ke = sep.getKeywordEvent();
                String searchedPattern = null;
                boolean handle = false;
                try {
                    if (ke.getType() == ScilabLexerConstants.OPERATOR) {
                        String str = sep.getDocument().getText(ke.getStart(), ke.getLength());
                        if (str.equals(".")) {
                            searchedPattern = " ";
                        }
                    } else if (ke.getType() == ScilabLexerConstants.FIELD) {
                        searchedPattern = sep.getDocument().getText(ke.getStart(), ke.getLength());
                    }
                } catch (BadLocationException e) {
                    System.err.println(e);
                }

                String[] scilabHandlesDictionary = Completion.searchHandleGraphicsPropertiesDictionary(searchedPattern);
                String type = Messages.gettext("Graphics handle field");

                list = new ArrayList<CompletionItem>();
                if (scilabHandlesDictionary != null) {
                    for (String item : scilabHandlesDictionary) {
                        dictionary.add(new CompletionItemImpl(type, item + " (" + type + ")", item, Messages.gettext("No help")));
                    }
                }

                return dictionary;
            }
            return list;
        }
    }

    /**
     * Inner class which implements interface ConsoleConfiguration.
     * Only use to be compatible with the way to complete in the console
     */
    class SciNotesCompletionConfiguration implements ConsoleConfiguration {

        private SciNotesInputParsingManager xipm = new SciNotesInputParsingManager();
        private SciCompletionManager scm = new SciNotesCompletionManager();
        private SciNotesCompletionWindow cwi;

        /**
         * Constructor
         * @param textPane where to complete
         */
        public SciNotesCompletionConfiguration() {
            scm.setInputParsingManager(xipm);
            cwi = new SciNotesCompletionWindow(editor, scm);
            cwi.setInputParsingManager(xipm);
            cwi.setGraphicalContext(editor.getTextPane());
        }

        /**
         * @return an InputParsingManager
         */
        public InputParsingManager getInputParsingManager() {
            return xipm;
        }

        /**
         * Returns the current completion manager
         * @return the completion manager
         */
        public CompletionManager getCompletionManager() {
            return scm;
        }

        /**
         * @return the CompletionWindow
         */
        public CompletionWindow getCompletionWindow() {
            return cwi;
        }

        /**
         * @return null
         */
        public PromptView getPromptView() {
            return null;
        }

        /**
         * @return null
         */
        public OutputView getOutputView() {
            return null;
        }

        /**
         * @return null
         */
        public InputCommandView getInputCommandView() {
            return null;
        }

        /**
         * @return null
         */
        public StyledDocument getOutputViewStyledDocument() {
            return null;
        }

        /**
         * @return null
         */
        public StyledDocument getInputCommandViewStyledDocument() {
            return null;
        }

        /**
         * @return null
         */
        public HistoryManager getHistoryManager() {
            return null;
        }

        /**
         * @return null
         */
        public String getBackgroundColor() {
            return null;
        }

        /**
         * @return null
         */
        public String getForegroundColor() {
            return null;
        }

        /**
         * @return 0
         */
        public int getScrollableUnitIncrement() {
            return 0;
        }

        /**
         * @return 0
         */
        public int getScrollableBlockIncrement() {
            return 0;
        }

        /**
         * @return true
         */
        public boolean getHorizontalWrapAllowed() {
            return true;
        }

        /**
         * @return true
         */
        public boolean getVerticalWrapAllowed() {
            return true;
        }

        /**
         * @return null
         */
        public String getFontName() {
            return null;
        }

        /**
         * @return 0
         */
        public int getFontStyle() {
            return 0;
        }

        /**
         * @return 0
         */
        public int getFontSize() {
            return 0;
        }

        /**
         * @return null
         */
        public String getWelcomeLine() {
            return null;
        }

        /**
         * @return null
         */
        public GenericInterpreter getGenericInterpreter() {
            return null;
        }

        /**
         * @return null
         */
        public ActionMap getActionMapping() {
            return null;
        }

        /**
         * @param profileName a String
         */
        public void setActiveProfile(String profileName) { }

        /**
         * @return null
         */
        public String getActiveProfile() {
            return null;
        }

        /**
         * @return null
         */
        public InputMap getKeyMapping() {
            return null;
        }
    }
}
