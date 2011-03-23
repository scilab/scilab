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
