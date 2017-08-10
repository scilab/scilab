/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - DIGITEO - Manuel JULIACHS
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

package org.scilab.modules.renderer.JoGLView.arrowDrawing;

import org.scilab.forge.scirenderer.SciRendererException;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.buffers.IndicesBuffer;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.geometry.DefaultGeometry;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.modules.graphic_objects.ObjectRemovedException;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.renderer.JoGLView.DataManager;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;
import org.scilab.modules.renderer.JoGLView.util.ColorFactory;
import org.scilab.modules.renderer.JoGLView.util.OutOfMemoryException;

import java.nio.FloatBuffer;
import java.nio.IntBuffer;

/**
 * ArrowDrawer class.
 * Utility class used by DrawerVisitor to draw arrows.
 * Arrows are drawn as identical isoceles triangles, whose sharpness
 * is specified by the apex angle.
 *
 * To do:
 * -optimize: segment projection occurs twice if a negative arrow size is specified.
 * -further comment (especially the reduction ratios).
 * -rationalize: use a map for arrow tip vertex buffers instead of pre-allocating
 *  data or using a single vertex buffer for small sets of segments.
 *
 * @author Manuel JULIACHS
 */
public class ArrowDrawer {

    /** The DrawerVisitor used. */
    private final DrawerVisitor visitor;

    /** The angle at the arrow triangle's apex. */
    private static final double APEX_ANGLE = 40.0;

    /** The cosine of the apex's half-angle. */
    private static final double COS_HALF_ANGLE = Math.cos(Math.toRadians(0.5 * APEX_ANGLE));

    /** The sine of the apex's half-angle. */
    private static final double SIN_HALF_ANGLE = Math.sin(Math.toRadians(0.5 * APEX_ANGLE));

    /** The reduction ratio: the size of an arrow relative to the Axes size. */
    private static final double REDUCTION_RATIO = 0.02;

    /** The reduction ratio for the Segs object: the size of an arrow relative to the Axes size. */
    private static final double SEGS_REDUCTION_RATIO = 0.075;

    /** The reduction ratio for negative arrow size values: size of an arrow head relative to the length. */
    private static final double REDUCTION_RATIO_DEPENDING = 0.15;

    /** The vertex buffer used to draw arrow vertices for non-preallocated vertex data. */
    ElementsBuffer drawerArrowVertices;

    /**
     * Constructor.
     * @param visitor the DrawerVisitor {@see DrawerVisitor}.
     */
    public ArrowDrawer(DrawerVisitor visitor) {
        this.visitor = visitor;

        drawerArrowVertices = visitor.getCanvas().getBuffersManager().createElementsBuffer();
    }

    /**
     * Computes and returns the arrow size (in pixels) for a set of segments.
     * It depends on properties of the set's parent Axes (mainly the view-related properties, like the displayed bounds).
     * @param parentAxes the parent Axes of the object corresponding to the segment set.
     * @param vertices the segments' vertices (5*Nsegments elements).
     * @param segmentIndices the segments' indices (2*Nsegments elements).
     * @param numSegments the number of segments (Nsegments).
     * @param arrowSize the input arrow size, as defined by the various arrowed segment objects.
     * @param thickness the arrow segment thickness.
     * @param isSegs specifies whether arrow size must be computed for a Segs object or not.
     * @return the arrow size in pixels.
     */
    private double computeArrowPixelSize(Axes parentAxes, ElementsBuffer vertices, IndicesBuffer segmentIndices, int numSegments, double arrowSize,
                                         double thickness, boolean isSegs) {

        double averageNorm = 0.0;
        double pixelArrowSize = 0.0;
        int canvasWidth = visitor.getDrawingTools().getCanvas().getWidth();
        int canvasHeight = visitor.getDrawingTools().getCanvas().getHeight();

        /* Get the projection */
        Transformation projection = visitor.getDrawingTools().getTransformationManager().getCanvasProjection();

        Double [] axesBounds = parentAxes.getAxesBounds();
        Double [] margins = parentAxes.getMargins();

        double[] axesDims = new double[2];

        /* To do: add a function to AxesDrawer to compute the Axes box's window coordinate dimensions */
        axesDims[0] = axesBounds[2] * (1.0 - margins[0] - margins[1]);
        axesDims[1] = axesBounds[3] * (1.0 - margins[2] - margins[3]);

        axesDims[0] *= (double) canvasWidth;
        axesDims[1] *= (double) canvasHeight;

        Double[] bounds = parentAxes.getDisplayedBounds();

        double xRange = (bounds[1] - bounds[0]);
        double yRange = (bounds[3] - bounds[2]);

        if (arrowSize < 0.0) {
            averageNorm = computeAverageNorm(projection, vertices.getData(), vertices.getElementsSize(), segmentIndices.getData(), numSegments);
        }

        pixelArrowSize = computeArrowPixelSize(averageNorm, axesDims, xRange, yRange, isSegs, thickness, arrowSize);

        return pixelArrowSize;
    }

