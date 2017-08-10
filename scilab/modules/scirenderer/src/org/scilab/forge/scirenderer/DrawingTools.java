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

package org.scilab.forge.scirenderer;

import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.clipping.ClippingManager;
import org.scilab.forge.scirenderer.lightning.LightManager;
import org.scilab.forge.scirenderer.renderer.Renderer;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.texture.AnchorPosition;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.tranformations.TransformationManager;
import org.scilab.forge.scirenderer.tranformations.Vector3d;

/**
 * @author Pierre Lando
 */
public interface DrawingTools {

    /**
     * Return the canvas where draw is done.
     * @return the canvas where draw is done.
     */
    Canvas getCanvas();

    /**
     * Return the transformation manager.
     * @return the transformation manager.
     */
    TransformationManager getTransformationManager();

    /**
     * Return the light manager.
     * @return the light manager.
     */
    LightManager getLightManager();

    /**
     * Return the clipping manager.
     * @return the clipping manager.
     */
    ClippingManager getClippingManager();

    /**
     * Clear the canvas with the given color.
     * @param color the color used to clear the canvas.
     */
    void clear(Color color);

    /**
     * Clear the canvas with the given color.
     * @param color the color used to clear the canvas.
     */
    void clear(java.awt.Color color);

    /**
     * Clear the depth buffer.
     */
    void clearDepthBuffer();

    /**
     * Ask the given renderer to perform a draw.
     * @param renderer the given renderer.
     */
    void draw(Renderer renderer);

    /**
     * Draw the given geometry with default appearance.
     * @param geometry the geometry to draw.
     * @throws SciRendererException if the draw is not possible.
     */
    void draw(Geometry geometry) throws SciRendererException;

    /**
     * Draw the given geometry.
     * @param geometry the geometry to draw.
     * @param appearance the appearance to use.
     * @throws SciRendererException if the draw is not possible.
     */
    void draw(Geometry geometry, Appearance appearance) throws SciRendererException;

    /**
     * Draw the texture on XY plane in current coordinate.
     * The texture is drawn on the rectangle [(0,0)-(texture width,texture height)].
     * @param texture the texture to drawn.
     * @throws SciRendererException if the texture is not drawable.
     */
    void draw(Texture texture) throws SciRendererException;

    /**
     * Draw the given texture at all given position.
     * @param texture the texture to draw.
     * @param anchor the texture anchor position.
     * @param positions the positions where the texture will be drawn.
     * @throws SciRendererException if the texture is not drawable.
     */
    void draw(Texture texture, AnchorPosition anchor, ElementsBuffer positions) throws SciRendererException;

    /**
     * Draw the given texture at all given position with the given rotation angle.
     * @param texture the texture to draw.
     * @param anchor the texture anchor position.
     * @param positions the positions where the texture will be drawn.
     * @param rotationAngle the rotation angle.
     * @throws SciRendererException if the texture is not drawable.
     */
    void draw(Texture texture, AnchorPosition anchor, ElementsBuffer positions, double rotationAngle) throws SciRendererException;

    /**
     * Draw the given texture at all given position with selected colors.
     * @param texture the texture to draw.
     * @param anchor the texture anchor position.
     * @param positions the positions where the texture will be drawn.
     * @param colors the colors of the texture.
     * @throws SciRendererException if the texture is not drawable.
     */
    void draw(Texture texture, AnchorPosition anchor, ElementsBuffer positions, Color auxColor, ElementsBuffer colors) throws SciRendererException;

    /**
     * Draw the given texture at all given position with the given rotation angle with selected colors.
     * @param texture the texture to draw.
     * @param anchor the texture anchor position.
     * @param positions the positions where the texture will be drawn.
     * @param rotationAngle the rotation angle.
     * @param colors the colors of the texture.
     * @throws SciRendererException if the texture is not drawable.
     */
    void draw(Texture texture, AnchorPosition anchor, ElementsBuffer positions, double rotationAngle, Color auxColor, ElementsBuffer colors) throws SciRendererException;

    /**
     * Draw the given texture at all given position with given offset and stride with the given rotation angle.
     * @param texture the texture to draw.
     * @param anchor the texture anchor position.
     * @param positions the positions where the texture will be drawn.
     * @param offset the position of the first mark.
     * @param stride the spacing between two marks.
     * @param rotationAngle the rotation angle.
     * @throws SciRendererException if the texture is not drawable.
     */
    void draw(Texture texture, AnchorPosition anchor, ElementsBuffer positions, int offset, int stride, double rotationAngle) throws SciRendererException;

    /**
     * Draw the given texture at all given position with given offset and stride with the given rotation angle with the given color.
     * @param texture the texture to draw.
     * @param anchor the texture anchor position.
     * @param positions the positions where the texture will be drawn.
     * @param offset the position of the first mark.
     * @param stride the spacing between two marks.
     * @param rotationAngle the rotation angle.
     * @param colors the colors of the texture.
     * @throws SciRendererException if the texture is not drawable.
     */
    void draw(Texture texture, AnchorPosition anchor, ElementsBuffer positions, int offset, int stride, double rotationAngle, Color auxColor, ElementsBuffer colors) throws SciRendererException;

    /**
     * Draw the given texture at given position.
     * @param texture the texture to draw.
     * @param anchor the texture anchor position.
     * @param position the position where the texture will be drawn.
     * @throws SciRendererException if the texture is not drawable.
     */
    void draw(Texture texture, AnchorPosition anchor, Vector3d position) throws SciRendererException;

    /**
     * Draw the given texture at given position with the given rotation angle.
     * @param texture the texture to draw.
     * @param anchor the texture anchor position.
     * @param position the position where the texture will be drawn.
     * @param rotationAngle the rotation angle.
     * @throws SciRendererException if the texture is not drawable.
     */
    void draw(Texture texture, AnchorPosition anchor, Vector3d position, double rotationAngle) throws SciRendererException;
}
