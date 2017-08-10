/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
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

package org.scilab.modules.gui.utils;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_AXES_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FIGURE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_PARENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TYPE__;

import java.awt.Component;
import java.awt.Dimension;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.bridge.CallScilabBridge;
import org.scilab.modules.gui.widget.Widget;

/**
 * Class used to convert uicontrols positions according to units property
 * @author Vincent COUVERT
 */
public final class UnitsConverter {

    private static double POINT_PER_INCH = 72.0;
    private static double CM_PER_INCH = 2.54;
    protected static final String __GO_UI_POINTS_UNITS__ =        "points";
    protected static final String __GO_UI_NORMALIZED_UNITS__ =    "normalized";
    protected static final String __GO_UI_INCHES_UNITS__ =        "inches";
    protected static final String __GO_UI_CENTIMETERS_UNITS__ =   "centimeters";
    protected static final String __GO_UI_PIXELS_UNITS__ =        "pixels";

    public enum UicontrolUnits { /* Needed in SwingScilabWidget */
        POINTS,
        NORMALIZED,
        INCHES,
        CENTIMETERS,
        PIXELS
    };

    public static UicontrolUnits stringToUnitsEnum(String style) {
        if (style.compareTo(__GO_UI_POINTS_UNITS__) == 0) {
            return UicontrolUnits.POINTS;
        }

        if (style.compareTo(__GO_UI_NORMALIZED_UNITS__) == 0) {
            return UicontrolUnits.NORMALIZED;
        }

        if (style.compareTo(__GO_UI_INCHES_UNITS__) == 0) {
            return UicontrolUnits.INCHES;
        }

        if (style.compareTo(__GO_UI_CENTIMETERS_UNITS__) == 0) {
            return UicontrolUnits.CENTIMETERS;
        }

        if (style.compareTo(__GO_UI_PIXELS_UNITS__) == 0) {
            return UicontrolUnits.PIXELS;
        }
        return null;
    }

    public static double convertFromPoint(double value, UicontrolUnits newUnit, Widget uicontrol, boolean widthAsRef) {
        int refSize = 0;

        SwingViewObject uicontrolAsView = (SwingViewObject) uicontrol;
        int widgetType = (Integer) GraphicController.getController().getProperty(uicontrolAsView.getId(), __GO_TYPE__);

        /* Get the component height from java */
        if (widgetType == __GO_FIGURE__) { /* Figure */
            newUnit = UicontrolUnits.PIXELS;
        }

        switch (newUnit) {
            case POINTS:
                return value;
            case NORMALIZED:
                Component comp = (Component)uicontrol;
                Dimension dims = comp.getParent().getSize();
                if (widthAsRef) {
                    refSize = dims.width;
                } else {
                    refSize = dims.height;
                }

                return value / (refSize * POINT_PER_INCH / CallScilabBridge.getScreenResolution());
            case INCHES:
                return value / POINT_PER_INCH;
            case CENTIMETERS:
                return value * CM_PER_INCH / POINT_PER_INCH;
            case PIXELS:
                return value * CallScilabBridge.getScreenResolution() / POINT_PER_INCH;
            default:
                return 0.0;
        }
    }

    public static double convertToPoint(double value, UicontrolUnits oldUnit, Widget uicontrol, boolean widthAsRef) {
        int refSize = 0;

        SwingViewObject uicontrolAsView = (SwingViewObject) uicontrol;
        int widgetType = (Integer) GraphicController.getController().getProperty(uicontrolAsView.getId(), __GO_TYPE__);

        /* Get the component height from java */
        if (widgetType == __GO_FIGURE__) { // Figure
            oldUnit = UicontrolUnits.PIXELS;
        }

        switch (oldUnit) {
            case POINTS:
                return (int) value;
            case NORMALIZED:
                Component comp = (Component)uicontrol;
                Dimension dims = comp.getParent().getSize();
                if (widthAsRef) {
                    refSize = dims.width;
                } else {
                    refSize = dims.height;
                }
                return (value * refSize * POINT_PER_INCH / CallScilabBridge.getScreenResolution());
            case INCHES:
                return (value * POINT_PER_INCH);
            case CENTIMETERS:
                return (value * POINT_PER_INCH / CM_PER_INCH);
            case PIXELS:
                return Math.round((value * POINT_PER_INCH / CallScilabBridge.getScreenResolution()));
            default:
                return 0;
        }
    }

    public static double convertFromPixel(int value, UicontrolUnits newUnit, Widget uicontrol, boolean widthAsRef) {
        if (uicontrol == null) {
            return (double) value;
        }

        SwingViewObject uicontrolAsView = (SwingViewObject) uicontrol;
        int widgetType = (Integer) GraphicController.getController().getProperty(uicontrolAsView.getId(), __GO_TYPE__);

        if ((uicontrol != null) && (widgetType == __GO_FIGURE__)) {/* Uicontrol figure */
            newUnit = UicontrolUnits.PIXELS;
        }

        if (newUnit == UicontrolUnits.PIXELS) {
            return (double) value;
        } else {
            return convertFromPoint(convertToPoint(value, UicontrolUnits.PIXELS, uicontrol, widthAsRef), newUnit, uicontrol, widthAsRef);
        }
    }

    public static int convertToPixel(double value, UicontrolUnits oldUnit, Widget uicontrol, boolean widthAsRef) {

        SwingViewObject uicontrolAsView = (SwingViewObject) uicontrol;
        int widgetType = (Integer) GraphicController.getController().getProperty(uicontrolAsView.getId(), __GO_TYPE__);

        if ((uicontrol != null) && (widgetType == __GO_FIGURE__)) { /* Uicontrol figure */
            oldUnit = UicontrolUnits.PIXELS;
        }

        if (oldUnit == UicontrolUnits.PIXELS) {
            return (int) value;
        } else {
            return (int) convertFromPoint(convertToPoint(value, oldUnit, uicontrol, widthAsRef), UicontrolUnits.PIXELS, uicontrol, widthAsRef);
        }
    }

    public static Double[] convertPositionToPixels(Double[] position, UicontrolUnits oldUnit, Widget uicontrol) {
        Double[] convertedPosition = new Double[4];
        convertedPosition[0] = new Double(convertToPixel(position[0].doubleValue(), oldUnit, uicontrol, true));
        convertedPosition[1] = new Double(convertToPixel(position[1].doubleValue(), oldUnit, uicontrol, false));
        convertedPosition[2] = new Double(convertToPixel(position[2].doubleValue(), oldUnit, uicontrol, true));
        convertedPosition[3] = new Double(convertToPixel(position[3].doubleValue(), oldUnit, uicontrol, false));
        return convertedPosition;
    }

}
