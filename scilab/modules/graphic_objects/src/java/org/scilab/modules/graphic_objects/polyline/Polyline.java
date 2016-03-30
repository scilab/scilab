/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Manuel JULIACHS
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

package org.scilab.modules.graphic_objects.polyline;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_ARROW_SIZE_FACTOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_BAR_WIDTH__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CLOSED__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DATATIPS_COUNT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DATATIPS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DATATIP_DISPLAY_FNC__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DATATIP_MARK__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_INTERP_COLOR_MODE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_INTERP_COLOR_VECTOR_SET__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_INTERP_COLOR_VECTOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POLYLINE_STYLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_X_SHIFT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_Y_SHIFT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_Z_SHIFT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_COLOR_SET__;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;

import org.scilab.modules.graphic_objects.ObjectRemovedException;
import org.scilab.modules.graphic_objects.contouredObject.ClippableContouredObject;
import org.scilab.modules.graphic_objects.datatip.Datatip;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;

/**
 * Polyline class
 * @author Manuel JULIACHS
 */
public class Polyline extends ClippableContouredObject {
    /** TBD: data */
    // Data data -> Data Model
    /* TODO: properties relative to the data model */
    /** Polyline properties names */
    private enum PolylineProperty { CLOSED, ARROWSIZEFACTOR, POLYLINESTYLE,
                                    INTERPCOLORVECTOR, INTERPCOLORVECTORSET, INTERPCOLORMODE,
                                    XSHIFT, YSHIFT, ZSHIFT, BARWIDTH, DATATIPS, DATATIPSCOUNT,
                                    TIP_DISPLAY_FNC, TIP_MARK, COLORSET
                                  };

    /** Specifies whether the polyline is closed */
    private boolean closed;

    /** Determines the arrow size */
    private double arrowSizeFactor;

    /** Polyline drawing style (normal, staircase, , etc.) */
    private int polylineStyle;

    /** Interpolation color vector (3- or 4-element array) */
    private int[] interpColorVector;

    /** Specifies whether the interpolation color vector has been set */
    private boolean interpColorVectorSet;

    /** Specifies whether interpolated shading is used */
    private boolean interpColorMode;

    /** Shift applied to the line points along the x-axis */
    private double[] xShift;

    /** Shift applied to the line points along the y-axis */
    private double[] yShift;

    /** Shift applied to the line points along the z-axis */
    private double[] zShift;

    /** Bar width */
    private double barWidth;

    /** Datatips objects list */
    private List<Integer> datatips;

    /** Display function*/
    private String displayFnc;

    private Integer tipMark;

    /** has color set */
    private boolean colorSet;

    /** Constructor */
    public Polyline() {
        super();
        closed = false;
        arrowSizeFactor = 1.0;
        polylineStyle = 1;
        interpColorVector = new int[4];
        interpColorVectorSet = false;
        interpColorMode = false;
        xShift = null;
        yShift = null;
        zShift = null;
        barWidth = 0.0;
        datatips = new ArrayList<Integer>();
        displayFnc = "";
        tipMark = 11;
        colorSet = false;
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
            case __GO_CLOSED__ :
                return PolylineProperty.CLOSED;
            case __GO_ARROW_SIZE_FACTOR__ :
                return PolylineProperty.ARROWSIZEFACTOR;
            case __GO_POLYLINE_STYLE__ :
                return PolylineProperty.POLYLINESTYLE;
            case __GO_INTERP_COLOR_VECTOR__ :
                return PolylineProperty.INTERPCOLORVECTOR;
            case __GO_INTERP_COLOR_VECTOR_SET__ :
                return PolylineProperty.INTERPCOLORVECTORSET;
            case __GO_INTERP_COLOR_MODE__ :
                return PolylineProperty.INTERPCOLORMODE;
            case __GO_X_SHIFT__ :
                return PolylineProperty.XSHIFT;
            case __GO_Y_SHIFT__ :
                return PolylineProperty.YSHIFT;
            case __GO_Z_SHIFT__ :
                return PolylineProperty.ZSHIFT;
            case __GO_BAR_WIDTH__ :
                return PolylineProperty.BARWIDTH;
            case __GO_DATATIPS__ :
                return PolylineProperty.DATATIPS;
            case __GO_DATATIPS_COUNT__ :
                return PolylineProperty.DATATIPSCOUNT;
            case __GO_DATATIP_DISPLAY_FNC__ :
                return PolylineProperty.TIP_DISPLAY_FNC;
            case __GO_DATATIP_MARK__ :
                return PolylineProperty.TIP_MARK;
            case __GO_COLOR_SET__ :
                return PolylineProperty.COLORSET;
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
        if (property instanceof PolylineProperty) {
            switch ((PolylineProperty) property) {
                case CLOSED:
                    return getClosed();
                case ARROWSIZEFACTOR:
                    return getArrowSizeFactor();
                case POLYLINESTYLE:
                    return getPolylineStyle();
                case INTERPCOLORVECTOR:
                    return getInterpColorVector();
                case INTERPCOLORVECTORSET:
                    return getInterpColorVectorSet();
                case INTERPCOLORMODE:
                    return getInterpColorMode();
                case XSHIFT:
                    return getXShift();
                case YSHIFT:
                    return getYShift();
                case ZSHIFT:
                    return getZShift();
                case BARWIDTH:
                    return getBarWidth();
                case DATATIPS:
                    return getDatatips();
                case DATATIPSCOUNT:
                    return datatips.size();
                case TIP_DISPLAY_FNC:
                    return getDisplayFunction();
                case TIP_MARK:
                    return getTipMark();
                case COLORSET:
                    return getColorSet();
            }
        }
        return super.getProperty(property);
    }

