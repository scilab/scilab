/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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

import org.scilab.modules.scinotes.ScilabDocument;

/**
 * Save File utility class
 * @author Allan CORNET
 *
 */
public final class SaveFile {

    private static final String LINE_SEPARATOR = "line.separator";

    /**
     * private Constructor
     */
    private SaveFile() {
    }
    /**
     * save text in JEditorPane
     * @param textPane JEditorPane
     * @param fOut File
     * @param editorKit EditorKit
     * @return true if saved
     */
    public static boolean doSave(JEditorPane textPane, File fOut, EditorKit editorKit) {

        ScilabDocument styledDocument = (ScilabDocument) textPane.getDocument();

        // get default eol
        String defaultEol = System.getProperty(LINE_SEPARATOR);

        // set eol used to save file
        if (styledDocument.getEOL().compareTo(defaultEol) != 0) {
            System.setProperty(LINE_SEPARATOR, styledDocument.getEOL());
        }
        boolean bReturn = false;

        BufferedWriter bw = null;
        OutputStreamWriter osw = null;
        FileOutputStream fos = null;

        try {
            fos = new FileOutputStream(fOut);
            osw = new OutputStreamWriter(fos, styledDocument.getEncoding());
            bw = new BufferedWriter(osw);
            editorKit.write(bw, styledDocument, 0, styledDocument.getLength());
            bw.flush();

            bReturn = true;
        } catch (IOException e) {
            bReturn = false;
        } catch (BadLocationException e) {
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
            } catch (IOException e) { }
        }

        // restore default eol
        System.setProperty(LINE_SEPARATOR, defaultEol);

        return bReturn;
    }
}
