
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.window;

import javax.swing.JFrame;
import javax.swing.JMenuBar;

import org.flexdock.docking.DockingManager;
import org.flexdock.docking.DockingPort;
import org.flexdock.docking.defaults.DefaultDockingPort;

import org.scilab.modules.gui.bridge.SwingScilabTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.window.Window;

/**
 * Swing implementation for Scilab windows in GUIs
 * This implementation uses FlexDock package
 * @author Vincent COUVERT
 * @author Bruno JOFRET
 */
public class SwingScilabWindow extends JFrame implements Window {

	private static final int DEFAULTWIDTH = 500;
	private static final int DEFAULTHEIGHT = 500;

	/**
	 * Constructor
	 */
	public SwingScilabWindow() {
		super();

		// TODO : Only for testing : Must be removed
		this.setDims(new Size(DEFAULTWIDTH, DEFAULTHEIGHT));
		this.setTitle("Scilab 5.0");

		/* Create automatically a docking port associated to the window */
		DefaultDockingPort sciDockingPort = new DefaultDockingPort();
		/* The docking port becomes the content panel of the new created window */
		this.setContentPane(sciDockingPort);
	}

	/**
	 * Creates a swing Scilab window
	 * @return the created window
	 */
	public static Window createWindow() {
		return new SwingScilabWindow();
	}

	/**
	 * Draws a swing Scilab window
	 * @see org.scilab.modules.gui.UIElement#draw()
	 */
	public void draw() {
		this.setVisible(true);
		this.doLayout();
	}

	/**
	 * Gets the dimensions (width and height) of a swing Scilab window
	 * @return the dimensions of the window
	 * @see org.scilab.modules.gui.UIElement#getDims()
	 */
	public Size getDims() {
		return new Size(getSize().width, getSize().height);
	}

	/**
	 * Sets the dimensions (width and height) of a swing Scilab window
	 * @param newWindowSize the dimensions to set to the window
	 * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newWindowSize) {
		this.setSize(newWindowSize.getWidth(), newWindowSize.getHeight());
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab window
	 * @return the position of the window
	 * @see org.scilab.modules.gui.UIElement#getPosition()
	 */
	public Position getPosition() {
		return new Position(this.getX(), this.getY());
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab window
	 * @param newWindowPosition the position to set to the window
	 * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newWindowPosition) {
		this.setLocation(newWindowPosition.getX(), newWindowPosition.getY());
	}

	/**
	 * Gets the title of a swing Scilab window
	 * @return the title of the window
	 * @see java.awt.Frame#getTitle(java.lang.String)
	 */
	public String getTitle() {
		return super.getTitle();
	}

	/**
	 * Sets the title of a swing Scilab window
	 * @param newWindowTitle the title to set to the window
	 * @see java.awt.Frame#setTitle(java.lang.String)
	 */
	public void setTitle(String newWindowTitle) {
		super.setTitle(newWindowTitle);
	}

	/**
	 * Gets the docking port associated to the window (created by default at window creation)
	 * @return the docking port associated to the window
	 */
	private DockingPort getDockingPort() {
		return (DockingPort) getContentPane();
	}

	/**
	 * Add a Scilab tab to a Scilab window
	 * @param newTab the Scilab tab to add to the Scilab window
	 * @see org.scilab.modules.gui.window.Window#addTab(org.scilab.modules.gui.tab.Tab)
	 */
	public void addTab(Tab newTab) {
		DockingManager.dock((SwingScilabTab) newTab, this.getDockingPort());
	}
	
	/**
	 * Sets a Scilab MenuBar to a Scilab window
	 * @param newMenuBar the Scilab MenuBar to add to the Scilab window
	 * @see org.scilab.modules.gui.window.Window#setMenuBar(org.scilab.modules.gui.widget.MenuBar)
	 */
	public void setMenuBar(MenuBar newMenuBar) {
		super.setJMenuBar((JMenuBar) newMenuBar);
	}
}
