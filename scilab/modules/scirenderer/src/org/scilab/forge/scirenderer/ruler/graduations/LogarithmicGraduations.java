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

/**
 * @author Pierre Lando
 */
public final class LogarithmicGraduations extends AbstractGraduations implements Graduations {

    /**
     * Exponent of the step.
     * Possible value are : 1, 2 and 3*k with k > 0
     */
    private final int stepExponent;

    private List<Double> allValues;
    private Graduations subGraduation;
    private Graduations moreGraduation;
    private Graduations alternativeGraduation;

    /**
     * Private child constructor.
     * The interval is copied from parent's one.
     * @param parentGraduations the parent graduation.
     * @param stepExponent the step exponent.
     */
    private LogarithmicGraduations(Graduations parentGraduations, int stepExponent) {
        super(parentGraduations);
        this.stepExponent = stepExponent;
    }

    /**
     * Private root graduation constructor.
     * This graduation has no parent.
     * @param lowerBound actual lower bound.
     * @param lowerBoundIncluded true if the lower bound is included in the interval.
     * @param upperBound actual upper bound.
     * @param upperBoundIncluded true if the upper bound is included in the interval.
     */
    private LogarithmicGraduations(double lowerBound, boolean lowerBoundIncluded, double upperBound, boolean upperBoundIncluded) {
        super(lowerBound, lowerBoundIncluded, upperBound, upperBoundIncluded);
        if (lowerBound != upperBound) {
            stepExponent = 1;
        } else {
            stepExponent = 0;
            allValues = new LinkedList<Double>();
            allValues.add(lowerBound);
        }
    }

    public static LogarithmicGraduations create(double lowerBound, double upperBound) {
        return create(lowerBound, true, upperBound, true);
    }

    public static LogarithmicGraduations create(double lowerBound, boolean lowerBoundIncluded, double upperBound, boolean upperBoundIncluded) {
        if (lowerBound < upperBound) {
            return new LogarithmicGraduations(lowerBound, lowerBoundIncluded, upperBound, upperBoundIncluded);
        } else {
            return new LogarithmicGraduations(upperBound, upperBoundIncluded, lowerBound, lowerBoundIncluded);
        }
    }

    @Override
    public List<Double> getAllValues() {
        if (allValues == null) {
            allValues = new LinkedList<Double>();
            int currentExponent = (int) Math.ceil(Math.log10(getLowerBound()));
            double currentValue = Math.pow(10, currentExponent);
            final double step = Math.pow(10, stepExponent);

            if ((currentValue == getLowerBound()) && (!isLowerBoundIncluded())) {
                currentValue *= step;
            }

            while (contain(currentValue) && !Double.isInfinite(currentValue)) {
                allValues.add(currentValue);
                currentValue *= step;
            }
        }
        return allValues;
    }

    @Override
    public List<Double> getNewValues() {
        return getAllValues();
    }

    @Override
    public Graduations getMore() {
        if (moreGraduation == null) {
            moreGraduation = new LinLogGraduation(this);
        }
        return moreGraduation;
    }

    @Override
    public Graduations getAlternative() {
        if (alternativeGraduation == null) {
            int nextStep = 3 + stepExponent - stepExponent % 3;
            alternativeGraduation = new LogarithmicGraduations(this, nextStep);
        }
        return alternativeGraduation;
    }

