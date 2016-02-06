/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
 * Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
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

package org.scilab.modules.gui.editor;

import java.awt.event.MouseEvent;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.renderer.CallRenderer;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;
import org.scilab.modules.renderer.JoGLView.axes.AxesDrawer;
import org.scilab.modules.graphic_objects.PolylineData;
import org.scilab.modules.localization.Messages;
import org.scilab.forge.scirenderer.tranformations.Vector3d;

/**
 * Polyline data editor.
 * Implements point insertion/ deletion and translation.
 *
 * @author Caio Souza <caioc2bolado@gmail.com>
 * @author Pedro Souza <bygrandao@gmail.com>
 *
 * @since 2012-07-12
 */
public class DataEditor {


    JPopupMenu menu;
    JMenuItem remove, insert, exit;

    polylineStyle savedStyle;
    Integer curPolyline = null;
    EntityPicker picker;
    EntityPicker.PickedPoint picked = null;
    Integer axes;
    Integer rClickPos[] = {0, 0};


    /**Class to save polyline style*/
    public class polylineStyle {
        Integer lineFgColor;
        Integer lineBgColor;
        Double lineSize;
        Integer lineStyle;
        Boolean lineEnabled;
        Boolean filed;
        Boolean closed;
        Boolean interp;

        Integer markFgColor;
        Integer markBgColor;
        Boolean markEnabled;
        Integer markStyle;
        Integer markSizeUnit;
        Integer markSize;

        boolean canRestore = false;

        void Save(Integer polyline) {
            lineFgColor = (Integer)GraphicController.getController().getProperty(polyline, GraphicObjectProperties.__GO_LINE_COLOR__);
            lineBgColor = (Integer)GraphicController.getController().getProperty(polyline, GraphicObjectProperties.__GO_BACKGROUND__);
            lineSize = (Double)GraphicController.getController().getProperty(polyline, GraphicObjectProperties.__GO_LINE_THICKNESS__);
            lineEnabled = (Boolean)GraphicController.getController().getProperty(polyline, GraphicObjectProperties.__GO_LINE_MODE__);
            lineStyle = (Integer)GraphicController.getController().getProperty(polyline, GraphicObjectProperties.__GO_POLYLINE_STYLE__);
            filed = (Boolean)GraphicController.getController().getProperty(polyline, GraphicObjectProperties.__GO_FILL_MODE__);
            closed = (Boolean)GraphicController.getController().getProperty(polyline, GraphicObjectProperties.__GO_CLOSED__);
            interp = (Boolean)GraphicController.getController().getProperty(polyline, GraphicObjectProperties.__GO_INTERP_COLOR_VECTOR_SET__);

            markFgColor = (Integer)GraphicController.getController().getProperty(polyline, GraphicObjectProperties.__GO_MARK_FOREGROUND__);
            markBgColor = (Integer)GraphicController.getController().getProperty(polyline, GraphicObjectProperties.__GO_MARK_BACKGROUND__);
            markEnabled = (Boolean)GraphicController.getController().getProperty(polyline, GraphicObjectProperties.__GO_MARK_MODE__);
            markStyle = (Integer)GraphicController.getController().getProperty(polyline, GraphicObjectProperties.__GO_MARK_STYLE__);
            markSizeUnit = (Integer)GraphicController.getController().getProperty(polyline, GraphicObjectProperties.__GO_MARK_SIZE_UNIT__);
            markSize = (Integer)GraphicController.getController().getProperty(polyline, GraphicObjectProperties.__GO_MARK_SIZE__);
            canRestore = true;
        }

