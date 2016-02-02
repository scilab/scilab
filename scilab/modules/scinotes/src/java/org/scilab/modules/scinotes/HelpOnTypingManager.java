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

package org.scilab.modules.scinotes;

import java.awt.event.KeyListener;
import java.awt.event.KeyEvent;

import javax.swing.text.BadLocationException;

import org.scilab.modules.commons.xml.XConfiguration;

/**
 * This class handles the help on typing
 * @author Calixte DENIZET
 */
public final class HelpOnTypingManager implements KeyListener {

    private static HelpOnTypingManager instance;
    private boolean openers;
    private boolean keywords;

    /**
     * Constructor
     */
    private HelpOnTypingManager() {
        super();
        instance = this;
        openers = SciNotesOptions.getSciNotesDisplay().autoCompleteOpeners;
        keywords = SciNotesOptions.getSciNotesDisplay().autoCompleteKeywords;
    }

    /**
     * Clean
     */
    public static void close() {
        instance = null;
    }

    /**
     * @return the singleton instance of HelpOnTypingManager
     */
    public static HelpOnTypingManager getInstance() {
        if (instance == null) {
            instance = new HelpOnTypingManager();
        }

        return instance;
    }

    /**
     * @param b true if autocompletion on '(', '{', ...
     */
    public void enableOpeners(boolean b) {
        openers = b;
        SciNotesOptions.getSciNotesDisplay().autoCompleteOpeners = b;
        XConfiguration.set(XConfiguration.getXConfigurationDocument(), SciNotesOptions.DISPLAYPATH + "/@auto-complete-openers", Boolean.toString(b));
    }

    /**
     * @param b true if autocompletion on 'if', 'function', ...
     */
    public void enableKeywords(boolean b) {
        keywords = b;
        SciNotesOptions.getSciNotesDisplay().autoCompleteKeywords = b;
        XConfiguration.set(XConfiguration.getXConfigurationDocument(), SciNotesOptions.DISPLAYPATH + "/@auto-complete-keywords", Boolean.toString(b));
    }

    /**
     * @return true if help on typing is active
     */
    public boolean isActive() {
        return openers || keywords;
    }

    /**
     * @return true if help on typing for the openers is active
     */
    public boolean isOpenersActive() {
        return openers;
    }

    /**
     * @return true if help on typing for the openers is active
     */
    public boolean isKeywordsActive() {
        return keywords;
    }

    /**
     * Nothing !
     * @param e the event
     */
    public void keyPressed(KeyEvent e) { }

    /**
     * Nothing !
     * @param e the event
     */
    public void keyReleased(KeyEvent e) { }

    /**
     * Called when a key is typed
     * @param e the event
     */
    public void keyTyped(KeyEvent e) {
        char c = e.getKeyChar();
        ScilabEditorPane textPane = ScilabEditorPane.getFocusedPane();
        ScilabDocument doc = (ScilabDocument) textPane.getDocument();
        if (isActive() && c != KeyEvent.CHAR_UNDEFINED && textPane.getSelectionStart() == textPane.getSelectionEnd()) {
            int pos = textPane.getCaretPosition();
            if (c == ' ' && e.getModifiers() == 0) {
                int end = doc.getDefaultRootElement().getElement(doc.getDefaultRootElement().getElementIndex(pos)).getEndOffset() - 1;
                /* the following test is used to know if an insertion in the line is done */
                if (pos == end) {
                    try {
                        doc.insertString(pos, " ", null);
                        e.consume();
                        KeywordEvent kwe = textPane.getKeywordEvent(pos);
                        int[] ret;
                        String kw;
                        switch (kwe.getType()) {
                            case ScilabLexerConstants.OSKEYWORD :
                                kw = doc.getText(kwe.getStart(), kwe.getLength());
                                if ("if".equals(kw)) {
                                    if (complete("end", textPane, doc, pos)) {
                                        doc.insertString(pos + 1, " then\nend", null);
                                        ret = textPane.getIndentManager().indentDoc(pos + 1, pos + 9);
                                        textPane.setCaretPosition(ret[0]);
                                    }
                                } else if (!"end".equals(kw)) {
                                    if (complete("end", textPane, doc, pos)) {
                                        doc.insertString(pos + 1, "\nend", null);
                                        ret = textPane.getIndentManager().indentDoc(pos + 1, pos + 4);
                                        textPane.setCaretPosition(ret[0]);
                                    }
                                }
                                break;
                            case ScilabLexerConstants.SKEYWORD :
                                kw = doc.getText(kwe.getStart(), kwe.getLength());
                                if ("elseif".equals(kw)) {
                                    doc.insertString(pos + 1, " then", null);
                                    textPane.setCaretPosition(pos + 1);
                                }
                                break;
                            case ScilabLexerConstants.FKEYWORD :
                                /* We have 'function' or 'endfunction' */
                                if ("f".equals(doc.getText(kwe.getStart(), 1)) && complete("endfunction", textPane, doc, pos)) {
                                    doc.insertString(pos + 1, "()\nendfunction", null);
                                    textPane.getIndentManager().indentDoc(pos + 3, pos + 14);
                                    textPane.setCaretPosition(pos + 1);
                                }
                                break;
                            default :
                        }
                    } catch (BadLocationException exc) {
                        System.err.println(exc);
                    }
                }
            } else if (openers) {
                if (SciNotesOptions.getSciNotesPreferences().completeAtEOL) {
                    int end = doc.getDefaultRootElement().getElement(doc.getDefaultRootElement().getElementIndex(pos)).getEndOffset() - 1;
                    if (pos != end) {
                        return;
                    }
                }

                try {
                    String str;
                    switch (c) {
                        case '(' :
                            if (complete(')', textPane, doc, pos)) {
                                str = "()";
                            } else {
                                return;
                            }
                            break;
                        case '[' :
                            if (complete(']', textPane, doc, pos)) {
                                str = "[]";
                            } else {
                                return;
                            }
                            break;
                        case '{' :
                            if (complete('}', textPane, doc, pos)) {
                                str = "{}";
                            } else {
                                return;
                            }
                            break;
                        case '\"' :
                            str = "\"\"";
                            break;
                        default :
                            return;
                    }

                    doc.insertString(pos, str, null);
                    e.consume();
                    textPane.setCaretPosition(pos + 1);
                } catch (BadLocationException exc) {
                    System.err.println(exc);
                }
            }
        }
    }

    private static boolean complete(char next, ScilabEditorPane pane, ScilabDocument doc, int pos) throws BadLocationException {
        MatchingBlockManager matchLR = pane.getMatchingBlockManager(true);
        MatchingBlockScanner scanner = matchLR.getScanner();
        MatchingBlockScanner.MatchingPositions mpos = scanner.getNextBlock(pos, true);
        if (mpos != null) {
            char mc = doc.getText(mpos.secondB, 1).charAt(0);
            if (mc != next) {
                return true;
            }

            return false;
        }

        return true;
    }

    private static boolean complete(String next, ScilabEditorPane pane, ScilabDocument doc, int pos) throws BadLocationException {
        MatchingBlockManager matchLR = pane.getMatchingBlockManager(true);
        MatchingBlockScanner scanner = matchLR.getScanner();
        MatchingBlockScanner.MatchingPositions mpos = scanner.getNextBlock(pos, true);
        if (mpos != null) {
            String ms = doc.getText(mpos.secondB, mpos.secondE - mpos.secondB);
            if (!next.equals(ms)) {
                return true;
            }

            return false;
        }

        return true;
    }
}
