/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Han DONG
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
package org.scilab.modules.gui.bridge.uidisplaytree;

import java.awt.Color;
import java.awt.Font;

import javax.swing.JLabel;
import javax.swing.JScrollPane;
import javax.swing.UIManager;
import javax.swing.border.Border;

import org.scilab.modules.gui.bridge.tree.SwingScilabTree;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.tree.ScilabDisplayTree;
import org.scilab.modules.gui.uidisplaytree.SimpleUiDisplayTree;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.PositionConverter;
import org.scilab.modules.gui.utils.ScilabAlignment;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab UiDisplayTree in GUIs
 * @author Han DONG
 */
public class SwingScilabUiDisplayTree extends JScrollPane implements SimpleUiDisplayTree {

    private static final long serialVersionUID = -1;

    private JLabel label;
    private SwingScilabTree sst;

    private Border defaultBorder = null;

    /**
     * Constructor
     */
    public SwingScilabUiDisplayTree() {
        super();
    }

    /**
     * Apply a new font for the uiTable.
     * @param font new font to use.
     */
    public void setFont(Font font) {
        getLabel().setFont(font);
    }

    /**
     * To get the Font of the element.
     * @return font the Font
     */
    public Font getFont() {
        return getLabel().getFont();
    }

    /**
     * To get the Foreground color of the element.
     * @return color the Color
     */
    public Color getForeground() {
        return getLabel().getForeground();
    }

    /**
     * To set the Foreground color of the element.
     * @param color the Color
     */
    public void setForeground(Color color) {
        getLabel().setForeground(color);
    }

    /**
     * To set the Background color of the element.
     * @param color the Color
     */
    public void setBackground(Color color) {
        getLabel().setBackground(color);
    }

    /**
     * To get the Background color of the element.
     * @return color the Color
     */
    public Color getBackground() {
        return getLabel().getBackground();
    }

    /**
     * Draws a swing Scilab PushButton
     * @see org.scilab.modules.gui.uielement.UIElement#draw()
     */
    public void draw() {
        this.setVisible(true);
        this.doLayout();
    }

    /**
     * Sets the visibility status of an UIElement
     * @param newVisibleState the visibility status we want to set for the UIElement
     *                      (true if the UIElement is visible, false if not)
     */
    public void setVisible(boolean newVisibleState) {
        super.setVisible(newVisibleState);
    }

    /**
     * Gets the dimensions (width and height) of a swing Scilab element
     * @return the dimensions of the element
     * @see org.scilab.modules.gui.uielement.UIElement#getDims()
     */
    public Size getDims() {
        return new Size(getWidth(), getHeight());
    }

    /**
     * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab element
     * @return the position of the element
     * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
     */
    public Position getPosition() {
        return PositionConverter.javaToScilab(getLocation(), getSize(), getParent());
    }

    /**
     * Sets the dimensions (width and height) of a swing Scilab element
     * @param newSize the dimensions to set to the element
     * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
     */
    public void setDims(Size newSize) {
        setSize(newSize.getWidth(), newSize.getHeight());
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab element
     * @param newPosition the position to set to the element
     * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
     */
    public void setPosition(Position newPosition) {
        Position javaPosition = PositionConverter.scilabToJava(newPosition, getDims(), getParent());
        setLocation(javaPosition.getX(), javaPosition.getY());
    }

    /**
     * Add a callback to the UiTable
     * @param callback the callback to set.
     */
    public void setCallback(CommonCallBack callback) {
        // Nothing to do...
    }

    /**
     * Setter for MenuBar
     * @param menuBarToAdd the MenuBar associated to the UiTable.
     */
    public void addMenuBar(MenuBar menuBarToAdd) {
        /* Unimplemented for UiTables */
        throw new UnsupportedOperationException();
    }

    /**
     * Setter for ToolBar
     * @param toolBarToAdd the ToolBar associated to the UiTable.
     */
    public void addToolBar(ToolBar toolBarToAdd) {
        /* Unimplemented for UiTables */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for MenuBar
     * @return MenuBar: the MenuBar associated to the UiTable.
     */
    public MenuBar getMenuBar() {
        /* Unimplemented for UiTables */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for ToolBar
     * @return ToolBar: the ToolBar associated to the UiTable.
     */
    public ToolBar getToolBar() {
        /* Unimplemented for UiTables */
        throw new UnsupportedOperationException();
    }

    /**
     * Set the horizontal alignment for the UiTable text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setHorizontalAlignment(String alignment) {
        getLabel().setHorizontalAlignment(ScilabAlignment.toSwingAlignment(alignment));
    }

    /**
     * Set the vertical alignment for the UiTable text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setVerticalAlignment(String alignment) {
        getLabel().setVerticalAlignment(ScilabAlignment.toSwingAlignment(alignment));
    }

    /**
     * Set the Relief of the UiTable
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    public void setRelief(String reliefType) {
        if (defaultBorder == null) {
            defaultBorder = getBorder();
        }
        setBorder(ScilabRelief.getBorderFromRelief(reliefType, defaultBorder));
    }

    /**
     * Destroy the UiTable
     */
    public void destroy() {
        ScilabSwingUtilities.removeFromParent(this);
    }

    /**
     * Setter for InfoBar
     * @param infoBarToAdd the InfoBar associated to the UiTable.
     */
    public void addInfoBar(TextBox infoBarToAdd) {
        /* Unimplemented for UiTables */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for InfoBar
     * @return the InfoBar associated to the UiTable.
     */
    public TextBox getInfoBar() {
        /* Unimplemented for UiTables */
        throw new UnsupportedOperationException();
    }

    /**
     * Create/Return the uiTable Java object
     * @return the uiTable
     */
    private JLabel getLabel() {
        if (label == null) {
            label = new JLabel();
        }
        return label;
    }

    /**
     * gets directory of image in image render
     * @return the directory string
     * @see org.scilab.modules.gui.text.SimpleText#getText()
     */
    public String getText() {
        return getLabel().getText();
    }

    /**
     * Sets the directory for image to render
     * @param newText the new directory to image
     */
    public void setText(String newText) {
        getLabel().setText(newText);
    }

    public void setEmptyText() {
        setText(null);
    }

    /**
     * Sets the tree data to be added to viewport
     * @param text the array of strings containing tree data
     */
    public void setData(String[] text) {
        sst = new SwingScilabTree(ScilabDisplayTree.uicontrolScilabDisplayTree(text));
        getViewport().add(sst.getJTree());
    }

    public void resetBackground() {
        Color color = (Color)UIManager.getLookAndFeelDefaults().get("ScrollPane.background");
        if (color != null) {
            setBackground(color);
        }
    }

    public void resetForeground() {
        Color color = (Color)UIManager.getLookAndFeelDefaults().get("ScrollPane.foreground");
        if (color != null) {
            setForeground(color);
        }
    }
}
