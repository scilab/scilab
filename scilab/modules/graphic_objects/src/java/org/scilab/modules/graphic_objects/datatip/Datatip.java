/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
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

package org.scilab.modules.graphic_objects.datatip;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DATATIP_DISPLAY_COMPONENTS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DATATIP_AUTOORIENTATION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DATATIP_BOX_MODE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DATATIP_DATA__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DATATIP_DISPLAY_FNC__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DATATIP_INTERP_MODE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DATATIP_LABEL_MODE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DATATIP_ORIENTATION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DATATIP_INDEXES__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DATATIP_DETACHED_MODE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DATATIP_DETACHED_POSITION__;

import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;

import org.scilab.forge.scirenderer.ruler.graduations.UserDefinedFormat;

import org.scilab.modules.action_binding.InterpreterManagement;
import static org.scilab.modules.graphic_objects.graphicObject.ClippableProperty.ClipStateType;
import org.scilab.modules.graphic_objects.PolylineData;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;
import org.scilab.modules.graphic_objects.textObject.Text;
import org.scilab.modules.localization.Messages;

public class Datatip extends Text {

    /** false = datatip text box is hidden*/
    Boolean tipBoxMode;
    /** false = datatip label is hidden*/
    Boolean tipLabelMode;
    /** selects which coordinates componet to diplay*/
    String displayComponents;
    /** false = no interpolation between point segments*/
    Boolean interpMode;
    /** Displayed number format*/
    DecimalFormat tipTextFormat;
    /** Display function*/
    String displayFnc;
    /** For automatic update the datatip orientation*/
    Boolean autoOrientation;
    /* index of data in parent object ( polyline, plot3d, fac3d*/
    /* size = 1 for polyline and 2 for others*/
    Integer dataIndex;
    Double ratio;


    enum DatatipObjectProperty { TIP_DATA, TIP_BOX_MODE, TIP_LABEL_MODE, TIP_ORIENTATION, TIP_AUTOORIENTATION, TIP_DISPLAY_COMPONENTS,
                                 TIP_INTERP_MODE, TIP_DISPLAY_FNC, TIP_INDEXES, TIP_DETACHED_MODE, TIP_DETACHED_POSITION
                               };

    enum TipOrientation { TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT, LEFT, RIGHT, TOP, BOTTOM;

                          /**
                           * Transform a integer to a TipOrientation enum.
                           */
    public static TipOrientation intToEnum(Integer i) {
        switch (i) {
            case 0:
                return TipOrientation.TOP_LEFT;
            case 1:
                return TipOrientation.TOP_RIGHT;
            case 2:
                return TipOrientation.BOTTOM_LEFT;
            case 3:
                return TipOrientation.BOTTOM_RIGHT;
            case 4:
                return TipOrientation.LEFT;
            case 5:
                return TipOrientation.RIGHT;
            case 6:
                return TipOrientation.TOP;
            case 7:
                return TipOrientation.BOTTOM;
            default:
                return TipOrientation.TOP_RIGHT;
        }
    }

                        };

    TipOrientation currentOrientation;

    Boolean detachedMode;
    Double[] detachedPosition;

    /**
     * Initializes the datatip, setup format, orientation and mark.
     */
    public Datatip() {
        super();
        displayComponents = "xy";
        autoOrientation = true;
        setOrientationAsEnum(TipOrientation.TOP_RIGHT);
        DecimalFormat fb = new DecimalFormat("#.####E00");
        DecimalFormatSymbols decimalFormatSymbols = fb.getDecimalFormatSymbols();
        decimalFormatSymbols.setDecimalSeparator('.');
        decimalFormatSymbols.setExponentSeparator("e");
        decimalFormatSymbols.setGroupingSeparator('\u00A0');
        fb.setDecimalFormatSymbols(decimalFormatSymbols);
        tipTextFormat = new UserDefinedFormat(fb, "%g", 1, 0);

        detachedMode = false;
        detachedPosition = new Double[] {0.0, 0.0, 0.0};

        tipBoxMode = true;
        tipLabelMode = true;
        interpMode = true;
        displayFnc = "";
        ratio = 0.;
        dataIndex = Integer.MIN_VALUE;
        setVisible(true);
        setBox(true);
        setLineMode(true);
        setLineStyle(3);
        setFillMode(true);
        setBackground(-2);
        setClipStateAsEnum(ClipStateType.OFF);

        setMarkMode(true);
        setMarkSize(8);
        setMarkBackground(-1);
        setMarkForeground(-1);
        setMarkStyle(11);
    }

