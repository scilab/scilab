/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Pierre Lando
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

package org.scilab.forge.scirenderer.texture;

import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.appearance.Color;

import javax.swing.Icon;

/**
 *
 * Interface for the sprite drawing tools.
 *
 *
 * @author Pierre Lando
 */
public interface TextureDrawingTools {

    /**
     * Draw a plus.
     * @param size the plus size.
     * @param appearance the used appearance.
     */
    void drawPlus(int size, Appearance appearance);

    /**
     * Draw a polyline.
     * @param coordinates polyline's point coordinates.
     * @param appearance  the used appearance.
     */
    void drawPolyline(int[] coordinates, Appearance appearance);


    /**
     * Fill a polygon.
     * @param coordinates polygon's point coordinates.
     * @param appearance  the used appearance.
     */
    void fillPolygon(int[] coordinates, Appearance appearance);

    /**
     * Draw a circle.
     * @param x          the x coordinate of the circle center.
     * @param y          the y coordinate of the circle center.
     * @param diameter   the circle diameter.
     * @param appearance the circle appearance.
     */
    void drawCircle(int x, int y, int diameter, Appearance appearance);

    /**
     * Fill a disc of given diameter, centered at (x, y) with the given appearance.
     * @param x        the x coordinate of the disc center.
     * @param y        the y coordinate of the disc center.
     * @param diameter the disc diameter.
     * @param color    the disc color.
     */
    void fillDisc(int x, int y, int diameter, Color color);

    /**
     * Draw the given text at the given position with the given appearance.
     * @param textEntity the text entity to draw.
     * @param x the x text position.
     * @param y the y text position.
     */
    void draw(TextEntity textEntity, int x, int y);

    /**
     * Draw the given {@link javax.swing.Icon} at the given position.
     * @param icon  the given icon to paint.
     * @param x the x text position.
     * @param y the y text position.
     */
    void draw(Icon icon, int x, int y);

    /**
     * Clear the sprite with the given color.
     * @param color the new background color.
     */
    void clear(Color color);
}
