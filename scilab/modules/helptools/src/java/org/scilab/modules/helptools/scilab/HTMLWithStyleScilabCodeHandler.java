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

package org.scilab.modules.helptools.scilab;

import java.io.IOException;
import java.io.Reader;
import java.io.Writer;

import java.util.Set;

public class HTMLWithStyleScilabCodeHandler extends AbstractScilabCodeHandler {

    public static final String OPENCLOSE = "<span style=\"font-style:normal;color:rgb(74,85,219)\">";
    public static final String FIELD = "<span style=\"font-style:normal;color:rgb(170,170,170)\">";
    public static final String FUNCTIONID = "<span style=\"font-weight:bold;color:rgb(0,0,0)\">";
    public static final String INPUTOUTPUTARGS = "<span style=\"font-weight:bold;color:rgb(131,67,16)\">";
    public static final String ID = "<span style=\"font-style:normal;color:rgb(0,0,0)\">";
    public static final String STRING = "<span style=\"font-style:normal;color:rgb(188,143,143)\">";
    public static final String MACRO = "<span style=\"font-style:normal;color:rgb(174,92,176)\">";
    public static final String COMMAND = "<span style=\"font-style:normal;color:rgb(50,185,185)\">";
    public static final String CKEYWORD = "<span style=\"font-style:normal;color:rgb(95,158,160)\">";
    public static final String SKEYWORD = "<span style=\"font-style:normal;color:rgb(160,32,240)\">";
    public static final String FKEYWORD = "<span style=\"font-style:normal;color:rgb(176,24,19)\">";
    public static final String NUMBER = "<span style=\"font-style:normal;color:rgb(188,143,143)\">";
    public static final String OPERATOR = "<span style=\"font-style:normal;color:rgb(92,92,92)\">";
    public static final String CONSTANTS = "<span style=\"font-style:normal;color:rgb(218,112,214)\">";
    public static final String SPECIAL = "<span style=\"font-style:normal;color:rgb(255,170,0)\">";
    public static final String DEFAULT = "<span style=\"font-style:normal;color:rgb(0,0,0)\">";
    public static final String COMMENT = "<span style=\"font-style:italic;color:rgb(1,168,1)\">";

    private static final int BUFCAPACITY = 8192;

    private Appendable buffer;
    private ScilabLexer lexer;

    /**
     * Default constructor
     * @param primFile the file containing the scilab primitives
     * @param macroFile the file containing the scilab macros
     **/
    public HTMLWithStyleScilabCodeHandler(String primFile, String macroFile) {
        this.lexer = new ScilabLexer(primFile, macroFile);
    }

    /**
     * Default constructor
     * @param primitives the set containing the scilab primitives
     * @param macros the set containing the scilab macros
     **/
    public HTMLWithStyleScilabCodeHandler(Set<String> primitives, Set<String> macros) {
        this.lexer = new ScilabLexer(primitives, macros);
    }

    /**
     * @param scilabCode the code to convert
     * @return a String containing the HTML
     */
    public void convert(Reader in, Writer out) throws IOException {
        buffer = out;
        buffer.append("<pre>\n");
        String s = lexer.convert(this, in, false);
        if (s == null) {
            out.append("An error occurred during the conversion");
        }
        buffer.append("\n</pre>\n");
        out.flush();
    }

    /**
     * @param scilabCode the code to convert
     * @return a String containing the HTML
     */
    public String convert(String scilabCode) {
        buffer = new StringBuilder(BUFCAPACITY);
        ((StringBuilder) buffer).append("<pre>\n");
        return lexer.convert(this, scilabCode) + "\n</pre>\n";
    }

    /**
     * {@inheritDoc}
     */
    public void handleDefault(String seq) throws IOException {
        buffer.append(DEFAULT);
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleOperator(String seq) throws IOException {
        buffer.append(OPERATOR);
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleOpenClose(String seq) throws IOException {
        buffer.append(OPENCLOSE);
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleFKeywords(String seq) throws IOException {
        buffer.append(FKEYWORD);
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleSKeywords(String seq) throws IOException {
        buffer.append(SKEYWORD);
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleCKeywords(String seq) throws IOException {
        buffer.append(CKEYWORD);
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleConstants(String seq) throws IOException {
        buffer.append(CONSTANTS);
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleCommand(String seq) throws IOException {
        buffer.append(COMMAND);
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleMacro(String seq) throws IOException {
        buffer.append(MACRO);
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleFunctionId(String seq) throws IOException {
        buffer.append(FUNCTIONID);
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleFunctionIdDecl(String seq) throws IOException {
        buffer.append(FUNCTIONID);
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleId(String seq) throws IOException {
        buffer.append(ID);
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleInputOutputArgs(String seq) throws IOException {
        buffer.append(INPUTOUTPUTARGS);
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleInputOutputArgsDecl(String seq) throws IOException {
        buffer.append(INPUTOUTPUTARGS);
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleNumber(String seq) throws IOException {
        buffer.append(NUMBER);
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleSpecial(String seq) throws IOException {
        buffer.append(SPECIAL);
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleString(String seq) throws IOException {
        buffer.append(STRING);
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleNothing(String seq) throws IOException {
        buffer.append(seq);
    }

    /**
     * {@inheritDoc}
     */
    public void handleField(String seq) throws IOException {
        buffer.append(FIELD);
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleComment(String seq) throws IOException {
        buffer.append(COMMENT);
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public String toString() {
        return buffer.toString();
    }
}
