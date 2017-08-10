/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Gustavo Barbosa Libotte
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

import org.scilab.modules.gui.datatip.DatatipCommon;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;
import org.scilab.modules.gui.editor.AxesHandler;
import org.scilab.modules.gui.editor.EntityPicker;
import org.scilab.modules.renderer.CallRenderer;

import org.scilab.modules.action_binding.InterpreterManagement;

/**
 * Orientate the datatip for "upper left", "upper right", "lower left" or "lower right" according to the quadrant position.
 * @author Gustavo Barbosa Libotte
 */
public class DatatipOrientation {

    /**
     * Setup the datatip orientation according with the curve segment inclination
     *
     * @param datatip the datatip uid
     */
    public static void setOrientation(Integer datatip) {
        Integer polyline = DatatipCommon.getParentPolyline(datatip);
        if (polyline != null) {
            Integer[] indexes = (Integer[])GraphicController.getController().getProperty(datatip, __GO_DATATIP_INDEXES__);
            DatatipCommon.Segment seg = EntityPicker.getSegment(polyline, indexes[0]);
            if (seg != null) {
                setOrientation(datatip, seg);
            }
        }
    }

    /**
     * Setup the datatip orientation according with the mouse movement.
     *
     * @param datatip the datatip uid
     */
    public static void setOrientation(Integer datatip, Integer lastClick[], Integer[] curClick) {
        int dx = curClick[0] - lastClick[0];
        int dy = curClick[1] - lastClick[1];
        Integer finalOrientation = 0;

        if (dx > 0) {
            if (dy < 0) {
                finalOrientation = 1;
            } else {
                finalOrientation = 3;
            }
        } else {
            if (dy < 0) {
                finalOrientation = 0;
            } else {
                finalOrientation = 2;
            }
        }
        GraphicController.getController().setProperty(datatip, __GO_DATATIP_ORIENTATION__, finalOrientation);
    }

    /**
     * Setup the datatip orientation according with the curve segment inclination
     *
     * @param datatip the datatip uid
     * @param seg the line segment
     */
    public static void setOrientation(Integer datatip, DatatipCommon.Segment seg) {
        Double[] bounds;
        Integer finalOrientation;

        double dx = seg.x1 - seg.x0;
        double dy = seg.y1 - seg.y0;

        /* dx < 0 ? flip dy*/
        if (dx < 0) {
            dy = -dy;
        }

        Integer axesUid = (Integer)GraphicController.getController().getProperty(datatip, __GO_PARENT_AXES__);
        if (AxesHandler.isZoomBoxEnabled(axesUid)) {
            bounds = (Double[])GraphicController.getController().getProperty(axesUid, __GO_ZOOM_BOX__);
        } else {
            bounds = (Double[])GraphicController.getController().getProperty(axesUid, __GO_DATA_BOUNDS__);
        }

        /* dy > 0 ? crescent (use top left) : decrescent (use top right) */
        finalOrientation = dy > 0 ? 0 : 1;
        Double[] tip_pos = (Double[])GraphicController.getController().getProperty(datatip, __GO_DATATIP_DATA__);
        /* tip_pos.y  is below the middle of the axes? use bottom instead of top orientation*/
        Double middle = bounds[2] + ((bounds[3] - bounds[2]) / 2.0);
        if (tip_pos[1] < middle) {
            finalOrientation = flipOrientation(finalOrientation);
        }

        GraphicController.getController().setProperty(datatip, __GO_DATATIP_ORIENTATION__, finalOrientation);

        /* check if the tip is visible*/
        CallRenderer.updateTextBounds(datatip);
        Double[] corners = (Double[])GraphicController.getController().getProperty(datatip, __GO_CORNERS__);

        /*if the tip is out of the  axes bounds try flip it*/
        if (!isInBounds(corners[0], corners[1], bounds) || !isInBounds(corners[6], corners[7], bounds)) {
            GraphicController.getController().setProperty(datatip, __GO_DATATIP_ORIENTATION__, flipOrientation(finalOrientation));
        }
    }

