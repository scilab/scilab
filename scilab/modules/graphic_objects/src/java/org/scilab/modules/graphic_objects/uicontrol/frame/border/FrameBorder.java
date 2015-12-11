package org.scilab.modules.graphic_objects.uicontrol.frame.border;

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
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER__;

import java.util.Arrays;

import org.scilab.modules.graphic_objects.ObjectRemovedException;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;

public class FrameBorder extends GraphicObject {
    enum FrameBorderProperty {
        TYPE, //RAISED or LOWERED
        COLOR, //string
        THICKNESS, //int
        ROUNDED, //boolean
        STYLE, //FrameBorderType
        HIGHLIGHT_OUT, //string
        HIGHLIGHT_IN, //string
        SHADOW_OUT, //string
        SHADOW_IN, //string
        TITLE_BORDER,
        TITLE, //string
        JUSTIFICATION, //JustificationType
        FONTNAME, //string
        FONTANGLE, // string
        FONT_SIZE, //int
        FONT_WEIGHT, //string
        TITLE_POSITION, // TitlePostionType
        POSITION, //tlbr
        IN_BORDER,
        OUT_BORDER,
    };

    public enum BorderType {
        RAISED, LOWERED;
        public static BorderType intToEnum(Integer value) {
            switch (value) {
                default:
                case 0:
                    return RAISED;
                case 1:
                    return LOWERED;
            }
        }

        public static BorderType stringToEnum(String value) {
            if (value == null || value.equals("")) {
                return null;
            }

            char[] chars = value.toCharArray();
            if (chars[0] == 'l' || chars[0] == 'L') {
                return LOWERED;
            }

            return RAISED;
        }
    }

    public enum JustificationType {
        LEADING, LEFT, CENTER, RIGHT, TRAILING;

        public static JustificationType intToEnum(Integer val) {
            switch (val) {
                default :
                case 0:
                    return LEADING;
                case 1:
                    return LEFT;
                case 2:
                    return CENTER;
                case 3:
                    return RIGHT;
                case 4:
                    return TRAILING;
            }
        }

        public static JustificationType stringToEnum(String value) {
            if (value == null || value.equals("")) {
                return null;
            }

            char[] chars = value.toCharArray();
            if (chars[0] == 'l' || chars[0] == 'L') {
                if (chars[3] == 'f' || chars[3] == 'F') {
                    return LEFT;
                }
                return LEADING;
            }

            if (chars[0] == 'c' || chars[0] == 'C') {
                return CENTER;
            }

            if (chars[0] == 'r' || chars[0] == 'R') {
                return RIGHT;
            }

            if (chars[0] == 't' || chars[0] == 'T') {
                return TRAILING;
            }


            return LEADING;
        }

    };

    public enum TitlePositionType {
        TOP, ABOVE_TOP, BELOW_TOP, BOTTOM, ABOVE_BOTTOM, BELOW_BOTTOM;

        public static TitlePositionType intToEnum(Integer val) {
            switch (val) {
                default :
                case 0:
                    return TOP;
                case 1:
                    return ABOVE_TOP;
                case 2:
                    return BELOW_TOP;
                case 3:
                    return BOTTOM;
                case 4:
                    return ABOVE_BOTTOM;
                case 5:
                    return BELOW_BOTTOM;
            }
        }

        public static TitlePositionType stringToEnum(String value) {
            if (value == null || value.equals("")) {
                return null;
            }

            if (value.equalsIgnoreCase("above_top")) {
                return ABOVE_TOP;
            }

            if (value.equalsIgnoreCase("below_top")) {
                return BELOW_TOP;
            }

            if (value.equalsIgnoreCase("bottom")) {
                return BOTTOM;
            }

            if (value.equalsIgnoreCase("above_bottom")) {
                return ABOVE_BOTTOM;
            }

            if (value.equalsIgnoreCase("below_bottom")) {
                return BELOW_BOTTOM;
            }

            return TOP;
        }
    };

