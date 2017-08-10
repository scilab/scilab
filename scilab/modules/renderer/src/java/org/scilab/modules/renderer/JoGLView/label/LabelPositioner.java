/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011-2012 - DIGITEO - Manuel JULIACHS
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

import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.texture.AnchorPosition;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.tranformations.DegenerateMatrixException;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.TransformationFactory;
import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.modules.graphic_objects.axes.Axes;

import java.awt.Dimension;

/**
 * LabelPositioner class
 *
 * Computes the position of a Label in box coordinates [-1, +1]^3, as used
 * when rulers are drawn {@see AxesDrawer}, where -1 and +1 respectively map
 * to the corresponding axis' maximum and minimum bounds.
 * As it is an abstract class, there are two derived classes implementing
 * positioning for respectively axis-associated labels and title labels
 * (respectively {@see AxisLabelPositioner} and {@see TitlePositioner}).
 * All positions and vectors are specified in box coordinates, unless stated otherwise.
 * The position and rotation angle can be both automatically computed or user-specified.
 *
 * To do:
 *    -take into account logarithmic coordinates.
 *    -implement window-coordinates displacement computation.
 *    -optimize the lower-left corner computation methods.
 *
 * @author Manuel JULIACHS
 */
public abstract class LabelPositioner {
    /** Specifies whether the label is automatically positioned or not. */
    protected boolean autoPosition;

    /** Specifies whether the label is automatically rotated or not. */
    protected boolean autoRotation;

    /**
     * The label's position, which is its anchor point's undisplaced position.
     * For axis-associated labels, it is located on the associated axis,
     * whereas for title labels, it is the point whose projection
     * is on the middle of the Axes box's projection's top segment (in 2D view mode).
     * Relevant only to automatic positioning.
     */
    protected Vector3d labelPosition;

    /** The label's user-specified position. */
    protected Vector3d labelUserPosition;

    /**
     * The label's rotation angle (degrees).
     * This is the angle from the window coordinate system's +X axis to
     * the label's half-width vector.  Positive values are measured clockwise
     * (opposite to the standard convention).
     */
    protected double rotationAngle;

    /**
     * The label's user-specified rotation angle (degrees). Positive values are
     * measured clockwise (see rotationAngle).
     */
    protected double userRotationAngle;

    /** The label displacement vector from its position to its anchor point. */
    protected Vector3d labelDisplacement;

    /** The label's anchor point position. */
    protected Vector3d anchorPoint;

    /** The label's sprite anchor position. */
    protected AnchorPosition anchorPosition;

    /** The label's associated sprite. */
    protected Texture labelTexture;

    /** The current drawing tools. */
    protected DrawingTools drawingTools;

    /** The label's current parent Axes. */
    protected Axes parentAxes;

    /**
     * A flag specifying whether drawing is done using box coordinates (false)
     * or window coordinates (true).
     */
    protected boolean useWindowCoordinates;

    /** The label's anchor point position in window coordinates. */
    protected Vector3d projAnchorPoint;

    /** The label's half-width vector in window coordinates. */
    protected Vector3d projHalfWidth;

    /** The label's half-height vector in window coordinates. */
    protected Vector3d projHalfHeight;

    /**
     * The positions of the corners of the label's bounding box in window coordinates,
     * stored in the following order: lower-left, lower-right, upper-left, upper-right.
     */
    protected Vector3d[] projCornerCoordinates;

    /**
     * Constructor.
     */
    public LabelPositioner() {
        autoPosition = false;
        autoRotation = false;
        labelPosition = new Vector3d(0.0, 0.0, 0.0);
        labelUserPosition = new Vector3d(0.0, 0.0, 0.0);
        rotationAngle = 0.0;
        userRotationAngle = 0.0;
        labelDisplacement = new Vector3d(0.0, 0.0, 0.0);
        anchorPoint = new Vector3d(0.0, 0.0, 0.0);
        anchorPosition = AnchorPosition.LOWER_LEFT;
        labelTexture = null;
        drawingTools = null;
        parentAxes = null;
        /* Labels are drawn in box coordinates as a default. */
        useWindowCoordinates = false;
        projAnchorPoint = new Vector3d(0.0, 0.0, 0.0);
        projHalfWidth = new Vector3d(0.0, 0.0, 0.0);
        projHalfHeight = new Vector3d(0.0, 0.0, 0.0);
        projCornerCoordinates = new Vector3d[4];
    }

