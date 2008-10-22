/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package javasci;
/**
 * This class manages exceptions from the construction of SciDoubleArray , SciStringArray. 
 * @author Allan Cornet - INRIA 2005
 */
public class BadDataArgumentException extends RuntimeException {
	/**
	 * Constructor of the exception
	 * @param s the error message
	 */
	BadDataArgumentException(String s) {
    super(s);
  }
}

