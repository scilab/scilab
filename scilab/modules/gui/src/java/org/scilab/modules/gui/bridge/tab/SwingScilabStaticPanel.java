/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Bruno JOFRET
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
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_SIZE__;

import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.event.HierarchyBoundsListener;
import java.awt.event.HierarchyEvent;

import javax.swing.ImageIcon;
import javax.swing.JLayeredPane;
import javax.swing.SwingUtilities;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.SwingViewWidget;
import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.editor.EditorEventListener;
import org.scilab.modules.gui.events.ScilabEventListener;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.widget.Widget;

public class SwingScilabStaticPanel extends SwingScilabScrollPane implements SwingScilabPanel {
    private static final long serialVersionUID = -3887923938827929317L;
    private Integer id;
    private TextBox infoBar;
    private MenuBar menuBar;
    private ToolBar toolBar;
    private String parentWindowId;
    private JLayeredPane uiContentPane;
    private JLayeredPane layeredPane;
    private boolean eventEnabled = false;
    private ComponentListener componentListener;
    private HierarchyBoundsListener ancestorListener;

    /** The listener for event handling */
    private ScilabEventListener eventHandler;
    private EditorEventListener editorEventHandler = null;

    private SwingScilabCanvas contentCanvas;
    protected boolean hasLayout;
    private Dimension deltaSize = null;

    private CommonCallBack callback;

