/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.palette.actions;

import java.awt.event.ActionEvent;
import java.util.ArrayList;
import java.util.List;

import org.flexdock.docking.Dockable;
import org.flexdock.docking.DockingManager;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.palette.XcosPaletteManager;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Palette Tab visibility management
 */
public final class ViewPaletteBrowserAction extends DefaultAction {
	/** Name of the action */
	public static final String NAME = XcosMessages.PALETTE_BROWSER;
	/** Icon name of the action */
	public static final String SMALL_ICON = "";
	/** Mnemonic key of the action */
	public static final int MNEMONIC_KEY = 0;
	/** Accelerator key for the action */
	public static final int ACCELERATOR_KEY = 0;

    private static List<CheckBoxMenuItem> menus = new ArrayList<CheckBoxMenuItem>();

    /**
     * Constructor
     * 
     * @param scilabGraph associated Scilab Graph
     */
    public ViewPaletteBrowserAction(ScilabGraph scilabGraph) {
    	super(scilabGraph);
    }

    /**
     * Create checkbox menu for the graph menu bar
     * 
     * @param scilabGraph associated Scilab Graph
     * @return the menu
     */
    public static CheckBoxMenuItem createCheckBoxMenu(ScilabGraph scilabGraph) {
    	CheckBoxMenuItem menu = createCheckBoxMenu(scilabGraph, ViewPaletteBrowserAction.class);
    	menus.add(menu);
    	return menu;
    }

	/**
	 * @param e parameter
	 * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
	 */
	@Override
	public void actionPerformed(ActionEvent e) {
	if (XcosPaletteManager.getPalettes() == null) {
	    XcosPaletteManager.loadPalette();
	} else {
	    setPalettesVisible(!XcosPaletteManager.isVisible());
	}
    }

    /**
     * Set the visibility of the palette Window and Tab Update all menus
     * "View/Palette browser"
     * 
     * @param status status to be set
     */
    public static void setPalettesVisible(boolean status) {
	Tab palette = Xcos.getPalettes();
	if (palette == null) {
	    return;
	}

	// If palette has no more parent, create a new one.
	if (status && palette.getParentWindow() == null) {
	    Window paletteWindow = ScilabWindow.createWindow();
	    paletteWindow.setVisible(true);
	    palette.setVisible(true);
	    paletteWindow.addTab(palette);
	}

	// Hide/Show parent window if the viewport is the only tab
	if (palette.getParentWindow() != null) {
	    if (palette.getParentWindow().getNbDockedObjects() == 1) {
		palette.getParentWindow().setVisible(status);
	    } else {
		DockingManager.undock((Dockable) palette.getAsSimpleTab());
		palette.setParentWindowId(-1);
	    }
	}

	// Hide/Show viewport tab
	palette.setVisible(status);
	// Check/Uncheck the menu
	for (int i = (menus.size() - 1); i >= 0; i--) {
	    menus.get(i).setChecked(status);
	}
    }
}
