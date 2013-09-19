/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
