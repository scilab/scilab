/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Manuel JULIACHS
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

package org.scilab.modules.renderer.JoGLView.label;

import org.scilab.forge.scirenderer.texture.AnchorPosition;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.modules.graphic_objects.axes.AxisProperty;

/**
 * TitlePositioner class.
 *
 * Computes the position of a Label in box coordinates [-1, +1]^3, as used
 * when rulers are drawn {@see AxesDrawer}, where -1 and +1 respectively map
 * to the corresponding axis' maximum and minimum bounds.
 * Automatic positioning uses the middle of the Axes box's projection's top segment
 * as a reference point. In order to keep the Title label fixed when the viewpoint changes,
 * its position is first computed in window coordinates (viewpoint-independent), using
 * the reference point, and is then unprojected to obtain the anchor point's position
 * in box coordinates (depends on the viewpoint).
 *
 * To do:
 *    -take into account label text angle and automatic rotation.
 *    -take into account logarithmic coordinates.
 *    -implement window-coordinates displacement computation.
 *
 * @author Manuel JULIACHS
 */
public class TitlePositioner extends LabelPositioner {
    /**
     * The offset in window coordinates from the top of the Axes box
     * to the Title label's anchor point. Relevant only to
     * automatic positioning.
     */
    public static final double TITLEOFFSET = 8.0;

    /**
     * The minimum z value in window coordinates.
     * In accordance with how window coordinates are usually defined, this should be 0,
     * however Scirenderer uses respectively -1 and +1 as the minimum and maximum z values
     * instead of 0 and 1. To be modified if deemed necessary.
     */
    private static final double ZNEAR = -1.0;

    /** The ratio between the maximum tick label sprite distance and the projected ticks direction norm. */
    private double distRatio;
    private int xlabelHeight;

    /**
     * Constructor.
     */
    public TitlePositioner() {
        super();
        /* Title labels are drawn in window coordinates to avoid jittering. */
        useWindowCoordinates = true;
    }

    /**
     * Sets the maximum sprite distance to projected ticks norm ratio.
     * @param distanceRatio the distance ratio to set.
     */
    public void setDistanceRatio(double distanceRatio) {
        this.distRatio = distanceRatio;
    }

    public void setXLabelHeight(int height) {
        this.xlabelHeight = height;
    }

    /**
     * Computes and returns the position of the label's anchor point,
     * obtained by adding the displacement vector to its position.
     * It additionally sets the displacement vector and label position members.
     * @return the position of the label's anchor point.
     */
    protected Vector3d computeDisplacedPosition() {
        Transformation canvasProjection = drawingTools.getTransformationManager().getCanvasProjection();

        Double[] axesBounds = {0.0, 0.0, 0.0, 0.0};
        Double[] margins = {0.0, 0.0, 0.0, 0.0};
        boolean onTop = false;
        if (parentAxes != null) {
            axesBounds = parentAxes.getAxesBounds();
            margins = parentAxes.getMargins();
            AxisProperty.AxisLocation xloc = parentAxes.getXAxis().getAxisLocation();
            if (xloc == AxisProperty.AxisLocation.TOP) {
                onTop = true;
            }
        }

        /* Compute the anchor point's position in window coordinates */
        double xmid = axesBounds[0] + axesBounds[2] * margins[0] + 0.5 * axesBounds[2] * (1.0 - margins[0] - margins[1]);
        double ymid = 1.0 - axesBounds[1] - axesBounds[3] * margins[2];

        Vector3d projAnchorPoint = new Vector3d(Math.floor((xmid) * (double) drawingTools.getCanvas().getWidth()),
                                                Math.floor((ymid) * (double) drawingTools.getCanvas().getHeight()),
                                                ZNEAR);

        Vector3d unprojAnchorPoint = new Vector3d(projAnchorPoint);
        unprojAnchorPoint = canvasProjection.unproject(unprojAnchorPoint);
        int h = 0;

        if (onTop) {
            h = (int) ((1 - margins[2] - margins[3]) * distRatio / 2 * axesBounds[3] * drawingTools.getCanvas().getHeight()) + xlabelHeight;
        }

        /* The anchor point is displaced along the y-axis by TITLEOFFSET pixels. */
        projAnchorPoint = projAnchorPoint.setY(projAnchorPoint.getY() + TITLEOFFSET + h);

        Vector3d unprojDispAnchorPoint = canvasProjection.unproject(projAnchorPoint);
        Vector3d position = new Vector3d(unprojDispAnchorPoint);

        /* Compute and set the LabelPositioner-inherited members */
        labelPosition = new Vector3d(unprojAnchorPoint);
        labelDisplacement = unprojDispAnchorPoint.minus(unprojAnchorPoint);

        return position;
    }

    /**
     * Returns the automatically computed sprite anchor position.
     * @return the sprite anchor position.
     */
    protected AnchorPosition getAutoAnchorPosition() {
        return AnchorPosition.DOWN;
    }
}
