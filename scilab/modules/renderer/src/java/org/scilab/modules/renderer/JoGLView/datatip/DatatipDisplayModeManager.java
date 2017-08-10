/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2016 - Scilab Enterprises
 * Copyright (C) 2016 - Caio SOUZA
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.renderer.JoGLView.datatip;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_PARENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DATA_MODEL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_PARENT_AXES__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DATATIP_DISPLAY_MODE__;
import static org.scilab.modules.graphic_objects.polyline.Polyline.DatatipDisplayMode;

import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.datatip.Datatip;

import org.scilab.modules.renderer.JoGLView.axes.AxesDrawer;

import java.awt.Component;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.AWTEvent;
import java.util.List;
import java.util.LinkedList;
import java.util.HashMap;
import java.util.Map;

public class DatatipDisplayModeManager {

    List<Integer> clicked_datatips = new LinkedList<Integer>();


    static class Position {
        public double x;
        public double y;
        public Position(double x, double y) {
            this.x = x;
            this.y = y;
        }
    };

    Integer hover = 0;

    private Map<Integer, Position> datatips = new HashMap<Integer, Position>();

    public DatatipDisplayModeManager(Component component) {

        if (component != null) {
            component.addMouseListener(new MouseAdapter() {
                @Override
                public void mouseClicked(MouseEvent event) {
                    onMouseClick(event.getX(), event.getY());
                }
            });

            component.addMouseMotionListener(new MouseAdapter() {
                @Override
                public void mouseMoved(MouseEvent event) {
                    onMouseMove(event.getX(), event.getY());
                }
            });
        }
    }

    public void add(Integer uid) {
        GraphicController controller =  GraphicController.getController();
        Datatip datatip = (Datatip) controller.getObjectFromId(uid);
        Double p[] = datatip.getTipData();
        Integer auid = (Integer) controller.getProperty(uid, __GO_PARENT_AXES__);
        Axes axes = (Axes) controller.getObjectFromId(auid);
        if (axes != null) {
            double position[] = AxesDrawer.computePixelFrom3dCoordinates(axes, new double[] {p[0], p[1], p[2]});
            datatips.put(uid, new Position(position[0], position[1]));
        }
    }

    public void remove(Integer uid) {
        datatips.remove(uid);
        clicked_datatips.remove(uid);
    }

    public boolean needDraw(Integer uid) {
        add(uid);
        GraphicController controller =  GraphicController.getController();
        Integer puid = (Integer) controller.getProperty(uid, __GO_PARENT__);
        Integer mode = (Integer) controller.getProperty(puid, __GO_DATATIP_DISPLAY_MODE__);
        DatatipDisplayMode eMode = DatatipDisplayMode.intToEnum(mode);
        switch (eMode) {
            case ALWAYS:
                return true;
            case MOUSECLICK:
                return clicked_datatips.contains(uid);
            case MOUSEOVER:
                return hover.equals(uid);
            default:
                return true;
        }
    }

    private Integer get(int x, int y) {
        for (Map.Entry<Integer, Position> entry : datatips.entrySet()) {
            Integer uid = entry.getKey();
            Position p = entry.getValue();
            double x2 = (p.x - x * 1.0) * (p.x - x * 1.0);
            double y2 = (p.y - y * 1.0) * (p.y - y * 1.0);

            if ((x2 + y2) < 20.0) {
                return uid;
            }
        }
        return 0;
    }

    public void onMouseMove(int x, int y) {
        Integer datatip = get(x, y);
        if (datatip > 0) {
            if (!hover.equals(datatip)) {
                hover = datatip;
                //triger redreaw
                GraphicController.getController().setProperty(datatip, __GO_DATA_MODEL__, 0);
            }
        } else {
            if (hover != 0) {
                GraphicController.getController().setProperty(hover, __GO_DATA_MODEL__, 0);
                hover = 0;
            }
        }
    }

    public void onMouseClick(int x, int y) {
        Integer datatip = get(x, y);
        if (datatip > 0) {
            if (clicked_datatips.contains(datatip)) {
                clicked_datatips.remove(datatip);
            } else {
                clicked_datatips.add(datatip);
            }
            //triger redreaw
            GraphicController.getController().setProperty(datatip, __GO_DATA_MODEL__, 0);
        }
    }
}
