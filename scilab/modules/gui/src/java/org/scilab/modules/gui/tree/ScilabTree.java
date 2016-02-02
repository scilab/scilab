/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
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

package org.scilab.modules.gui.tree;

import java.awt.Color;
import java.awt.Font;
import java.awt.Image;
import java.awt.Toolkit;
import java.util.Vector;

import javax.swing.Icon;
import javax.swing.ImageIcon;

import org.scilab.modules.graphic_objects.graphicObject.CallBack;
import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.bridge.tree.ScilabTreeCellRenderer;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab Tree in GUIs
 * @author Sylvestre KOUMAR
 */
public class ScilabTree implements Tree {

    private String label;
    private Icon icon;
    private CommonCallBack callback;
    private Vector<Tree> children;

    private SimpleTree component = null;

    public ScilabTree(String label, String iconPath, String callbackString) {
        this.label = label;

        // Creating an Iconimage with the image's path
        Toolkit toolkit = Toolkit.getDefaultToolkit();
        if (iconPath.equals("default")) {
            this.icon = ScilabTreeCellRenderer.iconDefaut;
        } else if (iconPath.equals("")) {
            this.icon = null;
        } else {
            //Image image = (toolkit.getImage(iconPath)).getScaledInstance(25, 25, Image.SCALE_DEFAULT);
            Image image = (toolkit.getImage(iconPath));
            this.icon = new ImageIcon(image);
        }

        // Creating a callback with the callback's string
        this.callback = CommonCallBack.createCallback(callbackString, CallBack.SCILAB_FUNCTION);

        this.children = new Vector<Tree>();
    }

    /**
     * GUI display of this tree
     */
    public void showTree() {
        ScilabBridge.showTree(this);
    }

    /**
     * Get label of a node
     * @return node's label
     */
    public String getLabel() {
        return label;
    }

    /**
     * Set label for a node
     * @param label of the node
     */
    public void setLabel(String label) {
        this.label = label;
    }

    /**
     * Get icon of a node
     * @return node's icon
     */
    public Icon getIcon() {
        return icon;
    }

    /**
     * Set icon for a node
     * @param icon of the node
     */
    public void setIcon(Icon icon) {
        this.icon = icon;
    }

    /**
     * Get callback of a node
     * @return node's callback
     */
    public CommonCallBack getCallback() {
        return callback;
    }

    /**
     * Set callback for a node
     * @param callback of the node
     */
    public void setCallback(CommonCallBack callback) {
        this.callback = callback;
    }

    /**
     * Get children of a tree
     * @return children
     */
    public Vector<Tree> getChildren() {
        return children;
    }

    /**
     * Add children for a tree
     * @param firstChild of the tree
     */
    public void addChild(Tree firstChild) {
        children.add(firstChild);
    }

    private void updateComponent() {
        if (component == null) {
            component = ScilabBridge.createTree(this);
        }
    }

    /**
     * Gets this Bridge component object
     * @return this Bridge component object
     */
    public SimpleTree getAsSimpleTree() {
        updateComponent();
        return component;
    }

    public void destroy() {
        // TODO Auto-generated method stub

    }

    public Color getBackground() {
        // TODO Auto-generated method stub
        return null;
    }

    public Font getFont() {
        // TODO Auto-generated method stub
        return null;
    }

    public Color getForeground() {
        // TODO Auto-generated method stub
        return null;
    }

    public String getText() {
        // TODO Auto-generated method stub
        return null;
    }

    public boolean isEnabled() {
        // TODO Auto-generated method stub
        return false;
    }

    public void requestFocus() {
        // TODO Auto-generated method stub

    }

    public void setBackground(Color color) {
        // TODO Auto-generated method stub

    }

    public void setEnabled(boolean status) {
        // TODO Auto-generated method stub

    }

    public void setFont(Font font) {
        // TODO Auto-generated method stub

    }

    public void setForeground(Color color) {
        // TODO Auto-generated method stub

    }

    public void setHorizontalAlignment(String alignment) {
        // TODO Auto-generated method stub

    }

    public void setRelief(String reliefType) {
        // TODO Auto-generated method stub

    }

    public void setText(String text) {
        // TODO Auto-generated method stub

    }

    public void setEmptyText() {
        setText(null);
    }

    public void setVerticalAlignment(String alignment) {
        // TODO Auto-generated method stub

    }

    public void addInfoBar(TextBox infoBarToAdd) {
        // TODO Auto-generated method stub

    }

    public void addMenuBar(MenuBar menuBarToAdd) {
        // TODO Auto-generated method stub

    }

    public void addToolBar(ToolBar toolBarToAdd) {
        // TODO Auto-generated method stub

    }

    public void draw() {
        // TODO Auto-generated method stub

    }

    public Size getDims() {
        // TODO Auto-generated method stub
        return null;
    }

    public TextBox getInfoBar() {
        // TODO Auto-generated method stub
        return null;
    }

    public MenuBar getMenuBar() {
        // TODO Auto-generated method stub
        return null;
    }

    public Position getPosition() {
        // TODO Auto-generated method stub
        return null;
    }

    public ToolBar getToolBar() {
        // TODO Auto-generated method stub
        return null;
    }

    public boolean isVisible() {
        // TODO Auto-generated method stub
        return false;
    }

    public void setDims(Size newSize) {
        updateComponent();
        component.setDims(newSize);
    }

    public void setPosition(Position newPosition) {
        updateComponent();
        component.setPosition(newPosition);
    }

    public void setVisible(boolean newVisibleState) {
        // TODO Auto-generated method stub

    }

    @Override
    public void setToolTipText(String tooltipText) {
        // TODO Auto-generated method stub
    }

    @Override
    public void resetBackground() {
        // TODO Auto-generated method stub

    }

    @Override
    public void resetForeground() {
        // TODO Auto-generated method stub

    }
}