    /**
     * Draws arrows for a given object (a set of segments).
     * Arrow tip vertex data is written into the vertex buffer where segment vertex data is located.
     * @param parentAxesId the identifier of the object's parent Axes.
     * @param identifier the object's identifier.
     * @param arrowSize the arrow size.
     * @param lineThickness the arrow segment thickness.
     * @param isSegs specifies whether the object is a Segs object.
     * @param isColored specifies whether the object is drawn with per-arrow colors.
     * @param lineColor the line color used for all the arrows (used if isColored is equal to false).
     * @throws org.scilab.forge.scirenderer.SciRendererException if drawing fails.
     * @throws ObjectRemovedException
     */
    public void drawArrows(Integer parentAxesId, Integer identifier, double arrowSize, double lineThickness, boolean isSegs,
                           boolean isColored, int lineColor, boolean isStripped) throws SciRendererException, ObjectRemovedException, OutOfMemoryException {

        DataManager dataManager = visitor.getDataManager();
        ColorMap colorMap = visitor.getColorMap();

        ElementsBuffer vertices = dataManager.getVertexBuffer(identifier);
        IndicesBuffer segmentIndices = dataManager.getWireIndexBuffer(identifier);
        IndicesBuffer triangleIndices = dataManager.getIndexBuffer(identifier);

        drawArrows(parentAxesId, vertices, dataManager.getColorBuffer(identifier), segmentIndices, triangleIndices, arrowSize, lineThickness, isSegs, isColored, lineColor, isStripped);
    }

    /**
     * Draw arrows for a set of segments.
     * It uses ArrowDrawer's own vertex buffer to write back arrow vertex data and should therefore
     * be used only for small sets of segments (typically Legend items). It does not allow per-segment colors.
     * @param parentAxesId the identifier of segment set's parent Axes.
     * @param vertices the segments' vertices.
     * @param segmentIndices the segments' indices.
     * @param arrowSize the arrow size.
     * @param lineThickness the arrow segment thickness.
     * @param lineColor the line color used for all the arrows.
     * @throws org.scilab.forge.scirenderer.SciRendererException if drawing fails.
     */
    public void drawArrows(Integer parentAxesId, ElementsBuffer vertices, IndicesBuffer segmentIndices,
                           double arrowSize, double lineThickness, int lineColor, boolean isStripped) throws SciRendererException {

        /*
         * No colors are specified as a unique color is used for all the segments.
         * As arrow tip vertices are written to the drawer's vertex buffer, triangle indices
         * are also set to null.
         */
        drawArrows(parentAxesId, vertices, null, segmentIndices, null, arrowSize, lineThickness, false, false, lineColor, isStripped);
    }

