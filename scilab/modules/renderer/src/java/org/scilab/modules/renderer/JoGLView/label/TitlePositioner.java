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

import org.scilab.forge.scirenderer.texture.AnchorPosition;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.Vector3d;

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
        private static final double TITLEOFFSET = 8.0;

        /**
         * The minimum z value in window coordinates.
         * In accordance with how window coordinates are usually defined, this should be 0,
         * however Scirenderer uses respectively -1 and +1 as the minimum and maximum z values
         * instead of 0 and 1. To be modified if deemed necessary.
         */
        private static final double ZNEAR = -1.0;

        /**
         * Constructor.
         */
        public TitlePositioner() {
                super();
                /* Title labels are drawn in window coordinates to avoid jittering. */
                useWindowCoordinates = true;
        }

        /**
         * Computes and returns the position of the label's anchor point,
         * obtained by adding the displacement vector to its position.
         * It additionally sets the displacement vector and label position members.
         * @return the position of the label's anchor point.
         */
        protected Vector3d computeDisplacedPosition() {
                Transformation canvasProjection = drawingTools.getTransformationManager().getCanvasProjection();

                Double [] axesBounds = {0.0, 0.0, 0.0, 0.0};
                Double [] margins = {0.0, 0.0, 0.0, 0.0};
                if (parentAxes != null) {
                    axesBounds = parentAxes.getAxesBounds();
                    margins = parentAxes.getMargins();
                }

                /* Compute the anchor point's position in window coordinates */
                double xmid = (axesBounds[0] + axesBounds[2] * margins[0] + 0.5*axesBounds[2]*(1.0-margins[0]-margins[1]));
                double ymid = (1.0 - axesBounds[1] + (margins[3]-1.0)*axesBounds[3] + axesBounds[3]*(1.0-margins[2]-margins[3]));

                Vector3d projAnchorPoint = new Vector3d(Math.floor((xmid) * (double) drawingTools.getCanvas().getWidth()),
                                                        Math.floor((ymid) * (double) drawingTools.getCanvas().getHeight()),
                                                        ZNEAR);

                Vector3d unprojAnchorPoint = new Vector3d(projAnchorPoint);
                unprojAnchorPoint = canvasProjection.unproject(unprojAnchorPoint);

                /* The anchor point is displaced along the y-axis by TITLEOFFSET pixels. */
                projAnchorPoint = projAnchorPoint.setY(projAnchorPoint.getY() + TITLEOFFSET);

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

