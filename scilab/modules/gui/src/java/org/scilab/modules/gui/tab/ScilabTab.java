
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.tab;

import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.container.ScilabContainer;
import org.scilab.modules.gui.dockable.Dockable;
import org.scilab.modules.gui.frame.Frame;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.utils.UIElementMapper;

/**
 * Class for tabs in Scilab GUIs
 * @author Vincent COUVERT
 */
public class ScilabTab extends ScilabContainer implements Tab {

	/**
	**	Just let the Bridge do his job translating Scilab thinking to Java
	*/
	private SimpleTab component;
	
	/**
	 * Constructor
	 * @param name - name of the Tab
	 */
	protected ScilabTab(String name) {
		component = ScilabBridge.createTab(name);
		component.setElementId(UIElementMapper.add(this));
		
		setMenuBarId(UIElementMapper.getDefaultId());
		setToolBarId(UIElementMapper.getDefaultId());
   }

	/**
	 * Creates a Scilab Tab
	 * @param name the name of the tab (used to identify it)
	 * @return the created Scilab Tab
	 */
	public static Tab createTab(String name) {
		return new ScilabTab(name);
	}

	/**
	 * Gets the GUI Tab.
	 * @return The GUI dummy Tab.
	 */
	public SimpleTab getAsSimpleTab() {
		return component;
	}

	/**
	 * Gets the title of a Scilab tab
	 * @return the title of the frame
	 * @see org.scilab.modules.gui.tab.Tab#getTitle()
	 */
	public String getName() {
		return ScilabBridge.getName(this);
	}

	/**
	 * Sets the name of a Scilab tab
	 * @param newTabName the name to set to the tab
	 * @see org.scilab.modules.gui.tab.Tab#setName(java.lang.String)
	 */
	public void setName(String newTabName) {
		ScilabBridge.setName(this, newTabName);
	}

	/**
	 * Draws a Scilab tab
	 * @see org.scilab.modules.gui.UIElement#draw()
	 */
	public void draw() {
		ScilabBridge.draw(this);
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab tab
	 * @return the dimension of the tab
	 * @see org.scilab.modules.gui.UIElement#getDims()
	 */
	public Size getDims() {
		return ScilabBridge.getDims(this);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab tab
	 * @return the position of the tab
	 * @see org.scilab.modules.gui.UIElement#getPosition()
	 */
	public Position getPosition() {
		return ScilabBridge.getPosition(this);
	}

	/**
	 * Gets the visibility status of a Scilab frame
	 * @return the visibility status of the frame (true if the frame is visible, false if not)
	 * @see org.scilab.modules.gui.UIElement#isVisible()
	 */
	public boolean isVisible() {
		return ScilabBridge.isVisible(this);
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab frame
	 * @param newSize the dimensiosn we want to set to the tab
	 * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		ScilabBridge.setDims(this, newSize);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab frame
	 * @param newPosition the position we want to set to the tab
	 * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		ScilabBridge.setPosition(this, newPosition);
	}

	/**
	 * Sets the visibility status of a Scilab frame
	 * @param newVisibleState the visibility status we want to set to the frame (true to set the frame visible, false else)
	 * @see org.scilab.modules.gui.UIElement#setVisible(boolean)
	 */
	public void setVisible(boolean newVisibleState) {
		ScilabBridge.setVisible(this, newVisibleState);
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(Canvas member) {
		return ScilabBridge.addMember(this, member);
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(Console member) {
		UIElementMapper.setConsoleId(this.getAsSimpleTab().getElementId());
		return ScilabBridge.addMember(this, member);
	}
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 * @see org.scilab.modules.gui.container.Container#addMember(org.scilab.modules.gui.dockable.Dockable)
	 */
	@Override
	public int addMember(Dockable member) {
		// FIXME must be coded
		//return addMember(member);
		throw new UnsupportedOperationException();
	}

	/**
	 * Sets a MenuBar to a Scilab tab
	 * @param newMenuBar the tab to add to the tab
	 * @see org.scilab.modules.gui.tab.Tab#setMenuBar(org.scilab.modules.gui.widget.MenuBar)
	 */
	public void addMenuBar(MenuBar newMenuBar) {
		super.addMenuBar(newMenuBar);
	}
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(Frame member) {
		return ScilabBridge.addMember(this, member);
	}

	/**
	 * We want to be able to add directly a pushbutton in a Tab.
	 * @param member the pushbutton to add
	 * @return the position of the pushbutton in the member list.
	 */
	public int addMember(PushButton member) {
		return ScilabBridge.addMember(this, member);
	}

	/**
	 * Get the current status of the Tab in its parent
	 * @return true is the tab is the tab currently "on top" in its parent
	 * @see org.scilab.modules.gui.tab.Tab#isCurrentTab()
	 */
	public boolean isCurrentTab() {
		return ScilabBridge.isCurrentTab(this);
	}

	/**
	 * Set the parent window id for this tab
	 * @param id the id of the parent window
	 * @see org.scilab.modules.gui.tab.Tab#setParentWindowId(int)
	 */
	public void setParentWindowId(int id) {
		ScilabBridge.setParentWindowId(this, id);
	}
	
	/**
	 * Get the parent window id for this tab
	 * @return the id of the parent window
	 * @see org.scilab.modules.gui.tab.Tab#getParentWindowId()
	 */
	public int getParentWindowId() {
		return ScilabBridge.getParentWindowId(this);
	}
}