    /**
     * Sets the label's auto position flag.
     * @param autoPosition the autoPosition to set.
     */
    public void setAutoPosition(boolean autoPosition) {
        this.autoPosition = autoPosition;
    }

    /**
     * Returns the label's auto position flag.
     * @return autoPosition the autoPosition to set.
     */
    public boolean getAutoPosition() {
        return autoPosition;
    }

    /**
     * Sets the label's auto rotation flag.
     * @param autoRotation the autoRotation to set.
     */
    public void setAutoRotation(boolean autoRotation) {
        this.autoRotation = autoRotation;
    }

    /**
     * Returns the label's auto rotation flag.
     * @return autoRotation the autoRotation to set.
     */
    public boolean getAutoRotation() {
        return autoRotation;
    }

    /**
     * Sets the label's position.
     * @param labelPosition the position to set.
     */
    public void setLabelPosition(Vector3d labelPosition) {
        this.labelPosition = new Vector3d(labelPosition);
    }

    /**
     * Returns the label's position.
     * @return the label position.
     */
    public Vector3d getLabelPosition() {
        return labelPosition;
    }

    /**
     * Sets the label's user position.
     * @param labelUserPosition the user position to set.
     */
    public void setLabelUserPosition(Vector3d labelUserPosition) {
        this.labelUserPosition = new Vector3d(labelUserPosition);
    }

    /**
     * Returns the label's user position.
     * @return the label user position.
     */
    public Vector3d
    getLabelUserPosition() {
        return labelUserPosition;
    }

    /**
     * Sets the label's rotation angle.
     * @param rotationAngle the rotation angle to set.
     */
    public void setRotationAngle(double rotationAngle) {
        this.rotationAngle = rotationAngle;
    }

    /**
     * Returns the label's rotation angle.
     * @return the rotation angle.
     */
    public double getRotationAngle() {
        return rotationAngle;
    }

    /**
     * Sets the label's user rotation angle.
     * @param userRotationAngle the user rotation angle to set.
     */
    public void setUserRotationAngle(double userRotationAngle) {
        this.userRotationAngle = userRotationAngle;
    }

    /**
     * Returns the label's user rotation angle.
     * @return the user rotation angle.
     */
    public double getUserRotationAngle() {
        return userRotationAngle;
    }

    /**
     * Sets the label displacement.
     * @param labelDisplacement the label displacement to set.
     */
    public void setLabelDisplacement(Vector3d labelDisplacement) {
        this.labelDisplacement = new Vector3d(labelDisplacement);
    }

    /**
     * Returns the label displacement.
     * @return the label displacement.
     */
    public Vector3d getLabelDisplacement() {
        return labelDisplacement;
    }

    /**
     * Returns the position of the label's anchor point.
     * @return the anchor point's position.
     */
    public Vector3d getAnchorPoint() {
        return anchorPoint;
    }

    /**
     * Returns the label's sprite anchor position.
     * @return the labe's sprite anchor position.
     */
    public AnchorPosition getAnchorPosition() {
        return anchorPosition;
    }

    /**
     * Sets the label's associated texture.
     * @param labelTexture the texture to set.
     */
    public void setLabelTexture(Texture labelTexture) {
        this.labelTexture = labelTexture;
    }

    /**
     * Sets the current drawing tools.
     * @param drawingTools the drawing tools to set.
     */
    public void setDrawingTools(DrawingTools drawingTools) {
        this.drawingTools = drawingTools;
    }

    /**
     * Sets the current parent Axes.
     * @param parentAxes the parent Axes to set.
     */
    public void setParentAxes(Axes parentAxes) {
        this.parentAxes = parentAxes;
    }