    public SwingScilabStaticPanel(String figureTitle, Integer figureId, Figure figure) {
        super(new JLayeredPane(), new JLayeredPane(), figure);

        editorEventHandler = new EditorEventListener(figure.getIdentifier());

        uiContentPane = (JLayeredPane) getUIComponent();
        layeredPane = (JLayeredPane) getGlobalComponent();
        setVisible(true);
        layeredPane.setLayout(null);
        layeredPane.setOpaque(true);
        setHasLayout(false);

        uiContentPane.setOpaque(false);
        uiContentPane.setLayout(null);
        layeredPane.add(uiContentPane, JLayeredPane.DEFAULT_LAYER + 1, 0);

        layeredPane.setVisible(true);
        uiContentPane.setVisible(true);

        /* Manage figure_position property */
        ancestorListener = new HierarchyBoundsListener() {
            public void ancestorResized(HierarchyEvent arg0) {
            }

            public void ancestorMoved(HierarchyEvent e) {
                if (e.getChanged() instanceof SwingScilabWindow) {
                    Position parentPosition = SwingScilabWindow.allScilabWindows.get(parentWindowId).getPosition();
                    Integer[] newPosition = new Integer[] { parentPosition.getX(), parentPosition.getY() };
                    GraphicController.getController().setProperty(id, __GO_POSITION__, newPosition);
                }
            }
        };
        addHierarchyBoundsListener(ancestorListener);

        /* Manage figure_size property */
        componentListener = new ComponentListener() {
            public void componentShown(ComponentEvent arg0) {
            }

            public void componentResized(ComponentEvent arg0) {

                /* Update the figure_size property */
                Size parentSize = SwingScilabWindow.allScilabWindows.get(parentWindowId).getDims();
                Integer[] newSize = new Integer[] { parentSize.getWidth(), parentSize.getHeight() };

                GraphicController.getController().setProperty(id, __GO_SIZE__, newSize);

                Boolean autoreSize = (Boolean) GraphicController.getController().getProperty(id, __GO_AUTORESIZE__);

                if (autoreSize != null && autoreSize) {
                    /* Update the axes_size property */
                    Integer[] newAxesSize = new Integer[] { getContentPane().getWidth(), getContentPane().getHeight() };
                    GraphicController.getController().setProperty(id, __GO_AXES_SIZE__, newAxesSize);
                }

                String resizeFcn = (String) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_RESIZEFCN__);
                if (resizeFcn != null && !resizeFcn.equals("")) {
                    String resizeCommand = "if exists(\"gcbo\") then %oldgcbo = gcbo; end;"
                                           + "gcbo = getcallbackobject(" + id + ");"
                                           + resizeFcn
                                           + ";if exists(\"%oldgcbo\") then gcbo = %oldgcbo; else clear gcbo; end;";
                    InterpreterManagement.requestScilabExec(resizeCommand);
                } else if (hasLayout == false) {
                    for (Component comp : getWidgetPane().getComponents()) {
                        if (comp instanceof Widget) {
                            Widget widget = (Widget) comp;
                            SwingViewObject obj = (SwingViewObject) comp;
                            SwingViewWidget.update(widget, __GO_POSITION__, GraphicController.getController().getProperty(obj.getId(), __GO_POSITION__));
                        }
                    }
                }
            }

            public void componentMoved(ComponentEvent arg0) {
            }

            public void componentHidden(ComponentEvent arg0) {
            }
        };
        addComponentListener(componentListener);
    }

    /**
     * Turn on event handling.
     */
    private void enableEventHandler() {
        if (contentCanvas != null) {
            contentCanvas.addEventHandlerKeyListener(eventHandler);
            contentCanvas.addEventHandlerMouseListener(eventHandler);
            contentCanvas.addEventHandlerMouseMotionListener(eventHandler);
        }
    }

    /**
     * Turn off event handling.
     */
    private void disableEventHandler() {
        if (eventHandler != null && contentCanvas != null) {
            contentCanvas.removeEventHandlerKeyListener(eventHandler);
            contentCanvas.removeEventHandlerMouseListener(eventHandler);
            contentCanvas.removeEventHandlerMouseMotionListener(eventHandler);
        }
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public Integer getId() {
        return id;
    }

    public void update(int property, Object value) {
        SwingScilabCommonPanel.update(this, property, value);
    }

    public void setFigureBackground(Color color) {
        if (layeredPane != null) {
            layeredPane.setBackground(color);
        }
    }

    public TextBox getInfoBar() {
        return infoBar;
    }

    public void setInfoBar(TextBox infoBar) {
        this.infoBar = infoBar;
    }

    public MenuBar getMenuBar() {
        return menuBar;
    }

    public void setMenuBar(MenuBar menuBar) {
        this.menuBar = menuBar;
    }

    public ToolBar getToolBar() {
        return toolBar;
    }

    public void setToolBar(ToolBar toolBar) {
        this.toolBar = toolBar;
    }

    /**
     * Set the event handler of the Canvas
     * @param funName the name of the Scilab function to call
     */
    public void setEventHandler(String funName) {
        disableEventHandler();
        eventHandler = new ScilabEventListener(funName, getId(), false);
        if (eventEnabled) {
            editorEventHandler.setEnable(false);
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
            editorEventHandler.setEnable(false);
            enableEventHandler();
            eventEnabled = true;
        } else {
            editorEventHandler.setEnable(true);
            disableEventHandler();
            eventEnabled = false;
        }
    }


    public void setParentWindowId(String parentWindowId) {
        this.parentWindowId = parentWindowId;
        SwingScilabWindow.allScilabWindows.get(parentWindowId).setIconImage(new ImageIcon(FindIconHelper.findIcon("graphic-window", "256x256")).getImage());
    }

    public void setWindowIcon(String windowIcon) {
    }

    public void addMember(SwingViewObject member) {
        if (member instanceof SwingScilabAxes) {
            if (contentCanvas == null) {
                contentCanvas = new SwingScilabCanvas((Figure) GraphicController.getController().getObjectFromId(((SwingScilabAxes) member).getFigureId()));
                contentCanvas.addEventHandlerKeyListener(editorEventHandler);
                contentCanvas.addEventHandlerMouseListener(editorEventHandler);
                contentCanvas.addEventHandlerMouseMotionListener(editorEventHandler);
                if (eventEnabled) {
                    editorEventHandler.setEnable(false);
                    enableEventHandler();
                }

                layeredPane.add(contentCanvas, JLayeredPane.FRAME_CONTENT_LAYER);
                setCanvas(contentCanvas);
            }
            return;
        }

        SwingScilabCommonPanel.addMember(this, member);
    }

    public String getParentWindowId() {
        return parentWindowId;
    }

    public SwingScilabWindow getParentWindow() {
        return (SwingScilabWindow) SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, this);
    }

    public JLayeredPane getWidgetPane() {
        return uiContentPane;
    }

    public void setCallback(CommonCallBack callback) {
        this.callback = callback;
    }

    public CommonCallBack getCallback() {
        return callback;
    }

    public Container getContentPane() {
        return this.getAsContainer();
    }

    public void close() {
        setMenuBar(null);
        setToolBar(null);
        setInfoBar(null);
        removeAll();
        // without this children canvas are not released.
        SwingScilabWindow win = getParentWindow();
        Container dummyContainer = new Container();
        win.setContentPane(dummyContainer);
        win.close();
        setVisible(false);
    }

    /**
     * Remove a SwingViewObject (from SwingView.java)
     * @param member the member to remove
     */
    public void removeMember(SwingViewObject member) {
        SwingScilabCommonPanel.removeMember(this, member);
    }

    public void setName(String name) {
        super.setName(name);
        getParentWindow().setName(name);
    }

    public void setHasLayout(boolean hasLayout) {
        this.hasLayout = hasLayout;
    }

    public void applyDeltaSize() {
        if (deltaSize != null) {
            if (deltaSize.getWidth() != 0 || deltaSize.getHeight() != 0) {
                //update view and update model ( from componentResize of Window )
                SwingScilabWindow figure = SwingScilabWindow.allScilabWindows.get(getParentWindowId());
                Size oldFigureSize = figure.getDims();
                figure.setDims(new Size((int)(oldFigureSize.getWidth() + deltaSize.getWidth()), (int)(oldFigureSize.getHeight() + deltaSize.getHeight())));
            }

            deltaSize = null;
        }
    }

    public void storeSizeDelta() {
        Dimension axesSize = getContentPane().getSize();
        SwingScilabWindow figure = SwingScilabWindow.allScilabWindows.get(getParentWindowId());
        Size figureSize = figure.getDims();
        deltaSize = new Dimension((int)(figureSize.getWidth() - axesSize.getWidth()), (int)(figureSize.getHeight() - axesSize.getHeight()));
    }

    public void disableResizeEvent() {
        removeComponentListener(componentListener);
    }

    public void enableResizeEvent() {
        addComponentListener(componentListener);
    }
}
