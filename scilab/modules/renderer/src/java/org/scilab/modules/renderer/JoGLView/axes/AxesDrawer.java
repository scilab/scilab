/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
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

package org.scilab.modules.renderer.JoGLView.axes;

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
import org.scilab.modules.graphic_objects.axes.AxisProperty;
import org.scilab.modules.graphic_objects.axes.Box;
import org.scilab.modules.graphic_objects.axes.Camera.ViewType;
import org.scilab.modules.graphic_objects.contouredObject.Line;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.ClippableProperty;
import org.scilab.modules.graphic_objects.graphicObject.ClippableProperty.ClipStateType;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.legend.Legend;
import org.scilab.modules.graphic_objects.legend.Legend.LegendLocation;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;
import org.scilab.modules.renderer.JoGLView.axes.ruler.AxesRulerDrawer;
import org.scilab.modules.renderer.JoGLView.label.AxisLabelPositioner;
import org.scilab.modules.renderer.JoGLView.label.LabelManager;
import org.scilab.modules.renderer.JoGLView.label.TitlePositioner;
import org.scilab.modules.renderer.JoGLView.label.YAxisLabelPositioner;
import org.scilab.modules.renderer.JoGLView.legend.LegendDrawer;
import org.scilab.modules.renderer.JoGLView.util.ColorFactory;
import org.scilab.modules.renderer.JoGLView.util.ScaleUtils;

