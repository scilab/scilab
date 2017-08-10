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

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;

import javax.swing.JMenuItem;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.KeywordEvent;
import org.scilab.modules.scinotes.MatchingBlockScanner;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.ScilabLexerConstants;

/**
 * Class to handle the block selection
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class SelectBlockAction extends DefaultAction {

    protected boolean isPopup;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public SelectBlockAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        ScilabEditorPane sep = getEditor().getTextPane();
        MatchingBlockScanner scanner = sep.getMatchingBlockManager(true).getScanner();
        KeywordEvent event = sep.getKeywordEvent(!isPopup, false);
        int tok = event.getType();
        int pos;
        MatchingBlockScanner.MatchingPositions mpos = null;
        if (ScilabLexerConstants.isMatchable(tok)) {
            pos = event.getStart();
            mpos = scanner.getMatchingBlock(pos, true);
            if (mpos != null) {
                sep.setSelectionStart(mpos.firstB);
                sep.setSelectionEnd(mpos.secondE);
            }
        }

        if (mpos == null) {
            scanner = sep.getMatchingBlockManager(false).getScanner();
            event = sep.getKeywordEvent(!isPopup, true);
            tok = event.getType();
            if (ScilabLexerConstants.isMatchable(tok)) {
                pos = event.getStart() + event.getLength();
                mpos = scanner.getMatchingBlock(pos, false);
                if (mpos != null) {
                    sep.setSelectionStart(mpos.secondB);
                    sep.setSelectionEnd(mpos.firstE);
                }
            }
        }
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, final SciNotes editor, KeyStroke key) {
        return createMenu(label, editor, key, new SelectBlockAction(label, editor));
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @param sba the SelectBlock action
     * @return MenuItem
     */
    protected static MenuItem createMenu(String label, final SciNotes editor, KeyStroke key, final SelectBlockAction sba) {
        final MenuItem menuitem = createMenu(label, null, sba, key);
        ((JMenuItem) menuitem.getAsSimpleMenuItem()).addPropertyChangeListener(new PropertyChangeListener() {
            public void propertyChange(PropertyChangeEvent e) {
                if (editor.getTextPane() != null) {
                    ScilabEditorPane sep = editor.getTextPane();
                    boolean block = ScilabLexerConstants.isMatchable(sep.getKeywordEvent(!sba.isPopup, false).getType());
                    block = block || ScilabLexerConstants.isMatchable(sep.getKeywordEvent(!sba.isPopup, true).getType());
                    menuitem.setEnabled(block);
                }
            }
        });

        return menuitem;
    }
}
