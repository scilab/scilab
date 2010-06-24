/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.scinotes.actions;

import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * AutoIndentAction Class
 * @author Bruno JOFRET
 *
 */
public final class AutoIndentAction extends DefaultCheckAction  {


    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = -1937347660350539353L;


    /**
     * Constructor
     * @param editor SciNotes
     */
    private AutoIndentAction(SciNotes editor) {
        super(SciNotesMessages.AUTO_INDENT, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        getEditor().setAutoIndent(this.getState());
        ConfigSciNotesManager.saveAutoIndent(this.getState());
    }


    /**
     * createCheckBoxMenu
     * @param editor SciNotes
     * @return CheckBoxMenuItem
     */
    public static CheckBoxMenuItem createCheckBoxMenu(SciNotes editor) {
        CheckBoxMenuItem autoIndent = createCheckBoxMenu(SciNotesMessages.AUTO_INDENT, null, new AutoIndentAction(editor), null);
        autoIndent.setChecked(ConfigSciNotesManager.getAutoIndent());
        return autoIndent;
    }
}
