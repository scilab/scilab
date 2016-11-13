/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
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

package org.scilab.modules.renderer.JoGLView;

import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.ruler.DefaultRulerModel;
import org.scilab.forge.scirenderer.ruler.RulerDrawer;
import org.scilab.forge.scirenderer.ruler.RulerSpriteFactory;
import org.scilab.forge.scirenderer.ruler.graduations.AbstractGraduations;
import org.scilab.forge.scirenderer.ruler.graduations.Graduations;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.texture.TextureManager;
import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.axis.Axis;
import org.scilab.modules.graphic_objects.textObject.FormattedText;
import org.scilab.modules.renderer.JoGLView.util.ColorFactory;
import org.scilab.modules.renderer.JoGLView.util.FormattedTextSpriteDrawer;
import org.scilab.modules.renderer.JoGLView.util.ScaleUtils;

import java.text.DecimalFormat;
import java.util.Collections;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

/**
 * @author Pierre Lando
 */
public class AxisDrawer {

    /** Ticks length in pixels. */
    private static final int TICKS_LENGTH = 8;

    /** Sub-ticks length in pixels. */
    private static final int SUB_TICKS_LENGTH = 5;

    /**Ticks sprites distance in pixels. */
    private static final int SPRITE_DISTANCE = 12;

    private final DrawerVisitor drawerVisitor;

    public AxisDrawer(DrawerVisitor drawerVisitor) {
        this.drawerVisitor = drawerVisitor;
    }

    public void draw(Axes axes, Axis axis) {
        double min;
        double max;

        DefaultRulerModel rulerModel = new DefaultRulerModel();
        rulerModel.setSpriteDistance(SPRITE_DISTANCE);
        rulerModel.setSubTicksLength(SUB_TICKS_LENGTH);
        rulerModel.setTicksLength(TICKS_LENGTH);
        boolean[] logFlags = new boolean[] {axes.getXAxisLogFlag(), axes.getYAxisLogFlag(), axes.getZAxisLogFlag()};

        Double[] xTicksValues;
        Double[] yTicksValues;
        double[] xMinAndMax;
        double[] yMinAndMax;
        double[][] factors = calcCorrectedFactors(axes, axis);

        if (axis.getXTicksCoords().length == 1) {
            xTicksValues = axis.getXTicksCoords();
            yTicksValues = decodeValue(axis.getYTicksCoords(), axis.getTicksStyle());
            xMinAndMax = getMinAndMax(xTicksValues);
            yMinAndMax = getMinAndMax(yTicksValues);
            min = yMinAndMax[0];
            max = yMinAndMax[1];
            rulerModel.setUserGraduation(new AxisGraduation(axis, yTicksValues, yMinAndMax));
        } else {
            xTicksValues = decodeValue(axis.getXTicksCoords(), axis.getTicksStyle());
            yTicksValues = axis.getYTicksCoords();
            xMinAndMax = getMinAndMax(xTicksValues);
            yMinAndMax = getMinAndMax(yTicksValues);
            min = xMinAndMax[0];
            max = xMinAndMax[1];
            rulerModel.setUserGraduation(new AxisGraduation(axis, xTicksValues, xMinAndMax));
        }

        Vector3d start = new Vector3d(xMinAndMax[0], yMinAndMax[0], 0);
        Vector3d end = new Vector3d(xMinAndMax[1], yMinAndMax[1], 0);
        start = ScaleUtils.applyLogScale(start, logFlags);
        end = ScaleUtils.applyLogScale(end, logFlags);

        start = new Vector3d(start.getX() * factors[0][0] + factors[1][0], start.getY() * factors[0][1] + factors[1][1], start.getZ() * factors[0][2] + factors[1][2]);
        end = new Vector3d(end.getX() * factors[0][0] + factors[1][0], end.getY() * factors[0][1] + factors[1][1], end.getZ() * factors[0][2] + factors[1][2]);

        // TODO : RulerModel should be an interface.
        rulerModel.setAutoTicks(false);
        rulerModel.setFirstValue(0);
        rulerModel.setSecondValue(1);
        rulerModel.setLineVisible(axis.getTicksSegment());
        rulerModel.setColor(ColorFactory.createColor(drawerVisitor.getColorMap(), axis.getTicksColor()));

        rulerModel.setPoints(start, end);
        rulerModel.setTicksDirection(computeTicksDirection(axis.getTicksDirectionAsEnum()));

        DrawingTools drawingTools = drawerVisitor.getDrawingTools();

        RulerDrawer rulerDrawer = new RulerDrawer(drawerVisitor.getCanvas().getTextureManager());
        rulerDrawer.setSpriteFactory(new AxisSpriteFactory(axis, min, max));
        rulerDrawer.draw(drawingTools, rulerModel);

        axis.getFormatn();
        rulerDrawer.disposeResources();
    }

