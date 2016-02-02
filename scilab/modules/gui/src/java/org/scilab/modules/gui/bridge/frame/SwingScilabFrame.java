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

package org.scilab.modules.gui.bridge.frame;


import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_BORDER_OPT_PADDING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CHILDREN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_GRID_OPT_GRID__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_GRID_OPT_PADDING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_LAYOUT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TAG__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TYPE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UICONTROL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ENABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ICON__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VALUE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_VISIBLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_EVENTHANDLER_NAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_EVENTHANDLER_ENABLE__;





import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.Insets;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;

import javax.swing.JPanel;
import javax.swing.UIManager;
import javax.swing.border.Border;

import org.scilab.modules.graphic_objects.axes.AxesContainer;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicModel.GraphicModel;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.uicontrol.Uicontrol;
import org.scilab.modules.graphic_objects.utils.LayoutType;
import org.scilab.modules.gui.SwingView;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.SwingViewWidget;
import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;
import org.scilab.modules.gui.bridge.console.SwingScilabConsole;
import org.scilab.modules.gui.bridge.tab.SwingScilabAxes;
import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.bridge.tab.SwingScilabTabGroup;
import org.scilab.modules.gui.bridge.textbox.SwingScilabTextBox;
import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.dockable.Dockable;
import org.scilab.modules.gui.editor.EditorEventListener;
import org.scilab.modules.gui.events.ScilabEventListener;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.BorderConvertor;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.PositionConverter;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.utils.UnitsConverter;
import org.scilab.modules.gui.utils.UnitsConverter.UicontrolUnits;
import org.scilab.modules.gui.widget.Widget;

