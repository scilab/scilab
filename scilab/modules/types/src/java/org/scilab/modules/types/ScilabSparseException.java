/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - Scilab Enterprises - Calixte DENIZET
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
