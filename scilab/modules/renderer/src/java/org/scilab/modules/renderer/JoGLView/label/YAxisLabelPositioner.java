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

import org.scilab.modules.graphic_objects.axes.Camera;

/**
 * YAxisLabelPositioner class.
 *
 * Implements automatic rotation specific to y-axis labels.
 * Refer to the parent class {@see AxisLabelPositioner} for the full
 * implementation of positioning.
 *
 * @author Manuel JULIACHS
 */
public class YAxisLabelPositioner extends AxisLabelPositioner {

        /**
         * Constructor.
         */
        public YAxisLabelPositioner() {
                super();
        }

        /**
         * Returns the automatically computed rotation angle.
         * It is equal to 270 when the parent axes' view mode is set to 2D, 0
         * in any other case.
         * @return the rotation angle.
         */
        protected double getAutoRotationAngle() {
                if (parentAxes != null && parentAxes.getViewAsEnum() == Camera.ViewType.VIEW_3D) {
                        return 0.0;
                } else {
                        return 270.0;
                }
        }

}

