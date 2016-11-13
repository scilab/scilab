/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Bruno JOFRET
 * Copyright (C) 2015 - Scilab Enterprises - Anais AUBERT
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
package org.scilab.modules.gui.bridge.tab;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_AUTORESIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_AXES_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_BACKGROUND__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_BORDER_OPT_PADDING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CALLBACK__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CHILDREN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_COLORMAP__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_EVENTHANDLER_ENABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_EVENTHANDLER_NAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_GRID_OPT_GRID__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_GRID_OPT_PADDING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_ID__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_INFOBAR_VISIBLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_INFO_MESSAGE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_LAYOUT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_MENUBAR_VISIBLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_NAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_RESIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TOOLBAR_VISIBLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TYPE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UICHECKEDMENU__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UICHILDMENU__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UIMENU__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UIPARENTMENU__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ICON__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_VISIBLE__;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.Insets;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.ImageIcon;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.figure.Figure.BarType;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicModel.GraphicModel;
import org.scilab.modules.graphic_objects.uicontrol.Uicontrol;
import org.scilab.modules.graphic_objects.utils.LayoutType;
import org.scilab.modules.gui.SwingView;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.bridge.frame.SwingScilabFrame;
import org.scilab.modules.gui.bridge.frame.SwingScilabScrollableFrame;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.events.callback.ScilabCloseCallBack;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.utils.ToolBarBuilder;
import org.scilab.modules.renderer.JoGLView.util.ColorFactory;

public class SwingScilabCommonPanel {

    public static final String GRAPHICS_TOOLBAR_DESCRIPTOR = System.getenv("SCI") + "/modules/gui/etc/graphics_toolbar.xml";

