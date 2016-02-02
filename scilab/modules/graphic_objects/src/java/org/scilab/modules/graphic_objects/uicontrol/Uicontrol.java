/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
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

package org.scilab.modules.graphic_objects.uicontrol;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_BORDER_OPT_PADDING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_EVENTHANDLER_ENABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_EVENTHANDLER_NAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_GRID_OPT_GRID__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_GRID_OPT_PADDING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_LAYOUT_SET__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_LAYOUT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_MARGINS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_STYLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UICONTROL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_BACKGROUNDCOLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_BORDER_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_BORDER_PREFERREDSIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_CHECKBOX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_EDIT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ENABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTANGLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTNAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTSIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTUNITS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTWEIGHT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FOREGROUNDCOLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_GRIDBAG_ANCHOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_GRIDBAG_FILL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_GRIDBAG_GRID__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_GRIDBAG_PADDING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_GRIDBAG_PREFERREDSIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_GRIDBAG_WEIGHT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_GROUP_NAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_HORIZONTALALIGNMENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ICON__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_IMAGE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LAYER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LISTBOXTOP_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LISTBOXTOP__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LISTBOX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MAX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MIN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_POPUPMENU__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_PUSHBUTTON__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_RADIOBUTTON__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_RELIEF__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SCROLLABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SLIDERSTEP__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SLIDER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SPINNER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING_COLNB__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TAB__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TEXT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TITLE_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TITLE_SCROLL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TOOLTIPSTRING_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TOOLTIPSTRING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_UNITS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VALUE_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VALUE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VERTICALALIGNMENT__;

import java.awt.Font;
import java.util.Arrays;
import java.util.StringTokenizer;

import org.scilab.modules.graphic_objects.console.Console;
import org.scilab.modules.graphic_objects.event.EventHandler;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;
import org.scilab.modules.graphic_objects.utils.LayoutType;

/**
 * @author Bruno JOFRET
 * @author Vincent COUVERT
 */
public class Uicontrol extends GraphicObject {

    public enum TitlePositionType {
        TOP, LEFT, BOTTOM, RIGHT;
        public static TitlePositionType intToEnum(Integer value) {
            switch (value) {
                default:
                case 0:
                    return TOP;
                case 1:
                    return LEFT;
                case 2:
                    return BOTTOM;
                case 3:
                    return RIGHT;
            }
        }

        public static TitlePositionType stringToEnum(String value) {
            if (value.equals("left")) {
                return LEFT;
            }

            if (value.equals("bottom")) {
                return BOTTOM;
            }

            if (value.equals("right")) {
                return RIGHT;
            }

            return TOP;
        }

        public static String enumToString(TitlePositionType value) {
            switch (value) {
                case BOTTOM:
                    return "bottom";
                case LEFT:
                    return "left";
                case RIGHT:
                    return "right";
                default:
                case TOP:
                    return "top";
            }
        }
    }

    public enum BorderLayoutType {
        BOTTOM, TOP, CENTER, LEFT, RIGHT;
        public static BorderLayoutType intToEnum(Integer value) {
            switch (value) {
                case 0:
                    return BorderLayoutType.BOTTOM;
                case 1:
                    return BorderLayoutType.TOP;
                default:
                case 2:
                    return BorderLayoutType.CENTER;
                case 3:
                    return BorderLayoutType.LEFT;
                case 4:
                    return BorderLayoutType.RIGHT;
            }
        }

        public static BorderLayoutType stringToEnum2(String value) {
            if (value == null || value.equals("")) {
                return CENTER;
            }

            char[] chars = value.toCharArray();
            if (chars[0] == 'n' || chars[0] == 'N') {
                return TOP;
            }

            if (chars[0] == 'e' || chars[0] == 'E') {
                return RIGHT;
            }

            if (chars[0] == 's' || chars[0] == 'S') {
                return BOTTOM;
            }

            if (chars[0] == 'w' || chars[0] == 'W') {
                return LEFT;
            }

            return CENTER;
        }

        public static String enumToString(BorderLayoutType value) {
            switch (value) {
                case BOTTOM:
                    return "bottom";
                default:
                case CENTER:
                    return "center";
                case LEFT:
                    return "left";
                case RIGHT:
                    return "right";
                case TOP:
                    return "top";
            }
        }

        public static BorderLayoutType stringToEnum(String value) {
            if (value.equals("bottom")) {
                return BOTTOM;
            }

            if (value.equals("left")) {
                return LEFT;
            }

            if (value.equals("right")) {
                return RIGHT;
            }

            if (value.equals("top")) {
                return TOP;
            }

            return CENTER;
        }
    }

    public enum FillType {
        NONE, VERTICAL, HORIZONTAL, BOTH;
        public static FillType intToEnum(Integer value) {
            switch (value) {
                default:
                case 0:
                    return FillType.NONE;
                case 1:
                    return FillType.VERTICAL;
                case 2:
                    return FillType.HORIZONTAL;
                case 3:
                    return FillType.BOTH;
            }
        }

        public static FillType stringToEnum(String value) {
            if (value == null || value.equals("")) {
                return NONE;
            }

            char[] chars = value.toCharArray();
            if (chars[0] == 'v' || chars[0] == 'V') {
                return VERTICAL;
            }

            if (chars[0] == 'h' || chars[0] == 'H') {
                return HORIZONTAL;
            }

            if (chars[0] == 'b' || chars[0] == 'B') {
                return BOTH;
            }

            return NONE;
        }

        public static String enumToString(FillType value) {
            switch (value) {
                case BOTH:
                    return "both";
                case HORIZONTAL:
                    return "horizontal";
                case NONE:
                default:
                    return "none";
                case VERTICAL:
                    return "vertical";
            }
        }
    }

    public enum AnchorType {
        CENTER, UPPER, LOWER, RIGHT, LEFT, UPPER_RIGHT, UPPER_LEFT, LOWER_RIGHT, LOWER_LEFT;
        //c,e,w,n,s,ne,nw,se,sw
        public static AnchorType intToEnum(Integer value) {
            switch (value) {
                default:
                case 0:
                    return CENTER;
                case 1:
                    return UPPER;
                case 2:
                    return LOWER;
                case 3:
                    return RIGHT;
                case 4:
                    return LEFT;
                case 5:
                    return UPPER_RIGHT;
                case 6:
                    return UPPER_LEFT;
                case 7:
                    return LOWER_RIGHT;
                case 8:
                    return LOWER_LEFT;
            }
        }

