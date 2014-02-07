/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_BORDER_OPT_PADDING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CALLBACKTYPE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CALLBACK__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CHILDREN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_GRID_OPT_GRID__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_GRID_OPT_PADDING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_LAYOUT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_PARENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_STYLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TAG__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TYPE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UICONTROL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_BACKGROUNDCOLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ENABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTANGLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTNAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTSIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTUNITS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTWEIGHT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FOREGROUNDCOLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_GROUP_NAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_HORIZONTALALIGNMENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LAYER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LISTBOXTOP__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MAX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MIN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_RELIEF__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SLIDERSTEP__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING_COLNB__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TOOLTIPSTRING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_UNITS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VALUE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VERTICALALIGNMENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_USER_DATA__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_VALID__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_VISIBLE__;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Font;
import java.awt.GridBagLayout;
import java.awt.GridLayout;

import javax.swing.AbstractButton;
import javax.swing.JComponent;
import javax.swing.border.Border;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.CallBack;
import org.scilab.modules.graphic_objects.utils.LayoutType;
import org.scilab.modules.gui.bridge.checkbox.SwingScilabCheckBox;
import org.scilab.modules.gui.bridge.editbox.SwingScilabEditBox;
import org.scilab.modules.gui.bridge.frame.SwingScilabFrame;
import org.scilab.modules.gui.bridge.frame.SwingScilabLayer;
import org.scilab.modules.gui.bridge.frame.SwingScilabScrollableFrame;
import org.scilab.modules.gui.bridge.groupmanager.GroupManager;
import org.scilab.modules.gui.bridge.listbox.SwingScilabListBox;
import org.scilab.modules.gui.bridge.popupmenu.SwingScilabPopupMenu;
import org.scilab.modules.gui.bridge.radiobutton.SwingScilabRadioButton;
import org.scilab.modules.gui.bridge.slider.SwingScilabScroll;
import org.scilab.modules.gui.bridge.slider.SwingScilabSlider;
import org.scilab.modules.gui.bridge.textbox.SwingScilabTextBox;
import org.scilab.modules.gui.bridge.uiimage.SwingScilabUiImage;
import org.scilab.modules.gui.bridge.uitable.SwingScilabUiTable;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.utils.BorderConvertor;
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
    public static void update(Widget uiControl, int property, Object value) {
        Integer uid = ((SwingViewObject) uiControl).getId();
        Font font = null;
        Double[] allColors = null;
        double maxValue = 0;

        GraphicController controller = GraphicController.getController();
        switch (property) {
            case __GO_UI_BACKGROUNDCOLOR__:
                allColors = ((Double[]) value);
                if (allColors[0] != -1) {
                    if (uiControl instanceof SwingScilabListBox) {
                        ((SwingScilabListBox)uiControl).setListBackground(new Color((int) (allColors[0] * COLORS_COEFF),
                                (int) (allColors[1] * COLORS_COEFF),
                                (int) (allColors[2] * COLORS_COEFF)));
                    } else {
                        uiControl.setBackground(new Color((int) (allColors[0] * COLORS_COEFF),
                                                          (int) (allColors[1] * COLORS_COEFF),
                                                          (int) (allColors[2] * COLORS_COEFF)));
                    }
                } else {
                    // Do not set BackgroundColor for widgets
                    // rely on Look and Feel
                }
                break;
            case __GO_CALLBACK__:
                int cbType = (Integer) controller.getProperty(uid, __GO_CALLBACKTYPE__);
                uiControl.setCallback(CommonCallBack.createCallback((String) value, cbType, uid));
                break;
            case __GO_CALLBACKTYPE__:
                String cbString = (String) controller.getProperty(uid, __GO_CALLBACK__);
                if ((Integer) value == CallBack.UNTYPED) {
                    /*
                     * Deactivate
                     * callback
                     */
                    uiControl.setCallback(null);
                } else {
                    uiControl.setCallback(CommonCallBack.createCallback(cbString, (Integer) value, uid));
                }
                break;
            case __GO_CHILDREN__:
                /* Nothing to do */
                break;
            case __GO_UI_ENABLE__:
                uiControl.setEnabled(((Boolean) value).booleanValue());
                break;
            case __GO_UI_FONTANGLE__ : {
                String angle = (String) value;
                if (angle.equals("") == false) {
                    font = uiControl.getFont();
                    if (font != null) {
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
                }
                break;
            }
            case __GO_UI_FONTNAME__ : {
                String name = (String) value;
                if (name.equals("") == false) {
                    font = uiControl.getFont();
                    if (font != null) {

                        font = new Font(name, font.getStyle(), font.getSize());

                        if (uiControl instanceof SwingScilabEditBox) {
                            ((SwingScilabEditBox)uiControl).setEditFont(font);
                        } else {
                            uiControl.setFont(font);
                        }
                    }
                }
                break;
            }
            case __GO_UI_FONTSIZE__ : {
                if ((Double)value != 0.0) {
                    UicontrolUnits fontUnitsProperty = UnitsConverter.stringToUnitsEnum((String) controller.getProperty(uid, __GO_UI_FONTUNITS__));
                    Double dblFontSize = UnitsConverter.convertToPoint((Double) value, fontUnitsProperty, uiControl, false);
                    font = uiControl.getFont();
                    if (font != null) {
                        int size = dblFontSize.intValue();
                        font = new Font(font.getName(), font.getStyle(), size);
                        uiControl.setFont(font);
                    }
                }
                break;
            }
            case __GO_UI_FONTUNITS__ :
                /* Nothing to do here, this property is used when setting position */
                break;
            case __GO_UI_FONTWEIGHT__ : {
                String weight = (String) value;
                if (weight.equals("") == false) {
                    font = uiControl.getFont();
                    if (font != null) {
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
                }
                break;
            }
            case __GO_UI_FOREGROUNDCOLOR__ :
                allColors = ((Double[]) value);
                uiControl.setForeground(new Color((int) (allColors[0] * COLORS_COEFF), (int) (allColors[1] * COLORS_COEFF), (int) (allColors[2] * COLORS_COEFF)));
                break;
            case __GO_UI_HORIZONTALALIGNMENT__ : {
                String align = (String)value;
                if (align.equals("") == false) {
                    uiControl.setHorizontalAlignment((String) value);
                }
                break;
            }
            case __GO_UI_LISTBOXTOP__ :
                if (uiControl instanceof SwingScilabListBox) {
                    Integer[] listboxtopValue = ((Integer[]) value);
                    if (listboxtopValue.length > 0) {
                        ((SwingScilabListBox) uiControl).setListBoxTop(listboxtopValue[0]);
                    }
                }
                break;
            case __GO_UI_MAX__:
                maxValue = ((Double) value);
                if (uiControl instanceof SwingScilabScroll) {
                    // Update the slider properties
                    double minValue = (Double) controller.getProperty(uid, __GO_UI_MIN__);
                    ((SwingScilabScroll) uiControl).setMaximumValue(maxValue);
                    Double[] sliderStep = ((Double[]) controller.getProperty(uid, __GO_UI_SLIDERSTEP__));
                    double minorSliderStep = sliderStep[0].doubleValue();
                    double majorSliderStep = sliderStep[1].doubleValue();
                    if (minValue <= maxValue) {
                        ((SwingScilabScroll) uiControl).setMinorTickSpacing(minorSliderStep);
                        ((SwingScilabScroll) uiControl).setMajorTickSpacing(majorSliderStep);
                    }
                } else if (uiControl instanceof SwingScilabSlider) {
                    // Update the slider properties
                    double minValue = (Double) controller.getProperty(uid, __GO_UI_MIN__);
                    ((SwingScilabSlider) uiControl).setMaximumValue(maxValue);
                    Double[] sliderStep = ((Double[]) controller.getProperty(uid, __GO_UI_SLIDERSTEP__));
                    double minorSliderStep = sliderStep[0].doubleValue();
                    double majorSliderStep = sliderStep[1].doubleValue();
                    if (minValue <= maxValue) {
                        ((SwingScilabSlider) uiControl).setMinorTickSpacing(minorSliderStep);
                        ((SwingScilabSlider) uiControl).setMajorTickSpacing(majorSliderStep);
                    }
                } else if (uiControl instanceof SwingScilabListBox) {
                    // Enable/Disable multiple selection
                    double minValue = (Double) controller.getProperty(uid, __GO_UI_MIN__);
                    ((SwingScilabListBox) uiControl).setMultipleSelectionEnabled(maxValue - minValue > 1);
                } else if (uiControl instanceof SwingScilabCheckBox) {
                    Double[] allValues = (Double[]) controller.getProperty(uid, __GO_UI_VALUE__);
                    if ((allValues == null) || (allValues.length == 0)) {
                        return;
                    }
                    double uicontrolValue = allValues[0];
                    // Check/Uncheck the CheckBox
                    ((SwingScilabCheckBox) uiControl).setChecked(maxValue == uicontrolValue);
                } else if (uiControl instanceof SwingScilabRadioButton) {
                    Double[] allValues = (Double[]) controller.getProperty(uid, __GO_UI_VALUE__);
                    if ((allValues == null) || (allValues.length == 0)) {
                        return;
                    }
                    double uicontrolValue = allValues[0];
                    // Check/Uncheck the RadioButton
                    ((SwingScilabRadioButton) uiControl).setChecked(maxValue == uicontrolValue);
                } else if (uiControl instanceof SwingScilabTextBox) {
                    //System.out.println("max value be come columns for textfield");
                    //((SwingScilabTextBox) uiControl).setColumns((int)maxValue);
                } else if (uiControl instanceof SwingScilabEditBox) {
                    double min = ((Double) controller.getProperty(uid, __GO_UI_MIN__));
                    double max = ((Double) controller.getProperty(uid, __GO_UI_MAX__));
                    if (max - min > 1.0) {
                        ((SwingScilabEditBox) uiControl).setMultiLineText(true);
                    } else {
                        ((SwingScilabEditBox) uiControl).setMultiLineText(false);
                    }
                    // Force String update
                    update(uiControl, __GO_UI_STRING__, GraphicController.getController().getProperty(uid, __GO_UI_STRING__));
                }
                break;
            case __GO_UI_MIN__:
                double minValue = ((Double) value);
                if (uiControl instanceof SwingScilabScroll) {
                    // Update the slider properties
                    maxValue = (Double) controller.getProperty(uid, __GO_UI_MAX__);
                    ((SwingScilabScroll) uiControl).setMinimumValue(minValue);
                    Double[] sliderStep = ((Double[]) controller.getProperty(uid, __GO_UI_SLIDERSTEP__));
                    double minorSliderStep = sliderStep[0].doubleValue();
                    double majorSliderStep = sliderStep[1].doubleValue();
                    if (minValue <= maxValue) {
                        ((SwingScilabScroll) uiControl).setMinorTickSpacing(minorSliderStep);
                        ((SwingScilabScroll) uiControl).setMajorTickSpacing(majorSliderStep);
                    }
                } else if (uiControl instanceof SwingScilabSlider) {
                    // Update the slider properties
                    maxValue = (Double) controller.getProperty(uid, __GO_UI_MAX__);
                    ((SwingScilabSlider) uiControl).setMinimumValue(minValue);
                    Double[] sliderStep = ((Double[]) controller.getProperty(uid, __GO_UI_SLIDERSTEP__));
                    double minorSliderStep = sliderStep[0].doubleValue();
                    double majorSliderStep = sliderStep[1].doubleValue();
                    if (minValue <= maxValue) {
                        ((SwingScilabSlider) uiControl).setMinorTickSpacing(minorSliderStep);
                        ((SwingScilabSlider) uiControl).setMajorTickSpacing(majorSliderStep);
                    }
                } else if (uiControl instanceof SwingScilabListBox) {
                    // Enable/Disable multiple selection
                    maxValue = (Double) controller.getProperty(uid, __GO_UI_MAX__);
                    ((SwingScilabListBox) uiControl).setMultipleSelectionEnabled(maxValue - minValue > 1);
                } else if (uiControl instanceof SwingScilabEditBox) {
                    double min = ((Double) controller.getProperty(uid, __GO_UI_MIN__));
                    double max = ((Double) controller.getProperty(uid, __GO_UI_MAX__));
                    if (max - min > 1.0) {
                        ((SwingScilabEditBox) uiControl).setMultiLineText(true);
                    } else {
                        ((SwingScilabEditBox) uiControl).setMultiLineText(false);
                    }
                    // Force String update
                    update(uiControl, __GO_UI_STRING__, GraphicController.getController().getProperty(uid, __GO_UI_STRING__));
                }
                break;
            case __GO_POSITION__: {

                /* Convert value according to units */
                UicontrolUnits unitsProperty = UnitsConverter.stringToUnitsEnum((String) GraphicController.getController().getProperty(uid, __GO_UI_UNITS__));
                Double[] dblValues = UnitsConverter.convertPositionToPixels((Double[]) value, unitsProperty, uiControl);
                // Set dimensions before position because position is adjusted according to size
                uiControl.setDims(new Size(dblValues[WIDTH_INDEX].intValue(), dblValues[HEIGHT_INDEX].intValue()));
                uiControl.setPosition(new Position(dblValues[X_INDEX].intValue(), dblValues[Y_INDEX].intValue()));

                Integer parent = (Integer)controller.getProperty(uid, __GO_PARENT__);
                if (parent != 0) {
                    Integer type = (Integer)controller.getProperty(parent, __GO_TYPE__);
                    if (type == __GO_UICONTROL__) {

                        Integer style = (Integer)controller.getProperty(parent, __GO_STYLE__);
                        if (style == __GO_UI_FRAME__) {
                            SwingViewObject obj = SwingView.getFromId(parent);
                            if (obj instanceof SwingScilabScrollableFrame) {
                                SwingScilabScrollableFrame frame = (SwingScilabScrollableFrame) SwingView.getFromId(parent);
                                frame.updateChildPosition((JComponent)uiControl);
                            }
                        }
                    }
                }
                // Manage sliders orientation
                if (uiControl instanceof SwingScilabScroll) {
                    if (dblValues[WIDTH_INDEX].intValue() > dblValues[HEIGHT_INDEX].intValue()) {
                        ((SwingScilabScroll) uiControl).setHorizontal();
                    } else {
                        ((SwingScilabScroll) uiControl).setVertical();
                    }
                } else if (uiControl instanceof SwingScilabSlider) {
                    if (dblValues[WIDTH_INDEX].intValue() > dblValues[HEIGHT_INDEX].intValue()) {
                        ((SwingScilabSlider) uiControl).setHorizontal();
                    } else {
                        ((SwingScilabSlider) uiControl).setVertical();
                    }
                }
                break;
            }
            case __GO_UI_RELIEF__:
                uiControl.setRelief((String) value);
                break;
            case __GO_UI_SLIDERSTEP__:
                Double[] sliderStep = ((Double[]) value);
                if (uiControl instanceof SwingScilabScroll) {
                    // Update the slider properties
                    double minorSliderStep = sliderStep[0].doubleValue();
                    double majorSliderStep = sliderStep[1].doubleValue();
                    ((SwingScilabScroll) uiControl).setMinorTickSpacing(minorSliderStep);
                    ((SwingScilabScroll) uiControl).setMajorTickSpacing(majorSliderStep);
                } else if (uiControl instanceof SwingScilabSlider) {
                    // Update the slider properties
                    double minorSliderStep = sliderStep[0].doubleValue();
                    double majorSliderStep = sliderStep[1].doubleValue();
                    ((SwingScilabSlider) uiControl).setMinorTickSpacing(minorSliderStep);
                    ((SwingScilabSlider) uiControl).setMajorTickSpacing(majorSliderStep);
                }
                break;
            case __GO_STYLE__:
                /* Nothing to do unless we want to change style interactively */
                break;
            case __GO_UI_STRING__:
                if (uiControl instanceof SwingScilabUiTable) {
                    // Update column names
                    String[] stringValue = (String[]) value;
                    int colNb = ((Integer) controller.getProperty(uid, __GO_UI_STRING_COLNB__));
                    String[] colNames = new String[colNb - 1];
                    for (int k = 1; k < colNb; k++) {
                        colNames[k - 1] = stringValue[k * (stringValue.length / colNb)];
                    }
                    ((SwingScilabUiTable) uiControl).setColumnNames(colNames);
                    // Update row names
                    String[] rowNames = new String[stringValue.length / colNb - 1];
                    for (int k = 1; k < stringValue.length / colNb; k++) {
                        rowNames[k - 1] = stringValue[k];
                    }
                    ((SwingScilabUiTable) uiControl).setRowNames(rowNames);
                    // Update data
                    String[] tableData = new String[rowNames.length * colNames.length];
                    int kData = 0;
                    for (int kCol = 1; kCol <= colNames.length; kCol++) {
                        for (int kRow = 1; kRow <= rowNames.length; kRow++) {
                            tableData[kData++] = stringValue[kCol * (stringValue.length / colNb) + kRow];
                        }
                    }
                    if (tableData.length != 0) {
                        ((SwingScilabUiTable) uiControl).setData(tableData);
                    }
                } else if (uiControl instanceof SwingScilabListBox) {
                    // Listboxes manage string vectors
                    ((SwingScilabListBox) uiControl).setText((String[]) value);
                } else if (uiControl instanceof SwingScilabPopupMenu) {
                    // Popupmenus manage string vectors
                    ((SwingScilabPopupMenu) uiControl).setText((String[]) value);
                } else if (uiControl instanceof SwingScilabEditBox) {
                    double min = ((Double) controller.getProperty(uid, __GO_UI_MIN__));
                    double max = ((Double) controller.getProperty(uid, __GO_UI_MAX__));
                    if (max - min > 1.0) {
                        ((SwingScilabEditBox) uiControl).setText((String[]) value);
                        ((SwingScilabEditBox) uiControl).setMultiLineText(true);
                    } else {
                        uiControl.setText(((String[]) value)[0]);
                        ((SwingScilabEditBox) uiControl).setMultiLineText(false);
                    }
                } else {
                    uiControl.setText(((String[]) value)[0]);
                }
                break;
            case __GO_UI_TOOLTIPSTRING__:
                String[] tooltipString = ((String[]) value);
                String tooltipText = tooltipString[0];
                if (tooltipString.length > 1) {
                    tooltipText = "<html>" + tooltipText;
                    for (int kString = 1; kString < tooltipString.length; kString++) {
                        tooltipText = tooltipText + "<br>" + tooltipString[kString];
                    }
                    tooltipText = tooltipText + "</html>";
                }
                if (tooltipText.equals("")) {
                    uiControl.setToolTipText(null);
                } else {
                    uiControl.setToolTipText(tooltipText);
                }
                break;
            case __GO_UI_STRING_COLNB__:
                /* Nothing to do */
            case __GO_TAG__:
                /* Nothing to do */
            case __GO_USER_DATA__:
                /* Nothing to do here */
            case __GO_UI_UNITS__:
                /*
                 * Nothing to do here, this property is used when setting
                 * position
                 */
            case __GO_VALID__:
                /* Nothing to do */
                break;
            case __GO_UI_VALUE__:

                Double[] doubleValue = ((Double[]) value);
                if (doubleValue.length == 0) {
                    return;
                }
                int[] intValue = new int[doubleValue.length];
                for (int k = 0; k < doubleValue.length; k++) {
                    intValue[k] = doubleValue[k].intValue();
                }

                if (uiControl instanceof SwingScilabListBox) {
                    // Update selected items in the listbox
                    ((SwingScilabListBox) uiControl).setSelectedIndices(intValue);
                } else if (uiControl instanceof SwingScilabPopupMenu) {
                    // Update selected items in the popupmenu
                    ((SwingScilabPopupMenu) uiControl).setUserSelectedIndex(intValue[0]);
                } else if (uiControl instanceof SwingScilabCheckBox) {
                    // Check the checkbox if the value is equal to MAX property
                    maxValue = ((Double) controller.getProperty(uid, __GO_UI_MAX__)).intValue();
                    ((SwingScilabCheckBox) uiControl).setChecked(maxValue == intValue[0]);
                } else if (uiControl instanceof SwingScilabRadioButton) {
                    // Check the radiobutton if the value is equal to MAX property
                    maxValue = ((Double) controller.getProperty(uid, __GO_UI_MAX__)).intValue();
                    ((SwingScilabRadioButton) uiControl).setChecked(maxValue == intValue[0]);
                } else if (uiControl instanceof SwingScilabScroll) {
                    // Update the slider value
                    ((SwingScilabScroll) uiControl).setUserValue(doubleValue[0]);
                } else if (uiControl instanceof SwingScilabSlider) {
                    // Update the slider value
                    ((SwingScilabSlider) uiControl).setUserValue(doubleValue[0]);
                } else if (uiControl instanceof SwingScilabUiImage) {
                    // Update the image parameters
                    double[] imageParams = new double[5];
                    if (doubleValue.length < 1) {
                        imageParams[0] = 1.0;
                    } else {
                        imageParams[0] = doubleValue[0];
                    }
                    if (doubleValue.length < 2) {
                        imageParams[1] = 1.0;
                    } else {
                        imageParams[1] = doubleValue[1];
                    }
                    if (doubleValue.length < 3) {
                        imageParams[2] = 0.0;
                    } else {
                        imageParams[2] = doubleValue[2];
                    }
                    if (doubleValue.length < 4) {
                        imageParams[3] = 0.0;
                    } else {
                        imageParams[3] = doubleValue[3];
                    }
                    if (doubleValue.length < 5) {
                        imageParams[4] = 0.0;
                    } else {
                        imageParams[4] = doubleValue[4];
                    }
                    double[] scale = new double[2];
                    scale[0] = imageParams[0];
                    scale[1] = imageParams[1];
                    ((SwingScilabUiImage) uiControl).setScale(scale);
                    double[] shear = new double[2];
                    shear[0] = imageParams[2];
                    shear[1] = imageParams[3];
                    ((SwingScilabUiImage) uiControl).setShear(shear);
                    ((SwingScilabUiImage) uiControl).setRotate(imageParams[4]);
                } else if (uiControl instanceof SwingScilabLayer) {
                    SwingScilabLayer layer = (SwingScilabLayer)uiControl;
                    //if intValue[0] is out of bounds, do not update view but let "wrong" value in model
                    layer.setActiveLayer(intValue[0]);
                }
                break;
            case __GO_UI_VERTICALALIGNMENT__ : {
                String align = (String)value;
                if (align.equals("") == false) {
                    uiControl.setVerticalAlignment((String) value);
                }
                break;
            }
            case __GO_VISIBLE__ : {
                boolean needUpdate = true;
                if (uiControl instanceof SwingScilabFrame || uiControl instanceof SwingScilabScrollableFrame) {
                    Component frame = (Component) uiControl;
                    Integer parent = (Integer) controller.getProperty(uid, __GO_PARENT__);
                    if (parent != 0) {
                        Integer type = (Integer) controller.getProperty(parent, __GO_TYPE__);
                        if (type == __GO_UICONTROL__) {
                            Integer style = (Integer) controller.getProperty(parent, __GO_STYLE__);
                            if (style == __GO_UI_LAYER__) {
                                //no no no don't touch visible on layer children !
                                Boolean visible = (Boolean)value;
                                SwingScilabLayer layer = (SwingScilabLayer) SwingView.getFromId(parent);
                                Boolean isActive = layer.isLayerActive(frame);
                                if (isActive != visible ) {
                                    controller.setProperty(uid, __GO_VISIBLE__, isActive);
                                }

                                needUpdate = false;
                            }
                        }
                    }
                }

                if (needUpdate) {
                    uiControl.setVisible(((Boolean) value).booleanValue());
                }
                break;
            }
            case __GO_PARENT__:
                /* Update position */
                SwingViewWidget.update(uiControl, __GO_POSITION__, controller.getProperty(uid, __GO_POSITION__));
                break;
            case __GO_UI_GROUP_NAME__:
                if (uiControl instanceof SwingScilabRadioButton || uiControl instanceof SwingScilabCheckBox) {
                    String groupName = (String) value;
                    if (groupName == null || groupName.equals("")) {
                        //remove rb from buttonGroup Map
                        GroupManager.getGroupManager().removeFromGroup((AbstractButton) uiControl);
                    } else {
                        GroupManager.getGroupManager().addToGroup(groupName, (AbstractButton) uiControl);
                    }
                }
                break;
            case __GO_LAYOUT__: {
                if (uiControl instanceof SwingScilabScrollableFrame) {
                    SwingScilabScrollableFrame frame = (SwingScilabScrollableFrame) uiControl;
                    LayoutType newLayout = LayoutType.intToEnum((Integer) value);
                    switch (newLayout) {
                        case BORDER: {
                            Integer[] padding = (Integer[]) controller.getProperty(frame.getId(), __GO_BORDER_OPT_PADDING__);
                            frame.setLayout(new BorderLayout(padding[0], padding[1]));
                            break;
                        }
                        case GRIDBAG:
                            frame.setLayout(new GridBagLayout());
                            break;
                        case GRID: {
                            Integer[] padding = (Integer[]) controller.getProperty(frame.getId(), __GO_GRID_OPT_PADDING__);
                            Integer[] grid = (Integer[]) controller.getProperty(frame.getId(), __GO_GRID_OPT_GRID__);
                            if (grid[0] == 0 && grid[1] == 0) {
                                grid[0] = 1;
                            }
                            frame.setLayout(new GridLayout(grid[0], grid[1], padding[0], padding[1]));
                            break;
                        }
                        case NONE:
                        default:
                            frame.setLayout(null);
                            break;
                    }
                } else if (uiControl instanceof SwingScilabFrame) {
                    SwingScilabFrame frame = (SwingScilabFrame) uiControl;
                    LayoutType newLayout = LayoutType.intToEnum((Integer) value);
                    switch (newLayout) {
                        case BORDER: {
                            Integer[] padding = (Integer[]) controller.getProperty(frame.getId(), __GO_BORDER_OPT_PADDING__);
                            frame.setLayout(new BorderLayout(padding[0], padding[1]));
                            break;
                        }
                        case GRIDBAG:
                            frame.setLayout(new GridBagLayout());
                            break;
                        case GRID: {
                            Integer[] padding = (Integer[]) controller.getProperty(frame.getId(), __GO_GRID_OPT_PADDING__);
                            Integer[] grid = (Integer[]) controller.getProperty(frame.getId(), __GO_GRID_OPT_GRID__);
                            if (grid[0] == 0 && grid[1] == 0) {
                                grid[0] = 1;
                            }
                            frame.setLayout(new GridLayout(grid[0], grid[1], padding[0], padding[1]));
                            break;
                        }
                        case NONE:
                        default:
                            frame.setLayout(null);
                            break;
                    }
                }
                break;
            }
            case __GO_UI_FRAME_BORDER__:
                if (uiControl instanceof SwingScilabScrollableFrame) {
                    SwingScilabScrollableFrame frame = (SwingScilabScrollableFrame) uiControl;
                    Integer borderId = (Integer) value;
                    Border border = BorderConvertor.getBorder(borderId);
                    frame.setBorder(border);
                } else if (uiControl instanceof SwingScilabFrame) {
                    SwingScilabFrame frame = (SwingScilabFrame) uiControl;
                    Integer borderId = (Integer) value;
                    Border border = BorderConvertor.getBorder(borderId);
                    frame.setBorder(border);
                }

                break;
            default:
                //System.err.println("[SwingScilabWidget.update] Property not mapped: " + property);
        }
    }
}
