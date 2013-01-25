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

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;

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
    public static void dragDatatip(String datatipUid, Integer endX, Integer endY) {

        String parentPolyline = DatatipCommon.getParentPolyline(datatipUid);

        if (parentPolyline != null) {
            Integer pos[] = {endX, endY};
            String figure = (String)GraphicController.getController().getProperty(datatipUid, __GO_PARENT_FIGURE__);
            double[] c2d = DatatipCommon.getTransformedPosition(figure, pos);

            DatatipCommon.Segment seg = DatatipCommon.getSegment(c2d[0], parentPolyline);

            Boolean useInterp = (Boolean)GraphicController.getController().getProperty(datatipUid, __GO_DATATIP_INTERP_MODE__);
            Double[] newPos;
            if (useInterp) {
                newPos = DatatipCommon.Interpolate(c2d[0], seg);
            } else {
                newPos = new Double[] {seg.x0, seg.y0, 0.0};
            }
            GraphicController.getController().setProperty(datatipUid, __GO_DATATIP_DATA__, newPos);

            Boolean AutoOrientation = (Boolean)GraphicController.getController().getProperty(datatipUid, __GO_DATATIP_AUTOORIENTATION__);
            if (AutoOrientation) {
                DatatipOrientation.setOrientation(datatipUid, seg);
            }
        }
    }




}