        public static AnchorType stringToEnum2(String value) {
            if (value == null || value.equals("")) {
                return CENTER;
            }

            char[] chars = value.toCharArray();
            if (chars[0] == 'e' || chars[0] == 'E') {
                return RIGHT;
            }

            if (chars[0] == 'w' || chars[0] == 'W') {
                return LEFT;
            }

            if (chars[0] == 'n' || chars[0] == 'N') {
                if (chars.length == 1) {
                    return UPPER;
                }

                if (chars[1] == 'e' || chars[1] == 'E') {
                    return UPPER_RIGHT;
                }

                if (chars[1] == 'w' || chars[1] == 'W') {
                    return UPPER_LEFT;
                }
                return UPPER;
            }

            if (chars[0] == 's' || chars[0] == 'S') {
                if (chars.length == 1) {
                    return LOWER;
                }

                if (chars[1] == 'e' || chars[1] == 'E') {
                    return LOWER_RIGHT;
                }

                if (chars[1] == 'w' || chars[1] == 'W') {
                    return LOWER_LEFT;
                }
                return LOWER;
            }

            return CENTER;
        }

        public static String enumToString(AnchorType value) {
            switch (value) {
                default:
                case CENTER:
                    return "center";
                case LEFT:
                    return "left";
                case LOWER:
                    return "lower";
                case LOWER_LEFT:
                    return "lower_left";
                case LOWER_RIGHT:
                    return "lower_right";
                case RIGHT:
                    return "right";
                case UPPER:
                    return "upper";
                case UPPER_LEFT:
                    return "upper_left";
                case UPPER_RIGHT:
                    return "upper_right";
            }
        }

        public static AnchorType stringToEnum(String value) {
            if (value.equals("left")) {
                return LEFT;
            }

            if (value.equals("lower")) {
                return LOWER;
            }

            if (value.equals("lower_left")) {
                return LOWER_LEFT;
            }

            if (value.equals("lower_right")) {
                return LOWER_RIGHT;
            }

            if (value.equals("right")) {
                return RIGHT;
            }

            if (value.equals("upper")) {
                return UPPER;
            }

            if (value.equals("upper_left")) {
                return UPPER_LEFT;
            }

            if (value.equals("upper_right")) {
                return UPPER_RIGHT;
            }

            return CENTER;
        }
    }

    protected static final String RELIEF_DEFAULT   = "default";
    protected static final String RELIEF_FLAT   = "flat";
    protected static final String RELIEF_RAISED = "raised";
    protected static final String RELIEF_SUNKEN = "sunken";
    protected static final String RELIEF_GROOVE = "groove";
    protected static final String RELIEF_RIDGE  = "ridge";
    protected static final String RELIEF_SOLID  = "solid";

    protected static final double DEFAULT_RED_BACKGROUND = 0.8;
    protected static final double DEFAULT_GREEN_BACKGROUND = 0.8;
    protected static final double DEFAULT_BLUE_BACKGROUND = 0.8;

    private static final double DEFAULT_X = 20.0;
    private static final double DEFAULT_Y = 40.0;
    private static final double DEFAULT_WIDTH = 40.0;
    private static final double DEFAULT_HEIGHT = 20.0;

    protected static final double DEFAULT_FONTSIZE = 10;
    protected static final String DEFAULT_FONTNAME = "helvetica";
    protected static final String DEFAULT_FONTWEIGHT = "normal";
    protected static final String DEFAULT_FONTANGLE = "normal";
    private static final String STRING_SEPARATOR = "|";

    private UicontrolStyle style;
    private Double[] backgroundColor = { -1.0, -1.0, -1.0};
    private Boolean enable = true;
    private String fontAngle = "";
    private String fontName = "";
    private double fontSize = 0;
    private String fontUnits = "points";
    private String fontWeight = "";
    private Double[] foregroundColor = { -1.0, -1.0, -1.0};
    private String horizontalAlignment = "";
    private String verticalAlignment = "";
    private Integer[] listboxTop;
    private double max = 1.0;
    private double min;
    private Double[] position = {DEFAULT_X, DEFAULT_Y, DEFAULT_WIDTH, DEFAULT_HEIGHT};
    private String relief = RELIEF_DEFAULT;
    private Double[] sliderStep = {0.01, 0.1};
    private String[] string = new String[] {};
    private int stringColNb = 1; // Used for tables
    private String[] tooltipString = {""};
    private String units = "pixels";
    private Double[] value;
    private Boolean scrollable = false;
    private LayoutType layout = LayoutType.NONE;
    private Double[] margins = new Double[] {0.0, 0.0, 0.0, 0.0};
    private Integer[] gridbagGrid = new Integer[] { -1, -1, 1, 1};
    private Double[] gridbagWeight = new Double[] {0.0, 0.0};
    private FillType gridbagFill = FillType.NONE;
    private AnchorType gridbagAnchor = AnchorType.CENTER;
    private Integer[] gridbagPadding = new Integer[] {0, 0};
    private Integer[] gridbagPreferredSize = new Integer[] { -1, -1};
    private BorderLayoutType borderPosition = BorderLayoutType.CENTER;
    private Integer[] borderPreferredSize = new Integer[] { -1, -1};
    private String icon = "";

    /** layout options */
    private Integer[] gridOptGrid = new Integer[] {0, 0};
    private Integer[] gridOptPadding = new Integer[] {0, 0};
    private Integer[] borderOptPadding = new Integer[] {0, 0};

    /** border options */
    private Integer frameBorder = 0;

    /** groupname for radiobutton */
    private String groupName = "";

    /** tab properties */
    private Boolean titleScroll = false;
    private TitlePositionType titlePosition = TitlePositionType.TOP;

    private EventHandler eventHandler;