    @Override
    public void accept(Visitor visitor) {
        visitor.visit(this);
    }

    /**
     * Convert the property name to the DatatipObjectProperty enum.
     */
    public Object getPropertyFromName(int propertyName) {
        switch (propertyName) {
            case __GO_DATATIP_DATA__:
                return DatatipObjectProperty.TIP_DATA;
            case __GO_DATATIP_BOX_MODE__:
                return DatatipObjectProperty.TIP_BOX_MODE;
            case __GO_DATATIP_LABEL_MODE__:
                return DatatipObjectProperty.TIP_LABEL_MODE;
            case __GO_DATATIP_ORIENTATION__:
                return DatatipObjectProperty.TIP_ORIENTATION;
            case __GO_DATATIP_DISPLAY_COMPONENTS__:
                return DatatipObjectProperty.TIP_DISPLAY_COMPONENTS;
            case __GO_DATATIP_AUTOORIENTATION__:
                return DatatipObjectProperty.TIP_AUTOORIENTATION;
            case __GO_DATATIP_INTERP_MODE__:
                return DatatipObjectProperty.TIP_INTERP_MODE;
            case __GO_DATATIP_DISPLAY_FNC__:
                return DatatipObjectProperty.TIP_DISPLAY_FNC;
            case __GO_DATATIP_INDEXES__ :
                return DatatipObjectProperty.TIP_INDEXES;
            case __GO_DATATIP_DETACHED_MODE__:
                return DatatipObjectProperty.TIP_DETACHED_MODE;
            case __GO_DATATIP_DETACHED_POSITION__:
                return DatatipObjectProperty.TIP_DETACHED_POSITION;
            default:
                return super.getPropertyFromName(propertyName);
        }
    }

    /**
     * @return the datatip property
     */
    public Object getProperty(Object property) {
        if (property instanceof DatatipObjectProperty) {
            switch ((DatatipObjectProperty) property) {
                case TIP_DATA:
                    return getTipData();
                case TIP_BOX_MODE:
                    return getTipBoxMode();
                case TIP_LABEL_MODE:
                    return getTipLabelMode();
                case TIP_ORIENTATION:
                    return getOrientation();
                case TIP_DISPLAY_COMPONENTS:
                    return getDisplayComponents();
                case TIP_AUTOORIENTATION:
                    return isAutoOrientationEnabled();
                case TIP_INTERP_MODE:
                    return getInterpMode();
                case TIP_DISPLAY_FNC:
                    return getDisplayFunction();
                case TIP_INDEXES:
                    return getIndexes();
                case TIP_DETACHED_MODE:
                    return getDetachedMode();
                case TIP_DETACHED_POSITION:
                    return getDetachedPosition();
            }
        }

        return super.getProperty(property);
    }

    /**
     * Set the datatip property
     * @param property the property
     * @param value the new property value.
     */
    public UpdateStatus setProperty(Object property, Object value) {
        if (property instanceof DatatipObjectProperty) {
            switch ((DatatipObjectProperty) property) {
                case TIP_BOX_MODE:
                    return setTipBoxMode((Boolean) value);
                case TIP_LABEL_MODE:
                    return setTipLabelMode((Boolean) value);
                case TIP_ORIENTATION:
                    return setOrientation((Integer) value);
                case TIP_DISPLAY_COMPONENTS:
                    return setDisplayComponents((String) value);
                case TIP_AUTOORIENTATION:
                    return setAutoOrientation((Boolean) value);
                case TIP_INTERP_MODE:
                    return setInterpMode((Boolean) value);
                case TIP_DISPLAY_FNC:
                    return setDisplayFunction((String) value);
                case TIP_INDEXES:
                    return setIndexes((Double[]) value);
                case TIP_DETACHED_MODE:
                    return setDetachedMode((Boolean)value);
                case TIP_DETACHED_POSITION:
                    return setDetachedPosition((Double[])value);
            }
        }

        return super.setProperty(property, value);
    }

