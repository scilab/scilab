/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class used to manage java threads from C++ code  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.utils.graphicSynchronization;

/**
 * Class used to manage java threads from C++ code
 * @author Jean-Baptiste Silvy
 */
public class GraphicSynchronizerJava {

	/**
	 * Default constructor
	 */
	public GraphicSynchronizerJava() {
		super();
	}
	
	/**
	 * @return Id of current thread
	 */
	public int getCurrentThreadId() {
		return (int) Thread.currentThread().getId();
	}
	
}
