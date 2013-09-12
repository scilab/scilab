/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.forge.scirenderer;

/**
 * The SciRendererException is the superclass of all Exception from SciRenderer.
 *
 * @author Pierre Lando
 */
@SuppressWarnings(value = { "serial" })
public class SciRendererException extends Exception {

    /**
     * Constructs a new exception with null as its detail message.
     */
    public SciRendererException() {
        super();
    }

    /**
     * Constructs a new exception with the specified detail message.
     * @param message - the detail message. The detail message is saved for later retrieval by the #Throwable.getMessage() method.
     */
    public SciRendererException(String message) {
        super(message);
    }

    /**
     * Constructs a new exception with the specified detail message and cause.
     * @param message - the detail message (which is saved for later retrieval by the Throwable.getMessage() method).
     * @param cause - the cause (which is saved for later retrieval by the Throwable.getCause() method). (A null value is permitted, and indicates that the cause is nonexistent or unknown.)
     */
    public SciRendererException(String message, Throwable cause) {
        super(message, cause);
    }

    /**
     * Constructs a new exception with the specified cause and a detail message of (cause==null ? null : cause.toString()) (which typically contains the class and detail message of cause).
     * @param cause - the cause (which is saved for later retrieval by the Throwable.getCause() method). (A null value is permitted, and indicates that the cause is nonexistent or unknown.)
     */
    public SciRendererException(Throwable cause) {
        super(cause);
    }
}
