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
public class RedoAction extends DefaultAction {

    /**
     * Constructor
     * @param editor Xpad
     */
    private RedoAction(Xpad editor) {
        super(XpadMessages.REDO, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        getEditor().redo();
    }

    /**
     * createMenu
     * @param editor Xpad
     * @param key KeyStroke
     */
    public static MenuItem createMenu(Xpad editor, KeyStroke key) {
        return createMenu(XpadMessages.REDO, null, new RedoAction(editor), key);
    }

    /**
     * createButton
     * @param editor Xpad
     */
    public static PushButton createButton(Xpad editor) {
        PushButton button = createButton(XpadMessages.REDO, "edit-redo.png", new RedoAction(editor));
        editor.setRedoButton(button);
        return button;
    }

    /**
     * Put input map
     * @param textPane JTextpane
     * @param editor Editor
     * @param key KeyStroke
     */
    public static void putInInputMap(JComponent textPane, Xpad editor, KeyStroke key) {
        textPane.getInputMap().put(key, new RedoAction(editor));
    }
}
