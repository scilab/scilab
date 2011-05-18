/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
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

import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.AnswerOption;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.ButtonType;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * ExecuteFileIntoScilabAction Class
 * @author Bruno JOFRET
 * @author Allan CORNET
 * @author Calixte DENIZET
 *
 */
public class ExecuteFileIntoScilabAction extends DefaultAction {

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = -8625083632641564277L;

    protected boolean saveBefore;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public ExecuteFileIntoScilabAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * Execute the file into Scilab
     * @param editor the Scilab editor
     */
    private void executeFile(SciNotes editor) {
        String filePath = editor.getTextPane().getName();
        if (filePath == null) {
            return;
        }

        filePath = filePath.replaceAll("\"", "\"\"");
        filePath = filePath.replaceAll("'", "''");
        if (filePath.compareTo("") != 0) {
            String cmdToExec = "exec('" + filePath + "', -1)";
            try {
                ScilabConsole.getConsole().getAsSimpleConsole().sendCommandsToScilab(cmdToExec, true, false);
            } catch (NoClassDefFoundError e) {
                /* This happens when SciNotes is launch as standalone (ie without
                 * Scilab) */
                ScilabModalDialog.show(editor, SciNotesMessages.COULD_NOT_FIND_CONSOLE);
            }
        }
    }

    /**
     * doAction
     */
    public void doAction() {
        SciNotes editor = getEditor();
        if (((ScilabDocument) getEditor().getTextPane().getDocument()).isContentModified()) {
            if (saveBefore || ScilabModalDialog.show(getEditor(), SciNotesMessages.EXECUTE_WARNING, SciNotesMessages.EXECUTE_FILE_INTO_SCILAB,
                                                     IconType.WARNING_ICON, ButtonType.CANCEL_OR_SAVE_AND_EXECUTE) == AnswerOption.SAVE_EXECUTE_OPTION) {
                if (editor.save(getEditor().getTabPane().getSelectedIndex(), true)) {
                    this.executeFile(editor);
                }
            }
        } else {
            this.executeFile(editor);
        }
    }

    /**
     * createButton
     * @param tooltip the tooltip
     * @param icon an icon name searched in SCI/modules/gui/images/icons/
     * @param editor SciNotes
     * @return PushButton
     */
    public static PushButton createButton(String tooltip, String icon, SciNotes editor) {
        return createButton(tooltip, icon, new ExecuteFileIntoScilabAction(tooltip, editor));
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new ExecuteFileIntoScilabAction(label, editor), key);
    }
}
