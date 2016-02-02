/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2012 - DIGITEO - Manuel JULIACHS
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.graphic_objects.vectfield;

import org.scilab.modules.graphic_objects.ObjectRemovedException;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;
import org.scilab.modules.graphic_objects.utils.Utils;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;

/**
 * Champ class
 * Stores the arrows in row-major order, all the arrows of
 * row j having the same y-coordinate.
 * @author Manuel JULIACHS
 */
public class Champ extends VectField {
    /** Champ properties names */
    private enum ChampProperty { DIMENSIONS, COLORED, BOUNDINGBOX, MAXLENGTH, MAXUSABLELENGTH };

    /** Dimensions: 2-element array (number of columns Ni, number of rows Nj) */
    private int[] dimensions;

    /** Specifies whether vectors are colored or not */
    private boolean colored;

    /** Constructor */
    public Champ() {
        super();
        colored = false;
        dimensions = new int[2];
        dimensions[0] = 0;
        dimensions[1] = 0;
    }

    @Override
    public void accept(Visitor visitor) throws ObjectRemovedException {
        visitor.visit(this);
    }

    /**
     * Returns the enum associated to a property name
     * @param propertyName the property name
     * @return the property enum
     */
    public Object getPropertyFromName(int propertyName) {
        switch (propertyName) {
            case __GO_CHAMP_DIMENSIONS__ :
                return ChampProperty.DIMENSIONS;
            case __GO_BASE_X__ :
                return Arrow.ArrowProperty.BASEX;
            case __GO_BASE_Y__ :
                return Arrow.ArrowProperty.BASEY;
            case __GO_COLORED__ :
                return ChampProperty.COLORED;
            case __GO_BOUNDING_BOX__ :
                return ChampProperty.BOUNDINGBOX;
            case __GO_MAX_LENGTH__ :
                return ChampProperty.MAXLENGTH;
            case __GO_MAX_USABLE_LENGTH__ :
                return ChampProperty.MAXUSABLELENGTH;
            default :
                return super.getPropertyFromName(propertyName);
        }
    }

    /**
     * Fast property get method
     * @param property the property to get
     * @return the property value
     */
    public Object getProperty(Object property) {
        if (property == ChampProperty.DIMENSIONS) {
            return getDimensions();
        } else if (property == Arrow.ArrowProperty.BASEX) {
            return getBaseX();
        } else if (property == Arrow.ArrowProperty.BASEY) {
            return getBaseY();
        } else if (property == ChampProperty.COLORED) {
            return getColored();
        } else if (property == ChampProperty.BOUNDINGBOX) {
            return getBoundingBox();
        } else if (property == ChampProperty.MAXLENGTH) {
            return getMaxLength();
        } else if (property == ChampProperty.MAXUSABLELENGTH) {
            return getMaxUsableLength();
        } else {
            return super.getProperty(property);
        }
    }

    /**
     * Fast property set method
     * @param property the property to set
     * @param value the property value
     * @return true if the property has been set, false otherwise
     */
    public UpdateStatus setProperty(Object property, Object value) {
        if (property == ChampProperty.DIMENSIONS) {
            setDimensions((Integer[]) value);
        } else if (property == Arrow.ArrowProperty.BASEX) {
            setBaseX((Double[]) value);
        } else if (property == Arrow.ArrowProperty.BASEY) {
            setBaseY((Double[]) value);
        } else if (property == ChampProperty.COLORED) {
            setColored((Boolean) value);
        } else {
            return super.setProperty(property, value);
        }

        return UpdateStatus.Success;
    }

    /**
     * @return the field's dimensions
     */
    public Integer[] getDimensions() {
        Integer[] retDimensions = new Integer[2];
        retDimensions[0] = dimensions[0];
        retDimensions[1] = dimensions[1];

        return retDimensions;
    }

    /**
     * @param dimensions the field dimensions to set
     */
    public UpdateStatus setDimensions(Integer[] dimensions) {
        this.dimensions[0] = dimensions[0];
        this.dimensions[1] = dimensions[1];
        return UpdateStatus.Success;
    }

    /**
     * Returns the arrow bases y coordinates (Ni-element array)
     * @return the arrow bases x coordinates
     */
    public Double[] getBaseX() {
        Double[] retArrowBaseX;

        retArrowBaseX = new Double[dimensions[0]];

        for (int i = 0; i < dimensions[0]; i++) {
            Double[] base = getArrow(i, 0).getBase();
            retArrowBaseX[i] = base[0];
        }

        return retArrowBaseX;
    }

