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
                if (editor.getTextPane() != null) {
                    String selection = editor.getTextPane().getSelectedText();
                    menuitem.setEnabled(selection != null);
                }
            }
        });

        return menuitem;
    }
}
