/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.bridge.checkbox;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MIN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MAX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VALUE__;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JCheckBox;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.gui.SwingViewWidget;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.checkbox.SimpleCheckBox;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.PositionConverter;
import org.scilab.modules.gui.utils.ScilabAlignment;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab CheckBox in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabCheckBox extends JCheckBox implements SwingViewObject, SimpleCheckBox {

    private static final long serialVersionUID = 3435428345694647542L;

    private String uid;

    private CommonCallBack callback;

    private ActionListener actListener;

    /**
     * Constructor
     */
    public SwingScilabCheckBox() {
        super();
        /* Avoid the L&F to erase user background settings */
        setContentAreaFilled(false);
        setOpaque(true);
        actListener = new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                Double[] value = new Double[1];
                value[0] = (Double) GraphicController.getController().getProperty(uid, __GO_UI_MIN__);
                if (isSelected()) {
                    value[0] = (Double) GraphicController.getController().getProperty(uid, __GO_UI_MAX__);
                }
                GraphicController.getController().setProperty(uid, __GO_UI_VALUE__, value);
                if (callback != null) {
                    callback.actionPerformed(e);
                }
            }
        };
        addActionListener(actListener);
    }

    /**
     * Draws a swing Scilab CheckBox
     * @see org.scilab.modules.gui.UIElement#draw()
     */
    public void draw() {
        this.setVisible(true);
        this.doLayout();
    }

    /**
     * Gets the dimensions (width and height) of a swing Scilab CheckBox
     * @return the dimensions of the CheckBox
     * @see org.scilab.modules.gui.uielement.UIElement#getDims()
     */
    public Size getDims() {
        return new Size(this.getSize().width, this.getSize().height);
    }

    /**
     * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab CheckBox
     * @return the position of the CheckBox
     * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
     */
    public Position getPosition() {
        return PositionConverter.javaToScilab(getLocation(), getSize(), getParent());
    }

    /**
     * Sets the dimensions (width and height) of a swing Scilab CheckBox
     * @param newSize the dimensions we want to set to the CheckBox
     * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
     */
    public void setDims(Size newSize) {
        this.setSize(newSize.getWidth(), newSize.getHeight());
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab CheckBox
     * @param newPosition the position we want to set to the CheckBox
     * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
     */
    public void setPosition(Position newPosition) {
        Position javaPosition = PositionConverter.scilabToJava(newPosition, getDims(), getParent());
        setLocation(javaPosition.getX(), javaPosition.getY());
    }

    /**
     * Add a callback to the CheckBox
     * @param cb the callback to set
     */
    public void setCallback(CommonCallBack cb) {
        callback = cb;
    }

    /**
     * Setter for MenuBar
     * @param menuBarToAdd the MenuBar associated to the Tab.
     */
    public void addMenuBar(MenuBar menuBarToAdd) {
        /* Unimplemented for CheckBoxes */
        throw new UnsupportedOperationException();
    }

    /**
     * Setter for ToolBar
     * @param toolBarToAdd the ToolBar associated to the Tab.
     */
    public void addToolBar(ToolBar toolBarToAdd) {
        /* Unimplemented for CheckBoxes */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for MenuBar
     * @return MenuBar: the MenuBar associated to the Tab.
     */
    public MenuBar getMenuBar() {
        /* Unimplemented for CheckBoxes */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for ToolBar
     * @return ToolBar: the ToolBar associated to the Tab.
     */
    public ToolBar getToolBar() {
        /* Unimplemented for CheckBoxes */
        throw new UnsupportedOperationException();
    }

    /**
     * Set the horizontal alignment for the CheckBox text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setHorizontalAlignment(String alignment) {
        setHorizontalAlignment(ScilabAlignment.toSwingAlignment(alignment));
    }

    /**
     * Set the vertical alignment for the CheckBox text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setVerticalAlignment(String alignment) {
        setVerticalAlignment(ScilabAlignment.toSwingAlignment(alignment));
    }

    /**
     * Set if the CheckBox is checked or not
     * @param status true to set the CheckBox checked
     */
    public void setChecked(boolean status) {
        /* Remove the listener to avoid the callback to be executed */
        if (actListener != null) {
            removeActionListener(actListener);
        }

        setSelected(status);

        /* Put back the listener */
        if (actListener != null) {
            addActionListener(actListener);
        }
    }

    /**
     * Get the status of the CheckBox
     * @return true if the CheckBox is checked
     */
    public boolean isChecked() {
        return isSelected();
    }

    /**
     * Set the Relief of the CheckBox
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    public void setRelief(String reliefType) {
        setBorder(ScilabRelief.getBorderFromRelief(reliefType));
    }

    /**
     * Destroy the CheckBox
     */
    public void destroy() {
        getParent().remove(this);
        this.setVisible(false);
    }

    /**
     * Setter for InfoBar
     * @param infoBarToAdd the InfoBar associated to the CheckBox.
     */
    public void addInfoBar(TextBox infoBarToAdd) {
        /* Unimplemented for CheckBoxes */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for InfoBar
     * @return the InfoBar associated to the CheckBox.
     */
    public TextBox getInfoBar() {
        /* Unimplemented for CheckBoxes */
        throw new UnsupportedOperationException();
    }

    /**
     * Set the UID
     * @param id the UID
     */
    public void setId(String id) {
        uid = id;
    }

    /**
     * Get the UID
     * @return the UID
     */
    public String getId() {
        return uid;
    }

    /**
     * Generic update method
     * @param property property name
     * @param value property value
     */
    public void update(int property, Object value) {
        SwingViewWidget.update(this, property, value);
    }
}
