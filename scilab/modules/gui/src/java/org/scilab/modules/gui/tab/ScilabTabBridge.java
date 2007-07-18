
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.tab;

import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.frame.Frame;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Bridge for Scilab tabs in GUIs
 * @author Vincent COUVERT
 */
public class ScilabTabBridge {

	/**
	 * Constructor
	 */
	protected ScilabTabBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	};

	/**
	 * Creates a Scilab tab
	 * @param name the name of the tab (used to identify it)
	 * @return the created tab
	 */
	public static SimpleTab createTab(String name) {
		return new SwingScilabTab(name);
	}

	/**
	 * Draws a Scilab tab
	 * @param tab the tab to draw
	 */
	public static void draw(Tab tab) {
		tab.getAsSimpleTab().draw();
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab tab
	 * @param tab the tab we want to get the dimensions of
	 * @return the dimensions of the tab
	 */
	public static Size getDims(Tab tab) {
		return tab.getAsSimpleTab().getDims();
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab tab
	 * @param tab the tab we want to set the dimensions of
	 * @param newTabSize the dimensions we want to set to the tab
	 */
	public static void setDims(Tab tab, Size newTabSize) {
		tab.getAsSimpleTab().setDims(newTabSize);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab tab
	 * @param tab the tab we want to get the position of
	 * @return the position of the tab
	 */
	public static Position getPosition(Tab tab) {
		return tab.getAsSimpleTab().getPosition();
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab tab
	 * @param tab the tab we want to set the position of
	 * @param newTabPosition the position we want to set to the tab
	 */
	public static void setPosition(Tab tab, Position newTabPosition) {
		tab.getAsSimpleTab().setPosition(newTabPosition);
	}

	/**
	 * Gets the Name of a Scilab tab
	 * @param tab the tab we want to get the title of
	 * @return the title of the tab
	 */
	public static String getName(Tab tab) {
		return tab.getAsSimpleTab().getName();
	}

	/**
	 * Sets the name of a Scilab tab
	 * @param tab the tab we want to set the name of
	 * @param newTabName the name to set to the tab
	 */
	public static void setName(Tab tab, String newTabName) {
		tab.getAsSimpleTab().setName(newTabName);
	}

	/**
	 * Gets the visibility status of a Siclab tab
	 * @param tab the tab we want to get the visibility status of
	 * @return the visibility status of the tab (true if the tab is visible, false if not)
	 */
	public static boolean isVisible(Tab tab) {
		return tab.getAsSimpleTab().isVisible();
	}

	/**
	 * Sets the visibility status of a Siclab tab
	 * @param tab the tab we want to set the visibility status of
	 * @param newVisibleState the visibility status to set to the tab (true to set the tab visible, false else)
	 */
	public static void setVisible(Tab tab, boolean newVisibleState) {
		tab.getAsSimpleTab().setVisible(newVisibleState);
	}

	/**
	 * Add a member (dockable element) to a tab and returns the index of this member
	 * @param tab the tab where we want to add the member
	 * @param member the member to add
 	 * @return index of member
 	 */
	public static int addMember(Tab tab, Frame member) {
		return tab.getAsSimpleTab().addMember(member);
	}

	/**
	 * Add a member (dockable element) to a tab and returns the index of this member
	 * @param tab the tab where we want to add the member
	 * @param member the member to add
 	 * @return index of member
 	 */
	public static int addMember(Tab tab, Console member) {
		return tab.getAsSimpleTab().addMember(member);
	}

	/**
	 * Add a member (dockable element) to a tab and returns the index of this member
	 * @param tab the tab where we want to add the member
	 * @param member the member to add
 	 * @return index of member
 	 */
	public static int addMember(Tab tab, Canvas member) {
		return (tab.getAsSimpleTab().addMember(member));
	}

}
