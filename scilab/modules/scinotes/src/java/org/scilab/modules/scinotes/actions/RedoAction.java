/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
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

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * CommentAction Class
 * @author Bruno JOFRET
 *
 */
public class RedoAction extends DefaultAction {

    /**
     * Constructor
     * @param editor SciNotes
     */
    private RedoAction(SciNotes editor) {
        super(SciNotesMessages.REDO, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        getEditor().redo();
    }

    /**
     * createMenu
     * @param editor SciNotes
     * @param key KeyStroke
     */
    public static MenuItem createMenu(SciNotes editor, KeyStroke key) {
        return createMenu(SciNotesMessages.REDO, null, new RedoAction(editor), key);
    }

    /**
     * createButton
     * @param editor SciNotes
     */
    public static PushButton createButton(SciNotes editor) {
        PushButton button = createButton(SciNotesMessages.REDO, "edit-redo.png", new RedoAction(editor));
        editor.setRedoButton(button);
        return button;
    }

    /**
     * Put input map
     * @param textPane JTextpane
     * @param editor Editor
     * @param key KeyStroke
     */
    public static void putInInputMap(JComponent textPane, SciNotes editor, KeyStroke key) {
        textPane.getInputMap().put(key, new RedoAction(editor));
    }
}