    /**
     * Sets the arrow bases x coordinates
     * The arrows part of column i have the same base x-coordinate
     * @param baseX the array of x coordinates (Ni elements)
     */
    public UpdateStatus setBaseX(Double[] baseX) {
        for (int j = 0; j < dimensions[1]; j++) {
            for (int i = 0; i < dimensions[0]; i++) {
                Double[] base = getArrow(i, j).getBase();

                base[0] = baseX[i];
                getArrow(i, j).setBase(base);
            }
        }
        return UpdateStatus.Success;
    }

    /**
     * Returns the arrow bases y coordinates (Nj-element array)
     * @return the arrow bases y coordinates
     */
    public Double[] getBaseY() {
        Double[] retArrowBaseY;

        retArrowBaseY = new Double[dimensions[1]];

        for (int j = 0; j < dimensions[1]; j++) {
            Double[] base = getArrow(0, j).getBase();
            retArrowBaseY[j] = base[1];
        }

        return retArrowBaseY;
    }

    /**
     * Sets the arrow bases y coordinates
     * The arrows part of row j have the same base y-coordinate
     * @param baseY the array of y coordinates (Nj elements)
     */
    public UpdateStatus setBaseY(Double[] baseY) {
        for (int j = 0; j < dimensions[1]; j++) {
            for (int i = 0; i < dimensions[0]; i++) {
                Double[] base = getArrow(i, j).getBase();

                base[1] = baseY[j];
                getArrow(i, j).setBase(base);
            }
        }
        return UpdateStatus.Success;
    }



    /**
     * @return the colored
     */
    public Boolean getColored() {
        return colored;
    }

    /**
     * @param colored the colored to set
     */
    public UpdateStatus setColored(Boolean colored) {
        this.colored = colored;
        return UpdateStatus.Success;
    }

    /**
     * Returns the champ's bounding box, which is required by
     * its builder function.
     * @return the bounding box [xmin, xmax, ymin, ymax, zmin, zmax]
     */
    public Double[] getBoundingBox() {
        Double[] retBoundingBox;

        retBoundingBox = computeBoundingBox();

        return retBoundingBox;
    }

    /**
     * Returns the champ's maximum vector length
     * @return the maximum length
     */
    public Double getMaxLength() {
        Double retMaxLength;

        retMaxLength = computeMaxLength();

        return retMaxLength;
    }

    /**
     * Returns the champ's maximum usable vector length
     * @return the maximum usable length
     */
    public Double getMaxUsableLength() {
        Double retMaxUsableLength;

        retMaxUsableLength = computeMaxUsableLength();

        return retMaxUsableLength;
    }

    /**
     * @return Type as String
     */
    public Integer getType() {
        return GraphicObjectProperties.__GO_CHAMP__;
    }

    /**
     * Computes the bounding box, which is required by the Champ
     * builder function.
     * @return the computed bounding box [xmin, xmax, ymin, ymax, zmin, zmax]
     */
    private Double[] computeBoundingBox() {
        double maxLength = 0.0;
        double maxUsableLength = 0.0;

        double[] min = new double[3];
        double[] max = new double[3];
        double[] tmp = new double[3];

        double[] base = new double[3];
        double[] direction = new double[3];

        Double[] boundingBox = new Double[6];

        if (colored == false) {
            maxLength = computeMaxLength();
        }

        maxUsableLength = computeMaxUsableLength();

        /* Compute the bounding box points using the previously computed lengths */

        base[0] = arrows.get(0).getBase()[0];
        base[1] = arrows.get(0).getBase()[1];
        base[2] = arrows.get(0).getBase()[2];

        direction[0] = arrows.get(0).getDirection()[0];
        direction[1] = arrows.get(0).getDirection()[1];
        direction[2] = arrows.get(0).getDirection()[2];

        if (colored == true) {
            maxLength = Math.sqrt(direction[0] * direction[0] + direction[1] * direction[1] + direction[2] * direction[2]);

            if (maxLength <= Double.MIN_VALUE) {
                maxLength = 1.0;
            }
        }

        direction[0] = base[0] + direction[0] * maxUsableLength / maxLength;
        direction[1] = base[1] + direction[1] * maxUsableLength / maxLength;
        direction[2] = base[2] + direction[2] * maxUsableLength / maxLength;

        computeMin(min, base, direction);
        computeMax(max, base, direction);

        for (int i = 1; i < arrows.size(); i++) {
            base[0] = arrows.get(i).getBase()[0];
            base[1] = arrows.get(i).getBase()[1];
            base[2] = arrows.get(i).getBase()[2];

            direction[0] = arrows.get(i).getDirection()[0];
            direction[1] = arrows.get(i).getDirection()[1];
            direction[2] = arrows.get(i).getDirection()[2];

            if (colored == true) {
                maxLength = Math.sqrt(direction[0] * direction[0] + direction[1] * direction[1] + direction[2] * direction[2]);

                if (maxLength <= Double.MIN_VALUE) {
                    maxLength = 1.0;
                }
            }

            direction[0] = base[0] + direction[0] * maxUsableLength / maxLength;
            direction[1] = base[1] + direction[1] * maxUsableLength / maxLength;
            direction[2] = base[2] + direction[2] * maxUsableLength / maxLength;

            computeMin(tmp, min, direction);
            computeMin(min, tmp, base);

            computeMax(tmp, max, direction);
            computeMax(max, tmp, base);
        }

        boundingBox[0] = min[0];
        boundingBox[1] = max[0];
        boundingBox[2] = min[1];
        boundingBox[3] = max[1];
        boundingBox[4] = min[2];
        boundingBox[5] = max[2];

        return boundingBox;
    }

