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

import org.scilab.modules.xcos.graph.PaletteDiagram;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.palette.model.PaletteModel;
import org.scilab.modules.xcos.palette.view.PaletteConfiguratorView;
import org.scilab.modules.xcos.utils.ConfigXcosManager;
import org.scilab.modules.xcos.utils.XcosComponent;

/**
 * A rawPalette configurator is used to configure any rawPalette state. It can
 * easily enable and disable rawPalette.
 * 
 * By now the implementation is suboptimal, it needs configuration saving then
 * loading to perform activation/deactivation.
 */
public class PaletteConfigurator {
	private final PaletteConfiguratorView view;
	private final Object palette;
	
	/**
	 * Default constructor
	 * @param palette the associated rawPalette
	 */
	public PaletteConfigurator(Object palette) {
		this.palette = palette;
		view = new PaletteConfiguratorView(this);
		
		updateCheckedStatus(palette);
	}

	/**
	 * @param palette the palette to work on
	 */
	private void updateCheckedStatus(Object palette) {
		boolean enabled = false;
		if (palette instanceof Palette) {
			enabled = ((Palette) palette).getModel().isEnable();
		} else {
			assert palette instanceof XcosDiagram;
			// non checked diagram are not loaded
			enabled = true;
		}

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
		if (palette instanceof Palette) {
			((Palette) palette).getModel().setEnable(enable);
			ConfigXcosManager.saveDefaultPalettes(PaletteModel.values());
		} else {
			assert palette instanceof XcosComponent;
			PaletteDiagram diagram = (PaletteDiagram) ((XcosComponent) palette).getGraph();
			String fileName = diagram.getFileName();
			if (enable) {
				PaletteManager.getInstance().getModel().addUserDefinedNode(diagram);
				ConfigXcosManager.saveUserDefinedPalettes(fileName);
			} else {
				ConfigXcosManager.removeUserDefinedPalettes(fileName);
			}
		}
		
		PaletteManager.getInstance().getModel().reloadTree();
		PaletteManager.getInstance().getView().getTree().revalidate();
	}
}
