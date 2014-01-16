/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Gustavo Barbosa Libotte <gustavolibotte@gmail.com>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.datatip;

import org.scilab.modules.graphic_objects.PolylineData;

import org.scilab.modules.gui.datatip.DatatipCommon;
import org.scilab.modules.gui.datatip.DatatipOrientation;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;
import org.scilab.modules.gui.editor.CommonHandler;

/**
 * Drag a datatip along a polyline
 * @author Gustavo Barbosa Libotte
 */
public class DatatipDrag {

    /**
    * Drag a datatip over a polyline using mouse
    *
    * @param datatipUid Datatip unique identifier.
    * @param endX Integer with pixel mouse position x after mouse drag.
    * @param endY Integer with pixel mouse position y after mouse drag.
    */
    public static void dragDatatip(int datatipUid, int endX, int endY) {
        GraphicController controller = GraphicController.getController();
        Integer parentPolyline = DatatipCommon.getParentPolyline(datatipUid);

        if (parentPolyline != 0) {

            Integer pos[] = {endX, endY};
            Integer figure = (Integer)controller.getProperty(datatipUid, __GO_PARENT_FIGURE__);
            double[] c2d = DatatipCommon.getTransformedPositionInViewScale(figure, pos);


            Integer axesUid = (Integer)controller.getProperty(parentPolyline, __GO_PARENT_AXES__);
            Integer viewInfo = (Integer)controller.getProperty(axesUid, __GO_VIEW__);

            if (viewInfo == 0 && PolylineData.isZCoordSet(parentPolyline) == 0) {
                DatatipCommon.Segment seg = DatatipCommon.getSegment(c2d[0], parentPolyline);

                Boolean useInterp = (Boolean)controller.getProperty(datatipUid, __GO_DATATIP_INTERP_MODE__);
                Double[] newPos;
                if (useInterp) {
                    newPos = DatatipCommon.Interpolate(c2d[0], seg);
                } else {
                    newPos = new Double[] {seg.x0, seg.y0, 0.0};
                }

                boolean[] logFlags = getLogFlags (axesUid);

                newPos[0] = CommonHandler.InverseLogScale(newPos[0], logFlags[0]);
                newPos[1] = CommonHandler.InverseLogScale(newPos[1], logFlags[1]);
                newPos[2] = CommonHandler.InverseLogScale(newPos[2], logFlags[2]);

                //controller.setProperty(datatipUid, __GO_DATATIP_DATA__, newPos);

                Boolean AutoOrientation = (Boolean)controller.getProperty(datatipUid, __GO_DATATIP_AUTOORIENTATION__);
                if (AutoOrientation) {
                    DatatipOrientation.setOrientation(datatipUid, seg);
                }

                //update indexes
                //compute ratioX and radioY
                double dx = (newPos[0] - seg.x0) / (seg.x1 - seg.x0);
                double dy = (newPos[1] - seg.y0) / (seg.y1 - seg.y0);

                controller.setProperty(datatipUid, __GO_DATATIP_INDEXES__, new Double[] {new Double(seg.pointIndex), dx, dy});
            } else {
                DatatipCommon.Segment seg3d = DatatipCommon.getSegment3dView(c2d[0], c2d[1], parentPolyline);

                Boolean useInterp = (Boolean)controller.getProperty(datatipUid, __GO_DATATIP_INTERP_MODE__);
                Double[] newPos3d;
                if (useInterp) {
                    newPos3d = DatatipCommon.Interpolate3dView(c2d[0], c2d[1], seg3d, parentPolyline);
                } else {
                    newPos3d = new Double[] {seg3d.x0, seg3d.y0, seg3d.z0};
                }

                boolean[] logFlags = getLogFlags (axesUid);

                newPos3d[0] = CommonHandler.InverseLogScale(newPos3d[0], logFlags[0]);
                newPos3d[1] = CommonHandler.InverseLogScale(newPos3d[1], logFlags[1]);
                newPos3d[2] = CommonHandler.InverseLogScale(newPos3d[2], logFlags[2]);

                //controller.setProperty(datatipUid, __GO_DATATIP_DATA__, newPos3d);

                Boolean AutoOrientation = (Boolean)controller.getProperty(datatipUid, __GO_DATATIP_AUTOORIENTATION__);
                if (AutoOrientation) {
                    DatatipOrientation.setOrientation(datatipUid, seg3d);
                }

                //update indexes
                //compute ratioX and radioY
                double dx = (newPos3d[0] - seg3d.x0) / (seg3d.x1 - seg3d.x0);
                double dy = (newPos3d[1] - seg3d.y0) / (seg3d.y1 - seg3d.y0);

                controller.setProperty(datatipUid, __GO_DATATIP_INDEXES__,
                new Double[] { new Double(seg3d.pointIndex),
                        new Double(seg3d.pointIndex), dx, dy
                });
            }
        }
    }

    private static boolean[] getLogFlags (Integer axes) {
        return new boolean[] {(Boolean)GraphicController.getController().getProperty(axes, __GO_X_AXIS_LOG_FLAG__),
                              (Boolean)GraphicController.getController().getProperty(axes, __GO_Y_AXIS_LOG_FLAG__),
                              (Boolean)GraphicController.getController().getProperty(axes, __GO_Z_AXIS_LOG_FLAG__)
                             };
    }
}