    /**
     * Calculate the current tip data based on the given array
     * @param data the data array
     * @return the tip data
     */
    private double calcTipData(final double[] data) {
        if (data.length < dataIndex + 2) {
            if (data.length >= 1) {
                return data[data.length - 1];
            } else {
                return 0.0;
            }
        }

        //get pt0 and pt1 from polyline data
        final double pt0 = data[dataIndex];
        final double pt1 = data[dataIndex + 1];

        return pt0 + (pt1 - pt0) * ratio;
    }

    /**
     * Get the current tip data X
     * @return the tip data X
     */
    public double getTipDataX() {
        final double[] dataX = (double[]) PolylineData.getDataX(getParent());
        return calcTipData(dataX);
    }

    /**
     * Get the current tip data Y
     * @return the tip data Y
     */
    public double getTipDataY() {
        final double[] dataY = (double[]) PolylineData.getDataY(getParent());
        return calcTipData(dataY);
    }

    /**
     * Get the current tip data Z
     * @return the tip data Z
     */
    public double getTipDataZ() {
        final double[] dataZ = (double[]) PolylineData.getDataZ(getParent());
        return calcTipData(dataZ);
    }

    /**
     * Get the current tip data
     * @return the tip data
     */
    public Double[] getTipData() {
        return new Double[] {getTipDataX(), getTipDataY(), getTipDataZ()};
    }

    /**
     * Get the current textbox orientation as an integer
     * @return current orientation.
     */
    public Integer getOrientation() {
        return getOrientationAsEnum().ordinal();
    }

    /**
     * Get the current oriantation as a enum
     * @return the current orientation
     */
    public TipOrientation getOrientationAsEnum() {
        return currentOrientation;
    }

    /**
     * Set the current orientation (updating the text position)
     * @param orientation the new orientation (integer).
     */
    public UpdateStatus setOrientation(Integer orientation) {
        currentOrientation = TipOrientation.intToEnum(orientation);
        return UpdateStatus.Success;
    }

    /**
     * Set the current orientation (updating the text position)
     * @param orientation the new orientation (TipOrientation enum).
     */
    public UpdateStatus setOrientationAsEnum(TipOrientation orientation) {
        currentOrientation = orientation;
        return UpdateStatus.Success;
    }

    /**
     * Get which components should be displayed.
     * @return the string containing the coordinates to display.
     */
    public String getDisplayComponents() {
        return displayComponents;
    }

    /**
     * Checks if a given string is valid for the display components
     * @return true if it is valid, false otherwise.
     */
    private boolean isValidComponent(String value) {
        if (value.length() < 1 || value.length() > 3) {
            return false;
        }

        boolean isXSet = false;
        boolean isYSet = false;
        boolean isZSet = false;
        for (int i = 0; i < value.length(); ++i) {
            if (value.charAt(i) == 'x' && !isXSet) {
                isXSet = true;
                continue;
            }
            if (value.charAt(i) == 'y' && !isYSet) {
                isYSet = true;
                continue;
            }
            if (value.charAt(i) == 'z' && !isZSet) {
                isZSet = true;
                continue;
            }
            return false;
        }
        return true;
    }

    /**
     * Set which components to display if @disp is valid.
     * @param disp The string containing which components to display
     */
    public UpdateStatus setDisplayComponents(String disp) {
        if (disp == null) {
            return UpdateStatus.Fail;
        }
        disp = disp.toLowerCase();
        if (!isValidComponent(disp)) {
            return UpdateStatus.Fail;
        }
        displayComponents = disp;
        updateText();
        return UpdateStatus.Success;
    }


    public Boolean isAutoOrientationEnabled() {
        return autoOrientation;
    }

    public UpdateStatus setAutoOrientation(Boolean status) {
        autoOrientation = status;
        return UpdateStatus.Success;
    }

    /**
     * Get tip formated text for the given index
     * @param index the component index
     * @return the formated string
     */
    private String getComponentFormatedText(int index) {
        switch (displayComponents.charAt(index)) {
            case 'x':
                return "X:" + tipTextFormat.format(getTipDataX());
            case 'y':
                return "Y:" + tipTextFormat.format(getTipDataY());
            case 'z':
                return "Z:" + tipTextFormat.format(getTipDataZ());
            default:
                return "";
        }
    }

