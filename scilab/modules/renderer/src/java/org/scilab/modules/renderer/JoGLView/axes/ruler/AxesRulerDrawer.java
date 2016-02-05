/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 * Copyright (C) 2011 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2013-2014 - Scilab Enterprises - Calixte DENIZET
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

package org.scilab.modules.renderer.JoGLView.axes.ruler;

import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.SciRendererException;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.ruler.DefaultRulerModel;
import org.scilab.forge.scirenderer.ruler.RulerDrawer;
import org.scilab.forge.scirenderer.ruler.RulerDrawingResult;
import org.scilab.forge.scirenderer.ruler.RulerModel;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.geometry.DefaultGeometry;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.tranformations.DegenerateMatrixException;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.TransformationFactory;
import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.axes.AxisProperty;
import org.scilab.modules.graphic_objects.axes.Camera;
import org.scilab.modules.graphic_objects.contouredObject.Line;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.renderer.JoGLView.axes.AxesDrawer;
import org.scilab.modules.renderer.JoGLView.label.AxisLabelPositioner;
import org.scilab.modules.renderer.JoGLView.label.TitlePositioner;
import org.scilab.modules.renderer.JoGLView.util.ColorFactory;

import java.awt.Dimension;
import java.nio.FloatBuffer;
import java.text.DecimalFormat;
import java.util.Arrays;
import java.util.List;

/**
 * @author Pierre Lando
 */
public class AxesRulerDrawer {

    private static final double LINEAR_MINIMAL_SUB_TICKS_DISTANCE = 8;
    private static final double LOG_MINIMAL_SUB_TICKS_DISTANCE = 2;

    /** Ticks length in pixels. */
    public static final int TICKS_LENGTH = 6;

    /** Sub-ticks length in pixels. */
    private static final int SUB_TICKS_LENGTH = 3;

    /**Ticks sprites distance in pixels. */
    private static final int SPRITE_DISTANCE = 12;


    private final RulerDrawerManager rulerDrawerManager;

    public AxesRulerDrawer(Canvas canvas) {
        this.rulerDrawerManager = new RulerDrawerManager(canvas.getTextureManager());
    }

    public RulerDrawer getRulerDrawer(Axes axes, int axis) {
        return rulerDrawerManager.get(axes)[axis];
    }

    /**
     * Get default ruler model
     * @param axes the axes
     * @param colorMap the colorMap
     * @return a DefaultRulerModel
     */
    private final DefaultRulerModel getDefaultRulerModel(Axes axes, ColorMap colorMap) {
        DefaultRulerModel rulerModel = new DefaultRulerModel();
        rulerModel.setTicksLength(TICKS_LENGTH);
        rulerModel.setSubTicksLength(SUB_TICKS_LENGTH);
        rulerModel.setLineWidth(axes.getLineThickness());
        rulerModel.setSpriteDistance(SPRITE_DISTANCE);
        rulerModel.setColor(ColorFactory.createColor(colorMap, axes.getLineColor()));

        return rulerModel;
    }

