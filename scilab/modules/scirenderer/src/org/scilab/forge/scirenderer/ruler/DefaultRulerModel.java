/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Pierre Lando
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
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

package org.scilab.forge.scirenderer.ruler;

import org.scilab.forge.scirenderer.ruler.graduations.Graduations;
import org.scilab.forge.scirenderer.ruler.graduations.LinearGraduations;
import org.scilab.forge.scirenderer.ruler.graduations.LogarithmicGraduations;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.tranformations.Vector3d;

/**
 * Default ruler model.
 *
 * @author Pierre Lando
 */
public final class DefaultRulerModel implements RulerModel {

    /**
     * Value associated with the first point.
     */
    private double firstValue = DEFAULT_FIRST_VALUE;

    /**
     * Value associated with the second point.
     */
    private double secondValue = DEFAULT_SECOND_VALUE;

    /**
     * First point position in world coordinate.
     */
    private Vector3d firstPoint = DEFAULT_FIRST_POINT;

    /**
     * Second point position in world coordinate.
     */
    private Vector3d secondPoint = DEFAULT_SECOND_POINT;

    /**
     * Ticks direction, in world coordinate.
     */
    private Vector3d ticksDirection = DEFAULT_TICKS_DIRECTION;

    /**
     * Ticks length, in pixel.
     */
    private int ticksLength = DEFAULT_TICK_LENGTH;

    /**
     * Sub ticks length, in pixel.
     */
    private int subTicksLength = DEFAULT_SUB_TICK_LENGTH;

    /**
     * Minimal sub-ticks distance.
     */
    private double minimalSubTicksDistance = DEFAULT_MINIMAL_SUB_TICKS_DISTANCE;

    /**
     * Sprite distance to segment, in pixel.

     */
    private int spriteDistance = DEFAULT_SPRITE_DISTANCE;

    /**
     * The graduations.
     */
    private Graduations graduations;

    /**
     * Current margin.
     * The margin is the minimal allowed distance between to sprite of the same ruler.
     */
    private double margin = DEFAULT_MARGIN;

    /**
     * Current line visibility.
     */
    private boolean lineVisible = DEFAULT_LINE_VISIBLE;

    /**
     * Auto-ticks status.
     */
    private boolean isAutoTicks = DEFAULT_AUTO_TICKS_STATUS;

    /**
     * Logarithmic status.
     */
    private boolean isLogarithmic = DEFAULT_LOGARITHMIC_STATUS;

    /**
     * Current color.
     */
    private Color color = DEFAULT_COLOR;

    /**
     * Used graduations when {@link this#isAutoTicks} is false.
     */
    private Graduations userGraduation;

    /**
     * Line width
     */
    private double lineWidth = DEFAULT_LINE_WIDTH;

    private int subticksNumber = -1;
    private String format = "";
    private double scale;
    private double translate;

    /**
     * Default constructor.
     */
    public DefaultRulerModel() {
    }

    @Override
    public double getFirstValue() {
        return firstValue;
    }

    /**
     * First value setter.
     * @param firstValue the new first value.
     */
    public void setFirstValue(double firstValue) {
        graduations = null;
        this.firstValue = (Double.isInfinite(firstValue) ? Double.MAX_VALUE : firstValue);
    }

    @Override
    public double getSecondValue() {
        return secondValue;
    }

    /**
     * Second value setter.
     * @param secondValue the new second value.
     */
    public void setSecondValue(double secondValue) {
        graduations = null;
        this.secondValue = (Double.isInfinite(secondValue) ? Double.MAX_VALUE : secondValue);
    }

    /**
     * Set the first and second value in one call.
     * @param firstValue the first value.
     * @param secondValue the second value.
     */
    public void setValues(double firstValue, double secondValue) {
        setFirstValue(firstValue);
        setSecondValue(secondValue);
    }

    @Override
    public Vector3d getFirstPoint() {
        return firstPoint;
    }

    /**
     * First point setter.
     * @param firstPoint the new first point.
     */
    public void setFirstPoint(Vector3d firstPoint) {
        this.firstPoint = firstPoint;
    }

    @Override
    public Vector3d getSecondPoint() {
        return secondPoint;
    }

    /**
     * Second point setter.
     * @param secondPoint the new second point.
     */
    public void setSecondPoint(Vector3d secondPoint) {
        this.secondPoint = secondPoint;
    }

    /**
     * Point setter.
     * @param firstPoint the new first point.
     * @param secondPoint the new second point.
     */
    public void setPoints(Vector3d firstPoint, Vector3d secondPoint) {
        this.firstPoint = firstPoint;
        this.secondPoint = secondPoint;
    }

    @Override
    public Vector3d getTicksDirection() {
        return ticksDirection;
    }

    /**
     * Ticks direction setter.
     * @param ticksDirection the new ticks direction.
     */
    public void setTicksDirection(Vector3d ticksDirection) {
        this.ticksDirection = ticksDirection;
    }

    @Override
    public int getTicksLength() {
        return ticksLength;
    }

