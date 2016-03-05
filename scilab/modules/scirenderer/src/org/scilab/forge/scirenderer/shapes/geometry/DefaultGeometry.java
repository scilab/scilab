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
 * Default implementation of a Geometry.
 *
 * @author Pierre Lando
 */
public class DefaultGeometry implements Geometry {

    private FaceCullingMode faceCullingMode = DEFAULT_FACE_CULLING_MODE;
    private FillDrawingMode fillDrawingMode = DEFAULT_FILL_DRAWING_MODE;
    private LineDrawingMode lineDrawingMode = DEFAULT_LINE_DRAWING_MODE;

    /** Specifies whether polygon offset is used or not when rendering geometry. */
    private boolean polygonOffsetMode = DEFAULT_POLYGON_OFFSET_MODE;

    private ElementsBuffer textureCoordinates;
    private ElementsBuffer vertices;
    private ElementsBuffer normals;
    private ElementsBuffer colors;

    private IndicesBuffer wireIndices;
    private IndicesBuffer indices;

    /**
     * Default constructor.
     */
    public DefaultGeometry() {
    }

    @Override
    public final FaceCullingMode getFaceCullingMode() {
        return faceCullingMode;
    }

    @Override
    public final FillDrawingMode getFillDrawingMode() {
        return fillDrawingMode;
    }

    @Override
    public final LineDrawingMode getLineDrawingMode() {
        return lineDrawingMode;
    }

    @Override
    public final boolean getPolygonOffsetMode() {
        return polygonOffsetMode;
    }

    @Override
    public final ElementsBuffer getVertices() {
        return vertices;
    }

    @Override
    public final IndicesBuffer getIndices() {
        return indices;
    }

    @Override
    public final IndicesBuffer getWireIndices() {
        return wireIndices;
    }

    @Override
    public final ElementsBuffer getColors() {
        return colors;
    }

    @Override
    public final ElementsBuffer getTextureCoordinates() {
        return textureCoordinates;
    }

    @Override
    public final ElementsBuffer getNormals() {
        return normals;
    }

    /**
     * Face culling mode setter.
     * @param faceCullingMode the new face culling mode.
     */
    public final void setFaceCullingMode(FaceCullingMode faceCullingMode) {
        this.faceCullingMode = faceCullingMode;
    }

    /**
     * Fill drawing mode setter.
     * @param fillDrawingMode the new fill drawing mode.
     */
    public final void setFillDrawingMode(FillDrawingMode fillDrawingMode) {
        this.fillDrawingMode = fillDrawingMode;
    }

    /**
     * Line drawing mode setter.
     * @param lineDrawingMode the new line drawing mode.
     */
    public final void setLineDrawingMode(LineDrawingMode lineDrawingMode) {
        this.lineDrawingMode = lineDrawingMode;
    }

    /**
     * Polygon offset mode setter.
     * @param polygonOffsetMode the new polygon offset mode.
     */
    public final void setPolygonOffsetMode(boolean polygonOffsetMode) {
        this.polygonOffsetMode = polygonOffsetMode;
    }

    /**
     * Texture coordinates setter.
     * @param textureCoordinates the new texture coordinate data.
     */
    public final void setTextureCoordinates(ElementsBuffer textureCoordinates) {
        this.textureCoordinates = textureCoordinates;
    }

    /**
     * Vertices setter.
     * @param vertices the new vertices data.
     */
    public final void setVertices(ElementsBuffer vertices) {
        this.vertices = vertices;
    }

    /**
     * Normals setter.
     * @param normals the new normals data.
     */
    public final void setNormals(ElementsBuffer normals) {
        this.normals = normals;
    }

    /**
     * Colors setter.
     * @param colors the new colors data.
     */
    public final void setColors(ElementsBuffer colors) {
        this.colors = colors;
    }

    /**
     * Wire indices setter.
     * @param wireIndices the new wire indices data.
     */
    public final void setWireIndices(IndicesBuffer wireIndices) {
        this.wireIndices = wireIndices;
    }

    /**
     * Indices setter.
     * @param indicesBuffer the new indices data.
     */
    public final void setIndices(IndicesBuffer indicesBuffer) {
        this.indices = indicesBuffer;
    }
}