    /**
     * Computes the maximum usable length
     * @return the maximum usable length
     */
    private double computeMaxUsableLength() {
        double minX;
        double minY;

        /*
         * Determines the minimum distance between two consecutive abscissas
         * Only the first row needs to be examined
         */
        if (dimensions[0] < 2) {
            minX = 1.0;
        } else {
            minX = Double.MAX_VALUE;

            for (int i = 0; i < dimensions[0] - 1; i++) {
                double length = Math.abs(getArrow(i + 1, 0).getBase()[0] - getArrow(i, 0).getBase()[0]);

                if (Utils.isValid(length)) {
                    if (length < minX) {
                        minX = length;
                    }
                }
            }

        }

        /*
         * Determines the minimum distance between two consecutive ordinates
         * Only the first column needs to be examined
         */
        if (dimensions[1] < 2) {
            minY = 1.0;
        } else {
            minY = Double.MAX_VALUE;

            for (int j = 0; j < dimensions[1] - 1; j++) {
                double length = Math.abs(getArrow(0, j + 1).getBase()[1] - getArrow(0, j).getBase()[1]);

                if (Utils.isValid(length)) {
                    if (length < minY) {
                        minY = length;
                    }
                }
            }

        }

        return Math.min(minX, minY);
    }

    /**
     * Computes the maximum arrow length
     * @return the maximum arrow length
     */
    private double computeMaxLength() {
        double length;
        double maxLength;
        double[] direction = new double[3];

        direction[0] = arrows.get(0).getDirection()[0];
        direction[1] = arrows.get(0).getDirection()[1];
        direction[2] = arrows.get(0).getDirection()[2];

        maxLength = direction[0] * direction[0] + direction[1] * direction[1] + direction[2] * direction[2];

        for (int i = 1; i < arrows.size(); i++) {
            direction[0] = arrows.get(i).getDirection()[0];
            direction[1] = arrows.get(i).getDirection()[1];
            direction[2] = arrows.get(i).getDirection()[2];

            length = direction[0] * direction[0] + direction[1] * direction[1] + direction[2] * direction[2];

            if (length > maxLength) {
                maxLength = length;
            }
        }

        maxLength = Math.sqrt(maxLength);

        return maxLength;
    }

    /**
     * Computes the component-wise minimum of two 3-element arrays
     * @param result the component-wise minimum (3-element array)
     * @param first the first 3-element array
     * @param second the second 3-element array
     */
    private void computeMin(double[] result, double[] first, double[] second) {
        result[0] = Math.min(first[0], second[0]);
        result[1] = Math.min(first[1], second[1]);
        result[2] = Math.min(first[2], second[2]);
    }

    /**
     * Computes the component-wise maximum of two 3-element arrays
     * @param result the component-wise maximum (3-element array)
     * @param first the first 3-element array
     * @param second the second 3-element array
     */
    private void computeMax(double[] result, double[] first, double[] second) {
        result[0] = Math.max(first[0], second[0]);
        result[1] = Math.max(first[1], second[1]);
        result[2] = Math.max(first[2], second[2]);
    }

    /**
     * Returns the arrow specified by a pair of indices
     * @param firstDimIndex the first dimension index
     * @param secondDimIndex the second dimension index
     * @return the Arrow
     */
    private Arrow getArrow(int firstDimIndex, int secondDimIndex) {
        return arrows.get(dimensions[0] * secondDimIndex + firstDimIndex);
    }
}