    /**
     * Update the tab after a modification of its properties
     * @param property the property name
     * @param value the property value
     * @see org.scilab.modules.gui.SwingViewObject#update(java.lang.String,
     * java.lang.Object)
     */
    protected static void update(SwingScilabPanel component, int property, Object value) {
        String name;
        Integer figureId;
        switch (property) {
            case __GO_NAME__:
                name = ((String) value);
                figureId = (Integer) GraphicController.getController().getProperty(component.getId(), __GO_ID__);
                updateTitle(component, name, figureId);
                break;
            case __GO_ID__:
                /* Update title */
                figureId = ((Integer) value);
                Figure localFigure = (Figure) GraphicController.getController().getObjectFromId(component.getId());
                name = localFigure.getName();
                updateTitle(component, name, figureId);

                /** Update tool bar */
                if (localFigure.getToolbarAsEnum() == BarType.FIGURE) {
                    SwingScilabWindow parentWindow = SwingScilabWindow.allScilabWindows.get(component.getParentWindowId());

                    boolean currentVisible = component.getToolBar().isVisible();

                    //only if toolbar is visible
                    if (currentVisible) {
                        //keep current delta between figure size and axes size
                        component.storeSizeDelta();
                    }

                    ToolBar toolbar = ToolBarBuilder.buildToolBar(GRAPHICS_TOOLBAR_DESCRIPTOR, figureId);
                    toolbar.setVisible(currentVisible);
                    component.setToolBar(toolbar);
                    parentWindow.addToolBar(toolbar);
                    //force redraw to get good value on contentpane.getHeight
                    parentWindow.validate();

                    if (currentVisible) {
                        //apply stored delta to new axes size
                        component.applyDeltaSize();
                    }
                }

                /* Update callback */
                String closingCommand = "if (get_figure_handle(" + figureId + ") <> []) then" + "  if (get(get_figure_handle(" + figureId + "), 'event_handler_enable') == 'on') then"
                                        + "    execstr(get(get_figure_handle(" + figureId + "), 'event_handler')+'(" + figureId + ", -1, -1, -1000)', 'errcatch', 'm');" + "  end;" + "  delete(get_figure_handle("
                                        + figureId + "));" + "end;";
                component.setCallback(null);
                component.setCallback(ScilabCloseCallBack.create(component.getId(), closingCommand));
                /* Update menus callback */
                Integer[] children = (Integer[]) GraphicController.getController().getProperty(component.getId(), __GO_CHILDREN__);
                updateChildrenCallbacks(children, figureId);
                break;
            case __GO_SIZE__: {
                Integer[] size = (Integer[]) value;
                SwingScilabWindow figure = SwingScilabWindow.allScilabWindows.get(component.getParentWindowId());
                Size oldFigureSize = figure.getDims();

                if (oldFigureSize.getWidth() != 0 && oldFigureSize.getHeight() != 0 && ((oldFigureSize.getWidth() != size[0]) || (oldFigureSize.getHeight() != size[1]))
                        && ((Boolean) GraphicController.getController().getProperty(component.getId(), __GO_AUTORESIZE__))) {
                    figure.setDims(new Size(size[0], size[1]));
                    figure.validate();
                    // Axes are resized by resize event: SwingScilabDockablePanel::componentResized or SwingScilabStaticPanel::componentResized
                    // ==> NO NEED TO FORCE RESIZE
                    // GraphicController.getController().setProperty(component.getId(), __GO_AXES_SIZE__, new Integer[] { figure.getContentPane().getWidth(), figure.getContentPane().getHeight() });

                }
                break;
            }
            case __GO_POSITION__:
                Integer[] position = (Integer[]) value;
                SwingScilabWindow.allScilabWindows.get(component.getParentWindowId()).setPosition(new Position(position[0], position[1]));
                break;
            case __GO_AXES_SIZE__:
                Integer[] axesSize = (Integer[]) value;
                Dimension oldAxesSize = component.getContentPane().getSize();
                if (oldAxesSize.getWidth() != 0 && oldAxesSize.getHeight() != 0 && ((oldAxesSize.getWidth() != axesSize[0]) || (oldAxesSize.getHeight() != axesSize[1]))
                        && ((Boolean) GraphicController.getController().getProperty(component.getId(), __GO_AUTORESIZE__))) {
                    // TODO manage tabs when there are docked (do not change the window size if more than one tab docked)
                    int deltaX = axesSize[0] - (int) oldAxesSize.getWidth();
                    int deltaY = axesSize[1] - (int) oldAxesSize.getHeight();
                    Size parentWindowSize = SwingScilabWindow.allScilabWindows.get(component.getParentWindowId()).getDims();
                    SwingScilabWindow.allScilabWindows.get(component.getParentWindowId()).setDims(new Size(parentWindowSize.getWidth() + deltaX, parentWindowSize.getHeight() + deltaY));
                    GraphicController.getController().setProperty(component.getId(), __GO_SIZE__, new Integer[] { parentWindowSize.getWidth() + deltaX, parentWindowSize.getHeight() + deltaY});
                }
                break;
            case __GO_INFO_MESSAGE__:
                if (component.getInfoBar() != null) {
                    component.getInfoBar().setText((String) value);
                }
                break;
            case __GO_EVENTHANDLER_ENABLE__:
                Boolean enabled = (Boolean) GraphicController.getController().getProperty(component.getId(), __GO_EVENTHANDLER_ENABLE__);
                component.setEventHandlerEnabled(enabled);
                break;
            case __GO_EVENTHANDLER_NAME__:
                String eventHandlerName = (String) GraphicController.getController().getProperty(component.getId(), __GO_EVENTHANDLER_NAME__);
                component.setEventHandler(eventHandlerName);
                break;
            case __GO_VISIBLE__:
                component.getContentPane().setVisible((Boolean) value);
                if (component.getParentWindow().getNbDockedObjects() == 1) {
                    component.getParentWindow().setVisible((Boolean) value);
                }
                break;
            case __GO_INFOBAR_VISIBLE__: {
                component.disableResizeEvent();
                Integer[] oldSize = (Integer[]) GraphicController.getController().getProperty(component.getId(), __GO_AXES_SIZE__);
                component.getInfoBar().setVisible((Boolean) value);
                SwingScilabWindow parentWindow = SwingScilabWindow.allScilabWindows.get(component.getParentWindowId());
                parentWindow.validate();
                GraphicController.getController().setProperty(component.getId(), __GO_AXES_SIZE__, oldSize);
                component.enableResizeEvent();
                break;
            }
            case __GO_TOOLBAR_VISIBLE__: {
                component.disableResizeEvent();
                Integer[] oldSize = (Integer[]) GraphicController.getController().getProperty(component.getId(), __GO_AXES_SIZE__);
                component.getToolBar().setVisible((Boolean) value);
                SwingScilabWindow parentWindow = SwingScilabWindow.allScilabWindows.get(component.getParentWindowId());
                parentWindow.validate();
                GraphicController.getController().setProperty(component.getId(), __GO_AXES_SIZE__, oldSize);
                component.enableResizeEvent();
                break;
            }
            case __GO_MENUBAR_VISIBLE__: {
                component.disableResizeEvent();
                Integer[] oldSize = (Integer[]) GraphicController.getController().getProperty(component.getId(), __GO_AXES_SIZE__);
                component.getMenuBar().setVisible((Boolean) value);
                SwingScilabWindow parentWindow = SwingScilabWindow.allScilabWindows.get(component.getParentWindowId());
                parentWindow.validate();
                GraphicController.getController().setProperty(component.getId(), __GO_AXES_SIZE__, oldSize);
                component.enableResizeEvent();
                break;
            }
            case __GO_RESIZE__:
                component.getParentWindow().setResizable((Boolean) value);
                //on some cases, set resizable can change frame size. ( L&F, Window Manager, ... )
                //so force set of axes_size.
                SwingScilabCommonPanel.update(component, __GO_AXES_SIZE__, GraphicController.getController().getProperty(component.getId(), __GO_AXES_SIZE__));

                break;
            case __GO_LAYOUT__:
                LayoutType newLayout = LayoutType.intToEnum((Integer) value);

                component.getWidgetPane().invalidate();

                switch (newLayout) {
                    case BORDER: {
                        Integer[] padding = (Integer[]) GraphicController.getController().getProperty(component.getId(), __GO_BORDER_OPT_PADDING__);
                        component.getWidgetPane().setLayout(new BorderLayout(padding[0], padding[1]));
                        component.setHasLayout(true);
                        break;
                    }
                    case GRIDBAG: {
                        component.getWidgetPane().setLayout(new GridBagLayout());
                        component.setHasLayout(true);
                        break;
                    }
                    case GRID: {
                        Integer[] padding = (Integer[]) GraphicController.getController().getProperty(component.getId(), __GO_GRID_OPT_PADDING__);
                        Integer[] grid = (Integer[]) GraphicController.getController().getProperty(component.getId(), __GO_GRID_OPT_GRID__);
                        Integer[] localGrid = new Integer[] { 0, 0 };
                        localGrid[0] = grid[0];
                        localGrid[1] = grid[1];

                        if (localGrid[0] == 0 && localGrid[1] == 0) {
                            localGrid[0] = 1;
                        }

                        component.getWidgetPane().setLayout(new GridLayout(localGrid[0], localGrid[1], padding[0], padding[1]));
                        component.setHasLayout(true);
                        break;
                    }
                    case NONE:
                    default:
                        component.getWidgetPane().setLayout(null);
                        component.setHasLayout(false);
                        break;
                }

                component.getWidgetPane().validate();

                break;
            case __GO_GRID_OPT_PADDING__:
            case __GO_GRID_OPT_GRID__: {
                Integer layout = (Integer) GraphicController.getController().getProperty(component.getId(), __GO_LAYOUT__);
                LayoutType layoutType = LayoutType.intToEnum(layout);

                if (layoutType != LayoutType.GRID) {
                    break;
                }

                Integer[] padding = (Integer[]) GraphicController.getController().getProperty(component.getId(), __GO_GRID_OPT_PADDING__);

                Integer[] grid = (Integer[]) GraphicController.getController().getProperty(component.getId(), __GO_GRID_OPT_GRID__);
                Integer[] localGrid = new Integer[] { 0, 0 };
                localGrid[0] = grid[0];
                localGrid[1] = grid[1];

                if (localGrid[0] == 0 && localGrid[1] == 0) {
                    localGrid[0] = 1;
                }

                component.getWidgetPane().invalidate();

                GridLayout gl = (GridLayout)component.getWidgetPane().getLayout();
                gl.setRows(localGrid[0]);
                gl.setColumns(localGrid[1]);
                gl.setHgap(padding[0]);
                gl.setVgap(padding[1]);

                component.getWidgetPane().validate();

                break;
            }
            case __GO_BORDER_OPT_PADDING__: {
                Integer layout = (Integer) GraphicController.getController().getProperty(component.getId(), __GO_LAYOUT__);
                LayoutType layoutType = LayoutType.intToEnum(layout);

                if (layoutType != LayoutType.BORDER) {
                    break;
                }

                component.getWidgetPane().invalidate();

                Integer[] padding = (Integer[])value;
                BorderLayout bl = (BorderLayout)component.getWidgetPane().getLayout();
                bl.setHgap(padding[0]);
                bl.setVgap(padding[1]);


                component.getWidgetPane().validate();

                break;
            }
            case __GO_UI_ICON__: {
                File file = new File((String) value);
                if (file.exists() == false) {
                    String filename = FindIconHelper.findImage((String) value);
                    file = new File(filename);
                }

                try {
                    BufferedImage icon = ImageIO.read(file);
                    component.getParentWindow().setIconImage(new ImageIcon(icon).getImage());
                } catch (IOException e) {
                }
                break;
            }
            case __GO_COLORMAP__: {
                // Force background
                Figure figure = (Figure) GraphicController.getController().getObjectFromId(component.getId());
                component.setFigureBackground(ColorFactory.createColor(figure.getColorMap(), figure.getBackground()));
                break;
            }
            case __GO_BACKGROUND__: {
                Figure figure = (Figure) GraphicController.getController().getObjectFromId(component.getId());
                component.setFigureBackground(ColorFactory.createColor(figure.getColorMap(), (Integer) value));
                break;
            }
        }
    }

