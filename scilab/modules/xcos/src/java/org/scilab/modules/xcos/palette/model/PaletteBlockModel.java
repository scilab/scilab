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

package org.scilab.modules.xcos.palette.model;

import javax.swing.ImageIcon;


/**
 * Represent a block model in the palette
 */
public final class PaletteBlockModel {
	private static final String ICON_PATH = System.getenv("SCI") + "/modules/xcos/images/palettes/";
	private static final String ICON_EXTENSION = ".jpg";
	
	private final String name;
	private final ImageIcon icon;
	
	/**
	 * Instantiate a palette block
	 * @param name Name of the block
	 */
	public PaletteBlockModel(String name) {
		this.name = name;
		this.icon = new ImageIcon(ICON_PATH + name + ICON_EXTENSION);
	}

	/**
	 * @return the name
	 */
	public String getName() {
		return name;
	}

	/**
	 * @return the icon
	 */
	public ImageIcon getIcon() {
		return icon;
	}
	
	/**
	 * Load all the block in the category
	 * @param cat the category to load
	 * @return All the associated models
	 */
	public static PaletteBlockModel[] loadCategory(PaletteModel cat) {
		final String[] names = cat.getBlockNames();
		final int blockNumber = names.length;
		PaletteBlockModel[] models = new PaletteBlockModel[blockNumber];
		
		for (int i = 0; i < blockNumber; i++) {
			models[i] = new PaletteBlockModel(names[i]);
		}
		return models;
	}
}
