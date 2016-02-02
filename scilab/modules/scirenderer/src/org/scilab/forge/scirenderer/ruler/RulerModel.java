/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
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
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.tranformations.Vector3d;

/**
 * @author Pierre Lando
 */
public interface RulerModel {

    /**
     * Default first value.
     */
    double DEFAULT_FIRST_VALUE = 0;

    /**
     * Default second value.
     */
    double DEFAULT_SECOND_VALUE = 1;

    /**
     * Default first point position.
     */
    Vector3d DEFAULT_FIRST_POINT = new Vector3d(0, 0, 0);

    /**
     * Default second point position.
     */
    Vector3d DEFAULT_SECOND_POINT = new Vector3d(1, 0, 0);

    /**
     * Default margin in pixel.
     */
    double DEFAULT_MARGIN = 12.0;

    /**
     * Default main line visibility.
     */
    boolean DEFAULT_LINE_VISIBLE = true;

    /**
     * Default {@link org.scilab.forge.scirenderer.sprite.Sprite} distance to the main line in pixel.
     */
    int DEFAULT_SPRITE_DISTANCE = 12;

    /**
     * Default sub-tick length in pixel.
     */
    int DEFAULT_SUB_TICK_LENGTH = 5;

    /**
     * Default tick length in pixel.
     */
    int DEFAULT_TICK_LENGTH = 10;

    /**
     * Default minimal sub-ticks distance.
     */
    double DEFAULT_MINIMAL_SUB_TICKS_DISTANCE = 8;

    /**
     * Default auto-ticks status.
     */
    boolean DEFAULT_AUTO_TICKS_STATUS = true;

    /**
     * Default logarithmic status.
     */
    boolean DEFAULT_LOGARITHMIC_STATUS = false;

    /**
     * Default ticks direction.
     */
    Vector3d DEFAULT_TICKS_DIRECTION = new Vector3d(1, 0, 0);

    /**
     * Default color.
     */
    Color DEFAULT_COLOR = new Color(0, 0, 0);

    /**
     * Default line width.
     */
    double DEFAULT_LINE_WIDTH = 1.0;

    /**
     * Return used graduation to draw this ruler.
     * @return the used graduation to draw this ruler.
     */
    Graduations getGraduations();

    /**
     * First value getter.
     * @return the first values.
     */
    double getFirstValue();

    /**
     * Second value getter.
     * @return the second values.
     */
    double getSecondValue();

    /**
     * First point getter.
     * @return the first point.
     */
    Vector3d getFirstPoint();

    /**
     * Second point getter.
     * @return the second point.
     */
    Vector3d getSecondPoint();

    /**
     * Ticks direction getter.
     * @return the ticks direction.
     */
    Vector3d getTicksDirection();

    /**
     * Ticks length getter.
     * @return the ticks length in pixel.
     */
    int getTicksLength();

    /**
     * Sub-ticks length getter.
     * @return the sub-ticks length in pixel.
     */
    int getSubTicksLength();

    /**
     * Return the position corresponding to the given value.
     * @param value the given value.
     * @return the position corresponding to the given value.
     */
    Vector3d getPosition(double value);

    /**
     * Return the accepted margin (in pixel) for label drawing.
     * @return the accepted margin (in pixel) for label drawing.
     */
    double getMargin();

    /**
     * Line visibility getter.
     * @return the line visibility status.
     */
    boolean isLineVisible();

    /**
     * Auto-ticking getter.
     * @return the auto-ticking status.
     */
    boolean isAutoTicks();

    /**
     * Logarithmic state getter.
     * @return the logarithmic state
     */
    boolean isLogarithmic();

    /**
     * Sprite distance getter.
     * @return the sprite distance (in pixel) between the ruler line and the sprites edges.
     */
    int getSpriteDistance();

    /**
     * Minimal accepted sub-ticks distance getter.
     * @return the minimal accepted sub-ticks distance.
     */
    double getMinimalSubTicksDistance();

    /**
     * Color getter.
     * @return the color of ruler line and ticks.
     */
    Color getColor();

    /**
     * Line width getter.
     * @return the line width of ruler line, grid and ticks.
     */
    double getLineWidth();

    /**
     * Number of subticks.
     * @return the number of subticks or -1 if the computation is automatic.
     */
    int getSubticksNumber();

    /**
     * Get the format to print the ticks label
     * @return the format
     */
    String getFormat();

    /**
     * Get the scale factor to recompute label value
     * @return the scale factor
     */
    double getScale();

    /**
     * Get the translate factor to recompute label value
     * @return the translate factor
     */
    double getTranslate();
}
