/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.types;

/**
 * Exception which can occurred when a Sparse matrix is created
 */
@SuppressWarnings(value = { "serial" })
public class ScilabSparseException extends Exception {

    /**
     * Constructor
     *
     * @param msg
     *            the error message
     */
    public ScilabSparseException(String msg) {
        super(msg);
    }
}
