/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_LINE_THICKNESS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TITLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTANGLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTNAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTSIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTWEIGHT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER_COLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER_HIGHLIGHT_IN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER_IN_BORDER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER_JUSTIFICATION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER_OUT_BORDER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER_ROUNDED__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER_SHADOW_IN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER_SHADOW_OUT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER_STYLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER_TITLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER_TYPE__;

import java.awt.Color;
import java.awt.Font;
import java.lang.reflect.Field;

import javax.swing.BorderFactory;
import javax.swing.UIManager;
import javax.swing.border.BevelBorder;
import javax.swing.border.Border;
import javax.swing.border.TitledBorder;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.uicontrol.frame.border.FrameBorder.BorderType;
import org.scilab.modules.graphic_objects.uicontrol.frame.border.FrameBorder.JustificationType;
import org.scilab.modules.graphic_objects.uicontrol.frame.border.FrameBorder.TitlePositionType;
import org.scilab.modules.graphic_objects.uicontrol.frame.border.FrameBorderType;

public class BorderConvertor {
    public static Color getColor(String str) {
        try {
            Field field = Class.forName("java.awt.Color").getField(str);
            return (Color) field.get(null);
        } catch (Exception e) {
            return Color.black;
        }
    }

    public static Border getBorder(Integer borderId) {
        GraphicController controller = GraphicController.getController();
        Integer styleInt = (Integer) controller.getProperty(borderId, __GO_UI_FRAME_BORDER_STYLE__);
        FrameBorderType style;

        if (styleInt == null) {
            // Default border : NONE
            style = FrameBorderType.NONE;
        } else {
            style = FrameBorderType.intToEnum(styleInt);
        }

        Border border = null;

        switch (style) {
            case BEVEL: {
                BorderType type = BorderType.intToEnum((Integer) controller.getProperty(borderId, __GO_UI_FRAME_BORDER_TYPE__));
                int bevelType = type == BorderType.LOWERED ? BevelBorder.LOWERED : BevelBorder.RAISED;
                String hlOut = (String) controller.getProperty(borderId, __GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__);
                String hlIn = (String) controller.getProperty(borderId, __GO_UI_FRAME_BORDER_HIGHLIGHT_IN__);
                String shadowOut = (String) controller.getProperty(borderId, __GO_UI_FRAME_BORDER_SHADOW_OUT__);
                String shadowIn = (String) controller.getProperty(borderId, __GO_UI_FRAME_BORDER_SHADOW_IN__);

                if (hlOut != null && shadowOut != null) {
                    if (hlIn != null && shadowIn != null) {
                        border = BorderFactory.createBevelBorder(bevelType, getColor(hlOut), getColor(hlIn), getColor(shadowOut), getColor(shadowIn));
                    } else {
                        border = BorderFactory.createBevelBorder(bevelType, getColor(hlOut), getColor(shadowOut));
                    }
                } else {
                    border = BorderFactory.createBevelBorder(bevelType);
                }
                break;
            }
            case COMPOUND: {
                Integer borderId1 = (Integer) controller.getProperty(borderId, __GO_UI_FRAME_BORDER_OUT_BORDER__);
                Integer borderId2 = (Integer) controller.getProperty(borderId, __GO_UI_FRAME_BORDER_IN_BORDER__);

                if (borderId1 != null && borderId2 != null) {
                    Border border1 = getBorder(borderId1);
                    Border border2 = getBorder(borderId2);

                    border = BorderFactory.createCompoundBorder(border1, border2);
                } else {
                    border = BorderFactory.createCompoundBorder();
                }
                break;
            }
            case EMPTY: {
                // TLBR
                Double[] pos = (Double[]) controller.getProperty(borderId, __GO_POSITION__);

                if (pos != null) {
                    border = BorderFactory.createEmptyBorder(pos[0].intValue(), pos[1].intValue(), pos[2].intValue(), pos[3].intValue());
                } else {
                    border = BorderFactory.createEmptyBorder();
                }
                break;
            }
            case ETCHED: {
                Integer type = (Integer) controller.getProperty(borderId, __GO_UI_FRAME_BORDER_TYPE__);
                String hlOut = (String) controller.getProperty(borderId, __GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__);
                String shadowOut = (String) controller.getProperty(borderId, __GO_UI_FRAME_BORDER_SHADOW_OUT__);

                if (type == null) {
                    if (hlOut != null && shadowOut != null) {
                        border = BorderFactory.createEtchedBorder(getColor(hlOut), getColor(shadowOut));
                    } else {
                        border = BorderFactory.createEtchedBorder();
                    }

                } else {
                    BorderType borderType = BorderType.intToEnum(type);
                    int bevelType = borderType == BorderType.RAISED ? BevelBorder.RAISED : BevelBorder.LOWERED;

                    if (hlOut != null && shadowOut != null) {
                        border = BorderFactory.createEtchedBorder(bevelType, getColor(hlOut), getColor(shadowOut));
                    } else {
                        border = BorderFactory.createEtchedBorder(bevelType);
                    }
                }
                break;
            }
            case LINE: {
                String color = (String) controller.getProperty(borderId, __GO_UI_FRAME_BORDER_COLOR__);
                Integer thickness = (Integer) controller.getProperty(borderId, __GO_LINE_THICKNESS__);
                Boolean rounded = (Boolean) controller.getProperty(borderId, __GO_UI_FRAME_BORDER_ROUNDED__);

                if (thickness != null) {
                    if (rounded != null) {
                        // Java 1.7
                        // border = BorderFactory.createLineBorder(
                        // getColor(color),
                        // thickness,
                        // rounded);
                        border = BorderFactory.createLineBorder(getColor(color), thickness);
                    } else {
                        border = BorderFactory.createLineBorder(getColor(color), thickness);
                    }
                } else {
                    border = BorderFactory.createLineBorder(getColor(color));
                }
                break;
            }
            case MATTE: {
                // TLBR
                Double[] pos = (Double[]) controller.getProperty(borderId, __GO_POSITION__);
                String color = (String) controller.getProperty(borderId, __GO_UI_FRAME_BORDER_COLOR__);

                border = BorderFactory.createMatteBorder(pos[0].intValue(), pos[1].intValue(), pos[2].intValue(), pos[3].intValue(), getColor(color));
                break;
            }
            case SOFTBEVEL: {

                BorderType type = BorderType.intToEnum((Integer) controller.getProperty(borderId, __GO_UI_FRAME_BORDER_TYPE__));
                int bevelType = type == BorderType.LOWERED ? BevelBorder.LOWERED : BevelBorder.RAISED;
                String hlOut = (String) controller.getProperty(borderId, __GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__);
                String hlIn = (String) controller.getProperty(borderId, __GO_UI_FRAME_BORDER_HIGHLIGHT_IN__);
                String shadowOut = (String) controller.getProperty(borderId, __GO_UI_FRAME_BORDER_SHADOW_OUT__);
                String shadowIn = (String) controller.getProperty(borderId, __GO_UI_FRAME_BORDER_SHADOW_IN__);

                /* in Java 1.6 use normal bevel function */
                if (hlOut != null && shadowOut != null) {
                    if (hlIn != null && shadowIn != null) {
                        border = BorderFactory.createBevelBorder(bevelType, getColor(hlOut), getColor(hlIn), getColor(shadowOut), getColor(shadowIn));
                    } else {
                        border = BorderFactory.createBevelBorder(bevelType, getColor(hlOut), getColor(shadowOut));
                    }
                } else {
                    border = BorderFactory.createBevelBorder(bevelType);
                }
                break;
            }
            case TITLED: {
                Integer borderId1 = (Integer) controller.getProperty(borderId, __GO_UI_FRAME_BORDER_TITLE__);
                String title = (String) controller.getProperty(borderId, __GO_TITLE__);
                Integer justification = (Integer) controller.getProperty(borderId, __GO_UI_FRAME_BORDER_JUSTIFICATION__);
                String fontName = (String) controller.getProperty(borderId, __GO_UI_FONTNAME__);
                String fontAngle = (String) controller.getProperty(borderId, __GO_UI_FONTANGLE__);
                Integer fontSize = (Integer) controller.getProperty(borderId, __GO_UI_FONTSIZE__);
                String fontWeight = (String) controller.getProperty(borderId, __GO_UI_FONTWEIGHT__);
                Integer pos = (Integer) controller.getProperty(borderId, __GO_UI_FRAME_BORDER_POSITION__);
                String color = (String) controller.getProperty(borderId, __GO_UI_FRAME_BORDER_COLOR__);

                if (borderId1 == null && title != null) {
                    border = BorderFactory.createTitledBorder(title);
                } else {
                    // create sub border
                    Border border1 = getBorder(borderId1);

                    if (title != null) {
                        if (justification != null && pos != null) {
                            int justify = TitledBorder.LEADING;
                            switch (JustificationType.intToEnum(justification)) {
                                case CENTER:
                                    justify = TitledBorder.CENTER;
                                    break;
                                case LEFT:
                                    justify = TitledBorder.LEFT;
                                    break;
                                case RIGHT:
                                    justify = TitledBorder.RIGHT;
                                    break;
                                case TRAILING:
                                    justify = TitledBorder.TRAILING;
                                    break;
                                case LEADING:
                                default:
                                    justify = TitledBorder.LEADING;
                                    break;
                            }

                            int position = TitledBorder.TOP;
                            switch (TitlePositionType.intToEnum(pos)) {
                                case ABOVE_BOTTOM:
                                    position = TitledBorder.ABOVE_BOTTOM;
                                    break;
                                case ABOVE_TOP:
                                    position = TitledBorder.ABOVE_TOP;
                                    break;
                                case BELOW_BOTTOM:
                                    position = TitledBorder.BELOW_BOTTOM;
                                    break;
                                case BELOW_TOP:
                                    position = TitledBorder.BELOW_TOP;
                                    break;
                                case BOTTOM:
                                    position = TitledBorder.BOTTOM;
                                    break;
                                case TOP:
                                default:
                                    position = TitledBorder.TOP;
                                    break;
                            }

                            if (fontName != null && fontAngle != null && fontSize != null && fontWeight != null) {
                                // create font
                                int fontStyle = Font.PLAIN;
                                if (fontAngle.equals("italic")) {
                                    fontStyle |= Font.ITALIC;
                                }

                                if (fontWeight.equals("bold")) {
                                    fontStyle |= Font.BOLD;
                                }

                                Font defaultFont = UIManager.getFont("Button.font");
                                Font font = new Font(fontName.equals("") == false ? fontName : defaultFont.getFontName(),
                                                     fontStyle,
                                                     fontSize != 0 ? fontSize : defaultFont.getSize());
                                border = BorderFactory.createTitledBorder(border1, title, justify, position, font, getColor(color));
                            } else {
                                border = BorderFactory.createTitledBorder(border1, title, justify, position);
                            }
                        } else {
                            border = BorderFactory.createTitledBorder(border1, title);
                        }
                    } else {
                        border = BorderFactory.createTitledBorder(border1);
                    }
                }
                break;
            }
            default:
            case NONE: {
                border = BorderFactory.createEmptyBorder();
                break;
            }
        }

        return border;
    }
}
