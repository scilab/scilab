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

import javax.swing.KeyStroke;

import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.SciNotesMessages;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;

/**
 * ActivateHelpOnTypingAction Class
 * @author Calixte DENIZET
 *
 */
public final class ActivateHelpOnTypingAction extends DefaultCheckAction {

    /**
     * Constructor
     * @param editor SciNotes
     */
    private ActivateHelpOnTypingAction(SciNotes editor) {
        super(SciNotesMessages.HELP_ON_TYPING, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        getEditor().setHelpOnTyping(this.getState());
        ConfigSciNotesManager.saveHelpOnTypingState(this.getState());
    }

    /**
     * createCheckBoxMenu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return CheckBoxMenuItem
     */
    public static CheckBoxMenuItem createCheckBoxMenu(SciNotes editor, KeyStroke key) {
        CheckBoxMenuItem cb = createCheckBoxMenu(SciNotesMessages.HELP_ON_TYPING, null, new ActivateHelpOnTypingAction(editor), key);
        boolean b = ConfigSciNotesManager.getHelpOnTypingState();
        cb.setChecked(b);
        return cb;
    }
}
