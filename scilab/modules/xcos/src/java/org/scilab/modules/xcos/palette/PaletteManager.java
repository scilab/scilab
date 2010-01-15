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

import javax.swing.SwingUtilities;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.xcos.palette.model.PaletteManagerModel;
import org.scilab.modules.xcos.palette.view.PaletteManagerView;

/**
 * Main class for the palette management.
 * 
 * Each palette can be statically defined or dynamically defined at startup
 * (following configuration) or dynamically defined by the user. If the
 * definition is static then the palette must appear on {@link Palette} with its
 * name and block components. Otherwise the palette correspond to a specific
 * diagram representation implemented on {@link PaletteDiagram}.
 */
public final class PaletteManager {
	private static PaletteManager instance;
	
	private PaletteManagerView view;
	private PaletteManagerModel model;
	
	/** Default constructor */
	private PaletteManager() {
		model = new PaletteManagerModel();
		view = new PaletteManagerView(this);
	}

	/** @return the view */
	public PaletteManagerView getView() {
		return view;
	}
	
	/** @return the model */
	public PaletteManagerModel getModel() {
		return model;
	}

	/** @return the default instance */
	public static PaletteManager getInstance() {
		if (instance == null) {
			instance = new PaletteManager();
		}
		return instance;
	}
	
	/** @return true if the palette window is visible, false otherwise */
	public static boolean isVisible() {
		return getInstance().getView().isVisible();
	}
	
	/**
	 * Set visible or hide the palette.
	 * @param status true to set visible, false to hide.
	 */
	public static void setVisible(boolean status) {
		getInstance().getView().setVisible(status);
	}
	
	/**
	 * Load a user diagram on the palette
	 * @param fileName File name and path to the diagram
	 */
	public static void loadUserPalette(String fileName) {
		
	}
	
	/**
	 * For debugging purpose
	 * @param args Non used
	 */
	public static void main(String[] args) {
		InterpreterManagement.requestScilabExec("");
		SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				setVisible(true);
			}
		});
	}
}