    private BorderType type = null;//BorderType.NONE;
    private String color = null;
    private String hlOutColor = null;
    private String hlInColor = null;
    private String shadowOutColor = null;
    private String shadowInColor = null;
    private Integer thickness = null;
    private Boolean rounded = null;
    private FrameBorderType style = null;//FrameBorderType.ETCHED;
    private String title = null;
    private JustificationType justification = null;//JustificationType.LEADING;
    private String fontAngle = null;
    private String fontName = null;
    private Integer fontSize = null;
    private String fontWeight = null;
    private TitlePositionType titlePosition = TitlePositionType.TOP;
    private Double[] position = null;//new Double[] {0.0, 0.0, 0.0, 0.0};//TLBR

    private Integer titleBorder = null;
    private Integer inBorder = null;
    private Integer outBorder = null;

    public FrameBorder() {

    }

    /**
     * Get style as a string
     * @param style the uicontrol style
     * @return the uicontrol style as a string
     */
    public Object getPropertyFromName(int propertyName) {
        switch (propertyName) {
            case __GO_UI_FRAME_BORDER_COLOR__:
                return FrameBorderProperty.COLOR;
            case __GO_UI_FONTANGLE__:
                return FrameBorderProperty.FONTANGLE;
            case __GO_UI_FONTNAME__:
                return FrameBorderProperty.FONTNAME;
            case __GO_UI_FONTSIZE__:
                return FrameBorderProperty.FONT_SIZE;
            case __GO_UI_FONTWEIGHT__:
                return FrameBorderProperty.FONT_WEIGHT;
            case __GO_UI_FRAME_BORDER_HIGHLIGHT_IN__:
                return FrameBorderProperty.HIGHLIGHT_IN;
            case __GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__:
                return FrameBorderProperty.HIGHLIGHT_OUT;
            case __GO_UI_FRAME_BORDER_IN_BORDER__:
                return FrameBorderProperty.IN_BORDER;
            case __GO_UI_FRAME_BORDER_JUSTIFICATION__:
                return FrameBorderProperty.JUSTIFICATION;
            case __GO_UI_FRAME_BORDER_OUT_BORDER__:
                return FrameBorderProperty.OUT_BORDER;
            case __GO_POSITION__:
                return FrameBorderProperty.POSITION;
            case __GO_UI_FRAME_BORDER_ROUNDED__:
                return FrameBorderProperty.ROUNDED;
            case __GO_UI_FRAME_BORDER_SHADOW_IN__:
                return FrameBorderProperty.SHADOW_IN;
            case __GO_UI_FRAME_BORDER_SHADOW_OUT__:
                return FrameBorderProperty.SHADOW_OUT;
            case __GO_UI_FRAME_BORDER_STYLE__:
                return FrameBorderProperty.STYLE;
            case __GO_LINE_THICKNESS__:
                return FrameBorderProperty.THICKNESS;
            case __GO_TITLE__:
                return FrameBorderProperty.TITLE;
            case __GO_UI_FRAME_BORDER_TITLE__:
                return FrameBorderProperty.TITLE_BORDER;
            case __GO_UI_FRAME_BORDER_POSITION__:
                return FrameBorderProperty.TITLE_POSITION;
            case __GO_UI_FRAME_BORDER_TYPE__:
                return FrameBorderProperty.TYPE;
            default:
                return super.getPropertyFromName(propertyName);
        }
    }

    public Object getProperty(Object property) {
        if (property instanceof FrameBorderProperty) {
            switch ((FrameBorderProperty)property) {
                case COLOR:
                    return getColor();
                case FONTANGLE:
                    return getFontAngle();
                case FONTNAME:
                    return getFontName();
                case FONT_SIZE:
                    return getFontSize();
                case FONT_WEIGHT:
                    return getFontWeight();
                case HIGHLIGHT_IN:
                    return getHlIn();
                case HIGHLIGHT_OUT:
                    return getHlOut();
                case IN_BORDER:
                    return getInBorder();
                case JUSTIFICATION:
                    return getJustification();
                case OUT_BORDER:
                    return getOutBorder();
                case POSITION:
                    return getPosition();
                case ROUNDED:
                    return getRounded();
                case SHADOW_IN:
                    return getShadowIn();
                case SHADOW_OUT:
                    return getShadowOut();
                case STYLE:
                    return getStyle();
                case THICKNESS:
                    return getThickness();
                case TITLE:
                    return getTitle();
                case TITLE_BORDER:
                    return getTitleBorder();
                case TITLE_POSITION:
                    return getTitlePosition();
                case TYPE:
                    return getBorderType();
            }
        }

        return super.getProperty(property);
    }