    /**
     * Draw arrows for a set of line segments.
     * It computes arrow tip vertex data in window coordinates, using the input segment vertex data.
     * Depending on whether a triangle index buffer is available, the computed data is written into
     * the segment vertex buffer or the drawer's own vertex buffer.
     * @param parentAxesId the identifier of the Axes in which arrows are drawn.
     * @param vertices the segments' vertices.
     * @param colors the segments' colors.
     * @param segmentIndices the segments' indices.
     * @param triangleIndices the arrow tips' triangle indices.
     * @param arrowSize the arrow size.
     * @param lineThickness the arrow segment thickness.
     * @param isSegs specifies whether arrows are drawn for a Segs object.
     * @param isColored specifies whether per-arrow colors are used.
     * @param lineColor the line color used for all the arrows (used if isColored is equal to false).
     * @throws org.scilab.forge.scirenderer.SciRendererException if drawing fails.
     */
    private void drawArrows(Integer parentAxesId, ElementsBuffer vertices, ElementsBuffer colors, IndicesBuffer segmentIndices, IndicesBuffer triangleIndices,
                            double arrowSize, double lineThickness, boolean isSegs, boolean isColored, int lineColor, boolean isStripped) throws SciRendererException {

        int offset = vertices.getElementsSize();

        int numSegments;
        if (isStripped) {
            numSegments = segmentIndices.getData().capacity() - 1;
        } else {
            numSegments = segmentIndices.getData().capacity() / 2;
        }

        /* Do not draw if there are no segments */
        if (numSegments == 0) {
            return;
        }

        /* Get the projection */
        Transformation projection = visitor.getDrawingTools().getTransformationManager().getCanvasProjection();

        /* Compute the arrow pixel size */
        Axes parentAxes = (Axes) GraphicController.getController().getObjectFromId(parentAxesId);

        arrowSize = computeArrowPixelSize(parentAxes, vertices, segmentIndices, numSegments, arrowSize, lineThickness, isSegs);

        /*
         * If a triangle index buffer is available, arrow tip vertices are written
         * into the same buffer as segment vertices, else, they are written into
         * the drawer's arrow tip buffer.
         */
        if (triangleIndices != null) {
            fillArrowVertexData(projection, vertices, segmentIndices,
                                numSegments, triangleIndices, arrowSize, isStripped);

            /* Forces update */
            vertices.setData(vertices.getData(), offset);
        } else {
            fillArrowVertexData(projection, vertices, segmentIndices,
                                numSegments, drawerArrowVertices, arrowSize, isStripped);
        }

        DefaultGeometry arrowTips = new DefaultGeometry();

        arrowTips.setLineDrawingMode(Geometry.LineDrawingMode.NONE);
        arrowTips.setFillDrawingMode(Geometry.FillDrawingMode.TRIANGLES);
        arrowTips.setFaceCullingMode(Geometry.FaceCullingMode.BOTH);

        if (triangleIndices != null) {
            arrowTips.setVertices(vertices);
            arrowTips.setIndices(triangleIndices);
        } else {
            /* No triangle indices are set as the drawer's vertex buffer contains only tip vertices. */
            arrowTips.setVertices(drawerArrowVertices);
        }

        Appearance arrowAppearance = new Appearance();

        if (isColored) {
            arrowTips.setColors(colors);
        } else {
            arrowAppearance.setFillColor(ColorFactory.createColor(visitor.getColorMap(), lineColor));
        }

        /* Draws in window coordinates */
        visitor.getDrawingTools().getTransformationManager().useWindowCoordinate();
        visitor.getDrawingTools().draw(arrowTips, arrowAppearance);
        visitor.getDrawingTools().getTransformationManager().useSceneCoordinate();
    }

