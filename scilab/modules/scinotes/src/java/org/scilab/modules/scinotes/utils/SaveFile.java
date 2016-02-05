/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
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
 *
 */
package org.scilab.modules.scinotes.utils;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.UnsupportedEncodingException;

import javax.swing.JEditorPane;
import javax.swing.text.BadLocationException;
import javax.swing.text.EditorKit;

import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.AnswerOption;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.ButtonType;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.scinotes.SciNotesAutosave;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.SciNotesOptions;

/**
 * Save File utility class
 * @author Allan CORNET
 * @author Calixte DENIZET
 */
public final class SaveFile {

    private static final String LINE_SEPARATOR = "line.separator";

    /**
     * private Constructor
     */
    private SaveFile() { }

    /**
     * save text in JEditorPane
     * @param textPane JEditorPane
     * @param fOut File
     * @param editorKit EditorKit
     * @return true if saved
     */
    public static boolean doSave(ScilabEditorPane textPane, int index, File fOut, EditorKit editorKit) {
        return doSave(textPane, index, fOut, editorKit, true, false);
    }

    /**
     * save a document
     * @param doc the document to save
     * @param fOut File
     * @param editorKit EditorKit
     * @return true if saved
     */
    public static boolean doSave(ScilabDocument doc, File fOut, EditorKit editorKit) throws IOException {
        // get default eol
        String defaultEol = System.getProperty(LINE_SEPARATOR);
        boolean bReturn = false;
        BufferedWriter bw = null;
        OutputStreamWriter osw = null;
        FileOutputStream fos = null;

        // set eol used to save file
        if (doc.getEOL().compareTo(defaultEol) != 0) {
            System.setProperty(LINE_SEPARATOR, doc.getEOL());
        }

        try {
            fos = new FileOutputStream(fOut);
            osw = new OutputStreamWriter(fos, SciNotesOptions.getSciNotesPreferences().encoding);
            bw = new BufferedWriter(osw);
            editorKit.write(bw, doc, 0, doc.getLength());
            bw.flush();
            bReturn = true;
        } catch (BadLocationException e) {
            System.err.println(e);
        } finally {
            try {
                if (fos != null) {
                    fos.close();
                }
                if (osw != null) {
                    osw.close();
                }
                if (bw != null) {
                    bw.close();
                }
            } catch (IOException e) {
                System.err.println(e);
            }
        }

        // restore default eol
        System.setProperty(LINE_SEPARATOR, defaultEol);

        return bReturn;
    }

    /**
     * save text in JEditorPane
     * @param textPane JEditorPane
     * @param fOut File
     * @param editorKit EditorKit
     * @return true if saved
     */
    public static boolean doSave(ScilabEditorPane textPane, int index, File fOut, EditorKit editorKit, boolean addEOL, boolean silent) {
        return doSave(textPane, index, fOut, editorKit, addEOL, silent, SciNotesOptions.getSciNotesAutosave().autoSave);
    }

    /**
     * save text in JEditorPane
     * @param textPane JEditorPane
     * @param fOut File
     * @param editorKit EditorKit
     * @return true if saved
     */
    public static boolean doSave(ScilabEditorPane textPane, int index, File fOut, EditorKit editorKit, boolean addEOL, boolean silent, boolean backup) {
        ScilabDocument styledDocument = (ScilabDocument) textPane.getDocument();
        boolean enc = false;
        File backupOut = null;
        if (!styledDocument.getEncoding().equalsIgnoreCase(SciNotesOptions.getSciNotesPreferences().encoding)) {
            if (!silent) {
                String msg = String.format(SciNotesMessages.DIFFERENT_ENCODINGS, styledDocument.getEncoding(), SciNotesOptions.getSciNotesPreferences().encoding);
                if (ScilabModalDialog.show(textPane.getEditor(), msg, SciNotesMessages.DIFFERENT_ENCODINGS_TITLE, IconType.QUESTION_ICON, ButtonType.YES_NO) == AnswerOption.NO_OPTION) {
                    return false;
                }
                enc = true;
            } else {
                return false;
            }
        }

        if (backup) {
            backupOut = SciNotesAutosave.getBackupFile(fOut.getName());
        }

        try {
            fOut.createNewFile();
            if (backupOut != null) {
                backupOut.createNewFile();
            }
        } catch (IOException e) {
            System.err.println(e);
        }

        if (!fOut.canWrite()) {
            if (!silent) {
                ScilabModalDialog.show(textPane.getEditor(), SciNotesMessages.NOTWRITABLE, SciNotesMessages.SCINOTES_ERROR, IconType.ERROR_ICON);
            }
            return false;
        }

        if (backupOut != null && !backupOut.canWrite()) {
            backupOut = null;
        }

        // get default eol
        String defaultEol = System.getProperty(LINE_SEPARATOR);

        // set eol used to save file
        if (styledDocument.getEOL().compareTo(defaultEol) != 0) {
            System.setProperty(LINE_SEPARATOR, styledDocument.getEOL());
        }
        if (addEOL) {
            styledDocument.addEOL();
        }

        boolean bReturn = false;

        BufferedWriter bw = null;
        OutputStreamWriter osw = null;
        FileOutputStream fos = null;

        File[] files = new File[] {fOut, backupOut};

        for (File file : files) {
            if (file != null) {
                try {
                    fos = new FileOutputStream(file);
                    osw = new OutputStreamWriter(fos, SciNotesOptions.getSciNotesPreferences().encoding);
                    bw = new BufferedWriter(osw);
                    editorKit.write(bw, styledDocument, 0, styledDocument.getLength());
                    bw.flush();
                    bReturn = true;
                } catch (IOException e) {
                    System.err.println(e);
                    bReturn = false;
                } catch (BadLocationException e) {
                    System.err.println(e);
                    bReturn = false;
                } finally {
                    try {
                        if (fos != null) {
                            fos.close();
                        }
                        if (osw != null) {
                            osw.close();
                        }
                        if (bw != null) {
                            bw.close();
                        }
                    } catch (IOException e) {
                        System.err.println(e);
                    }
                }
            }
        }

        // restore default eol
        System.setProperty(LINE_SEPARATOR, defaultEol);
        if (enc) {
            textPane.getEditor().reload(index);
        }

        return bReturn;
    }
}
