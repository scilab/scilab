package org.scilab.modules.gui.utils;

import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.toolbar.ToolBar;

/**
 * @author Bruno JOFRET - INRIA 2007
 *
 */
public final class BarUpdater {
	/**
	 * Local update for MenuBar and ToolBar
	 * Called when a Dock is complete.
	 * @param parentWindowsID : the ID of the window we want to update.
	 * @param newMenuBar : the new MenuBar to display.
	 * @param newToolBar : the new ToolBar to display.
	 */
	public static void updateBars(int parentWindowsID, MenuBar newMenuBar, ToolBar newToolBar) {
		UIElementMapper.getCorrespondingUIElement(parentWindowsID).addMenuBar(newMenuBar);
		UIElementMapper.getCorrespondingUIElement(parentWindowsID).addToolBar(newToolBar);
	}
}
