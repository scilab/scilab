/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.scinotes.actions;

import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;

/**
 * Class to handle the block selection in popup menu
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public final class SelectBlockInPopupAction extends SelectBlockAction {

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public SelectBlockInPopupAction(String name, SciNotes editor) {
        super(name, editor);
        isPopup = true;
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, final SciNotes editor, KeyStroke key) {
        return createMenu(label, editor, key, new SelectBlockInPopupAction(label, editor));
    }
}