    /**
     * Compute ticks and subticks on the rulers
     * @param axes the current {@see Axes}
     * @param axesDrawer the drawer used to draw the current {@see Axes}
     * @param colorMap current {@see ColorMap}
     * @param transformation the current modelView projection
     * @param canvasProjection the canvas projection
     * @throws org.scilab.forge.scirenderer.SciRendererException if draw fail.
     */
    public void computeRulers(Axes axes, AxesDrawer axesDrawer, ColorMap colorMap, Transformation transformation, Transformation canvasProjection) {
        Double[] bounds = axes.getDisplayedBounds();
        double[] matrix = transformation.getMatrix();

        RulerDrawingResult rulerDrawingResult;
        double[] values;

        RulerDrawer[] rulerDrawers = rulerDrawerManager.get(axes);
        DefaultRulerModel rulerModel = getDefaultRulerModel(axes, colorMap);

        Vector3d xAxisPosition = computeXAxisPosition(matrix, bounds, axes.getXAxis().getAxisLocation(), axes.getYAxis().getReverse());
        Vector3d yAxisPosition = computeYAxisPosition(matrix, bounds, axes.getYAxis().getAxisLocation(), axes.getXAxis().getReverse());

        Vector3d px = canvasProjection.projectDirection(new Vector3d(1, 0, 0)).setZ(0);
        Vector3d py = canvasProjection.projectDirection(new Vector3d(0, 1, 0)).setZ(0);
        Vector3d pz = canvasProjection.projectDirection(new Vector3d(0, 0, 1)).setZ(0);

        Vector3d xTicksDirection, yTicksDirection;
        if (py.getNorm2() > pz.getNorm2()) {
            xTicksDirection = new Vector3d(0, getNonZeroSignum(xAxisPosition.getY()), 0);
        } else {
            xTicksDirection = new Vector3d(0, 0, getNonZeroSignum(xAxisPosition.getZ()));
        }

        if (px.getNorm2() > pz.getNorm2()) {
            yTicksDirection = new Vector3d(getNonZeroSignum(yAxisPosition.getX()), 0, 0);
        } else {
            yTicksDirection = new Vector3d(0, 0, getNonZeroSignum(yAxisPosition.getZ()));
        }

        // Draw X ruler
        rulerModel.setTicksDirection(xTicksDirection);
        rulerModel.setFirstPoint(xAxisPosition.setX(-1));
        rulerModel.setSecondPoint(xAxisPosition.setX(1));
        if (!axes.getAutoSubticks()) {
            rulerModel.setSubticksNumber(axes.getXAxisSubticks());
        }

        setRulerBounds(axes.getXAxis(), rulerModel, bounds[0], bounds[1]);

        rulerModel.setFormat(axes.getXAxisFormat());
        rulerModel.setSTFactors(axes.getXAxisSTFactors());
        rulerModel.setLogarithmic(axes.getXAxis().getLogFlag());
        rulerModel.setMinimalSubTicksDistance(axes.getXAxis().getLogFlag() ? LOG_MINIMAL_SUB_TICKS_DISTANCE : LINEAR_MINIMAL_SUB_TICKS_DISTANCE);

        if (!axes.getXAxis().getAutoTicks()) {
            rulerModel.setUserGraduation(new UserDefineGraduation(axes.getXAxis(), bounds[0], bounds[1]));
            if (axes.getAutoSubticks()) {
                rulerModel.setSubticksNumber(axes.getXAxisSubticks());
            }
            rulerModel.setAutoTicks(false);
        } else {
            rulerModel.setAutoTicks(true);
        }

        double distanceRatio;
        AxisLabelPositioner xAxisLabelPositioner = axesDrawer.getXAxisLabelPositioner(axes);
        xAxisLabelPositioner.setLabelPosition(xAxisPosition);

        if (axes.getXAxisVisible()) {
            rulerDrawingResult = rulerDrawers[0].computeRuler(rulerModel, canvasProjection);
            values = rulerDrawingResult.getTicksValues();

            if (axes.getXAxisAutoTicks()) {
                Arrays.sort(values);
                GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_X_AXIS_TICKS_LOCATIONS__, toDoubleArray(values));
                GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_X_AXIS_TICKS_LABELS__, toStringArray(values, rulerDrawingResult.getFormat()));
                if (axes.getAutoSubticks()) {
                    //GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_X_AXIS_SUBTICKS__, rulerDrawingResult.getSubTicksDensity());
                    axes.setXAxisSubticks(rulerDrawingResult.getSubTicksDensity());
                }
            }

            distanceRatio = rulerDrawingResult.getMaxDistToTicksDirNorm();

