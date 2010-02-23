/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - Calixte Denizet
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer.utils.textRendering;


/**
 * Special Text Exception thrown when expression isn't in MathML or LaTeX code
 * @author Calixte Denizet
 */
public class SpecialTextException extends Exception {
    
	/**
	 * Constructor
	 * @param msg message
	 */
    public SpecialTextException(String msg) {
	super(msg);
    }
}