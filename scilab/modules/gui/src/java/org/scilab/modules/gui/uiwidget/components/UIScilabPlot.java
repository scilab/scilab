/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.uiwidget.components;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Color;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.Point;
import java.awt.Window;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.HierarchyBoundsListener;
import java.awt.event.HierarchyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import javax.media.opengl.GLAutoDrawable;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.SwingUtilities;
import javax.swing.Timer;
import javax.swing.event.AncestorEvent;
import javax.swing.event.AncestorListener;

import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.implementation.jogl.JoGLCanvasFactory;

import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIComponentAnnotation;
import org.scilab.modules.gui.uiwidget.UILayoutFactory;
import org.scilab.modules.gui.uiwidget.UIWidgetException;
import org.scilab.modules.gui.uiwidget.UIWidgetTools;

import org.scilab.modules.graphic_objects.GraphicObjectBuilder;
import org.scilab.modules.graphic_objects.ScilabNativeView;
import org.scilab.modules.graphic_objects.CallGraphicController;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicView.GraphicView;
import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;
import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvasImpl;
import org.scilab.modules.gui.events.GlobalEventWatcher;
import org.scilab.modules.gui.events.ScilabEventListener;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;
import org.scilab.modules.gui.uiwidget.callback.UICallback;

public class UIScilabPlot extends UIComponent implements GraphicView {

    static {
        try {
            System.loadLibrary("gluegen2-rt");
        } catch (Exception e) {
            System.err.println(e);
        }
    }

    final String[] pixel_drawing_modes = new String[] {"clear", "and", "andreverse", "copy", "andinverted", "noop", "xor", "or", "nor", "equiv", "invert", "orreverse", "copyinverted", "orinverted", "nand", "set"};

    protected Figure figure;
    protected Canvas canvas;
    protected Component drawableComponent;
    protected boolean isCanvas;
    protected JPanel panel;
    protected WindowAdapter winAdapter;
    protected Window parentWindow;
    protected ScilabEventListener eventHandler;
    protected boolean eventEnabled = false;
    protected UICallback onrotate;
    protected UICallback onzoom;
    protected boolean onrotateEnable = true;
    protected boolean onzoomEnable = true;
    protected boolean hasAction;
    protected GridBagConstraints gbc = new GridBagConstraints();
    protected boolean isResizing;
    protected boolean isMoving;
    protected Timer timer;
    protected List<UIComponent> toAddQueue;

    public UIScilabPlot(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public UIScilabPlot(UIComponent parent, Figure figure) throws UIWidgetException {
        super(parent);
        this.figure = figure;
    }

    public Object newInstance() {
        return null;
    }

    @UIComponentAnnotation(attributes = {"canvas", "onrotate", "onzoom", "figure-id"})
    public Object newInstance(boolean isCanvas, String onrotate, String onzoom, int figureId) {
        this.onrotate = UICallback.newInstance(this, onrotate);
        this.onzoom = UICallback.newInstance(this, onzoom);
        this.hasAction = (onrotate != null && !onrotate.isEmpty()) || (onzoom != null && !onzoom.isEmpty());
        this.drawableComponent = SwingScilabCanvasImpl.getInstance().createOpenGLComponent(isCanvas);
        this.isCanvas = (drawableComponent instanceof java.awt.Canvas);

        if (this.isCanvas) {
            panel = new JPanel(new BorderLayout()) {
                public void setSize(Dimension d) {
                    drawableComponent.setSize(d);
                }
            };
            panel.add(drawableComponent, BorderLayout.CENTER);
        } else {
            panel = (JPanel) drawableComponent;
            panel.setLayout(new NoLayout());
        }

        if (figure == null) {
            figure = GraphicObjectBuilder.createNewFigureWithAxes(figureId == Integer.MAX_VALUE ? null : figureId);
        }

        canvas = JoGLCanvasFactory.createCanvas((GLAutoDrawable) drawableComponent);
        DrawerVisitor drawerVisitor = new DrawerVisitor(drawableComponent, canvas, figure);
        canvas.setMainDrawer(drawerVisitor);

        drawableComponent.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseEntered(MouseEvent e) {
                GlobalEventWatcher.setAxesUID(figure.getIdentifier());
            }
        });

        winAdapter = new WindowAdapter() {
            public void windowClosed(WindowEvent e) {
                parentWindow.removeWindowListener(winAdapter);
                GraphicController.getController().unregister(UIScilabPlot.this);
                ScilabNativeView.ScilabNativeView__deleteObject(figure.getIdentifier());
            }
        };

