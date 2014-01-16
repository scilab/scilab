/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.graphic_objects.datatip;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DATATIP_3COMPONENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DATATIP_AUTOORIENTATION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DATATIP_BOX_MODE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DATATIP_DATA__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DATATIP_DISPLAY_FNC__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DATATIP_INTERP_MODE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DATATIP_LABEL_MODE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DATATIP_ORIENTATION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DATATIP_INDEXES__;

import java.text.DecimalFormat;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.graphic_objects.PolylineData;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;
import org.scilab.modules.graphic_objects.textObject.Text;

public class Datatip extends Text {

    /** false = datatip text box is hidden*/
    Boolean tipBoxMode;
    /** false = datatip label is hidden*/
    Boolean tipLabelMode;
    /** false = display only (X, Y), true = display (X, Y, Z)*/
    Boolean use3component;
    /** false = no interpolation between point segments*/
    Boolean interpMode;
    /** Displayed number format*/
    DecimalFormat tipTextFormat;
    /** Display function*/
    String displayFnc;
    /** For automatic update the datatip orientation*/
    Boolean autoOrientation;
    /* index of data in parent objec t ( polyline, plot3d, fac3d*/
    /* size = 1 for polyline and 2 for others*/
    Integer[] dataIndex;
    Double[] ratio;


