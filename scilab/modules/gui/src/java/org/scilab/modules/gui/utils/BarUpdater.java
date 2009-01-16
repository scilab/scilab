/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Bruno JOFRET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.utils;

import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.uielement.UIElement;
import org.scilab.modules.gui.window.Window;

/**
 * @author Bruno JOFRET
 */
public final class BarUpdater {
	
	/**
	 * Constructor
	 */
	private BarUpdater() {
		throw new UnsupportedOperationException();
	}
	
	/**
	 * Local update for MenuBar and ToolBar
	 * Called when a Dock is complete.
	 * @param parentWindowsID : the ID of the window we want to update.
	 * @param newMenuBar the new MenuBar to display.
	 * @param newToolBar the new ToolBar to display.
	 * @param newInfoBar the new InfoBar to display.
	 * @param newWindowTitle the new Title to display
	 */
	public static void updateBars(int parentWindowsID, MenuBar newMenuBar, ToolBar newToolBar, TextBox newInfoBar, String newWindowTitle) {
		UIElement element = UIElementMapper.getCorrespondingUIElement(parentWindowsID);
		if (element != null) {
			element.addMenuBar(newMenuBar);
			element.addToolBar(newToolBar);
			element.addInfoBar(newInfoBar);
			((Window) element).setTitle(newWindowTitle);
			/** The following line is used to update the menubar, toolbar, ... displayed on screen */
			((SwingScilabWindow) ((Window) element).getAsSimpleWindow()).validate();
		}
	}
}
