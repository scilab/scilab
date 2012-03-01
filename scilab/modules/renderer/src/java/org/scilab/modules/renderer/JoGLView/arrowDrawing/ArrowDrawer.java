/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - DIGITEO - Manuel JULIACHS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.renderer.JoGLView.DataManager;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;
import org.scilab.modules.renderer.JoGLView.util.ColorFactory;

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
 *
 * @author Manuel JULIACHS
 */
public class ArrowDrawer {

    /** The DrawerVisitor used. */
    private final DrawerVisitor visitor;

    /** The angle at the arrow triangle's apex. */
    private static final double APEX_ANGLE = 40.0;

    /** The cosine of the apex's half-angle. */
    private static final double COS_HALF_ANGLE = Math.cos(Math.toRadians(0.5*APEX_ANGLE));

    /** The sine of the apex's half-angle. */
    private static final double SIN_HALF_ANGLE = Math.sin(Math.toRadians(0.5*APEX_ANGLE));

    /** The reduction ratio: the size of an arrow relative to the Axes size. */
    private static final double REDUCTION_RATIO = 0.02;

    /** The reduction ratio for the Segs object: the size of an arrow relative to the Axes size. */
    private static final double SEGS_REDUCTION_RATIO = 0.075;

    /** The reduction ratio for negative arrow size values: size of an arrow head relative to the length. */
    private static final double REDUCTION_RATIO_DEPENDING = 0.15;