    /**
     * Returns the flag specifying whether drawing occurs in window coordinates or not.
     * @return the window coordinates flag.
     */
    public boolean getUseWindowCoordinates() {
        return useWindowCoordinates;
    }

    /**
     * Computes the label's anchor point and sprite anchor position
     * and sets the corresponding members to the results.
     */
    public void positionLabel() {
        computeAnchorPoint();
        computeRotationAngle();

        /* Depends on the rotation angle and must therefore be computed afterwards. */
        computeAnchorPosition();

        /* Computes the label's bounding box's corners (in window coordinates) */
        computeCorners();
    }

    /**
     * Computes the position of the label's anchor point depending on the
     * automatic position mode and sets the anchorPoint member to the result.
     * If positioning is not automatic, the position is set to the user-specified one.
     */
    private void computeAnchorPoint() {
        if (autoPosition) {
            anchorPoint = computeDisplacedPosition();
        } else {
            anchorPoint = labelUserPosition;
        }
    }

    /**
     * Computes and returns the position of the label's anchor point,
     * obtained by adding the displacement vector to its position.
     * It additionally sets the displacement vector member.
     * @return the position of the label's anchor point.
     */
    protected Vector3d computeDisplacedPosition() {
        Vector3d position = new Vector3d(labelPosition);

        /* Compute and set a zero displacement */
        labelDisplacement = new Vector3d(0.0, 0.0, 0.0);

        return position;
    }

