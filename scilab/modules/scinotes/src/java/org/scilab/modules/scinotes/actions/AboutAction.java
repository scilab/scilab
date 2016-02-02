/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
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