import java.awt.Dimension;
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
    private final Map<Integer, AxisLabelPositioner> xAxisLabelPositioner = new HashMap<Integer, AxisLabelPositioner>();

    /** The y-axis label positioner. */
    private final Map<Integer, AxisLabelPositioner> yAxisLabelPositioner = new HashMap<Integer, AxisLabelPositioner>();

    /** The z-axis label positioner. */
    private final Map<Integer, AxisLabelPositioner> zAxisLabelPositioner = new HashMap<Integer, AxisLabelPositioner>();

    /** The title positioner. */
    private final Map<Integer, TitlePositioner> titlePositioner = new HashMap<Integer, TitlePositioner>();

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
    private final Map<Integer, Transformation> projectionMap = new HashMap<Integer, Transformation>();

    /** The set of object (in 2d view mode) to window coordinate projections associated to all the Axes drawn by this drawer. */
    private final Map<Integer, Transformation> projection2dViewMap = new HashMap<Integer, Transformation>();

    /** This is a __MAP__ */
    private final Map<Integer, Transformation> sceneProjectionMap = new HashMap<Integer, Transformation>();

    /**
     * Default constructor.
     * @param visitor the parent {@see DrawerVisitor}.
     */
    public AxesDrawer(DrawerVisitor visitor) {
        this.visitor = visitor;
        this.labelManager = visitor.getLabelManager();
        this.geometries = new Geometries(visitor.getCanvas());
        this.rulerDrawer = new AxesRulerDrawer(visitor.getCanvas());

        reversedBounds = new double[6];
        reversedBoundsIntervals = new double[3];
    }

    /**
     * @return the axis label manager
     */
    public LabelManager getLabelManager() {
        return labelManager;
    }

    public Transformation getCurrentProjection(Axes axes) throws DegenerateMatrixException {
        DrawingTools drawingTools = visitor.getDrawingTools();
        Transformation zoneProjection = computeZoneProjection(axes);
        Transformation transformation = computeBoxTransformation(axes, visitor.getCanvas().getDimension(), false);
        Transformation dataTransformation = computeDataTransformation(axes);
        Transformation windowTrans;
        if (drawingTools == null) {
            windowTrans = TransformationFactory.getIdentity();
        } else {
            windowTrans = drawingTools.getTransformationManager().getWindowTransformation().getInverseTransformation();
        }
        Transformation current = zoneProjection.rightTimes(transformation);
        current = current.rightTimes(dataTransformation);

        return windowTrans.rightTimes(current);
    }

    /**
     * Compute the graduations on the axes
     * @param axes the axes
     */
    public void computeRulers(Axes axes) {
        Figure figure = (Figure) GraphicController.getController().getObjectFromId(axes.getParentFigure());

        //figure may be null during xml loading
        if (figure == null) {
            return;
        }

        final ColorMap colorMap = figure.getColorMap();
        try {
            Dimension dims = visitor.getCanvas().getDimension();
            double w = dims.getWidth() / 2.0;
            double h = dims.getHeight() / 2.0;

            Transformation windowTrans = TransformationFactory.getAffineTransformation(new Vector3d(w, h, 1), new Vector3d(w, h, 0));
            Transformation zoneProjection = computeZoneProjection(axes);
            Transformation transformation = computeBoxTransformation(axes, dims, false);
            Transformation canvasTrans = windowTrans.rightTimes(zoneProjection).rightTimes(transformation);

            rulerDrawer.computeRulers(axes, this, colorMap, transformation, canvasTrans);
        } catch (DegenerateMatrixException e) {

        }
    }

    public void computeMargins(Axes axes) {
        if (axes.getAutoMargins() && axes.getViewAsEnum() == ViewType.VIEW_2D) {
            Figure figure = (Figure) GraphicController.getController().getObjectFromId(axes.getParentFigure());
            ColorMap colorMap = null;
            if (figure != null) {
                colorMap = figure.getColorMap();
            } else {
                return;
            }

            Dimension[] marginLabels = labelManager.getLabelsSize(colorMap, axes, this);
            Integer[] size = {visitor.getCanvas().getWidth(), visitor.getCanvas().getHeight()};
            if (size[0] != 0 && size[1] != 0) {
                // [x_left, y_up, w, h]
                Double[] axesBounds = axes.getAxesBounds();
                // [l, r, t, b]
                Double[] margins = axes.getMargins();
                // m is a copy of margins
                Double[] mt = new Double[] { 0., 0., 0., 0. };
                Double[] ml = new Double[] { 0., 0., 0., 0. };
                Double[] ma = new Double[] { 0., 0., 0., 0. };
                Double[] m = new Double[] { 0., 0., 0., 0. };
                AxisProperty.AxisLocation xloc = axes.getXAxis().getAxisLocation();
                AxisProperty.AxisLocation yloc = axes.getYAxis().getAxisLocation();
                final double DEFAULT_MARGIN = 0.125;

                // We compute the adapted margins for axes titles.
                if (marginLabels[0].height != 0 || marginLabels[2].height != 0 || marginLabels[1].width != 0) {
                    if (marginLabels[2].height != 0) {
                        final double th = (marginLabels[2].height + 2 + TitlePositioner.TITLEOFFSET) / (size[1] * axesBounds[3]);
                        mt[2] = th;
                    }

                    if (marginLabels[0].height != 0 && (xloc == AxisProperty.AxisLocation.BOTTOM || xloc == AxisProperty.AxisLocation.TOP)) {
                        final double xh = (marginLabels[0].height + 2) / (size[1] * axesBounds[3]);
                        if (xloc == AxisProperty.AxisLocation.BOTTOM) {
                            mt[3] = xh;
                        } else {
                            mt[2] += xh;
                        }
                    }

                    if (marginLabels[1].width != 0 && (yloc == AxisProperty.AxisLocation.LEFT || yloc == AxisProperty.AxisLocation.RIGHT)) {
                        final double yh = (marginLabels[1].width + 2) / (size[0] * axesBounds[2]);
                        if (yloc == AxisProperty.AxisLocation.LEFT) {
                            mt[0] = yh;
                        } else {
                            mt[1] = yh;
                        }
                    }
                }

                //computeRulers(axes);
                final double xratio = rulerDrawer.getRulerDrawer(axes, 0).getDistanceRatio();
                final double yratio = rulerDrawer.getRulerDrawer(axes, 1).getDistanceRatio();

                if (xloc == AxisProperty.AxisLocation.BOTTOM) {
                    ma[3] = (1 - margins[2] - margins[3]) * xratio / 2.;
                } else if (xloc == AxisProperty.AxisLocation.TOP) {
                    ma[2] = (1 - margins[2] - margins[3]) * xratio / 2.;
                }

                if (yloc == AxisProperty.AxisLocation.LEFT) {
                    ma[0] = (1 - margins[0] - margins[1]) * yratio / 2.;
                } else if (yloc == AxisProperty.AxisLocation.RIGHT) {
                    ma[1] = (1 - margins[0] - margins[1]) * yratio / 2.;
                }

                // Get the legend if any (only one ???)
                if (axes.getChildren() != null) {
                    for (Integer i : axes.getChildren()) {
                        GraphicObject child = GraphicController.getController().getObjectFromId(i);
                        if (child instanceof Legend) {
                            Legend legend = (Legend) child;
                            Dimension legDims = visitor.getLegendDrawer().computeDimensions(axes, legend);
                            if (legDims != null) {
                                LegendLocation legLoc = legend.getLegendLocationAsEnum();
                                double C;
                                /*
                                 * Legends dimension are linearly dependent of margins... so we need to solve an equation
                                 * to find a good value for margins.
                                 * For example:
                                 *  legend.w = texture.w + 3/8 * line.w + line.w
                                 *  where line.w = LINE_WIDTH * ab[2] * (1 - m[0] - m[1]) * size[0];
                                 *  the minimal value for m[1] is the solution of the equation (where unknown is m[1]):
                                 *   legend.w = ab[2] * m[1] * size[0].
                                 */
                                switch (legLoc) {
                                    case OUT_UPPER_RIGHT:
                                    case OUT_LOWER_RIGHT:
                                        // 1/8 of LINE_WIDTH is xOffset
                                        // see legendDims[0] = ... in LegendDrawer::draw
                                        // we add 2*xoffset to have a little space around the box
                                        C = legend.getLineWidth() + LegendDrawer.LINE_WIDTH * (3. / 8. + 2. / 8.);
                                        m[0] = Math.max(ma[0] + mt[0], DEFAULT_MARGIN);
                                        m[1] = Math.max(((legDims.width + 2) / (axesBounds[2] * size[0]) + C * (1 - m[0])) / (1 + C) + ma[1] + mt[1], DEFAULT_MARGIN);
                                        break;
                                    case OUT_UPPER_LEFT:
                                    case OUT_LOWER_LEFT:
                                        C = legend.getLineWidth() + LegendDrawer.LINE_WIDTH * (3. / 8. + 2. / 8.);
                                        m[1] = Math.max(ma[1] + mt[1], DEFAULT_MARGIN);
                                        m[0] = Math.max(((legDims.width + 2) / (axesBounds[2] * size[0]) + C * (1 - m[1])) / (1 + C) + ma[0] + mt[0], DEFAULT_MARGIN);
                                        break;
                                    case UPPER_CAPTION:
                                        C = LegendDrawer.Y_OFFSET * (3. + 2.);
                                        m[3] = Math.max(ma[3] + mt[3], DEFAULT_MARGIN);
                                        m[2] = Math.max(Math.max(((legDims.height + 2) / (axesBounds[3] * size[1]) + C * (1 - m[3])) / (1 + C), mt[2]) + ma[2], DEFAULT_MARGIN);
                                        break;
                                    case LOWER_CAPTION:
                                        C = LegendDrawer.Y_OFFSET * (3. + 2.);
                                        m[2] = Math.max(ma[2] + mt[2], DEFAULT_MARGIN);
                                        m[3] = Math.max(Math.max(((legDims.height + 2) / (axesBounds[3] * size[1]) + C * (1 - m[2])) / (1 + C), mt[3]) + ma[3], DEFAULT_MARGIN);
                                        break;
                                    default:
                                }
                            }
                            break;
                        }
                    }
                }

                for (int i = 0; i < m.length; i++) {
                    if (m[i] == 0) {
                        m[i] = Math.max(ma[i] + mt[i], DEFAULT_MARGIN);
                    }
                }

                if (!m[0].equals(margins[0]) || !m[1].equals(margins[1]) || !m[2].equals(margins[2]) || !m[3].equals(margins[3])) {
                    axes.setMargins(m);
                    //computeRulers(axes);
                }
            }
        }
    }

    /**
     * Draw the given {@see Axes}.
     * @param axes {@see Axes} to draw.
     * @throws org.scilab.forge.scirenderer.SciRendererException if the draw fail.
     */
    public void draw(Axes axes) throws SciRendererException {
        DrawingTools drawingTools = visitor.getDrawingTools();
        //Integer[] size = visitor.getFigure().getAxesSize();
        //Dimension canvasDimension = new Dimension(size[0], size[1]);
        Dimension canvasDimension = visitor.getCanvas().getDimension();
        ColorMap colorMap = visitor.getColorMap();
        TransformationStack modelViewStack = drawingTools.getTransformationManager().getModelViewStack();
        TransformationStack projectionStack = drawingTools.getTransformationManager().getProjectionStack();

        // Set axes zone.
        Transformation zoneProjection = computeZoneProjection(axes);
        projectionStack.push(zoneProjection);

        // Set box projection.
        Transformation transformation = computeBoxTransformation(axes, canvasDimension, false);
        modelViewStack.pushRightMultiply(transformation);

        /* Compute the data scale and translate transformation. */
        Transformation dataTransformation = computeDataTransformation(axes);

        currentDataTransformation = dataTransformation;

        /* Compute the object to window coordinates projection. */
        currentProjection = zoneProjection.rightTimes(transformation);
        currentProjection = currentProjection.rightTimes(dataTransformation);

        sceneProjectionMap.put(axes.getIdentifier(), currentProjection);

        Transformation windowTrans = drawingTools.getTransformationManager().getInverseWindowTransformation();
        currentProjection = windowTrans.rightTimes(currentProjection);

        /* Update the projection maps with the resulting projections. */
        addProjection(axes.getIdentifier(), currentProjection);

        /* 2d view projection, to do: optimize computation */
        if ((axes.getRotationAngles()[0] != 0 || axes.getRotationAngles()[1] != DEFAULT_THETA)) {
            Transformation transformation2dView = computeBoxTransformation(axes, canvasDimension, true);
            currentProjection = zoneProjection.rightTimes(transformation2dView);
            currentProjection = currentProjection.rightTimes(dataTransformation);
            currentProjection = windowTrans.rightTimes(currentProjection);
        }

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
            if (axes.getViewAsEnum() != ViewType.VIEW_2D) {
                appearance.setLineColor(ColorFactory.createColor(colorMap, axes.getHiddenAxisColor()));
                appearance.setLineWidth(axes.getLineThickness().floatValue());
                appearance.setLinePattern(HIDDEN_BORDER_PATTERN.asPattern());
                drawingTools.draw(geometries.getHiddenBoxBorderGeometry(), appearance);
            }

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
     * Compute zone where the axes is draw. In normalized window coordinate.
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
        // Reverse data if needed.
        Transformation transformation = TransformationFactory.getScaleTransformation(
                                            axes.getAxes()[0].getReverse() ? 1 : -1,
                                            axes.getAxes()[1].getReverse() ? 1 : -1,
                                            axes.getAxes()[2].getReverse() ? 1 : -1
                                        );

        if (axes.getZoomEnabled()) {
            Double[] bounds = axes.getCorrectedBounds();

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

        return transformation;
    }


    /**
     * Compute box transformation for the given axes.
     *
     * The box transformation is applied to the axes box to fit in the canvas.
     *
     * @param axes the given {@see Axes}.
     * @param canvasDimension the current canvas {@see Canvas}.
     * @param use2dView specifies whether the default 2d view rotation angles must be used (true) or the given Axes' ones (false).
     * @return box transformation for the given axes.
     * @throws DegenerateMatrixException if data bounds are incorrect or canvas with or length are zero.
     */
    private Transformation computeBoxTransformation(Axes axes, Dimension canvasDimension, boolean use2dView) throws DegenerateMatrixException {
        Double[] bounds = axes.getDisplayedBounds();
        double theta;

        double tmpX;
        double tmpY;
        double tmpZ;

        Transformation transformation;

        // Set zone aspect ratio.
        Rectangle2D zone = computeZone(axes);
        double axesRatio = zone.getWidth() / zone.getHeight();
        transformation = TransformationFactory.getPreferredAspectRatioTransformation(canvasDimension, axesRatio);

        // Rotate.
        if (use2dView) {
            theta = 2 * DEFAULT_THETA;
        } else {
            double alpha = -axes.getRotationAngles()[0];
            theta = DEFAULT_THETA + axes.getRotationAngles()[1];
            if (alpha != 0) {
                Transformation alphaRotation = TransformationFactory.getRotationTransformation(alpha, 1.0, 0.0, 0.0);
                transformation = transformation.rightTimes(alphaRotation);
            }
        }

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
            if (tmpX != 1 || tmpY != 1 || tmpZ != 1) {
                Transformation cubeScale = TransformationFactory.getScaleTransformation(tmpX, tmpY, tmpZ);
                transformation = transformation.rightTimes(cubeScale);
            }
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
            Double[] margins = axes.getMargins();
            double w = (1 - margins[0] - margins[1]) * axesBounds[2];
            double h = (1 - margins[2] - margins[3]) * axesBounds[3];
            double minScale;
            if (h > w) {
                minScale = Math.min(tmpX, tmpY * (h / w));
            } else {
                minScale = Math.min(tmpX * (w / h), tmpY);
            }
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
        Double[] currentBounds = axes.getCorrectedBounds();

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
     * @param canvasDimension the current canvas dimension.
     * @param use2dView specifies whether the default 2d view rotation angles must be used (true) or the given Axes' ones (false).
     * @return the projection
     */
    private Transformation computeProjection(Axes axes, DrawingTools drawingTools, Dimension canvasDimension, boolean use2dView) {
        Transformation projection;

        if (drawingTools == null) {
            return TransformationFactory.getIdentity();
        }

        try {
            /* Compute the zone projection. */
            Transformation zoneProjection = computeZoneProjection(axes);

            /* Compute the box transformation. */
            Transformation transformation = computeBoxTransformation(axes, canvasDimension, use2dView);

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
    public synchronized void addProjection(Integer axesId, Transformation projection) {
        projectionMap.put(axesId, projection);
    }

    /**
     * Returns the projection from object to window coordinates corresponding
     * to a given Axes object.
     * @param id the identifier of the given Axes.
     * @return the projection.
     */
    public Transformation getProjection(Integer id) {
        return projectionMap.get(id);
    }

    /**
     * Removes the object to window coordinate projection corresponding to a given Axes from
     * the projection map.
     * @param axesId the identifier of the given Axes.
     */
    public void removeProjection(Integer axesId) {
        projectionMap.remove(axesId);
    }

    /**
     * Adds the projection from object (in 2d view mode) to window coordinates corresponding to a given Axes object
     * to the projection map.
     * @param axesId the identifier of the given Axes.
     * @param projection the corresponding projection.
     */
    public synchronized void addProjection2dView(Integer axesId, Transformation projection) {
        projection2dViewMap.put(axesId, projection);
    }

    /**
     * Returns the projection from object (in 2d view mode) to window coordinates corresponding
     * to a given Axes object.
     * @param id the identifier of the given Axes.
     * @return the projection.
     */
    public Transformation getProjection2dView(Integer id) {
        return projection2dViewMap.get(id);
    }

    public Transformation getSceneProjection(Integer id) {
        return sceneProjectionMap.get(id);
    }

    /**
     * Removes the object (in 2d view mode) to window coordinate projection corresponding to a given Axes from
     * the projection map.
     * @param axesId the identifier of the given Axes.
     */
    public void removeProjection2dView(Integer axesId) {
        projection2dViewMap.remove(axesId);
    }

    /**
     * Updates both the projection from object to window coordinates and the related
     * object (in 2d view mode) to window coordinates projection for the given Axes object.
     * @param axes the given Axes.
     */
    public static void updateAxesTransformation(Axes axes) {
        DrawerVisitor currentVisitor = DrawerVisitor.getVisitor(axes.getParentFrameOrFigure());
        AxesDrawer axesDrawer = currentVisitor.getAxesDrawer();
        Dimension canvasDimension = currentVisitor.getCanvas().getDimension();

        Transformation transformation = axesDrawer.getProjection(axes.getIdentifier());

        /* The projection must be updated */
        if (transformation == null) {
            Transformation projection = axesDrawer.computeProjection(axes, currentVisitor.getDrawingTools(), canvasDimension, false);

            axesDrawer.addProjection(axes.getIdentifier(), projection);
        }

        Transformation transformation2dView = axesDrawer.getProjection2dView(axes.getIdentifier());

        /* The projection must be updated */
        if (transformation2dView == null) {
            Transformation projection2dView = axesDrawer.computeProjection(axes, currentVisitor.getDrawingTools(), canvasDimension, true);

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
        // used in geom3d

        DrawerVisitor currentVisitor = DrawerVisitor.getVisitor(axes.getParentFrameOrFigure());
        AxesDrawer axesDrawer;
        Transformation projection;
        Transformation projection2d;
        double[] coords = coordinates;

        if (currentVisitor != null) {
            boolean[] logFlags = { axes.getXAxisLogFlag(), axes.getYAxisLogFlag(), axes.getZAxisLogFlag()};
            Dimension canvasDimension = currentVisitor.getCanvas().getDimension();
            double[][] factors = axes.getScaleTranslateFactors();
            ScaleUtils.applyLogScale(coords, logFlags);

            axesDrawer = currentVisitor.getAxesDrawer();
            coords[0] = coords[0] * factors[0][0] + factors[1][0];
            coords[1] = coords[1] * factors[0][1] + factors[1][1];
            coords[2] = coords[2] * factors[0][2] + factors[1][2];

            projection = axesDrawer.computeProjection(axes, currentVisitor.getDrawingTools(), canvasDimension, false);
            projection2d = axesDrawer.computeProjection(axes, currentVisitor.getDrawingTools(), canvasDimension, true);
            Vector3d point = new Vector3d(coords);
            point = projection.project(point);
            point = projection2d.unproject(point);

            coords = point.getData();
            coords[0] = (coords[0] - factors[1][0]) / factors[0][0];
            coords[1] = (coords[1] - factors[1][1]) / factors[0][1];
            coords[2] = (coords[2] - factors[1][2]) / factors[0][2];

            ScaleUtils.applyInverseLogScale(coords, logFlags);
        }

        return coords;
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
        // used by xchange

        DrawerVisitor currentVisitor = DrawerVisitor.getVisitor(axes.getParentFrameOrFigure());
        AxesDrawer axesDrawer;
        double[] coords2dView = new double[] {0.0, 0.0, 0.0};

        if (currentVisitor != null) {
            boolean[] logFlags = { axes.getXAxisLogFlag(), axes.getYAxisLogFlag(), axes.getZAxisLogFlag()};
            double[][] factors = axes.getScaleTranslateFactors();
            ScaleUtils.applyLogScale(coordinates, logFlags);

            axesDrawer = currentVisitor.getAxesDrawer();
            coords2dView[0] = coordinates[0] * factors[0][0] + factors[1][0];
            coords2dView[1] = coordinates[1] * factors[0][1] + factors[1][1];
            coords2dView[2] = coordinates[2] * factors[0][2] + factors[1][2];

            Transformation projection2d = axesDrawer.getProjection2dView(axes.getIdentifier());
            if (projection2d == null) {
                updateAxesTransformation(axes);
                projection2d = axesDrawer.getProjection2dView(axes.getIdentifier());
            }

            Vector3d point = new Vector3d(coords2dView);
            point = projection2d.project(point);

            /* Convert the window coordinates to pixel coordinates, only y changes due to the differing y-axis convention */
            coords2dView[0] = point.getX();
            coords2dView[1] = currentVisitor.getCanvas().getHeight() - point.getY();
            coords2dView[2] = 0;
        }

        return coords2dView;
    }

    /**
     * Computes and returns the pixel coordinates from a point's coordinates expressed in the current
     * 3d view coordinate frame, using the given Axes. The returned pixel coordinates are expressed
     * in the AWT's 2d coordinate frame.
     * @param axes the given Axes.
     * @param coordinates the 3d view coordinates (3-element array: x, y, z).
     * @returns the pixel coordinates (2-element array: x, y).
     */
    public static double[] computePixelFrom3dCoordinates(Axes axes, double[] coordinates) {
        DrawerVisitor currentVisitor;
        AxesDrawer axesDrawer;
        Transformation projection;
        Transformation projection2d;

        currentVisitor = DrawerVisitor.getVisitor(axes.getParentFrameOrFigure());
        boolean[] logFlags = { axes.getXAxisLogFlag(), axes.getYAxisLogFlag(), axes.getZAxisLogFlag()};
        double[][] factors = axes.getScaleTranslateFactors();

        Vector3d point = new Vector3d(coordinates);
        point = ScaleUtils.applyLogScale(point, logFlags);
        double[] coords = point.getData();

        if (currentVisitor != null) {
            axesDrawer = currentVisitor.getAxesDrawer();

            coords[0] = coords[0] * factors[0][0] + factors[1][0];
            coords[1] = coords[1] * factors[0][1] + factors[1][1];
            coords[2] = coords[2] * factors[0][2] + factors[1][2];

            projection = axesDrawer.computeProjection(axes, currentVisitor.getDrawingTools(), currentVisitor.getCanvas().getDimension(), false);

            point = new Vector3d(coords);
            point = projection.project(point);
        }

        return new double[] {point.getX(), currentVisitor.getCanvas().getHeight() - point.getY(), point.getZ()};
    }

    /**
     * Computes and returns the pixel coordinates from a point's coordinates expressed in the current
     * 3d view coordinate frame, using the given Axes. The returned pixel coordinates are expressed
     * in the AWT's 2d coordinate frame.
     * @param axes the given Axes.
     * @param coordinates the 3d view coordinates (3-element array: x, y, z).
     * @returns the pixel coordinates (2-element array: x, y).
     */
    public static double[][] computePixelFrom3dCoordinates(Axes axes, double[] coordsX, double[] coordsY, double[] coordsZ) {
        DrawerVisitor currentVisitor = DrawerVisitor.getVisitor(axes.getParentFrameOrFigure());

        if (currentVisitor != null) {
            AxesDrawer axesDrawer = currentVisitor.getAxesDrawer();
            Dimension canvasDimension = currentVisitor.getCanvas().getDimension();
            double height = canvasDimension.getHeight();
            boolean[] logFlags = { axes.getXAxisLogFlag(), axes.getYAxisLogFlag(), axes.getZAxisLogFlag()};
            double[][] factors = axes.getScaleTranslateFactors();
            double[] coords = new double[3];
            double[][] ret = new double[coordsX.length][2];
            Transformation projection = axesDrawer.computeProjection(axes, currentVisitor.getDrawingTools(), canvasDimension, false);

            for (int i = 0; i < coordsX.length; i++) {
                coords[0] = coordsX[i];
                coords[1] = coordsY[i];
                coords[2] = coordsZ[i];
                ScaleUtils.applyLogScale(coords, logFlags);

                coords[0] = coords[0] * factors[0][0] + factors[1][0];
                coords[1] = coords[1] * factors[0][1] + factors[1][1];
                coords[2] = coords[2] * factors[0][2] + factors[1][2];

                Vector3d point = new Vector3d(coords);
                point = projection.project(point);
                ret[i][0] = point.getX();
                ret[i][1] = height - point.getY();
            }

            return ret;
        }

        return null;
    }

    /**
     * Computes and returns the coordinates of a point onto the 3d view plane.
     * To compute them, the point is projected using the object to window coordinate projection, then
     * unprojected using the object to window coordinate projection corresponding to the 3d view
     * @param axes the given Axes.
     * @param coordinates the object (x,y,z) coordinates to project onto the 2d view plane (3-element array).
     * @returns the 3d view coordinates (3-element array).
     */
    public static double[] compute3dViewCoordinates(Axes axes, double[] coordinates) {
        DrawerVisitor currentVisitor = DrawerVisitor.getVisitor(axes.getParentFrameOrFigure());
        AxesDrawer axesDrawer;
        Transformation projection;
        Transformation projection2d;
        double[] coords = coordinates;

        if (currentVisitor != null) {
            if (axes.getViewAsEnum() == ViewType.VIEW_2D) {
                // No need to projet/unproject since the product is identity
                return new double[] {coords[0], coords[1], coords[2]};
            }

            boolean[] logFlags = { axes.getXAxisLogFlag(), axes.getYAxisLogFlag(), axes.getZAxisLogFlag()};
            Dimension canvasDimension = currentVisitor.getCanvas().getDimension();
            double[][] factors = axes.getScaleTranslateFactors();
            ScaleUtils.applyLogScale(coords, logFlags);

            axesDrawer = currentVisitor.getAxesDrawer();
            coords[0] = coords[0] * factors[0][0] + factors[1][0];
            coords[1] = coords[1] * factors[0][1] + factors[1][1];
            coords[2] = coords[2] * factors[0][2] + factors[1][2];

            projection = axesDrawer.computeProjection(axes, currentVisitor.getDrawingTools(), canvasDimension, false);
            projection2d = axesDrawer.computeProjection(axes, currentVisitor.getDrawingTools(), canvasDimension, true);

            Vector3d point = new Vector3d(coords);
            point = projection2d.project(point);
            point = projection.unproject(point);

            coords = point.getData();
            coords[0] = (coords[0] - factors[1][0]) / factors[0][0];
            coords[1] = (coords[1] - factors[1][1]) / factors[0][1];
            coords[2] = (coords[2] - factors[1][2]) / factors[0][2];

            ScaleUtils.applyInverseLogScale(coords, logFlags);
        }

        return coords;
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
        // used by xgetmouse and by xchange

        DrawerVisitor currentVisitor;
        AxesDrawer axesDrawer;
        double[] coords2dView = new double[] {0.0, 0.0, 0.0};

        currentVisitor = DrawerVisitor.getVisitor(axes.getParentFrameOrFigure());

        if (currentVisitor != null) {
            boolean[] logFlags = { axes.getXAxisLogFlag(), axes.getYAxisLogFlag(), axes.getZAxisLogFlag()};
            double[][] factors = axes.getScaleTranslateFactors();

            axesDrawer = currentVisitor.getAxesDrawer();

            /* Convert the pixel coordinates to window coordinates, only y changes due to the differing y-axis convention */
            Vector3d point = new Vector3d(coordinates[0], currentVisitor.getCanvas().getHeight() - coordinates[1], 0.0);

            Transformation projection2d = axesDrawer.getProjection2dView(axes.getIdentifier());
            if (projection2d == null) {
                updateAxesTransformation(axes);
                projection2d = axesDrawer.getProjection2dView(axes.getIdentifier());
            }

            point = projection2d.unproject(point);
            coords2dView = point.getData();

            coords2dView[0] = (coords2dView[0] - factors[1][0]) / factors[0][0];
            coords2dView[1] = (coords2dView[1] - factors[1][1]) / factors[0][1];
            coords2dView[2] = (coords2dView[2] - factors[1][2]) / factors[0][2];

            ScaleUtils.applyInverseLogScale(coords2dView, logFlags);
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
        DrawerVisitor currentVisitor = DrawerVisitor.getVisitor(axes.getParentFrameOrFigure());

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

        currentVisitor = DrawerVisitor.getVisitor(axes.getParentFrameOrFigure());

        if (currentVisitor != null) {
            double width = currentVisitor.getCanvas().getDimension().getWidth();
            double height = currentVisitor.getCanvas().getDimension().getHeight();
            double upperLeftY;
            AxesDrawer axesDrawer = currentVisitor.getAxesDrawer();
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
                Double[] bounds = parentAxes.getCorrectedBounds();

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

                double[][] factors = parentAxes.getScaleTranslateFactors();
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

                clipBounds[0] = clipBounds[0] * factors[0][0] + factors[1][0];
                clipBounds[1] = clipBounds[1] * factors[0][0] + factors[1][0];
                clipBounds[2] = clipBounds[2] * factors[0][1] + factors[1][1];
                clipBounds[3] = clipBounds[3] * factors[0][1] + factors[1][1];

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
    public AxisLabelPositioner getXAxisLabelPositioner(Axes axes) {
        AxisLabelPositioner positioner = this.xAxisLabelPositioner.get(axes.getIdentifier());
        if (positioner == null) {
            positioner = new AxisLabelPositioner();
            this.xAxisLabelPositioner.put(axes.getIdentifier(), positioner);
        }

        return positioner;
    }

    /**
     * Returns the y-axis label positioner.
     * @return the y-axis label positioner.
     */
    public AxisLabelPositioner getYAxisLabelPositioner(Axes axes) {
        AxisLabelPositioner positioner = this.yAxisLabelPositioner.get(axes.getIdentifier());
        if (positioner == null) {
            positioner = new YAxisLabelPositioner();
            this.yAxisLabelPositioner.put(axes.getIdentifier(), positioner);
        }

        return positioner;
    }

    /**
     * Returns the z-axis label positioner.
     * @return the z-axis label positioner.
     */
    public AxisLabelPositioner getZAxisLabelPositioner(Axes axes) {
        AxisLabelPositioner positioner = this.zAxisLabelPositioner.get(axes.getIdentifier());
        if (positioner == null) {
            positioner = new AxisLabelPositioner();
            this.zAxisLabelPositioner.put(axes.getIdentifier(), positioner);
        }

        return positioner;
    }

    /**
     * Returns the title positioner.
     * @return the title positioner.
     */
    public TitlePositioner getTitlePositioner(Axes axes) {
        TitlePositioner positioner = this.titlePositioner.get(axes.getIdentifier());
        if (positioner == null) {
            positioner = new TitlePositioner();
            this.titlePositioner.put(axes.getIdentifier(), positioner);
        }

        return positioner;
    }

    public void disposeAll() {
        this.rulerDrawer.disposeAll();
        this.projectionMap.clear();
        this.projection2dViewMap.clear();
        this.sceneProjectionMap.clear();
        this.xAxisLabelPositioner.clear();
        this.yAxisLabelPositioner.clear();
        this.zAxisLabelPositioner.clear();
        this.titlePositioner.clear();
    }

    public void update(Integer id, int property) {
        if (this.rulerDrawer.update(id, property)) {
            GraphicObject object = GraphicController.getController().getObjectFromId(id);
            if (object instanceof Axes) {
                computeRulers((Axes) object);
            }
        }
    }

    public void dispose(Integer id) {
        this.rulerDrawer.dispose(id);
        projectionMap.remove(id);
        projection2dViewMap.remove(id);
        sceneProjectionMap.remove(id);
        this.xAxisLabelPositioner.remove(id);
        this.yAxisLabelPositioner.remove(id);
        this.zAxisLabelPositioner.remove(id);
        this.titlePositioner.remove(id);
    }
}