    /**
     * Computes and fills arrow vertex data for a set of line segments.
     * Arrow tip vertices are computed in window coordinates and depends on the coordinates of the segment vertices.
     * These vertices are written to the segments' vertex buffer as specified by the triangle
     * index buffer. The vertex buffer therefore contains a total of (5*Nsegments) elements and must have been allocated beforehand.
     * @param projection the projection from object to window coordinates.
     * @param vertices the buffer storing the segments' vertices and to which arrow tips' vertices are written (5*Nsegments elements).
     * @param segmentIndices the segments' indices (2*Nsegments elements).
     * @param numSegments the number of segments (Nsegments).
     * @param triangleIndices the arrow tips' triangle indices (3*Nsegments elements).
     * @param arrowSize the arrow size (in pixels).
     */
    private void fillArrowVertexData(Transformation projection, ElementsBuffer vertices, IndicesBuffer segmentIndices,
                                     int numSegments, IndicesBuffer triangleIndices, double arrowSize, final boolean isStripped) {
        int[] segmentVertexOffsets = new int[2];
        int[] triVertexOffsets  = new int[3];
        int offset = vertices.getElementsSize();
        IntBuffer segmentIndexData = segmentIndices.getData();
        IntBuffer triangleIndexData = triangleIndices.getData();
        FloatBuffer vertexData = vertices.getData();

        for (int i = 0; i < numSegments; i++) {
            if (isStripped) {
                segmentVertexOffsets[0] = offset * segmentIndexData.get(i);
                segmentVertexOffsets[1] = offset * segmentIndexData.get(i + 1);
            } else {
                segmentVertexOffsets[0] = offset * segmentIndexData.get(2 * i);
                segmentVertexOffsets[1] = offset * segmentIndexData.get(2 * i + 1);
            }

            triVertexOffsets[0] = offset * triangleIndexData.get(3 * i);
            triVertexOffsets[1] = offset * triangleIndexData.get(3 * i + 1);
            triVertexOffsets[2] = offset * triangleIndexData.get(3 * i + 2);

            /* Vertex data is specified as both the input and output vertex buffer as it contains both the segment and arrow tip vertices. */
            computeAndWriteSingleArrowVertexData(projection, vertexData, vertexData, segmentVertexOffsets, triVertexOffsets, offset, arrowSize);
        }
    }

    /**
     * Computes and fills arrow vertex data for a set of line segments.
     * Arrow tip vertices are written into a separate vertex buffer, which is resized as needed.
     * @param projection the projection from object to window coordinates.
     * @param vertices the line segments' vertices (2*Nsegments elements).
     * @param segmentIndices the line segments' indices (2*Nsegments elements).
     * @param numSegments the number of segments (Nsegments).
     * @param arrowVertices the arrow tip vertices (3*Nsegment elements).
     * @param arrowSize the arrow size (in pixels).
     */
    private void fillArrowVertexData(Transformation projection, ElementsBuffer vertices, IndicesBuffer segmentIndices,
                                     int numSegments, ElementsBuffer arrowVertices, double arrowSize, final boolean isStripped) {
        int[] segmentVertexOffsets = new int[2];
        int[] triVertexOffsets = new int[3];
        int offset = vertices.getElementsSize();
        int arrowOffset;
        IntBuffer segmentIndexData = segmentIndices.getData();
        FloatBuffer vertexData = vertices.getData();
        FloatBuffer arrowVertexData;

        int bufferOffset = 0;

        /* Check whether resizing is required and accordingly get vertex data */
        if (isResizeRequired(arrowVertices, numSegments)) {
            arrowOffset = 4;
            arrowVertexData = FloatBuffer.allocate(arrowOffset * 3 * numSegments);
        } else {
            arrowOffset = arrowVertices.getElementsSize();
            arrowVertexData = arrowVertices.getData();
        }

        for (int i = 0; i < numSegments; i++) {
            if (isStripped) {
                segmentVertexOffsets[0] = offset * segmentIndexData.get(i);
                segmentVertexOffsets[1] = offset * segmentIndexData.get(i + 1);
            } else {
                segmentVertexOffsets[0] = offset * segmentIndexData.get(2 * i);
                segmentVertexOffsets[1] = offset * segmentIndexData.get(2 * i + 1);
            }

            triVertexOffsets[0] = bufferOffset;
            triVertexOffsets[1] = bufferOffset + arrowOffset;
            triVertexOffsets[2] = bufferOffset + 2 * arrowOffset;

            computeAndWriteSingleArrowVertexData(projection, vertexData, arrowVertexData, segmentVertexOffsets, triVertexOffsets, arrowOffset, arrowSize);

            bufferOffset += 3 * arrowOffset;
        }

        arrowVertices.setData(arrowVertexData, arrowOffset);
    }

