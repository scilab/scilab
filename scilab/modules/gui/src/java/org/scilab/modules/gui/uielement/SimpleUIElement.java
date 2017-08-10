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
package org.scilab.modules.gui.uielement;

import org.scilab.modules.gui.menubar.SimpleMenuBar;
import org.scilab.modules.gui.textbox.SimpleTextBox;
import org.scilab.modules.gui.toolbar.SimpleToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Interface for UIElementBridge the associated object to Scilab GUIs UIElement
 * @author Marouane BEN JELLOUL
 */
public interface SimpleUIElement {

    /**
     * Get the Id of the element in the UIElementMapper
     * @return the Id of the UIElement
     */
    int getElementId();

    /**
     * Set the Id of the element in the UIElementMapper
     * @param id the Id of the UIElement
     */
    void setElementId(int id);

    /**
     * Sets a MenuBar to a window
     * @param newMenuBar the MenuBar to add to the window
     */
    void addMenuBar(SimpleMenuBar newMenuBar);

    /**
     * Sets a ToolBar to a window
     * @param newToolBar the ToolBar to set to the window
     */
    void addToolBar(SimpleToolBar newToolBar);

    /**
     * Sets a InfoBar to a window
     * @param newInfoBar the InfoBar to set to the window
     */
    void addInfoBar(SimpleTextBox newInfoBar);

    //	public abstract void resize();
    //
    //	public abstract void focus();
    //
    /**
     * Gets the size of an UIElement (width and height)
     * @return the size of the UIElement
     */
    Size getDims();

    /**
     * Sets the size of an UIElement (width and height)
     * @param newSize the size we want to set to the UIElement
     */
    void setDims(Size newSize);

    /**
     * Gets the position of an UIElement (X-coordinate and Y-corrdinate)
     * @return the position of the UIElement
     */
    Position getPosition();

    /**
     * Sets the position of an UIElement (X-coordinate and Y-corrdinate)
     * @param newPosition the position we want to set to the UIElement
     */
    void setPosition(Position newPosition);

    /**
     * Gets the visibility status of an UIElement
     * @return the visibility status of the UIElement (true if the UIElement is visible, false if not)
     */
    boolean isVisible();

    /**
     * Sets the visibility status of an UIElement
     * @param newVisibleState the visibility status we want to set for the UIElement
     * 			(true if the UIElement is visible, false if not)
     */
    void setVisible(boolean newVisibleState);

    /**
     * Draws an UIElement
     */
    void draw();

    //	public abstract Layout getForegroundLayout();
    //
    //	public abstract void setForegroundLayout(Layout newLayout);
    //
    //	public abstract Layout getBackgroundLayout();
    //
    //	public abstract void setBackgroundLayout(Layout newLayout);
    //
    //	public abstract Layout getTextLayout();
    //
    //	public abstract void setTextLayout(Layout newLayout);

    /**
     * To set the Background color of the element.
     * @param color the Color
     */
    //void setBackground(Color color);
}