    enum DatatipObjectProperty { TIP_DATA, TIP_BOX_MODE, TIP_LABEL_MODE, TIP_ORIENTATION, TIP_AUTOORIENTATION, TIP_3COMPONENT, TIP_INTERP_MODE, TIP_DISPLAY_FNC, TIP_INDEXES};
    enum TipOrientation { TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT;

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
            default:
                return TipOrientation.TOP_RIGHT;
        }
    }

                        };

    TipOrientation currentOrientation;

    /**
     * Initializes the datatip, setup format, orientation and mark.
     */
    public Datatip() {
        super();
        use3component = false;
        autoOrientation = true;
        setOrientationAsEnum(TipOrientation.TOP_RIGHT);
        tipTextFormat = new DecimalFormat("#.####");
        tipBoxMode = true;
        tipLabelMode = true;
        interpMode = true;
        displayFnc = "";
        ratio = new Double[] {0.0, 0.0};
        dataIndex = new Integer[] {0, 0};
        setVisible(true);
        setBox(true);
        setLineMode(true);
        setFillMode(true);
        setBackground(-2);
        setClipState(1);

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
            case __GO_DATATIP_3COMPONENT__:
                return DatatipObjectProperty.TIP_3COMPONENT;
            case __GO_DATATIP_AUTOORIENTATION__:
                return DatatipObjectProperty.TIP_AUTOORIENTATION;
            case __GO_DATATIP_INTERP_MODE__:
                return DatatipObjectProperty.TIP_INTERP_MODE;
            case __GO_DATATIP_DISPLAY_FNC__:
                return DatatipObjectProperty.TIP_DISPLAY_FNC;
            case __GO_DATATIP_INDEXES__ :
                return DatatipObjectProperty.TIP_INDEXES;
            default:
                return super.getPropertyFromName(propertyName);
        }
    }

    /**
     * @return the datatip property
     */
    public Object getProperty(Object property) {
        if (property == DatatipObjectProperty.TIP_DATA) {
            return getTipData();
        } else if (property == DatatipObjectProperty.TIP_BOX_MODE) {
            return getTipBoxMode();
        } else if (property == DatatipObjectProperty.TIP_LABEL_MODE) {
            return getTipLabelMode();
        } else if (property == DatatipObjectProperty.TIP_ORIENTATION) {
            return getOrientation();
        } else if (property == DatatipObjectProperty.TIP_3COMPONENT) {
            return isUsing3Component();
        } else if (property == DatatipObjectProperty.TIP_AUTOORIENTATION) {
            return isAutoOrientationEnabled();
        } else if (property == DatatipObjectProperty.TIP_INTERP_MODE) {
            return getInterpMode();
        } else if (property == DatatipObjectProperty.TIP_DISPLAY_FNC) {
            return getDisplayFunction();
        } else if (property == DatatipObjectProperty.TIP_INDEXES) {
            return getIndexes();
        } else {
            return super.getProperty(property);
        }
    }

    /**
     * Set the datatip property
     * @param property the property
     * @param value the new property value.
     */
    public UpdateStatus setProperty(Object property, Object value) {
        if (property == DatatipObjectProperty.TIP_BOX_MODE) {
            setTipBoxMode((Boolean) value);
        } else if (property == DatatipObjectProperty.TIP_LABEL_MODE) {
            setTipLabelMode((Boolean) value);
        } else if (property == DatatipObjectProperty.TIP_ORIENTATION) {
            setOrientation((Integer) value);
        } else if (property == DatatipObjectProperty.TIP_3COMPONENT) {
            setUse3Component((Boolean)value);
        } else if (property == DatatipObjectProperty.TIP_AUTOORIENTATION) {
            setAutoOrientation((Boolean)value);
        } else if (property == DatatipObjectProperty.TIP_INTERP_MODE) {
            setInterpMode((Boolean) value);
        } else if (property == DatatipObjectProperty.TIP_DISPLAY_FNC) {
            setDisplayFunction((String) value);
        } else if (property == DatatipObjectProperty.TIP_INDEXES) {
            setIndexes((Double[]) value);
        } else {
            return super.setProperty(property, value);
        }

        return UpdateStatus.Success;
    }

    /**
     * Get the current tip data
     * @return the tip data
     */
    public Double[] getTipData() {

        double x = 0.0;
        double y = 0.0;
        double z = 0.0;
        //compute values from dataIndex and ratio
        if (dataIndex.length == 1) {
            double[] dataX = (double[])PolylineData.getDataX(getParent());
            double[] dataY = (double[])PolylineData.getDataY(getParent());

            //get pt0 and pt1 from polyline data
            double[] pt0 = new double[] {dataX[dataIndex[0]], dataY[dataIndex[0]]};
            double[] pt1 = new double[] {dataX[dataIndex[0] + 1], dataY[dataIndex[0] + 1]};

            //coefficient
            double a = (pt1[1] - pt0[1]) / (pt1[0] - pt0[0]);
            //origin
            double orig = pt0[1] - a * pt0[0];

            //y = a * x + b
            x = pt0[0] + ((pt1[0] - pt0[0]) * ratio[0]);
            y = a * x + orig;
        } else if (dataIndex.length == 2) {
            //    		double[] dataX = (double[])PolylineData.getDataX(getParent());
            //    		double[] dataY = (double[])PolylineData.getDataY(getParent());
            //    		double[] dataZ = (double[])PolylineData.getDataZ(getParent());
        }

        return new Double[] {x, y, z};
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
     * @return true if the datatip is displaying the Z component, false otherwise.
     */
    public Boolean isUsing3Component() {
        return use3component;
    }

    /**
     * If true set the Z component to be displayed.
     * @param useZ True to enable display the Z component, false to disable.
     */
    public UpdateStatus setUse3Component(Boolean useZ) {
        use3component = useZ;
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
                String[] textArray = new String[] {"X:", "Y:", "Z:"};
                Double[] tipData = getTipData();
                textArray[0] += tipTextFormat.format(tipData[0]);
                textArray[1] += tipTextFormat.format(tipData[1]);
                textArray[2] += tipTextFormat.format(tipData[2]);

                Integer[] dim = new Integer[2];
                dim[0] = use3component ? 3 : 2;
                dim[1] = 1;
                setTextArrayDimensions(dim);
                setTextStrings(textArray);
                return;
            }
        }

        String updateCommand = "try;" +
                               "d = getcallbackobject(" + getIdentifier() + ");" +
                               "d.text = " + fnc + "(d.data);" +
                               "clear(\"d\");" +
                               "catch;" +
                               "d.tip_disp_function = \"\";" +
                               "clear(\"d\");" +
                               "error(msprintf(_( \"%s: Wrong name of input argument #%d: Function ''%s'' not defined.\n\"),\"datatipSetDisplay\",2,\"" + fnc + "\"));" +
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
        //if length == 3
        //dataIndexX, ratioX, ratioY
        //if length == 4
        //dataIndexX, dataIndexY, ratioX, ratioY

        UpdateStatus status = UpdateStatus.NoChange;

        //check dimensions of dataIndex and values to know if update is needed
        if (value.length == 3) {
            if (dataIndex.length != 1) {
                status = UpdateStatus.Success;
            } else {
                if (dataIndex[0] != value[0].intValue()) {
                    status = UpdateStatus.Success;
                }
            }
        } else if (value.length == 4) {
            if (dataIndex.length != 2) {
                status = UpdateStatus.Success;
            } else {
                if (dataIndex[0] != value[0].intValue() || dataIndex[1] != value[1].intValue()) {
                    status = UpdateStatus.Success;
                }
            }
        }

        //check values of ration to know if update is needed
        if (value[value.length - 2] != ratio[0] || value[value.length - 1] != ratio[1]) {
            status = UpdateStatus.Success;
        }

        //update dataIndex
        if (status == UpdateStatus.Success) {
            if (value.length == 3) {
                dataIndex = new Integer[] {value[0].intValue()};
            } else if (value.length == 4) {
                dataIndex = new Integer[] {value[0].intValue(), value[1].intValue()};
            }

            ratio = new Double[] {value[value.length - 2], value[value.length - 1]};
            updateText();
        }

        return status;
    }

    public Integer[] getIndexes() {
        Integer[] value = new Integer[dataIndex.length];
        for (int i = 0 ; i < dataIndex.length ; i++) {
            value[i] = dataIndex[i];
        }
        return value;
    }

    /**
     * @return Type as String
     */
    public Integer getType() {
        return GraphicObjectProperties.__GO_DATATIP__;
    }


}
