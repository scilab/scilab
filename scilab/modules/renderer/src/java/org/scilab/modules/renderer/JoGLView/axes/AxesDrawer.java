/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView.axes;

import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.SciRendererException;
import org.scilab.forge.scirenderer.clipping.ClippingPlane;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry.FaceCullingMode;
import org.scilab.forge.scirenderer.tranformations.DegenerateMatrixException;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.TransformationFactory;
import org.scilab.forge.scirenderer.tranformations.TransformationStack;
import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.forge.scirenderer.tranformations.Vector4d;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.axes.Box;
import org.scilab.modules.graphic_objects.contouredObject.Line;
import org.scilab.modules.graphic_objects.graphicObject.ClippableProperty;
import org.scilab.modules.graphic_objects.graphicObject.ClippableProperty.ClipStateType;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;
import org.scilab.modules.renderer.JoGLView.axes.ruler.AxesRulerDrawer;
import org.scilab.modules.renderer.JoGLView.label.AxisLabelPositioner;
import org.scilab.modules.renderer.JoGLView.label.LabelManager;
import org.scilab.modules.renderer.JoGLView.label.LabelPositioner;
import org.scilab.modules.renderer.JoGLView.label.TitlePositioner;
import org.scilab.modules.renderer.JoGLView.label.YAxisLabelPositioner;
import org.scilab.modules.renderer.JoGLView.util.ColorFactory;

import java.awt.geom.Rectangle2D;
import java.util.HashMap;
import java.util.Map;

/**
 *
 * AxesDrawer are used by {@see DrawerVisitor} to draw {@see Axes}.
 *
 * @author Pierre Lando
 */
public class AxesDrawer {
    private static final double DEFAULT_THETA = 270.0;
    private static final Line.LineType HIDDEN_BORDER_PATTERN = Line.LineType.DASH;

    /** An epsilon value used to move the clipping planes in order to prevent strict clipping. */
    private static final double CLIPPING_EPSILON = 1e-5;

    private final DrawerVisitor visitor;
    private final Geometries geometries;

    private final AxesRulerDrawer rulerDrawer;

    /** The front face culling mode. */
    private FaceCullingMode frontFaceCullingMode;

    /** The back face culling mode. */
    private FaceCullingMode backFaceCullingMode;

    /** The label manager. */
    private final LabelManager labelManager;

    /** The x-axis label positioner. */
    private AxisLabelPositioner xAxisLabelPositioner;

    /** The y-axis label positioner. */
    private AxisLabelPositioner yAxisLabelPositioner;

    /** The z-axis label positioner. */
    private AxisLabelPositioner zAxisLabelPositioner;

    /** The title positioner. */
    private TitlePositioner titlePositioner;

    /**
     * The current reversed bounds. Used by the functions converting
     * between object and box coordinates.
     */
    private double[] reversedBounds;

    /** The current reversed bounds intervals. */
    private double[] reversedBoundsIntervals;

    /** The current projection (from object to window coordinates) used when drawing objects. */
    private Transformation currentProjection;

    /**  The current data scale and translate transformation. */
    private Transformation currentDataTransformation;

    /** The set of object to window coordinate projections associated to all the Axes drawn by this drawer. */
    private final Map<String, Transformation> projectionMap = new HashMap<String, Transformation>();

    /** The set of object (in 2d view mode) to window coordinate projections associated to all the Axes drawn by this drawer. */
    private final Map<String, Transformation> projection2dViewMap = new HashMap<String, Transformation>();

    /** This is a __MAP__ */
    private final Map<String, Transformation> sceneProjectionMap = new HashMap<String, Transformation>();

    /**
     * Default constructor.
     * @param visitor the parent {@see DrawerVisitor}.
     */
    public AxesDrawer(DrawerVisitor visitor) {
        this.visitor = visitor;
        this.labelManager = visitor.getLabelManager();
        this.geometries = new Geometries(visitor.getCanvas());
        this.rulerDrawer = new AxesRulerDrawer(visitor.getCanvas());

        this.xAxisLabelPositioner = new AxisLabelPositioner();
        this.yAxisLabelPositioner = new YAxisLabelPositioner();
        this.zAxisLabelPositioner = new AxisLabelPositioner();
        this.titlePositioner = new TitlePositioner();

        reversedBounds = new double[6];
        reversedBoundsIntervals = new double[3];
    }