    /**
     * Check if the given position (x, y) is in bounds
     *
     * @param x position in X axis
     * @param y position in Y axis
     * @param bounds vector with the bounds
     */
    private static boolean isInBounds(Double x, Double y, Double[] bounds) {
        if (x >= bounds[0] && x <= bounds[1]) {
            if (y >= bounds[2] && y <= bounds[3]) {
                return true;
            }
        }
        return false;
    }

    /**
     * Flip the given orientation
     * @param orientation the iven orientation
     * @return the flipped orientation
     */
    private static Integer flipOrientation(Integer orientation) {
        switch (orientation) {
                /* top left*/
            case 0:
                return 3;
                /* top right*/
            case 1:
                return 2;
                /* bottom left*/
            case 2:
                return 1;
                /* bottom right*/
            case 3:
                return 0;
                /* do nothing*/
            default:
                return orientation;
        }
    }

    /**
     * Setup the datatip orientation by program.
     *
     * @param datatipUid the datatip uid.
     * @param datatipOrientation String with datatip orientation.
     * @param datatipOrientationNum Integer with datatip orientation to set property.
     */
    public static void datatipSetOrientation(int datatipUid, String datatipOrientation, int datatipOrientationNum) {
        switch (datatipOrientationNum) {
            case 0: // upper left
                GraphicController.getController().setProperty(datatipUid, __GO_DATATIP_AUTOORIENTATION__, false);
                GraphicController.getController().setProperty(datatipUid, __GO_DATATIP_ORIENTATION__, 0);
                break;
            case 1: // upper right
                GraphicController.getController().setProperty(datatipUid, __GO_DATATIP_AUTOORIENTATION__, false);
                GraphicController.getController().setProperty(datatipUid, __GO_DATATIP_ORIENTATION__, 1);
                break;
            case 2: // lower left
                GraphicController.getController().setProperty(datatipUid, __GO_DATATIP_AUTOORIENTATION__, false);
                GraphicController.getController().setProperty(datatipUid, __GO_DATATIP_ORIENTATION__, 2);
                break;
            case 3: // lower right
                GraphicController.getController().setProperty(datatipUid, __GO_DATATIP_AUTOORIENTATION__, false);
                GraphicController.getController().setProperty(datatipUid, __GO_DATATIP_ORIENTATION__, 3);
                break;
            case 4: // left
                GraphicController.getController().setProperty(datatipUid, __GO_DATATIP_AUTOORIENTATION__, false);
                GraphicController.getController().setProperty(datatipUid, __GO_DATATIP_ORIENTATION__, 4);
                break;
            case 5: // right
                GraphicController.getController().setProperty(datatipUid, __GO_DATATIP_AUTOORIENTATION__, false);
                GraphicController.getController().setProperty(datatipUid, __GO_DATATIP_ORIENTATION__, 5);
                break;
            case 6: // upper
                GraphicController.getController().setProperty(datatipUid, __GO_DATATIP_AUTOORIENTATION__, false);
                GraphicController.getController().setProperty(datatipUid, __GO_DATATIP_ORIENTATION__, 6);
                break;
            case 7: // lower
                GraphicController.getController().setProperty(datatipUid, __GO_DATATIP_AUTOORIENTATION__, false);
                GraphicController.getController().setProperty(datatipUid, __GO_DATATIP_ORIENTATION__, 7);
                break;
            case 8: // automatic
                GraphicController.getController().setProperty(datatipUid, __GO_DATATIP_AUTOORIENTATION__, true);
                break;
            default:
                String errorMsg = "error(msprintf(_( \"%s: Unknown input argument #%d: ''%s'' is not valid.\n\"),\"datatipSetOrientation\",2,\"" + datatipOrientation + "\"));";
                InterpreterManagement.requestScilabExec(errorMsg);
        }
    }
}
