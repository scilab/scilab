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

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;

import javax.swing.KeyStroke;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.AnswerOption;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.ButtonType;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * LoadIntoScilabAction class
 * @author Allan CORNET
 *
 */
public final class LoadIntoScilabAction extends DefaultAction {

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = -5659317486404897280L;

    private static final int MAX_LINES_RECOMMANDED = 100;
    private static final int NB_STANDARD_COLUMNS = 80;
    private static final int MAX_CHARACTERS_RECOMMANDED = MAX_LINES_RECOMMANDED * NB_STANDARD_COLUMNS;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public LoadIntoScilabAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        String text = getEditor().getTextPane().getText();

        if (text.compareTo("") != 0) {
            boolean bContinue = false;

            if (maxLinesRecommandedDetected(text)) {
                if (ScilabModalDialog.show(getEditor(), SciNotesMessages.BIG_FILE_WARNING, SciNotesMessages.LOAD_INTO_SCILAB,
                                           IconType.QUESTION_ICON, ButtonType.YES_NO) == AnswerOption.YES_OPTION) {
                    bContinue = true;
                } else {
                    bContinue = false;
                }
            } else {
                bContinue = true;
            }

            if (bContinue) {
                String tmpFilename = "LOAD_INTO_SCILAB-";

                FileOutputStream fos = null;
                OutputStreamWriter out = null;
                // save file as UTF-8
                try {
                    File f = File.createTempFile(tmpFilename, ".sce");
                    String tmpFullFilename = f.getAbsolutePath();
                    fos = new FileOutputStream(f);
                    out = new OutputStreamWriter(fos , "UTF-8");
                    out.write(text);
                    out.flush();
                    String cmdToExec = "exec('" + tmpFullFilename + "', 1)";
                    InterpreterManagement.requestScilabExec(cmdToExec);
                } catch (IOException e) {
                    ScilabModalDialog.show(getEditor(), SciNotesMessages.COULD_NOT_FIND_TMPFILE);
                } finally {
                    try {
                        if (fos != null) {
                            fos.close();
                        }
                        if (out != null) {
                            out.close();
                        }
                    } catch (IOException e) { }
                }
            }
        }
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new LoadIntoScilabAction(label, editor), key);
    }

    /**
     * check if string is not too big
     * @param text to scan
     * @return true or false
     */
    private static boolean maxLinesRecommandedDetected(String text) {
        if (text.length() > MAX_CHARACTERS_RECOMMANDED) {
            return true;
        }

        String[] splitedStringOnLineSeparator = text.split(System.getProperty("line.separator"));
        if (splitedStringOnLineSeparator.length > MAX_LINES_RECOMMANDED) {
            return true;
        }

        return false;
    }
}
