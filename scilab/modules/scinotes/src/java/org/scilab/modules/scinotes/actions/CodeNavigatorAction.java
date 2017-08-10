/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
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

import javax.swing.JButton;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.tabfactory.CodeNavigatorTab;
import org.scilab.modules.scinotes.utils.NavigatorWindow;

/**
 * @author Bruno JOFRET
 * @author Calixte DENIZET
 */
@SuppressWarnings("serial")
public final class CodeNavigatorAction extends DefaultAction {

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public CodeNavigatorAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return createMenu
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new CodeNavigatorAction(label, editor), key);
    }

    /**
     * Action !!
     */
    public void doAction() {
        getEditor().addNavigator();
    }

    /**
     * createButton
     * @param tooltip the tooltip
     * @param icon an icon name searched in SCI/modules/gui/images/icons/
     * @param editor SciNotes
     * @return PushButton
     */
    public static JButton createButton(String tooltip, String icon, SciNotes editor) {
        return createButton(tooltip, icon, new CodeNavigatorAction(tooltip, editor));
    }
}
