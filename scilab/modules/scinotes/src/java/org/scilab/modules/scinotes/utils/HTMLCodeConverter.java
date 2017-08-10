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
import java.io.IOException;

import org.scilab.modules.scinotes.ScilabContext;
import org.scilab.modules.scinotes.ScilabEditorKit;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.ScilabLexerConstants;

import org.scilab.modules.helptools.scilab.AbstractScilabCodeHandler;
import org.scilab.modules.helptools.scilab.ScilabLexer;

/**
 * Code converter Class
 * @author Calixte DENIZET
 */
public class HTMLCodeConverter extends AbstractScilabCodeHandler {

    protected StringBuilder buffer = new StringBuilder(8192);

    protected ScilabContext ctx;
    protected ScilabLexer lexer;
    protected boolean printLineNumber;
    protected int lineNumber = 1;

    /**
     * Default constructor
     * @param pane where the conversion occurs
     */
    public HTMLCodeConverter(ScilabEditorPane pane) {
        this.ctx = ((ScilabEditorKit) pane.getEditorKit()).getStylePreferences();
        this.lexer = new ScilabLexer(org.scilab.modules.scinotes.ScilabLexer.commands, org.scilab.modules.scinotes.ScilabLexer.macros);
    }

    /**
     * @param code the Scilab code to convert
     * @param printLineNumber is true if the line numbers must be printed
     * @return the converted code in HTML
     */
    public String convert(String code, boolean printLineNumber) {
        this.printLineNumber = printLineNumber;
        buffer.setLength(0);
        buffer.append("<pre style=\"font-family:" + ctx.tokenFonts[0].getFamily() + ";font-style:normal;font-size:" + Float.toString(ctx.tokenFonts[0].getSize2D()) + ";\">");
        if (printLineNumber) {
            String attr = "font-family:" + ctx.tokenFonts[0].getFamily() + ";font-size:" + Float.toString(ctx.tokenFonts[0].getSize2D() * 9f / 14f) + ";";
            buffer.append("<span style=\"" + attr + "\">" + getStringInt(lineNumber++) + "</span>  ");
        }
        return lexer.convert(this, code) + "</pre>";
    }

    /**
     * @param c the color to convert
     * @return the css color as rgb(R, G ,B);
     */
    public String convertColor(Color c) {
        return "color:rgb(" + c.getRed() + "," + c.getGreen() + "," + c.getBlue() + ");";
    }

    /**
     * @param f the font to convert
     * @return the css styles for a font
     */
    public String convertFont(Font f) {
        String html = "";
        if (f.isBold()) {
            html += "font-weight:bold;";
        }
        if (f.isItalic()) {
            html += "font-style:italic;";
        }

        return html;
    }

    /**
     * @param a the attributes: 1 for underline &amp; 2 for line-through
     * @return the css styles for this text-decoration
     */
    public String convertAttrib(int a) {
        String html = "";
        if ((a & 1) != 0) {
            html += "text-decoration:underline;";
        }
        if ((a & 2) != 0) {
            html += "text-decoration:line-through;";
        }

        return html;
    }

    /**
     * @param tok the kind of token according to ScilabLexerConstants
     * @param contents contents to be enclosed
     * @return a span with contents correctly decorated
     */
    public String convertTokenStyle(int tok, String contents) {
        return "<span style=\"" + convertColor(ctx.tokenColors[tok]) + convertFont(ctx.tokenFonts[tok]) + convertAttrib(ctx.tokenAttrib[tok]) + "\">" + contents + "</span>";
    }

    /**
     * {@inheritDoc}
     */
    public void handleDefault(String seq) throws IOException {
        buffer.append(convertTokenStyle(ScilabLexerConstants.DEFAULT, seq));
    }

    /**
     * {@inheritDoc}
     */
    public void handleOperator(String seq) throws IOException {
        buffer.append(convertTokenStyle(ScilabLexerConstants.OPERATOR, seq));
    }

    /**
     * {@inheritDoc}
     */
    public void handleOpenClose(String seq) throws IOException {
        buffer.append(convertTokenStyle(ScilabLexerConstants.OPENCLOSE, seq));
    }

