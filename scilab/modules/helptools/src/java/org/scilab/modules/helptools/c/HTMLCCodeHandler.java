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

package org.scilab.modules.helptools.c;

import java.io.IOException;

public class HTMLCCodeHandler extends AbstractCCodeHandler {

    private static final int BUFCAPACITY = 8192;
    private static HTMLCCodeHandler handler = new HTMLCCodeHandler();

    protected Appendable buffer;

    protected HTMLCCodeHandler() {
        buffer = new StringBuilder(BUFCAPACITY);
    }

    public static AbstractCCodeHandler getInstance() {
        ((StringBuilder) handler.buffer).setLength(0);
        return handler;
    }

    public static String replaceEntity(String str) {
        return str.replaceAll("&", "&amp;").replaceAll("<", "&#0060;").replaceAll(">", "&#0062;").replaceAll("\"", "&#0034;").replaceAll("\'", "&#0039;");
    }

    /**
     * {@inheritDoc}
     */
    public void handleDefault(String seq) throws IOException {
        buffer.append("<span class=\"cdefault\">");
        buffer.append(replaceEntity(seq));
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleComment(String seq) throws IOException {
        buffer.append("<span class=\"ccomment\">");
        buffer.append(replaceEntity(seq));
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
    public void handleString(String seq) throws IOException {
        buffer.append("<span class=\"cstring\">");
        buffer.append(replaceEntity(seq));
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleNumber(String seq) throws IOException {
        buffer.append("<span class=\"cnumber\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleKeyword(String seq) throws IOException {
        buffer.append("<span class=\"ckeyword\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleType(String seq) throws IOException {
        buffer.append("<span class=\"ctype\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleModifier(String seq) throws IOException {
        buffer.append("<span class=\"cmodifier\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handlePreprocessor(String seq) throws IOException {
        buffer.append("<span class=\"cpreprocessor\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleOpenClose(String seq) throws IOException {
        buffer.append("<span class=\"copenclose\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleOperator(String seq) throws IOException {
        buffer.append("<span class=\"coperator\">");
        buffer.append(replaceEntity(seq));
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleId(String seq) throws IOException {
        buffer.append("<span class=\"cid\">");
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