    public UpdateStatus setProperty(Object property, Object value) {
        if (property instanceof FrameBorderProperty) {
            switch ((FrameBorderProperty) property) {
                case COLOR:
                    return setColor((String)value);
                case FONTANGLE:
                    return setFontAngle((String)value);
                case FONTNAME:
                    return setFontName((String)value);
                case FONT_SIZE:
                    return setFontSize((Integer)value);
                case FONT_WEIGHT:
                    return setFontWeight((String)value);
                case HIGHLIGHT_IN:
                    return setHlIn((String)value);
                case HIGHLIGHT_OUT:
                    return setHlOut((String)value);
                case IN_BORDER:
                    return setInBorder((Integer)value);
                case JUSTIFICATION:
                    return setJustification((Integer)value);
                case OUT_BORDER:
                    return setOutBorder((Integer)value);
                case POSITION:
                    return setPosition((Double[])value);
                case ROUNDED:
                    return setRounded((Boolean)value);
                case SHADOW_IN:
                    return setShadowIn((String)value);
                case SHADOW_OUT:
                    return setShadowOut((String)value);
                case STYLE:
                    return setStyle((Integer)value);
                case THICKNESS:
                    return setThickness((Integer)value);
                case TITLE:
                    return setTitle((String)value);
                case TITLE_BORDER:
                    return setTitleBorder((Integer)value);
                case TITLE_POSITION:
                    return setTitlePosition((Integer)value);
                case TYPE:
                    return setBorderType((Integer)value);
            }
        }
        return super.setProperty(property, value);
    }

    /** type */
    public Integer getBorderType() {
        return type == null ? null : type.ordinal();
    }

    public BorderType getBorderTypeAsEnum() {
        return type;
    }

    public UpdateStatus setBorderType(BorderType value) {
        if (value == type) {
            return UpdateStatus.NoChange;
        }

        type = value;
        return UpdateStatus.Success;
    }

    public UpdateStatus setBorderType(Integer value) {
        return setBorderType(BorderType.intToEnum(value));
    }

