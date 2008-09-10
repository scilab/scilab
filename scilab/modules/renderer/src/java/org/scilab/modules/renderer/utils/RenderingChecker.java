/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : GLEventListener used to retrieve informations from the
 * canvas
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.utils;

/**
 * Class containg checking function wich test wether we will be able to
 * perform some operations
 * @author Jean-Baptiste Silvy
 */
public final class RenderingChecker {
	
	/** Assume maxomum needed memory to create a graphic window is 20Mb for now */
	private static final long CREATING_WINDOW_NEEDED_MEMORY = (long) 20e6;
	
	/**
	 * Should not be called
	 */
	protected RenderingChecker() { }
	
	/**
	 * Check wether the Java environement allows to create a new window
	 * @return true if the creation is OK, false otherwise
	 */
	public static boolean isAbleToCreateWindow() {
		if (RenderingCapabilities.getAvailableJavaMemory() < CREATING_WINDOW_NEEDED_MEMORY) {
			// try to reduce memory usage by calling the gc
			System.gc();
			// check again
			if (RenderingCapabilities.getAvailableJavaMemory() < CREATING_WINDOW_NEEDED_MEMORY) {
				// still not enough memory
				return false;
			}
		}
		
		// ok!!
		return true;
	}
	
	
	
}
