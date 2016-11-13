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

public abstract class AbstractScilabCodeHandler {

    /**
     * Handle default: when a char sequence is unidentified
     * @param seq the sequence to append
     */
    public void handleDefault(String seq) throws IOException { }

    /**
     * Handle operator: '+', '/', '*', ...
     * @param seq the sequence to append
     */
    public void handleOperator(String seq) throws IOException { }

    /**
     * Handle open/close: '(', ')', '[', ']', ...
     * @param seq the sequence to append
     */
    public void handleOpenClose(String seq) throws IOException { }

    /**
     * Handle function keywords: 'function' or 'endfunction'
     * @param seq the sequence to append
     */
    public void handleFKeywords(String seq) throws IOException { }

    /**
     * Handle structure keywords: 'if', 'for', ...
     * @param seq the sequence to append
     */
    public void handleSKeywords(String seq) throws IOException { }

    /**
     * Handle control keywords: 'abort', 'return', ...
     * @param seq the sequence to append
     */
    public void handleCKeywords(String seq) throws IOException { }

    /**
     * Handle contants: '%pi', '%e', ...
     * @param seq the sequence to append
     */
    public void handleConstants(String seq) throws IOException { }

    /**
     * Handle command: 'sin', 'cos', ...
     * @param seq the sequence to append
     */
    public void handleCommand(String seq) throws IOException { }

    /**
     * Handle macro: 'cosh', 'sinh', ...
     * @param seq the sequence to append
     */
    public void handleMacro(String seq) throws IOException { }

    /**
     * Handle function identifier which appears in the code
     * @param seq the sequence to append
     */
    public void handleFunctionId(String seq) throws IOException { }

    /**
     * Handle function identifier: 'function identifier(...)'
     * @param seq the sequence to append
     */
    public void handleFunctionIdDecl(String seq) throws IOException { }

    /**
     * Handle a valid scilab identifier
     * @param seq the sequence to append
     */
    public void handleId(String seq) throws IOException { }

    /**
     * Handle in/out args: 'function [...,out,...]=id(...,in,...)'
     * @param seq the sequence to append
     */
    public void handleInputOutputArgsDecl(String seq) throws IOException { }

    /**
     * Handle in/out args which appears in the code
     * @param seq the sequence to append
     */
    public void handleInputOutputArgs(String seq) throws IOException { }

    /**
     * Handle number: 123, 123.4, 123e4...
     * @param seq the sequence to append
     */
    public void handleNumber(String seq) throws IOException { }

    /**
     * Handle special: '$', ':', '..' (break operator)
     * @param seq the sequence to append
     */
    public void handleSpecial(String seq) throws IOException { }

    /**
     * Handle string: "hello world"
     * @param seq the sequence to append
     */
    public void handleString(String seq) throws IOException { }

    /**
     * Handle nothing: when no particular formatting is required
     * @param seq the sequence to append
     */
    public void handleNothing(String seq) throws IOException { }

    /**
     * Handle field: 'foo.bar'
     * @param seq the sequence to append
     */
    public void handleField(String seq) throws IOException { }

    /**
     * Handle comment: '// comments...'
     * @param seq the sequence to append
     */
    public void handleComment(String seq) throws IOException { }


    /**
     * Inner class to have different way to handle links
     */
    public static class LinkWriter {

        /**
         * Default Constructor
         */
        public LinkWriter() { }

        /**
         * @param id the id
         * @return the modified link
         */
        public String getLink(String id) {
            return id;
        }
    }
}
