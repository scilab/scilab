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
import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.Insets;
import java.awt.LayoutManager;

import javax.swing.JComponent;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.ScrollPaneLayout;
import javax.swing.SwingUtilities;
import javax.swing.border.Border;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicModel.GraphicModel;
import org.scilab.modules.graphic_objects.uicontrol.Uicontrol;
import org.scilab.modules.gui.SwingView;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.SwingViewWidget;
import org.scilab.modules.gui.bridge.tab.SwingScilabPanel;
import org.scilab.modules.gui.bridge.tab.SwingScilabTabGroup;
import org.scilab.modules.gui.bridge.textbox.SwingScilabTextBox;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.dockable.Dockable;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.frame.SimpleFrame;
import org.scilab.modules.gui.menubar.MenuBar;
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
public class SwingScilabScrollableFrame extends JScrollPane implements SwingViewObject, SimpleFrame {

    private static final long serialVersionUID = -7401084975837285447L;

    private Integer uid;
    private JPanel panel = new JPanel();
    //    private Dimension panelSize = new Dimension(0, 0);
    //    private Point panelLoc = new Point(0, 0);

    /**
     * Constructor
     */
    public SwingScilabScrollableFrame() {
        super();
        setViewportView(panel);
        // the Default layout is null so we have to set a Position and a Size of every Dockable we add to it
        panel.setLayout(null);
        setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
    }

    public JPanel getPanel() {
        return panel;
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
        setSize(newSize.getWidth(), newSize.getHeight());
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
                    panel.add((Component) member, BorderLayout.SOUTH);
                    break;
                case TOP:
                    panel.add((Component) member, BorderLayout.NORTH);
                    break;
                case LEFT:
                    panel.add((Component) member, BorderLayout.WEST);
                    break;
                case RIGHT:
                    panel.add((Component) member, BorderLayout.EAST);
                    break;
                case CENTER:
                    panel.add((Component) member, BorderLayout.CENTER);
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

            panel.add((Component) member, constraints);
            revalidate();
        } else if (getLayout() instanceof GridLayout) {
            this.panel.add((Component) member, 0);
        } else {
            //define width and height for panel
            //and set preferred size

            panel.add((Component) member);
            updateChildPosition((JComponent)member);
        }

        SwingScilabPanel win = (SwingScilabPanel)SwingUtilities.getAncestorOfClass(SwingScilabPanel.class, this);
        if (win != null) {
            System.out.println("add member pack pack");
            SwingScilabWindow parentWindow = SwingScilabWindow.allScilabWindows.get(win.getParentWindowId());
            parentWindow.pack();
        }
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
        switch (property) {
            case __GO_UI_STRING__: {
                // Update tab title
                Container parent = getParent();
                if (parent instanceof SwingScilabTabGroup) {
                    SwingScilabTabGroup tab = (SwingScilabTabGroup) parent;
                    Component[] components = tab.getComponents();
                    for (int i = 0; i < components.length ; ++i) {
                        if (components[i] instanceof SwingScilabScrollableFrame && this.getId() == ((SwingScilabScrollableFrame) components[i]).getId()) {
                            tab.setTitleAt(i, ((String[]) value)[0]);
                            break;
                        }
                    }
                }
                break;
            }
            default :
                SwingViewWidget.update(this, property, value);
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

    public void setLayout(LayoutManager layout) {
        if (layout != null) {
            if (layout instanceof ScrollPaneLayout) {
                super.setLayout(layout);
            } else if (panel != null) {
                panel.setLayout(layout);
            }
        }
    }

    public LayoutManager getLayout() {
        if (panel != null) {
            return panel.getLayout();
        }
        return null;
    }

    public void updateChildPosition(JComponent child) {
        /*        System.out.println("updateChildPosition");
                Dimension size = child.getSize();
                System.out.println("child size : " + size.toString());
                Point loc = child.getLocation();
                System.out.println("child loc : " + loc.toString());
                if(Math.abs(loc.x + size.width) > panelSize.width) {
                    System.out.println("update width from " + panelSize.width + " to " + Math.abs(loc.x + size.width));
                    panelSize.width = Math.abs(loc.x + size.width);
                }

                if(Math.abs(loc.y + size.height) > panelSize.height) {
                    System.out.println("update height from " + panelSize.height + " to " + Math.abs(loc.y + size.height));
                    panelSize.height = Math.abs(loc.y + size.height);
                }

                if(loc.x < panelLoc.x) {
                    System.out.println("update x from " + panelLoc.x + " to " + loc.x);
                    panelLoc.x = loc.x;
                }

                if(loc.y < panelLoc.y) {
                    System.out.println("update y from " + panelLoc.y + " to " + loc.y);
                    panelLoc.y = loc.y;
                }

                panel.setPreferredSize(panelSize);
                panel.setLocation(panelLoc);
                Point newLoc = panel.getLocation();
                System.out.println("newLoc : " + newLoc.toString());
                System.out.println("view port location : " + getViewport().getViewPosition().toString());
                System.out.println("view port size : " + getViewport().getViewSize().toString());
                revalidate();
        */
    }

    public void setBorder(Border border) {
        if (panel != null) {
            panel.setBorder(border);
        }
    }

    public Border getBorder() {
        if (panel != null) {
            return panel.getBorder();
        }

        return null;
    }

    public void setBackground(Color bg) {
        if (panel != null) {
            panel.setBackground(bg);
        }
    }

}