        void Restore(Integer polyline) {
            if (canRestore) {
                GraphicController.getController().setProperty(polyline, GraphicObjectProperties.__GO_LINE_COLOR__, lineFgColor);
                GraphicController.getController().setProperty(polyline, GraphicObjectProperties.__GO_BACKGROUND__, lineBgColor);
                GraphicController.getController().setProperty(polyline, GraphicObjectProperties.__GO_LINE_THICKNESS__, lineSize);
                GraphicController.getController().setProperty(polyline, GraphicObjectProperties.__GO_LINE_MODE__, lineEnabled);
                GraphicController.getController().setProperty(polyline, GraphicObjectProperties.__GO_POLYLINE_STYLE__, lineStyle);
                GraphicController.getController().setProperty(polyline, GraphicObjectProperties.__GO_FILL_MODE__, filed);
                GraphicController.getController().setProperty(polyline, GraphicObjectProperties.__GO_CLOSED__, closed);
                GraphicController.getController().setProperty(polyline, GraphicObjectProperties.__GO_INTERP_COLOR_VECTOR_SET__, interp);

                GraphicController.getController().setProperty(polyline, GraphicObjectProperties.__GO_MARK_FOREGROUND__, markFgColor);
                GraphicController.getController().setProperty(polyline, GraphicObjectProperties.__GO_MARK_BACKGROUND__, markBgColor);
                GraphicController.getController().setProperty(polyline, GraphicObjectProperties.__GO_MARK_MODE__, markEnabled);
                GraphicController.getController().setProperty(polyline, GraphicObjectProperties.__GO_MARK_STYLE__, markStyle);
                GraphicController.getController().setProperty(polyline, GraphicObjectProperties.__GO_MARK_SIZE_UNIT__, markSizeUnit);
                GraphicController.getController().setProperty(polyline, GraphicObjectProperties.__GO_MARK_SIZE__, markSize);
            }
        }

        /**Polyline style used by data editor*/
        private polylineStyle setDefaulStyle() {
            lineFgColor = -3;
            lineBgColor = -2;
            lineSize = 1.0;
            lineStyle = 1;
            lineEnabled = true;
            filed = false;
            closed = false;
            interp = false;

            markFgColor = -3;
            markBgColor = -2;
            markEnabled = true;
            markStyle = 11;
            markSizeUnit = 0;
            markSize = 6;
            canRestore = true;
            return this;
        }

        /**Switch to data editor polyline style*/
        public void changeToEditStyle(Integer polyline) {
            polylineStyle ps = new polylineStyle();
            ps.setDefaulStyle();
            ps.Restore(polyline);
        }
    }

    public DataEditor() {
        init();
    }