    /** titleposition */
    public Integer getTitlePosition() {
        return titlePosition == null ? null : titlePosition.ordinal();
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

    /** titleborder */
    public Integer getTitleBorder() {
        return titleBorder;
    }

    public UpdateStatus setTitleBorder(Integer value) {
        if (titleBorder != null && titleBorder.equals(value)) {
            return UpdateStatus.NoChange;
        }

        titleBorder = value;
        return UpdateStatus.Success;
    }

    /** title */
    public String getTitle() {
        return title;
    }

    public UpdateStatus setTitle(String value) {
        if (title != null && title.equals(value)) {
            return UpdateStatus.NoChange;
        }

        title = value;
        return UpdateStatus.Success;
    }

    /** thickness */
    public Integer getThickness() {
        return thickness;
    }

    public UpdateStatus setThickness(Integer value) {
        if (thickness != null && thickness.equals(value)) {
            return UpdateStatus.NoChange;
        }

        thickness = value;
        return UpdateStatus.Success;
    }

    /** style */
    public Integer getStyle() {
        return style == null ? null : style.ordinal();
    }

    public FrameBorderType getStyleAsEnum() {
        return style;
    }

    public UpdateStatus setStyle(Integer value) {
        return setStyle(FrameBorderType.intToEnum(value));
    }

    public UpdateStatus setStyle(FrameBorderType value) {
        if (style != null && style.equals(value)) {
            return UpdateStatus.NoChange;
        }

        style = value;
        return UpdateStatus.Success;
    }

    /** shadowOut */
    public String getShadowOut() {
        return shadowOutColor;
    }

    public UpdateStatus setShadowOut(String value) {
        if (shadowOutColor != null && shadowOutColor.equals(value)) {
            return UpdateStatus.NoChange;
        }

        shadowOutColor = value;
        return UpdateStatus.Success;
    }

    /** shadowIn */
    public String getShadowIn() {
        return shadowInColor;
    }

    public UpdateStatus setShadowIn(String value) {
        if (shadowInColor != null && shadowInColor.equals(value)) {
            return UpdateStatus.NoChange;
        }

        shadowInColor = value;
        return UpdateStatus.Success;
    }

    /** rounded */
    public Boolean getRounded() {
        return rounded;
    }

    public UpdateStatus setRounded(Boolean value) {
        if (rounded != null && rounded.equals(value)) {
            return UpdateStatus.NoChange;
        }

        rounded = value;
        return UpdateStatus.Success;
    }

    /** position */
    public Double[] getPosition() {
        return position;
    }

    public UpdateStatus setPosition(Double[] value) {
        if (position != null && Arrays.equals(position, value)) {
            return UpdateStatus.NoChange;
        }

        position = value;
        return UpdateStatus.Success;
    }

    /** outBorder */
    public Integer getOutBorder() {
        return outBorder;
    }

    public UpdateStatus setOutBorder(Integer value) {
        if (outBorder != null && outBorder.equals(value)) {
            return UpdateStatus.NoChange;
        }

        outBorder = value;
        return UpdateStatus.Success;
    }

    /** justification */
    public Integer getJustification() {
        return justification == null ? null : justification.ordinal();
    }

    public JustificationType getJustificationAsEnum() {
        return justification;
    }

    public UpdateStatus setJustification(Integer value) {
        return setJustification(JustificationType.intToEnum(value));
    }

    public UpdateStatus setJustification(JustificationType value) {
        if (justification == value) {
            return UpdateStatus.NoChange;
        }

        justification = value;
        return UpdateStatus.Success;
    }

    /** inBorder */
    public Integer getInBorder() {
        return inBorder;
    }

    public UpdateStatus setInBorder(Integer value) {
        if (inBorder != null && inBorder.equals(value)) {
            return UpdateStatus.NoChange;
        }

        inBorder = value;
        return UpdateStatus.Success;
    }

    /** hlOutColor */
    public String getHlOut() {
        return hlOutColor;
    }

    public UpdateStatus setHlOut(String value) {
        if (hlOutColor != null && hlOutColor.equals(value)) {
            return UpdateStatus.NoChange;
        }

        hlOutColor = value;
        return UpdateStatus.Success;
    }

    /** hlInColor */
    public String getHlIn() {
        return hlInColor;
    }

    public UpdateStatus setHlIn(String value) {
        if (hlInColor != null && hlInColor.equals(value)) {
            return UpdateStatus.NoChange;
        }

        hlInColor = value;
        return UpdateStatus.Success;
    }

    /** fontWeight */
    public String getFontWeight() {
        return fontWeight;
    }

    public UpdateStatus setFontWeight(String value) {
        if (fontWeight != null && fontWeight.equals(value)) {
            return UpdateStatus.NoChange;
        }

        fontWeight = value;
        return UpdateStatus.Success;
    }

    /** fontSize */
    public Integer getFontSize() {
        return fontSize;
    }

    public UpdateStatus setFontSize(Integer value) {
        if (fontSize != null && fontSize.equals(value)) {
            return UpdateStatus.NoChange;
        }

        fontSize = value;
        return UpdateStatus.Success;
    }

    /** fontName */
    public String getFontName() {
        return fontName;
    }

    public UpdateStatus setFontName(String value) {
        if (fontName != null && fontName.equals(value)) {
            return UpdateStatus.NoChange;
        }

        fontName = value;
        return UpdateStatus.Success;
    }

    /** fontAngle */
    public String getFontAngle() {
        return fontAngle;
    }

    public UpdateStatus setFontAngle(String value) {
        if (fontAngle != null && fontAngle.equals(value)) {
            return UpdateStatus.NoChange;
        }

        fontAngle = value;
        return UpdateStatus.Success;
    }

    /** color */
    public String getColor() {
        return color;
    }

    public UpdateStatus setColor(String value) {
        if (color != null && color.equals(value)) {
            return UpdateStatus.NoChange;
        }

        color = value;
        return UpdateStatus.Success;
    }

    public void accept(Visitor visitor) throws ObjectRemovedException {
    }

    public Integer getType() {
        return __GO_UI_FRAME_BORDER__;
    }

}
