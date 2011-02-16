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
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.TransformationFactory;
import org.scilab.forge.scirenderer.tranformations.TransformationStack;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.axes.Box;
import org.scilab.modules.graphic_objects.contouredObject.Line;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;
import org.scilab.modules.renderer.JoGLView.axes.ruler.AxesRulerDrawer;
import org.scilab.modules.renderer.JoGLView.axes.ruler.RulerSpriteManagerSet;
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
    private static final Line.LineType HIDDEN_BORDER_PATTERN = Line.LineType.STYLE1;

    private final DrawerVisitor visitor;
    private final Geometries geometries;

    private final AxesRulerDrawer rulerDrawer;
    private final RulerSpriteManagerSet rulerSpriteManagerSet;


    /**
     * Default constructor.
     * @param visitor the parent {@see DrawerVisitor}.
     */
    public AxesDrawer(DrawerVisitor visitor) {
        this.visitor = visitor;
        this.geometries = new Geometries(visitor.getCanvas());

        rulerSpriteManagerSet = new RulerSpriteManagerSet(visitor.getCanvas().getSpriteManager());
        rulerDrawer = new AxesRulerDrawer(rulerSpriteManagerSet);
    }


    /**
     * Draw the given {@see Axes}.
     * @param axes {@see Axes} to draw.
     */
    public void draw(Axes axes) {
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
        rulerDrawer.drawRuler(axes, colorMap, drawingTools);

        /**
         * Scale and translate for data fitting.
         * And draw data.
         */
        Transformation dataTransformation = computeDataTransformation(axes);
        modelViewStack.pushRightMultiply(dataTransformation);
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
     * @param colorMap the curent {@see ColorMap}
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
        return TransformationFactory.getScaleTransformation(
                matrix[2] < 0 ? 1 : -1,
                matrix[6] < 0 ? 1 : -1,
                matrix[10] < 0 ? 1 : -1
        );
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
     */
    private Transformation computeZoneProjection(Axes axes) {
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
     */
    private Transformation computeDataTransformation(Axes axes) {
        Double[] bounds = axes.getDataBounds();

        // Reverse data if needed.
        Transformation transformation = TransformationFactory.getScaleTransformation(
                axes.getAxes()[0].getReverse() ? -1 : 1,
                axes.getAxes()[1].getReverse() ? -1 : 1,
                axes.getAxes()[2].getReverse() ? -1 : 1
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
     */
    private Transformation computeBoxTransformation(Axes axes, Canvas canvas) {
        // TODO : zoom box ?
        Double[] bounds = axes.getDataBounds();

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
        Transformation thetaRotation = TransformationFactory.getRotationTransformation(DEFAULT_THETA - axes.getRotationAngles()[1], 0.0, 0.0, 1.0);
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

    public RulerSpriteManagerSet getRulerSpriteManagerSet() {
        return rulerSpriteManagerSet;
    }
}
