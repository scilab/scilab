/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.scinotes.actions;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import javax.swing.JMenuItem;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;

/**
 * OverwriteAction class
 * @author Calixte DENIZET
 *
 */
public final class OverwriteAction extends DefaultAction {

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public OverwriteAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * Create Menu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, final SciNotes editor, KeyStroke key) {
        final MenuItem menuitem = createMenu(label, null, new OverwriteAction(label, editor), key);
        ((JMenuItem) menuitem.getAsSimpleMenuItem()).addPropertyChangeListener(new PropertyChangeListener() {
                public void propertyChange(PropertyChangeEvent e) {
                    if (editor.getTextPane() != null) {
                        menuitem.setEnabled(editor.getTextPane().checkExternalModif());
                    }
                }
            });

        return menuitem;
    }

    /**
     * DoAction
     */
    public void doAction() {
        getEditor().overwrite(getEditor().getTabPane().getSelectedIndex());
        getEditor().getTextPane().updateInfosWhenFocused();
    }
}
