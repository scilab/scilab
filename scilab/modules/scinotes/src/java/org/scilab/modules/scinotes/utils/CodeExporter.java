/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.scinotes.utils;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStream;
import java.io.StringReader;

import java.awt.print.PageFormat;

import javax.xml.transform.Result;
import javax.xml.transform.Source;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.sax.SAXResult;
import javax.xml.transform.stream.StreamSource;

import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.forge.scidoc.scilab.AbstractScilabCodeHandler;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;

import org.apache.fop.apps.FopFactory;
import org.apache.fop.apps.Fop;
import org.apache.fop.apps.FOUserAgent;
import org.apache.fop.apps.MimeConstants;

/**
 * Code converter Class
 * @author Calixte DENIZET
 */
public class CodeExporter extends FOCodeConverter {

    /**
     * Constant for PDF
     */
    public static final String PDF = MimeConstants.MIME_PDF;

    /**
     * Constant for PS
     */
    public static final String PS = MimeConstants.MIME_POSTSCRIPT;

    /**
     * Constant for EPS
     */
    public static final String EPS = MimeConstants.MIME_EPS;

    /**
     * Constant for RTF
     */
    public static final String RTF = MimeConstants.MIME_RTF;

    /**
     * Constant for PNG
     */
    public static final String PNG = MimeConstants.MIME_PNG;

    /**
     * Constant for PREVIEW a document in AWT window
     */
    public static final String PREVIEW = MimeConstants.MIME_FOP_AWT_PREVIEW;

    /**
     * Constant for PRINT
     */
    public static final String PRINT = MimeConstants.MIME_FOP_PRINT;

    private static final String CREATOR = "SciNotes for Scilab";
    private SciNotes editor;

    /**
     * Constructor
     * @param pane the ScilabEditorPane where the code must be converted.
     */
    public CodeExporter(ScilabEditorPane pane) {
        super(pane);
        editor = pane.getEditor();
    }

    /**
     * Make the conversion of a ScilabEditorPane
     * @param pane the ScilabEditorPane
     * @param the fileName for the output
     * @param type must on of the constants PDF, EPS, ...
     * @param format the page format
     */
    public static void convert(ScilabEditorPane pane, String fileName, String type, PageFormat format) {
        CodeExporter exporter = new CodeExporter(pane);
        exporter.convert(pane.getText(), pane.getXln().getLineNumber(), fileName, type, pane.getShortName(), format);
    }

    /**
     * Make the conversion of a ScilabEditorPane
     * @param pane the ScilabEditorPane
     * @param lineNumberArray an array containing integers to number the lines
     * @param the fileName for the output
     * @param type must on of the constants PDF, EPS, ...
     * @param format the page format
     */
    public void convert(String code, int[] lineNumberArray, String fileName, String type, String title, PageFormat format) {
        FopFactory fopFactory = FopFactory.newInstance();
        OutputStream out = null;

        try {
            fopFactory.setUserConfig(new File(ScilabConstants.SCI + "/modules/helptools/etc/fopconf.xml"));
            FOUserAgent userAgent = fopFactory.newFOUserAgent();
            userAgent.setProducer(CREATOR);
            userAgent.setTitle(title);
            userAgent.setAuthor(System.getProperty("user.name"));
            Fop fop;
            if (!type.equals(PRINT) && !type.equals(PREVIEW) && fileName != null && !fileName.isEmpty()) {
                out = new BufferedOutputStream(new FileOutputStream(new File(fileName)));
                fop = fopFactory.newFop(type, userAgent, out);
            } else {
                fop = fopFactory.newFop(type, userAgent);
            }
            TransformerFactory factory = TransformerFactory.newInstance();
            Transformer transformer = factory.newTransformer();
            String str = super.convert(code, lineNumberArray, format);
            Source src = new StreamSource(new StringReader(str));
            Result res = new SAXResult(fop.getDefaultHandler());
            transformer.transform(src, res);
        } catch (Exception e) {
            if (type.equals(PRINT)) {
                ScilabModalDialog.show(editor, SciNotesMessages.PRINTERERROR + "\n" + e.getLocalizedMessage());
            } else {
                System.err.println(SciNotesMessages.EXPORTERROR + ":");
                e.printStackTrace();
            }
        } finally {
            try {
                if (out != null) {
                    out.close();
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
}