    /**
     * Constructor.
     * @param visitor the DrawerVisitor {@see DrawerVisitor}.
     */
    public ArrowDrawer(DrawerVisitor visitor) {
        this.visitor = visitor;
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
        axesDims[0] = axesBounds[2]*(1.0-margins[0]-margins[1]);
        axesDims[1] = axesBounds[3]*(1.0-margins[2]-margins[3]);

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
     * Fills vertex data corresponding to the arrow tips' coordinates and draws the resulting
     * tips in window coordinates.
     * @param parentAxesId the identifier of the object's parent Axes.
     * @param identifier the object's identifier.
     * @param arrowSize the arrow size.
     * @param thickness the arrow segment thickness.
     * @param isSegs specifies whether the object is a Segs object.
     * @param isColored specifies whether the object is drawn with per-arrow colors.
     * @param lineColor the line color used for all the arrows (used if isColored is equal to false).
     * @throws org.scilab.forge.scirenderer.SciRendererException if drawing fails.
     */
    public void drawArrows(String parentAxesId, String identifier, double arrowSize, double lineThickness, boolean isSegs,
        boolean isColored, int lineColor) throws SciRendererException {

        DataManager dataManager = visitor.getDataManager();
        ColorMap colorMap = visitor.getColorMap();

        ElementsBuffer vertices = dataManager.getVertexBuffer(identifier);
        IndicesBuffer segmentIndices = dataManager.getWireIndexBuffer(identifier);
        IndicesBuffer triangleIndices = dataManager.getIndexBuffer(identifier);

        int offset = vertices.getElementsSize();
        double averageNorm = 0.0;

        int numSegments = segmentIndices.getData().capacity() / 2;

        /* Do not draw if there are no segments */
        if (numSegments == 0) {
            return;
        }

        /* Get the projection */
        Transformation projection = visitor.getDrawingTools().getTransformationManager().getCanvasProjection();

        /* Compute the arrow pixel size */
        Axes parentAxes = (Axes) GraphicController.getController().getObjectFromId(parentAxesId);

        arrowSize = computeArrowPixelSize(parentAxes, vertices, segmentIndices, numSegments, arrowSize, lineThickness, isSegs);

        /* Computes and writes arrow vertex data to the object's vertex buffer */
        fillArrowVertexData(projection, vertices, segmentIndices,
            numSegments, triangleIndices, arrowSize);

        /* Forces update */
        vertices.setData(vertices.getData(), offset);

        DefaultGeometry arrowTips = new DefaultGeometry();

        arrowTips.setLineDrawingMode(Geometry.LineDrawingMode.NONE);
        arrowTips.setFillDrawingMode(Geometry.FillDrawingMode.TRIANGLES);
        arrowTips.setFaceCullingMode(Geometry.FaceCullingMode.BOTH);
        arrowTips.setVertices(dataManager.getVertexBuffer(identifier));
        arrowTips.setIndices(dataManager.getIndexBuffer(identifier));

        Appearance arrowAppearance = new Appearance();

        if (isColored) {
            arrowTips.setColors(dataManager.getColorBuffer(identifier));
        } else {
            arrowAppearance.setFillColor(ColorFactory.createColor(colorMap, lineColor));
        }

        /* Draws in window coordinates */
        visitor.getDrawingTools().getTransformationManager().useWindowCoordinate();
        visitor.getDrawingTools().draw(arrowTips, arrowAppearance);
        visitor.getDrawingTools().getTransformationManager().useSceneCoordinate();
    }

    /**
     * Computes and fills arrow vertex data for a set of line segments.
     * Arrow vertex data is computed in window coordinates and depends on the coordinates of the segment vertices.
     * To do so, the vertices of each segment are projected, and then used to compute the 3 vertices of the corresponding arrow
     * in window coordinates. These vertices are then written to the object's vertex buffer as specified by the triangle
     * index buffer. The vertex buffer therefore contains a total of (5*Nsegments) elements and must have been allocated beforehand.
     * @param projection the projection from object to window coordinates.
     * @param vertices the buffer storing the segments' vertices and to which arrow tips' vertices are written (5*Nsegments elements).
     * @param segmentIndices the segments' indices (2*Nsegments elements).
     * @param numSegments the number of segments (Nsegments).
     * @param triangleIndices the arrow tips' triangle indices (3*Nsegments elements).
     * @param arrowSize the arrow size (in pixels).
     */
    private void fillArrowVertexData(Transformation projection, ElementsBuffer vertices, IndicesBuffer segmentIndices,
        int numSegments, IndicesBuffer triangleIndices, double arrowSize) {
        int i0;
        int i1;
        int[] triIndices = new int[3];
        int offset = vertices.getElementsSize();
        IntBuffer segmentIndexData = segmentIndices.getData();
        IntBuffer triangleIndexData = triangleIndices.getData();
        FloatBuffer vertexData = vertices.getData();

        for (int i = 0; i < numSegments; i++) {
            i0 = segmentIndexData.get(2*i);
            i1 = segmentIndexData.get(2*i+1);

            /* Compute the arrow tip vertices in window coordinates from the object coordinate segment vertices */
            Vector3d v0 = new Vector3d(vertexData.get(i0*offset), vertexData.get(i0*offset+1), vertexData.get(i0*offset+2));
            Vector3d v1 = new Vector3d(vertexData.get(i1*offset), vertexData.get(i1*offset+1), vertexData.get(i1*offset+2));

            v0 = projection.project(v0);
            v1 = projection.project(v1);

            Vector3d direction = v1.minus(v0).getNormalized();

            Vector3d[] singleArrowVertices = computeArrowVertices(v1, direction, arrowSize);

            triIndices[0] = offset*triangleIndexData.get(3*i);
            triIndices[1] = offset*triangleIndexData.get(3*i+1);
            triIndices[2] = offset*triangleIndexData.get(3*i+2);

            vertexData.put(triIndices[0], (float) singleArrowVertices[0].getX());
            vertexData.put(triIndices[0]+1, (float) singleArrowVertices[0].getY());
            vertexData.put(triIndices[0]+2, (float) singleArrowVertices[0].getZ());

            vertexData.put(triIndices[1], (float) singleArrowVertices[1].getX());
            vertexData.put(triIndices[1]+1, (float) singleArrowVertices[1].getY());
            vertexData.put(triIndices[1]+2, (float) singleArrowVertices[1].getZ());

            vertexData.put(triIndices[2], (float) singleArrowVertices[2].getX());
            vertexData.put(triIndices[2]+1, (float) singleArrowVertices[2].getY());
            vertexData.put(triIndices[2]+2, (float) singleArrowVertices[2].getZ());

            if (offset == 4) {
                vertexData.put(triIndices[0]+3, 1.0f);
                vertexData.put(triIndices[1]+3, 1.0f);
                vertexData.put(triIndices[2]+3, 1.0f);
            }
        }
    }

    /**
     * Computes the average norm of a set of segments projected in window coordinates.
     * @param projection the projection from object to window coordinates.
     * @param vertexData the segments' vertex data (5*Nsegments*offset elements).
     * @param offset the number of components taken by one vertex (3 or 4).
     * @param segmentIndexData the segment index data (2*Nsegments elements).
     * @param numSegments the number of segments (Nsegments).
     * @return the average norm of the projected segments.
     */
    private double computeAverageNorm(Transformation projection, FloatBuffer vertexData, int offset, IntBuffer indexData, int numSegments) {
        double averageNorm = 0.0;

        for (int i = 0; i < numSegments; i++) {
            int i0;
            int i1;
            i0 = indexData.get(2*i);
            i1 = indexData.get(2*i+1);

            Vector3d v0 = new Vector3d(vertexData.get(i0*offset), vertexData.get(i0*offset+1), vertexData.get(i0*offset+2));
            Vector3d v1 = new Vector3d(vertexData.get(i1*offset), vertexData.get(i1*offset+1), vertexData.get(i1*offset+2));

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
}
