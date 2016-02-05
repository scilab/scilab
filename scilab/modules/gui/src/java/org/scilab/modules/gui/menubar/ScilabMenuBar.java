/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
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

package org.scilab.modules.gui.menubar;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.uielement.ScilabUIElement;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Class for Scilab MenuBars in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabMenuBar extends ScilabUIElement implements MenuBar {

    private SimpleMenuBar component;

    /**
     * Constructor
     */
    protected ScilabMenuBar() {
        component = ScilabBridge.createMenuBar();
    }

    /**
     * Creates a Scilab MenuBar object
     * @return the created MenuBar
     */
    public static MenuBar createMenuBar() {
        return new ScilabMenuBar();
    }

    /**
     * Gets this Bridge component object
     * @return this Bridge component object
     */
    public SimpleMenuBar getAsSimpleMenuBar() {
        return component;
    }

    /**
     * Add a Menu to a MenuBar
     * @param newMenu the Menu to add to the MenuBar
     * @see org.scilab.modules.gui.widget.MenuBar#add(org.scilab.modules.gui.widget.ContextMenu)
     */
    public void add(Menu newMenu) {
        ScilabBridge.add(this, newMenu);
    }

    /**
     * Should not be used, just here to implement org.scilab.modules.gui.UIElement#draw()
     */
    public void draw() {
        throw new UnsupportedOperationException();
    }

    /**
     * Should not be used, just here to implement org.scilab.modules.gui.UIElement#draw()
     * @return nothing
     */
    public Size getDims() {
        throw new UnsupportedOperationException();
    }

    /**
     * Should not be used, just here to implement org.scilab.modules.gui.UIElement#draw()
     * @return nothing
     */
    public Position getPosition() {
        throw new UnsupportedOperationException();
    }

    /**
     * Should not be used, just here to implement org.scilab.modules.gui.UIElement#draw()
     * @return nothing
     */
    public boolean isVisible() {
        throw new UnsupportedOperationException();
    }

    /**
     * Should not be used, just here to implement org.scilab.modules.gui.UIElement#draw()
     * @param newSize is not used
     */
    public void setDims(Size newSize) {
        throw new UnsupportedOperationException();
    }

    /**
     * Should not be used, just here to implement org.scilab.modules.gui.UIElement#draw()
     * @param newPosition is not used
     */
    public void setPosition(Position newPosition) {
        throw new UnsupportedOperationException();
    }

    /**
     * Should not be used, just here to implement org.scilab.modules.gui.UIElement#draw()
     * @param newVisibleState is not used
     */
    public void setVisible(boolean newVisibleState) {
        component.setVisible(newVisibleState);
    }
}
