/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Gustavo Barbosa Libotte
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

package org.scilab.modules.gui.datatip;

import org.scilab.modules.graphic_objects.PolylineData;
import org.scilab.modules.gui.datatip.DatatipCommon;
import org.scilab.modules.gui.datatip.DatatipOrientation;
import org.scilab.modules.renderer.CallRenderer;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;
import org.scilab.modules.gui.editor.CommonHandler;
import org.scilab.modules.gui.editor.EntityPicker;

/**
 * Move a datatip along the curve
 * @author Gustavo Barbosa Libotte
 */
public class DatatipMove {

    /**
     * Move a datatip to the right using keyboard
     *
     * @param markerUid datatip marker unique identifier
     */
    public static void moveRight(Integer datatipUid) {
        move(datatipUid, 1);
    }

    /**
     * Move a datatip to the left using keyboard
     *
     * @param datatipUid datatip unique identifier
     */
    public static void moveLeft(Integer datatipUid) {
        move(datatipUid, -1);
    }

    /*
     * moves the datatip in the given dir(X axis)
     * seg_offset is used when interp_mode is off so the
     * datatip is shifted in the right direction (0 -> left, 2 -> right,
     * the number 0 and 2 is because getSegment returns one segment more
     * the right would be -1, 1. Probably this is because float round error.
     */
    private static void move(Integer datatipUid, int dir) {
        Integer parentPolyline = DatatipCommon.getParentPolyline(datatipUid);
        if (parentPolyline != null) {
            GraphicController controller = GraphicController.getController();
            Integer axes = (Integer) controller.getProperty(parentPolyline, __GO_PARENT_AXES__);
            Double[] data = (Double[]) controller.getProperty(datatipUid, __GO_DATATIP_DATA__);
            int index = (Integer) controller.getProperty(datatipUid, __GO_DATATIP_INDEXES__);
            DatatipCommon.Segment seg = EntityPicker.getSegment(parentPolyline, index);
            double[] datax = (double[]) PolylineData.getDataX(parentPolyline);
            Boolean useInterp = (Boolean) controller.getProperty(datatipUid, __GO_DATATIP_INTERP_MODE__) && CommonHandler.isLineEnabled(parentPolyline);
            Boolean AutoOrientation = (Boolean) controller.getProperty(datatipUid, __GO_DATATIP_AUTOORIENTATION__);

            if (useInterp) {
                double[] start = CallRenderer.getPixelFrom3dCoordinates(axes, new double[] {seg.x0, seg.y0, seg.z0});
                double[] end = CallRenderer.getPixelFrom3dCoordinates(axes, new double[] {seg.x1, seg.y1, seg.z1});
                final double ns = Math.hypot(end[0] - start[0], end[1] - start[1]);
                final double nsx = (2 * dir * (end[0] - start[0])) / ns;
                final double nsy = (2 * dir * (end[1] - start[1])) / ns;

                double[] pix_pos = CallRenderer.getPixelFrom3dCoordinates(axes, new double[] {data[0], data[1], data[2]});
                double[] info = EntityPicker.getNearestSegmentIndex(axes, parentPolyline, (int) (pix_pos[0] + nsx), (int) (pix_pos[1] + nsy));

                controller.setProperty(datatipUid, __GO_DATATIP_INDEXES__, new Double[] {info[0], info[1]});
                if (AutoOrientation) {
                    DatatipOrientation.setOrientation(datatipUid, EntityPicker.getSegment(parentPolyline, (int) info[0]));
                }
            } else {
                double ind = Math.min(Math.max(index + dir, 0), datax.length - 1);
                controller.setProperty(datatipUid, __GO_DATATIP_INDEXES__, new Double[] {ind, 0.});
                if (AutoOrientation) {
                    DatatipOrientation.setOrientation(datatipUid, EntityPicker.getSegment(parentPolyline, (int) ind));
                }
            }
        }
    }
}
