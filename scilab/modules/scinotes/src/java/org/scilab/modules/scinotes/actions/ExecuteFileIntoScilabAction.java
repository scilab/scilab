/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
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
 *
 */
public final class ExecuteFileIntoScilabAction extends DefaultAction {

        /**
         * serialVersionUID
         */
        private static final long serialVersionUID = -8625083632641564277L;

        /**
         * Constructor
         * @param editor SciNotes
         */
        private ExecuteFileIntoScilabAction(SciNotes editor) {
                super(SciNotesMessages.EXECUTE_FILE_INTO_SCILAB, editor);
        }

        /**
         * Execute the file into Scilab
         * @param editor the Scilab editor
         */
        private void executeFile(SciNotes editor) {

            String filePath = editor.getTextPane().getName();
                /*TODO : check if that fixes 7032 */
                filePath = filePath.replaceAll("\"", "\"\"");
                filePath = filePath.replaceAll("'", "''");
                /* end */
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
            /* Will execute the document file (file sould be saved)*/

            SciNotes editor = getEditor();

            if (((ScilabDocument) getEditor().getTextPane().getDocument()).isContentModified()) {
                if (ScilabModalDialog.show(getEditor(), SciNotesMessages.EXECUTE_WARNING, SciNotesMessages.EXECUTE_FILE_INTO_SCILAB,
                        IconType.WARNING_ICON, ButtonType.CANCEL_OR_SAVE_AND_EXECUTE) == AnswerOption.SAVE_EXECUTE_OPTION) {
                    if (editor.save(getEditor().getTabPane().getSelectedIndex(), false)) {
                                this.executeFile(editor);
                    }
                }
            } else {
                        this.executeFile(editor);
            }
        }

        /**
         * createMenu
         * @param editor SciNotes
         * @param key KeyStroke
         * @return MenuItem
         */
        public static MenuItem createMenu(SciNotes editor, KeyStroke key) {
                return createMenu(SciNotesMessages.EXECUTE_FILE_INTO_SCILAB, null, new ExecuteFileIntoScilabAction(editor), key);
        }
}
