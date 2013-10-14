/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Gustavo Barbosa Libotte
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.datatip;


import org.scilab.modules.gui.datatip.DatatipCommon;
import org.scilab.modules.gui.datatip.DatatipOrientation;

import org.scilab.modules.renderer.CallRenderer;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;
import org.scilab.modules.gui.editor.CommonHandler;


/**
 * Move a datatip along the curve
 * @author Gustavo Barbosa Libotte
 */
public class DatatipMove {

    public static double[] graphCoordDouble = new double[3];
    public static Integer[] coordInteger = new Integer[2];

    /**
    * Move a datatip to the right using keyboard
    *
    * @param markerUid datatip marker unique identifier
    */
    public static void moveRight(Integer datatipUid) {
        move(datatipUid, 1, 2);
    }


    /**
    * Move a datatip to the left using keyboard
    *
    * @param datatipUid datatip unique identifier
    */
    public static void moveLeft(Integer datatipUid) {
        move(datatipUid, -1, 0);
    }

    /*
     * moves the datatip in the given dir(X axis)
     * seg_offset is used when interp_mode is off so the
     * datatip is shifted in the right direction (0 -> left, 2 -> right,
     * the number 0 and 2 is because getSegment returns one segment more
     * the right would be -1, 1. Probably this is because float round error.
     */
    private static void move(Integer datatipUid, int dir, int seg_offset) {

        Integer parentPolyline = DatatipCommon.getParentPolyline(datatipUid);

        if (parentPolyline != null) {
            Integer figure = (Integer)GraphicController.getController().getProperty(datatipUid, __GO_PARENT_FIGURE__);
            Boolean useInterp = (Boolean)GraphicController.getController().getProperty(datatipUid, __GO_DATATIP_INTERP_MODE__);
            Integer[] pos = getCoordInteger(datatipUid);


            DatatipCommon.Segment seg;
            Double[] newPos;
            if (useInterp) {
                pos[0] += dir;
                double[] c2d = DatatipCommon.getTransformedPositionInViewScale(figure, pos);
                seg = DatatipCommon.getSegment(c2d[0], parentPolyline);
                newPos = DatatipCommon.Interpolate(c2d[0], seg);
            } else {
                double[] c2d = DatatipCommon.getTransformedPositionInViewScale(figure, pos);
                seg = DatatipCommon.getSegment(c2d[0], parentPolyline, seg_offset);
                newPos = new Double[] {seg.x0, seg.y0, 0.0};
            }

            Integer axes = (Integer)GraphicController.getController().getProperty(parentPolyline, __GO_PARENT_AXES__);
            boolean[] logFlags = new boolean[] {(Boolean)GraphicController.getController().getProperty(axes, __GO_X_AXIS_LOG_FLAG__),
                                                (Boolean)GraphicController.getController().getProperty(axes, __GO_Y_AXIS_LOG_FLAG__),
                                                (Boolean)GraphicController.getController().getProperty(axes, __GO_Z_AXIS_LOG_FLAG__)
                                               };

            newPos[0] = CommonHandler.InverseLogScale(newPos[0], logFlags[0]);
            newPos[1] = CommonHandler.InverseLogScale(newPos[1], logFlags[1]);
            newPos[2] = CommonHandler.InverseLogScale(newPos[2], logFlags[2]);

            GraphicController.getController().setProperty(datatipUid, __GO_DATATIP_DATA__, newPos);

            Boolean AutoOrientation = (Boolean)GraphicController.getController().getProperty(datatipUid, __GO_DATATIP_AUTOORIENTATION__);
            if (AutoOrientation) {
                DatatipOrientation.setOrientation(datatipUid, seg);
            }

        }
    }

    /**
    * Get the pixel integer coordinates of a datatip
    *
    * @param datatipUid Datatip unique identifier
    * @return Array with x, y coordinates
    */
    public static Integer[] getCoordInteger(Integer datatipUid) {

        Double[] markerPosition = (Double[]) GraphicController.getController().getProperty(datatipUid, __GO_DATATIP_DATA__);
        for (int i = 0 ; i < graphCoordDouble.length ; i++) {
            graphCoordDouble[i] = markerPosition[i];
        }

        Integer axes = (Integer)GraphicController.getController().getProperty(datatipUid, __GO_PARENT_AXES__);
        if (axes != null) {
            double[] pixelCoordinates = CallRenderer.getPixelFrom2dViewCoordinates(axes, graphCoordDouble);
            int xInt = (int) pixelCoordinates[0];
            int yInt = (int) pixelCoordinates[1];
            coordInteger[0] = (Integer) xInt;
            coordInteger[1] = (Integer) yInt;
            return coordInteger;
        }
        return null;
    }

}
