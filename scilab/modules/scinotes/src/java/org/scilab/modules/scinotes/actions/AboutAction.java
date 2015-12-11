/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
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
import org.scilab.modules.gui.utils.ScilabAboutBox;
import org.scilab.modules.scinotes.SciNotes;

/**
 * Display about box for the text editor
 * @author Bruno JOFRET
 * @author Vincent COUVERT
 */
public final class AboutAction extends DefaultAction {

    private static final long serialVersionUID = -8172282717877209957L;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor associated editor
     */
    public AboutAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * Menu created for "Help menu"
     * @param label label of the menu
     * @param editor associated editor
     * @param key KeyStroke
     * @return the menu
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new AboutAction(label, editor), key);
    }

    /**
     * Action !!
     * @see org.scilab.modules.graph.actions.DefaultAction#doAction()
     */
    public void doAction() {
        ScilabAboutBox.displayAndWait();
    }
}
