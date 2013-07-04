/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.bridge.editbox;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING__;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;

import javax.swing.JTextField;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.gui.SwingView;
import org.scilab.modules.gui.SwingViewWidget;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.editbox.SimpleEditBox;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.PositionConverter;
import org.scilab.modules.gui.utils.ScilabAlignment;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab EditBox in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabEditBox extends JTextField implements SwingViewObject, SimpleEditBox {

    private static final long serialVersionUID = 1L;

    private String uid;

    private CommonCallBack callback;

    private FocusListener focusListener;
    private ActionListener actionListener;

    /**
     * Constructor
     */
    public SwingScilabEditBox() {
        super();
        // Create a focus listener to call the callback action
        focusListener = new FocusListener() {
            public void focusGained(FocusEvent arg0) {
                // Do nothing
            }
            public void focusLost(FocusEvent arg0) {
                // Validates user input
                if (getParent() != null) { // To avoid to execute the callback when then parent figure is destroyed
                    String[] stringProperty = new String[] {getText()};
                    GraphicController.getController().setProperty(uid, __GO_UI_STRING__, stringProperty);

                    if (SwingView.getFromId(uid) != null && callback != null) {
                        callback.actionPerformed(null);
                    }
                }
            }
        };
        addFocusListener(focusListener);

        // Create n action listener to get ENTER keystrokes
        actionListener = new ActionListener() {

            public void actionPerformed(ActionEvent arg0) {
                // Validates user input
                if (getParent() != null) { // To avoid to execute the callback when then parent figure is destroyed
                    String[] stringProperty = new String[] {getText()};
                    GraphicController.getController().setProperty(uid, __GO_UI_STRING__, stringProperty);
                    if (SwingView.getFromId(uid) != null && callback != null) {
                        callback.actionPerformed(null);
                    }
                }
            }

        };
        addActionListener(actionListener);
    }

    /**
     * Draws a swing Scilab EditBox
     * @see org.scilab.modules.gui.uielement.UIElement#draw()
     */
    public void draw() {
        this.setVisible(true);
        this.doLayout();
    }

    /**
     * Gets the dimensions (width and height) of a swing Scilab EditBox
     * @return the dimensions of the EditBox
     * @see org.scilab.modules.gui.uielement.UIElement#getDims()
     */
    public Size getDims() {
        return new Size(this.getSize().width, this.getSize().height);
    }

    /**
     * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab EditBox
     * @return the position of the EditBox
     * @see org.scilab.modules.gui.uielement.UIElement#getPosition()
     */
    public Position getPosition() {
        return PositionConverter.javaToScilab(getLocation(), getSize(), getParent());
    }

    /**
     * Sets the dimensions (width and height) of a swing Scilab EditBox
     * @param newSize the dimensions we want to set to the EditBox
     * @see org.scilab.modules.gui.uielement.UIElement#setDims(org.scilab.modules.gui.utils.Size)
     */
    public void setDims(Size newSize) {
        this.setSize(newSize.getWidth(), newSize.getHeight());
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab EditBox
     * @param newPosition the position we want to set to the EditBox
     * @see org.scilab.modules.gui.uielement.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
     */
    public void setPosition(Position newPosition) {
        Position javaPosition = PositionConverter.scilabToJava(newPosition, getDims(), getParent());
        setLocation(javaPosition.getX(), javaPosition.getY());
    }

    /**
     * Add a callback to the EditBox
     * @param cb the callback to set.
     */
    public void setCallback(CommonCallBack cb) {
        this.callback = cb;
    }

    /**
     * Set if the EditBox is enabled or not
     * @param status true if the EditBox is enabled
     */
    public void setEnabled(boolean status) {
        super.setEnabled(status);
        /* (Des)Activate the callback */
        if (callback != null) {
            if (status) {
                removeFocusListener(focusListener); /* To be sure the callback is not added two times */
                removeActionListener(actionListener); /* To be sure the callback is not added two times */
                addFocusListener(focusListener);
                addActionListener(actionListener);
            } else {
                removeFocusListener(focusListener);
                removeActionListener(actionListener);
            }
        }
    }

    /**
     * Setter for MenuBar
     * @param menuBarToAdd the MenuBar associated to the Tab.
     */
    public void addMenuBar(MenuBar menuBarToAdd) {
        /* Unimplemented for EditBoxes */
        throw new UnsupportedOperationException();
    }

    /**
     * Setter for ToolBar
     * @param toolBarToAdd the ToolBar associated to the Tab.
     */
    public void addToolBar(ToolBar toolBarToAdd) {
        /* Unimplemented for EditBoxes */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for MenuBar
     * @return MenuBar: the MenuBar associated to the Tab.
     */
    public MenuBar getMenuBar() {
        /* Unimplemented for EditBoxes */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for ToolBar
     * @return ToolBar: the ToolBar associated to the Tab.
     */
    public ToolBar getToolBar() {
        /* Unimplemented for EditBoxes */
        throw new UnsupportedOperationException();
    }

    /**
     * Set the horizontal alignment for the EditBox text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setHorizontalAlignment(String alignment) {
        setHorizontalAlignment(ScilabAlignment.toSwingAlignment(alignment));
    }

    /**
     * Set the vertical alignment for the EditBox text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setVerticalAlignment(String alignment) {
        // NOTHING TO DO HERE UNTIL WE HAVE MULTI-LINE EDIT UICONTROLS
    }

    /**
     * Set the Relief of the EditBox
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    public void setRelief(String reliefType) {
        setBorder(ScilabRelief.getBorderFromRelief(reliefType));
    }

    /**
     * Destroy the EditBox
     */
    public void destroy() {
        ScilabSwingUtilities.removeFromParent(this);
    }

    /**
     * Setter for InfoBar
     * @param infoBarToAdd the InfoBar associated to the EditBox.
     */
    public void addInfoBar(TextBox infoBarToAdd) {
        /* Unimplemented for EditBoxes */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for InfoBar
     * @return the InfoBar associated to the EditBox.
     */
    public TextBox getInfoBar() {
        /* Unimplemented for EditBoxes */
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