/**
 * Swing implementation for Scilab frames in GUI
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabFrame extends JPanel implements SwingViewObject, Widget {

    private static final long serialVersionUID = -7401084975837285447L;

    private Integer uid = -1;
    int redraw = 0;
    protected boolean hasLayout = false;
    private Border defaultBorder = null;
    private SwingScilabCanvas canvas = null;

    // BJ: This EditorEventListener leads to a huge leak mem openning many Axes withih Frames.
    // DO NOT ACTIVATE THIS until EditorEventListener can manage Axes within Frames (Only Figure is working now)
    //private EditorEventListener editorEventHandler = null;

    private ScilabEventListener eventHandler;
    private boolean eventEnabled = false;

    /**
     * Constructor
     */
    public SwingScilabFrame() {
        super();
        // the Default layout is null so we have to set a Position and a Size of every Dockable we add to it
        super.setLayout(null);
        hasLayout = false;
        addComponentListener(new ComponentListener() {
            public void componentShown(ComponentEvent e) { }

            public void componentResized(ComponentEvent e) {
                if (getId() != -1 && getParent() != null) {

                    Double[] newPosition = new Double[4];
                    Double[] positions = (Double[]) GraphicController.getController().getProperty(getId(), GraphicObjectProperties.__GO_POSITION__);
                    if (positions == null) {
                        // Position property not yet set
                        return;
                    }

                    UicontrolUnits unitsProperty = UnitsConverter.stringToUnitsEnum((String) GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_UI_UNITS__));

                    //normalized values are always good
                    if (unitsProperty != UicontrolUnits.NORMALIZED) {
                        newPosition[0] = UnitsConverter.convertFromPixel(getPosition().getX(), unitsProperty, (SwingScilabFrame) SwingView.getFromId(uid), true);
                        newPosition[1] = UnitsConverter.convertFromPixel(getPosition().getY(), unitsProperty, (SwingScilabFrame) SwingView.getFromId(uid), true);
                        newPosition[2] = UnitsConverter.convertFromPixel(getWidth(), unitsProperty, (SwingScilabFrame) SwingView.getFromId(uid), true);
                        newPosition[3] = UnitsConverter.convertFromPixel(getHeight(), unitsProperty, (SwingScilabFrame) SwingView.getFromId(uid), true);
                        positions[2] = newPosition[2];
                        positions[3] = newPosition[3];
                        if (getParent() != null && getParent().getLayout() == null) {
                            GraphicController.getController().setProperty(getId(), GraphicObjectProperties.__GO_POSITION__, newPosition);
                        } else {
                            GraphicController.getController().setProperty(getId(), GraphicObjectProperties.__GO_POSITION__, positions);
                        }
                    }
                }

                if (hasLayout == false) {
                    for (Component comp : getComponents()) {
                        if (comp instanceof Widget) {
                            SwingViewObject obj = (SwingViewObject) comp;
                            obj.update(__GO_POSITION__, GraphicController.getController().getProperty(obj.getId(), __GO_POSITION__));
                        }
                    }
                }
            }

            public void componentMoved(ComponentEvent e) { }

            public void componentHidden(ComponentEvent e) { }
        });
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
        //forward disable status
        if (isEnabled() == false) {
            ((Component) member).setEnabled(false);
        }

        if (member instanceof SwingScilabAxes) {
            if (canvas == null) {
                //editorEventHandler = new EditorEventListener(getId());
                AxesContainer frame = (AxesContainer) GraphicModel.getModel().getObjectFromId(getId());
                canvas = new SwingScilabCanvas(frame);
                //canvas.addEventHandlerKeyListener(editorEventHandler);
                //canvas.addEventHandlerMouseListener(editorEventHandler);
                //canvas.addEventHandlerMouseMotionListener(editorEventHandler);
                //editorEventHandler.setEnable(true);

                setLayout(new GridLayout(1, 1));
                hasLayout = true;
                add(canvas);
            }
            return;
        }

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

            Integer[] preferredSize = uicontrol.getBorderPreferredSize();
            if (preferredSize[0].equals(-1) == false && preferredSize[1].equals(-1) == false) {
                ((Component) member).setPreferredSize(new Dimension(preferredSize[0], preferredSize[1]));
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

            Integer[] preferredSize = uicontrol.getGridBagPreferredSize();
            if (preferredSize[0].equals(-1) == false && preferredSize[1].equals(-1) == false) {
                ((Component) member).setPreferredSize(new Dimension(preferredSize[0], preferredSize[1]));
            }

            add((Component) member, constraints);
            revalidate();
        } else if (getLayout() instanceof GridLayout) {
            this.add((Component) member, 0);
        } else {
            this.add((Component) member);
        }

        //force update position
        member.update(__GO_POSITION__, GraphicController.getController().getProperty(member.getId(), __GO_POSITION__));
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

    public void setEmptyText() {
        setText("");
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
        if (defaultBorder == null) {
            defaultBorder = getBorder();
        }
        setBorder(ScilabRelief.getBorderFromRelief(reliefType, defaultBorder));
    }

    /**
     * Destroy the Frame
     */
    public void destroy() {
        this.setVisible(false);
        //if (editorEventHandler != null) {
        //    editorEventHandler.onExit();
        //}
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
        GraphicController controller = GraphicController.getController();

        switch (property) {
            case __GO_UI_VALUE__: {
                if (this instanceof SwingScilabLayer) {
                    SwingScilabLayer layer = (SwingScilabLayer) this;
                    Double[] doubleValue = ((Double[]) value);
                    if (doubleValue.length == 0) {
                        return;
                    }

                    Integer val = doubleValue[0].intValue();

                    //if intValue[0] is out of bounds, do not update view but let "wrong" value in model
                    layer.setActiveLayer(val);
                }
                break;
            }
            case __GO_UI_STRING__: {
                // Update tab title
                Container parent = getParent();
                if (parent instanceof SwingScilabTabGroup) {
                    SwingScilabTabGroup tab = (SwingScilabTabGroup) parent;
                    int index = tab.indexOfComponent(this);
                    if (index != -1) {
                        tab.setTitleAt(index, ((String[]) value)[0]);
                    }
                }

                if (this instanceof SwingScilabLayer) {
                    SwingScilabLayer layer = (SwingScilabLayer) this;
                    //if intValue[0] is out of bounds, do not update view but let "wrong" value in model
                    layer.setActiveLayerFromName(((String[]) value)[0]);
                }
                break;
            }
            case __GO_UI_ICON__: {
                // Update tab icon title
                Container parent = getParent();
                if (parent instanceof SwingScilabTabGroup) {
                    SwingScilabTabGroup tab = (SwingScilabTabGroup) parent;
                    int index = tab.indexOfComponent(this);
                    if (index != -1) {
                        tab.setIconAt(index, (String) value);
                    }
                }

                break;
            }
            case __GO_POSITION__: {
                SwingViewWidget.updatePosition(this, uid, value);
                validate();
                doLayout();
                break;
            }
            case __GO_UI_FRAME_BORDER__: {
                Integer borderId = (Integer) value;
                Border border = BorderConvertor.getBorder(borderId);
                setBorder(border);
                break;
            }
            case __GO_LAYOUT__: {
                hasLayout = false;

                invalidate();

                LayoutType newLayout = LayoutType.intToEnum((Integer) value);
                switch (newLayout) {
                    case BORDER: {
                        Integer[] padding = (Integer[]) controller.getProperty(getId(), __GO_BORDER_OPT_PADDING__);
                        setLayout(new BorderLayout(padding[0], padding[1]));
                        hasLayout = true;
                        break;
                    }
                    case GRIDBAG:
                        setLayout(new GridBagLayout());
                        hasLayout = true;
                        break;
                    case GRID: {
                        Integer[] padding = (Integer[]) controller.getProperty(getId(), __GO_GRID_OPT_PADDING__);
                        Integer[] grid = (Integer[]) controller.getProperty(getId(), __GO_GRID_OPT_GRID__);
                        if (grid[0] == 0 && grid[1] == 0) {
                            grid[0] = 1;
                        }

                        setLayout(new GridLayout(grid[0], grid[1], padding[0], padding[1]));
                        hasLayout = true;
                        break;
                    }
                    case NONE:
                    default: {
                        setLayout(null);
                        hasLayout = false;
                        break;
                    }
                }

                validate();
                break;
            }
            case __GO_GRID_OPT_PADDING__:
            case __GO_GRID_OPT_GRID__: {
                Integer layout = (Integer) GraphicController.getController().getProperty(getId(), __GO_LAYOUT__);
                LayoutType layoutType = LayoutType.intToEnum(layout);

                if (layoutType != LayoutType.GRID) {
                    break;
                }

                Integer[] padding = (Integer[]) GraphicController.getController().getProperty(getId(), __GO_GRID_OPT_PADDING__);

                Integer[] grid = (Integer[]) GraphicController.getController().getProperty(getId(), __GO_GRID_OPT_GRID__);
                Integer[] localGrid = new Integer[] { 0, 0 };
                localGrid[0] = grid[0];
                localGrid[1] = grid[1];

                if (localGrid[0] == 0 && localGrid[1] == 0) {
                    localGrid[0] = 1;
                }

                invalidate();

                GridLayout gl = (GridLayout)getLayout();
                gl.setRows(localGrid[0]);
                gl.setColumns(localGrid[1]);
                gl.setHgap(padding[0]);
                gl.setVgap(padding[1]);

                validate();
                break;
            }
            case __GO_BORDER_OPT_PADDING__: {
                Integer layout = (Integer) GraphicController.getController().getProperty(getId(), __GO_LAYOUT__);
                LayoutType layoutType = LayoutType.intToEnum(layout);

                if (layoutType != LayoutType.BORDER) {
                    break;
                }

                invalidate();

                Integer[] padding = (Integer[])value;
                BorderLayout bl = (BorderLayout)getLayout();
                bl.setHgap(padding[0]);
                bl.setVgap(padding[1]);

                validate();
                break;
            }
            case __GO_VISIBLE__: {
                boolean needUpdate = true;
                Component parent = getParent();
                if (parent instanceof SwingScilabLayer) {
                    //no no no don't touch visible on layer children !
                    Boolean visible = (Boolean) value;
                    SwingScilabLayer layer = (SwingScilabLayer) parent;
                    Boolean isActive = layer.isLayerActive(this);
                    if (isActive != visible) {
                        controller.setProperty(uid, __GO_VISIBLE__, isActive);
                    }

                    needUpdate = false;
                } else if (parent instanceof SwingScilabTabGroup) {
                    //no no no don't touch visible on layer children !
                    Boolean visible = (Boolean) value;
                    SwingScilabTabGroup layer = (SwingScilabTabGroup) parent;
                    Boolean isActive = layer.isTabActive(this);
                    if (isActive != visible) {
                        controller.setProperty(uid, __GO_VISIBLE__, isActive);
                    }

                    needUpdate = false;
                }

                if (needUpdate) {
                    setVisible(((Boolean) value).booleanValue());
                }

                break;
            }
            case __GO_TAG__ : {
                Component parent = getParent();
                if (parent instanceof SwingScilabLayer) {
                    SwingScilabLayer layer = (SwingScilabLayer)parent;
                    layer.updateModelProperties(null, layer.getActiveLayer());
                } else if (parent instanceof SwingScilabTabGroup) {
                    ((SwingScilabTabGroup)parent).updateModelProperties();
                }
                break;
            }
            case __GO_EVENTHANDLER_ENABLE__ : {
                Boolean enabled = (Boolean) GraphicController.getController().getProperty(getId(), __GO_EVENTHANDLER_ENABLE__);
                setEventHandlerEnabled(enabled);
                break;
            }
            case __GO_EVENTHANDLER_NAME__: {
                String eventHandlerName = (String) GraphicController.getController().getProperty(getId(), __GO_EVENTHANDLER_NAME__);
                setEventHandler(eventHandlerName);
                break;
            }
            default:
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
                Integer type = (Integer)GraphicController.getController().getProperty(children[kChild], __GO_TYPE__);
                if (type == __GO_UICONTROL__) {
                    Boolean childStatus = (Boolean) GraphicController.getController().getProperty(children[kChild], __GO_UI_ENABLE__);
                    SwingView.getFromId(children[kChild]).update(__GO_UI_ENABLE__, childStatus);
                }
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

        //if parent is a tab enable/disable children tab
        Component parent = getParent();
        if (parent instanceof SwingScilabTabGroup) {
            SwingScilabTabGroup tab = (SwingScilabTabGroup)parent;
            Integer index = tab.getIndex(this);
            if (index != -1) {
                tab.setEnabledAt(index, status);
            }
        }
    }

    //    public void setForeground(Color color) {
    //        System.out.println((Color)UIManager.getLookAndFeelDefaults().get("Panel.foreground"));
    //        super.setForeground((Color)UIManager.getLookAndFeelDefaults().get("Panel.foreground"));
    //    }

    public void resetBackground() {
        Color color = (Color)UIManager.getLookAndFeelDefaults().get("Panel.background");
        if (color != null) {
            setBackground(color);
        }
    }

    public void resetForeground() {
        Color color = (Color)UIManager.getLookAndFeelDefaults().get("Panel.foreground");
        if (color != null) {
            setForeground(color);
        }
    }

    public void removeAxes() {
        if (canvas != null) {
            canvas.setEnabled(false);
            remove(canvas);
            canvas.removeNotify();
            canvas = null;
            repaint();
        }
    }

    /**
     * Set the event handler of the Canvas
     * @param funName the name of the Scilab function to call
     */
    public void setEventHandler(String funName) {
        disableEventHandler();
        eventHandler = new ScilabEventListener(funName, getId());
        if (eventEnabled) {
            //editorEventHandler.setEnable(false);
            enableEventHandler();
        }
    }

    /**
     * Set the status of the event handler of the Canvas
     * @param status is true to set the event handler active
     */
    public void setEventHandlerEnabled(boolean status) {
        if (status && eventEnabled) {
            return;
        }

        if (status) {
            //editorEventHandler.setEnable(false);
            enableEventHandler();
            eventEnabled = true;
        } else {
            //editorEventHandler.setEnable(true);
            disableEventHandler();
            eventEnabled = false;
        }
    }

    /**
     * Turn on event handling.
     */
    private void enableEventHandler() {
        if (canvas != null) {
            canvas.addEventHandlerKeyListener(eventHandler);
            canvas.addEventHandlerMouseListener(eventHandler);
            canvas.addEventHandlerMouseMotionListener(eventHandler);
        }
    }

    /**
     * Turn off event handling.
     */
    private void disableEventHandler() {
        if (eventHandler != null && canvas != null) {
            canvas.removeEventHandlerKeyListener(eventHandler);
            canvas.removeEventHandlerMouseListener(eventHandler);
            canvas.removeEventHandlerMouseMotionListener(eventHandler);
        }
    }

}
