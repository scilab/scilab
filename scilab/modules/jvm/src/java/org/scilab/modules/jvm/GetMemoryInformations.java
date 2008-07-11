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


package org.scilab.modules.jvm;

/**
 * Class containing routines giving informations about the Java memory
 * @author Jean-Baptiste Silvy
 */
public final class GetMemoryInformations {

	/**
	 * Should not be called
	 */
	protected GetMemoryInformations() {
		
	}
	
	/**
	 * Get the amount of memory java can allocate.
	 * @return Available memory size in byte.
	 */
	public static long getAvailableJavaMemory() {
		Runtime runtime = Runtime.getRuntime();
		long possibleAllocation = runtime.maxMemory() - runtime.totalMemory();
		long freeHeapSpace = runtime.freeMemory();
		return possibleAllocation + freeHeapSpace;

	}
}
