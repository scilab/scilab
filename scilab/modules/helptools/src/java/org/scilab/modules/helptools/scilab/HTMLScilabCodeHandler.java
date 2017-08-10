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

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class HTMLScilabCodeHandler extends AbstractScilabCodeHandler {

    private static final int BUFCAPACITY = 8192;
    private static HTMLScilabCodeHandler handler = new HTMLScilabCodeHandler();
    private static LinkWriter linkWriter = new LinkWriter();

    private List<String> undoc = new ArrayList<String>();

    protected String currentCommand;
    protected String currentFileName;
    protected Appendable buffer;

    /**
     * Default constructor
     */
    protected HTMLScilabCodeHandler() {
        buffer = new StringBuilder(BUFCAPACITY);
    }

    /**
     * @param lw the LinkWriter to use to handle links
     */
    public static void setLinkWriter(LinkWriter lw) {
        HTMLScilabCodeHandler.linkWriter = lw;
    }

    /**
     * @return an instance of HTMLScilabCodeHandler
     */
    public static AbstractScilabCodeHandler getInstance(String currentCommand, String filename) {
        handler.currentCommand = currentCommand;
        handler.currentFileName = filename;
        ((StringBuilder) handler.buffer).setLength(0);
        return handler;
    }

    /**
     * {@inheritDoc}
     */
    public void handleDefault(String seq) throws IOException {
        buffer.append("<span class=\"scilabdefault\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleOperator(String seq) throws IOException {
        buffer.append("<span class=\"scilaboperator\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleOpenClose(String seq) throws IOException {
        buffer.append("<span class=\"scilabopenclose\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleFKeywords(String seq) throws IOException {
        buffer.append("<span class=\"scilabfkeyword\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleSKeywords(String seq) throws IOException {
        buffer.append("<span class=\"scilabskeyword\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleCKeywords(String seq) throws IOException {
        buffer.append("<span class=\"scilabckeyword\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleConstants(String seq) throws IOException {
        buffer.append("<span class=\"scilabconstants\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleCommand(String seq) throws IOException {
        if (seq.equals(currentCommand)) {
            buffer.append("<span class=\"scilabcommand\">");
            buffer.append(seq);
            buffer.append("</span>");
        } else {
            String link = linkWriter.getLink(seq);
            if (link == null) {// the command is not internal
                buffer.append("<span class=\"scilabcommand\">");
                buffer.append(seq);
                buffer.append("</span>");
                //if (!undoc.contains(seq)) {
                System.err.println("Warning: the command " + seq + " is used in an example and is undocumented (" + new File(currentFileName).getName() + ").");
                undoc.add(seq);
                //}
            } else {
                buffer.append("<a class=\"scilabcommand\" href=\"");
                buffer.append(link);
                buffer.append("\">");
                buffer.append(seq);
                buffer.append("</a>");
            }
        }
    }

    /**
     * {@inheritDoc}
     */
    public void handleMacro(String seq) throws IOException {
        if (seq.equals(currentCommand)) {
            buffer.append("<span class=\"scilabmacro\">");
            buffer.append(seq);
            buffer.append("</span>");
        } else {
            String link = linkWriter.getLink(seq);
            if (link == null) {
                buffer.append("<span class=\"scilabmacro\">");
                buffer.append(seq);
                buffer.append("</span>");
                //if (!undoc.contains(seq)) {
                System.err.println("Warning: the macro " + seq + " is used in an example and is undocumented (" + new File(currentFileName).getName() + ").");
                undoc.add(seq);
                //}
            } else {
                buffer.append("<a class=\"scilabmacro\" href=\"");
                buffer.append(link);
                buffer.append("\">");
                buffer.append(seq);
                buffer.append("</a>");
            }
        }
    }

    /**
     * {@inheritDoc}
     */
    public void handleFunctionId(String seq) throws IOException {
        buffer.append("<span class=\"scilabfunctionid\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleFunctionIdDecl(String seq) throws IOException {
        buffer.append("<span class=\"scilabfunctionid\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleId(String seq) throws IOException {
        buffer.append("<span class=\"scilabid\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleInputOutputArgs(String seq) throws IOException {
        buffer.append("<span class=\"scilabinputoutputargs\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleInputOutputArgsDecl(String seq) throws IOException {
        buffer.append("<span class=\"scilabinputoutputargs\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleNumber(String seq) throws IOException {
        buffer.append("<span class=\"scilabnumber\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleSpecial(String seq) throws IOException {
        buffer.append("<span class=\"scilabspecial\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleString(String seq) throws IOException {
        buffer.append("<span class=\"scilabstring\">");
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
        buffer.append("<span class=\"scilabfield\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleComment(String seq) throws IOException {
        buffer.append("<span class=\"scilabcomment\">");
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
