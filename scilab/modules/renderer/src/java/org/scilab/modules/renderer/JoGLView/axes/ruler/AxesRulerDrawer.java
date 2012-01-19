/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 * Copyright (C) 2011 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView.axes.ruler;

import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.ruler.RulerDrawer;
import org.scilab.forge.scirenderer.ruler.RulerDrawingResult;
import org.scilab.forge.scirenderer.ruler.RulerModel;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.shapes.geometry.GeometryImpl;
import org.scilab.forge.scirenderer.tranformations.DegenerateMatrixException;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.TransformationFactory;
import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.axes.AxisProperty;
import org.scilab.modules.graphic_objects.axes.Camera;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.renderer.JoGLView.axes.AxesDrawer;
import org.scilab.modules.renderer.JoGLView.label.AxisLabelPositioner;
import org.scilab.modules.renderer.JoGLView.util.ColorFactory;

import java.nio.FloatBuffer;

/**
 * @author Pierre Lando
 */
public class AxesRulerDrawer {

    private static final short GRID_LINE_PATTERN = (short) 0xF0F0;
    private static final double TICKS_SIZE = .1;

    private final RulerSpriteManagerSet rulerSpriteManagerSet;

    public AxesRulerDrawer(Canvas canvas) {
        this.rulerSpriteManagerSet = new RulerSpriteManagerSet(canvas.getSpriteManager());
    }