    /**
     * Draw the given {@see Axes}.
     * @param axes {@see Axes} to draw.
     * @throws org.scilab.forge.scirenderer.SciRendererException if the draw fail.
     */
    public void draw(Axes axes) throws SciRendererException {
        DrawingTools drawingTools = visitor.getDrawingTools();
        Canvas canvas = visitor.getCanvas();
        ColorMap colorMap = visitor.getColorMap();
        TransformationStack modelViewStack = drawingTools.getTransformationManager().getModelViewStack();
        TransformationStack projectionStack = drawingTools.getTransformationManager().getProjectionStack();

        // Axes are drawn on top of everything.
        drawingTools.clearDepthBuffer();

        // Set axes zone.
        Transformation zoneProjection = computeZoneProjection(axes);
        projectionStack.push(zoneProjection);

        // Set box projection.
        Transformation transformation = computeBoxTransformation(axes, canvas, false);
        Transformation transformation2dView = computeBoxTransformation(axes, canvas, true);
        modelViewStack.pushRightMultiply(transformation);

        /* Compute the data scale and translate transformation. */
        Transformation dataTransformation = computeDataTransformation(axes);

        currentDataTransformation = dataTransformation;

        /* Compute the object to window coordinates projection. */
        currentProjection = zoneProjection.rightTimes(transformation);
        currentProjection = currentProjection.rightTimes(dataTransformation);

        sceneProjectionMap.put(axes.getIdentifier(), currentProjection);
        Transformation windowTrans = drawingTools.getTransformationManager().getWindowTransformation().getInverseTransformation();
        currentProjection = windowTrans.rightTimes(currentProjection);

        /* Update the projection maps with the resulting projections. */
        addProjection(axes.getIdentifier(), currentProjection);

        /* 2d view projection, to do: optimize computation */
        currentProjection = zoneProjection.rightTimes(transformation2dView);
        currentProjection = currentProjection.rightTimes(dataTransformation);
        currentProjection = windowTrans.rightTimes(currentProjection);

        addProjection2dView(axes.getIdentifier(), currentProjection);

        /**
         * Draw the axes background.
         */
        drawBackground(axes, drawingTools, colorMap);

        /**
         * Mirror the transformation such that the corner with the maximum Z value was (-1, -1, -1).
         * And draw the box.
         */
        Transformation cubeOrientation = computeCubeMirroring(transformation);
        modelViewStack.pushRightMultiply(cubeOrientation);
        drawBox(axes, drawingTools, colorMap);
        modelViewStack.pop();

        // Ruler are drawn in box coordinate.
        rulerDrawer.drawRuler(axes, this, colorMap, drawingTools);

        /* Compute reversed bounds. */
        computeReversedBounds(axes);

        /**
         * Scale and translate for data fitting.
         * And draw data.
         */
        modelViewStack.pushRightMultiply(dataTransformation);

        /* Compute the front and back culling modes */
        computeFaceCullingModes(axes);

        visitor.askAcceptVisitor(axes.getChildren());
        modelViewStack.pop();

        // Reset transformation stacks.
        modelViewStack.pop();
        projectionStack.pop();
    }

    /**
     * Draw the axes background.
     * @param axes the {@see Axes}
     * @param drawingTools the {@see DrawingTools} to use.
     * @param colorMap the current {@see ColorMap}
     * @throws org.scilab.forge.scirenderer.SciRendererException if the draw fail.
     */
    private void drawBackground(Axes axes, DrawingTools drawingTools, ColorMap colorMap) throws SciRendererException {
        if (axes.getFilled()) {
            Appearance appearance = new Appearance();
            appearance.setFillColor(ColorFactory.createColor(colorMap, axes.getBackground()));
            drawingTools.draw(geometries.getCubeGeometry(), appearance);
            drawingTools.clearDepthBuffer();
        }
    }

    /**
     * Draw the box of the given {@see Axes}
     * @param axes the given {@see Axes}.
     * @param drawingTools the {@see DrawingTools} to use.
     * @param colorMap the current {@see ColorMap}.
     * @throws org.scilab.forge.scirenderer.SciRendererException if the draw fail.
     */
    private void drawBox(Axes axes, DrawingTools drawingTools, ColorMap colorMap) throws SciRendererException {
        Box.BoxType boxed = axes.getBox().getBox();
        if (boxed != Box.BoxType.OFF) {
            Appearance appearance = new Appearance();

            /**
             * Draw hidden part of box.
             */
            appearance.setLineColor(ColorFactory.createColor(colorMap, axes.getHiddenAxisColor()));
            appearance.setLineWidth(axes.getLineThickness().floatValue());
            appearance.setLinePattern(HIDDEN_BORDER_PATTERN.asPattern());
            drawingTools.draw(geometries.getHiddenBoxBorderGeometry(), appearance);


            if (boxed != Box.BoxType.HIDDEN_AXES) {

                /**
                 * Draw box border.
                 */
                appearance.setLineColor(ColorFactory.createColor(colorMap, axes.getLineColor()));
                appearance.setLineWidth(axes.getLineThickness().floatValue());
                appearance.setLinePattern(axes.getLine().getLineStyle().asPattern());
                drawingTools.draw(geometries.getBoxBorderGeometry(), appearance);


                if (boxed != Box.BoxType.BACK_HALF) {
                    /**
                     * Draw front part of box.
                     */
                    drawingTools.draw(geometries.getFrontBoxBorderGeometry(), appearance);
                }
            }
        }
    }


