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

import javax.swing.JTree;
import javax.swing.SwingUtilities;
import javax.swing.tree.TreePath;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.xcos.graph.PaletteDiagram;
import org.scilab.modules.xcos.palette.model.PaletteManagerModel;
import org.scilab.modules.xcos.palette.view.PaletteManagerView;
import org.scilab.modules.xcos.utils.XcosMessages;

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
	private static final int USER_DEFINED_ROW = 2;

	private static PaletteManager instance;
	
	private PaletteManagerView view;
	private PaletteManagerModel model;
	
	/** Default constructor */
	private PaletteManager() {}
	
	/**
	 * @param view the view to set
	 */
	public void setView(PaletteManagerView view) {
		this.view = view;
	}

	/**
	 * @param model the model to set
	 */
	public void setModel(PaletteManagerModel model) {
		this.model = model;
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
			instance.setModel(new PaletteManagerModel(instance));
			instance.setView(new PaletteManagerView(instance));
			instance.getModel().loadTree();
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
	public void loadUserPalette(String fileName) {
		PaletteDiagram diagram = new PaletteDiagram();
		diagram.installListeners();
		if (diagram.openDiagramAsPal(fileName)) {
			getModel().addUserDefinedNode(diagram);
			
			JTree tree = getView().getTree();
			tree.revalidate();
			tree.expandRow(USER_DEFINED_ROW);
			tree.setSelectionPath(
					new TreePath(getModel().getUserNode(diagram).getPath()));
		}
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
