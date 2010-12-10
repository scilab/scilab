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

package org.scilab.forge.scidoc.scilab;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class HTMLScilabCodeHandler extends AbstractScilabCodeHandler {

    private static final int BUFCAPACITY = 8192;
    private static HTMLScilabCodeHandler handler = new HTMLScilabCodeHandler();

    private List<String> undoc = new ArrayList();

    protected Map<String, String> mapId;
    protected String currentCommand;
    protected Appendable buffer;

    protected HTMLScilabCodeHandler() {
        buffer = new StringBuilder(BUFCAPACITY);
    }

    public static AbstractScilabCodeHandler getInstance(String currentCommand, Map<String, String> mapId) {
        handler.currentCommand = currentCommand;
        handler.mapId = mapId;
        ((StringBuilder) handler.buffer).setLength(0);
        return handler;
    }

    /**
     * {@inheritDoc}
     */
    public void handleDefault(String seq) throws IOException {
        buffer.append("<span class=\"default\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleOperator(String seq) throws IOException {
        buffer.append("<span class=\"operator\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleOpenClose(String seq) throws IOException {
        buffer.append("<span class=\"openclose\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleFKeywords(String seq) throws IOException {
        buffer.append("<span class=\"fkeyword\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleSKeywords(String seq) throws IOException {
        buffer.append("<span class=\"skeyword\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleCKeywords(String seq) throws IOException {
        buffer.append("<span class=\"ckeyword\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleConstants(String seq) throws IOException {
        buffer.append("<span class=\"constants\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleCommand(String seq) throws IOException {
        if (seq.equals(currentCommand)) {
            buffer.append("<span class=\"command\">");
            buffer.append(seq);
            buffer.append("</span>");
        } else {
            String link = mapId.get(seq);
            if (link == null) {
                buffer.append("<span class=\"command\">");
                buffer.append(seq);
                buffer.append("</span>");
                if (!undoc.contains(seq)) {
                    System.err.println("Warning: the command " + seq + " is used in an example and is undocumented.");
                    undoc.add(seq);
                }
            } else {
                buffer.append("<a class=\"command\" href=\"");
                buffer.append(mapId.get(seq));
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
            buffer.append("<span class=\"macro\">");
            buffer.append(seq);
            buffer.append("</span>");
        } else {
            String link = mapId.get(seq);
            if (link == null) {
                buffer.append("<span class=\"macro\">");
                buffer.append(seq);
                buffer.append("</span>");
                if (!undoc.contains(seq)) {
                    System.err.println("Warning: the macro " + seq + " is used in an example and is undocumented.");
                    undoc.add(seq);
                }
            } else {
                buffer.append("<a class=\"macro\" href=\"");
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
        buffer.append("<span class=\"functionid\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleFunctionIdDecl(String seq) throws IOException {
        buffer.append("<span class=\"functionid\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleId(String seq) throws IOException {
        buffer.append("<span class=\"id\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleInputOutputArgs(String seq) throws IOException {
        buffer.append("<span class=\"inputoutputargs\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleInputOutputArgsDecl(String seq) throws IOException {
        buffer.append("<span class=\"inputoutputargs\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleNumber(String seq) throws IOException {
        buffer.append("<span class=\"number\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleSpecial(String seq) throws IOException {
        buffer.append("<span class=\"special\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleString(String seq) throws IOException {
        buffer.append("<span class=\"string\">");
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
        buffer.append("<span class=\"field\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleComment(String seq) throws IOException {
        buffer.append("<span class=\"comment\">");
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
