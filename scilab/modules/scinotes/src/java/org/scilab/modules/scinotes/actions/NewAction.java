/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.scinotes.actions;

import javax.swing.JButton;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;

/**
 * File creation management
 * @author Bruno JOFRET
 */
public final class NewAction extends DefaultAction {

    private static final long serialVersionUID = -6865132453503118587L;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor associated SciNotes instance
     */
    public NewAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * Create file action
     * @see org.scilab.modules.scinotes.actions.DefaultAction#doAction()
     */
    public void doAction() {
        SciNotes ed = getEditor();
        ed.addEmptyTab();
        ConfigSciNotesManager.saveToOpenFiles(ed.getTextPane().getName(), ed, ed.getTextPane());
    }

    /**
     * Create a menu to add to SciNotes menu bar
     * @param label label of the menu
     * @param editor associated SciNotes instance
     * @param key KeyStroke
     * @return the menu
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new NewAction(label, editor), key);
    }

    /**
     * createButton
     * @param tooltip the tooltip
     * @param icon an icon name searched in SCI/modules/gui/images/icons/
     * @param editor SciNotes
     * @return PushButton
     */
    public static JButton createButton(String tooltip, String icon, SciNotes editor) {
        return createButton(tooltip, icon, new NewAction(tooltip, editor));
    }
}