    /**
     * Compute the mirroring matrix needed to have the [-1; -1; -1] point projected with the maximal Z value.
     * @param transformation the current transformation.
     * @return the mirroring matrix needed to have the [-1; -1; -1] point projected with the maximal Z value.
     */
    private Transformation computeCubeMirroring(Transformation transformation) {
        double[] matrix = transformation.getMatrix();
        try {
            return TransformationFactory.getScaleTransformation(
                       matrix[2] < 0 ? 1 : -1,
                       matrix[6] < 0 ? 1 : -1,
                       matrix[10] < 0 ? 1 : -1
                   );
        } catch (DegenerateMatrixException e) {
            // Should never happen.
            return TransformationFactory.getIdentity();
        }
    }


    /**
     * Compute zone where the axes is draw. In normalised window coordinate.
     * @param axes the given {@see axes}.
     * @return the zone where the axes is draw.
     */
    private Rectangle2D computeZone(Axes axes) {
        Double[] axesBounds = axes.getAxesBounds();
        Double[] margins = axes.getMargins();

        // TODO :  zoom box.
        double x = (axesBounds[0] + axesBounds[2] * margins[0]) * 2 - 1;  
        double y = (1.0 - axesBounds[1] - axesBounds[3] * (1.0 - margins[3])) * 2 - 1;
        double w = (1 - margins[0] - margins[1]) * axesBounds[2];
        double h = (1 - margins[2] - margins[3]) * axesBounds[3];

        if (axes.getIsoview()) {
            double minSize = Math.min(w, h);
            y += (h - minSize);
            h = minSize;
            x += (w - minSize);
            w = minSize;
        }

        return new Rectangle2D.Double(x, y, w, h);
    }


    /**
     * Compute the projection for the given axes.
     * @param axes the given axes.
     * @return the projection matrix.
     * @throws DegenerateMatrixException if axes represent a nul area.
     */
    private Transformation computeZoneProjection(Axes axes) throws DegenerateMatrixException {
        Rectangle2D zone = computeZone(axes);
        Transformation zoneTranslation = TransformationFactory.getTranslateTransformation(zone.getMaxX(), zone.getMaxY(), 0);

        // We scale by 0.5 in Z to allow ruler to be drawn.
        Transformation zoneScale = TransformationFactory.getScaleTransformation(zone.getWidth(), zone.getHeight(), .5);

        return zoneTranslation.rightTimes(zoneScale);
    }

    /**
     * Compute data transformation for the given {@see Axes}.
     *
     * The data transformation is applied to data to fit in axes box.
     *
     * @param axes the given {@see Axes}
     * @return data transformation.
     * @throws DegenerateMatrixException if data bounds are not corrects.
     */
    private Transformation computeDataTransformation(Axes axes) throws DegenerateMatrixException {
        Double[] bounds = axes.getDisplayedBounds();

        // Reverse data if needed.
        Transformation transformation = TransformationFactory.getScaleTransformation(
                                            axes.getAxes()[0].getReverse() ? 1 : -1,
                                            axes.getAxes()[1].getReverse() ? 1 : -1,
                                            axes.getAxes()[2].getReverse() ? 1 : -1
                                        );

        // Scale data.
        Transformation scaleTransformation = TransformationFactory.getScaleTransformation(
                2.0 / (bounds[1] - bounds[0]),
                2.0 / (bounds[3] - bounds[2]),
                2.0 / (bounds[5] - bounds[4])
                                             );
        transformation = transformation.rightTimes(scaleTransformation);


        // Translate data.
        Transformation translateTransformation = TransformationFactory.getTranslateTransformation(
                    -(bounds[0] + bounds[1]) / 2.0,
                    -(bounds[2] + bounds[3]) / 2.0,
                    -(bounds[4] + bounds[5]) / 2.0
                );
        transformation = transformation.rightTimes(translateTransformation);
        return transformation;
    }