            xAxisLabelPositioner.setTicksDirection(xTicksDirection);
            xAxisLabelPositioner.setDistanceRatio(distanceRatio);
            xAxisLabelPositioner.setProjectedTicksDirection(rulerDrawingResult.getNormalizedTicksDirection().setZ(0));
        } else {
            xAxisLabelPositioner.setTicksDirection(xTicksDirection);
            Vector3d projTicksDir = canvasProjection.projectDirection(xTicksDirection);
            xAxisLabelPositioner.setDistanceRatio((double) TICKS_LENGTH / projTicksDir.getNorm());
            xAxisLabelPositioner.setProjectedTicksDirection(projTicksDir.getNormalized().setZ(0));
        }

        TitlePositioner titlePositioner = axesDrawer.getTitlePositioner(axes);
        Dimension xdim = axesDrawer.getLabelManager().getXLabelSize(colorMap, axes, axesDrawer);
        titlePositioner.setXLabelHeight(xdim.height);
        titlePositioner.setDistanceRatio(xAxisLabelPositioner.getDistanceRatio());

        // Draw Y ruler
        rulerModel = getDefaultRulerModel(axes, colorMap);
        rulerModel.setTicksDirection(yTicksDirection);
        rulerModel.setFirstPoint(yAxisPosition.setY(-1));
        rulerModel.setSecondPoint(yAxisPosition.setY(1));
        if (!axes.getAutoSubticks()) {
            rulerModel.setSubticksNumber(axes.getYAxisSubticks());
        }

        setRulerBounds(axes.getYAxis(), rulerModel, bounds[2], bounds[3]);
        rulerModel.setFormat(axes.getYAxisFormat());
        rulerModel.setSTFactors(axes.getYAxisSTFactors());
        rulerModel.setLogarithmic(axes.getYAxis().getLogFlag());
        rulerModel.setMinimalSubTicksDistance(axes.getYAxis().getLogFlag() ? LOG_MINIMAL_SUB_TICKS_DISTANCE : LINEAR_MINIMAL_SUB_TICKS_DISTANCE);

        if (!axes.getYAxis().getAutoTicks()) {
            rulerModel.setUserGraduation(new UserDefineGraduation(axes.getYAxis(), bounds[2], bounds[3]));
            if (axes.getAutoSubticks()) {
                rulerModel.setSubticksNumber(axes.getYAxisSubticks());
            }
            rulerModel.setAutoTicks(false);
        } else {
            rulerModel.setAutoTicks(true);
        }

        AxisLabelPositioner yAxisLabelPositioner = axesDrawer.getYAxisLabelPositioner(axes);
        yAxisLabelPositioner.setLabelPosition(yAxisPosition);

        if (axes.getYAxisVisible()) {
            rulerDrawingResult = rulerDrawers[1].computeRuler(rulerModel, canvasProjection);
            values = rulerDrawingResult.getTicksValues();
            if (axes.getYAxisAutoTicks()) {
                Arrays.sort(values);
                GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_Y_AXIS_TICKS_LOCATIONS__, toDoubleArray(values));
                GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_Y_AXIS_TICKS_LABELS__, toStringArray(values, rulerDrawingResult.getFormat()));
                if (axes.getAutoSubticks()) {
                    //GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_Y_AXIS_SUBTICKS__, rulerDrawingResult.getSubTicksDensity());
                    axes.setYAxisSubticks(rulerDrawingResult.getSubTicksDensity());
                }
            }

            distanceRatio = rulerDrawingResult.getMaxDistToTicksDirNorm();

            yAxisLabelPositioner.setTicksDirection(yTicksDirection);
            yAxisLabelPositioner.setDistanceRatio(distanceRatio);
            yAxisLabelPositioner.setProjectedTicksDirection(rulerDrawingResult.getNormalizedTicksDirection().setZ(0));
        } else {
            /* y-axis not visible: compute the projected ticks direction and distance ratio (see the x-axis case). */
            yAxisLabelPositioner.setTicksDirection(yTicksDirection);
            Vector3d projTicksDir = canvasProjection.projectDirection(yTicksDirection);
            yAxisLabelPositioner.setDistanceRatio((double) TICKS_LENGTH / projTicksDir.getNorm());
            yAxisLabelPositioner.setProjectedTicksDirection(projTicksDir.getNormalized().setZ(0));
        }

        // Draw Z ruler
        if (axes.getViewAsEnum() == Camera.ViewType.VIEW_3D) {
            double txs, tys, xs, ys;
            if (Math.abs(matrix[2]) < Math.abs(matrix[6])) {
                xs = matrix[2] > 0 ? 1 : -1;
                ys = matrix[6] > 0 ? -1 : 1;
                txs = xs;
                tys = 0;
            } else {
                xs = matrix[2] > 0 ? -1 : 1;
                ys = matrix[6] > 0 ? 1 : -1;
                txs = 0;
                tys = ys;
            }

            rulerModel = getDefaultRulerModel(axes, colorMap);
            rulerModel.setFirstPoint(new Vector3d(xs, ys, -1));
            rulerModel.setSecondPoint(new Vector3d(xs, ys, 1));
            rulerModel.setTicksDirection(new Vector3d(txs, tys, 0));
            if (!axes.getAutoSubticks()) {
                rulerModel.setSubticksNumber(axes.getZAxisSubticks());
            }

            setRulerBounds(axes.getZAxis(), rulerModel, bounds[4], bounds[5]);
            rulerModel.setFormat(axes.getZAxisFormat());
            rulerModel.setSTFactors(axes.getZAxisSTFactors());
            rulerModel.setLogarithmic(axes.getZAxis().getLogFlag());
            rulerModel.setMinimalSubTicksDistance(axes.getZAxis().getLogFlag() ? LOG_MINIMAL_SUB_TICKS_DISTANCE : LINEAR_MINIMAL_SUB_TICKS_DISTANCE);

            if (!axes.getZAxis().getAutoTicks()) {
                rulerModel.setUserGraduation(new UserDefineGraduation(axes.getZAxis(), bounds[4], bounds[5]));
                if (axes.getAutoSubticks()) {
                    rulerModel.setSubticksNumber(axes.getZAxisSubticks());
                }
                rulerModel.setAutoTicks(false);
            } else {
                rulerModel.setAutoTicks(true);
            }

            AxisLabelPositioner zAxisLabelPositioner = axesDrawer.getZAxisLabelPositioner(axes);
            zAxisLabelPositioner.setLabelPosition(new Vector3d(xs, ys, 0));

            if (axes.getZAxisVisible()) {
                rulerDrawingResult = rulerDrawers[2].computeRuler(rulerModel, canvasProjection);
                values = rulerDrawingResult.getTicksValues();
                if (axes.getZAxisAutoTicks()) {
                    Arrays.sort(values);
                    GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_Z_AXIS_TICKS_LOCATIONS__, toDoubleArray(values));
                    GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_Z_AXIS_TICKS_LABELS__, toStringArray(values, rulerDrawingResult.getFormat()));
                    if (axes.getAutoSubticks()) {
                        //GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_Z_AXIS_SUBTICKS__, rulerDrawingResult.getSubTicksDensity());
                        axes.setZAxisSubticks(rulerDrawingResult.getSubTicksDensity());
                    }
                }

                distanceRatio = rulerDrawingResult.getMaxDistToTicksDirNorm();

                zAxisLabelPositioner.setTicksDirection(new Vector3d(txs, tys, 0.0));
                zAxisLabelPositioner.setDistanceRatio(distanceRatio);
                zAxisLabelPositioner.setProjectedTicksDirection(rulerDrawingResult.getNormalizedTicksDirection().setZ(0).setY(1e-7));
            } else {
                /* z-axis not visible: compute the projected ticks direction and distance ratio (see the x-axis case). */
                Vector3d zTicksDirection = new Vector3d(txs, tys, 0);

                zAxisLabelPositioner.setTicksDirection(zTicksDirection);
                Vector3d projTicksDir = canvasProjection.projectDirection(zTicksDirection);
                zAxisLabelPositioner.setDistanceRatio((double) TICKS_LENGTH / projTicksDir.getNorm());
                zAxisLabelPositioner.setProjectedTicksDirection(projTicksDir.getNormalized().setZ(0));
            }
        }
    }

    /**
     * Draw the ruler.
     * @param axes the current {@see Axes}
     * @param axesDrawer the drawer used to draw the current {@see Axes}
     * @param colorMap current {@see ColorMap}
     * @param drawingTools the used {@see DrawingTools}
     * @throws org.scilab.forge.scirenderer.SciRendererException if draw fail.
     */
    public void drawRuler(Axes axes, AxesDrawer axesDrawer, ColorMap colorMap, DrawingTools drawingTools) throws SciRendererException {
        Appearance gridAppearance = new Appearance();

        Double[] bounds = axes.getDisplayedBounds();
        double[] matrix = drawingTools.getTransformationManager().getModelViewStack().peek().getMatrix();

        RulerDrawer[] rulerDrawers = rulerDrawerManager.get(axes);
        ElementsBuffer vertexBuffer = drawingTools.getCanvas().getBuffersManager().createElementsBuffer();
        final boolean is3D = axes.getViewAsEnum() == Camera.ViewType.VIEW_3D;// && axes.getRotationAngles()[1] != 90.0;

        if (rulerDrawers[0].getModel() == null || rulerDrawers[1].getModel() == null || (is3D && rulerDrawers[2].getModel() == null)) {
            computeRulers(axes, axesDrawer, colorMap, drawingTools.getTransformationManager().getModelViewStack().peek(), drawingTools.getTransformationManager().getCanvasProjection());
        }

        int gridPosition;
        if (axes.getGridPositionAsEnum().equals(Axes.GridPosition.FOREGROUND)) {
            gridPosition = 1;
        } else {
            gridPosition = -1;
        }

        // Draw X ruler
        if (axes.getXAxisVisible()) {
            rulerDrawers[0].draw(drawingTools);

            if (axes.getXAxisGridColor() != -1) {
                FloatBuffer vertexData;
                if (axes.getXAxisLogFlag()) {
                    List<Double> values = rulerDrawers[0].getSubTicksValue();
                    if (values == null || values.isEmpty()) {
                        vertexData = getXGridData(rulerDrawers[0].getTicksValue(), rulerDrawers[0].getModel());
                    } else {
                        vertexData = getXGridData(values, rulerDrawers[0].getModel());
                    }
                } else {
                    vertexData = getXGridData(rulerDrawers[0].getTicksValue(), rulerDrawers[0].getModel());
                }
                vertexBuffer.setData(vertexData, 4);

                Transformation mirror;
                try {
                    mirror = TransformationFactory.getScaleTransformation(
                                 1,
                                 matrix[6] < 0 ? gridPosition : -gridPosition,
                                 matrix[10] < 0 ? gridPosition : -gridPosition
                             );
                } catch (DegenerateMatrixException ignored) {
                    // Should never happens as long as gridPosition the value 1 or -1
                    mirror = TransformationFactory.getIdentity();
                }

                gridAppearance.setLineColor(ColorFactory.createColor(colorMap, axes.getXAxisGridColor()));
                gridAppearance.setLineWidth(axes.getXAxisGridThickness().floatValue());
                gridAppearance.setLinePattern(Line.LineType.fromScilabIndex(axes.getXAxisGridStyle()).asPattern());
                drawingTools.getTransformationManager().getModelViewStack().pushRightMultiply(mirror);
                DefaultGeometry gridGeometry = new DefaultGeometry();
                gridGeometry.setFillDrawingMode(Geometry.FillDrawingMode.NONE);
                gridGeometry.setLineDrawingMode(Geometry.LineDrawingMode.SEGMENTS);
                gridGeometry.setVertices(vertexBuffer);
                drawingTools.draw(gridGeometry, gridAppearance);
                drawingTools.getTransformationManager().getModelViewStack().pop();
            }
        }

        // Draw Y ruler
        if (axes.getYAxisVisible()) {
            rulerDrawers[1].draw(drawingTools);

            if (axes.getYAxisGridColor() != -1) {
                FloatBuffer vertexData;
                if (axes.getYAxisLogFlag()) {
                    List<Double> values = rulerDrawers[1].getSubTicksValue();
                    if (values == null || values.isEmpty()) {
                        vertexData = getYGridData(rulerDrawers[1].getTicksValue(), rulerDrawers[1].getModel());
                    } else {
                        vertexData = getYGridData(values, rulerDrawers[1].getModel());
                    }
                } else {
                    vertexData = getYGridData(rulerDrawers[1].getTicksValue(), rulerDrawers[1].getModel());
                }
                vertexBuffer.setData(vertexData, 4);

                Transformation mirror;
                try {
                    mirror = TransformationFactory.getScaleTransformation(
                                 matrix[2] < 0 ? gridPosition : -gridPosition,
                                 1,
                                 matrix[10] < 0 ? gridPosition : -gridPosition
                             );
                } catch (DegenerateMatrixException ignored) {
                    // Should never happens as long as gridPosition the value 1 or -1
                    mirror = TransformationFactory.getIdentity();
                }

                gridAppearance.setLineColor(ColorFactory.createColor(colorMap, axes.getYAxisGridColor()));
                gridAppearance.setLineWidth(axes.getYAxisGridThickness().floatValue());
                gridAppearance.setLinePattern(Line.LineType.fromScilabIndex(axes.getYAxisGridStyle()).asPattern());
                drawingTools.getTransformationManager().getModelViewStack().pushRightMultiply(mirror);
                DefaultGeometry gridGeometry = new DefaultGeometry();
                gridGeometry.setFillDrawingMode(Geometry.FillDrawingMode.NONE);
                gridGeometry.setLineDrawingMode(Geometry.LineDrawingMode.SEGMENTS);
                gridGeometry.setVertices(vertexBuffer);
                drawingTools.draw(gridGeometry, gridAppearance);
                drawingTools.getTransformationManager().getModelViewStack().pop();
            }
        }

        // Draw Z ruler
        if (is3D) {
            if (axes.getZAxisVisible()) {
                rulerDrawers[2].draw(drawingTools);

                if (axes.getZAxisGridColor() != -1) {
                    FloatBuffer vertexData;
                    if (axes.getZAxisLogFlag()) {
                        List<Double> values = rulerDrawers[2].getSubTicksValue();
                        if (values == null || values.isEmpty()) {
                            vertexData = getZGridData(rulerDrawers[2].getTicksValue(), rulerDrawers[2].getModel());
                        } else {
                            vertexData = getZGridData(values, rulerDrawers[2].getModel());
                        }
                    } else {
                        vertexData = getZGridData(rulerDrawers[2].getTicksValue(), rulerDrawers[2].getModel());
                    }
                    vertexBuffer.setData(vertexData, 4);

                    Transformation mirror;
                    try {
                        mirror = TransformationFactory.getScaleTransformation(
                                     matrix[2] < 0 ? gridPosition : -gridPosition,
                                     matrix[6] < 0 ? gridPosition : -gridPosition,
                                     1
                                 );
                    } catch (DegenerateMatrixException ignored) {
                        // Should never happens as long as gridPosition the value 1 or -1
                        mirror = TransformationFactory.getIdentity();
                    }

                    gridAppearance.setLineColor(ColorFactory.createColor(colorMap, axes.getZAxisGridColor()));
                    gridAppearance.setLineWidth(axes.getZAxisGridThickness().floatValue());
                    gridAppearance.setLinePattern(Line.LineType.fromScilabIndex(axes.getZAxisGridStyle()).asPattern());
                    drawingTools.getTransformationManager().getModelViewStack().pushRightMultiply(mirror);
                    DefaultGeometry gridGeometry = new DefaultGeometry();
                    gridGeometry.setFillDrawingMode(Geometry.FillDrawingMode.NONE);
                    gridGeometry.setLineDrawingMode(Geometry.LineDrawingMode.SEGMENTS);
                    gridGeometry.setVertices(vertexBuffer);
                    drawingTools.draw(gridGeometry, gridAppearance);
                    drawingTools.getTransformationManager().getModelViewStack().pop();
                }
            }
        }

        drawingTools.getCanvas().getBuffersManager().dispose(vertexBuffer);
    }

    private static final double getNonZeroSignum(double value) {
        if (value < 0) {
            return -1;
        } else {
            return 1;
        }
    }

    private void setRulerBounds(AxisProperty axis, DefaultRulerModel rulerModel, double axisMin, double axisMax) {
        double min, max;
        if (axis.getReverse()) {
            min = axisMin;
            max = axisMax;
        } else {
            min = axisMax;
            max = axisMin;
        }

        if (axis.getLogFlag()) {
            min = Math.pow(10, min);
            max = Math.pow(10, max);
        }
        rulerModel.setValues(min, max);
    }

    private Vector3d computeXAxisPosition(double[] projectionMatrix, Double[] bounds, AxisProperty.AxisLocation axisLocation, boolean reverse) {
        double y, z;
        switch (axisLocation) {
            default:
            case BOTTOM:
                z = -Math.signum(projectionMatrix[9]);  // First : switch Z such that Y was minimal.
                y = -Math.signum(projectionMatrix[6]) * z * Math.signum(projectionMatrix[10]);
                if (y == 0) {
                    y = +1;
                }
                break;
            case MIDDLE:
                z = Math.signum(projectionMatrix[9]);  // First : switch Z such that Y was maximal.
                y = 0;
                break;
            case TOP:
                z = Math.signum(projectionMatrix[9]);  // First : switch Z such that Y was maximal.
                y = -Math.signum(projectionMatrix[6]) * z * Math.signum(projectionMatrix[10]);
                if (y == 0) {
                    y = -1;
                }
                break;
            case ORIGIN:
                z = Math.signum(projectionMatrix[9]);  // First : switch Z such that Y was maximal.
                y = (reverse ? -1 : 1) * (bounds[3] + bounds[2]) / (bounds[3] - bounds[2]);
                if (Math.abs(y) > 1) {
                    y = Math.signum(y);
                }
                break;
        }
        return new Vector3d(0, y, z);
    }

    private Vector3d computeYAxisPosition(double[] matrix, Double[] bounds, AxisProperty.AxisLocation axisLocation, boolean reverse) {
        double x, z;
        switch (axisLocation) {
            default:
            case LEFT:
                z = -Math.signum(matrix[9]);  // First : switch Z such that Y was minimal.
                x = -Math.signum(matrix[2]) * z * Math.signum(matrix[10]);
                if (x == 0) {
                    x = +1;
                }
                break;
            case MIDDLE:
                z = Math.signum(matrix[9]);  // First : switch Z such that Y was minimal.
                x = 0;
                break;
            case RIGHT:
                z = Math.signum(matrix[9]);  // First : switch Z such that Y was minimal.
                x = -Math.signum(matrix[2]) * z * Math.signum(matrix[10]); // Then switch X such that Z max but not in the middle.
                if (x == 0) {
                    x = -1;
                }
                break;
            case ORIGIN:
                z = Math.signum(matrix[9]);  // First : switch Z such that Y was minimal.
                x = (reverse ? -1 : 1) * (bounds[1] + bounds[0]) / (bounds[1] - bounds[0]);
                if (Math.abs(x) > 1) {
                    x = Math.signum(x);
                }
                break;
        }
        return new Vector3d(x, 0, z);
    }

    private String[] toStringArray(double[] values, DecimalFormat format) {
        AxesRulerSpriteFactory.setScilabStyle(format);
        String[] r = new String[values.length];
        for (int i = 0; i < values.length; i++) {
            r[i] = format.format(values[i]);
        }
        return r;
    }

    private Double[] toDoubleArray(double[] values) {
        Double[] r = new Double[values.length];
        for (int i = 0; i < values.length; i++) {
            r[i] = values[i];
        }
        return r;
    }

    /**
     * Build X grid data.
     * @param values X values where grid is drawn.
     * @param rulerModel used rulerModel to compute grid world position.
     * @return X grid data.
     */
    private FloatBuffer getXGridData(List<Double> values, RulerModel rulerModel) {
        FloatBuffer vertexData = FloatBuffer.allocate(values.size() * 16);
        for (double value : values) {
            float p = (float) rulerModel.getPosition(value).getX();
            vertexData.put(p);
            vertexData.put(+1);
            vertexData.put(+1);
            vertexData.put(1);
            vertexData.put(p);
            vertexData.put(-1);
            vertexData.put(+1);
            vertexData.put(1);
            vertexData.put(p);
            vertexData.put(+1);
            vertexData.put(+1);
            vertexData.put(1);
            vertexData.put(p);
            vertexData.put(+1);
            vertexData.put(-1);
            vertexData.put(1);
        }
        vertexData.rewind();
        return vertexData;
    }

    /**
     * Build Y grid data.
     * @param values Y values where grid is drawn.
     * @param rulerModel used rulerModel to compute grid world position.
     * @return Y grid data.
     */
    private FloatBuffer getYGridData(List<Double> values, RulerModel rulerModel) {
        FloatBuffer vertexData = FloatBuffer.allocate(values.size() * 16);
        for (double value : values) {
            float p = (float) rulerModel.getPosition(value).getY();
            vertexData.put(+1);
            vertexData.put(p);
            vertexData.put(+1);
            vertexData.put(1);
            vertexData.put(-1);
            vertexData.put(p);
            vertexData.put(+1);
            vertexData.put(1);
            vertexData.put(+1);
            vertexData.put(p);
            vertexData.put(+1);
            vertexData.put(1);
            vertexData.put(+1);
            vertexData.put(p);
            vertexData.put(-1);
            vertexData.put(1);
        }
        vertexData.rewind();
        return vertexData;
    }

    /**
     * Build Z grid data.
     * @param values Z values where grid is drawn.
     * @param rulerModel used rulerModel to compute grid world position.
     * @return Z grid data.
     */
    private FloatBuffer getZGridData(List<Double> values, RulerModel rulerModel) {
        FloatBuffer vertexData = FloatBuffer.allocate(values.size() * 16);
        int limit = 0;
        for (double value : values) {
            float p = (float) rulerModel.getPosition(value).getZ();
            vertexData.put(+1);
            vertexData.put(+1);
            vertexData.put(p);
            vertexData.put(1);
            vertexData.put(-1);
            vertexData.put(+1);
            vertexData.put(p);
            vertexData.put(1);
            vertexData.put(+1);
            vertexData.put(+1);
            vertexData.put(p);
            vertexData.put(1);
            vertexData.put(+1);
            vertexData.put(-1);
            vertexData.put(p);
            vertexData.put(1);
            limit += 16;
        }
        vertexData.limit(limit);
        return vertexData;
    }

    public void disposeAll() {
        this.rulerDrawerManager.disposeAll();
    }

    public boolean update(Integer id, int property) {
        return this.rulerDrawerManager.update(id, property);
    }

    public void dispose(Integer id) {
        this.rulerDrawerManager.dispose(id);
    }
}
