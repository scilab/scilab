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

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CALLBACK__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CALLBACKTYPE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_PARENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_STYLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TAG__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_USER_DATA__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_BACKGROUNDCOLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_COLUMNNAMES__;
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
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ROWNAMES__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SCALE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SHEAR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SLIDERSTEP__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TABLEDATA__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_UNITS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VALUE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VERTICALALIGNMENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_VISIBLE__;

import java.awt.Color;
import java.awt.Font;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.gui.bridge.checkbox.SwingScilabCheckBox;
import org.scilab.modules.gui.bridge.imagerenderer.SwingScilabImageRenderer;
import org.scilab.modules.gui.bridge.listbox.SwingScilabListBox;
import org.scilab.modules.gui.bridge.popupmenu.SwingScilabPopupMenu;
import org.scilab.modules.gui.bridge.radiobutton.SwingScilabRadioButton;
import org.scilab.modules.gui.bridge.slider.SwingScilabSlider;
import org.scilab.modules.gui.bridge.uitable.SwingScilabUiTable;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.utils.UnitsConverter;
import org.scilab.modules.gui.utils.UnitsConverter.UicontrolUnits;
import org.scilab.modules.gui.widget.Widget;

/**
 * @author Bruno JOFRET
 * @author Vincent COUVERT
 */
public final class SwingViewWidget {

    private static final String OBLIQUEFONT = "oblique";
    private static final String ITALICFONT = "italic";
    private static final String BOLDFONT = "bold";
    private static final int COLORS_COEFF = 255;
    private static final int X_INDEX = 0;
    private static final int Y_INDEX = 1;
    private static final int WIDTH_INDEX = 2;
    private static final int HEIGHT_INDEX = 3;

    /**
     * Constructor
     */
    private SwingViewWidget() {
        throw new UnsupportedOperationException();
    }