    /**
     * Compute box transformation for the given axes.
     *
     * The box transformation is applied to the axes box to fit in the canvas.
     *
     * @param axes the given {@see Axes}.
     * @param canvas the current {@see Canvas}.
     * @param use2dView specifies whether the default 2d view rotation angles must be used (true) or the given Axes' ones (false).
     * @return box transformation for the given axes.
     * @throws DegenerateMatrixException if data bounds are incorrect or canvas with or length are zero.
     */
    private Transformation computeBoxTransformation(Axes axes, Canvas canvas, boolean use2dView) throws DegenerateMatrixException {
        Double[] bounds = axes.getDisplayedBounds();


        double alpha;
        double theta;

        double tmpX;
        double tmpY;
        double tmpZ;

        Transformation transformation;

        // Set zone aspect ratio.
        Rectangle2D zone = computeZone(axes);
        double axesRatio = zone.getWidth() / zone.getHeight();
        transformation = TransformationFactory.getPreferredAspectRatioTransformation(canvas.getDimension(), axesRatio);

        // Rotate.
        if (use2dView) {
            alpha = 0.0;
            theta = 2 * DEFAULT_THETA;
        } else {
            alpha = -axes.getRotationAngles()[0];
            theta = DEFAULT_THETA + axes.getRotationAngles()[1];
        }

        Transformation alphaRotation = TransformationFactory.getRotationTransformation(alpha, 1.0, 0.0, 0.0);
        transformation = transformation.rightTimes(alphaRotation);
        Transformation thetaRotation = TransformationFactory.getRotationTransformation(theta, 0.0, 0.0, 1.0);
        transformation = transformation.rightTimes(thetaRotation);

        // If there is no cube scaling, we must take into account the distribution of data.
        if (!axes.getCubeScaling()) {
            tmpX = (bounds[1] - bounds[0]);
            tmpY = (bounds[3] - bounds[2]);
            tmpZ = (bounds[5] - bounds[4]);

            /**
             * Here, we should divide the values by their maximum.
             * But the next operation will automatically.
             */
            Transformation cubeScale = TransformationFactory.getScaleTransformation(tmpX, tmpY, tmpZ);
            transformation = transformation.rightTimes(cubeScale);
        }

        // Compute bounds of projected data.
        double[] matrix = transformation.getMatrix();
        tmpX = 1 / (Math.abs(matrix[0]) + Math.abs(matrix[4]) + Math.abs(matrix[8]));
        tmpY = 1 / (Math.abs(matrix[1]) + Math.abs(matrix[5]) + Math.abs(matrix[9]));
        tmpZ = 1 / (Math.abs(matrix[2]) + Math.abs(matrix[6]) + Math.abs(matrix[10]));

        // Scale projected data to fit in the cube.
        Transformation isoScale;
        if (axes.getIsoview()) {
            Double[] axesBounds = axes.getAxesBounds();
            double minScale = Math.min(tmpX * axesBounds[2], tmpY * axesBounds[3]);
            isoScale = TransformationFactory.getScaleTransformation(minScale / axesBounds[2], minScale / axesBounds[3], tmpZ);
        } else {
            isoScale = TransformationFactory.getScaleTransformation(tmpX, tmpY, tmpZ);
        }
        transformation = transformation.leftTimes(isoScale);

        return transformation;
    }


    /**
     * Computes and sets the reversed bounds of the given Axes.
     * @param axes the given {@see Axes}.
     */
    private void computeReversedBounds(Axes axes) {
        Double[] currentBounds = axes.getDisplayedBounds();

        /* Reverse */
        if (axes.getAxes()[0].getReverse()) {
            reversedBounds[0] = currentBounds[1];
            reversedBounds[1] = currentBounds[0];
        } else {
            reversedBounds[0] = currentBounds[0];
            reversedBounds[1] = currentBounds[1];
        }

        if (axes.getAxes()[1].getReverse()) {
            reversedBounds[2] = currentBounds[3];
            reversedBounds[3] = currentBounds[2];
        } else {
            reversedBounds[2] = currentBounds[2];
            reversedBounds[3] = currentBounds[3];
        }

        if (axes.getAxes()[2].getReverse()) {
            reversedBounds[4] = currentBounds[5];
            reversedBounds[5] = currentBounds[4];
        } else {
            reversedBounds[4] = currentBounds[4];
            reversedBounds[5] = currentBounds[5];
        }

        /*
         * Interval values are set to 1 when bounds are equal to avoid divides by 0
         * in the object to box coordinates conversion function.
         */
        if (reversedBounds[1] == reversedBounds[0]) {
            reversedBoundsIntervals[0] = 1.0;
        } else {
            reversedBoundsIntervals[0] = reversedBounds[1] - reversedBounds[0];
        }

        if (reversedBounds[3] == reversedBounds[2]) {
            reversedBoundsIntervals[1] = 1.0;
        } else {
            reversedBoundsIntervals[1] = reversedBounds[3] - reversedBounds[2];
        }

        if (reversedBounds[5] == reversedBounds[4]) {
            reversedBoundsIntervals[2] = 1.0;
        } else {
            reversedBoundsIntervals[2] = reversedBounds[5] - reversedBounds[4];
        }

    }

    /**
     * Computes the culling modes respectively corresponding to front and back faces
     * of the given Axes' child objects as a function of its {X,Y,Z} reverse properties.
     * It must be called by draw prior to rendering any child object.
     * @param axes the given {@see Axes}.
     */
    private void computeFaceCullingModes(Axes axes) {
        if (axes.getAxes()[0].getReverse() ^ axes.getAxes()[1].getReverse() ^ axes.getAxes()[2].getReverse()) {
            /* Front: CW */
            this.frontFaceCullingMode = FaceCullingMode.CW;
            this.backFaceCullingMode = FaceCullingMode.CCW;
        } else {
            /* Front: CCW */
            this.frontFaceCullingMode = FaceCullingMode.CCW;
            this.backFaceCullingMode = FaceCullingMode.CW;
        }
    }

