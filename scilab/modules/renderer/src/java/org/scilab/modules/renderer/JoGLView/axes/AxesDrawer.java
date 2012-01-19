/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 * Copyright (C) 2011-2012 - DIGITEO - Manuel Juliachs
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
import org.scilab.modules.graphic_objects.axes.Camera;
import org.scilab.modules.graphic_objects.contouredObject.Line;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.label.Label;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;
import org.scilab.modules.renderer.JoGLView.axes.ruler.AxesRulerDrawer;
import org.scilab.modules.renderer.JoGLView.label.AxisLabelPositioner;
import org.scilab.modules.renderer.JoGLView.label.LabelManager;
import org.scilab.modules.renderer.JoGLView.label.LabelPositioner;
import org.scilab.modules.renderer.JoGLView.label.TitlePositioner;
import org.scilab.modules.renderer.JoGLView.label.YAxisLabelPositioner;
import org.scilab.modules.renderer.JoGLView.util.ColorFactory;

import java.awt.geom.Rectangle2D;

/**
 *
 * AxesDrawer are used by {@see DrawerVisitor} to draw {@see Axes}.
 *
 * @author Pierre Lando
 */
public class AxesDrawer {
    private static final double DEFAULT_THETA = 270.0;
    private static final Line.LineType HIDDEN_BORDER_PATTERN = Line.LineType.DASH;

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