    /**
     * Draw the rulers and sets the axis label positioners' relevant values.
     * @param axes the current {@see Axes}
     * @param axesDrawer the drawer used to draw the current {@see Axes}
     * @param colorMap current {@see ColorMap}
     * @param drawingTools the used {@see DrawingTools}
     */
    public void drawRuler(Axes axes, AxesDrawer axesDrawer, ColorMap colorMap, DrawingTools drawingTools) {
        Double[] bounds = axes.computeDisplayedBounds();
        double[] matrix = drawingTools.getTransformationManager().getModelViewStack().peek().getMatrix();

        RulerDrawingResult rulerDrawingResult;
        double[] values;

        RulerDrawer rulerDrawer = new RulerDrawer(rulerSpriteManagerSet.get(axes));
        RulerModel rulerModel = new RulerModel();

        Transformation canvasProjection = drawingTools.getTransformationManager().getCanvasProjection();
        Vector3d px = canvasProjection.projectDirection(new Vector3d(1, 0, 0)).setZ(0);
        Vector3d py = canvasProjection.projectDirection(new Vector3d(0, 1, 0)).setZ(0);
        Vector3d pz = canvasProjection.projectDirection(new Vector3d(0, 0, 1)).setZ(0);

        // Draw X ruler

        ElementsBuffer vertexBuffer = drawingTools.getCanvas().getBuffersManager().createElementsBuffer();
        int gridPosition;
        if (axes.getGridPositionAsEnum().equals(Axes.GridPosition.FOREGROUND)) {
            gridPosition = 1;
        } else {
            gridPosition = -1;
        }

        Appearance gridAppearance = new Appearance();
        gridAppearance.setLinePattern(GRID_LINE_PATTERN);


        double zs;
        double ys;
        double xs;

        if (axes.getAxes()[0].getAxisLocation().equals(AxisProperty.AxisLocation.BOTTOM)) {
            zs = -Math.signum(matrix[9]);  // First : switch Z such that Y was minimal.
            ys = -Math.signum(matrix[6]) * zs * Math.signum(matrix[10]);
            if (ys == 0) {
                ys = +1;
            }
        } else {
            zs = Math.signum(matrix[9]);  // First : switch Z such that Y was maximal.
            ys = -Math.signum(matrix[6]) * zs * Math.signum(matrix[10]);
            if (ys == 0) {
                ys = -1;
            }
        }


        Vector3d xTicksDirection;
        if (py.getNorm2() > pz.getNorm2()) {
            xTicksDirection = new Vector3d(0, TICKS_SIZE * ys, 0);
        } else {
            xTicksDirection = new Vector3d(0, 0, TICKS_SIZE * zs);
        }
        rulerModel.setTicksDirection(xTicksDirection);

        rulerModel.setFirstPoint(new Vector3d(1, ys, zs));
        rulerModel.setSecondPoint(new Vector3d(-1, ys, zs));

        if (axes.getAxes()[0].getReverse()) {
            rulerModel.setValues(bounds[0], bounds[1]);
        } else {
            rulerModel.setValues(bounds[1], bounds[0]);
        }

        double [] midpoints = new double[]{0.0, 0.0, 0.0};
        double [] otherbounds = new double[2];

        otherbounds[0] = (ys > 0) ? +1 : -1;
        otherbounds[1] = (zs > 0) ? +1 : -1;

        Vector3d labelPosition = new Vector3d(midpoints[0], otherbounds[0], otherbounds[1]);
        double distanceRatio;

        AxisLabelPositioner xAxisLabelPositioner = (AxisLabelPositioner) axesDrawer.getXAxisLabelPositioner();
        xAxisLabelPositioner.setLabelPosition(labelPosition);

        if (axes.getXAxisVisible()) {
            rulerDrawingResult = rulerDrawer.draw(drawingTools, rulerModel);
            values = rulerDrawingResult.getTicksValues();
            axes.setXAxisTicksLocations(toDoubleArray(values));
            axes.setXAxisTicksLabels(toStringArray(values));
            axes.setXAxisSubticks(rulerDrawingResult.getSubTicksDensity() - 1);

            distanceRatio  = rulerDrawingResult.getMaxDistToTicksDirNorm();
            double [] xticksdir = rulerDrawingResult.getNormalizedTicksDirection();

            xAxisLabelPositioner.setTicksDirection(xTicksDirection);
            xAxisLabelPositioner.setDistanceRatio(distanceRatio);
            xAxisLabelPositioner.setProjectedTicksDirection(new Vector3d(xticksdir[0], xticksdir[1], 0.0));

            if (axes.getXAxisGridColor() != -1) {
                FloatBuffer vertexData = getXGridData(values, rulerModel);
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
                drawingTools.getTransformationManager().getModelViewStack().pushRightMultiply(mirror);
                drawingTools.draw(new GeometryImpl(Geometry.DrawingMode.SEGMENTS, vertexBuffer), gridAppearance);
                drawingTools.getTransformationManager().getModelViewStack().pop();
            }

        }


        // Draw Y ruler
        if (axes.getAxes()[1].getAxisLocation().equals(AxisProperty.AxisLocation.LEFT)) {
            zs = -Math.signum(matrix[9]);  // First : switch Z such that Y was minimal.
            xs = -Math.signum(matrix[2]) * zs * Math.signum(matrix[10]);
            if (xs == 0) {
                xs = +1;
            }
        } else {
            zs = Math.signum(matrix[9]);  // First : switch Z such that Y was minimal.
            xs = -Math.signum(matrix[2]) * zs * Math.signum(matrix[10]); // Then switch X such that Z max but not in the middle.
            if (xs == 0) {
                xs = -1;
            }
        }

        rulerModel.setFirstPoint(new Vector3d(xs, 1, zs));
        rulerModel.setSecondPoint(new Vector3d(xs, -1, zs));


        if (px.getNorm2() > pz.getNorm2()) {
            rulerModel.setTicksDirection(new Vector3d(TICKS_SIZE * xs, 0, 0));
        } else {
            rulerModel.setTicksDirection(new Vector3d(0, 0, TICKS_SIZE * zs));
        }


        if (axes.getAxes()[1].getReverse()) {
            rulerModel.setValues(bounds[2], bounds[3]);
        } else {
            rulerModel.setValues(bounds[3], bounds[2]);
        }

        otherbounds[0] = (xs > 0) ? +1 : -1;
        otherbounds[1] = (zs > 0) ? +1 : -1;

        labelPosition = new Vector3d(otherbounds[0], midpoints[1], otherbounds[1]);

        AxisLabelPositioner yAxisLabelPositioner = (AxisLabelPositioner) axesDrawer.getYAxisLabelPositioner();
        yAxisLabelPositioner.setLabelPosition(labelPosition);

        if (axes.getYAxisVisible()) {
            rulerDrawingResult = rulerDrawer.draw(drawingTools, rulerModel);
            values = rulerDrawingResult.getTicksValues();
            axes.setYAxisTicksLocations(toDoubleArray(values));
            axes.setYAxisTicksLabels(toStringArray(values));
            axes.setYAxisSubticks(rulerDrawingResult.getSubTicksDensity() - 1);

            Vector3d yTicksDirection = rulerModel.getTicksDirection();

            distanceRatio = rulerDrawingResult.getMaxDistToTicksDirNorm();
            double [] yticksdir = rulerDrawingResult.getNormalizedTicksDirection();

            yAxisLabelPositioner.setTicksDirection(yTicksDirection);
            yAxisLabelPositioner.setDistanceRatio(distanceRatio);
            yAxisLabelPositioner.setProjectedTicksDirection(new Vector3d(yticksdir[0], yticksdir[1], 0.0));

            if (axes.getYAxisGridColor() != -1) {
                FloatBuffer vertexData = getYGridData(values, rulerModel);
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
                drawingTools.getTransformationManager().getModelViewStack().pushRightMultiply(mirror);
                drawingTools.draw(new GeometryImpl(Geometry.DrawingMode.SEGMENTS, vertexBuffer), gridAppearance);
                drawingTools.getTransformationManager().getModelViewStack().pop();
            }
        }

        // Draw Z ruler
        if (axes.getViewAsEnum() == Camera.ViewType.VIEW_3D && axes.getRotationAngles()[1] != 90.0) {
            double txs;
            double tys;
            if (Math.abs(matrix[2]) < Math.abs(matrix[6])) {
                xs = Math.signum(matrix[2]);
                ys = -Math.signum(matrix[6]);
                txs = xs;
                tys = 0;
            } else {
                xs = -Math.signum(matrix[2]);
                ys = Math.signum(matrix[6]);
                txs = 0;
                tys = ys;
            }

            rulerModel.setFirstPoint(new Vector3d(xs, ys, 1));
            rulerModel.setSecondPoint(new Vector3d(xs, ys, -1));
            rulerModel.setTicksDirection(new Vector3d(TICKS_SIZE * txs, TICKS_SIZE * tys, 0));
            if (axes.getAxes()[2].getReverse()) {
                rulerModel.setValues(bounds[4], bounds[5]);
            } else {
                rulerModel.setValues(bounds[5], bounds[4]);
            }

            otherbounds[0] = (xs > 0) ? +1 : -1;
            otherbounds[1] = (ys > 0) ? +1 : -1;

            labelPosition = new Vector3d(otherbounds[0], otherbounds[1], midpoints[2]);

            AxisLabelPositioner zAxisLabelPositioner = (AxisLabelPositioner) axesDrawer.getZAxisLabelPositioner();
            zAxisLabelPositioner.setLabelPosition(labelPosition);

            if (axes.getZAxisVisible()) {
                rulerDrawingResult = rulerDrawer.draw(drawingTools, rulerModel);
                values = rulerDrawingResult.getTicksValues();
                axes.setZAxisTicksLocations(toDoubleArray(values));
                axes.setZAxisTicksLabels(toStringArray(values));
                axes.setZAxisSubticks(rulerDrawingResult.getSubTicksDensity() - 1);

                distanceRatio = rulerDrawingResult.getMaxDistToTicksDirNorm();
                double [] zticksdir = rulerDrawingResult.getNormalizedTicksDirection();

                zAxisLabelPositioner.setTicksDirection(new Vector3d(TICKS_SIZE * txs, TICKS_SIZE * tys, 0.0));
                zAxisLabelPositioner.setDistanceRatio(distanceRatio);
                zAxisLabelPositioner.setProjectedTicksDirection(new Vector3d(zticksdir[0], zticksdir[1], 0.0));

                if (axes.getZAxisGridColor() != -1 || !axes.getZAxisVisible()) {
                    FloatBuffer vertexData = getZGridData(values, rulerModel);
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
                    drawingTools.getTransformationManager().getModelViewStack().pushRightMultiply(mirror);
                    drawingTools.draw(new GeometryImpl(Geometry.DrawingMode.SEGMENTS, vertexBuffer), gridAppearance);
                    drawingTools.getTransformationManager().getModelViewStack().pop();
                }
            }
        }

        drawingTools.getCanvas().getBuffersManager().dispose(vertexBuffer);
    }