    /**
     * Converts a point from object coordinates to box coordinates (used when drawing axis rulers).
     * @param point the point in object coordinates.
     * @return the point in box coordinates.
     */
    public Vector3d getBoxCoordinates(Vector3d point) {
        double[] dataCoordinates = new double[3];

        dataCoordinates[0] = 1 - 2.0 * (point.getX() - reversedBounds[0]) / reversedBoundsIntervals[0];
        dataCoordinates[1] = 1 - 2.0 * (point.getY() - reversedBounds[2]) / reversedBoundsIntervals[1];
        dataCoordinates[2] = 1 - 2.0 * (point.getZ() - reversedBounds[4]) / reversedBoundsIntervals[2];

        return new Vector3d(dataCoordinates);
    }

    /**
     * Converts a point from box coordinates (used when drawing axis rulers) to object coordinates.
     * @param point the point in box coordinates.
     * @return the point in object coordinates.
     */
    public Vector3d getObjectCoordinates(Vector3d point) {
        double[] objectCoordinates = new double[3];

        objectCoordinates[0] = 0.5 * (1.0 - point.getX()) * (reversedBounds[1] - reversedBounds[0]) + reversedBounds[0];
        objectCoordinates[1] = 0.5 * (1.0 - point.getY()) * (reversedBounds[3] - reversedBounds[2]) + reversedBounds[2];
        objectCoordinates[2] = 0.5 * (1.0 - point.getZ()) * (reversedBounds[5] - reversedBounds[4]) + reversedBounds[4];

        return new Vector3d(objectCoordinates);
    }

    /**
     * Computes and return the object to window coordinate projection corresponding to the given Axes object.
     * @param axes the given Axes.
     * @param drawingTools the drawing tools.
     * @param canvas the current canvas.
     * @param use2dView specifies whether the default 2d view rotation angles must be used (true) or the given Axes' ones (false).
     * @return the projection
     */
    private Transformation computeProjection(Axes axes, DrawingTools drawingTools, Canvas canvas, boolean use2dView) {
        Transformation projection;

        try {
            /* Compute the zone projection. */
            Transformation zoneProjection = computeZoneProjection(axes);

            /* Compute the box transformation. */
            Transformation transformation = computeBoxTransformation(axes, canvas, use2dView);

            /* Compute the data scale and translate transformation. */
            Transformation dataTransformation = computeDataTransformation(axes);

            /* Compute the object to window coordinates projection. */
            projection = zoneProjection.rightTimes(transformation);
            projection = projection.rightTimes(dataTransformation);

            Transformation windowTrans = drawingTools.getTransformationManager().getWindowTransformation().getInverseTransformation();
            projection = windowTrans.rightTimes(projection);
        } catch (DegenerateMatrixException e) {
            return TransformationFactory.getIdentity();
        }

        return projection;
    }

    /**
     * Returns the current projection from object to window coordinates.
     * @return the projection.
     */
    public Transformation getProjection() {
        return currentProjection;
    }

    /**
     * Returns the current data scale and translate transformation.
     * @return the data transformation.
     */
    public Transformation getDataTransformation() {
        return currentDataTransformation;
    }

    /**
     * Adds the projection from object to window coordinates corresponding to a given Axes object
     * to the projection map.
     * @param axesId the identifier of the given Axes.
     * @param projection the corresponding projection.
     */
    public synchronized void addProjection(String axesId, Transformation projection) {
        projectionMap.put(axesId, projection);
    }

    /**
     * Returns the projection from object to window coordinates corresponding
     * to a given Axes object.
     * @param id the identifier of the given Axes.
     * @return the projection.
     */
    public Transformation getProjection(String id) {
        return projectionMap.get(id);
    }

    /**
     * Removes the object to window coordinate projection corresponding to a given Axes from
     * the projection map.
     * @param axesId the identifier of the given Axes.
     */
    public void removeProjection(String axesId) {
        projectionMap.remove(axesId);
    }

    /**
     * Adds the projection from object (in 2d view mode) to window coordinates corresponding to a given Axes object
     * to the projection map.
     * @param axesId the identifier of the given Axes.
     * @param projection the corresponding projection.
     */
    public synchronized void addProjection2dView(String axesId, Transformation projection) {
        projection2dViewMap.put(axesId, projection);
    }

    /**
     * Returns the projection from object (in 2d view mode) to window coordinates corresponding
     * to a given Axes object.
     * @param id the identifier of the given Axes.
     * @return the projection.
     */
    public Transformation getProjection2dView(String id) {
        return projection2dViewMap.get(id);
    }

    public Transformation getSceneProjection(String id) {
        return sceneProjectionMap.get(id);
    }

    /**
     * Removes the object (in 2d view mode) to window coordinate projection corresponding to a given Axes from
     * the projection map.
     * @param axesId the identifier of the given Axes.
     */
    public void removeProjection2dView(String axesId) {
        projection2dViewMap.remove(axesId);
    }

