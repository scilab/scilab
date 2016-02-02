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

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_ARROWS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_ARROW_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_BASE_X__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_BASE_Y__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_BASE_Z__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_BASE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DIRECTION_X__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DIRECTION_Y__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DIRECTION_Z__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DIRECTION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_LINE_MODE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_LINE_STYLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_LINE_THICKNESS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_NUMBER_ARROWS__;

import java.util.ArrayList;

import org.scilab.modules.graphic_objects.contouredObject.Line;
import org.scilab.modules.graphic_objects.contouredObject.Line.LineType;
import org.scilab.modules.graphic_objects.graphicObject.GraphicClippableObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;

/**
 * VectField class
 * @author Manuel JULIACHS
 */
public abstract class VectField extends GraphicClippableObject {
    /** VectField properties names */
    private enum VectFieldProperty { ARROWS, NUMBERARROWS };

    /** List of arrows composing the field */
    protected ArrayList <Arrow> arrows;

    /** Constructor */
    public VectField() {
        super();
        arrows = new ArrayList<Arrow>(0);
    }

    /**
     * Returns the enum associated to a property name
     * @param propertyName the property name
     * @return the property enum
     */
    public Object getPropertyFromName(int propertyName) {
        switch (propertyName) {
            case __GO_ARROWS__ :
                return VectFieldProperty.ARROWS;
            case __GO_NUMBER_ARROWS__ :
                return VectFieldProperty.NUMBERARROWS;
            case __GO_BASE__ :
                return Arrow.ArrowProperty.BASE;
            case __GO_BASE_X__ :
                return Arrow.ArrowProperty.BASEX;
            case __GO_BASE_Y__ :
                return Arrow.ArrowProperty.BASEY;
            case __GO_BASE_Z__ :
                return Arrow.ArrowProperty.BASEZ;
            case __GO_DIRECTION__ :
                return Arrow.ArrowProperty.DIRECTION;
            case __GO_DIRECTION_X__ :
                return Arrow.ArrowProperty.DIRECTIONX;
            case __GO_DIRECTION_Y__ :
                return Arrow.ArrowProperty.DIRECTIONY;
            case __GO_DIRECTION_Z__ :
                return Arrow.ArrowProperty.DIRECTIONZ;
            case __GO_ARROW_SIZE__ :
                return Arrow.ArrowProperty.ARROWSIZE;
            case __GO_LINE_MODE__ :
                return Line.LinePropertyType.MODE;
            case __GO_LINE_STYLE__ :
                return Line.LinePropertyType.LINESTYLE;
            case __GO_LINE_THICKNESS__ :
                return Line.LinePropertyType.THICKNESS;
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
        if (property == VectFieldProperty.ARROWS) {
            return getArrows();
        } else if (property == VectFieldProperty.NUMBERARROWS) {
            return getNumberArrows();
        } else if (property == Arrow.ArrowProperty.BASE) {
            return getBase();
        } else if (property == Arrow.ArrowProperty.BASEX) {
            return getBaseX();
        } else if (property == Arrow.ArrowProperty.BASEY) {
            return getBaseY();
        } else if (property == Arrow.ArrowProperty.BASEZ) {
            return getBaseZ();
        } else if (property == Arrow.ArrowProperty.DIRECTION) {
            return getDirection();
        } else if (property == Arrow.ArrowProperty.DIRECTIONX) {
            return getDirectionX();
        } else if (property == Arrow.ArrowProperty.DIRECTIONY) {
            return getDirectionY();
        } else if (property == Arrow.ArrowProperty.DIRECTIONZ) {
            return getDirectionZ();
        } else if (property == Arrow.ArrowProperty.ARROWSIZE) {
            return getArrowSize();
        } else if (property == Line.LinePropertyType.MODE) {
            return getLineMode();
        } else if (property == Line.LinePropertyType.LINESTYLE) {
            return getLineStyle();
        } else if (property == Line.LinePropertyType.THICKNESS) {
            return getLineThickness();
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
        if (property == VectFieldProperty.ARROWS) {
            setArrows((ArrayList<Arrow>) value);
        } else if (property == VectFieldProperty.NUMBERARROWS) {
            setNumberArrows((Integer) value);
        } else if (property == Arrow.ArrowProperty.BASE) {
            setBase((Double[]) value);
        } else if (property == Arrow.ArrowProperty.BASEX) {
            setBaseX((Double[]) value);
        } else if (property == Arrow.ArrowProperty.BASEY) {
            setBaseY((Double[]) value);
        } else if (property == Arrow.ArrowProperty.BASEZ) {
            setBaseZ((Double[]) value);
        } else if (property == Arrow.ArrowProperty.DIRECTION) {
            setDirection((Double[]) value);
        } else if (property == Arrow.ArrowProperty.DIRECTIONX) {
            setDirectionX((Double[]) value);
        } else if (property == Arrow.ArrowProperty.DIRECTIONY) {
            setDirectionY((Double[]) value);
        } else if (property == Arrow.ArrowProperty.DIRECTIONZ) {
            setDirectionZ((Double[]) value);
        } else if (property == Arrow.ArrowProperty.ARROWSIZE) {
            setArrowSize((Double) value);
        } else if (property == Line.LinePropertyType.MODE) {
            setLineMode((Boolean) value);
        } else if (property == Line.LinePropertyType.LINESTYLE) {
            setLineStyle((Integer) value);
        } else if (property == Line.LinePropertyType.THICKNESS) {
            setLineThickness((Double) value);
        } else {
            return super.setProperty(property, value);
        }

        return UpdateStatus.Success;
    }

    /**
     * @return the arrows
     */
    public ArrayList<Arrow> getArrows() {
        return arrows;
    }

    /**
     * @param arrows the arrows to set
     */
    public UpdateStatus setArrows(ArrayList<Arrow> arrows) {
        this.arrows = arrows;
        return UpdateStatus.Success;
    }

    /**
     * @return the number of arrows
     */
    public Integer getNumberArrows() {
        return arrows.size();
    }

    /**
     * Sets the number of arrows
     * Empties the current arrow array, if required, and creates the
     * specified number of arrows, which are added to the array.
     * If the new number is equal to the current number, the
     * array is left unmodified.
     * If the existing array is not empty, the newly created
     * arrows are cloned from the latter's first arrow,
     * as all arrows are considered to have identical Contour values for now.
     * To be modified when Contour values can be specified independently
     * for each Arrow.
     * @param number the number of arrows to set
     * @return
     */
    public UpdateStatus setNumberArrows(Integer number) {
        if (!arrows.isEmpty() && number != arrows.size()) {
            ArrayList <Arrow> newArrows = new ArrayList<Arrow>(0);

            /* Clones the Contour properties of the first arrow */
            for (int i = 0; i < number; i++) {
                Arrow clonedArrow = arrows.get(0).clone();
                newArrows.add(clonedArrow);
            }

            arrows.clear();
            arrows = newArrows;

        } else if (arrows.isEmpty()) {
            for (int i = 0; i < number; i++) {
                arrows.add(new Arrow());
            }
        }

        return UpdateStatus.Success;
    }

    /**
     * @return the arrow bases
     */
    public Double[] getBase() {
        Double[] retBase;

        retBase = new Double[3 * arrows.size()];

        for (int i = 0; i < arrows.size(); i++) {
            Double[] base = arrows.get(i).getBase();
            retBase[3 * i] = base[0];
            retBase[3 * i + 1] = base[1];
            retBase[3 * i + 2] = base[2];
        }

        return retBase;
    }

    /**
     * @param bases the arrow bases to set
     */
    public UpdateStatus setBase(Double[] bases) {
        Double[] arrowBase = new Double[3];

        final int len = Math.min(bases.length / 3, arrows.size());
        for (int i = 0; i < len; i++) {
            arrowBase[0] = bases[3 * i];
            arrowBase[1] = bases[3 * i + 1];
            arrowBase[2] = bases[3 * i + 2];
            arrows.get(i).setBase(arrowBase);
        }
        return UpdateStatus.Success;
    }

    /**
     * @return the arrow bases x coordinates
     */
    public Double[] getBaseX() {
        Double[] retArrowBaseX;

        retArrowBaseX = new Double[arrows.size()];

        for (int i = 0; i < arrows.size(); i++) {
            Double[] base = arrows.get(i).getBase();
            retArrowBaseX[i] = base[0];
        }

        return retArrowBaseX;
    }

    /**
     * @param baseX the arrow base x coordinates to set
     */
    public UpdateStatus setBaseX(Double[] baseX) {
        final int len = Math.min(baseX.length, arrows.size());
        for (int i = 0; i < len; i++) {
            Double[] base = arrows.get(i).getBase();
            base[0] = baseX[i];
            arrows.get(i).setBase(base);
        }
        return UpdateStatus.Success;
    }

    /**
     * @return the arrow bases y coordinates
     */
    public Double[] getBaseY() {
        Double[] retArrowBaseY;

        retArrowBaseY = new Double[arrows.size()];

        for (int i = 0; i < arrows.size(); i++) {
            Double[] base = arrows.get(i).getBase();
            retArrowBaseY[i] = base[1];
        }

        return retArrowBaseY;
    }

    /**
     * @param baseY the arrow base y coordinates to set
     */
    public UpdateStatus setBaseY(Double[] baseY) {
        final int len = Math.min(baseY.length, arrows.size());
        for (int i = 0; i < len; i++) {
            Double[] base = arrows.get(i).getBase();
            base[1] = baseY[i];
            arrows.get(i).setBase(base);
        }
        return UpdateStatus.Success;
    }

    /**
     * @return the arrow bases z coordinates
     */
    public Double[] getBaseZ() {
        Double[] retArrowBaseZ;

        retArrowBaseZ = new Double[arrows.size()];

        for (int i = 0; i < arrows.size(); i++) {
            Double[] base = arrows.get(i).getBase();
            retArrowBaseZ[i] = base[2];
        }

        return retArrowBaseZ;
    }

    /**
     * @param baseZ the arrow base z coordinates to set
     */
    public UpdateStatus setBaseZ(Double[] baseZ) {
        final int len = Math.min(baseZ.length, arrows.size());
        for (int i = 0; i < len; i++) {
            Double[] base = arrows.get(i).getBase();
            base[2] = baseZ[i];
            arrows.get(i).setBase(base);
        }
        return UpdateStatus.Success;
    }

    /**
     * @return the arrow directions
     */
    public Double[] getDirection() {
        Double[] retDirection;

        retDirection = new Double[3 * arrows.size()];

        for (int i = 0; i < arrows.size(); i++) {
            Double[] base = arrows.get(i).getDirection();
            retDirection[3 * i] = base[0];
            retDirection[3 * i + 1] = base[1];
            retDirection[3 * i + 2] = base[2];
        }

        return retDirection;
    }

    /**
     * @param directions the arrow directions to set
     */
    public UpdateStatus setDirection(Double[] directions) {
        Double[] arrowDirection = new Double[3];

        final int len = Math.min(directions.length / 3, arrows.size());
        for (int i = 0; i < len; i++) {
            arrowDirection[0] = directions[3 * i];
            arrowDirection[1] = directions[3 * i + 1];
            arrowDirection[2] = directions[3 * i + 2];
            arrows.get(i).setDirection(arrowDirection);
        }
        return UpdateStatus.Success;
    }

    /**
     * @return the arrow directions x coordinates
     */
    public Double[] getDirectionX() {
        Double[] retArrowDirectionX;

        retArrowDirectionX = new Double[arrows.size()];

        for (int i = 0; i < arrows.size(); i++) {
            Double[] direction = arrows.get(i).getDirection();
            retArrowDirectionX[i] = direction[0];
        }

        return retArrowDirectionX;
    }

    /**
     * @param directionX the arrow direction x coordinates to set
     */
    public UpdateStatus setDirectionX(Double[] directionX) {
        final int len = Math.min(directionX.length, arrows.size());
        for (int i = 0; i < len; i++) {
            Double[] direction = arrows.get(i).getDirection();
            direction[0] = directionX[i];
            arrows.get(i).setDirection(direction);
        }
        return UpdateStatus.Success;
    }

    /**
     * @return the arrow directions y coordinates
     */
    public Double[] getDirectionY() {
        Double[] retArrowDirectionY;

        retArrowDirectionY = new Double[arrows.size()];

        for (int i = 0; i < arrows.size(); i++) {
            Double[] direction = arrows.get(i).getDirection();
            retArrowDirectionY[i] = direction[1];
        }

        return retArrowDirectionY;
    }

    /**
     * @param directionY the arrow direction y coordinates to set
     */
    public UpdateStatus setDirectionY(Double[] directionY) {
        final int len = Math.min(directionY.length, arrows.size());
        for (int i = 0; i < len; i++) {
            Double[] direction = arrows.get(i).getDirection();
            direction[1] = directionY[i];
            arrows.get(i).setDirection(direction);
        }
        return UpdateStatus.Success;
    }


    /**
     * @return the arrow directions z coordinates
     */
    public Double[] getDirectionZ() {
        Double[] retArrowDirectionZ;

        retArrowDirectionZ = new Double[arrows.size()];

        for (int i = 0; i < arrows.size(); i++) {
            Double[] direction = arrows.get(i).getDirection();
            retArrowDirectionZ[i] = direction[2];
        }

        return retArrowDirectionZ;
    }


    /**
     * @param directionZ the arrow direction z coordinates to set
     */
    public UpdateStatus setDirectionZ(Double[] directionZ) {
        final int len = Math.min(directionZ.length, arrows.size());
        for (int i = 0; i < len; i++) {
            Double[] direction = arrows.get(i).getDirection();
            direction[2] = directionZ[i];
            arrows.get(i).setDirection(direction);
        }
        return UpdateStatus.Success;
    }

    /**
     * All arrows are supposed to have the same size
     * To be done: return an array of Double
     * @return the arrow size
     */
    public Double getArrowSize() {
        Double retArrowSize;

        retArrowSize = arrows.get(0).getArrowSize();

        return retArrowSize;
    }

    /**
     * All arrows are supposed to have the same size
     * To be done: change the argument to an array of Double
     * @param size the arrow size to set
     */
    public UpdateStatus setArrowSize(Double size) {
        for (int i = 0; i < arrows.size(); i++) {
            arrows.get(i).setArrowSize(size);
        }
        return UpdateStatus.Success;
    }

    /**
     * All arrows are supposed to have the same line mode
     * To be done: return an array of Boolean
     * @return the arrow line mode
     */
    public Boolean getLineMode() {
        Boolean retLineMode;

        retLineMode = arrows.get(0).getLineMode();
        return retLineMode;
    }

    /**
     * All arrows are supposed to have the same line mode
     * To be done: change the argument to an array of Boolean
     * @param lineMode the arrow line mode to set
     */
    public UpdateStatus setLineMode(Boolean lineMode) {
        for (int i = 0; i < arrows.size(); i++) {
            arrows.get(i).setLineMode(lineMode);
        }
        return UpdateStatus.Success;
    }

    /**
     * All arrows are supposed to have the same line style
     * To be done: return an array of Integer
     * @return the arrow line style
     */
    public Integer getLineStyle() {
        Integer retLineStyle;

        retLineStyle = arrows.get(0).getLineStyle();
        return retLineStyle;
    }

    /**
     * All arrows are supposed to have the same line style
     * To be done: change the argument to an array of Integer
     * @param lineStyle the arrow line style to set
     */
    public UpdateStatus setLineStyle(Integer lineStyle) {
        for (int i = 0; i < arrows.size(); i++) {
            arrows.get(i).setLineStyle(lineStyle);
        }
        return UpdateStatus.Success;
    }

    /**
     * All arrows are supposed to have the same line style
     * To be done: return an array of LineType
     * @return the arrow line style
     */
    public LineType getLineStyleAsEnum() {
        LineType retLineStyle;

        retLineStyle = arrows.get(0).getLineStyleAsEnum();
        return retLineStyle;
    }

    /**
     * All arrows are supposed to have the same line thickness
     * To be done: return an array of Double
     * @return the arrow line thickness
     */
    public Double getLineThickness() {
        Double retLineThickness;

        retLineThickness = arrows.get(0).getLineThickness();
        return retLineThickness;
    }

    /**
     * All arrows are supposed to have the same line thickness
     * To be done: change the argument to an array of Double
     * @param lineThickness the arrow line thickness to set
     */
    public UpdateStatus setLineThickness(Double lineThickness) {
        for (int i = 0; i < arrows.size(); i++) {
            arrows.get(i).setLineThickness(lineThickness);
        }
        return UpdateStatus.Success;
    }

    /**
     * All arrows are supposed to have the same line color
     * To be done: return an array of Integer
     * @return the arrow line color
     */
    public Integer getLineColor() {
        Integer retLineColor;

        retLineColor = arrows.get(0).getLineColor();
        return retLineColor;
    }

    /**
     * All arrows are supposed to have the same line color
     * To be done: change the argument to an array of Integer
     * @param lineColor the arrow line color to set
     */
    public UpdateStatus setLineColor(Integer lineColor) {
        for (int i = 0; i < arrows.size(); i++) {
            arrows.get(i).setLineColor(lineColor);
        }
        return UpdateStatus.Success;
    }

}
