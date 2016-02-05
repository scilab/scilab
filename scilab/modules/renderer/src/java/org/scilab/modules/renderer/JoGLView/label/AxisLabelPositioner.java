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
import org.scilab.forge.scirenderer.tranformations.Vector3d;

import java.util.ArrayList;
import java.util.Arrays;

/**
 * AxisLabelPositioner class.
 *
 * Computes the position of a Label in box coordinates [-1, +1]^3, as used
 * when rulers are drawn {@see AxesDrawer}, where -1 and +1 respectively map
 * to the corresponding axis' maximum and minimum bounds.
 * All positions and vectors are specified in box coordinates, except
 * the projected ticks direction.
 * Position can be automatically computed or user-specified. In the former case,
 * four parameters resulting from the axis ruler drawing algorithms are needed:
 * the distance ratio and projected ticks direction, which are computed by the Ruler
 * drawer (used by AxesRulerDrawer), and the label position and box coordinates
 * ticks direction, obtained from the AxesRuler drawer {@see AxesRulerDrawer}.
 * These parameters must be set before computing the label's position values.
 * The rotation angle can also be determined automatically or user-specified
 * and requires the same parameters (as well as for sprite anchor position correction).
 *
 * To do:
 *    -take into account logarithmic coordinates.
 *    -implement window-coordinates displacement computation.
 *    -optimize the anchor position computation methods.
 *
 * @author Manuel JULIACHS
 */
public class AxisLabelPositioner extends LabelPositioner {
    /** The box coordinates ticks direction, as specified by the ruler model. */
    private Vector3d ticksDirection;

    /** The ratio between the maximum tick label sprite distance and the projected ticks direction norm. */
    private double distRatio;

    /** The normalized projected ticks direction. */
    private Vector3d projectedTicksDirection;

    /**
     * The array of allowed anchor positions.
     * It is used to determine the corrected anchor position as a function of the rotation angle.
     * The uncorrected position (angle = 0) is obtained from the auto-positioning algorithm.
     * Adding k*90 (respectively -k*90) degrees to its angle amounts to shifing by k elements to the right
     * (respectively to the left) from its location in the array, modulo the array size.
     */
    private static final AnchorPosition[] allowedAnchorPositionsArray = {AnchorPosition.LEFT, AnchorPosition.DOWN,
                                                                         AnchorPosition.RIGHT, AnchorPosition.UP
                                                                        };

    /** The list of allowed anchor positions. */
    private static final ArrayList<AnchorPosition> allowedAnchorPositions = new ArrayList<AnchorPosition>(Arrays.asList(allowedAnchorPositionsArray));

    /**
     * Constructor.
     */
    public AxisLabelPositioner() {
        super();
        ticksDirection = new Vector3d(0.0, 0.0, 0.0);
        distRatio = 0.0;
        projectedTicksDirection = new Vector3d(0.0, 0.0, 0.0);
    }

    /**
     * Sets the ticks direction in box coordinates.
     * @param ticksDirection the ticks direction to set.
     */
    public void setTicksDirection(Vector3d ticksDirection) {
        this.ticksDirection = new Vector3d(ticksDirection);
    }

    /**
     * Returns the box coordinates ticks direction.
     * @return the box coordinates ticks direction.
     */
    public Vector3d getTicksDirection() {
        return ticksDirection;
    }

    /**
     * Sets the maximum sprite distance to projected ticks norm ratio.
     * @param distanceRatio the distance ratio to set.
     */
    public void setDistanceRatio(double distanceRatio) {
        this.distRatio = distanceRatio;
    }

    /**
     * Returns the maximum sprite distance to projected ticks norm ratio.
     * @return the distance ratio.
     */
    public double getDistanceRatio() {
        return distRatio;
    }

    /**
     * Sets the normalized projected ticks direction.
     * @param projectedTicksDirection the projected ticks direction to set.
     */
    public void setProjectedTicksDirection(Vector3d projectedTicksDirection) {
        this.projectedTicksDirection = new Vector3d(projectedTicksDirection);
    }

    /**
     * Returns the normalized projected ticks direction.
     * @return the projected ticks direction.
     */
    public Vector3d getProjectedTicksDirection() {
        return projectedTicksDirection;
    }

    /**
     * Determines and returns the rotation angle (in degrees) from the window coordinate system's axis
     * (+Y, -Y, +X, -X) most closely aligned with the projected ticks direction to the projected ticks direction itself.
     * (the most closely aligned axis is the one such that the angle between the two is minimal). Positive angles
     * are measured clockwise.
     * @return the angle from the most closely aligned axis to the projected ticks direction.
     */
    private double computeTicksDirectionAngle() {
        double sign;

        Vector3d axis;

        double signX = Math.signum(projectedTicksDirection.getX());
        double signY = Math.signum(projectedTicksDirection.getY());

        if (projectedTicksDirection.getY() > Math.abs(projectedTicksDirection.getX())) {
            /* Nearest: +Y */
            axis = new Vector3d(0.0, 1.0, 0.0);
            sign = signX;

        } else if (projectedTicksDirection.getY() < -Math.abs(projectedTicksDirection.getX())) {
            /* Nearest: -Y */
            axis = new Vector3d(0.0, -1.0, 0.0);
            sign = -signX;

        } else if (projectedTicksDirection.getX() > 0.0) {
            /* Nearest: +X */
            axis = new Vector3d(1.0, 0.0, 0.0);
            sign = -signY;

        } else {
            /* Nearest: -X */
            axis = new Vector3d(-1.0, 0.0, 0.0);
            sign = signY;
        }

        double dp = axis.scalar(projectedTicksDirection);
        double angle = Math.acos(dp) * 180.0 / Math.PI;

        angle *= sign;

        return angle;
    }