    /**
     * Update the component in the view
     * @param uiControl the component
     * @param property the property name
     * @param value the property value
     */
    public static void update(Widget uiControl, String property, Object value) {
        String uid = ((SwingViewObject) uiControl).getId();
        if (property.equals(__GO_UI_BACKGROUNDCOLOR__)) {
            Double[] allColors = ((Double[]) value);
            uiControl.setBackground(new Color((int) (allColors[0] * COLORS_COEFF),
                                              (int) (allColors[1] * COLORS_COEFF),
                                              (int) (allColors[2] * COLORS_COEFF)));
        } else if (property.equals(__GO_CALLBACK__)) {
            int cbType = (Integer) GraphicController.getController().getProperty(uid, __GO_CALLBACKTYPE__);
            uiControl.setCallback(CommonCallBack.createCallback((String) value, cbType, uid));
        } else if (property.equals(__GO_CALLBACKTYPE__)) {
            String cbString = (String) GraphicController.getController().getProperty(uid, __GO_CALLBACK__);
            uiControl.setCallback(CommonCallBack.createCallback(cbString, (Integer) value, uid));
        } else if (property.equals(__GO_UI_COLUMNNAMES__)) {
            ((SwingScilabUiTable) uiControl).setColumnNames((String[]) value);
        } else if (property.equals(__GO_UI_ENABLE__)) {
            uiControl.setEnabled(((Boolean) value).booleanValue());
        } else if (property.equals(__GO_UI_FONTANGLE__)) {
            Font font = uiControl.getFont();
            if (font != null) {
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
            }
        } else if (property.equals(__GO_UI_FONTNAME__)) {
            Font font = uiControl.getFont();
            if (font != null) {
                String name = (String) value;

                font = new Font(name, font.getStyle(), font.getSize());

                uiControl.setFont(font);
            }
        } else if (property.equals(__GO_UI_FONTSIZE__)) {
            UicontrolUnits fontUnitsProperty = UnitsConverter.stringToUnitsEnum((String) GraphicController.getController()
                                               .getProperty(uid, __GO_UI_FONTUNITS__));
            Double dblFontSize = UnitsConverter.convertToPoint((Double) value, fontUnitsProperty, uiControl, false);
            Font font = uiControl.getFont();
            if (font != null) {
                int size = dblFontSize.intValue();
                font = new Font(font.getName(), font.getStyle(), size);
                uiControl.setFont(font);
            }
        } else if (property.equals(__GO_UI_FONTUNITS__)) {
            /* Nothing to do here, this property is used when setting position */
        } else if (property.equals(__GO_UI_FONTWEIGHT__)) {
            Font font = uiControl.getFont();
            if (font != null) {
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
            }
        } else if (property.equals(__GO_UI_FOREGROUNDCOLOR__)) {
            Double[] allColors = ((Double[]) value);
            uiControl.setForeground(new Color((int) (allColors[0] * COLORS_COEFF),
                                              (int) (allColors[1] * COLORS_COEFF),
                                              (int) (allColors[2] * COLORS_COEFF)));
        } else if (property.equals(__GO_UI_HORIZONTALALIGNMENT__)) {
            uiControl.setHorizontalAlignment((String) value);
        } else if (property.equals(__GO_UI_LISTBOXTOP__)) {
            if (uiControl instanceof SwingScilabListBox) {
                ((SwingScilabListBox) uiControl).setListBoxTop(((Double) value).intValue());
            }
        } else if (property.equals(__GO_UI_MAX__)) {
            double maxValue = ((Double) value);
            Double[] allValues = (Double[]) GraphicController.getController().getProperty(uid, __GO_UI_VALUE__);
            if ((allValues == null) || (allValues.length == 0)) {
                return;
            }
            double uicontrolValue = allValues[0];
            if (uiControl instanceof SwingScilabSlider) {
                // Update the slider properties
                double minValue = (Double) GraphicController.getController().getProperty(uid, __GO_UI_MIN__);
                ((SwingScilabSlider) uiControl).setMaximumValue(maxValue);
                Double[] sliderStep = ((Double[]) GraphicController.getController().getProperty(uid, __GO_UI_SLIDERSTEP__));
                double minorSliderStep = sliderStep[0].doubleValue();
                double majorSliderStep = sliderStep[1].doubleValue();
                if (minValue <= maxValue) {
                    ((SwingScilabSlider) uiControl).setMinorTickSpacing((int) (minorSliderStep * (maxValue - minValue)));
                    ((SwingScilabSlider) uiControl).setMajorTickSpacing((int) (majorSliderStep * (maxValue - minValue)));
                }
            } else if (uiControl instanceof SwingScilabListBox) {
                // Enable/Disable multiple selection
                double minValue = (Double) GraphicController.getController().getProperty(uid, __GO_UI_MIN__);
                ((SwingScilabListBox) uiControl).setMultipleSelectionEnabled(maxValue - minValue > 1);
            } else if (uiControl instanceof SwingScilabCheckBox) {
                // Check/Uncheck the CheckBox
                ((SwingScilabCheckBox) uiControl).setChecked(maxValue == uicontrolValue);
            } else if (uiControl instanceof SwingScilabRadioButton) {
                // Check/Uncheck the RadioButton
                ((SwingScilabRadioButton) uiControl).setChecked(maxValue == uicontrolValue);
            }
        } else if (property.equals(__GO_UI_MIN__)) {
            double minValue = ((Double) value);
            if (uiControl instanceof SwingScilabSlider) {
                // Update the slider properties
                double maxValue = (Double) GraphicController.getController().getProperty(uid, __GO_UI_MAX__);
                ((SwingScilabSlider) uiControl).setMinimumValue(minValue);
                Double[] sliderStep = ((Double[]) GraphicController.getController().getProperty(uid, __GO_UI_SLIDERSTEP__));
                double minorSliderStep = sliderStep[0].doubleValue();
                double majorSliderStep = sliderStep[1].doubleValue();
                if (minValue <= maxValue) {
                    ((SwingScilabSlider) uiControl).setMinorTickSpacing((int) (minorSliderStep * (maxValue - minValue)));
                    ((SwingScilabSlider) uiControl).setMajorTickSpacing((int) (majorSliderStep * (maxValue - minValue)));
                }
            } else if (uiControl instanceof SwingScilabListBox) {
                // Enable/Disable multiple selection
                double maxValue = (Double) GraphicController.getController().getProperty(uid, __GO_UI_MAX__);
                ((SwingScilabListBox) uiControl).setMultipleSelectionEnabled(maxValue - minValue > 1);
            }
        } else if (property.equals(__GO_POSITION__)) {
            /* Convert value according to units */
            UicontrolUnits unitsProperty = UnitsConverter.stringToUnitsEnum((String) GraphicController
                                           .getController().getProperty(uid, __GO_UI_UNITS__));
            Double[] dblValues = UnitsConverter.convertPositionToPixels((Double[]) value, unitsProperty, uiControl, false);
            /* Set dimensions before position because position is adjusted according to size */
            uiControl.setDims(new Size(dblValues[WIDTH_INDEX].intValue(), dblValues[HEIGHT_INDEX].intValue()));
            uiControl.setPosition(new Position(dblValues[X_INDEX].intValue(), dblValues[Y_INDEX].intValue()));
            /* Manage sliders orientation */
            if (uiControl instanceof SwingScilabSlider) {
                if (dblValues[WIDTH_INDEX].intValue() > dblValues[HEIGHT_INDEX].intValue()) {
                    ((SwingScilabSlider) uiControl).setHorizontal();
                } else {
                    ((SwingScilabSlider) uiControl).setVertical();
                }
            }
        } else if (property.equals(__GO_UI_RELIEF__)) {
            uiControl.setRelief((String) value);
        } else if (property.equals(__GO_UI_ROWNAMES__)) {
            ((SwingScilabUiTable) uiControl).setRowNames((String[]) value);
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
            Double[] sliderStep = ((Double[]) value);
            if (uiControl instanceof SwingScilabSlider) {
                // Update the slider properties
                double minorSliderStep = sliderStep[0].doubleValue();
                double majorSliderStep = sliderStep[1].doubleValue();
                ((SwingScilabSlider) uiControl).setMinorTickSpacing(minorSliderStep);
                ((SwingScilabSlider) uiControl).setMajorTickSpacing(majorSliderStep);
            }
        } else if (property.equals(__GO_STYLE__)) {
            /* Nothing to do unless we want to change style interactively */
        } else if (property.equals(__GO_UI_STRING__)) {
            // Listboxes & Popupmenus manage string vectors
            if (uiControl instanceof SwingScilabListBox) {
                ((SwingScilabListBox) uiControl).setText((String[]) value);
            } else if (uiControl instanceof SwingScilabPopupMenu) {
                ((SwingScilabPopupMenu) uiControl).setText((String[]) value);
            } else {
                uiControl.setText(((String[]) value)[0]);
            }
        } else if (property.equals(__GO_TAG__)) {
            /* Nothing to do */
        } else if (property.equals(__GO_UI_TABLEDATA__)) {
            ((SwingScilabUiTable) uiControl).setData((String[]) value);
        } else if (property.equals(__GO_USER_DATA__)) {
            /* Nothing to do here */
        } else if (property.equals(__GO_UI_UNITS__)) {
            /* Nothing to do here, this property is used when setting position */
        } else if (property.equals(__GO_UI_VALUE__)) {

            Double[] doubleValue = ((Double[]) value);
            int[] intValue = new int[doubleValue.length];
            for (int k = 0; k < doubleValue.length; k++) {
                intValue[k] = doubleValue[k].intValue();
            }

            if (uiControl instanceof SwingScilabListBox) {
                // Update selected items in the listbox
                ((SwingScilabListBox) uiControl).setSelectedIndices(intValue);
            } else if (uiControl instanceof SwingScilabPopupMenu) {
                // Update selected items in the popupmenu
                ((SwingScilabPopupMenu) uiControl).setSelectedIndex(intValue[0]);
            } else if (uiControl instanceof SwingScilabCheckBox) {
                // Check the checkbox if the value is equal to MAX property
                double maxValue = ((Double) GraphicController.getController().getProperty(uid, __GO_UI_MAX__)).intValue();
                ((SwingScilabCheckBox) uiControl).setChecked(maxValue == intValue[0]);
            } else if (uiControl instanceof SwingScilabRadioButton) {
                // Check the radiobutton if the value is equal to MAX property
                double maxValue = ((Double) GraphicController.getController().getProperty(uid, __GO_UI_MAX__)).intValue();
                ((SwingScilabRadioButton) uiControl).setChecked(maxValue == intValue[0]);
            } else if (uiControl instanceof SwingScilabSlider) {
                // Update the slider value
                ((SwingScilabSlider) uiControl).setUserValue(doubleValue[0]);
            }
        } else if (property.equals(__GO_UI_VERTICALALIGNMENT__)) {
            uiControl.setVerticalAlignment((String) value);
        } else if (property.equals(__GO_VISIBLE__)) {
            uiControl.setVisible(((Boolean) value).booleanValue());
        } else if (property.equals(__GO_PARENT__)) {
            /* Update position */
            SwingViewWidget.update(uiControl, __GO_POSITION__,
                                   (Double[]) GraphicController.getController()
                                   .getProperty(uid, __GO_POSITION__));
        } else {
            System.err.println("[SwingScilabWidget.update] Property not mapped: " + property);
        }
    }
}

