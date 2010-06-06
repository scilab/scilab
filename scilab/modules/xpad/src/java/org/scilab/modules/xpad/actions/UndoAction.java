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

package org.scilab.modules.xpad.actions;

import javax.swing.JComponent;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.utils.XpadMessages;

/**
 * CommentAction Class
 * @author Bruno JOFRET
 *
 */
public final class UndoAction extends DefaultAction {

    /**
     * Constructor
     * @param editor Xpad
     */
    private UndoAction(Xpad editor) {
        super(XpadMessages.UNDO, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        getEditor().undo();
    }

    /**
     * Create the MenuItem for undo action
     * @param editor Editor
     * @param key KeyStroke
     * @return a MenuItem
     */
    public static MenuItem createMenu(Xpad editor, KeyStroke key) {
        return createMenu(XpadMessages.UNDO, null, new UndoAction(editor), key);
    }


    /**
     * Create the menu for undo action
     * @param editor Editor
     * @return a PushButton
     */
    public static PushButton createButton(Xpad editor) {
        PushButton button = createButton(XpadMessages.UNDO, "edit-undo.png", new UndoAction(editor));
        editor.setUndoButton(button);
        return button;
    }

    /**
     * Put input map
     * @param textPane JTextpane
     * @param editor Editor
     * @param key KeyStroke
     */
    public static void putInInputMap(JComponent textPane, Xpad editor, KeyStroke key) {
        textPane.getInputMap().put(key, new UndoAction(editor));
    }
}
