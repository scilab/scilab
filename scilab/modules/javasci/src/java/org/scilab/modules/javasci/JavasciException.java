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


public abstract class JavasciException extends Exception {


	public static class InitializationException extends JavasciException {

		public InitializationException(String message) {
			super(message);
		}

		public InitializationException(String message, Throwable cause) {
			super(message, cause);
		}
		
	}

	public static class UnsupportedTypeException extends JavasciException {

		public UnsupportedTypeException(String message) {
			super(message);
		}

	}

	public static class ScilabInternalException extends JavasciException {

		public ScilabInternalException(String message) {
			super(message);
		}

	}

	public static class UnknownTypeException extends JavasciException {

		public UnknownTypeException(String message) {
			super(message);
		}

	}

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
