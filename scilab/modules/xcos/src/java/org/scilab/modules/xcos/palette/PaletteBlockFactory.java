/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.palette;

import java.util.HashMap;
import java.util.Map;

/**
 * Create instance of palette blocks. This class is used to share PaletteBlock
 * instances of the same block name (duplicate names on {@link PaletteModel}).
 * 
 * This class implement the non-thread-safe singleton pattern
 */
public final class PaletteBlockFactory {
	private static PaletteBlockFactory sharedInstance;
	
	private Map<String, PaletteBlock> instances;
	
	/** Default constructor */
	private PaletteBlockFactory() {
		instances = new HashMap<String, PaletteBlock>();
	}

	/**
	 * Get the blockName named block. A new block will be instantiated if it had
	 * not already been.
	 * 
	 * @param blockName The block name
	 * @return The palette block
	 */
	public PaletteBlock getInstance(String blockName) {
		PaletteBlock instance;
		
		if (instances.containsKey(blockName)) {
			instance = instances.get(blockName);
		} else {
			instance = new PaletteBlock(blockName);
			instances.put(blockName, instance);
		}
		
		return instance;
	}
	
	/** @return the global shared instance */
	public static PaletteBlockFactory getInstance() {
		if (sharedInstance == null) {
			sharedInstance = new PaletteBlockFactory();
		}
		return sharedInstance;
	}
}