    /**
     * Update the title of the Tab
     * @param figureName figure_name property
     * @param figureId figure_id property
     */
    private static void updateTitle(SwingScilabPanel component, String figureName, Integer figureId) {
        if ((figureName != null) && (figureId != null)) {
            String figureTitle = figureName.replaceFirst("%d", figureId.toString());
            component.setName(figureTitle);
        }
    }

    /**
     * Update the menus callbacks when they are linked to the figure ID
     * @param children the children UID
     * @param parentFigureId the figure ID
     */
    private static void updateChildrenCallbacks(Integer[] children, int parentFigureId) {
        for (int kChild = 0; kChild < children.length; kChild++) {
            Integer childType = (Integer) GraphicController.getController().getProperty(children[kChild], __GO_TYPE__);
            if (childType != null && (childType == __GO_UIMENU__ || childType == __GO_UIPARENTMENU__ || childType == __GO_UICHILDMENU__ || childType == __GO_UICHECKEDMENU__)) {
                String cb = (String) GraphicController.getController().getProperty(children[kChild], __GO_CALLBACK__);
                SwingView.getFromId(children[kChild]).update(__GO_CALLBACK__, replaceFigureID(cb, parentFigureId));
                Integer[] menuChildren = (Integer[]) GraphicController.getController().getProperty(children[kChild], __GO_CHILDREN__);
                updateChildrenCallbacks(menuChildren, parentFigureId);
            }
        }
    }