    private String[] toStringArray(double[] values) {
        String[] r = new String[values.length];
        for (int i = 0; i < values.length; i++) {
            r[i] = String.valueOf(values[i]);
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
    private FloatBuffer getXGridData(double[] values, RulerModel rulerModel) {
        FloatBuffer vertexData = FloatBuffer.allocate(values.length * 16);
        int limit = 0;
        for (double value : values) {
            float p = (float) rulerModel.getPosition(value).getX();
            if ((p != -1) && (p!=1)) {
                vertexData.put(p); vertexData.put(+1); vertexData.put(+1); vertexData.put(1);
                vertexData.put(p); vertexData.put(-1); vertexData.put(+1); vertexData.put(1);
                vertexData.put(p); vertexData.put(+1); vertexData.put(+1); vertexData.put(1);
                vertexData.put(p); vertexData.put(+1); vertexData.put(-1); vertexData.put(1);
                limit += 16;
            }
        }
        vertexData.limit(limit);
        return vertexData;
    }

    /**
     * Build Y grid data.
     * @param values Y values where grid is drawn.
     * @param rulerModel used rulerModel to compute grid world position.
     * @return Y grid data.
     */
    private FloatBuffer getYGridData(double[] values, RulerModel rulerModel) {
        FloatBuffer vertexData = FloatBuffer.allocate(values.length * 16);
        int limit = 0;
        for (double value : values) {
            float p = (float) rulerModel.getPosition(value).getY();
            if ((p != -1) && (p!=1)) {
                vertexData.put(+1); vertexData.put(p); vertexData.put(+1); vertexData.put(1);
                vertexData.put(-1); vertexData.put(p); vertexData.put(+1); vertexData.put(1);
                vertexData.put(+1); vertexData.put(p); vertexData.put(+1); vertexData.put(1);
                vertexData.put(+1); vertexData.put(p); vertexData.put(-1); vertexData.put(1);
                limit += 16;
            }
        }
        vertexData.limit(limit);
        return vertexData;
    }

    /**
     * Build Z grid data.
     * @param values Z values where grid is drawn.
     * @param rulerModel used rulerModel to compute grid world position.
     * @return Z grid data.
     */
    private FloatBuffer getZGridData(double[] values, RulerModel rulerModel) {
        FloatBuffer vertexData = FloatBuffer.allocate(values.length * 16);
        int limit = 0;
        for (double value : values) {
            float p = (float) rulerModel.getPosition(value).getZ();
            if ((p != -1) && (p!=1)) {
                vertexData.put(+1); vertexData.put(+1); vertexData.put(p); vertexData.put(1);
                vertexData.put(-1); vertexData.put(+1); vertexData.put(p); vertexData.put(1);
                vertexData.put(+1); vertexData.put(+1); vertexData.put(p); vertexData.put(1);
                vertexData.put(+1); vertexData.put(-1); vertexData.put(p); vertexData.put(1);
                limit += 16;
            }
        }
        vertexData.limit(limit);
        return vertexData;
    }

    public void disposeAll() {
        this.rulerSpriteManagerSet.disposeAll();
    }

    public void update(String id, String property) {
        this.rulerSpriteManagerSet.update(id, property);
    }

    public void dispose(String id) {
        this.rulerSpriteManagerSet.dispose(id);
    }
}