    /**
     * {@inheritDoc}
     */
    public void handleFKeywords(String seq) throws IOException {
        buffer.append(convertTokenStyle(ScilabLexerConstants.FKEYWORD, seq));
    }

    /**
     * {@inheritDoc}
     */
    public void handleSKeywords(String seq) throws IOException {
        buffer.append(convertTokenStyle(ScilabLexerConstants.SKEYWORD, seq));
    }

    /**
     * {@inheritDoc}
     */
    public void handleCKeywords(String seq) throws IOException {
        buffer.append(convertTokenStyle(ScilabLexerConstants.CKEYWORD, seq));
    }

    /**
     * {@inheritDoc}
     */
    public void handleConstants(String seq) throws IOException {
        buffer.append(convertTokenStyle(ScilabLexerConstants.CONSTANTES, seq));
    }

    /**
     * {@inheritDoc}
     */
    public void handleCommand(String seq) throws IOException {
        buffer.append(convertTokenStyle(ScilabLexerConstants.COMMANDS, seq));
    }

    /**
     * {@inheritDoc}
     */
    public void handleMacro(String seq) throws IOException {
        buffer.append(convertTokenStyle(ScilabLexerConstants.MACROS, seq));
    }

    /**
     * {@inheritDoc}
     */
    public void handleFunctionId(String seq) throws IOException {
        buffer.append(convertTokenStyle(ScilabLexerConstants.MACROINFILE, seq));
    }

    /**
     * {@inheritDoc}
     */
    public void handleFunctionIdDecl(String seq) throws IOException {
        buffer.append(convertTokenStyle(ScilabLexerConstants.MACROINFILE, seq));
    }

    /**
     * {@inheritDoc}
     */
    public void handleId(String seq) throws IOException {
        buffer.append(convertTokenStyle(ScilabLexerConstants.ID, seq));
    }

    /**
     * {@inheritDoc}
     */
    public void handleInputOutputArgsDecl(String seq) throws IOException {
        buffer.append(convertTokenStyle(ScilabLexerConstants.INPUTOUTPUTARGS, seq));
    }

    /**
     * {@inheritDoc}
     */
    public void handleInputOutputArgs(String seq) throws IOException {
        buffer.append(convertTokenStyle(ScilabLexerConstants.INPUTOUTPUTARGS, seq));
    }

    /**
     * {@inheritDoc}
     */
    public void handleNumber(String seq) throws IOException {
        buffer.append(convertTokenStyle(ScilabLexerConstants.NUMBER, seq));
    }

    /**
     * {@inheritDoc}
     */
    public void handleSpecial(String seq) throws IOException {
        buffer.append(convertTokenStyle(ScilabLexerConstants.SPECIAL, seq));
    }

    /**
     * {@inheritDoc}
     */
    public void handleString(String seq) throws IOException {
        buffer.append(convertTokenStyle(ScilabLexerConstants.STRING, seq));
    }

    /**
     * {@inheritDoc}
     */
    public void handleNothing(String seq) throws IOException {
        if (seq.equals("\n") && printLineNumber) {
            String attr = "font-family:" + ctx.tokenFonts[0].getFamily() + ";font-size:" + Float.toString(ctx.tokenFonts[0].getSize2D() * 9f / 14f) + ";";
            buffer.append("\n<span style=\"" + attr + "\">" + getStringInt(lineNumber++) + "</span>  ");
        } else {
            buffer.append(seq);
        }
    }

    /**
     * {@inheritDoc}
     */
    public void handleField(String seq) throws IOException {
        buffer.append(convertTokenStyle(ScilabLexerConstants.FIELD, seq));
    }

    /**
     * {@inheritDoc}
     */
    public void handleComment(String seq) throws IOException {
        buffer.append(convertTokenStyle(ScilabLexerConstants.COMMENT, seq));
    }

    /**
     * {@inheritDoc}
     */
    public String toString() {
        return buffer.toString();
    }

    /**
     * @param n the line number
     * @return a string containing line number on 4 digits: 0001, 0002..., 9999
     */
    public String getStringInt(int n) {
        if (n < 10) {
            return "000" + n;
        } else if (n < 100) {
            return "00" + n;
        } else if (n < 1000) {
            return "0" + n;
        }

        return Integer.toString(n);
    }
}
