/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Pierre Lando
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.forge.scirenderer.ruler.graduations;

import java.text.DecimalFormat;
import java.util.List;

/**
 * @author Pierre Lando
 */
public interface Graduations {

    /**
     * Return the lower bound.
     * @return the lower bound.
     */
    double getLowerBound();

    /**
     * Return true if the lower bound is included.
     * @return true if the lower bound is included.
     */
    boolean isLowerBoundIncluded();

    /**
     * Return the upper bound.
     * @return the upper bound.
     */
    double getUpperBound();

    /**
     * Return true if the lower bound is included.
     * @return true if the lower bound is included.
     */
    boolean isUpperBoundIncluded();

    /**
     * Return true if the interval contain the given value.
     * @param value the given value.
     * @return true if the interval contain the given value.
     */
    boolean contain(double value);

    /**
     * Return an adapted number format.
     * @return an adapted number format.
     */
    DecimalFormat getFormat();

    /**
     * Return all values of this graduation.
     * @return all values of this graduation.
     */
    List<Double> getAllValues();

    /**
     * Return values not present in parents graduations.
     * @return values not present in parents graduations.
     */
    List<Double> getNewValues();

    /**
     * Return the parent graduation.
     * @return the parent graduation.
     */
    Graduations getParentGraduations();

    /**
     * Return a child graduation with more values.
     * @return a child graduation with more values.
     */
    Graduations getMore();

    /**
     * Return a child graduation with more values but less than <code>getMore()</code>
     * @return a child graduation with more values but less than <code>getMore()</code>
     */
    Graduations getAlternative();

    /**
     * Return a child graduation for sub ticks.
     * @return a child graduation for sub ticks.
     */
    Graduations getSubGraduations();

    /**
     * Get the list of subgraduations corresponding to N subticks between two main consecutives ticks
     * @param N the number of graduations
     * @return the corresponding list
     */
    List<Double> getSubGraduations(int N);

    /**
     * Return the density of sub ticks.
     * @return the density of sub ticks.
     */
    int getSubDensity();
}
