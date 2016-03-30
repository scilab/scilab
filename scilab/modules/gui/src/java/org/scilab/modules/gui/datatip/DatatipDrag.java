/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Gustavo Barbosa Libotte <gustavolibotte@gmail.com>
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

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;
import org.scilab.modules.gui.editor.CommonHandler;
import org.scilab.modules.gui.editor.EntityPicker;

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
            Integer axesUid = (Integer) controller.getProperty(parentPolyline, __GO_PARENT_AXES__);
            double[] info = EntityPicker.getNearestSegmentIndex(axesUid, parentPolyline, endX, endY);
            Boolean useInterp = (Boolean) controller.getProperty(datatipUid, __GO_DATATIP_INTERP_MODE__) && CommonHandler.isLineEnabled(parentPolyline);
            Boolean AutoOrientation = (Boolean) controller.getProperty(datatipUid, __GO_DATATIP_AUTOORIENTATION__);
            if (AutoOrientation) {
                DatatipOrientation.setOrientation(datatipUid, EntityPicker.getSegment(parentPolyline, (int) info[0]));
            }

            if (useInterp) {
                controller.setProperty(datatipUid, __GO_DATATIP_INDEXES__, new Double[] {info[0], info[1]});
            } else {
                double c = info[1] <= 0.5 ? info[0] : (info[0] + 1);
                controller.setProperty(datatipUid, __GO_DATATIP_INDEXES__, new Double[] {c, 0.});
            }
        }
    }
}