    /**
     * All uicontrol properties
     */
    private enum UicontrolProperty {
        STYLE,
        BACKGROUNDCOLOR,
        ENABLE,
        FONTANGLE,
        FONTNAME,
        FONTSIZE,
        FONTUNITS,
        FONTWEIGHT,
        FOREGROUNDCOLOR,
        HORIZONTALALIGNMENT,
        LISTBOXTOP,
        LISTBOXTOP_SIZE,
        MAX,
        MIN,
        POSITION,
        SCALE,
        SHEAR,
        SLIDERSTEP,
        STRING,
        STRING_SIZE,
        STRING_COLNB,
        RELIEF,
        TOOLTIPSTRING,
        TOOLTIPSTRING_SIZE,
        UNITS,
        VALUE,
        VALUE_SIZE,
        VERTICALALIGNMENT,
        LAYOUT,
        LAYOUT_SET,
        MARGINS,
        GRIDBAG_GRID,
        GRIDBAG_WEIGHT,
        GRIDBAG_FILL,
        GRIDBAG_ANCHOR,
        GRIDBAG_PADDING,
        GRIDBAG_PREFERREDSIZE,
        BORDER_POSITION,
        BORDER_PREFERREDSIZE,
        GRIDOPT_GRID,
        GRIDOPT_PADDING,
        BORDEROPT_PADDING,
        FRAMEBORDER,
        GROUPNAME,
        TITLE_POSITION,
        TITLE_SCROLL,
        SCROLLABLE,
        ICON,
        EVENTHANDLER,
        EVENTHANDLERENABLE
    };

    /**
     * All uicontrol styles
     */
    public enum UicontrolStyle {
        CHECKBOX,
        EDIT,
        FRAME,
        IMAGE,
        LISTBOX,
        POPUPMENU,
        PUSHBUTTON,
        RADIOBUTTON,
        SLIDER,
        TABLE,
        TEXT,
        TABGROUP,
        TAB,
        LAYER,
        SPINNER
    };

    /**
     * Constructor
     */
    public Uicontrol() {
        super();
        eventHandler = new EventHandler();
        setVisible(false); /* To avoid to see the object rendered before all its properties to be set (See bug #10346) */

        if (Console.getConsole().getUseDeprecatedLF()) {
            setBackgroundColor(new Double[] {
                                   DEFAULT_RED_BACKGROUND,
                                   DEFAULT_GREEN_BACKGROUND,
                                   DEFAULT_BLUE_BACKGROUND
                               });

            setHorizontalAlignment("center");
            setVerticalAlignment("middle");

            setFontName(DEFAULT_FONTNAME);
            setFontSize(DEFAULT_FONTSIZE);
            setFontAngle(DEFAULT_FONTANGLE);
            setFontWeight(DEFAULT_FONTWEIGHT);
        }
    }

    /**
     * Get style as a string
     * @param style the uicontrol style
     * @return the uicontrol style as a string
     */
    private int styleEnumToInt(UicontrolStyle style) {
        switch (style) {
            case CHECKBOX:
                return __GO_UI_CHECKBOX__;
            case EDIT:
                return __GO_UI_EDIT__;
            case FRAME:
                return __GO_UI_FRAME__;
            case IMAGE:
                return __GO_UI_IMAGE__;
            case LISTBOX:
                return __GO_UI_LISTBOX__;
            case POPUPMENU:
                return __GO_UI_POPUPMENU__;
            case PUSHBUTTON:
                return __GO_UI_PUSHBUTTON__;
            case RADIOBUTTON:
                return __GO_UI_RADIOBUTTON__;
            case SLIDER:
                return __GO_UI_SLIDER__;
            case TABLE:
                return __GO_UI_TABLE__;
            case TEXT:
                return __GO_UI_TEXT__;
            case TAB:
                return __GO_UI_TAB__;
            case LAYER:
                return __GO_UI_LAYER__;
            case SPINNER:
                return __GO_UI_SPINNER__;
            default :
                return -1;
        }
    }

    /**
     * Get style as an enum element
     * @param style the uicontrol style
     * @return the uicontrol style as an enum element
     */
    private UicontrolStyle intToStyleEnum(int style) {
        switch (style) {
            case __GO_UI_CHECKBOX__ :
                return UicontrolStyle.CHECKBOX;
            case __GO_UI_EDIT__ :
                return UicontrolStyle.EDIT;
            case __GO_UI_FRAME__ :
                return UicontrolStyle.FRAME;
            case __GO_UI_IMAGE__ :
                return  UicontrolStyle.IMAGE;
            case __GO_UI_LISTBOX__ :
                return UicontrolStyle.LISTBOX;
            case __GO_UI_POPUPMENU__ :
                return UicontrolStyle.POPUPMENU;
            case __GO_UI_PUSHBUTTON__ :
                return UicontrolStyle.PUSHBUTTON;
            case __GO_UI_RADIOBUTTON__ :
                return UicontrolStyle.RADIOBUTTON;
            case __GO_UI_SLIDER__ :
                return UicontrolStyle.SLIDER;
            case __GO_UI_TABLE__ :
                return UicontrolStyle.TABLE;
            case __GO_UI_TEXT__ :
                return UicontrolStyle.TEXT;
            case __GO_UI_TAB__:
                return UicontrolStyle.TAB;
            case __GO_UI_LAYER__:
                return UicontrolStyle.LAYER;
            case __GO_UI_SPINNER__:
                return UicontrolStyle.SPINNER;
            default :
                return null;
        }
    }

    /**
     * Get this object type
     * @return uicontrol
     * @see org.scilab.modules.graphic_objects.graphicObject.GraphicObject#getType()
     */
    public Integer getType() {
        return __GO_UICONTROL__;
    }