    /**
     * Replace pattern [SCILAB_FIGURE_ID] by the figure index
     * @param initialString string read in XML file
     * @param parentFigureId the figure ID
     * @return callback string
     */
    private static String replaceFigureID(String initialString, Integer parentFigureId) {
        return initialString.replaceAll("\\[SCILAB_FIGURE_ID\\]", Integer.toString(parentFigureId));
    }

    /**
     * Add a SwingViewObject (from SwingView.java) to container and returns its
     * index
     * @param member the member to add
     */
    protected static void addMember(SwingScilabPanel component, SwingViewObject member) {
        //member.get
        Uicontrol uicontrol = (Uicontrol) GraphicModel.getModel().getObjectFromId(member.getId());
        if (component.getWidgetPane().getLayout() instanceof BorderLayout) {
            switch (uicontrol.getBorderPositionAsEnum()) {
                case BOTTOM:
                    component.getWidgetPane().add((Component) member, BorderLayout.SOUTH);
                    break;
                case TOP:
                    component.getWidgetPane().add((Component) member, BorderLayout.NORTH);
                    break;
                case LEFT:
                    component.getWidgetPane().add((Component) member, BorderLayout.WEST);
                    break;
                case RIGHT:
                    component.getWidgetPane().add((Component) member, BorderLayout.EAST);
                    break;
                case CENTER:
                    component.getWidgetPane().add((Component) member, BorderLayout.CENTER);
                    break;
                default:
                    break;
            }

            Integer[] preferredSize = uicontrol.getBorderPreferredSize();
            if (preferredSize[0].equals(-1) == false && preferredSize[1].equals(-1) == false) {
                ((Component) member).setPreferredSize(new Dimension(preferredSize[0], preferredSize[1]));
            }
        } else if (component.getWidgetPane().getLayout() instanceof GridBagLayout) {
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
                case RIGHT:
                    constraints.anchor = GridBagConstraints.EAST;
                    break;
                case UPPER:
                    constraints.anchor = GridBagConstraints.NORTH;
                    break;
                case LOWER:
                    constraints.anchor = GridBagConstraints.SOUTH;
                    break;
                case LOWER_RIGHT:
                    constraints.anchor = GridBagConstraints.SOUTHEAST;
                    break;
                case LOWER_LEFT:
                    constraints.anchor = GridBagConstraints.SOUTHWEST;
                    break;
                case LEFT:
                    constraints.anchor = GridBagConstraints.WEST;
                    break;
                case UPPER_RIGHT:
                    constraints.anchor = GridBagConstraints.NORTHEAST;
                    break;
                case UPPER_LEFT:
                    constraints.anchor = GridBagConstraints.NORTHWEST;
                    break;
                case CENTER:
                default:
                    constraints.anchor = GridBagConstraints.CENTER;
                    break;
            }

            // Fill
            switch (uicontrol.getGridBagFillAsEnum()) {
                case BOTH:
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
            constraints.insets = new Insets(margins[0].intValue(), margins[1].intValue(), margins[2].intValue(), margins[3].intValue());

            // Padding
            Integer[] padding = uicontrol.getGridBagPadding();
            constraints.ipadx = padding[0];
            constraints.ipady = padding[1];

            Integer[] preferredSize = uicontrol.getGridBagPreferredSize();
            if (preferredSize[0].equals(-1) == false && preferredSize[1].equals(-1) == false) {
                ((Component) member).setPreferredSize(new Dimension(preferredSize[0], preferredSize[1]));
            }

            component.getWidgetPane().add((Component) member, constraints);
            component.getWidgetPane().revalidate();
        } else {
            if (member instanceof SwingScilabScrollableFrame || member instanceof SwingScilabFrame) {
                component.getWidgetPane().add((Component) member, 0);
            } else {
                component.getWidgetPane().add((Component) member, 0);
            }
        }
    }

    protected static void removeMember(SwingScilabPanel component, SwingViewObject member) {
        component.getWidgetPane().remove((Component) member);
    }
}
