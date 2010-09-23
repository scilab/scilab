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
 * CopySelectionInNewTabAction class
 * @author Calixte DENIZET
 */
public final class CopySelectionInNewTabAction extends DefaultAction {

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = 320938663765236236L;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public CopySelectionInNewTabAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        String selection = getEditor().getTextPane().getSelectedText();
        SciNotes.scinotesWithText(selection);
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor scinotes
     * @param key KeyStroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, final SciNotes editor, KeyStroke key) {
        final MenuItem menuitem = createMenu(label, null, new CopySelectionInNewTabAction(label, editor), key);
        ((JMenuItem) menuitem.getAsSimpleMenuItem()).addPropertyChangeListener(new PropertyChangeListener() {
                public void propertyChange(PropertyChangeEvent e) {
                    String selection = editor.getTextPane().getSelectedText();
                    menuitem.setEnabled(selection != null);
                }
            });

        return menuitem;
    }
}
