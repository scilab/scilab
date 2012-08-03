/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
            super(message + "\nCode: " +errorCode);
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