    /**
     * Updates both the projection from object to window coordinates and the related
     * object (in 2d view mode) to window coordinates projection for the given Axes object.
     * @param axes the given Axes.
     */
    public static void updateAxesTransformation(Axes axes) {
        DrawerVisitor currentVisitor = DrawerVisitor.getVisitor(axes.getParentFigure());
        AxesDrawer axesDrawer = currentVisitor.getAxesDrawer();

        Transformation transformation = axesDrawer.getProjection(axes.getIdentifier());

        /* The projection must be updated */
        if (transformation == null) {
            Transformation projection = axesDrawer.computeProjection(axes, currentVisitor.getDrawingTools(), currentVisitor.getCanvas(), false);

            axesDrawer.addProjection(axes.getIdentifier(), projection);
        }

        Transformation transformation2dView = axesDrawer.getProjection2dView(axes.getIdentifier());

        /* The projection must be updated */
        if (transformation2dView == null) {
            Transformation projection2dView = axesDrawer.computeProjection(axes, currentVisitor.getDrawingTools(), currentVisitor.getCanvas(), true);

            axesDrawer.addProjection2dView(axes.getIdentifier(), projection2dView);
        }
    }

    /**
     * Computes and returns the coordinates of a point projected onto the default 2d view plane.
     * To compute them, the point is projected using the object to window coordinate projection, then
     * unprojected using the object to window coordinate projection corresponding to the default 2d view
     * (which uses the default camera rotation angles).
     * To do: optimize by using the already computed 3d view projection.
     * @param axes the given Axes.
     * @param coordinates the object (x,y,z) coordinates to project onto the 2d view plane (3-element array).
     * @returns the 2d view coordinates (3-element array).
     */
    public static double[] compute2dViewCoordinates(Axes axes, double[] coordinates) {
        DrawerVisitor currentVisitor;
        AxesDrawer axesDrawer;
        Transformation projection;
        Transformation projection2d;

        currentVisitor = DrawerVisitor.getVisitor(axes.getParentFigure());

        Vector3d point = new Vector3d(coordinates);

        if (currentVisitor != null) {
            axesDrawer = currentVisitor.getAxesDrawer();

            projection = axesDrawer.computeProjection(axes, currentVisitor.getDrawingTools(), currentVisitor.getCanvas(), false);
            projection2d = axesDrawer.computeProjection(axes, currentVisitor.getDrawingTools(), currentVisitor.getCanvas(), true);

            point = projection.project(point);
            point = projection2d.unproject(point);
        }

        return new double[] {point.getX(), point.getY(), point.getZ()};
    }

    /**
     * Computes and returns the pixel coordinates from a point's coordinates expressed in the default
     * 2d view coordinate frame, using the given Axes. The returned pixel coordinates are expressed
     * in the AWT's 2d coordinate frame.
     * @param axes the given Axes.
     * @param coordinates the 2d view coordinates (3-element array: x, y, z).
     * @returns the pixel coordinates (2-element array: x, y).
     */
    public static double[] computePixelFrom2dViewCoordinates(Axes axes, double[] coordinates) {
        DrawerVisitor currentVisitor;
        AxesDrawer axesDrawer;

        double[] coords2dView = new double[] {0.0, 0.0, 0.0};

        currentVisitor = DrawerVisitor.getVisitor(axes.getParentFigure());

        if (currentVisitor != null) {
            double height;

            axesDrawer = currentVisitor.getAxesDrawer();
            height = currentVisitor.getCanvas().getHeight();

            Transformation projection2d = axesDrawer.getProjection2dView(axes.getIdentifier());

            Vector3d point = new Vector3d(coordinates);
            point = projection2d.project(point);

            /* Convert the window coordinates to pixel coordinates, only y changes due to the differing y-axis convention */
            coords2dView[0] = point.getX();
            coords2dView[1] = height - point.getY();
        }

        return coords2dView;
    }

    /**
     * Computes and returns the coordinates of a point projected onto the default 2d view plane
     * from its pixel coordinates, using the given Axes. Pixel coordinates are expressed in
     * the AWT's 2d coordinate frame.
     * The returned point's z component is set to 0, as we only have x and y as an input.
     * @param axes the given Axes.
     * @param coordinates the pixel coordinates (2-element array: x, y).
     * @return coordinates the 2d view coordinates (3-element array: x, y, z).
     */
    public static double[] compute2dViewFromPixelCoordinates(Axes axes, double[] coordinates) {
        DrawerVisitor currentVisitor;
        AxesDrawer axesDrawer;

        double[] coords2dView = new double[] {0.0, 0.0, 0.0};

        currentVisitor = DrawerVisitor.getVisitor(axes.getParentFigure());

        if (currentVisitor != null) {
            double height;

            axesDrawer = currentVisitor.getAxesDrawer();
            height = currentVisitor.getCanvas().getHeight();

            /* Convert the pixel coordinates to window coordinates, only y changes due to the differing y-axis convention */
            Vector3d point = new Vector3d(coordinates[0], height - coordinates[1], 0.0);

            Transformation projection2d = axesDrawer.getProjection2dView(axes.getIdentifier());
            point = projection2d.unproject(point);
            coords2dView = point.getData();
        }

        return coords2dView;
    }