        drawableComponent.addComponentListener(new ComponentAdapter() {
            public void componentResized(ComponentEvent e) {
                synchronized (UIScilabPlot.this) {
                    isResizing = true;
                }

                Integer[] newSize = new Integer[] {drawableComponent.getWidth(), drawableComponent.getHeight()};
                GraphicController.getController().setProperty(figure.getIdentifier(), GraphicObjectProperties.__GO_SIZE__, newSize);

                Boolean autoreSize = (Boolean) GraphicController.getController().getProperty(figure.getIdentifier(), GraphicObjectProperties.__GO_AUTORESIZE__);

                if (autoreSize != null && autoreSize) {
                    Integer[] newAxesSize = new Integer[] {newSize[0], newSize[1]};
                    GraphicController.getController().setProperty(figure.getIdentifier(), GraphicObjectProperties.__GO_AXES_SIZE__, newAxesSize);
                }

                synchronized (UIScilabPlot.this) {
                    isResizing = false;
                }
            }
        });

        drawableComponent.addHierarchyBoundsListener(new HierarchyBoundsListener() {
            public void ancestorResized(HierarchyEvent arg0) {
            }

            public void ancestorMoved(HierarchyEvent e) {
                if (e.getChanged() instanceof JFrame) {
                    synchronized (UIScilabPlot.this) {
                        isMoving = true;
                    }
                    JFrame win = (JFrame) e.getChanged();
                    Point p = win.getLocation();
                    Integer[] newPosition = new Integer[] {p.x, p.y};
                    GraphicController.getController().setProperty(figure.getIdentifier(), GraphicObjectProperties.__GO_POSITION__, newPosition);
                    synchronized (UIScilabPlot.this) {
                        isMoving = false;
                    }
                }
            }
        });

        panel.addAncestorListener(new AncestorListener() {
            public void ancestorAdded(AncestorEvent event) {
                GraphicController.getController().register(UIScilabPlot.this);
                Window win = SwingUtilities.getWindowAncestor(panel);
                if (win != parentWindow) {
                    if (parentWindow != null) {
                        parentWindow.removeWindowListener(winAdapter);
                    }
                    parentWindow = win;
                    if (parentWindow != null) {
                        parentWindow.addWindowListener(winAdapter);
                    }
                }
            }

            public void ancestorMoved(AncestorEvent event) { }

            public void ancestorRemoved(AncestorEvent event) {
                GraphicController.getController().unregister(UIScilabPlot.this);
            }
        });

        panel.setBackground(Color.white);
        panel.setFocusable(true);
        panel.setEnabled(true);

        // Trick to be sure that the canvas could be resized when in a UISplit
        panel.setMinimumSize(new Dimension(0, 0));

