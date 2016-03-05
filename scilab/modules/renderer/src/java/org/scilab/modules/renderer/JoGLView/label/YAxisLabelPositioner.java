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