    /**
     * Calculates corrected axes factors when zoom is enabled
     * @param axes the given axes
     * @param axis the given axis
     * @return corrected factors
     */
    private double[][] calcCorrectedFactors(Axes axes, Axis axis) {
        Double [] bounds = axes.getDisplayedBounds();

        double[][] zoomFactors = new double[2][];
        zoomFactors[0] = new double[] { 2 / (bounds[1] - bounds[0]),
                                        2 / (bounds[3] - bounds[2]),
                                        2 / (bounds[5] - bounds[4])
                                      };
        zoomFactors[1] = new double[] { -(bounds[1] + bounds[0]) / (bounds[1] - bounds[0]),
                                        -(bounds[3] + bounds[2]) / (bounds[3] - bounds[2]),
                                        -(bounds[5] + bounds[4]) / (bounds[5] - bounds[4])
                                      };
        double[][] factors = axes.getScaleTranslateFactors();
        //Z coordinate
        double scale = factors[0][2] / (zoomFactors[0][2] == 0.0 ? 1.0 : zoomFactors[0][2]);
        double invScale = zoomFactors[0][2] / (factors[0][2] == 0.0 ? 1.0 : factors[0][2]);
        factors[0][2] *= scale;
        factors[1][2] = scale * (factors[1][2] + (invScale * factors[1][2] - zoomFactors[1][2]));



        if (axis.getYNumberTicks() == 1 || isConst(axis.getYTicksCoords())) {
            scale = factors[0][1] / (zoomFactors[0][1] == 0.0 ? 1.0 : zoomFactors[0][1]);
            invScale = zoomFactors[0][1] / (factors[0][1] == 0.0 ? 1.0 : factors[0][1]);
            factors[0][1] *= scale;
            factors[1][1] = scale * (factors[1][1] + (invScale * factors[1][1] - zoomFactors[1][1]));
        }

        if (axis.getXNumberTicks() == 1 || isConst(axis.getXTicksCoords())) {
            scale = factors[0][0] / (zoomFactors[0][0] == 0.0 ? 1.0 : zoomFactors[0][0]);
            invScale = zoomFactors[0][0] / (factors[0][0] == 0.0 ? 1.0 : factors[0][0]);
            factors[0][0] *= scale;
            factors[1][0] = scale * (factors[1][0] + (invScale * factors[1][0] - zoomFactors[1][0]));
        }

        return factors;
    }