    /**
     * Determines whether the arrow vertex buffer passed as an argument must be resized or not
     * depending on the number of segments for which arrow tip data must be computed.
     * @param arrowVertices the arrow vertex buffer.
     * @param numSegments the number of segments required.
     * @return true if the buffer must be resized, false if not.
     */
    private boolean isResizeRequired(ElementsBuffer arrowVertices, int numSegments) {
        boolean resize = false;

        if (arrowVertices.getData() == null) {
            resize = true;
        } else {
            int numPreviousVertices = arrowVertices.getData().capacity() / arrowVertices.getElementsSize();

            if (3 * numSegments != numPreviousVertices) {
                resize = true;
            }
        }

        return resize;
    }

    /**
     * Computes and returns the 3 vertices of an arrow tip corresponding to a single segment.
     * Arrow vertex data is computed in window coordinates and depends on the coordinates of the segment vertices.
     * To do so, the vertices of the segment are projected, and then used to compute the 3 vertices of the corresponding arrow.
     * @param projection the projection from object to window coordinates.
     * @param vertexData the segments' vertex data.
     * @param arrowVertexData the arrow tip vertex data in which the computed vertices are output.
     * @param segmentVertexOffsets the offsets of the segment's endpoints into the vertex buffer (2 elements).
     * @param arrowVertexOffsets the offsets of the tip's vertices into the arrow vertex buffer (3 elements).
     * @param offset the number of components taken by one arrow vertex (3 or 4).
     * @param arrowSize the arrow size in pixels.
     */
    private void computeAndWriteSingleArrowVertexData(Transformation projection, FloatBuffer vertexData, FloatBuffer arrowVertexData,
            int[] segmentVertexOffsets, int[] arrowVertexOffsets, int offset, double arrowSize) {

        /* Compute the arrow tip vertices in window coordinates from the object coordinate segment vertices */
        Vector3d v0 = new Vector3d(vertexData.get(segmentVertexOffsets[0]), vertexData.get(segmentVertexOffsets[0] + 1), vertexData.get(segmentVertexOffsets[0] + 2));
        Vector3d v1 = new Vector3d(vertexData.get(segmentVertexOffsets[1]), vertexData.get(segmentVertexOffsets[1] + 1), vertexData.get(segmentVertexOffsets[1] + 2));

        v0 = projection.project(v0);
        v1 = projection.project(v1);

        Vector3d direction = v1.minus(v0).getNormalized();

        Vector3d[] singleArrowVertices = computeArrowVertices(v1, direction, arrowSize);

        arrowVertexData.put(arrowVertexOffsets[0], (float) singleArrowVertices[0].getX());
        arrowVertexData.put(arrowVertexOffsets[0] + 1, (float) singleArrowVertices[0].getY());
        arrowVertexData.put(arrowVertexOffsets[0] + 2, (float) singleArrowVertices[0].getZ());

        arrowVertexData.put(arrowVertexOffsets[1], (float) singleArrowVertices[1].getX());
        arrowVertexData.put(arrowVertexOffsets[1] + 1, (float) singleArrowVertices[1].getY());
        arrowVertexData.put(arrowVertexOffsets[1] + 2, (float) singleArrowVertices[1].getZ());

        arrowVertexData.put(arrowVertexOffsets[2], (float) singleArrowVertices[2].getX());
        arrowVertexData.put(arrowVertexOffsets[2] + 1, (float) singleArrowVertices[2].getY());
        arrowVertexData.put(arrowVertexOffsets[2] + 2, (float) singleArrowVertices[2].getZ());

        if (offset == 4) {
            arrowVertexData.put(arrowVertexOffsets[0] + 3, 1.0f);
            arrowVertexData.put(arrowVertexOffsets[1] + 3, 1.0f);
            arrowVertexData.put(arrowVertexOffsets[2] + 3, 1.0f);
        }
    }

