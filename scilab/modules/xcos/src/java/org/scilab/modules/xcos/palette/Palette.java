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

import org.scilab.modules.xcos.palette.model.PaletteModel;
import org.scilab.modules.xcos.palette.view.PaletteView;

/**
 * Implement the default palette functionality. These functionality must be
 * similar to those performed by the {@link PaletteDiagram} class.
 */
public final class Palette {
	private PaletteModel model;
	private PaletteView view;
	
	/**
	 * Default constructor
	 * @param category The associated category
	 */
	public Palette(PaletteModel category) {
		model = category;
		view = new PaletteView(this);
	}
	
	/**
	 * @return the model
	 */
	public PaletteModel getModel() {
		return model;
	}
	
	/**
	 * @return the view
	 */
	public PaletteView getView() {
		return view;
	}
	
	/**
	 * Load the icons of the associated category
	 * @return the view
	 */
	public PaletteView loadView() {
		if (!view.isLoaded()) {
			PaletteBlockFactory factory = PaletteBlockFactory.getInstance();

			for (String blockName : model.getBlockNames()) {
				PaletteBlock block = factory.getInstance(blockName);
				block.installDnd();
				view.add(block.getView());
				block.getView().setVisible(true);
			}

			view.setLoaded(true);
		}
		return getView();
	}
	
	/**
	 * @return the localized string message
	 * @see java.lang.Object#toString()
	 */
	@Override
	public String toString() {
		return getModel().toString();
	}
	
	/** @return the Palette list */
	public static Palette[] getDatas() {
		final int length = PaletteModel.values().length;
		Palette[] pal = new Palette[length];

		for (int i = 0; i < length; i++) {
			pal[i] = new Palette(PaletteModel.values()[i]);
		}

		return pal;
	}
}
