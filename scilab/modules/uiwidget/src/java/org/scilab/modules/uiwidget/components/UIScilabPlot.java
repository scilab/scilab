/*
 * Uicontrol2 ( http://forge.scilab.org/index.php/p/uicontrol2/ ) - This file is a part of Uicontrol2
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.uiwidget.components;

import java.awt.Dimension;
import java.awt.Window;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.Map;

import javax.swing.JPanel;
import javax.swing.SwingUtilities;
import javax.swing.event.AncestorEvent;
import javax.swing.event.AncestorListener;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIComponentAnnotation;
import org.scilab.modules.uiwidget.UIWidgetException;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.core.Scilab;
import org.scilab.modules.graphic_objects.GraphicObjectBuilder;
import org.scilab.modules.graphic_objects.ScilabNativeView;
import org.scilab.modules.graphic_objects.CallGraphicController;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicView.GraphicView;
import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;

public class UIScilabPlot extends UIComponent implements GraphicView {

    WindowAdapter winAdapter;
    Window parentWindow;
    JPanel plot;
    Figure figure;
    SwingScilabCanvas canvas;
    String onrotate;
    String onzoom;
    boolean hasAction;

    public UIScilabPlot(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        return null;
    }

    @UIComponentAnnotation(attributes = {"canvas", "onrotate", "onzoom", "figure-id"})
    public Object newInstance(boolean isCanvas, String onrotate, String onzoom, int figureId) {
        this.onrotate = onrotate;
        this.onzoom = onzoom;
        this.hasAction = (onrotate != null && !onrotate.isEmpty()) || (onzoom != null && !onzoom.isEmpty());

        figure = GraphicObjectBuilder.createNewFigureWithAxes(figureId == Integer.MAX_VALUE ? null : figureId);
        int id = CallGraphicController.getGraphicObjectPropertyAsInteger(figure.getIdentifier(), GraphicObjectProperties.__GO_ID__);
        canvas = new SwingScilabCanvas(id, figure, isCanvas, true);
        winAdapter = new WindowAdapter() {
            public void windowClosed(WindowEvent e) {
                GraphicController.getController().unregister(UIScilabPlot.this);
                ScilabNativeView.ScilabNativeView__deleteObject(figure.getIdentifier());
            }
        };

        canvas.addComponentListener(new ComponentAdapter() {
            public void componentResized(ComponentEvent e) {
                Integer[] newSize = new Integer[] {canvas.getWidth(), canvas.getHeight()};
                GraphicController.getController().setProperty(figure.getIdentifier(), GraphicObjectProperties.__GO_SIZE__, newSize);

                Boolean autoreSize = (Boolean) GraphicController.getController().getProperty(figure.getIdentifier(), GraphicObjectProperties.__GO_AUTORESIZE__);

                if (autoreSize != null && autoreSize) {
                    Integer[] newAxesSize = new Integer[] {canvas.getWidth(), canvas.getHeight()};
                    GraphicController.getController().setProperty(figure.getIdentifier(), GraphicObjectProperties.__GO_AXES_SIZE__, newAxesSize);
                }
            }
        });

        canvas.addAncestorListener(new AncestorListener() {
            public void ancestorAdded(AncestorEvent event) {
                GraphicController.getController().register(UIScilabPlot.this);
                Window win = SwingUtilities.getWindowAncestor(canvas);
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

        Scilab.registerFinalHook(new Runnable() {

            public void run() {
                GraphicController.getController().unregister(UIScilabPlot.this);
            }
        });

        // Trick to be sure that the canvas could be resized when in a UISplit
        canvas.setMinimumSize(new Dimension(0, 0));

        return canvas;
    }

    public int getFigureId() {
        return CallGraphicController.getGraphicObjectPropertyAsInteger(figure.getIdentifier(), GraphicObjectProperties.__GO_ID__);
    }

    public void setOnrotate(String onrotate) {
        this.onrotate = onrotate;
        this.hasAction = (onrotate != null && !onrotate.isEmpty()) || (onzoom != null && !onzoom.isEmpty());
    }

    public void setOnzoom(String onzoom) {
        this.onzoom = onzoom;
        this.hasAction = (onrotate != null && !onrotate.isEmpty()) || (onzoom != null && !onzoom.isEmpty());
    }

    public String getOnrotate() {
        return onrotate;
    }

    public String getOnzoom() {
        return onzoom;
    }

    @Override
    public void updateObject(String id, int property) {
        if (hasAction && GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__) == ((Integer) GraphicObjectProperties.__GO_AXES__)) {
            GraphicObject obj = GraphicController.getController().getObjectFromId(id);
            String parentFigure = obj.getParentFigure();
            if (figure.getIdentifier().equals(parentFigure)) {
                if (property == GraphicObjectProperties.__GO_ROTATION_ANGLES__ && onrotate != null && !onrotate.isEmpty()) {
                    Double[] angles = (Double[]) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_ROTATION_ANGLES__);
                    InterpreterManagement.requestScilabExec(onrotate + "(\"" + UIScilabPlot.this.getUIPath() + "\",[" + angles[0].toString() + "," + angles[1].toString() + "])");
                } else if (property == GraphicObjectProperties.__GO_ZOOM_BOX__ && onzoom != null && !onzoom.isEmpty()) {
                    Double[] box = (Double[]) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_ZOOM_BOX__);
                    if (box.length == 6) {
                        InterpreterManagement.requestScilabExec(onzoom + "(\"" + UIScilabPlot.this.getUIPath() + "\",[" + box[0].toString() + "," + box[1].toString() + "," + box[2].toString() + "," + box[3].toString() + "," + box[4].toString() + "," + box[5].toString() + "])");
                    } else {
                        InterpreterManagement.requestScilabExec(onzoom + "(\"" + UIScilabPlot.this.getUIPath() + "\",[])");
                    }
                }
            }
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
