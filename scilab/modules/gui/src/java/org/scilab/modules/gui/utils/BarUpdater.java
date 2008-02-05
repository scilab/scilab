package org.scilab.modules.gui.utils;

import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.uielement.UIElement;
import org.scilab.modules.gui.window.Window;

/**
 * @author Bruno JOFRET - INRIA 2007
 *
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
	 * @param newWindowTitle the new Title to display
	 */
	public static void updateBars(int parentWindowsID, MenuBar newMenuBar, ToolBar newToolBar, String newWindowTitle) {
		UIElement element = UIElementMapper.getCorrespondingUIElement(parentWindowsID);
		if (element != null) {
			element.addMenuBar(newMenuBar);
			element.addToolBar(newToolBar);
			((Window) element).setTitle(newWindowTitle);
		}
	}
}
