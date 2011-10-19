/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Manuel JULIACHS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView.label;

import org.scilab.forge.scirenderer.sprite.Sprite;
import org.scilab.forge.scirenderer.sprite.SpriteAnchorPosition;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.Vector3d;

/**
 * LabelPositioner class.
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
 *
 * To do:
 *    -take into account label text angle and automatic rotation.
 *    -take into account logarithmic coordinates.
 *    -implement window-coordinates displacement computation.
 *    -optimize the anchor position and lower-left corner computation methods.
 *
 * @author Manuel JULIACHS
 */
public class LabelPositioner {
        /** Specifies whether the label is automatically positioned or not. */
        private boolean autoPosition;

        /**
         * The label's position, which is its anchor point's undisplaced position
         * on the associated axis. Relevant only to automatic positioning.
         */
        private Vector3d labelPosition;

        /** The label's user-specified position. */
        private Vector3d labelUserPosition;

        /** The box coordinates ticks direction, as specified by the ruler model. */
        private Vector3d ticksDirection;

        /** The ratio between the maximum tick label sprite distance and the projected ticks direction norm. */
        private double distRatio;

        /** The normalized projected ticks direction. */
        private Vector3d projectedTicksDirection;

        /** The label displacement vector from its position to its anchor point. */
        private Vector3d labelDisplacement;

        /** The label's anchor point position. */
        private Vector3d anchorPoint;

        /** The label's sprite anchor position. */
        private SpriteAnchorPosition anchorPosition;

        /** The label's associated sprite. */
        private Sprite labelSprite;

        /** The current canvas projection. */
        private Transformation canvasProjection;

