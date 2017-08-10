/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

package org.scilab.modules.scinotes.utils;

import java.awt.Color;
import java.awt.Font;
import java.awt.print.PageFormat;
import java.io.IOException;

import org.scilab.modules.scinotes.ScilabEditorPane;

/**
 * Code to XSL-FO converter Class
 * @author Calixte DENIZET
 */
public class FOCodeConverter extends HTMLCodeConverter {

    private static final String ENTETE = "<?xml version=\"1.0\"?>\n"
                                         + "<root xmlns=\"http://www.w3.org/1999/XSL/Format\">\n"
                                         + "<layout-master-set>\n"
                                         + "<simple-page-master master-name=\"content\"\n"
                                         + "margin-right=\"MRin\"\n"
                                         + "margin-left=\"MLin\"\n"
                                         + "margin-bottom=\"MBin\"\n"
                                         + "margin-top=\"MTin\"\n"
                                         + "page-width=\"PWin\"\n"
                                         + "page-height=\"PHin\">\n"
                                         + "<region-body/>\n"
                                         + "</simple-page-master>\n"
                                         + "</layout-master-set>\n"
                                         + "<page-sequence master-reference=\"content\">\n"
                                         + "<flow flow-name=\"xsl-region-body\">";

    private static final String END = "</flow></page-sequence></root>";

    private int[] lineNumberArray;

    /**
     * Constructor
     * @param pane the ScilabEditorPane to convert (needed to have the context)
     */
    public FOCodeConverter(ScilabEditorPane pane) {
        super(pane);
    }

    /**
     * @param code the code to convert
     * @param lineNumberArray an array of integers to number correctly the file (depends on whereami)
     * @param format the page format to use
     * @return the XSL-FO corresponding code
     */
    public String convert(String code, int[] lineNumberArray, PageFormat format) {
        this.printLineNumber = lineNumberArray != null;
        this.lineNumberArray = lineNumberArray;
        buffer.setLength(0);
        Font f = ctx.tokenFonts[0];
        double ml = format.getImageableX() / 72;
        double mr = (format.getWidth() - format.getImageableWidth()) / 72 - ml;
        double mt = format.getImageableY() / 72;
        double mb = (format.getHeight() - format.getImageableHeight()) / 72 - mt;
        double pw = format.getWidth() / 72;
        double ph = format.getHeight() / 72;
        String entete = ENTETE.replace("MR", Double.toString(mr)).replace("ML", Double.toString(ml)).replace("MT", Double.toString(mt)).replace("MB", Double.toString(mb)).replace("PW", Double.toString(pw)).replace("PH", Double.toString(ph));
        buffer.append(entete);
        buffer.append("<block white-space-treatment=\"preserve\" linefeed-treatment=\"preserve\" white-space-collapse=\"false\" hyphenate=\"false\" wrap-option=\"wrap\" font-family=\"" + f.getFamily() + "\" font-size=\"" + Float.toString(f.getSize2D() * 9f / 14f) + "pt\">");
        if (printLineNumber) {
            String attr = "color=\"rgb(127,127,127)\" font-family=\"" + ctx.tokenFonts[0].getFamily() + "\" font-size=\"" + Float.toString(ctx.tokenFonts[0].getSize2D() * 9f / 14f) + "\"";
            buffer.append("<inline " + attr + ">" + getStringInt(lineNumber++) + "</inline>  ");
        }

        return lexer.convert(this, code) + "</block>\n" + END;
    }

    /**
     * {@inheritDoc}
     */
    public String convertColor(Color c) {
        return " color=\"rgb(" + c.getRed() + "," + c.getGreen() + "," + c.getBlue() + ")\" ";
    }

    /**
     * {@inheritDoc}
     */
    public String convertFont(Font f) {
        String xml = "";
        if (f.isBold()) {
            xml += "font-weight=\"bold\" ";
        }
        if (f.isItalic()) {
            xml += "font-style=\"italic\" ";
        } else {
            xml += "font-style=\"normal\" ";
        }

        return xml;
    }

    /**
     * {@inheritDoc}
     */
    public String convertAttrib(int a) {
        String xml = "";
        if ((a & 1) != 0) {
            xml += " text-decoration=\"underline\" ";
        }
        if ((a & 2) != 0) {
            xml += " text-decoration=\"line-through\" ";
        }

        return xml;
    }

    /**
     * {@inheritDoc}
     */
    public String convertTokenStyle(int tok, String contents) {
        return "<inline " + convertColor(ctx.tokenColors[tok]) + convertFont(ctx.tokenFonts[tok]) + convertAttrib(ctx.tokenAttrib[tok]) + ">" + contents + "</inline>";
    }

    /**
     * {@inheritDoc}
     */
    public void handleNothing(String seq) throws IOException {
        if (seq.equals("\n") && printLineNumber) {
            String attr = "color=\"rgb(127,127,127)\" font-family=\"" + ctx.tokenFonts[0].getFamily() + "\" font-size=\"" + Float.toString(ctx.tokenFonts[0].getSize2D() * 9f / 14f) + "\"";
            buffer.append("\n<inline " + attr + ">" + getStringInt(lineNumber++) + "</inline>  ");
        } else {
            buffer.append(seq);
        }
    }

    /**
     * {@inheritDoc}
     */
    public String getStringInt(int n) {
        return super.getStringInt(lineNumberArray[n - 1]);
    }
}