    /**
     * Fast property set method
     * @param property the property to set
     * @param value the property value
     * @return true if the property has been set, false otherwise
     */
    public UpdateStatus setProperty(Object property, Object value) {
        synchronized (this) {
            if (property instanceof PolylineProperty) {
                switch ((PolylineProperty) property) {
                    case CLOSED:
                        setClosed((Boolean) value);
                        break;
                    case ARROWSIZEFACTOR:
                        return setArrowSizeFactor((Double) value);
                    case POLYLINESTYLE:
                        return setPolylineStyle((Integer) value);
                    case INTERPCOLORVECTOR:
                        setInterpColorVector((Integer[]) value);
                        break;
                    case INTERPCOLORVECTORSET:
                        setInterpColorVectorSet((Boolean) value);
                        break;
                    case INTERPCOLORMODE:
                        return setInterpColorMode((Boolean) value);
                    case XSHIFT:
                        setXShift((double[]) value);
                        break;
                    case YSHIFT:
                        setYShift((double[]) value);
                        break;
                    case ZSHIFT:
                        setZShift((double[]) value);
                        break;
                    case BARWIDTH:
                        setBarWidth((Double) value);
                        break;
                    case DATATIPS:
                        setDatatips((Integer[]) value);
                        break;
                    case DATATIPSCOUNT:
                        // nothing should be done
                        break;
                    case TIP_DISPLAY_FNC:
                        setDisplayFunction((String) value);
                        break;
                    case TIP_MARK:
                        setTipMark((Integer) value);
                        break;
                    case COLORSET:
                        setColorSet((Boolean) value);
                        break;
                }
            }
            return super.setProperty(property, value);
        }
    }

    /**
     * @return the arrowSizeFactor
     */
    public Double getArrowSizeFactor() {
        return arrowSizeFactor;
    }

    /**
     * @param arrowSizeFactor the arrowSizeFactor to set
     */
    public UpdateStatus setArrowSizeFactor(Double arrowSizeFactor) {
        if (this.arrowSizeFactor == arrowSizeFactor) {
            return UpdateStatus.NoChange;
        }
        this.arrowSizeFactor = arrowSizeFactor;
        return UpdateStatus.Success;
    }

    /**
     * @return the barWidth
     */
    public Double getBarWidth() {
        return barWidth;
    }

    /**
     * @param barWidth the barWidth to set
     */
    public UpdateStatus setBarWidth(Double barWidth) {
        this.barWidth = barWidth;
        return UpdateStatus.Success;
    }

    /**
     * @return the closed
     */
    public Boolean getClosed() {
        return closed;
    }

    /**
     * @param closed the closed to set
     */
    public UpdateStatus setClosed(Boolean closed) {
        this.closed = closed;
        return UpdateStatus.Success;
    }

    /**
     * @return the interpColorMode
     */
    public Boolean getInterpColorMode() {
        return interpColorMode;
    }

