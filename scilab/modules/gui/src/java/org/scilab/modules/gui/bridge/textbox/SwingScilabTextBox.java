/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
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
package org.scilab.modules.gui.bridge.textbox;

import java.awt.Color;

import javax.swing.JTextArea;
import javax.swing.UIManager;
import javax.swing.border.Border;

import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.textbox.SimpleTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab TextBox in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabTextBox extends JTextArea implements SimpleTextBox {

    private static final long serialVersionUID = 3632560416759268432L;

    private Border defaultBorder = null;

    /**
     * Constructor
     */
    public SwingScilabTextBox() {
        super();
        setEditable(false);
        /* Default settings for InfoBar (do not modify) */
        setRelief(ScilabRelief.RIDGE);
        setOpaque(false);
        setFocusable(false);
    }

    /**
     * Sets the text of a Text Widget
     * @param newText the text to set to the Text Widget
     */
    public void setText(String newText) {
        super.setText(newText);
    }

    public void setEmptyText() {
        setText(null);
    }

    /**
     * Gets the text of a Text Widget
     * @return the text of the Text Widget
     */
    public String getText() {
        return super.getText();
    }

    /**
     * Draws a swing Scilab TextBox
     * @see org.scilab.modules.gui.uielement.UIElement#draw()
     */
    public void draw() {
        this.setVisible(true);
        this.doLayout();
    }

    /**
     * Gets the dimensions (width and height) of a swing Scilab TextBox
     * @return the dimensions of the TextBox
     * @see org.scilab.modules.gui.uielement.UIElement#getDims()
     */
    public Size getDims() {
        return new Size(super.getSize().width, super.getSize().height);
    }

    /**
     * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab TextBox
     * @return the position of the TextBox
     * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
     */
    public Position getPosition() {
        return new Position(super.getX(), super.getY());
    }

    /**
     * Sets the dimensions (width and height) of a swing Scilab TextBox
     * @param newSize the dimensions to set to the TextBox
     * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
     */
    public void setDims(Size newSize) {
        super.setSize(newSize.getWidth(), newSize.getHeight());
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab TextBox
     * @param newPosition the position to set to the TextBox
     * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
     */
    public void setPosition(Position newPosition) {
        this.setLocation(newPosition.getX(), newPosition.getY());
    }


    /**
     * Get the id of the menu bar associated to the TextBox
     * @return the menubar
     * @see org.scilab.modules.gui.uielement.UIElement#getMenuBar()
     */
    public MenuBar getMenuBar() {
        // Must not be there...
        return null;
    }

    /**
     * Get the id of the tool bar associated to the TextBox
     * @return the toolbar
     * @see org.scilab.modules.gui.uielement.UIElement#getToolBar()
     */
    public ToolBar getToolBar() {
        // Must not be there...
        return null;
    }

    /**
     * Add a Scilab MenuBar to a Scilab TextBox
     * @param menuBarToAdd the Scilab MenuBar to add to the Scilab TextBox
     * @see org.scilab.modules.gui.window.Window#setMenuBar(org.scilab.modules.gui.menubar.MenuBar)
     */
    public void addMenuBar(MenuBar menuBarToAdd) {
        // TODO Auto-generated method stub

    }

    /**
     * Add a Scilab Toolbar to a Scilab TextBox
     * @param toolBarToAdd the Scilab ToolBar to add to the Scilab TextBox
     * @see org.scilab.modules.gui.window.Window#setToolBar(org.scilab.modules.gui.menubar.ToolBar)
     */
    public void addToolBar(ToolBar toolBarToAdd) {
        // TODO Auto-generated method stub

    }

    /**
     * Add a callback to the TextBox
     * @param callback the callback to set.
     */
    public void setCallback(CommonCallBack callback) {
        throw new UnsupportedOperationException();
    }

    /**
     * Set the horizontal alignment for the TextBox text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setHorizontalAlignment(String alignment) {
        throw new UnsupportedOperationException();
    }

    /**
     * Set the vertical alignment for the TextBox text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setVerticalAlignment(String alignment) {
        throw new UnsupportedOperationException();
    }

    /**
     * Set the Relief of the TextBox
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    public void setRelief(String reliefType) {
        if (defaultBorder == null) {
            defaultBorder = getBorder();
        }
        setBorder(ScilabRelief.getBorderFromRelief(reliefType, defaultBorder));
    }

    /**
     * Destroy the TextBox
     */
    public void destroy() {
        ScilabSwingUtilities.removeFromParent(this);
    }

    /**
     * Setter for InfoBar
     * @param infoBarToAdd the InfoBar associated to the TextBox.
     */
    public void addInfoBar(TextBox infoBarToAdd) {
        /* Unimplemented for TextBoxes */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for InfoBar
     * @return the InfoBar associated to the TextBox.
     */
    public TextBox getInfoBar() {
        /* Unimplemented for TextBoxes */
        throw new UnsupportedOperationException();
    }

    public void resetBackground() {
        Color color = (Color)UIManager.getLookAndFeelDefaults().get("TextArea.background");
        if (color != null) {
            setBackground(color);
        }
    }

    public void resetForeground() {
        Color color = (Color)UIManager.getLookAndFeelDefaults().get("TextArea.foreground");
        if (color != null) {
            setForeground(color);
        }
    }
}
