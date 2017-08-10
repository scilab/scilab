/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Pierre Lando
 * Copyright (C) 2013-2015 - Scilab Enterprises - Calixte DENIZET
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

package org.scilab.forge.scirenderer.ruler.graduations;

import java.util.Collections;
import java.util.LinkedList;
import java.util.List;
import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;

/**
 * @author Pierre Lando
 */
public abstract class AbstractGraduations implements Graduations {

    protected static final double PRECISION = 1e-8;

    /** The left bracket used by {@link #toString()} */
    private static final String LEFT_BRACKET = "[";

    /** The right bracket used by {@link #toString()} */
    private static final String RIGHT_BRACKET = "]";

    /** True if the lower bound is included in the graduation interval. */
    private final boolean isLowerBoundIncluded;

    /** True if the upper bound is included in the graduation interval. */
    private final boolean isUpperBoundIncluded;

    /** Interval lower bound. */
    private final double lowerBound;

    /** Interval upper bound. */
    private final double upperBound;

    private final Graduations parentGraduations;
    private DecimalFormat numberFormat;
    protected List<Double> subValues;

    /**
     * Constructor from parent graduations.
     * This constructor copy information from given {@link Graduations} and set it as is parent.
     * @param parentGraduations the parent graduations to copy.
     */
    public AbstractGraduations(Graduations parentGraduations) {
        this.parentGraduations = parentGraduations;
        this.isLowerBoundIncluded = parentGraduations.isLowerBoundIncluded();
        this.isUpperBoundIncluded = parentGraduations.isUpperBoundIncluded();
        this.lowerBound = parentGraduations.getLowerBound();
        this.upperBound = parentGraduations.getUpperBound();
    }

    /**
     * Root constructor.
     * Graduations made this way don't have a parent.
     * @param lowerBound the actual lower bounds.
     * @param lowerBoundIncluded the actual lower bounds included status.
     * @param upperBound the actual upper bounds.
     * @param upperBoundIncluded the actual upper bounds included status.
     */
    public AbstractGraduations(double lowerBound, boolean lowerBoundIncluded, double upperBound, boolean upperBoundIncluded) {
        this.parentGraduations = null;
        this.isLowerBoundIncluded = lowerBoundIncluded;
        this.isUpperBoundIncluded = upperBoundIncluded;
        this.lowerBound = lowerBound;
        this.upperBound = upperBound;
    }

    /**
     * Root constructor.
     * Graduations made this way don't have a parent.
     * There bounds are included.
     * @param lowerBound the actual lower bounds included status.
     * @param upperBound the actual upper bounds included status.
     */
    public AbstractGraduations(double lowerBound, double upperBound) {
        this.parentGraduations = null;
        this.isLowerBoundIncluded = true;
        this.isUpperBoundIncluded = true;
        this.lowerBound = lowerBound;
        this.upperBound = upperBound;
    }

    /**
     * Child constructor.
     * @param parentGraduations the parent graduation.
     * @param lowerBound the actual lower bounds.
     * @param lowerBoundIncluded the actual lower bounds included status.
     * @param upperBound the actual upper bounds.
     * @param upperBoundIncluded the actual upper bounds included status.
     */
    public AbstractGraduations(Graduations parentGraduations, double lowerBound, boolean lowerBoundIncluded, double upperBound, boolean upperBoundIncluded) {
        this.parentGraduations = parentGraduations;
        this.isLowerBoundIncluded = lowerBoundIncluded;
        this.isUpperBoundIncluded = upperBoundIncluded;
        this.lowerBound = lowerBound;
        this.upperBound = upperBound;
    }

    @Override
    public final double getLowerBound() {
        return lowerBound;
    }

    @Override
    public final boolean isLowerBoundIncluded() {
        return isLowerBoundIncluded;
    }

    @Override
    public final double getUpperBound() {
        return upperBound;
    }

    @Override
    public final boolean isUpperBoundIncluded() {
        return isUpperBoundIncluded;
    }

    @Override
    public final Graduations getParentGraduations() {
        return parentGraduations;
    }

    @Override
    public final boolean contain(double value) {
        if (value == lowerBound) {
            return isLowerBoundIncluded;
        }
        if (value == upperBound) {
            return isUpperBoundIncluded;
        }
        return (lowerBound < value) && (value < upperBound);
    }

    /**
     * Equivalent to contain but for interval [0, upper-lower] (to avoid rounding error in computations)
     */
    public final boolean containRelative(double value) {
        if (value == 0 || Math.abs(value / (upperBound - lowerBound)) <= PRECISION) {
            return isLowerBoundIncluded;
        }
        if (value == upperBound - lowerBound || Math.abs(1 - value / (upperBound - lowerBound)) <= PRECISION) {
            return isUpperBoundIncluded;
        }
        return (0 < value) && (value < upperBound - lowerBound);
    }

    @Override
    public final DecimalFormat getFormat() {
        if (numberFormat == null) {
            double maxDisplayedValue = Math.max(Math.abs(lowerBound), Math.abs(upperBound));
            double len = Math.abs(upperBound - lowerBound);

            if (maxDisplayedValue < 1e-3) {
                numberFormat = new DecimalFormat("0.##########E00");
            } else if (false && len <= 1e-3) {
                // desactivated for now...
                // the user should be able to do that itself
                numberFormat = new TinyIntervalFormat("0.####E00", "0.##E00");
            } else if (maxDisplayedValue >= 1e6) {
                numberFormat = new DecimalFormat("0.##########E00");
            } else if (maxDisplayedValue < 1) {
                numberFormat = new DecimalFormat("0.######");
            } else {
                numberFormat = new DecimalFormat("#,##0.####");
            }

            DecimalFormatSymbols decimalFormatSymbols = numberFormat.getDecimalFormatSymbols();
            decimalFormatSymbols.setExponentSeparator("e");
            numberFormat.setDecimalFormatSymbols(decimalFormatSymbols);
        }
        return numberFormat;
    }

    @Override
    public List<Double> getSubGraduations(final int N) {
        if (subValues == null) {
            List<Double> ticksValue = getAllValues();
            if (N == 0 || ticksValue.size() == 0) {
                subValues = new LinkedList<Double>();
            } else {
                Collections.sort(ticksValue);
                subValues = new LinkedList<Double>();

                for (int i = 0; i < ticksValue.size() - 1; i++) {
                    final double first = ticksValue.get(i);
                    final double second = ticksValue.get(i + 1);
                    final double step = (second - first) / (N + 1);
                    double v = first;
                    for (int j = 0; j <= N; j++) {
                        subValues.add(v);
                        v += step;
                    }
                }
                subValues.add(ticksValue.get(ticksValue.size() - 1));
            }
        }

        return subValues;
    }

    @Override
    public String toString() {
        String lowerBoundBracket;
        String upperBoundBracket;

        if (isLowerBoundIncluded) {
            lowerBoundBracket = LEFT_BRACKET;
        } else {
            lowerBoundBracket = RIGHT_BRACKET;
        }

        if (isUpperBoundIncluded) {
            upperBoundBracket = RIGHT_BRACKET;
        } else {
            upperBoundBracket = LEFT_BRACKET;
        }
        return getClass().getSimpleName() + lowerBoundBracket
               + getFormat().format(lowerBound) + ", "
               + getFormat().format(upperBound) + upperBoundBracket;
    }
}
