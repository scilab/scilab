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

public abstract class AbstractCCodeHandler {

    /**
     * Handle default: when a char sequence is unidentified
     * @param seq the sequence to append
     */
    public void handleDefault(String seq) throws IOException { }

    /**
     * Handle comments
     * @param seq the sequence to append
     */
    public void handleComment(String seq) throws IOException { }

    /**
     * Nothing special
     * @param seq the sequence to append
     */
    public void handleNothing(String seq) throws IOException { }

    /**
     * Handle string
     * @param seq the sequence to append
     */
    public void handleString(String seq) throws IOException { }

    /**
     * Handle number
     * @param seq the sequence to append
     */
    public void handleNumber(String seq) throws IOException { }

    /**
     * Handle keyword such as 'for'
     * @param seq the sequence to append
     */
    public void handleKeyword(String seq) throws IOException { }

    /**
     * Handle type such as 'int'
     * @param seq the sequence to append
     */
    public void handleType(String seq) throws IOException { }

    /**
     * Handle modifier such as 'inline'
     * @param seq the sequence to append
     */
    public void handleModifier(String seq) throws IOException { }

    /**
     * Handle preprocessor instruction such as '#define'
     * @param seq the sequence to append
     */
    public void handlePreprocessor(String seq) throws IOException { }

    /**
     * Handle (,),{,},...
     * @param seq the sequence to append
     */
    public void handleOpenClose(String seq) throws IOException { }

    /**
     * Handle +,-,*,...
     * @param seq the sequence to append
     */
    public void handleOperator(String seq) throws IOException { }

    /**
     * Handle an identifier (starts with a lower case char)
     * @param seq the sequence to append
     */
    public void handleId(String seq) throws IOException { }
}