    /**
     * Computes the average norm of a set of segments projected in window coordinates.
     * @param projection the projection from object to window coordinates.
     * @param vertexData the segments' vertex data (at least 2*Nsegments*offset elements).
     * @param offset the number of components taken by one vertex (3 or 4).
     * @param indexData the segments' index data (2*Nsegments elements).
     * @param numSegments the number of segments (Nsegments).
     * @return the average norm of the projected segments.
     */
    private double computeAverageNorm(Transformation projection, FloatBuffer vertexData, int offset, IntBuffer indexData, int numSegments) {
        double averageNorm = 0.0;

        for (int i = 0; i < numSegments; i++) {
            int i0;
            int i1;
            i0 = indexData.get(2 * i);
            i1 = indexData.get(2 * i + 1);

            Vector3d v0 = new Vector3d(vertexData.get(i0 * offset), vertexData.get(i0 * offset + 1), vertexData.get(i0 * offset + 2));
            Vector3d v1 = new Vector3d(vertexData.get(i1 * offset), vertexData.get(i1 * offset + 1), vertexData.get(i1 * offset + 2));

            v0 = projection.project(v0);
            v1 = projection.project(v1);

            double norm = v1.minus(v0).getNorm();

            averageNorm += norm;
        }

        if (numSegments > 0) {
            averageNorm /= (double) numSegments;
        }

        return averageNorm;
    }

    /**
     * Computes and returns the arrow size in pixels.
     * It is computed depending on the Axes box's dimensions and the displayed data bounds in order to obtain
     * a constant size in pixels, or else a size varying with the zoom level.
     * @param averageNorm the average norm of a segment in window coordinates (used only if the input arrow size is < 0).
     * @param axesDims the dimensions of the Axes box zone in pixels (2 elements: x and y).
     * @param xRange the displayed data bounds's x interval.
     * @param xRange the displayed data bounds's y interval.
     * @param isSegs specifies whether arrow size must be computed for a Segs object.
     * @param thickness the arrow segment thickness.
     * @param arrowSize the input arrow size, as defined by the various arrow segment objects.
     * @return the arrow size in pixels.
     */
    private double computeArrowPixelSize(double averageNorm, double[] axesDims, double xRange, double yRange, boolean isSegs,
                                         double thickness, double arrowSize) {

        if (arrowSize < 0.0) {
            /* If the size is negative, the arrow size depends on the average length of the segments */
            arrowSize *= thickness;
            arrowSize = -arrowSize * averageNorm * REDUCTION_RATIO_DEPENDING;
        } else if (isSegs) {
            /*
             * Special case for Segs: the arrow size is adjusted depending on the Axes' dimensions
             * (in window coordinates) and the displayed bounds' maximum.
             */
            double maxRange = Math.max(xRange, yRange);

            if (maxRange == 0.0) {
                maxRange = 1.0;
            }

            arrowSize *= thickness;
            arrowSize = arrowSize * Math.min(axesDims[0], axesDims[1]) * SEGS_REDUCTION_RATIO / maxRange;
        } else {
            /* Champ objects: the arrow size remains constant in window coordinates */
            arrowSize *= thickness;
            arrowSize =  arrowSize * Math.min(axesDims[0], axesDims[1]) * REDUCTION_RATIO;
        }

        return arrowSize;
    }

    /**
     * Computes and returns the 3 vertices of an arrow tip.
     * The vertices are given starting from the apex and going in counter-clockwise order.
     * @param tipVertex the arrow tip's apex vertex.
     * @param direction the normalized direction vector from the tip to the arrow base.
     * @param arrowSize the arrow size in pixels.
     * @return the 3 vertices of an arrow tip.
     */
    private Vector3d[] computeArrowVertices(Vector3d tipVertex, Vector3d direction, double arrowSize) {
        Vector3d[] arrowVertices = new Vector3d[3];

        Vector3d orthDirection = new Vector3d(-direction.getY(), direction.getX(), direction.getZ());

        double height = arrowSize * COS_HALF_ANGLE;
        double halfBase = arrowSize * SIN_HALF_ANGLE;

        Vector3d baseProjection = tipVertex.minus(direction.times(height));

        arrowVertices[0] = tipVertex;
        arrowVertices[1] = baseProjection.minus(orthDirection.times(halfBase));
        arrowVertices[2] = baseProjection.plus(orthDirection.times(halfBase));

        return arrowVertices;
    }

    /**
     * Disposes all the ArrowDrawer resources.
     */
    public void disposeAll() {
        visitor.getCanvas().getBuffersManager().dispose(drawerArrowVertices);
    }

}
