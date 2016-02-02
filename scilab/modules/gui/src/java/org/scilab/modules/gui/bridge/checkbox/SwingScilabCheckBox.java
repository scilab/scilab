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

package org.scilab.modules.gui.bridge.checkbox;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_GROUP_NAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MAX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MIN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VALUE__;

import java.awt.Color;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Enumeration;

import javax.swing.AbstractButton;
import javax.swing.JCheckBox;
import javax.swing.UIManager;
import javax.swing.border.Border;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.SwingViewWidget;
import org.scilab.modules.gui.bridge.groupmanager.GroupManager;
import org.scilab.modules.gui.bridge.radiobutton.SwingScilabRadioButton;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.PositionConverter;
import org.scilab.modules.gui.utils.ScilabAlignment;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.widget.Widget;

/**
 * Swing implementation for Scilab CheckBox in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabCheckBox extends JCheckBox implements SwingViewObject, Widget {

    private static final long serialVersionUID = 3435428345694647542L;

    private Integer uid;

    private CommonCallBack callback;

    private ActionListener actListener;

    private Border defaultBorder = null;

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
                if (isSelected()) {
                    value[0] = (Double) GraphicController.getController().getProperty(uid, __GO_UI_MAX__);
                } else {
                    value[0] = (Double) GraphicController.getController().getProperty(uid, __GO_UI_MIN__);
                }

                if (isSelected()) {
                    String groupname = (String) GraphicController.getController().getProperty(uid, __GO_UI_GROUP_NAME__);
                    if (groupname != null && groupname.equals("") == false) {
                        Enumeration<AbstractButton> elements = GroupManager.getGroupManager().getGroupElements(groupname);
                        while (elements.hasMoreElements()) {
                            AbstractButton aButton = elements.nextElement();
                            if (aButton == e.getSource()) {
                                continue;
                            }

                            Integer id = 0;
                            if (aButton instanceof SwingScilabRadioButton) {
                                id = ((SwingScilabRadioButton) aButton).getId();
                            } else if (aButton instanceof SwingScilabRadioButton) {
                                id = ((SwingScilabCheckBox) aButton).getId();
                            } else {
                                continue;
                            }
                            //update model with min value
                            Double newValue[] = new Double[1];
                            newValue[0] = (Double) GraphicController.getController().getProperty(id, __GO_UI_MIN__);
                            GraphicController.getController().setProperty(id, __GO_UI_VALUE__, newValue);
                        }
                    }
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
     * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab
     * CheckBox
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
     * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab
     * CheckBox
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

        String groupname = (String) GraphicController.getController().getProperty(uid, __GO_UI_GROUP_NAME__);
        if (groupname != null && groupname.equals("") == false) {
            // use setSelected of ButtonGroup instead of JRadioButton
            GroupManager.getGroupManager().setSelected(getModel(), groupname, status);

            //update model with changes
            Enumeration<AbstractButton> elements = GroupManager.getGroupManager().getGroupElements(groupname);
            while (elements.hasMoreElements()) {
                AbstractButton aButton = elements.nextElement();
                Integer id = 0;
                boolean selected = false;

                if (aButton instanceof SwingScilabRadioButton) {
                    id = ((SwingScilabRadioButton) aButton).getId();
                    selected = ((SwingScilabRadioButton) aButton).isSelected();
                } else if (aButton instanceof SwingScilabCheckBox) {
                    id = ((SwingScilabCheckBox) aButton).getId();
                    selected = ((SwingScilabCheckBox) aButton).isSelected();
                } else {
                    continue;
                }

                // update model with min value
                Double newValue[] = new Double[1];
                if (selected) {
                    newValue[0] = (Double) GraphicController.getController().getProperty(id, __GO_UI_MAX__);
                } else {
                    newValue[0] = (Double) GraphicController.getController().getProperty(id, __GO_UI_MIN__);
                }
                GraphicController.getController().setProperty(id, __GO_UI_VALUE__, newValue);
            }

        } else {
            setSelected(status);
        }

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
        if (defaultBorder == null) {
            defaultBorder = getBorder();
        }
        setBorder(ScilabRelief.getBorderFromRelief(reliefType, defaultBorder));
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
    public void setId(Integer id) {
        uid = id;
    }

    /**
     * Get the UID
     * @return the UID
     */
    public Integer getId() {
        return uid;
    }

    /**
     * Generic update method
     * @param property property name
     * @param value property value
     */
    public void update(int property, Object value) {
        GraphicController controller = GraphicController.getController();
        switch (property) {
            case __GO_UI_GROUP_NAME__: {
                String groupName = (String) value;
                if (groupName == null || groupName.equals("")) {
                    //remove rb from buttonGroup Map
                    GroupManager.getGroupManager().removeFromGroup(this);
                } else {
                    GroupManager.getGroupManager().addToGroup(groupName, this);
                }
                break;
            }
            case __GO_UI_MAX__: {
                Double maxValue = ((Double) value);
                Double[] allValues = (Double[]) controller.getProperty(uid, __GO_UI_VALUE__);
                if ((allValues == null) || (allValues.length == 0)) {
                    return;
                }

                double uicontrolValue = allValues[0];
                // Check/Uncheck the CheckBox
                setChecked(maxValue == uicontrolValue);
                break;
            }
            case __GO_UI_VALUE__: {
                Double[] doubleValue = ((Double[]) value);
                if (doubleValue.length == 0) {
                    return;
                }

                int[] intValue = new int[doubleValue.length];
                for (int k = 0; k < doubleValue.length; k++) {
                    intValue[k] = doubleValue[k].intValue();
                }

                // Check the checkbox if the value is equal to MAX property
                Integer maxValue = ((Double) controller.getProperty(uid, __GO_UI_MAX__)).intValue();
                setChecked(maxValue == intValue[0]);
                break;
            }
            default: {
                SwingViewWidget.update(this, property, value);
                break;
            }
        }
    }

    public void resetBackground() {
        Color color = (Color) UIManager.getLookAndFeelDefaults().get("CheckBox.background");
        if (color != null) {
            setBackground(color);
        }
    }

    public void resetForeground() {
        Color color = (Color)UIManager.getLookAndFeelDefaults().get("CheckBox.foreground");
        if (color != null) {
            setForeground(color);
        }
    }

    public void setEmptyText() {
        setText(null);
    }
}
