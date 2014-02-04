/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.bridge.frame;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CHILDREN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ENABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING__;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Container;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.Insets;

import javax.swing.JPanel;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicModel.GraphicModel;
import org.scilab.modules.graphic_objects.uicontrol.Uicontrol;
import org.scilab.modules.gui.SwingView;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.SwingViewWidget;
import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;
import org.scilab.modules.gui.bridge.checkbox.SwingScilabCheckBox;
import org.scilab.modules.gui.bridge.console.SwingScilabConsole;
import org.scilab.modules.gui.bridge.editbox.SwingScilabEditBox;
import org.scilab.modules.gui.bridge.label.SwingScilabLabel;
import org.scilab.modules.gui.bridge.listbox.SwingScilabListBox;
import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.bridge.radiobutton.SwingScilabRadioButton;
import org.scilab.modules.gui.bridge.slider.SwingScilabScroll;
import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.bridge.tab.SwingScilabTabGroup;
import org.scilab.modules.gui.bridge.textbox.SwingScilabTextBox;
import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.checkbox.CheckBox;
import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.dockable.Dockable;
import org.scilab.modules.gui.editbox.EditBox;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.frame.Frame;
import org.scilab.modules.gui.frame.SimpleFrame;
import org.scilab.modules.gui.label.Label;
import org.scilab.modules.gui.listbox.ListBox;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.radiobutton.RadioButton;
import org.scilab.modules.gui.slider.Slider;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.PositionConverter;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab frames in GUI
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabFrame extends JPanel implements SwingViewObject, SimpleFrame {

    private static final long serialVersionUID = -7401084975837285447L;

    private Integer uid;

    /**
     * Constructor
     */
    public SwingScilabFrame() {
        super();
        // the Default layout is null so we have to set a Position and a Size of every Dockable we add to it
        super.setLayout(null);
    }

    /**
     * Draws a Swing Scilab frame
     * @see org.scilab.modules.gui.UIElement#draw()
     */
    public void draw() {
        this.setVisible(true);
        this.doLayout();
    }

    /**
     * Gets the dimensions (width and height) of a swing Scilab frame
     * @return the dimension of the frame
     * @see org.scilab.modules.gui.UIElement#getDims()
     */
    public Size getDims() {
        return new Size(this.getSize().width, this.getSize().height);
    }

    /**
     * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab frame
     * @return the position of the frame
     * @see org.scilab.modules.gui.UIElement#getPosition()
     */
    public Position getPosition() {
        return PositionConverter.javaToScilab(getLocation(), getSize(), getParent());
    }

    /**
     * Sets the dimensions (width and height) of a swing Scilab frame
     * @param newSize the size we want to set to the frame
     * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
     */
    public void setDims(Size newSize) {
        this.setSize(newSize.getWidth(), newSize.getHeight());
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab frame
     * @param newPosition the position we want to set to the frame
     * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
     */
    public void setPosition(Position newPosition) {
        Position javaPosition = PositionConverter.scilabToJava(newPosition, getDims(), getParent());
        setLocation(javaPosition.getX(), javaPosition.getY());
    }

    /**
     * Add a SwingViewObject (from SwingView.java) to container and returns its index
     * @param member the member to add
     */
    public void addMember(SwingViewObject member) {
        Uicontrol uicontrol = (Uicontrol) GraphicModel.getModel().getObjectFromId(member.getId());
        if (getLayout() instanceof BorderLayout) {
            switch (uicontrol.getBorderPositionAsEnum()) {
                case BOTTOM:
                    add((Component) member, BorderLayout.SOUTH);
                    break;
                case TOP:
                    add((Component) member, BorderLayout.NORTH);
                    break;
                case LEFT:
                    add((Component) member, BorderLayout.WEST);
                    break;
                case RIGHT:
                    add((Component) member, BorderLayout.EAST);
                    break;
                case CENTER:
                    add((Component) member, BorderLayout.CENTER);
                    break;
                default:
                    break;
            }
        } else if (getLayout() instanceof GridBagLayout) {
            GridBagConstraints constraints = new GridBagConstraints();

            // Grid
            Integer[] grid = uicontrol.getGridBagGrid();
            constraints.gridx = grid[0];
            constraints.gridy = grid[1];
            constraints.gridwidth = grid[2];
            constraints.gridheight = grid[3];

            // Weight
            Double[] weight = uicontrol.getGridBagWeight();
            constraints.weightx = weight[0];
            constraints.weighty = weight[1];

            // Anchor
            switch (uicontrol.getGridBagAnchorAsEnum()) {
                case LEFT :
                    constraints.anchor = GridBagConstraints.WEST;
                    break;
                case UPPER :
                    constraints.anchor = GridBagConstraints.NORTH;
                    break;
                case LOWER:
                    constraints.anchor = GridBagConstraints.SOUTH;
                    break;
                case LOWER_LEFT:
                    constraints.anchor = GridBagConstraints.SOUTHWEST;
                    break;
                case LOWER_RIGHT:
                    constraints.anchor = GridBagConstraints.SOUTHEAST;
                    break;
                case RIGHT:
                    constraints.anchor = GridBagConstraints.EAST;
                    break;
                case UPPER_LEFT:
                    constraints.anchor = GridBagConstraints.NORTHWEST;
                    break;
                case UPPER_RIGHT:
                    constraints.anchor = GridBagConstraints.NORTHEAST;
                    break;
                case CENTER :
                default :
                    constraints.anchor = GridBagConstraints.CENTER;
                    break;
            }

            // Fill
            switch (uicontrol.getGridBagFillAsEnum()) {
                case BOTH :
                    constraints.fill = GridBagConstraints.BOTH;
                    break;
                case HORIZONTAL:
                    constraints.fill = GridBagConstraints.HORIZONTAL;
                    break;
                case VERTICAL:
                    constraints.fill = GridBagConstraints.VERTICAL;
                    break;
                case NONE:
                default:
                    constraints.fill = GridBagConstraints.NONE;
                    break;
            }

            // Insets
            Double[] margins = uicontrol.getMargins();
            constraints.insets = new Insets(
                margins[0].intValue(), margins[1].intValue(),
                margins[2].intValue(), margins[3].intValue());

            // Padding
            Integer[] padding = uicontrol.getGridBagPadding();
            constraints.ipadx = padding[0];
            constraints.ipady = padding[1];

            add((Component) member, constraints);
            revalidate();
        } else if (getLayout() instanceof GridLayout) {
            this.add((Component) member, 0);
        } else {
            this.add((Component) member);
        }
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    public int addMember(Console member) {
        return this.addMember((SwingScilabConsole) member.getAsSimpleConsole());
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    private int addMember(SwingScilabConsole member) {
        this.add(member);
        return this.getComponentZOrder(member);
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    public int addMember(EditBox member) {
        return this.addMember((SwingScilabEditBox) member.getAsSimpleEditBox());
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    private int addMember(SwingScilabEditBox member) {
        this.add(member);
        return this.getComponentZOrder(member);
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    public int addMember(Label member) {
        // FIXME replace member with member.getAsSimpleLabel() when ready
        return this.addMember((SwingScilabLabel) member);
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    private int addMember(SwingScilabLabel member) {
        this.add(member);
        return this.getComponentZOrder(member);
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    public int addMember(Frame member) {
        return this.addMember((SwingScilabFrame) member.getAsSimpleFrame());
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    private int addMember(SwingScilabFrame member) {
        this.add(member);
        return this.getComponentZOrder(member);
    }

    /**
     * Add a Frame in a Frame with a BorderLayout.
     * @param member the member to add
     * @param borderLayout the BorderLayout to use
     * @return the position of the Frame in the member list.
     */
    public int addMember(Frame member, String borderLayout) {
        return this.addMember((SwingScilabFrame) member.getAsSimpleFrame(), borderLayout);
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @param borderLayout the BorderLayout to use
     * @return index of member in ArrayList
     */
    private int addMember(SwingScilabFrame member, String borderLayout) {
        this.add(member, borderLayout);
        return this.getComponentZOrder(member);
    }

    /**
     * Add a Frame in a Frame with a layout.
     * @param member the member to add
     * @param layoutPosition the Layout position to use
     * @return the position of the Frame in the member list.
     */
    public int addMember(Frame member, int layoutPosition) {
        return this.addMember((SwingScilabFrame) member.getAsSimpleFrame(), layoutPosition);
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @param layoutPosition the Layout position to use
     * @return index of member in ArrayList
     */
    private int addMember(SwingScilabFrame member, int layoutPosition) {
        this.add(member, layoutPosition);
        return this.getComponentZOrder(member);
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    public int addMember(Canvas member) {
        return this.addMember((SwingScilabCanvas) member.getAsSimpleCanvas());
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    private int addMember(SwingScilabCanvas member) {
        return 0;
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    public int addMember(CheckBox member) {
        // FIXME replace member with member.getAsSimpleCheckBox() when ready
        return this.addMember((SwingScilabCheckBox) member);
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    private int addMember(SwingScilabCheckBox member) {
        this.add(member);
        return this.getComponentZOrder(member);
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    public int addMember(ListBox member) {
        return this.addMember((SwingScilabListBox) member.getAsSimpleListBox());
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    private int addMember(SwingScilabListBox member) {
        this.add(member);
        return this.getComponentZOrder(member);
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    public int addMember(PushButton member) {
        return this.addMember((SwingScilabPushButton) member.getAsSimplePushButton());
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    private int addMember(SwingScilabPushButton member) {
        this.add(member);
        return this.getComponentZOrder(member);
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    public int addMember(RadioButton member) {
        //		 FIXME replace member with member.getAsSimpleRadioButton() when ready
        return this.addMember((SwingScilabRadioButton) member);
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    private int addMember(SwingScilabRadioButton member) {
        this.add(member);
        return this.getComponentZOrder(member);
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    public int addMember(Slider member) {
        //		 FIXME replace member with member.getAsSimpleSlider() when ready
        return this.addMember((SwingScilabScroll) member);
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    private int addMember(SwingScilabScroll member) {
        this.add(member);
        return this.getComponentZOrder(member);
    }

    /**
     * Add a PushButton in a Frame with a BorderLayout.
     * @param member the PushButton to add
     * @param borderLayout the BorderLayout to use
     * @return the position of the PushButton in the member list.
     */
    public int addMember(PushButton member, String borderLayout) {
        return this.addMember((SwingScilabPushButton) member.getAsSimplePushButton(), borderLayout);
    }

    /**
     * Add a PushButton (dockable element) to Frame and returns its index
     * @param member the PushButton to add
     * @param layoutPosition the layout Position to use
     * @return index of member in ArrayList
     */
    private int addMember(SwingScilabPushButton member, int layoutPosition) {
        this.add(member, layoutPosition);
        return this.getComponentZOrder(member);
    }

    /**
     * Add a PushButton in a Frame with a layout.
     * @param member the PushButton to add
     * @param layoutPosition the layout Position to use
     * @return the position of the PushButton in the member list.
     */
    public int addMember(PushButton member, int layoutPosition) {
        return this.addMember((SwingScilabPushButton) member.getAsSimplePushButton(), layoutPosition);
    }

    /**
     * Add a PushButton (dockable element) to Frame and returns its index
     * @param member the PushButton to add
     * @param borderLayout the BorderLayout to use
     * @return index of member in ArrayList
     */
    private int addMember(SwingScilabPushButton member, String borderLayout) {
        this.add(member, borderLayout);
        return this.getComponentZOrder(member);
    }

    //	 TODO : Check if it should be possible to add a Tab to a frame and how it should behave
    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    public int addMember(Tab member) {
        return this.addMember((SwingScilabDockablePanel) member.getAsSimpleTab());
    }
    //	 TODO : Check if it should be possible to add a Tab to a frame and how it should behave
    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    private int addMember(SwingScilabDockablePanel member) {
        this.add(member);
        return this.getComponentZOrder(member);
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    public int addMember(TextBox member) {
        return this.addMember((SwingScilabTextBox) member.getAsSimpleTextBox());
    }

    /**
     * Add a member (dockable element) to container and returns its index
     * @param member the member to add
     * @return index of member in ArrayList
     */
    private int addMember(SwingScilabTextBox member) {
        this.add(member);
        return this.getComponentZOrder(member);
    }

    /**
     * Add a callback to the Frame
     * @param callback the callback to set.
     */
    public void setCallback(CommonCallBack callback) {
        // Nothing to do...
    }

    /**
     * Setter for MenuBar
     * @param menuBarToAdd the MenuBar associated to the Frame.
     */
    public void addMenuBar(MenuBar menuBarToAdd) {
        /* Unimplemented for Frames */
        throw new UnsupportedOperationException();
    }

    /**
     * Setter for ToolBar
     * @param toolBarToAdd the ToolBar associated to the Frame.
     */
    public void addToolBar(ToolBar toolBarToAdd) {
        /* Unimplemented for Frames */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for MenuBar
     * @return MenuBar: the MenuBar associated to the Frame.
     */
    public MenuBar getMenuBar() {
        /* Unimplemented for Frames */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for ToolBar
     * @return ToolBar: the ToolBar associated to the Frame.
     */
    public ToolBar getToolBar() {
        /* Unimplemented for Frames */
        throw new UnsupportedOperationException();
    }

    /**
     * Get the text of the Frame
     * @return the text of the frame
     * @see org.scilab.modules.gui.frame.SimpleFrame#getText()
     */
    public String getText() {
        return this.getName();
    }

    /**
     * Set the text of the Frame
     * @param text the text to set to the frame
     * @see org.scilab.modules.gui.frame.SimpleFrame#setText()
     */
    public void setText(String text) {
        this.setName(text);
    }

    /**
     * Add a dockable element in the Frame (Not available for the moment)
     * @param member the object we want to add to the Frame
     * @return the index of the member in the Frame
     * @see org.scilab.modules.gui.container.Container#addMember(org.scilab.modules.gui.dockable.Dockable)
     */
    public int addMember(Dockable member) {
        /* Unimplemented for Frames */
        throw new UnsupportedOperationException();
    }

    /**
     * Set the Relief of the Frame
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    public void setRelief(String reliefType) {
        if (org.scilab.modules.graphic_objects.console.Console.getConsole().getUseDeprecatedLF()) {
            setBorder(ScilabRelief.getBorderFromRelief(reliefType));
        }
    }

    /**
     * Destroy the Frame
     */
    public void destroy() {
        getParent().remove(this);
        this.setVisible(false);
    }

    /**
     * Setter for InfoBar
     * @param infoBarToAdd the InfoBar associated to the Frame.
     */
    public void addInfoBar(TextBox infoBarToAdd) {
        /* Unimplemented for Frames */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for InfoBar
     * @return the InfoBar associated to the Frame.
     */
    public TextBox getInfoBar() {
        /* Unimplemented for Frames */
        throw new UnsupportedOperationException();
    }

    /**
     * Set the horizontal alignment for the Slider text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setHorizontalAlignment(String alignment) {
        // Nothing to do here
    }

    /**
     * Set the vertical alignment for the Slider text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setVerticalAlignment(String alignment) {
        // Nothing to do here
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
        SwingViewWidget.update(this, property, value);
        switch (property) {
            case __GO_UI_STRING__:
                // Update tab title
                Container parent = getParent();
                if (parent instanceof SwingScilabTabGroup) {
                    SwingScilabTabGroup tab = (SwingScilabTabGroup) parent;
                    Component[] components = tab.getComponents();
                    for (int i = 0; i < components.length ; ++i) {
                        if (components[i] instanceof SwingScilabFrame && this.getId() == ((SwingScilabFrame) components[i]).getId()) {
                            tab.setTitleAt(i, ((String[]) value)[0]);
                            break;
                        }
                    }
                }
                break;
            default :
                break;
        }
    }

    /**
     * Set the enable status of the frame and its children
     * @param status the status to set
     */
    public void setEnabled(boolean status) {
        if (status) {
            // Enable the frame
            super.setEnabled(status);
            // Enable its children according to their __GO_UI_ENABLE__ property
            Integer[] children = (Integer[]) GraphicController.getController().getProperty(uid, __GO_CHILDREN__);
            for (int kChild = 0; kChild < children.length; kChild++) {
                Boolean childStatus = (Boolean) GraphicController.getController().getProperty(children[kChild], __GO_UI_ENABLE__);
                SwingView.getFromId(children[kChild]).update(__GO_UI_ENABLE__, childStatus);
            }
        } else {
            // Disable the frame
            super.setEnabled(status);
            // Disable its children
            Component[] components = getComponents();
            for (int compIndex = 0; compIndex < components.length; compIndex++) {
                components[compIndex].setEnabled(false);
            }
        }
    }
}
