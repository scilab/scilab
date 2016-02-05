/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
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

package org.scilab.modules.renderer.JoGLView.axes.ruler;

import org.scilab.forge.scirenderer.ruler.graduations.Graduations;
import org.scilab.modules.graphic_objects.axes.AxisProperty;

import java.text.DecimalFormat;
import java.util.Arrays;
import java.util.Collections;
import java.util.LinkedList;
import java.util.List;

/**
 * @author Pierre Lando
 */
class UserDefineGraduation implements Graduations {
    private final AxisProperty axisProperty;
    private final double lowerBound;
    private final double upperBound;

    private List<Double> allValues;
    private List<Double> subValues;

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
            Double[] locs = axisProperty.getTicksLocations();
            final boolean log = axisProperty.getLogFlag();
            allValues = new LinkedList<Double>();
            for (Double d : locs) {
                final double v = log ? Math.log10(d) : d;
                if (contain(v)) {
                    allValues.add(d);
                }
            }
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
    public List<Double> getSubGraduations(final int N) {
        if (subValues == null) {
            if (N == 0) {
                subValues = new LinkedList<Double>();
                return subValues;
            }

            final boolean log = axisProperty.getLogFlag();
            List<Double> ticksValue = getAllValues();
            if (ticksValue.isEmpty()) {
                Double[] locs = axisProperty.getTicksLocations();
                ticksValue = new LinkedList<Double>();
                for (Double d : locs) {
                    ticksValue.add(d);
                }
            }

            Collections.sort(ticksValue);
            subValues = new LinkedList<Double>();

            if (!ticksValue.isEmpty()) {
                for (int i = 0; i < ticksValue.size() - 1; i++) {
                    final double first = ticksValue.get(i);
                    final double second = ticksValue.get(i + 1);
                    final double step = (second - first) / (N + 1);
                    double v = first;
                    for (int j = 0; j <= N; j++) {
                        final double d = log ? Math.log10(v) : v;
                        if (contain(d)) {
                            subValues.add(v);
                        }

                        v += step;
                    }
                }

                double v = ticksValue.get(ticksValue.size() - 1);
                final double d = log ? Math.log10(v) : v;
                if (contain(d)) {
                    subValues.add(v);
                }
            }
        }

        return subValues;
    }

    @Override
    public int getSubDensity() {
        return axisProperty.getSubticks() + 1;
    }
}
