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

package org.scilab.modules.helptools.XML;

import java.io.IOException;

public abstract class AbstractXMLCodeHandler {

    /**
     * Handle default: when a char sequence is unidentified
     * @param seq the sequence to append
     */
    public void handleDefault(String seq) throws IOException { }

    /**
     * Handle entity such as &amp;amp;
     * @param seq the sequence to append
     */
    public void handleEntity(String seq) throws IOException { }

    /**
     * Nothing special
     * @param seq the sequence to append
     */
    public void handleNothing(String seq) throws IOException { }

    /**
     * Handle &lt;?
     * @param seq the sequence to append
     */
    public void handleOpenInstr(String seq) throws IOException { }

    /**
     * Handle ?&gt;
     * @param seq the sequence to append
     */
    public void handleCloseInstr(String seq) throws IOException { }

    /**
     * Handle instruction name
     * @param seq the sequence to append
     */
    public void handleInstrName(String seq) throws IOException { }

    /**
     * Handle &lt;
     * @param seq the sequence to append
     */
    public void handleLow(String seq) throws IOException { }

    /**
     * Handle &lt;name
     * @param seq the sequence to append
     */
    public void handleOpenTagName(String seq) throws IOException { }

    /**
     * Handle &lt;/
     * @param seq the sequence to append
     */
    public void handleLowClose(String seq) throws IOException { }

    /**
     * Handle &gt;
     * @param seq the sequence to append
     */
    public void handleGreat(String seq) throws IOException { }

    /**
     * Handle &lt;!--
     * @param seq the sequence to append
     */
    public void handleOpenComment(String seq) throws IOException { }

    /**
     * Handle &lt;![CDATA[
     * @param seq the sequence to append
     */
    public void handleOpenCdata(String seq) throws IOException { }

    /**
     * Handle an attribute name
     * @param seq the sequence to append
     */
    public void handleAttributeName(String seq) throws IOException { }

    /**
     * Handle '=' used in attribute definition
     * @param seq the sequence to append
     */
    public void handleEqual(String seq) throws IOException { }

    /**
     * Handle "value" or 'value'
     * @param seq the sequence to append
     */
    public void handleAttributeValue(String seq) throws IOException { }

    /**
     * Handle /&gt;
     * @param seq the sequence to append
     */
    public void handleAutoClose(String seq) throws IOException { }

    /**
     * Handle --&gt;
     * @param seq the sequence to append
     */
    public void handleCommentEnd(String seq) throws IOException { }

    /**
     * Handle comments
     * @param seq the sequence to append
     */
    public void handleComment(String seq) throws IOException { }

    /**
     * Handle ]]&gt;
     * @param seq the sequence to append
     */
    public void handleCdataEnd(String seq) throws IOException { }

    /**
     * Handle cdata contents
     * @param seq the sequence to append
     */
    public void handleCdata(String seq) throws IOException { }
}