    /**
     * Computes and returns the position of the label's anchor point,
     * obtained by adding the displacement vector to its position.
     * It additionally sets the displacement vector member.
     * @return the position of the label's anchor point.
     */
    protected Vector3d computeDisplacedPosition() {
        Vector3d position = new Vector3d(labelPosition);

        /* Compute the label displacement and set it */
        labelDisplacement = ticksDirection.times(distRatio);
        position = position.plus(labelDisplacement);

        return position;
    }

    /**
     * Returns the automatically computed rotation angle.
     * Set to 0 as a default.
     * @return the rotation angle.
     */
    protected double getAutoRotationAngle() {
        return 0.0;
    }

    /**
     * Returns the automatically computed sprite anchor position.
     * The anchor position is first determined from the projected ticks direction and
     * then corrected depending on the label's rotation angle.
     * To do so, a signed offset is computed (number of quadrants swept by the angle)
     * and added to the uncorrected position (read from the array of allowed values)
     * to obtain the corrected one.
     * The orientation of the label's axis is taken into account by subtracting the angle between
     * the closest window coordinate axis and the ticks direction from the rotation angle.
     * The resulting angle therefore amounts to the angle from the ticks direction to
     * the vector going from the anchor point to (and orthogonal to) the opposite side of the label box.
     * The anchor position remains constant when the corrected angle lies in the initial quadrant
     * centered about the ticks direction (from -45 to 45 degrees relative to it),
     *
     * The result is such that the anchor-to-opposite side vector always points away from
     * the interior of the Axes box. This avoids overlapping between the label's box and its axis
     * when the rotation angle is such that the label box's width vector is parallel or orthogonal
     * to the axis.
     *
     * @return the sprite anchor position.
     */
    protected AnchorPosition getAutoAnchorPosition() {
        AnchorPosition anchorPosition = getUncorrectedAutoAnchorPosition();

        double tmpRotationAngle;

        /* Get the angle from the closest window coordinate axis to the ticks direction. */
        double ticksDirectionAngle = computeTicksDirectionAngle();

        tmpRotationAngle = rotationAngle % 360.0;

        /*
         * Subtract the closest axis to ticks direction angle from the rotation angle
         * to take into account the associated axis' orientation.
         */
        tmpRotationAngle -= ticksDirectionAngle;

        int anchorIndex = 0;

        anchorIndex = allowedAnchorPositions.indexOf(anchorPosition);

        /*
         * The offset is the number of quadrants swept by the rotation angle.
         * The inital quadrant is -45/+45 degrees about the reference vector
         * (ticks direction) and amounts to 0.
         */
        int offset = (int) ((Math.abs(tmpRotationAngle) + 45.0) / 90.0);

        if (tmpRotationAngle < 0.0) {
            offset = -offset;
        }

        /* Cycle through the allowed anchor positions. */
        anchorIndex = (anchorIndex + offset) % 4;

        if (anchorIndex < 0) {
            anchorIndex = 4 + anchorIndex;
        }

        anchorPosition = allowedAnchorPositions.get(anchorIndex);

        return anchorPosition;
    }

    /**
     * Returns the automatically computed sprite anchor position corresponding to the projected ticks direction.
     * It is determined independently of the rotation angle.
     * @return the sprite anchor position.
     */
    private AnchorPosition getUncorrectedAutoAnchorPosition() {
        if (projectedTicksDirection.getY() > Math.abs(projectedTicksDirection.getX())) {
            return AnchorPosition.DOWN;
        } else if (projectedTicksDirection.getY() < -Math.abs(projectedTicksDirection.getX())) {
            return AnchorPosition.UP;
        } else if (projectedTicksDirection.getX() > 0.0) {
            return AnchorPosition.LEFT;
        } else {
            return AnchorPosition.RIGHT;
        }
    }

    /**
     * Returns the automatically computed sprite anchor position corresponding to the projected ticks direction.
     * It uses all the different sprite anchor position values, compared to the 4 used by getAutoAnchorPosition.
     * However the actual result is less visually pleasing as jumps from one position to the other are
     * more frequent. To be further tested.
     * @return the sprite anchor position.
     */
    private AnchorPosition getAutoAnchorPosition2() {
        if (projectedTicksDirection.getY() > 0.0) {
            if (projectedTicksDirection.getY() > Math.sin(3.0 * Math.PI / 8.0)) {
                return AnchorPosition.DOWN;
            } else if (projectedTicksDirection.getY() > Math.sin(Math.PI / 8.0)) {
                if (projectedTicksDirection.getX() > 0.0) {
                    return AnchorPosition.LOWER_LEFT;
                } else {
                    return AnchorPosition.LOWER_RIGHT;
                }
            } else {
                if (projectedTicksDirection.getX() > 0.0) {
                    return AnchorPosition.LEFT;
                } else {
                    return AnchorPosition.RIGHT;
                }
            }
        } else {
            if (projectedTicksDirection.getY() < -Math.sin(3.0 * Math.PI / 8.0)) {
                return AnchorPosition.UP;
            } else if (projectedTicksDirection.getY() < -Math.sin(Math.PI / 8.0)) {
                if (projectedTicksDirection.getX() > 0.0) {
                    return AnchorPosition.UPPER_LEFT;
                } else {
                    return AnchorPosition.UPPER_RIGHT;
                }
            } else {
                if (projectedTicksDirection.getX() > 0.0) {
                    return AnchorPosition.LEFT;
                } else {
                    return AnchorPosition.RIGHT;
                }
            }
        }
    }

}