    @Override
    public Graduations getSubGraduations() {
        if (subGraduation == null) {
            if (stepExponent > 1) {
                subGraduation = new LogarithmicGraduations(this, stepExponent / 3);
            } else {
                subGraduation = new LinLogGraduation(this).getSubGraduations();
            }
        }
        return subGraduation;
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
                    final double first = Math.log10(ticksValue.get(i));
                    final double second = Math.log10(ticksValue.get(i + 1));
                    final double step = (second - first) / (N + 1);
                    double v = first;
                    for (int j = 0; j <= N; j++) {
                        subValues.add(Math.pow(10, v));
                        v += step;
                    }
                }
                subValues.add(ticksValue.get(ticksValue.size() - 1));
            }
        }

        return subValues;
    }

    @Override
    public int getSubDensity() {
        if (stepExponent >= 3) {
            return 3;
        } else if (stepExponent == 2) {
            return stepExponent;
        } else {
            return getSubGraduations().getSubDensity();
        }
    }

    @Override
    public String toString() {
        String s = super.toString();
        s += "; stepExponent=" + stepExponent + "; parent=" + getParentGraduations();

        return s;
    }

    /**
     * This class manage linear graduation between 10^n and 10^(n+1)
     */
    private class LinLogGraduation extends AbstractGraduations implements Graduations {
        private Graduations alternativeLLGraduation;
        private Graduations moreLLGraduation;
        private Graduations subLLGraduation;

        private List<Double> allValues;
        private List<Double> newValues;

        private final List<Graduations> graduationsList;

        public LinLogGraduation(LogarithmicGraduations parentGraduations) {
            super(parentGraduations);
            graduationsList = computeGraduationsList();
        }

        private LinLogGraduation(Graduations parentGraduations, List<Graduations> graduationsList) {
            super(parentGraduations);
            this.graduationsList = graduationsList;
        }

        @Override
        public List<Double> getAllValues() {
            if (allValues == null) {
                allValues = new LinkedList<Double>();
                for (Graduations graduations : graduationsList) {
                    allValues.addAll(graduations.getAllValues());
                }

                allValues.addAll(getLogarithmicParent().getAllValues());
            }
            return allValues;
        }

        @Override
        public List<Double> getNewValues() {
            if (newValues == null) {
                newValues = new LinkedList<Double>();
                if (getParentGraduations() instanceof LogarithmicGraduations) {
                    for (Graduations graduations : graduationsList) {
                        newValues.addAll(graduations.getAllValues());
                    }
                } else {
                    for (Graduations graduations : graduationsList) {
                        newValues.addAll(graduations.getNewValues());
                    }
                }
            }
            return newValues;
        }

        @Override
        public Graduations getMore() {
            if (moreLLGraduation == null) {
                List<Graduations> moreList = new LinkedList<Graduations>();
                for (Graduations graduations : graduationsList) {
                    Graduations more = graduations.getMore();
                    if (more != null) {
                        moreList.add(more);
                    }
                }
                if (!moreList.isEmpty()) {
                    moreLLGraduation = new LinLogGraduation(this, moreList);
                }
            }
            return moreLLGraduation;
        }

        @Override
        public Graduations getAlternative() {
            if (alternativeLLGraduation == null) {
                List<Graduations> alternativeList = new LinkedList<Graduations>();
                for (Graduations graduations : graduationsList) {
                    Graduations alternative = graduations.getAlternative();
                    if (alternative != null) {
                        alternativeList.add(alternative);
                    }
                }
                if (!alternativeList.isEmpty()) {
                    alternativeLLGraduation = new LinLogGraduation(this, alternativeList);
                }
            }
            return alternativeLLGraduation;
        }

        @Override
        public Graduations getSubGraduations() {
            if (subLLGraduation == null) {
                List<Graduations> subList = new LinkedList<Graduations>();
                for (Graduations graduations : graduationsList) {
                    Graduations sub = graduations.getSubGraduations();
                    if (sub != null) {
                        subList.add(sub);
                    }
                }
                if (subList.isEmpty()) {
                    subLLGraduation = getMore().getSubGraduations();
                } else {
                    subLLGraduation = new LinLogGraduation(this, subList);
                }
            }
            return subLLGraduation;
        }

        @Override
        public int getSubDensity() {
            return 0;
        }

        private List<Graduations> computeGraduationsList() {
            List<Graduations> list = new LinkedList<Graduations>();

            /**
             * Let a and b a power of 10.
             * lowerBound < a < b < upperBound
             */

            double aPower = Math.ceil(Math.log10(getLowerBound()));
            double bPower = Math.floor(Math.log10(getUpperBound()));
            double a = Math.pow(10, aPower);
            double b = Math.pow(10, bPower);

            if (aPower > bPower) {
                // Case of 10^n <= a < b <= 10^(n+1)
                list.add(LinearGraduations.create(
                             this,
                             getLowerBound(), true,
                             getUpperBound(), true
                         ));
            } else {
                if (a != getLowerBound()) {
                    list.add(LinearGraduations.create(
                                 this,
                                 getLowerBound(), true,
                                 a, false
                             ));
                }

                if (aPower != bPower) {
                    // Limit iterations on power smaller than 10^310 (Max double ~ 10^308)
                    for (double i = aPower; i < Math.min(bPower, 310); i++) {
                        list.add(LinearGraduations.create(
                                     this,
                                     Math.pow(10, i), false,
                                     Math.pow(10, i + 1), false
                                 ));
                    }
                }

                if (b != getUpperBound()) {
                    list.add(LinearGraduations.create(
                                 this,
                                 b, false,
                                 getUpperBound(), true
                             ));
                }
            }

            return list;
        }

        private Graduations getLogarithmicParent() {
            Graduations currentGraduation = getParentGraduations();
            while (!(currentGraduation instanceof LogarithmicGraduations)) {
                currentGraduation = currentGraduation.getParentGraduations();
            }
            return currentGraduation;
        }
    }
}