    /**
     * Update the text from the datatip base on current tipData value.
     */
    public void updateText() {
        //if display function is empty look in parent
        //if parent is empty too use default print

        String fnc = getDisplayFunction();
        if (fnc == null || fnc.equals("")) {
            //look in parent
            fnc = (String) GraphicController.getController().getProperty(getParent(), GraphicObjectProperties.__GO_DATATIP_DISPLAY_FNC__);
            if (fnc == null || fnc.equals("")) {
                int numCoords = displayComponents.length();
                String[] textArray = new String[numCoords];
                for (int i = 0; i < numCoords; ++i) {
                    textArray[i] = getComponentFormatedText(i);
                }
                Integer[] dim = new Integer[2];
                dim[0] = numCoords;
                dim[1] = 1;
                setTextArrayDimensions(dim);
                setTextStrings(textArray);
                return;
            }
        }

        String errMsg =  Messages.gettext("Wrong value for '%s' property: A valid function name expected.\n");
        errMsg = errMsg.replace("'", "''");
        errMsg = errMsg.replace("\n", "\\n");
        String updateCommand = "try;" +
                               "GDZa786XBSq7899SHKp=getcallbackobject(" + getIdentifier() + ");" +
                               "set(GDZa786XBSq7899SHKp,\"text\"," + fnc + "(GDZa786XBSq7899SHKp));" +
                               "clear(\"GDZa786XBSq7899SHKp\");" +
                               "catch;" +
                               "set(GDZa786XBSq7899SHKp,\"display_function\",\"\");" +
                               "set(GDZa786XBSq7899SHKp.parent,\"display_function\",\"\");" +
                               "clear(\"GDZa786XBSq7899SHKp\");" +
                               "error(msprintf(\"" + errMsg + "\", \"display_function\"));" +
                               "end;";
        InterpreterManagement.requestScilabExec(updateCommand);
    }

    public Boolean getTipBoxMode() {
        return tipBoxMode;
    }

    public Boolean getTipLabelMode() {
        return tipLabelMode;
    }

    public Boolean getInterpMode() {
        return interpMode;
    }

    public String getDisplayFunction() {
        return displayFnc;
    }

    public UpdateStatus setTipBoxMode(Boolean mode) {
        tipBoxMode = mode;
        setBox(tipBoxMode);
        return UpdateStatus.Success;
    }

    public UpdateStatus setTipLabelMode(Boolean mode) {
        tipLabelMode = mode;
        return UpdateStatus.Success;
    }

    public UpdateStatus setInterpMode(Boolean mode) {
        interpMode = mode;
        return UpdateStatus.Success;
    }

    public UpdateStatus setDisplayFunction(String fnc) {
        displayFnc = fnc;
        updateText();
        return UpdateStatus.Success;
    }

    @Override
    public Double[] getPosition() {
        return getTipData();
    }

    public UpdateStatus setIndexes(Double[] value) {
        if (value[0].intValue() != dataIndex || !value[1].equals(ratio)) {
            dataIndex = value[0].intValue();
            ratio = new Double(value[1]);
            updateText();

            return UpdateStatus.Success;
        }

        return UpdateStatus.NoChange;
    }

    public Integer getIndexes() {
        return dataIndex;
    }

    /**
     * @return detached mode
     */
    public Boolean getDetachedMode() {
        return detachedMode;
    }

    /**
     * Sets the detached mode, when true datatip position
     * is given by detachedPosition.
     * @param status true for enable detached mode
     */
    public UpdateStatus setDetachedMode(Boolean status) {
        detachedMode = status;
        return UpdateStatus.Success;
    }

    /**
     * Position is only used when detachedMode is true.
     * @return datatip detached position
     */
    public Double[] getDetachedPosition() {
        return detachedPosition;
    }

    /**
     * Sets the datatip detached position
     * @param pos detached position
     */
    public UpdateStatus setDetachedPosition(Double[] pos) {
        if (pos.length != 3) {
            return UpdateStatus.Fail;
        }
        for (int i = 0; i < 3; ++i) {
            detachedPosition[i] = pos[i];
        }
        return UpdateStatus.Success;
    }

    /**
     * @return Type as String
     */
    public Integer getType() {
        return GraphicObjectProperties.__GO_DATATIP__;
    }
}
