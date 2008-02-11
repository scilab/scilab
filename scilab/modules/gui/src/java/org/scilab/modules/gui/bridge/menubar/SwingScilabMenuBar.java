
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.menubar;

import javax.swing.JMenuBar;

import org.scilab.modules.gui.bridge.menu.SwingScilabMenu;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menubar.SimpleMenuBar;

/**
 * Swing implementation for Scilab MenuBars in GUIs
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabMenuBar extends JMenuBar implements SimpleMenuBar {

	private static final long serialVersionUID = 1L;

	private int elementId;
	
	/**
	 * Constructor
	 */
	public SwingScilabMenuBar() {
		super();
	}

	/**
	 * Append a Menu to a MenuBar
	 * @param newMenu the Menu to add to the MenuBar
	 * @see org.scilab.modules.gui.widget.MenuBar#add(org.scilab.modules.gui.widget.ContextMenu)
	 */
	public void add(Menu newMenu) {
		this.add((SwingScilabMenu) newMenu.getAsSimpleMenu());
	}
	
	/**
	 * Append a Menu to a MenuBar
	 * @param newMenu the Menu to add to the MenuBar
	 * @see org.scilab.modules.gui.widget.MenuBar#add(org.scilab.modules.gui.widget.ContextMenu)
	 */
	public void add(SwingScilabMenu newMenu) {
		super.add(newMenu);
	}

	/**
	 * Get the element id for this menubar
	 * @return id the id of the corresponding menubar object
	 */
	public int getElementId() {
		return this.elementId;
	}

	/**
	 * Set the element id for this menubar
	 * @param id the id of the corresponding menubar object
	 */
	public void setElementId(int id) {
		this.elementId = id;
	}
	
	/**
	 * Enabale/Disable a menu giving its name
	 * @param menuName the name of the menu
	 * @param status true to enable the menu
	 */
	public void setMenuEnabled(String menuName, boolean status) {
		
		for (int menuIndex = 0; menuIndex < this.getMenuCount(); menuIndex++) {
			// Check the name of each menu until one matches the name
			if (this.getMenu(menuIndex).getText().equals(menuName)) {
				this.getMenu(menuIndex).setEnabled(status);
				break;
			}
		}
		
	}

	/**
	 * Remove a menu giving its name
	 * @param menuName the name of the menu
	 */
	public void removeMenu(String menuName) {

		for (int menuIndex = 0; menuIndex < this.getMenuCount(); menuIndex++) {
			// Check the name of each menu until one matches the name
			if (this.getMenu(menuIndex).getText().equals(menuName)) {
				this.remove(menuIndex);
				// Redraw the menuBar to display the changes
				this.repaint();
				this.doLayout();
				break;
			}
		}
		
	}
}