    /**
     * Computes and sets the label's rotation angle depending on
     * the automatic rotation mode. If it is not automatic, the angle
     * is set to the user-specified one.
     */
    private void computeRotationAngle() {
        if (autoRotation) {
            setRotationAngle(getAutoRotationAngle());
        } else {
            setRotationAngle(userRotationAngle);
        }
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
     * @return the sprite anchor position.
     */
    protected AnchorPosition getAutoAnchorPosition() {
        return AnchorPosition.LEFT;
    }

    /**
     * Computes the label's sprite anchor position depending on the automatic position mode
     * and sets the anchorPosition member to the result.
     */
    private void computeAnchorPosition() {
        if (autoPosition) {
            anchorPosition = getAutoAnchorPosition();
        } else {
            anchorPosition = AnchorPosition.LOWER_LEFT;
        }
    }

    /**
     * Computes and returns the position of the label's lower-left corner either in box or window coordinates
     * from the label's position and half-width/-height vectors (also specified either in box or window coordinates).
     * The label's anchor point and anchor position must have been determined beforehand.
     * @param anchorPoint the position of the label's anchor point.
     * @param halfWidth the displacement vector corresponding to half the label's width.
     * @param halfHeight the displacement vector corresponding to half the label's height.
     * @return the position of the label's lower-left corner.
     */
    private Vector3d computeLowerLeftCornerPosition(Vector3d anchorPoint, Vector3d halfWidth, Vector3d halfHeight) {
        Vector3d returnedPosition = new Vector3d(anchorPoint);

        if (anchorPosition == AnchorPosition.LEFT) {
            returnedPosition = returnedPosition.minus(halfHeight);
        } else if (anchorPosition == AnchorPosition.RIGHT) {
            returnedPosition = returnedPosition.minus(halfWidth.times(2.0));
            returnedPosition = returnedPosition.minus(halfHeight);
        } else if (anchorPosition == AnchorPosition.UP) {
            returnedPosition = returnedPosition.minus(halfWidth);
            returnedPosition = returnedPosition.minus(halfHeight.times(2.0));
        } else if (anchorPosition == AnchorPosition.DOWN) {
            returnedPosition = returnedPosition.minus(halfWidth);
        } else if (anchorPosition == AnchorPosition.UPPER_LEFT) {
            returnedPosition = returnedPosition.minus(halfHeight.times(2.0));
        } else if (anchorPosition == AnchorPosition.UPPER_RIGHT) {
            returnedPosition = returnedPosition.minus(halfWidth.times(2.0));
            returnedPosition = returnedPosition.minus(halfHeight.times(2.0));
        } else if (anchorPosition == AnchorPosition.LOWER_LEFT) {
            // Do nothing: in this case, the positions of the anchor point and the lower-left corner are equal.
        } else if (anchorPosition == AnchorPosition.LOWER_RIGHT) {
            returnedPosition = returnedPosition.minus(halfWidth.times(2.0));
        }

        return returnedPosition;
    }

    /**
     * Computes and returns the position of the label's lower-left corner in box coordinates.
     * It projects the label's anchor point to compute the points located from it at
     * respectively half the sprite width and half its height, using the previously computed
     * projected anchor point, half-width and half-height vectors, finally computing the half-vectors
     * in box coordinates to obtain the lower-left corner's position.
     * The label's anchor point and anchor position, rotation angle, associated sprite, and drawing tools
     * must have been initialized beforehand.
     * @return the label's lower-left corner position.
     */
    public Vector3d getLowerLeftCornerPosition() {
        Transformation canvasProjection = drawingTools.getTransformationManager().getCanvasProjection();
        Vector3d labelPoint = new Vector3d(anchorPoint);
        Vector3d projLabelPoint = new Vector3d(projAnchorPoint);

        Vector3d projRightPoint = projLabelPoint.plus(projHalfWidth);
        Vector3d unprojRightPoint = canvasProjection.unproject(projRightPoint);

        Vector3d halfWidth = unprojRightPoint.minus(labelPoint);

        Vector3d projUpperPoint = projLabelPoint.plus(projHalfHeight);
        Vector3d unprojUpperPoint = canvasProjection.unproject(projUpperPoint);

        Vector3d halfHeight = unprojUpperPoint.minus(labelPoint);

        Vector3d cornerPosition = computeLowerLeftCornerPosition(labelPoint, halfWidth, halfHeight);

        return cornerPosition;
    }

    /**
     * Computes the corners of the label's bounding box, in window coordinates.
     * The label's point, rotation angle and anchor position must have been determined beforehand.
     * It also computes the anchor point, half-width and half-height vectors in window coordinates.
     */
    private void computeCorners() {
        Transformation canvasProjection = drawingTools.getTransformationManager().getCanvasProjection();
        Vector3d labelPoint = new Vector3d(anchorPoint);

        /*
         * Ought to be -rotationAngle as positive angle values are measured clockwise for labels.
         * Apparently uses the same convention as the labels (clockwise positive directions).
         * To be verified.
         */
        Transformation winRotation;
        try {
            winRotation = TransformationFactory.getRotationTransformation(rotationAngle, 0.0, 0.0, 1.0);
        } catch (DegenerateMatrixException e) {
            // Should never happen, as long as the rotation axes is (0, 0, 1) and not zero.
            winRotation = TransformationFactory.getIdentity();
        }

        Dimension textureSize = labelTexture.getDataProvider().getTextureSize();
        projHalfWidth = new Vector3d(0.5 * textureSize.getWidth(), 0.0, 0.0);
        projHalfHeight = new Vector3d(0.0, 0.5 * textureSize.getHeight(), 0.0);

        projHalfWidth = winRotation.projectDirection(projHalfWidth);
        projHalfHeight = winRotation.projectDirection(projHalfHeight);

        projAnchorPoint = canvasProjection.project(labelPoint);

        /* Compute the corners of the label's bounding box in window coordinates */
        Vector3d projCornerPosition = computeLowerLeftCornerPosition(projAnchorPoint, projHalfWidth, projHalfHeight);

        projCornerCoordinates[0] = new Vector3d(projCornerPosition);
        projCornerCoordinates[1] = projCornerPosition.plus(projHalfWidth.times(2.0));
        projCornerCoordinates[2] = projCornerPosition.plus(projHalfHeight.times(2.0));
        projCornerCoordinates[3] = projCornerCoordinates[2].plus(projHalfWidth.times(2.0));
    }

    /**
     * Returns the positions of the label's corners (in window coordinates).
     * The returned corners are in the following order: lower-left, lower-right,
     * upper-left and upper-right.
     * @return the corners' window coordinates (4-element array).
     */
    public Vector3d[] getProjCorners() {
        return projCornerCoordinates;
    }
}