    /**
     * Checks if the given array have all its values equal(constant)
     * @param data the given array
     * @return true if it is constant, false otherwise
     */
    private boolean isConst(Double[] data) {
        if (data.length > 0) {
            double d = data[0];
            for (int i = 0; i < data.length; i++) {
                if (data[i] != d) {
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * Return [min, max] the minimum and maximum of given values.
     * @param values the given value.
     * @return [min, max] the minimum and maximum of given values.
     */
    private double[] getMinAndMax(Double[] values) {
        double min = +Double.MAX_VALUE;
        double max = -Double.MAX_VALUE;
        for (double value : values) {
            min = Math.min(min, value);
            max = Math.max(max, value);
        }
        return new double[] {min, max};
    }

    /**
     * Decode ticks coordinate.
     * If ticksStyle='0' then v gives the tics positions along the axis.
     * If ticksStyle='1' then v must be of size 3. r=[xmin,xmax,n] and n gives the number of intervals.
     * If ticksStyle='2' then v must be of size 4, r=[k1,k2,a,n]. then xmin=k1*10^a, xmax=k2*10^a and n gives the number of intervals
     * @param v the given value.
     * @param ticksStyle used ticks style.
     * @return decoded ticks coordinate.
     */
    private Double[] decodeValue(Double[] v, int ticksStyle) {
        if ((ticksStyle == 1) && (v.length >= 3)) {
            double min = v[0], max = v[1];
            int n = v[2].intValue();
            Double[] r = new Double[n + 1];
            double k = (max - min) / n;
            for (int i = 0 ; i <=  n ; i++) {
                r[i] = min + i * k;
            }
            return r;
        } else if ((ticksStyle == 2) && (v.length >= 4)) {
            double pow10 = Math.pow(10, v[2]);
            double min = v[0] * pow10;
            double max = v[1] * pow10;
            int n = v[3].intValue();
            Double[] r = new Double[n + 1];
            double k = (max - min) / n;
            for (int i = 0 ; i <=  n ; i++) {
                r[i] = min + i * k;
            }
            return r;
        } else {
            return v;
        }
    }

    /**
     * Return the ticks direction in scene coordinate.
     * @param direction the ticks direction as an {@see Axis.TicksDirection}
     * @return the ticks direction in scene coordinate.
     */
    private Vector3d computeTicksDirection(Axis.TicksDirection direction) {
        switch (direction) {
            case TOP:
                return new Vector3d(0, +1, 0);
            case BOTTOM:
                return new Vector3d(0, -1, 0);
            case LEFT:
                return new Vector3d(-1, 0, 0);
            default:
                return new Vector3d(+1, 0, 0);
        }
    }

    private class AxisGraduation extends AbstractGraduations {
        private final List<Double> subTicksValue;
        private final List<Double> allValues;
        private final Axis axis;


        AxisGraduation(Axis axis, Double[] ticksCoordinate, double[] minAndMax) {
            super(0., 1.);
            this.axis = axis;
            allValues = computeValue(ticksCoordinate, minAndMax);
            subTicksValue = computeSubValue(allValues, axis.getSubticks());
        }


        /**
         * Compute the sub-ticks value.
         * @param allValues the sorted list of ticks value.
         * @param subTicks the number of sub-division between two ticks.
         * @return the sub-ticks value.
         */
        private List<Double> computeSubValue(List<Double> allValues, Integer subTicks) {
            if ((allValues == null) || (allValues.size() < 2)) {
                return new LinkedList<Double>();
            } else {
                LinkedList<Double> subTicksValue = new LinkedList<Double>();
                Iterator<Double> iterator = allValues.iterator();
                double lastValue = iterator.next();
                while (iterator.hasNext()) {
                    double currentValue = iterator.next();
                    double k = (currentValue - lastValue) / subTicks;
                    for (int i = 1 ; i < subTicks ; i++) {
                        subTicksValue.add(lastValue + i * k);
                    }
                    lastValue = currentValue;
                }
                return subTicksValue;
            }
        }

        /**
         * Return a list of graduation value corresponding to the given coordinate.
         * Value are in the range [0, 1] and the coordinate are linearly mapped
         * to this range.
         * If the coordinates are empty, empty list is returned.
         * If the coordinates are all the same, a same-sized list of zero is returned.
         *
         * @param coordinates given coordinate.
         * @param minAndMax the min an max of the given coordinate.
         * @return a list of graduation value corresponding to the given coordinate.
         */
        private List<Double> computeValue(Double[] coordinates, double[] minAndMax) {
            if ((coordinates == null) || (coordinates.length == 0)) {
                return new LinkedList<Double>();
            } else if (coordinates.length == 1) {
                LinkedList<Double> allValues = new LinkedList<Double>();
                allValues.add(0.);
                return allValues;
            } else {
                LinkedList<Double> allValues = new LinkedList<Double>();
                double min = minAndMax[0];
                double max = minAndMax[1];
                if (max == min) {
                    for (Double coordinate : coordinates) {
                        allValues.add(0.);
                    }
                } else {
                    double k = 1 / (max - min);
                    for (double value : coordinates) {
                        allValues.add(k * (value - min));
                    }
                    Collections.sort(allValues);
                }
                return allValues;
            }
        }

        @Override
        public List<Double> getAllValues() {
            return allValues;
        }

        @Override
        public List<Double> getNewValues() {
            return allValues;
        }

        @Override
        public Graduations getMore() {
            return null;
        }

        @Override
        public Graduations getAlternative() {
            return null;
        }

        @Override
        public Graduations getSubGraduations() {
            return new AbstractGraduations(this) {
                @Override
                public List<Double> getAllValues() {
                    return subTicksValue;
                }

                @Override
                public List<Double> getNewValues() {
                    return getAllValues();
                }

                @Override
                public Graduations getMore() {
                    return null;
                }

                @Override
                public Graduations getAlternative() {
                    return null;
                }

                @Override
                public Graduations getSubGraduations() {
                    return null;
                }

                @Override
                public int getSubDensity() {
                    return 0;
                }
            };
        }

        @Override
        public int getSubDensity() {
            return axis.getSubticks();
        }
    }

    private class AxisSpriteFactory implements RulerSpriteFactory {
        private final Axis axis;
        private final double min;
        private final double max;

        public AxisSpriteFactory(Axis axis, double min, double max) {
            this.axis = axis;
            this.min = min;
            this.max = max;
        }

        @Override
        public Texture create(double value, DecimalFormat adaptedFormat, TextureManager spriteManager) {
            String label = getLabel(value);
            if (label != null) {
                FormattedText formattedText = new FormattedText();
                formattedText.setFont(axis.getFont());
                formattedText.setText(getLabel(value));

                FormattedTextSpriteDrawer textureDrawer = new FormattedTextSpriteDrawer(drawerVisitor.getColorMap(), formattedText);
                Texture texture = spriteManager.createTexture();
                texture.setMagnificationFilter(Texture.Filter.LINEAR);
                texture.setMinifyingFilter(Texture.Filter.LINEAR);
                texture.setDrawer(textureDrawer);

                return texture;
            } else {
                return null;
            }
        }

        /**
         * Return the label corresponding to the given value.
         * @param value the given value.
         * @return the label corresponding to the given value.
         */
        private String getLabel(double value) {
            // 0 <= value <= 1
            // Should find right index through given labels.
            String[] ticksLabel = axis.getTicksLabels();
            int index = (int) Math.round(value * (ticksLabel.length - 1));
            if ((index < 0) || (index > ticksLabel.length) || ticksLabel.length == 0) {
                return null;
            } else {
                return ticksLabel[index];
            }
        }
    }
}