        return panel;
    }

    /**
     * {@inheritDoc}
     */
    public void add(final UIComponent c) throws UIWidgetException {
        Object o = c.getComponent();
        if (!isCanvas && o instanceof Component && !(o instanceof JPopupMenu)) {
            if (true || !panel.isVisible()) {
                UIPanel.addToPanel(panel, c, gbc);
            } else {
                synchronized (this) {
                    if (timer == null) {
                        toAddQueue = new LinkedList<UIComponent>();
                        toAddQueue.add(c);
                        timer = new Timer(10, new ActionListener() {
                            public void actionPerformed(ActionEvent e) {
                                final List<UIComponent> list;
                                synchronized (UIScilabPlot.this) {
                                    list = toAddQueue;
                                    toAddQueue = null;
                                    timer = null;
                                }
                                SwingUtilities.invokeLater(new Runnable() {
                                    public void run() {
                                        try {
                                            for (UIComponent c : list) {
                                                UIPanel.addToPanel(panel, c, gbc);
                                            }
                                            if (panel.isVisible()) {
                                                panel.revalidate();
                                            }
                                        } catch (Exception e ) {
                                            System.err.println(e);
                                        }
                                    }
                                });
                            }
                        });
                        timer.setRepeats(false);
                        timer.start();
                    } else {
                        timer.restart();
                        toAddQueue.add(c);
                    }
                }
            }
        } else {
            super.add(c);
        }
    }

    synchronized boolean isResizing() {
        return isResizing;
    }

    synchronized boolean isMoving() {
        return isMoving;
    }

    /**
     * {@inheritDoc}
     */
    public void setLayout(String layout) throws UIWidgetException {
        if (!isCanvas) {
            panel.setLayout(UILayoutFactory.getLayout(panel, layout));
            if (panel.isVisible()) {
                panel.revalidate();
            }
        }
    }

    public int getFigureId() {
        return (Integer)GraphicController.getController().getProperty(figure.getIdentifier(), GraphicObjectProperties.__GO_ID__);
    }

    public void setOnrotate(String onrotate) {
        this.onrotate = UICallback.newInstance(this, onrotate);
        this.hasAction = (this.onrotate != null || this.onzoom != null);
    }

    public void setOnzoom(String onzoom) {
        this.onzoom = UICallback.newInstance(this, onzoom);
        this.hasAction = (onrotate != null || this.onzoom != null);
    }

    public void setOnrotateEnable(boolean b) {
        this.onrotateEnable = b;
    }

    public void setOnzoomEnable(boolean b) {
        this.onzoomEnable = b;
    }

    public boolean getOnzoomEnable() {
        return this.onzoomEnable;
    }

    public boolean getOnrotateEnable() {
        return this.onrotateEnable;
    }

    public UICallback getOnrotate() {
        return onrotate;
    }

    public UICallback getOnzoom() {
        return onzoom;
    }

    public void setColorMap(Double[] colorMap) {
        GraphicController.getController().setProperty(figure.getIdentifier(), GraphicObjectProperties.__GO_COLORMAP__, colorMap);
    }

    public double[][] getColorMap() {
        Double[] colormap = (Double[]) GraphicController.getController().getProperty(figure.getIdentifier(), GraphicObjectProperties.__GO_COLORMAP__);
        if (colormap != null) {
            final int rows = colormap.length / 3;
            double[][] ret = new double[rows][3];
            for (int i = 0; i < rows; i++) {
                ret[i][0] = colormap[i];
                ret[i][1] = colormap[i + rows];
                ret[i][2] = colormap[i + rows * 2];
            }

            return ret;
        }

        return null;
    }

    public void setPixmap(boolean pixmap) {
        GraphicController.getController().setProperty(figure.getIdentifier(), GraphicObjectProperties.__GO_PIXMAP__, new Boolean(pixmap));
    }

    public boolean getPixmap() {
        return (Boolean) GraphicController.getController().getProperty(figure.getIdentifier(), GraphicObjectProperties.__GO_PIXMAP__);
    }

    public void setImmediateDrawing(boolean im) {
        GraphicController.getController().setProperty(figure.getIdentifier(), GraphicObjectProperties.__GO_IMMEDIATE_DRAWING__, new Boolean(im));
    }

    public boolean getImmediateDrawing() {
        return (Boolean) GraphicController.getController().getProperty(figure.getIdentifier(), GraphicObjectProperties.__GO_IMMEDIATE_DRAWING__);
    }

    public void setAntiAliasing(int aa) {
        GraphicController.getController().setProperty(figure.getIdentifier(), GraphicObjectProperties.__GO_ANTIALIASING__, new Integer(aa));
    }

    public int getAntiAliasing() {
        return (Integer) GraphicController.getController().getProperty(figure.getIdentifier(), GraphicObjectProperties.__GO_ANTIALIASING__);
    }

    public void setBackground(int index) {
        GraphicController.getController().setProperty(figure.getIdentifier(), GraphicObjectProperties.__GO_BACKGROUND__, new Integer(index));
    }

    public int getBackground() {
        return (Integer) GraphicController.getController().getProperty(figure.getIdentifier(), GraphicObjectProperties.__GO_BACKGROUND__);
    }

    public void setPixelDrawingMode(String mode) {
        if (mode != null && !mode.isEmpty()) {
            String low = mode.toLowerCase();
            int val = -1;
            for (int i = 0; i < pixel_drawing_modes.length; i++) {
                if (low.equals(pixel_drawing_modes[i])) {
                    val = i;
                    break;
                }
            }

            if (val != -1) {
                GraphicController.getController().setProperty(figure.getIdentifier(), GraphicObjectProperties.__GO_PIXEL_DRAWING_MODE__, new Integer(val));
            }
        }
    }

    public String getPixelDrawingMode() {
        int val = (Integer) GraphicController.getController().getProperty(figure.getIdentifier(), GraphicObjectProperties.__GO_PIXEL_DRAWING_MODE__);
        if (val < 0 || val >= pixel_drawing_modes.length) {
            return null;
        }

        return pixel_drawing_modes[val];
    }

    public void setRotationStyle(String style) {
        if (style != null && !style.isEmpty()) {
            String low = style.toLowerCase();
            if (low.equals("unary")) {
                GraphicController.getController().setProperty(figure.getIdentifier(), GraphicObjectProperties.__GO_ROTATION_TYPE__, new Integer(0));
            } else if (low.equals("multiple")) {
                GraphicController.getController().setProperty(figure.getIdentifier(), GraphicObjectProperties.__GO_ROTATION_TYPE__, new Integer(1));
            }
        }
    }

    public String getRotationStyle() {
        int val = (Integer) GraphicController.getController().getProperty(figure.getIdentifier(), GraphicObjectProperties.__GO_ROTATION_TYPE__);
        if (val == 0) {
            return "unary";
        } else {
            return "multiple";
        }
    }

    /*public void setEventHandler(String handler) {
      GraphicController.getController().setProperty(figure.getIdentifier(), GraphicObjectProperties.__GO_EVENTHANDLER_NAME__, handler);
      }*/

    public String getEventHandler() {
        return (String) GraphicController.getController().getProperty(figure.getIdentifier(), GraphicObjectProperties.__GO_EVENTHANDLER_NAME__);
    }

    /*public void setEventHandlerEnable(boolean enable) {
      GraphicController.getController().setProperty(figure.getIdentifier(), GraphicObjectProperties.__GO_EVENTHANDLER_ENABLE__, new Boolean(enable));
      }*/

    public boolean getEventHandlerEnable() {
        return (Boolean) GraphicController.getController().getProperty(figure.getIdentifier(), GraphicObjectProperties.__GO_EVENTHANDLER_ENABLE__);
    }

    @Override
    public void updateObject(String id, int property) {
        if ((hasAction && (onrotateEnable || onzoomEnable)) && GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__) == ((Integer) GraphicObjectProperties.__GO_AXES__)) {
            GraphicObject obj = GraphicController.getController().getObjectFromId(id);
            String parentFigure = obj.getParentFigure();
            if (figure.getIdentifier().equals(parentFigure)) {
                if (property == GraphicObjectProperties.__GO_ROTATION_ANGLES__ && onrotateEnable && onrotate != null) {
                    Double[] angles = (Double[]) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_ROTATION_ANGLES__);
                    UIWidgetTools.execAction(onrotate, "[" + angles[0].toString() + "," + angles[1].toString() + "]");
                } else if (property == GraphicObjectProperties.__GO_ZOOM_BOX__ && onzoomEnable && onzoom != null) {
                    Double[] box = (Double[]) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_ZOOM_BOX__);
                    if (box.length == 6) {
                        UIWidgetTools.execAction(onzoom, "[" + box[0].toString() + "," + box[1].toString() + "," + box[2].toString() + "," + box[3].toString() + "," + box[4].toString() + "," + box[5].toString() + "]");
                    } else {
                        UIWidgetTools.execAction(onzoom, "[]");
                    }
                }
            }
        }
    }

    /**
     * Set the event handler of the Canvas
     * @param funName the name of the Scilab function to call
     */
    void setEventHandler(String funName) {
        disableEventHandler();
        eventHandler = new ScilabEventListener(funName, figure.getIdentifier());//getId());
        if (eventEnabled) {
            enableEventHandler();
        }
    }

    /**
     * Set the status of the event handler of the Canvas
     * @param status is true to set the event handler active
     */
    void setEventHandlerEnabled(boolean status) {
        if (status && eventEnabled) {
            return;
        }

        if (status) {
            enableEventHandler();
            eventEnabled = true;
        } else {
            disableEventHandler();
            eventEnabled = false;
        }
    }

    /**
     * Turn on event handling.
     */
    private void enableEventHandler() {
        panel.addKeyListener(eventHandler);
        drawableComponent.addMouseListener(eventHandler);
        drawableComponent.addMouseMotionListener(eventHandler);
    }

    /**
     * Turn off event handling.
     */
    private void disableEventHandler() {
        if (eventHandler != null) {
            panel.removeKeyListener(eventHandler);
            drawableComponent.removeMouseListener(eventHandler);
            drawableComponent.removeMouseMotionListener(eventHandler);
        }
    }

    @Override
    public void createObject(String id) {
    }

    @Override
    public void deleteObject(String id) {
        if (figure.getIdentifier().equals(id)) {
            //GraphicController.getController().unregister(this);
        }
    }
}