        /**
         * Constructor.
         */
        public LabelPositioner() {
                autoPosition = false;
                labelPosition = new Vector3d(0.0, 0.0, 0.0);
                labelUserPosition = new Vector3d(0.0, 0.0, 0.0);
                ticksDirection = new Vector3d(0.0, 0.0, 0.0);
                distRatio = 0.0;
                projectedTicksDirection = new Vector3d(0.0, 0.0, 0.0);
                labelDisplacement = new Vector3d(0.0, 0.0, 0.0);
                anchorPoint = new Vector3d(0.0, 0.0, 0.0);
                anchorPosition = SpriteAnchorPosition.LOWER_LEFT;
                labelSprite = null;
                canvasProjection = null;
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
        public Vector3d getLabelUserPosition() {
                return labelUserPosition;
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
         * Sets the label's associated sprite.
         * @param labelSprite the sprite to set.
         */
        public void setLabelSprite(Sprite labelSprite) {
                this.labelSprite = labelSprite;
        }

        /**
         * Sets the current canvas projection.
         * @param canvasProjection the canvas projection to set.
         */
        public void setCanvasProjection(Transformation canvasProjection) {
                this.canvasProjection = canvasProjection;
        }

        /**
         * Computes the label's anchor point and sprite anchor position
         * and sets the corresponding members to the results.
         */
        public void positionLabel() {
                computeAnchorPoint();
                computeAnchorPosition();
        }

        /**
         * Returns the position of the label's anchor point.
         * @return the anchor point's position.
         */
        public Vector3d getAnchorPoint() {
                return anchorPoint;
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
        private Vector3d computeDisplacedPosition() {
                Vector3d position = new Vector3d(labelPosition);

                /* Compute the label displacement and set it */
                labelDisplacement = ticksDirection.times(distRatio);

                position = position.plus(labelDisplacement);

                return position;
        }

        /**
         * Returns the label's sprite anchor position.
         * @return the labe's sprite anchor position.
         */
        public SpriteAnchorPosition getAnchorPosition() {
                return anchorPosition;
        }

        /**
         * Computes the label's sprite anchor position depending on the automatic position mode
         * and sets the anchorPosition member to the result.
         */
        private void computeAnchorPosition() {
                if (autoPosition) {
                        anchorPosition = getAutoAnchorPosition();
                } else {
                        anchorPosition = SpriteAnchorPosition.LOWER_LEFT;
                }
        }

        /**
         * Returns the automatically computed sprite anchor position corresponding to the projected ticks direction.
         * @return the sprite anchor position.
         */
        private SpriteAnchorPosition getAutoAnchorPosition() {
                if (projectedTicksDirection.getY() > Math.abs(projectedTicksDirection.getX())) {
                        return SpriteAnchorPosition.DOWN;
                } else if (projectedTicksDirection.getY() < -Math.abs(projectedTicksDirection.getX())) {
                        return SpriteAnchorPosition.UP;
                } else if (projectedTicksDirection.getX() > 0.0) {
                        return SpriteAnchorPosition.LEFT;
                } else {
                        return SpriteAnchorPosition.RIGHT;
                }
        }

        /**
         * Returns the automatically computed sprite anchor position corresponding to the projected ticks direction.
         * It uses all the different sprite anchor position values, compared to the 4 used by getAutoAnchorPosition.
         * However the actual result is less visually pleasing as jumps from one position to the other are
         * more frequent. To be further tested.
         * @return the sprite anchor position.
         */
        private SpriteAnchorPosition getAutoAnchorPosition2() {
                if (projectedTicksDirection.getY() > 0.0) {
                    if (projectedTicksDirection.getY() > Math.sin(3.0 * Math.PI / 8.0)) {
                            return SpriteAnchorPosition.DOWN;
                    } else if (projectedTicksDirection.getY() > Math.sin(Math.PI / 8.0)) {
                            if (projectedTicksDirection.getX() > 0.0) {
                                    return SpriteAnchorPosition.LOWER_LEFT;
                            } else {
                                    return SpriteAnchorPosition.LOWER_RIGHT;
                            }
                    } else {
                            if (projectedTicksDirection.getX() > 0.0) {
                                    return SpriteAnchorPosition.LEFT;
                            } else {
                                    return SpriteAnchorPosition.RIGHT;
                            }
                    }
                } else {
                    if (projectedTicksDirection.getY() < -Math.sin(3.0 * Math.PI / 8.0)) {
                            return SpriteAnchorPosition.UP;
                    } else if (projectedTicksDirection.getY() < -Math.sin(Math.PI / 8.0)) {
                            if (projectedTicksDirection.getX() > 0.0) {
                                    return SpriteAnchorPosition.UPPER_LEFT;
                            } else {
                                    return SpriteAnchorPosition.UPPER_RIGHT;
                            }
                    } else {
                            if (projectedTicksDirection.getX() > 0.0) {
                                    return SpriteAnchorPosition.LEFT;
                            } else {
                                    return SpriteAnchorPosition.RIGHT;
                            }
                    }
               }
        }

        /**
         * Computes and returns the position of the label's lower-left corner in box coordinates
         * from the label's position and half-width/-height vectors (specified in box coordinates).
         * The label's anchor point and anchor position must have been determined beforehand.
         * @param halfWidth the displacement vector corresponding to half the label's width.
         * @param halfHeight the displacement vector corresponding to half the label's height.
         * @return the position of the label's lower-left corner.
         */
        private Vector3d computeLowerLeftCornerPosition(Vector3d halfWidth, Vector3d halfHeight) {
                Vector3d returnedPosition = new Vector3d(anchorPoint);

                if (anchorPosition == SpriteAnchorPosition.LEFT) {
                        returnedPosition = returnedPosition.minus(halfHeight);
                } else if (anchorPosition == SpriteAnchorPosition.RIGHT) {
                        returnedPosition = returnedPosition.minus(halfWidth.times(2.0));
                        returnedPosition = returnedPosition.minus(halfHeight);
                } else if (anchorPosition == SpriteAnchorPosition.UP) {
                        returnedPosition = returnedPosition.minus(halfWidth);
                        returnedPosition = returnedPosition.minus(halfHeight.times(2.0));
                } else if (anchorPosition == SpriteAnchorPosition.DOWN) {
                        returnedPosition = returnedPosition.minus(halfWidth);
                } else if (anchorPosition == SpriteAnchorPosition.UPPER_LEFT) {
                        returnedPosition = returnedPosition.minus(halfHeight.times(2.0));
                } else if (anchorPosition == SpriteAnchorPosition.UPPER_RIGHT) {
                        returnedPosition = returnedPosition.minus(halfWidth.times(2.0));
                        returnedPosition = returnedPosition.minus(halfHeight.times(2.0));
                } else if (anchorPosition == SpriteAnchorPosition.LOWER_LEFT) {
                        // Do nothing: in this case, the positions of the anchor point and the lower-left corner are equal.
                } else if (anchorPosition == SpriteAnchorPosition.LOWER_RIGHT) {
                        returnedPosition = returnedPosition.minus(halfWidth.times(2.0));
                }

                return returnedPosition;
        }

        /**
         * Computes and returns the position of the label's lower-left corner in box coordinates.
         * It projects the label's anchor point to compute the points located from it at
         * respectively half the sprite width and half its height, which are then used
         * to obtain the label's half-width and half-height vectors in box coordinates.
         * The label's anchor point and anchor position, associated sprite, and canvas projection
         * must have been initialized beforehand.
         * @return the label's lower-left corner position.
         */
        public Vector3d getLowerLeftCornerPosition() {
                Vector3d labelPoint = new Vector3d(anchorPoint);

                Vector3d projLabelPoint = canvasProjection.project(labelPoint);

                Vector3d projRightPoint = new Vector3d(projLabelPoint.getX() + 0.5 * (double) labelSprite.getWidth(), projLabelPoint.getY(), projLabelPoint.getZ());
                Vector3d unprojRightPoint = canvasProjection.unproject(projRightPoint);

                Vector3d halfWidth = unprojRightPoint.minus(labelPoint);

                Vector3d projUpperPoint = new Vector3d(projLabelPoint.getX(), projLabelPoint.getY() + 0.5 * (double) labelSprite.getHeight(), projLabelPoint.getZ());
                Vector3d unprojUpperPoint = canvasProjection.unproject(projUpperPoint);

                Vector3d halfHeight = unprojUpperPoint.minus(labelPoint);

                Vector3d cornerPosition = computeLowerLeftCornerPosition(halfWidth, halfHeight);

                return cornerPosition;
        }

}

