/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - INRIA - Sylvestre LEDRU
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

package org.scilab.modules.javasci;

/**
 * This class provides a general exception class for Javasci v2 API.
 */
public abstract class JavasciException extends Exception {


    /**
     * This class provides a exception class when an error occurs on the
     * initialization
     */
    public static class InitializationException extends JavasciException {

        public InitializationException(String message) {
            super(message);
        }

        public InitializationException(String message, Throwable cause) {
            super(message, cause);
        }

    }


    /**
     * This class provides a exception class when an unsupported type is used
     */
    public static class UnsupportedTypeException extends JavasciException {

        public UnsupportedTypeException(String message) {
            super(message);
        }
    }


    /**
     * This class provides a exception class when a type is unknown
     * This should be very very rare.
     */
    public static class UnknownTypeException extends JavasciException {

        public UnknownTypeException(String message) {
            super(message);
        }

    }

    /**
     * This class provides a exception class when Scilab is already running
     */
    public static class AlreadyRunningException extends JavasciException {

        public AlreadyRunningException(String message) {
            super(message);
        }
    }


    /**
     * This class provides a exception class when an internal error occurs
     */
    public static class ScilabInternalException extends JavasciException {

        public ScilabInternalException(String message) {
            super(message);
        }

    }

    /**
     * This class provides a exception class when a Scilab error occurs
     * @since 5.4.0
     */
    public static class ScilabErrorException extends JavasciException {

        public ScilabErrorException(String message, int errorCode) {
            super(message + "\nCode: " + errorCode);
        }

    }


    /**
     * This class provides a exception class when an non-existing variable is
     * accessed.
     */
    public static class UndefinedVariableException extends JavasciException {

        public UndefinedVariableException(String message) {
            super(message);
        }

    }

    /**
     * Default constructor
     */
    public JavasciException() {
        super();
    }

    /**
     * Default constructor
     */
    public JavasciException(String message) {
        super(message);
    }

    /**
     *
     * @param message the message to be printed
     * @param cause the cause
     */
    public JavasciException(String message, Throwable cause) {
        super(message, cause);
    }


}
