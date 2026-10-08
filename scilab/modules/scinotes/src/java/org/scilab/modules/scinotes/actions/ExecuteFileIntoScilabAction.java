/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * Copyright (C) 2010 - Calixte DENIZET
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 * Copyright (C) 2021 - UTC - Stéphane MOTTELET
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
import javax.swing.JComponent;
import javax.swing.KeyStroke;
import javax.swing.SwingUtilities;


import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.AnswerOption;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.ButtonType;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.scinotes.MonacoEditorPane;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.utils.SciNotesMessages;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.console.SciPromptView;
import org.scilab.modules.console.AdvCLIManagement;

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

    /** Whether the file is run in debug mode, stopping on its breakpoints. */
    protected boolean debug;

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
    protected void executeFile(SciNotes editor, String filePath) {
        if (filePath == null || filePath.isEmpty()) {
            return;
        }

        // The debug session runs the file itself when there is one, debugged
        // or not: the session knows the run began. The path is passed raw, the
        // endpoint quotes it.
        if (allowDapExecution() && executeThroughDap(editor, filePath, false, debug)) {
            return;
        }
        if (refuseWhileDebugging(editor)) {
            return;
        }

        filePath = filePath.replaceAll("\"", "\"\"");
        filePath = filePath.replaceAll("'", "''");
        if (filePath.compareTo("") != 0) {
            String cmdToExec = "exec('" + filePath + "', -1)";
            if (ScilabConsole.isExistingConsole())
            {
                ScilabConsole.getConsole().getAsSimpleConsole().sendCommandsToScilab(cmdToExec, true, false);
            } else {
                /* This happens when SciNotes is launched as standalone (ie without
                    * Scilab) or Scilab launched in -nw mode */
                new Thread(() -> {
                    try {
                        System.out.println(cmdToExec);
                        ScilabInterpreterManagement.synchronousScilabExec(cmdToExec);
                        System.out.println();
                        System.out.print(AdvCLIManagement.GetCurrentPrompt());
                    }
                    catch (ScilabInterpreterManagement.InterpreterException e) {
                        System.out.println(e.getMessage());
                    }
                }).start();
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
                    this.executeFile(editor, editor.getTextPane().getName());
                }
            }
        } else {
            this.executeFile(editor, editor.getTextPane().getName());
        }
    }

    /**
     * Whether this action may hand its file to the debug session. Running
     * several files in a row needs sequencing the session does not offer yet.
     * @return true when the debug session may take the run
     */
    protected boolean allowDapExecution() {
        return true;
    }

    /**
     * Hand one file to the debug session of this window.
     * @param editor SciNotes
     * @param filePath file to run
     * @param echo whether the file is executed with echo
     * @param debug whether the file is run in debug mode
     * @return true when the session took the run
     */
    public static boolean executeThroughDap(SciNotes editor, String filePath, boolean echo, boolean debug) {
        ScilabEditorPane pane = editor == null ? null : editor.getTextPane();
        return pane instanceof MonacoEditorPane
            && ((MonacoEditorPane) pane).executeFileThroughDap(filePath, echo, debug);
    }

    /**
     * Keep a run out of the interpreter while the debug session holds it,
     * and say why in the info bar.
     * @param editor SciNotes
     * @return true when the run must not start
     */
    public static boolean refuseWhileDebugging(SciNotes editor) {
        ScilabEditorPane pane = editor == null ? null : editor.getTextPane();
        if (pane instanceof MonacoEditorPane && ((MonacoEditorPane) pane).isDebugSessionBusy()) {
            editor.getInfoBar().setText(SciNotesMessages.DEBUG_SESSION_BUSY);
            return true;
        }
        return false;
    }


    /**
     * createButton
     * @param tooltip the tooltip
     * @param icon an icon name searched in SCI/modules/gui/images/icons/
     * @param editor SciNotes
     * @return PushButton
     */
    public static JButton createButton(String tooltip, String icon, SciNotes editor) {
        JButton button = createButton(tooltip, icon, new ExecuteFileIntoScilabAction(tooltip, editor));
        DebugCommandAction.registerExecuteControl(editor, button);
        return button;
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        MenuItem item = createMenu(label, null, new ExecuteFileIntoScilabAction(label, editor), key);
        DebugCommandAction.registerExecuteControl(editor, (JComponent) item.getAsSimpleMenuItem());
        return item;
    }
}
