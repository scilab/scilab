/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_PARENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_STYLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_BACKGROUNDCOLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ENABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTANGLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTNAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTSIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTUNITS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTWEIGHT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FOREGROUNDCOLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_HORIZONTALALIGNMENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LISTBOXTOP__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MAX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MIN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_RELIEF__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SCALE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SHEAR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SLIDERSTEP__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_UNITS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VALUE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VERTICALALIGNMENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_VISIBLE__;

import java.awt.Color;
import java.awt.Font;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.gui.bridge.imagerenderer.SwingScilabImageRenderer;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.utils.UnitsConverter;
import org.scilab.modules.gui.utils.UnitsConverter.UicontrolUnits;
import org.scilab.modules.gui.widget.Widget;

public class SwingScilabWidget {

    private static final String OBLIQUEFONT = "oblique";
    private static final String ITALICFONT = "italic";
    private static final String BOLDFONT = "bold";

    public static void update(Widget uiControl, String property, Object value) {
    	if (property.equals(__GO_UI_BACKGROUNDCOLOR__)) {
    		Double[] allColors = ((Double[]) value);
    		uiControl.setBackground(new Color((int) (allColors[0]*255), (int) (allColors[1]*255), (int) (allColors[2]*255)));
    	} else if (property.equals(__GO_UI_ENABLE__)) {
    		uiControl.setEnabled(((Boolean) value).booleanValue());
    	} else if (property.equals(__GO_UI_FONTANGLE__)) {
            Font font = uiControl.getFont();
            String angle = (String) value;
            
            if (angle.equalsIgnoreCase(ITALICFONT) || angle.equalsIgnoreCase(OBLIQUEFONT)) {
                if (font.isBold()) {
                    font = new Font(font.getName(), Font.ITALIC + Font.BOLD, font.getSize());
                } else {
                    font = new Font(font.getName(), Font.ITALIC, font.getSize());
                }
            } else {
                if (font.isBold()) {
                    font = new Font(font.getName(), Font.BOLD, font.getSize());
                } else {
                    font = new Font(font.getName(), Font.PLAIN, font.getSize());
                }
            }

            uiControl.setFont(font);
    	} else if (property.equals(__GO_UI_FONTNAME__)) {
            Font font = uiControl.getFont();
            String name = (String) value;
            
            font = new Font(name, font.getStyle(), font.getSize());
            
            uiControl.setFont(font);
    	} else if (property.equals(__GO_UI_FONTSIZE__)) {
            Font font = uiControl.getFont();
            int size = ((Double) value).intValue();
            
            font = new Font(font.getName(), font.getStyle(), size);
            
            uiControl.setFont(font);
    	} else if (property.equals(__GO_UI_FONTUNITS__)) {
    		/* TO BE DONE */
    	} else if (property.equals(__GO_UI_FONTWEIGHT__)) {
            Font font = uiControl.getFont();
            String weight = (String) value;
            
            if (weight.equalsIgnoreCase(BOLDFONT)) {
                if (font.isItalic()) {
                    font = new Font(font.getName(), Font.ITALIC + Font.BOLD, font.getSize());
                } else {
                    font = new Font(font.getName(), Font.BOLD, font.getSize());
                }
            } else {
                if (font.isItalic()) {
                    font = new Font(font.getName(), Font.ITALIC, font.getSize());
                } else {
                    font = new Font(font.getName(), Font.PLAIN, font.getSize());
                }
            }

            uiControl.setFont(font);
    	} else if (property.equals(__GO_UI_FOREGROUNDCOLOR__)) {
    		Double[] allColors = ((Double[]) value);
    		uiControl.setForeground(new Color((int) (allColors[0]*255), (int) (allColors[1]*255), (int) (allColors[2]*255)));
    	} else if (property.equals(__GO_UI_HORIZONTALALIGNMENT__)) {
    		uiControl.setHorizontalAlignment((String) value);
    	} else if (property.equals(__GO_UI_LISTBOXTOP__)) {
    	} else if (property.equals(__GO_UI_MAX__)) {
    	} else if (property.equals(__GO_UI_MIN__)) {
    	} else if (property.equals(__GO_POSITION__)) {
    		/* Convert value according to units */
    		UicontrolUnits unitsProperty = UnitsConverter.stringToUnitsEnum((String) GraphicController.getController().getProperty(((SwingViewObject) uiControl).getId(), __GO_UI_UNITS__));
    		System.out.println("Input position = " + ((Double[]) value)[0] + " " + ((Double[]) value)[1] + " "+ ((Double[]) value)[2] + " "+ ((Double[]) value)[3] + " ");
    		Double[] dblValues = UnitsConverter.convertPositionToPixels((Double[]) value, unitsProperty, uiControl, false);
    		/* Set dimensions before position because position is adjusted according to size */
    		System.out.println("Converted position = " + dblValues[0] + " " + dblValues[1] + " "+ dblValues[2] + " "+ dblValues[3] + " ");
    		uiControl.setDims(new Size(dblValues[2].intValue(), dblValues[3].intValue()));
    		uiControl.setPosition(new Position(dblValues[0].intValue(), dblValues[1].intValue()));
    	} else if (property.equals(__GO_UI_RELIEF__)) {
    		uiControl.setRelief((String) value);
    	} else if (property.equals(__GO_UI_SCALE__)) {
    		Double[] scale = ((Double[]) value);
    		double[] convertedScale = new double[2];
    		convertedScale[0] = scale[0].doubleValue();
    		convertedScale[1] = scale[1].doubleValue();
    		((SwingScilabImageRenderer) uiControl).setScale(convertedScale);
    	} else if (property.equals(__GO_UI_SHEAR__)) {
    		Double[] shear = ((Double[]) value);
    		double[] convertedShear = new double[2];
    		convertedShear[0] = shear[0].doubleValue();
    		convertedShear[1] = shear[1].doubleValue();
    		((SwingScilabImageRenderer) uiControl).setShear(convertedShear);
    	} else if (property.equals(__GO_UI_SLIDERSTEP__)) {
    	} else if (property.equals(__GO_STYLE__)) {
    	} else if (property.equals(__GO_UI_STRING__)) {
    		uiControl.setText(((String[]) value)[0]);
    	} else if (property.equals(__GO_UI_UNITS__)) {
    	} else if (property.equals(__GO_UI_VALUE__)) {
    	} else if (property.equals(__GO_UI_VERTICALALIGNMENT__)) {
    		uiControl.setVerticalAlignment((String) value);
    	} else if (property.equals(__GO_VISIBLE__)) {
        	uiControl.setVisible(((Boolean) value).booleanValue());
    	} else if (property.equals(__GO_PARENT__)) {
    		/* Update position */
    		SwingScilabWidget.update(uiControl, __GO_POSITION__, (Double[]) GraphicController.getController().getProperty(((SwingViewObject) uiControl).getId(), __GO_POSITION__));
    	} else {
    		System.err.println("[SwingScilabWidget.update] Property not mapped: " + property);
    	}
    }
}

