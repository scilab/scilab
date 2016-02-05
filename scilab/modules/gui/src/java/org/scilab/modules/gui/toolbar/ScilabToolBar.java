/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
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

package org.scilab.modules.gui.toolbar;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.uielement.ScilabUIElement;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Class for Scilab ToolBars in GUIs
 * @author Marouane BEN JELLOUL
 * @author Vincent COUVERT
 */
public class ScilabToolBar extends ScilabUIElement implements ToolBar {

    private SimpleToolBar component;

    /**
     * Constructor
     */
    protected ScilabToolBar() {
        component = ScilabBridge.createToolBar();
    }

    /**
     * Creates a Scilab MenuBar object
     * @return the created MenuBar
     */
    public static ToolBar createToolBar() {
        return new ScilabToolBar();
    }

    /**
     * Gets this Bridge component object
     * @return this Bridge component object
     */
    public SimpleToolBar getAsSimpleToolBar() {
        return component;
    }


    /**
     * Add a separator in the toolbar
     * @see org.scilab.modules.gui.toolbar.ToolBar#addSeparator()
     */
    public void addSeparator() {
        ScilabBridge.addSeparator(this);
    }

    /**
     * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#draw()
     */
    public void draw() {
        throw new UnsupportedOperationException();
    }

    /**
     * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#draw()
     * @return nothing
     */
    public Size getDims() {
        throw new UnsupportedOperationException();
    }

    /**
     * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#draw()
     * @return nothing
     */
    public Position getPosition() {
        throw new UnsupportedOperationException();
    }

    /**
     * Get the visibility of the TooBar
     * @return true if the ToolBar is Visible
     */
    public boolean isVisible() {
        return ScilabBridge.isVisible(this);
    }

    /**
     * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#draw()
     * @param newSize is not used
     */
    public void setDims(Size newSize) {
        throw new UnsupportedOperationException();
    }

    /**
     * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#draw()
     * @param newPosition is not used
     */
    public void setPosition(Position newPosition) {
        throw new UnsupportedOperationException();
    }

    /**
     * Set the visibility of the Toolbar
     * @param newVisibleState true to set the toolbar visible
     */
    public void setVisible(boolean newVisibleState) {
        ScilabBridge.setVisible(this, newVisibleState);
    }
}