    /**
     * Default constructor.
     * @param visitor the parent {@see DrawerVisitor}.
     * @param labelManager the parent's {@see LabelManager}.
     */
    public AxesDrawer(DrawerVisitor visitor, LabelManager labelManager) {
        this.visitor = visitor;
        this.geometries = new Geometries(visitor.getCanvas());
        this.rulerDrawer = new AxesRulerDrawer(visitor.getCanvas());

        this.labelManager = labelManager;

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
     * @throws DegenerateMatrixException if the axes is not representable.
     */
    public void draw(Axes axes) throws DegenerateMatrixException {
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
        Transformation transformation = computeBoxTransformation(axes, canvas);
        modelViewStack.pushRightMultiply(transformation);

        /* Compute the data scale and translate transformation. */
        Transformation dataTransformation = computeDataTransformation(axes);

        /* Compute the object to window coordinates projection. */
        currentProjection = zoneProjection.rightTimes(transformation);
        currentProjection = currentProjection.rightTimes(dataTransformation);

        Transformation windowTrans = drawingTools.getTransformationManager().getWindowTransformation().getInverseTransformation();
        currentProjection = windowTrans.rightTimes(currentProjection);

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

        /* Draw the labels. */

        /* X-axis label */
        String labelID = axes.getXAxisLabel();
        Label label = (Label) GraphicController.getController().getObjectFromId(labelID);

        labelManager.positionAndDraw(drawingTools, colorMap, label, xAxisLabelPositioner, this, true);

        /* Y-axis label */
        labelID = axes.getYAxisLabel();
        label = (Label) GraphicController.getController().getObjectFromId(labelID);

        labelManager.positionAndDraw(drawingTools, colorMap, label, yAxisLabelPositioner, this, true);

        /* Z-axis label */
        labelID = axes.getZAxisLabel();
        label = (Label) GraphicController.getController().getObjectFromId(labelID);

        labelManager.positionAndDraw(drawingTools, colorMap, label, zAxisLabelPositioner, this, (axes.getViewAsEnum() == Camera.ViewType.VIEW_3D));

        /* Title */
        labelID = axes.getTitle();
        label = (Label) GraphicController.getController().getObjectFromId(labelID);

        labelManager.positionAndDraw(drawingTools, colorMap, label, titlePositioner, this, true);


        /**
         * Scale and translate for data fitting.
         * And draw data.
         */
        modelViewStack.pushRightMultiply(dataTransformation);

        // TODO implement clipgrf
        Double[] bounds = axes.computeDisplayedBounds();
        Vector4d[] equations = new Vector4d[]{
                new Vector4d(+1, 0, 0, -bounds[0]),
                new Vector4d(-1, 0, 0, +bounds[1]),

                new Vector4d(0, +1, 0, -bounds[2]),
                new Vector4d(0, -1, 0, +bounds[3]),

                new Vector4d(0, 0, +1, -bounds[4]),
                new Vector4d(0, 0, -1, +bounds[5])
        };

        Transformation currentTransformation = drawingTools.getTransformationManager().getTransformation();
        for (int i = 0 ; i < 6 ; i++) {
            ClippingPlane plane = drawingTools.getClippingManager().getClippingPlane(i);
            plane.setTransformation(currentTransformation);
            plane.setEquation(equations[i]);
            plane.setEnable(true);
        }

        /* Compute the front and back culling modes */
        computeFaceCullingModes(axes);

        visitor.askAcceptVisitor(axes.getChildren());
        modelViewStack.pop();

        drawingTools.getClippingManager().disableClipping();

        // Reset transformation stacks.
        modelViewStack.pop();
        projectionStack.pop();
    }

    /**
     * Draw the axes background.
     * @param axes the {@see Axes}
     * @param drawingTools the {@see DrawingTools} to use.
     * @param colorMap the current {@see ColorMap}
     */
    private void drawBackground(Axes axes, DrawingTools drawingTools, ColorMap colorMap) {
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
     */
    private void drawBox(Axes axes, DrawingTools drawingTools, ColorMap colorMap) {
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
        Double[] bounds = axes.computeDisplayedBounds();

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
     * @param canvas the current {@see Canvas}
     * @return box transformation for the given axes.
     * @throws DegenerateMatrixException if data bounds are incorrect or canvas with or length are zero.
     */
    private Transformation computeBoxTransformation(Axes axes, Canvas canvas) throws DegenerateMatrixException {
        Double[] bounds = axes.computeDisplayedBounds();

        double tmpX;
        double tmpY;
        double tmpZ;

        Transformation transformation;

        // Set zone aspect ratio.
        Rectangle2D zone = computeZone(axes);
        double axesRatio = zone.getWidth() / zone.getHeight();
        transformation = TransformationFactory.getPreferredAspectRatioTransformation(canvas.getDimension(), axesRatio);

        // Rotate.
        Transformation alphaRotation = TransformationFactory.getRotationTransformation(-axes.getRotationAngles()[0], 1.0, 0.0, 0.0);
        transformation = transformation.rightTimes(alphaRotation);
        Transformation thetaRotation = TransformationFactory.getRotationTransformation(DEFAULT_THETA + axes.getRotationAngles()[1], 0.0, 0.0, 1.0);
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
            double minScale = Math.min(tmpX, tmpY);
            isoScale = TransformationFactory.getScaleTransformation(minScale, minScale, tmpZ);
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
        Double[] currentBounds = axes.computeDisplayedBounds();

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

        dataCoordinates[0] = 1 - 2.0*(point.getX()-reversedBounds[0])/reversedBoundsIntervals[0];
        dataCoordinates[1] = 1 - 2.0*(point.getY()-reversedBounds[2])/reversedBoundsIntervals[1];
        dataCoordinates[2] = 1 - 2.0*(point.getZ()-reversedBounds[4])/reversedBoundsIntervals[2];

        return new Vector3d(dataCoordinates);
    }

    /**
     * Converts a point from box coordinates (used when drawing axis rulers) to object coordinates.
     * @param point the point in box coordinates.
     * @return the point in object coordinates.
     */
    public Vector3d getObjectCoordinates(Vector3d point) {
        double[] objectCoordinates = new double[3];

        objectCoordinates[0] = 0.5*(1.0-point.getX())*(reversedBounds[1]-reversedBounds[0]) + reversedBounds[0];
        objectCoordinates[1] = 0.5*(1.0-point.getY())*(reversedBounds[3]-reversedBounds[2]) + reversedBounds[2];
        objectCoordinates[2] = 0.5*(1.0-point.getZ())*(reversedBounds[5]-reversedBounds[4]) + reversedBounds[4];

        return new Vector3d(objectCoordinates);
    }

    /**
     * Returns the current projection from object to window coordinates.
     * @return the projection.
     */
    public Transformation getProjection() {
        return currentProjection;
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
     * Returns the x-axis label positioner.
     * @return the x-axis label positioner.
     */
    public LabelPositioner getXAxisLabelPositioner() {
        return this.xAxisLabelPositioner;
    }

    /**
     * Returns the y-axis label positioner.
     * @return the y-axis label positioner.
     */
    public LabelPositioner getYAxisLabelPositioner() {
        return this.yAxisLabelPositioner;
    }

    /**
     * Returns the z-axis label positioner.
     * @return the z-axis label positioner.
     */
    public LabelPositioner getZAxisLabelPositioner() {
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
    }

    public void update(String id, String property) {
        this.rulerDrawer.update(id, property);
    }

    public void dispose(String id) {
        this.rulerDrawer.dispose(id);
    }
}
