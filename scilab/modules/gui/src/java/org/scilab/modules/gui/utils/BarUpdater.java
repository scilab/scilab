package org.scilab.modules.gui.utils;

import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.uielement.UIElement;

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
		UIElement element = UIElementMapper.getCorrespondingUIElement(parentWindowsID);
		if(element != null) {
			element.addMenuBar(newMenuBar);
			element.addToolBar(newToolBar);
		}
	}
}