    /**
     * Ticks length setter.
     * @param ticksLength the new tick length in pixels.
     */
    public void setTicksLength(int ticksLength) {
        this.ticksLength = ticksLength;
    }

    @Override
    public int getSubTicksLength() {
        return subTicksLength;
    }

    /**
     * Sub-ticks length setter.
     * @param subTicksLength the new sub-tick length in pixels.
     */
    public void setSubTicksLength(int subTicksLength) {
        this.subTicksLength = subTicksLength;
    }

    @Override
    public Graduations getGraduations() {
        if (isAutoTicks) {
            if (graduations == null) {
                if (isLogarithmic) {
                    graduations = LogarithmicGraduations.create(getFirstValue(), getSecondValue());
                } else {
                    graduations = LinearGraduations.create(getFirstValue(), getSecondValue());
                }
            }
            return graduations;
        } else {
            return userGraduation;
        }
    }

    /**
     * User graduation setter.
     * @param graduations the new user-defined graduations.
     */
    public void setUserGraduation(Graduations graduations) {
        this.userGraduation = graduations;
    }

    @Override
    public Vector3d getPosition(double value) {
        if ((firstPoint != null) && (secondPoint != null)) {
            if (isLogarithmic()) {
                double valueLog = Math.log10(value);
                double firstValueLog = Math.log10(firstValue);
                double secondValueLog = Math.log10(secondValue);
                double s = (secondValueLog - firstValueLog);
                double k1 = (valueLog - firstValueLog) / s;
                double k2 = (secondValueLog - valueLog) / s;
                return firstPoint.times(k2).plus(secondPoint.times(k1));
            } else {
                double s = (secondValue - firstValue);
                return firstPoint.times((secondValue - value) / s).plus(secondPoint.times((value - firstValue) / s));
            }
        } else {
            return null;
        }
    }

    @Override
    public double getMargin() {
        return margin;
    }

    /**
     * Margin setter.
     * The margin is minimal distance accepted between ticks labels.
     * @param margin the new margin value.
     */
    public void setMargin(double margin) {
        this.margin = margin;
    }

    @Override
    public boolean isLineVisible() {
        return lineVisible;
    }

    /**
     * Line visibility setter.
     * @param lineVisible the new line visibility status.
     */
    public void setLineVisible(boolean lineVisible) {
        this.lineVisible = lineVisible;
    }

    @Override
    public boolean isAutoTicks() {
        return isAutoTicks;
    }

    /**
     * Auto-ticks parameter setter.
     * If it set to {@code false}, the user defined graduation will be used.
     * @param isAutoTicks the new auto-ticks status.
     */
    public void setAutoTicks(boolean isAutoTicks) {
        this.isAutoTicks = isAutoTicks;
    }

    @Override
    public boolean isLogarithmic() {
        return isLogarithmic;
    }

    /**
     * Logarithmic or linear setter
     * @param isLogarithmic the new logarithmic status.
     */
    public void setLogarithmic(boolean isLogarithmic) {
        if (isLogarithmic != this.isLogarithmic) {
            this.isLogarithmic = isLogarithmic;
            graduations = null;
        }
    }

    @Override
    public int getSpriteDistance() {
        return spriteDistance;
    }

    /**
     * Sprite distance setter.
     * @param spriteDistance the new sprite distance to the main ticks in pixel.
     */
    public void setSpriteDistance(int spriteDistance) {
        this.spriteDistance = spriteDistance;
    }

    @Override
    public double getMinimalSubTicksDistance() {
        return minimalSubTicksDistance;
    }

    /**
     * Minimal accepted distance between sub-ticks setter.
     * @param minimalSubTicksDistance the new minimal accepted distance between sub-ticks.
     */
    public void setMinimalSubTicksDistance(double minimalSubTicksDistance) {
        this.minimalSubTicksDistance = minimalSubTicksDistance;
    }

    @Override
    public Color getColor() {
        return color;
    }

    /**
     * Ruler color setter.
     * @param color the new rule color.
     */
    public void setColor(Color color) {
        this.color = color;
    }

    @Override
    public double getLineWidth() {
        return lineWidth;
    }

    /**
     * Ruler line width setter
     * @param lineWidth the new line width
     */
    public void setLineWidth(double lineWidth) {
        this.lineWidth = lineWidth;
    }

    @Override
    public int getSubticksNumber() {
        return subticksNumber;
    }

    /**
     * Set number of subticks.
     * @param N the number of subticks or -1 if the computation is automatic.
     */
    public void setSubticksNumber(int N) {
        this.subticksNumber = N;
    }

    @Override
    public String getFormat() {
        return format;
    }

    /**
     * Set the format.
     * @param format the format as used in String.format.
     */
    public void setFormat(String format) {
        this.format = format;
    }

    @Override
    public double getScale() {
        return scale;
    }

    @Override
    public double getTranslate() {
        return translate;
    }

    /**
     * Set the scale-translate factors.
     * @param .
     */
    public void setSTFactors(Double[] factors) {
        this.scale = factors[0];
        this.translate = factors[1];
    }
}
