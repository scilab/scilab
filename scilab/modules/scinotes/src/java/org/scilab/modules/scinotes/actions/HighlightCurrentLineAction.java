/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.scinotes.actions;

import javax.swing.JComponent;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.SciNotesMessages;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;

/**
 * HighlightCurrentLineAction Class
 * @author Sylvestre KOUMAR
 *
 */
public final class HighlightCurrentLineAction extends DefaultCheckAction {

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = -1489762718469013039L;

    /**
     * Constructor
     * @param editor SciNotes
     */
    private HighlightCurrentLineAction(SciNotes editor) {
        super(SciNotesMessages.HIGHLIGHT_CURRENT_LINE, editor);
        setState(false);
    }

    /**
     * doAction
     */
    public void doAction() {
        getEditor().enableHighlightedLine(this.getState());
        ConfigSciNotesManager.saveHighlightState(this.getState());
    }

    /**
     * createCheckBoxMenu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return CheckBoxMenuItem
     */
    public static CheckBoxMenuItem createCheckBoxMenu(SciNotes editor, KeyStroke key) {
        CheckBoxMenuItem cb = createCheckBoxMenu(SciNotesMessages.HIGHLIGHT_CURRENT_LINE, null, new HighlightCurrentLineAction(editor), key);
        cb.setChecked(ConfigSciNotesManager.getHighlightState());
        return cb;
    }

    /**
     * Put input map
     * @param textPane JTextpane
     * @param editor Editor
     * @param key KeyStroke
     */
    public static void putInInputMap(JComponent textPane, SciNotes editor, KeyStroke key) {
        textPane.getInputMap().put(key, new HighlightCurrentLineAction(editor));
    }
}
