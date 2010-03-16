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
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreeNode;
import javax.swing.tree.TreePath;

import org.scilab.modules.xcos.palette.model.PaletteNode;
import org.scilab.modules.xcos.palette.view.PaletteConfiguratorView;

/**
 * A rawPalette configurator is used to configure any rawPalette state. It can
 * easily enable and disable rawPalette.
 * 
 * By now the implementation is suboptimal, it needs configuration saving then
 * loading to perform activation/deactivation.
 */
public class PaletteConfigurator {
	private final PaletteConfiguratorView view;
	private final PaletteNode palette;
	
	/**
	 * Default constructor
	 * @param palette the associated palette
	 */
	public PaletteConfigurator(PaletteNode palette) {
		this.palette = palette;
		view = new PaletteConfiguratorView(this);
		
		updateCheckedStatus();
	}

	/**
	 * @param palette the palette to work on
	 */
	private void updateCheckedStatus() {
		boolean enabled = palette.isEnable();
		view.setChecked(enabled);
	}

	/**
	 * @return the associated palette
	 */
	public Object getPalette() {
		return palette;
	}
	
	/**
	 * @return The associated view
	 */
	public PaletteConfiguratorView getView() {
		return view;
	}
	
	/**
	 * @param enable the enable state
	 */
	public void setEnable(boolean enable) {
		boolean enabled = palette.isEnable();
		if (enabled != enable) {
			palette.setEnable(enable);
			PaletteManager.getInstance().saveConfig();
			
			final JTree t = PaletteManager.getInstance().getView().getTree();
			final TreePath p = t.getSelectionPath();
			
			((DefaultTreeModel) t.getModel()).reload((TreeNode) p.getLastPathComponent());
		}
	}
}
