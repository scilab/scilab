/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
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
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 *
 * @author Bruno JOFRET
 *
 */
public class SaveAsAction extends DefaultAction {

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = 8327808176820789884L;

    private String path;

    /**
     * Constructor
     * @param name the name of the action
     * @param path the default path where to open the filechooser
     * @param editor SciNotes
     */
    public SaveAsAction(String name, String path, SciNotes editor) {
        super(name, editor);
        this.path = path;
    }

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public SaveAsAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * DoAction
     */
    public void doAction() {
        if (!getEditor().saveAs(path)) {
            ScilabModalDialog.show(getEditor(), SciNotesMessages.COULD_NOT_SAVE_FILE,
                                   SciNotesMessages.SCINOTES_ERROR, IconType.ERROR_ICON);
        }
    }

    /**
     * CreateMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return MenuItem
     */
    public static Object createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new SaveAsAction(label, null, editor), key);
    }

    /**
     * createButton
     * @param tooltip the tooltip
     * @param icon an icon name searched in SCI/modules/gui/images/icons/
     * @param editor SciNotes
     * @return PushButton
     */
    public static JButton createButton(String tooltip, String icon, SciNotes editor) {
        return createButton(tooltip, icon, new SaveAsAction(tooltip, null, editor));
    }
}
