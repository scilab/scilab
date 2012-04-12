/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView.axes.ruler;

import org.scilab.forge.scirenderer.ruler.graduations.Graduations;
import org.scilab.modules.graphic_objects.axes.AxisProperty;

import java.text.DecimalFormat;
import java.util.Arrays;
import java.util.List;

/**
 * @author Pierre Lando
 */
class UserDefineGraduation implements Graduations {
    private final AxisProperty axisProperty;
    private final double lowerBound;
    private final double upperBound;

    private List<Double> allValues;

    public UserDefineGraduation(AxisProperty axisProperty, double lowerBound, double upperBound) {
        this.axisProperty = axisProperty;
        this.lowerBound = lowerBound;
        this.upperBound = upperBound;
    }

    @Override
    public double getLowerBound() {
        return lowerBound;
    }

    @Override
    public boolean isLowerBoundIncluded() {
        return true;
    }

    @Override
    public double getUpperBound() {
        return upperBound;
    }

    @Override
    public boolean isUpperBoundIncluded() {
        return true;
    }

    @Override
    public boolean contain(double value) {
        return (getLowerBound() <= value) && (value <= getUpperBound());
    }

    @Override
    public DecimalFormat getFormat() {
        /**
         * Will never be used. The label was defined by the user too.
         */
        return new DecimalFormat();
    }

    @Override
    public List<Double> getAllValues() {
        if (allValues == null) {
            allValues = Arrays.asList(axisProperty.getTicksLocations());
        }
        return allValues;
    }

    @Override
    public List<Double> getNewValues() {
        return getAllValues();
    }

    @Override
    public Graduations getParentGraduations() {
        return null;
    }

    @Override
    public Graduations getMore() {
        return null;
    }

    @Override
    public Graduations getAlternative() {
        return null;
    }

    @Override
    public Graduations getSubGraduations() {
        // TODO
        return null;
    }

    @Override
    public int getSubDensity() {
        return axisProperty.getSubticks() + 1;
    }

}
