/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
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

import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.SciNotesMessages;
import org.scilab.modules.scinotes.utils.NavigatorWindow;

/**
 * @author Bruno JOFRET
 * @author Calixte DENIZET
 */
@SuppressWarnings("serial")
public final class CodeNavigatorAction extends DefaultAction {

    /**
     * Constructor
     * @param editor SciNotes
     */
    private CodeNavigatorAction(SciNotes editor) {
        super(SciNotesMessages.CODE_NAVIGATOR, editor);
    }

    /**
     * createMenu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return createMenu
     */
    public static MenuItem createMenu(SciNotes editor, KeyStroke key) {
        return createMenu(SciNotesMessages.CODE_NAVIGATOR, null, new CodeNavigatorAction(editor), key);
    }

    /**
     * Action !!
     */
    public void doAction() {
        if (!getEditor().getTextPane().hasNavigator()) {
            new NavigatorWindow(getEditor().getTextPane());
        }
    }
}