    /**Initializes and setup the class*/
    private void init() {
        menu = new JPopupMenu();

        insert = new JMenuItem(Messages.gettext("Insert"));
        insert.setToolTipText(Messages.gettext("Insert a new point here"));
        remove = new JMenuItem(Messages.gettext("Remove"));
        remove.setToolTipText(Messages.gettext("Remove this point"));
        exit = new JMenuItem(Messages.gettext("Exit"));
        exit.setToolTipText(Messages.gettext("Leave data editor mode"));

        insert.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickInsert(rClickPos);
            }
        });

        remove.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                onClickRemove(picked.point);
            }
        });

        menu.add(insert);
        menu.add(remove);
        menu.add(exit);

        savedStyle = new polylineStyle();
        picker = new EntityPicker();
    }

    /**
     * Set the exit menu item action
     * @param action The ActionListener.
     */
    public void setLeaveAction(ActionListener action) {
        exit.addActionListener(action);
    }


    /**
     * Begin data edition in the given polyline.
     * @param polyline The polyline to be edited.
     */
    public void beginEdit(Integer polyline) {
        savedStyle.Save(polyline);
        savedStyle.changeToEditStyle(polyline);
        curPolyline = polyline;
        axes = (new ObjectSearcher()).searchParent(curPolyline, GraphicObjectProperties.__GO_AXES__);
    }

    /**
     * End data edition in the given polyline and restore its style.
     */
    public void endEdit() {
        savedStyle.Restore(curPolyline);
    }

    /**
     * Try pick a point or segment on left mouse pressed.
     * @param event The mouse event.
     */
    public void onLeftMouseDown(MouseEvent event) {
        picked = picker.pickPoint(curPolyline, event.getX(), event.getY());
        if (picked != null && picked.point != -1) {
            GraphicObject go = 	GraphicController.getController().getObjectFromId(curPolyline);
            Integer figureUid = go.getParentFrameOrFigure();
            DrawerVisitor.getVisitor(figureUid).getInteractionManager().setTranslationEnable(false);
        }
    }

    public void onLeftMouseRelease(MouseEvent event) {
        if (picked != null && picked.point != -1) {
            GraphicObject go = 	GraphicController.getController().getObjectFromId(curPolyline);
            Integer figureUid = go.getParentFrameOrFigure();
            DrawerVisitor.getVisitor(figureUid).getInteractionManager().setTranslationEnable(true);
        }
    }

    /**
     * If there is a picked point translate it..
     * @param lastClick Initial mouse position (x, y).
     * @param newClick Final mouse position (x, y).
     */
    public void onDrag(Integer[] lastClick, Integer[] newClick) {
        if (picked != null && picked.point != -1) {
            if (!picked.isSegment) {
                double[] datax = (double[])PolylineData.getDataX(curPolyline);
                double[] datay = (double[])PolylineData.getDataY(curPolyline);
                double[] dataz = (double[])PolylineData.getDataZ(curPolyline);

                //Current point
                Vector3d planePoint = new Vector3d(datax[picked.point], datay[picked.point], dataz[picked.point]);
                Vector3d planeNorm = new Vector3d(0.0, 0.0, 1.0);
                //2d coords for current click
                double[] pos = {newClick[0].doubleValue(), newClick[1].doubleValue(), 1.0};
                double[] c2d = CallRenderer.get2dViewFromPixelCoordinates(axes, pos);

                Axes axesObj = AxesHandler.getAxesFromUid(axes);
                //3d ray for the current click
                double[] c3d1 = AxesDrawer.compute3dViewCoordinates(axesObj, c2d);
                c2d[2] += 1.0;
                double[] c3d2 = AxesDrawer.compute3dViewCoordinates(axesObj, c2d);

                Vector3d v0 = new Vector3d(c3d1);
                Vector3d v1 = new Vector3d(c3d2);
                Vector3d dir = v1.minus(v0);
                //if the dir is parallel to the plane there isn't intersection
                if (dir.getZ() == 0) {
                    return;
                }
                //General plane intersection
                //double u = planePoint.minus(v0).scalar(planeNorm) /  dir.scalar(planeNorm);
                double u = (planePoint.getZ() - v0.getZ()) / dir.getZ();
                Vector3d pointNew = dir.times(u);
                pointNew = pointNew.plus(v0);

                PolylineData.translatePoint(curPolyline, picked.point, pointNew.getX() - planePoint.getX(), pointNew.getY() - planePoint.getY(), 0.0 , 0, 0, 0);

            } else {
                PolylineHandler.getInstance().dragPolyline(curPolyline, lastClick, newClick);
            }
        }
    }

    /**
     * Try pick a point or segment and show the popup menu.
     * @param event The mouse event.
     */
    public void onRightClick(MouseEvent event) {
        picked = picker.pickPoint(curPolyline, event.getX(), event.getY());
        rClickPos[0] = event.getX();
        rClickPos[1] = event.getY();
        if (picked.point != -1) {
            /*enable delete menu option*/
            if (!picked.isSegment) {
                remove.setEnabled(true);
                insert.setEnabled(false);
            } else {
                /*enable add point menu*/
                remove.setEnabled(false);
                insert.setEnabled(true);
            }
        } else {
            remove.setEnabled(false);
            insert.setEnabled(false);
        }
        menu.show(event.getComponent(), event.getX(), event.getY());
    }

    /**
     * Try pick a point/segment and remove/insert the/a point.
     * @param event The mouse event.
     */
    public void onLeftDoubleClick(MouseEvent event) {
        picked = picker.pickPoint(curPolyline, event.getX(), event.getY());
        if (picked.point != -1) {
            /*double click over a point delete it*/
            if (!picked.isSegment) {
                PolylineData.removePoint(curPolyline, picked.point);
            } else {
                /*double click over a segment insert a new point*/
                double[] pos = {1.0 * event.getX(), 1.0 * event.getY(), 1.0};
                double[] c2d = CallRenderer.get2dViewFromPixelCoordinates(axes, pos);
                double[] point = CommonHandler.computeIntersection(curPolyline, picked.point, c2d);
                PolylineData.insertPoint(curPolyline, picked.point, point[0], point[1], point[2]);
            }
        }
    }

    /**
     * Implements remove menu action.
     * @param point index to remove.
     */
    void onClickRemove(int index) {
        PolylineData.removePoint(curPolyline, index);
    }

    /**
     * Implements insert menu action.
     * @param clicPos Position (x, y) where the will be inserted.
     */
    void onClickInsert(Integer[] clickPos) {
        picked = picker.pickPoint(curPolyline, clickPos[0], clickPos[1]);
        double[] pos = {1.0 * clickPos[0], 1.0 * clickPos[1], 1.0};
        double[] c2d = CallRenderer.get2dViewFromPixelCoordinates(axes, pos);
        double[] point = CommonHandler.computeIntersection(curPolyline, picked.point, c2d);
        PolylineData.insertPoint(curPolyline, picked.point, point[0], point[1], point[2]);
    }
}
