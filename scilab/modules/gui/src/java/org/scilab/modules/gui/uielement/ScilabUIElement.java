/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Bruno JOFRET
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

import org.scilab.modules.graphic_objects.uicontrol.Uicontrol;
import org.scilab.modules.gui.utils.Layout;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;

/**
 * Abstract class defining an object in Scilab GUIs
 * In the following, an UIElement is an object in Scilab GUIs
 * @author Vincent COUVERT
 * @author Bruno JOFRET
 * @author Marouane BEN JELLOUL
 */
public abstract class ScilabUIElement implements UIElement {

    /**
     * The toolBar associated to the UIElement
     */
    private ToolBar toolBar;

    /**
     * The menubar associated to the UIElement
     */
    private MenuBar menuBar;

    /**
     * The InfoBar associated to the UIElement
     */
    private TextBox infoBar;

    /**
     * The layout of the foreground of the UIElement
     * @see org.scilab.modules.gui.utils.Layout
     */
    private Layout foregroundLayout;

    /**
     * The layout of the background of the UIElement
     * @see org.scilab.modules.gui.utils.Layout
     */
    private Layout backgroundLayout;

    /**
     * The layout of the text of the UIElement
     * @see org.scilab.modules.gui.utils.Layout
     */
    private Layout textLayout;

    /***********************/
    /* GETTERS AND SETTERS */
    /***********************/

    /**
     * Gets the background layout of an UIElement
     * @return the background layout of the UIElement
     * @see org.scilab.modules.gui.utils.Layout
     */
    public Layout getBackgroundLayout() {
        return backgroundLayout;
    }

    /**
     * Sets the background layout of an UIElement
     * @param backgroundLayout the backgroundLayout to set
     * @see org.scilab.modules.gui.utils.Layout
     */
    public void setBackgroundLayout(Layout backgroundLayout) {
        this.backgroundLayout = backgroundLayout;
    }

    /**
     * Gets the foreground layout of an UIElement
     * @return the foreground layout of the UIElement
     * @see org.scilab.modules.gui.utils.Layout
     */
    public Layout getForegroundLayout() {
        return foregroundLayout;
    }

    /**
     * Sets the foreground layout of an UIElement
     * @param foregroundLayout the foregroundLayout to set
     * @see org.scilab.modules.gui.utils.Layout
     */
    public void setForegroundLayout(Layout foregroundLayout) {
        this.foregroundLayout = foregroundLayout;
    }

    /**
     * Sets the menubar associated to an UIElement
     * @param menuBar the menuBar to set
     */
    public void addMenuBar(MenuBar menuBar) {
        this.menuBar = menuBar;
    }

    /**
     * Get the menubar in the UIElementMapper
     * @return the MenuBar of the UIElement
     */
    public MenuBar getMenuBar() {
        return menuBar;
    }

    /**
     * Gets the layout of the text of an UIElement
     * @return the textLayout
     * @see org.scilab.modules.gui.utils.Layout
     */
    public Layout getTextLayout() {
        return textLayout;
    }

    /**
     * Sets the layout of the text of an UIElement
     * @param textLayout the textLayout to set
     * @see org.scilab.modules.gui.utils.Layout
     */
    public void setTextLayout(Layout textLayout) {
        this.textLayout = textLayout;
    }

    /**
     * Sets the toolbar associated to an UIElement
     * @param toolBar the toolBar to set
     */
    public void addToolBar(ToolBar toolBar) {
        this.toolBar = toolBar;
    }

    /**
     * Gets the toolbar associated to an UIElement
     * @return the toolBar
     */
    public ToolBar getToolBar() {
        return toolBar;
    }

    /**
     * Sets the InfoBar associated to an UIElement
     * @param infoBar the InfoBar to set
     */
    public void addInfoBar(TextBox infoBar) {
        this.infoBar = infoBar;
    }

    /**
     * Gets the InfoBar associated to an UIElement
     * @return the InfoBar
     */
    public TextBox getInfoBar() {
        return infoBar;
    }

    /*******************************/
    /* UIElement interface methods */
    /*******************************/

    /**
     * Draws an UIElement
     */
    public abstract void draw();
}
