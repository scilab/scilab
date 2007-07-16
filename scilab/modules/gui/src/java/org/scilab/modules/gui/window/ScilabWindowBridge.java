
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.window;

import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.toolbar.ToolBarBridge;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.menubar.MenuBarBridge;

/**
 * Bridge for Scilab windows in GUIs
 * @author Vincent COUVERT
 * @author Bruno JOFRET
 * @author Marouane BEN JELLOUL
 */
public class ScilabWindowBridge {

	/**
	 * Constructor
	 */
	protected ScilabWindowBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	};

	/**
	 * Draws a Scilab window
	 * @param window the window to draw
	 */
	public static void draw(WindowBridge window) {
		window.draw();
	}

	/**
	 * Creates a Scilab window
	 * @return the created window
	 */
	public static WindowBridge createWindow() {
		return new SwingScilabWindow();
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab window
	 * @param window the window we want to get the dimensions of
	 * @return the dimensions of the window
	 */
	public static Size getDims(WindowBridge window) {
		return window.getDims();
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab window
	 * @param window the window we want to set the dimensions of
	 * @param newWindowSize the dimensions we want to set to the window
	 */
	public static void setDims(WindowBridge window, Size newWindowSize) {
		window.setDims(newWindowSize);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab window
	 * @param window the window we want to get the position of
	 * @return the position of the window
	 */
	public static Position getPosition(WindowBridge window) {
		return window.getPosition();
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab window
	 * @param window the window we want to set the position of
	 * @param newWindowPosition the position to set to the window
	 */
	public static void setPosition(WindowBridge window, Position newWindowPosition) {
		window.setPosition(newWindowPosition);
	}

	/**
	 * Gets the title of a Scilab windo
	 * @param window the window we want to get the title of
	 * @return the title of the window
	 */
	public static String getTitle(WindowBridge window) {
		return window.getTitle();
	}

	/**
	 * Sets the title of a Scilab window
	 * @param window the window we want to set the title of
	 * @param newWindowTitle the title we want to set to the window
	 */
	public static void setTitle(WindowBridge window, String newWindowTitle) {
		window.setTitle(newWindowTitle);
	}

	/**
	 * Gets the visibility status of a Scilab window
	 * @param window the window we want to getthe visibility status of
	 * @return the visibility status of the window (true if the window is visible, false if not)
	 */
	public static boolean isVisible(WindowBridge window) {
		return window.isVisible();
	}

	/**
	 * Sets the visibility status of a Scilab window
	 * @param window the window we want to set the visibility status of
	 * @param newVisibleState the visibility status we want to set to the window (true to set the window visible, false else)
	 */
	public static void setVisible(WindowBridge window, boolean newVisibleState) {
		window.setVisible(newVisibleState);
	}

	/**
	 * Add a tab to a Scilab window
	 * @param window the window which we want to add the tab to
	 * @param newTab the tab to add to the window
	 */
	// TODO uncomment when TabBridge ready
//	public static void addTab(WindowBridge window, TabBridge newTab) {
//		window.addTab(newTab);
//	}
	
	/**
	 * Sets a MenuBar to a Scilab window
	 * @param window the window which we want to add the MenuBar to
	 * @param newMenuBar the MenuBar to add to the window
	 */
	public static void setMenuBar(WindowBridge window, MenuBarBridge newMenuBar) {
		window.setMenuBar(newMenuBar);
	}
	
	/**
	 * Sets a ToolBar to a Scilab window
	 * @param window the window which we want to add the newToolBar to
	 * @param newToolBar the ToolBar to add to the window
	 */
	public static void setToolBar(WindowBridge window, ToolBarBridge newToolBar) {
		window.setToolBar(newToolBar);
	}
}
