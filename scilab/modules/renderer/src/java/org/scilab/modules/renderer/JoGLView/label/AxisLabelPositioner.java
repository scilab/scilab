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

import org.scilab.forge.scirenderer.sprite.SpriteAnchorPosition;
import org.scilab.forge.scirenderer.tranformations.Vector3d;

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
 *
 * To do:
 *    -take into account label text angle and automatic rotation.
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
         * Returns the automatically computed sprite anchor position corresponding to the projected ticks direction.
         * @return the sprite anchor position.
         */
        protected SpriteAnchorPosition getAutoAnchorPosition() {
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

}

