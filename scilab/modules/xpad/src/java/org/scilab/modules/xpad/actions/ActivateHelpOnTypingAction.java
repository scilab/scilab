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
package org.scilab.modules.xpad.actions;

import javax.swing.JComponent;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.utils.XpadMessages;
import org.scilab.modules.xpad.utils.ConfigXpadManager;

/**
 * ActivateHelpOnTypingAction Class
 * @author Calixte DENIZET
 *
 */
public final class ActivateHelpOnTypingAction extends DefaultCheckAction {

    /**
     * Constructor
     * @param editor Xpad
     */
    private ActivateHelpOnTypingAction(Xpad editor) {
        super(XpadMessages.HELP_ON_TYPING, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        getEditor().setHelpOnTyping(this.getState());
        ConfigXpadManager.saveHelpOnTypingState(this.getState());
    }

    /**
     * createCheckBoxMenu
     * @param editor Xpad
     * @param key KeyStroke
     * @return CheckBoxMenuItem
     */
    public static CheckBoxMenuItem createCheckBoxMenu(Xpad editor, KeyStroke key) {
        CheckBoxMenuItem cb = createCheckBoxMenu(XpadMessages.HELP_ON_TYPING, null, new ActivateHelpOnTypingAction(editor), key);
        boolean b = ConfigXpadManager.getHelpOnTypingState();
        cb.setChecked(b);
        return cb;
    }
}