    /**
     * Returns the enum associated to a property name
     * @param propertyName the property name
     * @return the property enum
     */
    public Object getPropertyFromName(int propertyName) {
        switch (propertyName) {
            case __GO_STYLE__ :
                return UicontrolProperty.STYLE;
            case __GO_UI_BACKGROUNDCOLOR__ :
                return UicontrolProperty.BACKGROUNDCOLOR;
            case __GO_UI_ENABLE__ :
                return UicontrolProperty.ENABLE;
            case __GO_UI_FONTANGLE__ :
                return UicontrolProperty.FONTANGLE;
            case __GO_UI_FONTNAME__ :
                return UicontrolProperty.FONTNAME;
            case __GO_UI_FONTSIZE__ :
                return UicontrolProperty.FONTSIZE;
            case __GO_UI_FONTUNITS__ :
                return UicontrolProperty.FONTUNITS;
            case __GO_UI_FONTWEIGHT__ :
                return UicontrolProperty.FONTWEIGHT;
            case __GO_UI_FOREGROUNDCOLOR__ :
                return UicontrolProperty.FOREGROUNDCOLOR;
            case __GO_UI_HORIZONTALALIGNMENT__ :
                return UicontrolProperty.HORIZONTALALIGNMENT;
            case __GO_UI_LISTBOXTOP__ :
                return UicontrolProperty.LISTBOXTOP;
            case __GO_UI_LISTBOXTOP_SIZE__ :
                return UicontrolProperty.LISTBOXTOP_SIZE;
            case __GO_UI_MAX__ :
                return UicontrolProperty.MAX;
            case __GO_UI_MIN__ :
                return UicontrolProperty.MIN;
            case __GO_POSITION__ :
                return UicontrolProperty.POSITION;
            case __GO_UI_RELIEF__ :
                return UicontrolProperty.RELIEF;
            case __GO_UI_SLIDERSTEP__ :
                return UicontrolProperty.SLIDERSTEP;
            case __GO_UI_STRING__ :
                return UicontrolProperty.STRING;
            case __GO_UI_STRING_SIZE__ :
                return UicontrolProperty.STRING_SIZE;
            case __GO_UI_STRING_COLNB__ :
                return UicontrolProperty.STRING_COLNB;
            case __GO_UI_TOOLTIPSTRING__ :
                return UicontrolProperty.TOOLTIPSTRING;
            case __GO_UI_TOOLTIPSTRING_SIZE__ :
                return UicontrolProperty.TOOLTIPSTRING_SIZE;
            case __GO_UI_UNITS__ :
                return UicontrolProperty.UNITS;
            case __GO_UI_VALUE__ :
                return UicontrolProperty.VALUE;
            case __GO_UI_VALUE_SIZE__ :
                return UicontrolProperty.VALUE_SIZE;
            case __GO_UI_VERTICALALIGNMENT__ :
                return UicontrolProperty.VERTICALALIGNMENT;
            case __GO_LAYOUT__ :
                return UicontrolProperty.LAYOUT;
            case __GO_LAYOUT_SET__ :
                return UicontrolProperty.LAYOUT_SET;
            case __GO_MARGINS__:
                return UicontrolProperty.MARGINS;
            case __GO_UI_GRIDBAG_GRID__:
                return UicontrolProperty.GRIDBAG_GRID;
            case __GO_UI_GRIDBAG_WEIGHT__:
                return UicontrolProperty.GRIDBAG_WEIGHT;
            case __GO_UI_GRIDBAG_FILL__:
                return UicontrolProperty.GRIDBAG_FILL;
            case __GO_UI_GRIDBAG_ANCHOR__:
                return UicontrolProperty.GRIDBAG_ANCHOR;
            case __GO_UI_GRIDBAG_PADDING__:
                return UicontrolProperty.GRIDBAG_PADDING;
            case __GO_UI_GRIDBAG_PREFERREDSIZE__:
                return UicontrolProperty.GRIDBAG_PREFERREDSIZE;
            case __GO_UI_BORDER_POSITION__:
                return UicontrolProperty.BORDER_POSITION;
            case __GO_UI_BORDER_PREFERREDSIZE__:
                return UicontrolProperty.BORDER_PREFERREDSIZE;
            case __GO_GRID_OPT_GRID__:
                return UicontrolProperty.GRIDOPT_GRID;
            case __GO_GRID_OPT_PADDING__:
                return UicontrolProperty.GRIDOPT_PADDING;
            case __GO_BORDER_OPT_PADDING__:
                return UicontrolProperty.BORDEROPT_PADDING;
            case __GO_UI_FRAME_BORDER__:
                return UicontrolProperty.FRAMEBORDER;
            case __GO_UI_GROUP_NAME__:
                return UicontrolProperty.GROUPNAME;
            case __GO_UI_TITLE_POSITION__:
                return UicontrolProperty.TITLE_POSITION;
            case __GO_UI_TITLE_SCROLL__:
                return UicontrolProperty.TITLE_SCROLL;
            case __GO_UI_SCROLLABLE__:
                return UicontrolProperty.SCROLLABLE;
            case __GO_UI_ICON__:
                return UicontrolProperty.ICON;
            case __GO_EVENTHANDLER_NAME__:
                return UicontrolProperty.EVENTHANDLER;
            case __GO_EVENTHANDLER_ENABLE__:
                return UicontrolProperty.EVENTHANDLERENABLE;
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
        if (property == UicontrolProperty.STYLE) {
            return getStyle();
        } else if (property == UicontrolProperty.BACKGROUNDCOLOR) {
            return getBackgroundColor();
        } else if (property == UicontrolProperty.ENABLE) {
            return getEnable();
        } else if (property == UicontrolProperty.FONTANGLE) {
            return getFontAngle();
        } else if (property == UicontrolProperty.FONTNAME) {
            return getFontName();
        } else if (property == UicontrolProperty.FONTSIZE) {
            return getFontSize();
        } else if (property == UicontrolProperty.FONTUNITS) {
            return getFontUnits();
        } else if (property == UicontrolProperty.FONTWEIGHT) {
            return getFontWeight();
        } else if (property == UicontrolProperty.FOREGROUNDCOLOR) {
            return getForegroundColor();
        } else if (property == UicontrolProperty.HORIZONTALALIGNMENT) {
            return getHorizontalAlignment();
        } else if (property == UicontrolProperty.LISTBOXTOP) {
            return getListboxTop();
        } else if (property == UicontrolProperty.LISTBOXTOP_SIZE) {
            return getListboxTopSize();
        } else if (property == UicontrolProperty.MAX) {
            return getMax();
        } else if (property == UicontrolProperty.MIN) {
            return getMin();
        } else if (property == UicontrolProperty.POSITION) {
            return getUiPosition();
        } else if (property == UicontrolProperty.RELIEF) {
            return getRelief();
        } else if (property == UicontrolProperty.SLIDERSTEP) {
            return getSliderStep();
        } else if (property == UicontrolProperty.STRING) {
            return getString();
        } else if (property == UicontrolProperty.STRING_SIZE) {
            return getString().length;
        } else if (property == UicontrolProperty.STRING_COLNB) {
            return getStringColNb();
        } else if (property == UicontrolProperty.TOOLTIPSTRING) {
            return getTooltipString();
        } else if (property == UicontrolProperty.TOOLTIPSTRING_SIZE) {
            return getTooltipString().length;
        } else if (property == UicontrolProperty.UNITS) {
            return getUnits();
        } else if (property == UicontrolProperty.VALUE) {
            return getUiValue();
        } else if (property == UicontrolProperty.VALUE_SIZE) {
            return getUiValueSize();
        } else if (property == UicontrolProperty.VERTICALALIGNMENT) {
            return getVerticalAlignment();
        } else if (property == UicontrolProperty.LAYOUT) {
            return getLayout();
        } else if (property == UicontrolProperty.LAYOUT_SET) {
            return isLayoutSettable();
        } else if (property == UicontrolProperty.MARGINS) {
            return getMargins();
        } else if (property == UicontrolProperty.GRIDBAG_GRID) {
            return getGridBagGrid();
        } else if (property == UicontrolProperty.GRIDBAG_WEIGHT) {
            return getGridBagWeight();
        } else if (property == UicontrolProperty.GRIDBAG_FILL) {
            return getGridBagFill();
        } else if (property == UicontrolProperty.GRIDBAG_ANCHOR) {
            return getGridBagAnchor();
        } else if (property == UicontrolProperty.GRIDBAG_PADDING) {
            return getGridBagPadding();
        } else if (property == UicontrolProperty.GRIDBAG_PREFERREDSIZE) {
            return getGridBagPreferredSize();
        } else if (property == UicontrolProperty.BORDER_POSITION) {
            return getBorderPosition();
        } else if (property == UicontrolProperty.BORDER_PREFERREDSIZE) {
            return getBorderPreferredSize();
        } else if (property == UicontrolProperty.GRIDOPT_GRID) {
            return getGridOptGrid();
        } else if (property == UicontrolProperty.GRIDOPT_PADDING) {
            return getGridOptPadding();
        } else if (property == UicontrolProperty.BORDEROPT_PADDING) {
            return getBorderOptPadding();
        } else if (property == UicontrolProperty.FRAMEBORDER) {
            return getFrameBorder();
        } else if (property == UicontrolProperty.GROUPNAME) {
            return getGroupName();
        } else if (property == UicontrolProperty.TITLE_POSITION) {
            return getTitlePosition();
        } else if (property == UicontrolProperty.TITLE_SCROLL) {
            return getTitleScroll();
        } else if (property == UicontrolProperty.SCROLLABLE) {
            return getScrollable();
        } else if (property == UicontrolProperty.ICON) {
            return getIcon();
        } else if (property == UicontrolProperty.EVENTHANDLER) {
            return getEventHandler();
        } else if (property == UicontrolProperty.EVENTHANDLERENABLE) {
            return getEventHandlerEnable();
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
        if (!(property instanceof UicontrolProperty)) {
            return super.setProperty(property, value);
        }

        UicontrolProperty p = (UicontrolProperty) property;
        switch (p) {
            case STYLE:
                return setStyle((Integer) value);
            case BACKGROUNDCOLOR:
                return setBackgroundColor((Double[]) value);
            case ENABLE:
                return setEnable((Boolean) value);
            case FONTANGLE:
                return setFontAngle((String) value);
            case FONTNAME:
                return setFontName((String) value);
            case FONTSIZE:
                return setFontSize((Double) value);
            case FONTUNITS:
                return setFontUnits((String) value);
            case FONTWEIGHT:
                return setFontWeight((String) value);
            case FOREGROUNDCOLOR:
                return setForegroundColor((Double[]) value);
            case HORIZONTALALIGNMENT:
                return setHorizontalAlignment((String) value);
            case LISTBOXTOP:
                return setListboxTop((Integer[]) value);
            case MAX:
                return setMax((Double) value);
            case MIN:
                return setMin((Double) value);
            case POSITION:
                return setUiPosition((Double[]) value);
            case RELIEF:
                return setRelief((String) value);
            case SLIDERSTEP:
                return setSliderStep((Double[]) value);
            case STRING:
                return setString((String[]) value);
            case STRING_COLNB:
                return setStringColNb((Integer) value);
            case TOOLTIPSTRING:
                return setTooltipString((String[]) value);
            case UNITS:
                return setUnits((String) value);
            case VALUE:
                return setUiValue((Double[]) value);
            case VERTICALALIGNMENT:
                return setVerticalAlignment((String) value);
            case LAYOUT:
                return setLayout((Integer) value);
            case MARGINS:
                return setMargins((Double[]) value);
            case GRIDBAG_GRID:
                return setGridBagGrid((Integer[]) value);
            case GRIDBAG_WEIGHT:
                return setGridBagWeight((Double[]) value);
            case GRIDBAG_FILL:
                return setGridBagFill((Integer) value);
            case GRIDBAG_ANCHOR:
                return setGridBagAnchor((Integer) value);
            case GRIDBAG_PADDING:
                return setGridBagPadding((Integer[]) value);
            case GRIDBAG_PREFERREDSIZE:
                return setGridBagPreferredSize((Integer[]) value);
            case BORDER_POSITION:
                return setBorderPosition((Integer) value);
            case BORDER_PREFERREDSIZE:
                return setBorderPreferredSize((Integer[]) value);
            case GRIDOPT_GRID:
                return setGridOptGrid((Integer[]) value);
            case GRIDOPT_PADDING:
                return setGridOptPadding((Integer[]) value);
            case BORDEROPT_PADDING:
                return setBorderOptPadding((Integer[]) value);
            case FRAMEBORDER:
                return setFrameBorder((Integer) value);
            case GROUPNAME:
                return setGroupName((String) value);
            case TITLE_POSITION:
                return setTitlePosition((Integer) value);
            case TITLE_SCROLL:
                return setTitleScroll((Boolean) value);
            case SCROLLABLE:
                return setScrollable((Boolean) value);
            case ICON:
                return setIcon((String) value);
            case EVENTHANDLER:
                return setEventHandler((String) value);
            case EVENTHANDLERENABLE:
                return setEventHandlerEnable((Boolean) value);
            default:
                return super.setProperty(property, value);
        }
    }

    /**
         * Get the style
         * @return the style
         */
    public Integer getStyle() {
        return styleEnumToInt(style);
    }

    public UicontrolStyle getStyleAsEnum() {
        return style;
    }

    /**
     * Set the style
     * @param style the style
     */
    public UpdateStatus setStyle(int style) {
        UicontrolStyle val =  intToStyleEnum(style);
        if (val == this.style) {
            return UpdateStatus.NoChange;
        }

        this.style = val;
        return UpdateStatus.Success;
    }

    /* Background Color */
    public Double[] getBackgroundColor() {
        return this.backgroundColor;
    }

    public UpdateStatus setBackgroundColor(Double[] colors) {
        if (Arrays.equals(colors, this.backgroundColor)) {
            return UpdateStatus.NoChange;
        }

        this.backgroundColor = colors;
        return UpdateStatus.Success;
    }

    /* Enable */
    public Boolean getEnable() {
        return enable;
    }

    public UpdateStatus setEnable(Boolean status) {
        if (enable.equals(status)) {
            return UpdateStatus.NoChange;
        }
        enable = status;
        return UpdateStatus.Success;
    }

    /* FontAngle */
    public String getFontAngle() {
        return this.fontAngle;
    }

    public UpdateStatus setFontAngle(String fontAngle) {
        if (this.fontAngle.equals(fontAngle)) {
            return UpdateStatus.NoChange;
        }
        this.fontAngle = fontAngle;
        return UpdateStatus.Success;
    }

    /* FontName */
    public String getFontName() {
        return this.fontName;
    }

    public UpdateStatus setFontName(String fontName) {
        if (this.fontName.equals(fontName)) {
            return UpdateStatus.NoChange;
        }
        this.fontName = fontName;
        return UpdateStatus.Success;
    }

    /* FontSize */
    public double getFontSize() {
        return this.fontSize;
    }

    public UpdateStatus setFontSize(double fontSize) {
        if (this.fontSize == fontSize) {
            return UpdateStatus.NoChange;
        }
        this.fontSize = fontSize;
        return UpdateStatus.Success;
    }

    /* FontUnits */
    public String getFontUnits() {
        return this.fontUnits;
    }

    public UpdateStatus setFontUnits(String fontUnits) {
        if (this.fontUnits.equals(fontUnits)) {
            return UpdateStatus.NoChange;
        }
        this.fontUnits = fontUnits;
        return UpdateStatus.Success;
    }

    /* FontWeight */
    public String getFontWeight() {
        return this.fontWeight;
    }

    public UpdateStatus setFontWeight(String fontWeight) {
        if (this.fontWeight.equals(fontWeight)) {
            return UpdateStatus.NoChange;
        }
        this.fontWeight = fontWeight;
        return UpdateStatus.Success;
    }

    /* Foreground Color */
    public Double[] getForegroundColor() {
        return this.foregroundColor;
    }

    public UpdateStatus setForegroundColor(Double[] colors) {
        if (Arrays.equals(this.foregroundColor, colors)) {
            return UpdateStatus.NoChange;
        }
        this.foregroundColor = colors;
        return UpdateStatus.Success;
    }

    /* Horizontal Alignment */
    public String getHorizontalAlignment() {
        return this.horizontalAlignment;
    }

    public UpdateStatus setHorizontalAlignment(String alignment) {
        if (this.horizontalAlignment.equals(alignment)) {
            return UpdateStatus.NoChange;
        }
        this.horizontalAlignment = alignment;
        return UpdateStatus.Success;
    }

    /* Listbox Top */
    public Integer getListboxTopSize() {
        return (listboxTop != null ? listboxTop.length : 0);
    }

    public Integer[] getListboxTop() {
        return this.listboxTop;
    }

    public UpdateStatus setListboxTop(Integer[] listboxTop) {
        if (Arrays.equals(this.listboxTop, listboxTop)) {
            return UpdateStatus.NoChange;
        }
        this.listboxTop = listboxTop;
        return UpdateStatus.Success;
    }

    /* Max */
    public Double getMax() {
        return this.max;
    }

    public UpdateStatus setMax(double max) {
        if (this.max == max) {
            return UpdateStatus.NoChange;
        }
        this.max = max;
        return UpdateStatus.Success;
    }

    /* Min */
    public Double getMin() {
        return this.min;
    }

    public UpdateStatus setMin(double min) {
        if (this.min == min) {
            return UpdateStatus.NoChange;
        }
        this.min = min;
        return UpdateStatus.Success;
    }

    /* Position */
    public Double[] getUiPosition() {
        return this.position;
    }

    public UpdateStatus setUiPosition(Double[] position) {
        if (Arrays.equals(this.position, position)) {
            return UpdateStatus.NoChange;
        }
        this.position = position;
        return UpdateStatus.Success;
    }

    /* Relief */
    public String getRelief() {
        return this.relief;
    }

    public UpdateStatus setRelief(String relief) {
        if (this.relief.equals(relief)) {
            return UpdateStatus.NoChange;
        }
        this.relief = relief;
        return UpdateStatus.Success;
    }

    /**
     * Get the string
     * @return the string
     */
    public String[] getString() {
        return this.string;
    }

    /**
     * Set the string
     * @param string the string
     */
    public UpdateStatus setString(String[] string) {
        //update needed ?

        //both null or same object
        if (this.string == string) {
            return UpdateStatus.NoChange;
        }

        boolean needUpdate = false;
        if (this.string.length == string.length) {
            for (int i = 0 ; i < string.length ; i++) {

                if (this.string[i] == null && string[i] == null) {
                    continue;
                }

                //only one null
                if ((this.string[i] == null || string[i] == null) &&
                        this.string[i] != string[i]) {
                    needUpdate = true;
                    break;
                }

                if (this.string[i].equals(string[i]) == false) {
                    needUpdate = true;
                    break;
                }
            }
        } else {
            needUpdate = true;
        }

        if (needUpdate == false) {
            return UpdateStatus.NoChange;
        }

        if (this.style == UicontrolStyle.LISTBOX || this.style == UicontrolStyle.POPUPMENU) {
            /* String can be set using a|b|c|d */
            if (string.length == 0) {
                if (this.string.length == 0) {
                    return UpdateStatus.NoChange;
                }

                this.string = string;
                return UpdateStatus.Success;
            } else if (string.length == 1 && string[0].contains(STRING_SEPARATOR)) {
                StringTokenizer strTok = new StringTokenizer(string[0], STRING_SEPARATOR);
                String[] stringTab = new String[strTok.countTokens()];
                while (strTok.hasMoreTokens()) {
                    stringTab[stringTab.length - strTok.countTokens()] = strTok.nextToken();
                }
                this.string = stringTab;
                return UpdateStatus.Success;
            }
        }

        this.string = string;
        return UpdateStatus.Success;
    }

    /**
     * Get the string column number
     * @return the number of columns
     */
    public int getStringColNb() {
        return stringColNb;
    }

    /**
     * Set the string column number
     * @param stringColNb the number of columns
     */
    public UpdateStatus setStringColNb(Integer stringColNb) {
        if (this.stringColNb == stringColNb) {
            return UpdateStatus.NoChange;
        }
        this.stringColNb = stringColNb;
        return UpdateStatus.Success;
    }


    /**
     * Get the tooltip string
     * @return the tooltip string
     */
    public String[] getTooltipString() {
        return this.tooltipString;
    }

    /**
     * Set the tooltip string
     * @param tooltipString the tooltip string
     */
    public UpdateStatus setTooltipString(String[] tooltipString) {
        if (Arrays.equals(this.tooltipString, tooltipString)) {
            return UpdateStatus.NoChange;
        }
        this.tooltipString = tooltipString;
        return UpdateStatus.Success;
    }

    /* Slider Step */
    public Double[] getSliderStep() {
        return this.sliderStep;
    }

    public UpdateStatus setSliderStep(Double[] sliderStep) {
        if (Arrays.equals(this.sliderStep, sliderStep)) {
            return UpdateStatus.NoChange;
        }
        this.sliderStep = sliderStep;
        return UpdateStatus.Success;
    }

    /* Units */
    public String getUnits() {
        return this.units;
    }

    public UpdateStatus setUnits(String units) {
        if (this.units.equals(units)) {
            return UpdateStatus.NoChange;
        }
        this.units = units;
        return UpdateStatus.Success;
    }

    /* Value */
    public Integer getUiValueSize() {
        return (value != null ? value.length : 0);
    }

    public Double[] getUiValue() {
        return this.value;
    }

    public UpdateStatus setUiValue(Double[] value) {
        if (Arrays.equals(this.value, value)) {
            return UpdateStatus.NoChange;
        }
        this.value = value;
        return UpdateStatus.Success;
    }

    /* Vertical Alignment */
    public String getVerticalAlignment() {
        return this.verticalAlignment;
    }

    public UpdateStatus setVerticalAlignment(String alignment) {
        if (this.verticalAlignment.equals(alignment)) {
            return UpdateStatus.NoChange;
        }
        this.verticalAlignment = alignment;
        return UpdateStatus.Success;
    }

    public Integer getLayout() {
        return layout.ordinal();
    }

    public LayoutType getLayoutAsEnum() {
        return layout;
    }

    public boolean isLayoutSettable() {
        return (this.layout == LayoutType.NONE);
    }

    public UpdateStatus setLayout(Integer value) {
        return setLayout(LayoutType.intToEnum(value));
    }

    public UpdateStatus setLayout(LayoutType layout) {
        //avoid to set layout twice
        if (this.layout == LayoutType.NONE) {
            if (layout == LayoutType.NONE) {
                return UpdateStatus.NoChange;
            }

            this.layout = layout;
            return UpdateStatus.Success;
        }

        return UpdateStatus.Fail;
    }

    public Double[] getMargins() {
        return margins;
    }

    public UpdateStatus setMargins(Double[] value) {
        UpdateStatus status = UpdateStatus.NoChange;
        if (value.length != 4) {
            return UpdateStatus.Fail;
        }

        for (int i = 0 ; i < 4 ; i++) {
            if (margins[i].equals(value[i]) == false) {
                margins[i] = value[i];
                status = UpdateStatus.Success;
            }
        }

        return status;
    }

    public Integer getBorderPosition() {
        return borderPosition.ordinal();
    }

    public BorderLayoutType getBorderPositionAsEnum() {
        return borderPosition;
    }

    public UpdateStatus setBorderPosition(Integer value) {
        return setBorderPosition(BorderLayoutType.intToEnum(value));
    }

    public UpdateStatus setBorderPosition(BorderLayoutType value) {
        if (borderPosition == value) {
            return UpdateStatus.NoChange;
        }

        borderPosition = value;
        return UpdateStatus.Success;
    }

    public Integer[] getGridBagPadding() {
        return gridbagPadding;
    }

    public UpdateStatus setGridBagPadding(Integer[] value) {
        UpdateStatus status = UpdateStatus.NoChange;
        if (gridbagPadding.length != value.length) {
            return UpdateStatus.Fail;
        }

        for (int i = 0 ; i < value.length ; i++) {
            if (gridbagPadding[i] != value[i]) {
                gridbagPadding[i] = value[i];
                status = UpdateStatus.Success;
            }
        }

        return status;
    }

    public Integer[] getGridBagPreferredSize() {
        return gridbagPreferredSize;
    }

    public UpdateStatus setGridBagPreferredSize(Integer[] value) {
        UpdateStatus status = UpdateStatus.NoChange;
        if (gridbagPreferredSize.length != value.length) {
            return UpdateStatus.Fail;
        }

        for (int i = 0 ; i < value.length ; i++) {
            if (gridbagPreferredSize[i] != value[i]) {
                gridbagPreferredSize[i] = value[i];
                status = UpdateStatus.Success;
            }
        }

        return status;
    }

    public Integer[] getBorderPreferredSize() {
        return borderPreferredSize;
    }

    public UpdateStatus setBorderPreferredSize(Integer[] value) {
        UpdateStatus status = UpdateStatus.NoChange;
        if (borderPreferredSize.length != value.length) {
            return UpdateStatus.Fail;
        }

        for (int i = 0 ; i < value.length ; i++) {
            if (borderPreferredSize[i] != value[i]) {
                borderPreferredSize[i] = value[i];
                status = UpdateStatus.Success;
            }
        }

        return status;
    }

    public Integer getGridBagAnchor() {
        return gridbagAnchor.ordinal();
    }

    public AnchorType getGridBagAnchorAsEnum() {
        return gridbagAnchor;
    }

    public UpdateStatus setGridBagAnchor(Integer value) {
        return setGridBagAnchor(AnchorType.intToEnum(value));
    }

    public UpdateStatus setGridBagAnchor(AnchorType value) {
        if (gridbagAnchor == value) {
            return UpdateStatus.NoChange;
        }

        gridbagAnchor = value;
        return UpdateStatus.Success;
    }

    public Integer getGridBagFill() {
        return gridbagFill.ordinal();
    }

    public FillType getGridBagFillAsEnum() {
        return gridbagFill;
    }

    public UpdateStatus setGridBagFill(Integer value) {
        return setGridBagFill(FillType.intToEnum(value));
    }

    public UpdateStatus setGridBagFill(FillType value) {
        if (gridbagFill == value) {
            return UpdateStatus.NoChange;
        }

        gridbagFill = value;
        return UpdateStatus.Success;
    }

    public Double[] getGridBagWeight() {
        return gridbagWeight;
    }

    public UpdateStatus setGridBagWeight(Double[] value) {
        UpdateStatus status = UpdateStatus.NoChange;
        if (gridbagWeight.length != value.length) {
            return UpdateStatus.Fail;
        }

        for (int i = 0 ; i < value.length ; i++) {
            if (gridbagWeight[i] != value[i]) {
                gridbagWeight[i] = value[i];
                status = UpdateStatus.Success;
            }
        }

        return status;
    }

    public Integer[] getGridBagGrid() {
        return gridbagGrid;
    }

    public UpdateStatus setGridBagGrid(Integer[] value) {
        UpdateStatus status = UpdateStatus.NoChange;
        if (gridbagGrid.length != value.length) {
            return UpdateStatus.Fail;
        }

        for (int i = 0 ; i < value.length ; i++) {
            if (gridbagGrid[i] != value[i]) {
                gridbagGrid[i] = value[i];
                status = UpdateStatus.Success;
            }
        }

        return status;
    }

    public Integer[] getBorderOptPadding() {
        return borderOptPadding;
    }

    public UpdateStatus setBorderOptPadding(Integer[] value) {
        UpdateStatus status = UpdateStatus.NoChange;
        if (borderOptPadding.length != value.length) {
            return UpdateStatus.Fail;
        }

        for (int i = 0 ; i < value.length ; i++) {
            if (borderOptPadding[i] != value[i]) {
                borderOptPadding[i] = value[i];
                status = UpdateStatus.Success;
            }
        }

        return status;
    }

    public Integer[] getGridOptPadding() {
        return gridOptPadding;
    }

    public UpdateStatus setGridOptPadding(Integer[] value) {
        UpdateStatus status = UpdateStatus.NoChange;
        if (gridOptPadding.length != value.length) {
            return UpdateStatus.Fail;
        }

        for (int i = 0 ; i < value.length ; i++) {
            if (gridOptPadding[i] != value[i]) {
                gridOptPadding[i] = value[i];
                status = UpdateStatus.Success;
            }
        }

        return status;
    }

    public Integer[] getGridOptGrid() {
        return gridOptGrid;
    }

    public UpdateStatus setGridOptGrid(Integer[] value) {
        UpdateStatus status = UpdateStatus.NoChange;
        if (gridOptGrid.length != value.length) {
            return UpdateStatus.Fail;
        }

        for (int i = 0 ; i < value.length ; i++) {
            if (gridOptGrid[i] != value[i]) {
                gridOptGrid[i] = value[i];
                status = UpdateStatus.Success;
            }
        }

        return status;
    }

    public Integer getFrameBorder() {
        return frameBorder;
    }

    public UpdateStatus setFrameBorder(Integer value) {
        if (frameBorder.equals(value)) {
            return UpdateStatus.NoChange;
        }

        frameBorder = value;
        return UpdateStatus.Success;
    }

    public String getGroupName() {
        return groupName;
    }

    public UpdateStatus setGroupName(String value) {
        if (groupName.equals(value)) {
            return UpdateStatus.NoChange;
        }

        groupName = value;
        return UpdateStatus.Success;
    }

    public Boolean getTitleScroll() {
        return titleScroll;
    }

    public UpdateStatus setTitleScroll(Boolean value) {
        if (titleScroll == value) {
            return UpdateStatus.NoChange;
        }

        titleScroll = value;
        return UpdateStatus.Success;
    }

    public Integer getTitlePosition() {
        return titlePosition.ordinal();
    }

    public TitlePositionType getTitlePositionAsEnum() {
        return titlePosition;
    }


    public UpdateStatus setTitlePosition(Integer value) {
        return setTitlePosition(TitlePositionType.intToEnum(value));
    }

    public UpdateStatus setTitlePosition(TitlePositionType value) {
        if (titlePosition == value) {
            return UpdateStatus.NoChange;
        }

        titlePosition = value;
        return UpdateStatus.Success;
    }

    public UpdateStatus setScrollable(Boolean scrollable) {
        if (this.scrollable == scrollable) {
            return UpdateStatus.NoChange;
        }

        this.scrollable = scrollable;
        return UpdateStatus.Success;
    }

    public Boolean getScrollable() {
        return scrollable;
    }

    public void setFont(Font font) {
        if (font == null) {
            return;
        }

        setFontName(font.getName());
        setFontSize(font.getSize());

        if (font.isItalic()) {
            setFontAngle("italic");
        } else {
            setFontAngle("normal");
        }

        if (font.isBold()) {
            setFontWeight("bold");
        } else {
            setFontWeight("normal");
        }
    }

    public UpdateStatus setIcon(String icon) {
        if (this.icon.equals(icon)) {
            return UpdateStatus.NoChange;
        }

        this.icon = icon;
        return UpdateStatus.Success;
    }

    public String getIcon() {
        return icon;
    }

    public UpdateStatus setEventHandler(String eventHandlerName) {
        return eventHandler.setEventHandlerString(eventHandlerName);
    }

    public String getEventHandler() {
        return eventHandler.getEventHandlerString();
    }

    public UpdateStatus setEventHandlerEnable(Boolean eventHandlerEnabled) {
        return eventHandler.setEventHandlerEnabled(eventHandlerEnabled);
    }

    public boolean getEventHandlerEnable() {
        return eventHandler.getEventHandlerEnabled();
    }

    public void accept(Visitor visitor) {
    }
}