    /**
     * Un-project the given point from AWT coordinate to given axes coordinate.
     * @param axes returned coordinate are relative to this axes.
     * @param point un-projected point.
     * @return The un-projected point.
     */
    public static Vector3d unProject(Axes axes, Vector3d point) {
        DrawerVisitor currentVisitor = DrawerVisitor.getVisitor(axes.getParentFigure());

        if (currentVisitor != null) {
            AxesDrawer axesDrawer = currentVisitor.getAxesDrawer();
            double height = currentVisitor.getCanvas().getHeight() - 1;

            Transformation projection2d = axesDrawer.getProjection(axes.getIdentifier());
            return projection2d.unproject(new Vector3d(point.getX(), height - point.getY(), point.getZ()));
        } else {
            return new Vector3d(0, 0, 0);
        }
    }

    /**
     * Computes and returns the viewing area corresponding to the given Axes object.
     * The viewing area is described by the (x, y) coordinates of the Axes box's upper-left corner
     * and the Axes box's dimensions (width and height), all values are in pixel.
     * The 2d coordinate frame in which the area is expressed uses the AWT convention:
     * upper-left window corner at (0, 0), y-axis pointing downwards).
     * @param axes the given Axes.
     * @return the Axes' viewing area (4-element array: x, y, width, height).
     */
    public static double[] getViewingArea(Axes axes) {
        DrawerVisitor currentVisitor;

        double[] viewingArea = new double[] {0.0, 0.0, 0.0, 0.0};

        currentVisitor = DrawerVisitor.getVisitor(axes.getParentFigure());

        if (currentVisitor != null) {
            double width;
            double height;
            double upperLeftY;

            AxesDrawer axesDrawer = currentVisitor.getAxesDrawer();

            width = (double) currentVisitor.getCanvas().getWidth();
            height = (double) currentVisitor.getCanvas().getHeight();

            Rectangle2D axesZone = axesDrawer.computeZone(axes);

            /* Compute the upper-left point's y coordinate */
            upperLeftY = axesZone.getY() + axesZone.getHeight() * 2.0;

            /* Convert from normalized coordinates to 2D pixel coordinates */
            viewingArea[0] = (axesZone.getX() + 1.0) * 0.5 * width;
            viewingArea[1] = (1.0 - upperLeftY) * 0.5 * height;
            viewingArea[2] = axesZone.getWidth() * width;
            viewingArea[3] = axesZone.getHeight() * height;
        }

        return viewingArea;
    }

    /**
     * Returns the culling mode corresponding to front faces.
     * @return the front face culling mode.
     */
    public FaceCullingMode getFrontFaceCullingMode() {
        return this.frontFaceCullingMode;
    }

    /**
     * Returns the culling mode corresponding to back faces.
     * @return the back face culling mode.
     */
    public FaceCullingMode getBackFaceCullingMode() {
        return this.backFaceCullingMode;
    }

