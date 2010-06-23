/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO
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

import java.awt.Font;
import java.util.List;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

public class ResetFontAction extends DefaultAction {

    private ResetFontAction(SciNotes editor) {
        super(SciNotesMessages.RESET_DEFAULT_FONT, editor);
    }

    public void doAction() {
        Font oldFont = ConfigSciNotesManager.getDefaultFont();
        List<String> listStylesName = ConfigSciNotesManager.getAllStyleName();

        getEditor().getTextPane().resetFont(oldFont);

        /*we need to loop on every style , if not after the second change, styles will not change anymore
          except default*/
        int numberOfTab = getEditor().getTabPane().getComponentCount();
        for (int j = 0; j < numberOfTab; j++) {

            ScilabEditorPane textPane = getEditor().getTextPane(j);
            textPane.resetFont(oldFont);
            if (textPane.getOtherPaneInSplit() != null) {
                textPane.getOtherPaneInSplit().resetFont(oldFont);
            }
        }
        getEditor().getTextPane().setFocusable(true);

        ConfigSciNotesManager.saveFont(oldFont);
    }

    public static MenuItem createMenu(SciNotes editor) {
        return createMenu(SciNotesMessages.RESET_DEFAULT_FONT, null, new ResetFontAction(editor), null);
    }

}
