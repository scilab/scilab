/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Bruno JOFRET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.gui.window;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.uielement.ScilabUIElement;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.utils.UIElementMapper;
import org.scilab.modules.gui.menubar.MenuBar;

/**
 * Class for Scilab Windows in GUIs
 * @author Vincent COUVERT
 * @author Bruno JOFRET
 */
public class ScilabWindow extends ScilabUIElement implements Window {

    private SimpleWindow component;

    /**
     * Constructor
     */
    protected ScilabWindow() {
        component = ScilabBridge.createWindow();
        component.setElementId(UIElementMapper.add(this));

        super.addMenuBar(null);
        super.addToolBar(null);
        super.addInfoBar(null);
    }

    /**
     * Creates a Scilab window object
     * @return the created window
     */
    public static Window createWindow() throws java.awt.HeadlessException {
        return new ScilabWindow();
    }

    /**
     * Gets the GUI Window.
     * @return the GUI dummy window.
     */
    public SimpleWindow getAsSimpleWindow() {
        return component;
    }

    /**
     * Draw a Scilab window
     * @see org.scilab.modules.gui.ScilabUIElement#draw()
     */
    public void draw() {
        ScilabBridge.draw(this);
    }

    /**
     * Deiconify the window and put it in front of other window
     */
    public void raise() {
        ScilabBridge.raise(this);
    }

    /**
     * Gets the dimensions (width and height) of a Scilab window
     * @return the dimensions of the window
     * @see org.scilab.modules.gui.UIElement#getDims()
     */
    public Size getDims() {
        return ScilabBridge.getDims(this);
    }

    /**
     * Sets the dimensions of a Scilab window
     * @param newWindowSize the size we want to set to the window
     * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
     */
    public void setDims(Size newWindowSize) {
        ScilabBridge.setDims(this, newWindowSize);
    }

    /**
     * Gets the position (X-coordinate and Y-coordinate) of a Scilab window
     * @return the position of the window
     * @see org.scilab.modules.gui.UIElement#getPosition()
     */
    public Position getPosition() {
        return ScilabBridge.getPosition(this);
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a Scilab window
     * @param newWindowPosition the position we want to set to the window
     * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
     */
    public void setPosition(Position newWindowPosition) {
        ScilabBridge.setPosition(this, newWindowPosition);
    }

    /**
     * Gets the title of a Scilab window
     * @return the title of the window
     * @see org.scilab.modules.gui.window.Window#getTitle()
     */
    public String getTitle() {
        return ScilabBridge.getTitle(this);
    }

    /**
     * Sets the title of a Scilab window
     * @param newWindowTitle the title to set to the window
     * @see org.scilab.modules.gui.window.Window#setTitle(java.lang.String)
     */
    public void setTitle(String newWindowTitle) {
        ScilabBridge.setTitle(this, newWindowTitle);
    }

    /**
     * Gets the visibility status of a Scilab window
     * @return the visibility status of the window (true if the window is visible, false if not)
     * @see org.scilab.modules.gui.UIElement#isVisible()
     */
    public boolean isVisible() {
        return ScilabBridge.isVisible(this);
    }

    /**
     * Sets the visibility status of a Scilab window
     * @param newVisibleState the visibility status we want to set to the window (true to set the window visible, false else)
     * @see org.scilab.modules.gui.UIElement#setVisible(boolean)
     */
    public void setVisible(boolean newVisibleState) {
        ScilabBridge.setVisible(this, newVisibleState);
    }

    /**
     * Add a tab to a Scilab window
     * @param newTab the tab to add to the window
     * @see org.scilab.modules.gui.window.Window#addTab(org.scilab.modules.gui.tab.Tab)
     */
    public void addTab(Tab newTab) {
        ScilabBridge.addTab(this, newTab);
    }

    /**
     * Remove a tab from a Scilab window
     * @param newTab the tab to remove from the window
     * @see org.scilab.modules.gui.window.Window#removeTab(org.scilab.modules.gui.tab.Tab)
     */
    public void removeTab(Tab newTab) {
        ScilabBridge.removeTab(this, newTab);
    }

    /**
     * Sets a MenuBar to a Scilab window
     * @param newMenuBar the tab to add to the window
     * @see org.scilab.modules.gui.window.Window#setMenuBar(org.scilab.modules.gui.widget.MenuBar)
     */
    public void addMenuBar(MenuBar newMenuBar) {
        super.addMenuBar(newMenuBar);
        ScilabBridge.addMenuBar(this, newMenuBar);
    }

    /**
     * Sets a Scilab ToolBar to a Scilab window
     * @param newToolBar the Scilab ToolBar to set to the Scilab window
     * @see org.scilab.modules.gui.window.Window#setToolBar(org.scilab.modules.gui.toolbar.ToolBar)
     */
    public void addToolBar(ToolBar newToolBar) {
        super.addToolBar(newToolBar);
        ScilabBridge.addToolBar(this, newToolBar);
    }

    /**
     * Sets a Scilab InfoBar to a Scilab window
     * @param newInfoBar the Scilab InfoBar to set to the Scilab window
     * @see org.scilab.modules.gui.window.Window#setInfoBar(org.scilab.modules.gui.textbox.TextBox)
     */
    public void addInfoBar(TextBox newInfoBar) {
        super.addInfoBar(newInfoBar);
        ScilabBridge.addInfoBar(this, newInfoBar);
    }

    /**
     * @return number of objects (tabs) docked in this window
     */
    public int getNbDockedObjects() {
        return ScilabBridge.getNbDockedObjects(this);
    }

    /**
     * Update the dimension of the window and its component.
     * Only useful when the window is not yet visible
     */
    public void updateDimensions() {
        ScilabBridge.updateDimensions(this);
    }

    /**
    * Deiconified Window
    */
    public void windowDeiconified() {
        ScilabBridge.windowDeiconified(this);
    }

    /**
    * Iconified Window
    */
    public void windowIconified() {
        ScilabBridge.windowIconified(this);
    }

    /**
    * Maximized Window
    */
    public void windowMaximized() {
        ScilabBridge.windowMaximized(this);
    }

    /**
    * Window is in the "normal" state.
    */
    public void windowNormal() {
        ScilabBridge.windowNormal(this);
    }
}