    /**
     * Enables clipping for the given {@link ClippableProperty}, which describes
     * the clipping state of a clippable graphic object.
     * Depending on the object's clip state property, clipping can be either
     * disabled (OFF), performed against the parent Axes' box planes (CLIPGRF),
     * or performed against the planes defined by the object's clip box.
     * To do: find a better way to compute the clipping planes' offsets as the current one
     * may lead to problems when the interval between the min and max bounds is too small.
     * @param parentAxes the clipped object's parent Axes.
     * @param clipProperty the clipping property of a clippable object.
     */
    public void enableClipping(Axes parentAxes, ClippableProperty clipProperty) {
        DrawingTools drawingTools = visitor.getDrawingTools();

        if (clipProperty.getClipState() != ClipStateType.OFF) {
            int numPlanes = 0;
            Vector4d[] equations = null;

            /* Stores the (xmin,xmax) ,(ymin,ymax) and (zmin,zmax) clipping bounds */
            double[] clipBounds = new double[6];

            /* The offsets used for the x, y and z planes in order to avoid strict clipping */
            double[] offsets = new double[3];

            if (clipProperty.getClipState() == ClipStateType.CLIPGRF) {
                /*
                 * All the clipping planes are set as clipping is performed
                 * against the Axes box planes.
                 */
                numPlanes = 6;
                Double[] bounds = parentAxes.getDisplayedBounds();

                for (int i = 0; i < numPlanes; i++) {
                    clipBounds[i] = bounds[i];
                }

                offsets[0] = CLIPPING_EPSILON * (bounds[1] - bounds[0]);
                offsets[1] = CLIPPING_EPSILON * (bounds[3] - bounds[2]);
                offsets[2] = CLIPPING_EPSILON * (bounds[5] - bounds[4]);
            } else if (clipProperty.getClipState() == ClipStateType.ON) {
                /*
                 * The clip box property defines values only for the x and y axes,
                 * we therefore set only the x and y clipping planes.
                 */
                numPlanes = 4;
                Double[] clipBox = clipProperty.getClipBox();

                /* The clip box stores the upper-left point coordinates. */
                clipBounds[0] = clipBox[0];
                clipBounds[1] = clipBox[0] + clipBox[2];
                clipBounds[2] = clipBox[1] - clipBox[3];
                clipBounds[3] = clipBox[1];

                Double[] bounds = parentAxes.getMaximalDisplayedBounds();

                /*
                 * The logarithmic scale must be applied to clip bounds values.
                 * If any of them are invalid, we set them to the displayed
                 * left bounds (xmin or ymin).
                 */
                if (parentAxes.getXAxisLogFlag()) {
                    if (clipBounds[0] <= 0.0) {
                        clipBounds[0] = bounds[0];
                    } else {
                        clipBounds[0] = Math.log10(clipBounds[0]);
                    }

                    if (clipBounds[1] <= 0.0) {
                        clipBounds[1] = bounds[0];
                    } else {
                        clipBounds[1] = Math.log10(clipBounds[1]);
                    }
                }

                if (parentAxes.getYAxisLogFlag()) {
                    if (clipBounds[2] <= 0.0) {
                        clipBounds[2] = bounds[2];
                    } else {
                        clipBounds[2] = Math.log10(clipBounds[2]);
                    }

                    if (clipBounds[3] <= 0.0) {
                        clipBounds[3] = bounds[2];
                    } else {
                        clipBounds[3] = Math.log10(clipBounds[3]);
                    }
                }

                offsets[0] = CLIPPING_EPSILON * (clipBounds[1] - clipBounds[0]);
                offsets[1] = CLIPPING_EPSILON * (clipBounds[3] - clipBounds[2]);
            }

            equations = new Vector4d[numPlanes];

            equations[0] = new Vector4d(+1, 0, 0, -clipBounds[0] + offsets[0]);
            equations[1] = new Vector4d(-1, 0, 0, +clipBounds[1] + offsets[0]);
            equations[2] = new Vector4d(0, +1, 0, -clipBounds[2] + offsets[1]);
            equations[3] = new Vector4d(0, -1, 0, +clipBounds[3] + offsets[1]);

            /* If clipping is performed against the Axes box, the z plane equations must be initialized. */
            if (numPlanes == 6) {
                equations[4] = new Vector4d(0, 0, +1, -clipBounds[4] + offsets[2]);
                equations[5] = new Vector4d(0, 0, -1, +clipBounds[5] + offsets[2]);
            }

            Transformation currentTransformation = drawingTools.getTransformationManager().getTransformation();
            for (int i = 0 ; i < numPlanes; i++) {
                ClippingPlane plane = drawingTools.getClippingManager().getClippingPlane(i);
                plane.setTransformation(currentTransformation);
                plane.setEquation(equations[i]);
                plane.setEnable(true);
            }
        }
    }

    /**
     * Disables clipping for the given {@link ClippableProperty}.
     * @param clipProperty the clip property for which clipping is disabled.
     */
    public void disableClipping(ClippableProperty clipProperty) {
        int numPlanes = 0;

        if (clipProperty.getClipState() == ClipStateType.CLIPGRF) {
            numPlanes = 6;
        } else if (clipProperty.getClipState() == ClipStateType.ON) {
            numPlanes = 4;
        }

        for (int i = 0 ; i < numPlanes; i++) {
            ClippingPlane plane = visitor.getDrawingTools().getClippingManager().getClippingPlane(i);
            plane.setEnable(false);
        }

        visitor.getDrawingTools().getClippingManager().disableClipping();
    }

    /**
     * Returns the x-axis label positioner.
     * @return the x-axis label positioner.
     */
    public AxisLabelPositioner getXAxisLabelPositioner() {
        return this.xAxisLabelPositioner;
    }

    /**
     * Returns the y-axis label positioner.
     * @return the y-axis label positioner.
     */
    public AxisLabelPositioner getYAxisLabelPositioner() {
        return this.yAxisLabelPositioner;
    }

    /**
     * Returns the z-axis label positioner.
     * @return the z-axis label positioner.
     */
    public AxisLabelPositioner getZAxisLabelPositioner() {
        return this.zAxisLabelPositioner;
    }

    /**
     * Returns the title positioner.
     * @return the title positioner.
     */
    public LabelPositioner getTitlePositioner() {
        return this.titlePositioner;
    }

    public void disposeAll() {
        this.rulerDrawer.disposeAll();
        this.projectionMap.clear();
    }

    public void update(String id, int property) {
        this.rulerDrawer.update(id, property);
    }

    public void dispose(String id) {
        this.rulerDrawer.dispose(id);
    }
}