    /**
     * @param interpColorMode the interpColorMode to set
     */
    public UpdateStatus setInterpColorMode(Boolean interpColorMode) {
        if (this.interpColorMode == interpColorMode) {
            return UpdateStatus.NoChange;
        }
        this.interpColorMode = interpColorMode;
        return UpdateStatus.Success;
    }

    /**
     * @return the interpColorVector
     */
    public Integer[] getInterpColorVector() {
        Integer[] retVector = new Integer[interpColorVector.length];
        for (int i = 0; i < interpColorVector.length; i++) {
            retVector[i] = interpColorVector[i];
        }

        return retVector;
    }

    /**
     * @param interpColorVector the interpColorVector to set
     */
    public UpdateStatus setInterpColorVector(Integer[] interpColorVector) {
        if (interpColorVectorSet == false) {
            interpColorVectorSet = true;
        }

        for (int i = 0; i < interpColorVector.length; i++) {
            this.interpColorVector[i] = interpColorVector[i];
        }
        return UpdateStatus.Success;
    }

    /**
     * @return the interpColorVectorSet
     */
    public Boolean getInterpColorVectorSet() {
        return interpColorVectorSet;
    }

    /**
     * @param interpColorVectorSet the interpColorVectorSet to set
     */
    public UpdateStatus setInterpColorVectorSet(Boolean interpColorVectorSet) {
        this.interpColorVectorSet = interpColorVectorSet;
        return UpdateStatus.Success;
    }

    /**
     * @return the polylineStyle
     */
    public Integer getPolylineStyle() {
        return polylineStyle;
    }

    /**
     * @param polylineStyle the polylineStyle to set
     */
    public UpdateStatus setPolylineStyle(Integer polylineStyle) {
        if (this.polylineStyle == polylineStyle) {
            return UpdateStatus.NoChange;
        }
        this.polylineStyle = polylineStyle;
        return UpdateStatus.Success;
    }

    /**
     * @return the xShift
     */
    public double[] getXShift() {
        return xShift;
    }

    /**
     * @param shift the xShift to set
     */
    public UpdateStatus setXShift(double[] shift) {
        xShift = shift;
        return UpdateStatus.Success;
    }

    /**
     * @return the yShift
     */
    public double[] getYShift() {
        return yShift;
    }

    /**
     * @param shift the yShift to set
     */
    public UpdateStatus setYShift(double[] shift) {
        yShift = shift;
        return UpdateStatus.Success;
    }

    /**
     * @return the zShift
     */
    public double[] getZShift() {
        return zShift;
    }

    /**
     * @param shift the zShift to set
     */
    public UpdateStatus setZShift(double[] shift) {
        zShift = shift;
        return UpdateStatus.Success;
    }

    /**
     * @return datatips
     */
    public Integer[] getDatatips() {
        return datatips.toArray(new Integer[datatips.size()]);
    }

    /**
     * @param datatips the datatips to set
     */
    public UpdateStatus setDatatips(Integer[] datatips) {
        this.datatips = new LinkedList<Integer>(Arrays.asList(datatips));
        return UpdateStatus.Success;
    }

    public String getDisplayFunction() {
        return displayFnc;
    }

    public UpdateStatus setDisplayFunction(String fnc) {
        GraphicController controller =  GraphicController.getController();
        displayFnc = fnc;
        //update datatips
        for (int i = 0 ; i < datatips.size() ; i++) {
            Datatip tip = (Datatip) controller.getObjectFromId(datatips.get(i));
            tip.updateText();
        }
        return UpdateStatus.Success;
    }

    public Integer getTipMark() {
        return tipMark;
    }

    public UpdateStatus setTipMark(Integer tipMark) {
        if (!this.tipMark.equals(tipMark)) {
            this.tipMark = tipMark;

            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }

    /**
     * @return the colorSet
     */
    public Boolean getColorSet() {
        return colorSet;
    }

    /**
     * @param colorSet the colorSet to set
     */
    public UpdateStatus setColorSet(Boolean colorSet) {
        this.colorSet = colorSet;
        return UpdateStatus.Success;
    }

    /**
     * @return Type as String
     */
    public Integer getType() {
        return GraphicObjectProperties.__GO_POLYLINE__;
    }
}
