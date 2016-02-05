/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
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

import java.awt.Toolkit;

import javax.swing.JButton;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.ScilabEditorPane;

/**
 * CutAction Class
 * @author Bruno JOFRET
 * @author Calixte DENIZET
 */
public final class CutAction extends DefaultAction {

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = -4831313579986185630L;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public CutAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        ScilabDocument doc = (ScilabDocument) getEditor().getTextPane().getDocument();
        doc.mergeEditsBegin();
        if (!getEditor().getTextPane().copyColumnSelectionInClipBoard() || !getEditor().getTextPane().removeColumnSelection()) {
            String selection = getEditor().getTextPane().getSelectedText();
            if (selection != null) {
                CopyAsHTMLAction.HTMLSelection sel = new CopyAsHTMLAction.HTMLSelection(getEditor().getTextPane(), selection, false);
                Toolkit.getDefaultToolkit().getSystemClipboard().setContents(sel, sel);
                getEditor().getTextPane().replaceSelection("");
            }
        }
        doc.mergeEditsEnd();
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new CutAction(label, editor), key);
    }

    /**
     * createButton
     * @param tooltip the tooltip
     * @param icon an icon name searched in SCI/modules/gui/images/icons/
     * @param editor SciNotes
     * @return PushButton
     */
    public static JButton createButton(String tooltip, String icon, SciNotes editor) {
        return createButton(tooltip, icon, new CutAction(tooltip, editor));
    }
}
