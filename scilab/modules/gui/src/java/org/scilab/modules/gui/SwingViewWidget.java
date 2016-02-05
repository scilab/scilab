/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
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

package org.scilab.modules.gui;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CALLBACKTYPE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CALLBACK__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_PARENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_BACKGROUNDCOLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ENABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTANGLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTNAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTSIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTUNITS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTWEIGHT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FOREGROUNDCOLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_HORIZONTALALIGNMENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_RELIEF__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TOOLTIPSTRING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_UNITS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VERTICALALIGNMENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_VISIBLE__;

import java.awt.Color;
import java.awt.Component;
import java.awt.Font;

import javax.swing.JComponent;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.CallBack;
import org.scilab.modules.gui.bridge.editbox.SwingScilabEditBox;
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
    public static void update(Widget uiControl, int property, Object value) {
        Integer uid = ((SwingViewObject) uiControl).getId();

        GraphicController controller = GraphicController.getController();
        switch (property) {
            case __GO_UI_BACKGROUNDCOLOR__: {
                Double[] allColors = ((Double[]) value);
                if (allColors[0] == -1) {
                    // Do not set BackgroundColor for widgets
                    // rely on Look and Feel
                    ((JComponent) uiControl).setOpaque(true);
                    uiControl.resetBackground();
                } else if (allColors[0] == -2) {
                    ((JComponent) uiControl).setOpaque(false);
                } else {
                    ((JComponent) uiControl).setOpaque(true);
                    uiControl.setBackground(new Color(
                                                (int) (allColors[0] * COLORS_COEFF),
                                                (int) (allColors[1] * COLORS_COEFF),
                                                (int) (allColors[2] * COLORS_COEFF)));
                }
                break;
            }
            case __GO_CALLBACK__: {
                int cbType = (Integer) controller.getProperty(uid, __GO_CALLBACKTYPE__);
                uiControl.setCallback(CommonCallBack.createCallback((String) value, cbType, uid));
                break;
            }
            case __GO_CALLBACKTYPE__: {
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
            }
            case __GO_UI_ENABLE__: {
                uiControl.setEnabled(((Boolean) value).booleanValue());
                break;
            }
            case __GO_UI_FONTANGLE__ : {
                String angle = (String) value;
                if (angle.equals("") == false) {
                    Font font = uiControl.getFont();
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
                    Font font = uiControl.getFont();
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
                    Font font = uiControl.getFont();
                    if (font != null) {
                        int size = dblFontSize.intValue();
                        font = new Font(font.getName(), font.getStyle(), size);
                        uiControl.setFont(font);
                    }
                }
                break;
            }
            case __GO_UI_FONTWEIGHT__ : {
                String weight = (String) value;
                if (weight.equals("") == false) {
                    Font font = uiControl.getFont();
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
            case __GO_UI_FOREGROUNDCOLOR__ : {
                Double[] allColors = ((Double[]) value);
                if (allColors[0] != -1) {
                    uiControl.setForeground(new Color(
                                                (int) (allColors[0] * COLORS_COEFF),
                                                (int) (allColors[1] * COLORS_COEFF),
                                                (int) (allColors[2] * COLORS_COEFF)));
                } else {
                    // Do not set ForegroundColor for widgets
                    // rely on Look and Feel
                    uiControl.resetForeground();
                }
                break;
            }
            case __GO_UI_HORIZONTALALIGNMENT__ : {
                String align = (String)value;
                if (align.equals("") == false) {
                    uiControl.setHorizontalAlignment((String) value);
                }
                break;
            }
            case __GO_POSITION__: {
                /* Convert value according to units */
                updatePosition(uiControl, uid, value);
                break;
            }
            case __GO_UI_UNITS__: {
                /* Convert value according to units */
                Double[] p = (Double[]) GraphicController.getController().getProperty(uid, __GO_POSITION__);
                updatePosition(uiControl, uid, p);
                break;
            }
            case __GO_UI_RELIEF__: {
                uiControl.setRelief((String) value);
                break;
            }
            case __GO_UI_STRING__: {
                if (((String[])value).length == 0) {
                    uiControl.setEmptyText();
                } else {
                    uiControl.setText(((String[]) value)[0]);
                }
                break;
            }
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

            case __GO_UI_VERTICALALIGNMENT__ : {
                String align = (String)value;
                if (align.equals("") == false) {
                    uiControl.setVerticalAlignment((String) value);
                }
                break;
            }
            case __GO_VISIBLE__ : {
                uiControl.setVisible(((Boolean) value).booleanValue());
                if (((Component) uiControl) != null && ((Component) uiControl).getParent() != null) {
                    ((Component) uiControl).getParent().invalidate();
                    ((Component) uiControl).getParent().doLayout();
                }
                break;
            }
            case __GO_PARENT__:
                /* Update position */
                ((SwingViewObject)uiControl).update(__GO_POSITION__, controller.getProperty(uid, __GO_POSITION__));
                break;
            default:
                break;
        }
    }

    public static Double[] updatePosition(Widget uiControl, Integer uid, Object value) {
        /* Convert value according to units */
        UicontrolUnits unitsProperty = UnitsConverter.stringToUnitsEnum((String) GraphicController.getController().getProperty(uid, __GO_UI_UNITS__));
        Double[] dblValues = UnitsConverter.convertPositionToPixels((Double[]) value, unitsProperty, uiControl);
        // Set dimensions before position because position is adjusted according to size
        uiControl.setDims(new Size(dblValues[WIDTH_INDEX].intValue(), dblValues[HEIGHT_INDEX].intValue()));
        uiControl.setPosition(new Position(dblValues[X_INDEX].intValue(), dblValues[Y_INDEX].intValue()));
        return dblValues;
    }
}
