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

package org.scilab.forge.scirenderer.shapes.geometry;

import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.buffers.IndicesBuffer;

/**
 *
 * Interface for a geometry.
 *
 * @author Pierre Lando
 */
public interface Geometry {

    /**
     * This enum specify face culling.
     */
    enum FaceCullingMode {
        /**
         * Both faces are rendered (default value)
         */
        BOTH,

        /**
         * Only counter clockwise faces are visible.
         */
        CCW,

        /**
         * Only clockwise faces are visible.
         */
        CW
    }

    /**
     * Default face culling mode.
     */
    FaceCullingMode DEFAULT_FACE_CULLING_MODE = FaceCullingMode.BOTH;

    /**
     * This enum specify how geometry is rendered.
     */
    enum FillDrawingMode {
        /**
         * Treats each triplet of vertices as an independent triangle.
         */
        TRIANGLES,

        /**
         * Draws a connected group of triangles.
         */
        TRIANGLE_STRIP,

        /**
         * Draws a connected group of triangles with common first element.
         */
        TRIANGLE_FAN,

        /**
         * Nothing is filled.
         */
        NONE
    }

    /**
     * Default fill drawing mode.
     */
    FillDrawingMode DEFAULT_FILL_DRAWING_MODE = FillDrawingMode.TRIANGLES;

    /**
     * Line drawing modes declaration.
     */
    enum LineDrawingMode {
        /**
         * Each pair of vertices define an independent segment.
         */
        SEGMENTS,

        /**
         * Connected group of segments from the first vertex to the last.
         */
        SEGMENTS_STRIP,

        /**
         * Connected group of segments from the first vertex to the last, then back to the first.
         */
        SEGMENTS_LOOP,

        /**
         * No segments.
         */
        NONE
    }

    /**
     * Default wire drawing mode.
     */
    LineDrawingMode DEFAULT_LINE_DRAWING_MODE = LineDrawingMode.NONE;

    /**
     * Default polygon offset mode.
     */
    boolean DEFAULT_POLYGON_OFFSET_MODE = false;

    /**
     * Face-culling mode getter.
     * @return the face culling mode.
     */
    FaceCullingMode getFaceCullingMode();

    /**
     * Fill drawing mode getter.
     * @return the fill-drawing mode for this object.
     */
    FillDrawingMode getFillDrawingMode();

    /**
     * Line drawing mode getter.
     * @return the line drawing mode.
     */
    LineDrawingMode getLineDrawingMode();

    /**
     * Polygon offset mode getter.
     * @return the polygon offset mode.
     */
    boolean getPolygonOffsetMode();

    /**
     * Return the vertices.
     * @return the vertices.
     */
    ElementsBuffer getVertices();

    /**
     * Return the colors.
     * @return the colors.
     */
    ElementsBuffer getColors();

    /**
     * Texture coordinates getter.
     * @return the texture coordinate.
     */
    ElementsBuffer getTextureCoordinates();

    /**
     * Return the normals.
     * @return the normals.
     */
    ElementsBuffer getNormals();

    /**
     * Return the indices if any.
     * If <code>null</code>  is returned, indices should be treated as consecutive number.
     * @return the indices.
     */
    IndicesBuffer getIndices();

    /**
     * Return the wire indices.
     * If <code>null</code>  is returned, no edges are drawn.
     * @return the edges indices.
     */
    IndicesBuffer getWireIndices();
}
