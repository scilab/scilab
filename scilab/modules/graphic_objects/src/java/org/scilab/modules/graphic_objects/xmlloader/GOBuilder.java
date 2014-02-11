package org.scilab.modules.graphic_objects.xmlloader;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_BORDER_OPT_PADDING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DOCKABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_GRID_OPT_GRID__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_GRID_OPT_PADDING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_INFOBAR_VISIBLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_LAYOUT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_LINE_THICKNESS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_MARGINS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_MENUBAR_VISIBLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_NAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_RESIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_STYLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TAG__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TITLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TOOLBAR_VISIBLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TYPE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_BACKGROUNDCOLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_BORDER_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_CHECKBOX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ENABLE__;
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
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_SCROLLABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_GRIDBAG_ANCHOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_GRIDBAG_FILL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_GRIDBAG_GRID__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_GRIDBAG_PADDING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_GRIDBAG_WEIGHT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_HORIZONTALALIGNMENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ICON__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MAX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_PUSHBUTTON__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SCROLLABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TAB__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TEXT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_VISIBLE__;

import java.awt.Color;
import java.io.File;
import java.lang.reflect.Field;
import java.util.Map;

import org.scilab.modules.graphic_objects.builder.Builder;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.uicontrol.Uicontrol;
import org.scilab.modules.graphic_objects.uicontrol.frame.border.FrameBorder;
import org.scilab.modules.graphic_objects.uicontrol.frame.border.FrameBorder.BorderType;
import org.scilab.modules.graphic_objects.uicontrol.frame.border.FrameBorderType;
import org.scilab.modules.graphic_objects.utils.LayoutType;
import org.xml.sax.Attributes;

public class GOBuilder {
    public static Integer figureBuilder(GraphicController controller, Attributes attributes) {
        Integer fig = Builder.createFigure(false, 0, 0, false, false);
        //Integer fig = Builder.createNewFigureWithAxes();
        String item = null;

        // hide toolbar
        controller.setProperty(fig, __GO_TOOLBAR_VISIBLE__, false);

        // hide menubar
        controller.setProperty(fig, __GO_MENUBAR_VISIBLE__, false);

        // hide infobar
        controller.setProperty(fig, __GO_INFOBAR_VISIBLE__, false);

        // remove docking bar
        controller.setProperty(fig, __GO_DOCKABLE__, false);

        // id
        XmlTools.setPropAsString(fig, __GO_TAG__, attributes.getValue("id"));

        // position
        Integer[] position = (Integer[]) controller.getProperty(fig, __GO_POSITION__);

        // posX
        item = attributes.getValue("posX");
        if (item != null) {
            position[0] = Integer.parseInt(item);
        }

        // posY
        item = attributes.getValue("posY");
        if (item != null) {
            position[1] = Integer.parseInt(item);
        }

        // set position
        controller.setProperty(fig, __GO_POSITION__, position);

        Integer[] size = (Integer[]) controller.getProperty(fig, __GO_SIZE__);
        // width
        item = attributes.getValue("width");
        if (item != null) {
            size[0] = Integer.parseInt(item);
        }

        // height
        item = attributes.getValue("height");
        if (item != null) {
            size[1] = Integer.parseInt(item);
        }

        // set size
        controller.setProperty(fig, __GO_SIZE__, size);

        // title
        XmlTools.setPropAsString(fig, __GO_NAME__, attributes.getValue("title"));

        // icon
        // XmlTools.setPropAsString(fig, __GO_ICON__,
        // attributes.getValue("icon"));

        // resizable
        XmlTools.setPropAsBoolean(fig, __GO_RESIZE__, attributes.getValue("resizable"));

        // layout and layout_options
        item = attributes.getValue("layout");
        if (item == null) {
            item = "name:border"; //default layout is border
        }

        setLayoutProperty(controller, fig, item);


        //icon
        item = attributes.getValue("icon");
        if (item != null) {
            controller.setProperty(fig, __GO_UI_ICON__, item);
        }

        // visible
        XmlTools.setPropAsBoolean(fig, __GO_VISIBLE__, attributes.getValue("visible"));

        return fig;
    }

    public static Integer uicontrolBuilder(GraphicController controller, int type, Attributes attributes, int parent) {
        if (type == __GO_UI_FRAME__) {
            String item = attributes.getValue("scrollable");
            if (item != null && item.equals("true")) {
                type = __GO_UI_FRAME_SCROLLABLE__;
            }
        }

        Integer uic = controller.askObject(GraphicObject.getTypeFromName(type));
        return uicontrolUpdater(controller, uic, attributes, parent);
    }

    public static Integer uicontrolUpdater(GraphicController controller, int uic, Attributes attributes, int parent) {
        return uicontrolUpdater(controller, uic, attributes, parent, null);
    }

    public static Integer uicontrolUpdater(GraphicController controller, int uic, Attributes attributes, int parent, Map<String, String> fromModel) {
        String item = null;

        try {
            Map<String, String> map = null;
            // Common attributes

            // id
            XmlTools.setPropAsString(uic, __GO_TAG__, attributes.getValue("id"));

            // layout and layout_options
            item = attributes.getValue("layout");
            if (item == null) {
                if (fromModel != null) {
                    item = fromModel.get("layout");
                }
            }

            if (item == null) {
                item = "name:none";
            }
            setLayoutProperty(controller, uic, item);

            //visible
            item = attributes.getValue("visible");
            if (item == null || item.equals("true") || item.equals("on")) {
                controller.setProperty(uic, __GO_VISIBLE__, true);
            } else {
                controller.setProperty(uic, __GO_VISIBLE__, false);
            }

            //enable
            item = attributes.getValue("enable");
            if (item == null || item.equals("true") || item.equals("on")) {
                controller.setProperty(uic, __GO_UI_ENABLE__, true);
            } else {
                controller.setProperty(uic, __GO_UI_ENABLE__, false);
            }

            //backgroundcolor
            item = attributes.getValue("background");
            if (item != null) {

                controller.setProperty(uic, __GO_UI_BACKGROUNDCOLOR__, getColor(item));
            }

            // constraints

            // get parent layout
            LayoutType layout = LayoutType.NONE;
            if (parent != 0) {
                layout = LayoutType.intToEnum((Integer) controller.getProperty(parent, __GO_LAYOUT__));
            }

            if (layout != LayoutType.NONE) {
                item = attributes.getValue("constraint");
                if (item == null && fromModel != null) {
                    // use value from model if property is not in "declaration"
                    item = XmlTools.getFromMap(fromModel, "constraint");
                }

                if (item != null) {
                    map = CSSParser.parseLine(item);
                }

                switch (layout) {
                    case BORDER: {
                        item = XmlTools.getFromMap(map, "position", "center");
                        controller.setProperty(uic, __GO_UI_BORDER_POSITION__, Uicontrol.BorderLayoutType.stringToEnum(item).ordinal());
                        break;
                    }
                    case GRIDBAG: {
                        Integer[] grid = new Integer[] { 0, 0, 0, 0 };
                        grid[0] = Integer.parseInt(XmlTools.getFromMap(map, "gridx", "0"));
                        grid[1] = Integer.parseInt(XmlTools.getFromMap(map, "gridy", "0"));
                        grid[2] = Integer.parseInt(XmlTools.getFromMap(map, "gridwidth", "1"));
                        grid[3] = Integer.parseInt(XmlTools.getFromMap(map, "gridheight", "1"));

                        Double[] weight = new Double[] { 0.0, 0.0 };
                        weight[0] = Double.parseDouble(XmlTools.getFromMap(map, "weightx", "1.0"));
                        weight[1] = Double.parseDouble(XmlTools.getFromMap(map, "weighty", "1.0"));

                        Integer fill = Uicontrol.FillType.stringToEnum(XmlTools.getFromMap(map, "fill", "none")).ordinal();
                        Integer anchor = Uicontrol.AnchorType.stringToEnum(XmlTools.getFromMap(map, "anchor", "center")).ordinal();

                        Integer[] padding = new Integer[] { 0, 0 };
                        padding[0] = Integer.parseInt(XmlTools.getFromMap(map, "ipadx", "0"));
                        padding[1] = Integer.parseInt(XmlTools.getFromMap(map, "ipady", "0"));

                        controller.setProperty(uic, __GO_UI_GRIDBAG_GRID__, grid);
                        controller.setProperty(uic, __GO_UI_GRIDBAG_WEIGHT__, weight);
                        controller.setProperty(uic, __GO_UI_GRIDBAG_FILL__, fill);
                        controller.setProperty(uic, __GO_UI_GRIDBAG_ANCHOR__, anchor);
                        controller.setProperty(uic, __GO_UI_GRIDBAG_PADDING__, padding);
                        break;
                    }
                    default:
                        break;
                }

                // insets -> marging
                String insets = XmlTools.getFromMap(map, "insets", "0,0,0,0");
                Double[] margins = new Double[] { 0.0, 0.0, 0.0, 0.0 };
                String[] insert = insets.split(",");
                for (int i = 0; i < insert.length && i < 4; i++) {
                    margins[i] = Double.parseDouble(insert[i]);
                }

                controller.setProperty(uic, __GO_MARGINS__, margins);
            }

            // specific attributes
            Integer type = (Integer) controller.getProperty(uic, __GO_STYLE__);
            switch (type) {
                case __GO_UI_FRAME__: {
                    // border
                    item = attributes.getValue("border");
                    if (item == null && fromModel != null) {
                        // use value from model if property is not in
                        // "declaration"
                        item = XmlTools.getFromMap(fromModel, "border");
                    }

                    if (item != null) {
                        map = CSSParser.parseLine(item);
                    }

                    FrameBorderType borderType = FrameBorderType.stringToEnum(XmlTools.getFromMap(map, "name", "none"));
                    Integer border = createBorder(controller, uic, borderType, map);
                    controller.setProperty(uic, __GO_UI_FRAME_BORDER__, border);

                    String[] text = new String[1];
                    text[0] = attributes.getValue("tab-title");
                    if (text[0] != null) {
                        controller.setProperty(uic, __GO_UI_STRING__, text);
                    }

                    item = attributes.getValue("scrollable");
                    if (item == null || item.equals("false")) {
                        controller.setProperty(uic, __GO_UI_SCROLLABLE__, false);
                    } else {
                        controller.setProperty(uic, __GO_UI_SCROLLABLE__, true);
                    }

                    break;
                }

                case __GO_UI_TAB__: {
                    item = attributes.getValue("ui-style");
                    if (item != null) {
                        map = CSSParser.parseLine(item);
                        item = XmlTools.getFromMap(map, "bold", "false");
                        if (item.equals("true")) {
                            controller.setProperty(uic, __GO_UI_FONTWEIGHT__, "bold");
                        }

                        item = XmlTools.getFromMap(map, "italic", "false");
                        if (item.equals("true")) {
                            controller.setProperty(uic, __GO_UI_FONTANGLE__, "italic");
                        }
                    }
                    break;
                }
                case __GO_UI_CHECKBOX__: {
                    controller.setProperty(uic, __GO_UI_HORIZONTALALIGNMENT__, "left");

                    String[] text = new String[1];
                    text[0] = attributes.getValue("text");
                    if (text[0] != null) {
                        controller.setProperty(uic, __GO_UI_STRING__, text);
                    }
                    break;
                }
                case __GO_UI_PUSHBUTTON__: {

                    //string
                    String[] text = new String[1];
                    text[0] = attributes.getValue("text");
                    if (text[0] != null) {
                        controller.setProperty(uic, __GO_UI_STRING__, text);
                    }


                    //icon
                    item = attributes.getValue("icon");
                    if (item != null) {
                        controller.setProperty(uic, __GO_UI_ICON__, item);
                    }

                    break;
                }
                case __GO_UI_TEXT__:
                    controller.setProperty(uic, __GO_UI_HORIZONTALALIGNMENT__, "left");
                    String[] text = new String[1];
                    text[0] = attributes.getValue("text");
                    if (text[0] != null) {
                        controller.setProperty(uic, __GO_UI_STRING__, text);
                    }

                    String columns = attributes.getValue("columns");
                    if (columns != null) {
                        controller.setProperty(uic, __GO_UI_MAX__, Double.parseDouble(columns));
                    }

                    //icon
                    item = attributes.getValue("icon");
                    if (item != null) {
                        controller.setProperty(uic, __GO_UI_ICON__, item);
                    }
                    break;
            }

        } catch (CSSParserException e) {
            e.printStackTrace();
        }

        return uic;
    }

    private static void setLayoutProperty(GraphicController controller, Integer uid, String item) {
        try {
            Map<String, String> map = null;
            map = CSSParser.parseLine(item);
            item = map.get("name");
            LayoutType layout = LayoutType.stringToEnum(item);
            controller.setProperty(uid, __GO_LAYOUT__, layout.ordinal());

            // layout_options
            switch (layout) {
                case BORDER: {
                    Integer[] pad = new Integer[] { 0, 0 };
                    pad[0] = Integer.parseInt(XmlTools.getFromMap(map, "hgap", "0"));
                    pad[1] = Integer.parseInt(XmlTools.getFromMap(map, "vgap", "0"));

                    controller.setProperty(uid, __GO_BORDER_OPT_PADDING__, pad);
                    break;
                }
                case GRID: {
                    Integer[] grid = new Integer[] { 0, 0 };
                    grid[0] = Integer.parseInt(XmlTools.getFromMap(map, "rows", "0"));
                    grid[1] = Integer.parseInt(XmlTools.getFromMap(map, "cols", "0"));

                    Integer[] pad = new Integer[] { 0, 0 };
                    pad[0] = Integer.parseInt(XmlTools.getFromMap(map, "hgap", "0"));
                    pad[1] = Integer.parseInt(XmlTools.getFromMap(map, "vgap", "0"));

                    controller.setProperty(uid, __GO_GRID_OPT_GRID__, grid);
                    controller.setProperty(uid, __GO_GRID_OPT_PADDING__, pad);
                    break;
                }
                default:
                case GRIDBAG:
                case NONE:
                    // nothing to do
                    break;
            }
        } catch (CSSParserException e) {
            e.printStackTrace();
        }

    }

    private static Integer createBorder(GraphicController controller, Integer uic, FrameBorderType borderType, Map<String, String> map) {
        Integer border = controller.askObject(GraphicObject.getTypeFromName(__GO_UI_FRAME_BORDER__));
        controller.setProperty(border, __GO_UI_FRAME_BORDER_STYLE__, borderType.ordinal());

        switch (borderType) {
            case SOFTBEVEL:
            case BEVEL: {
                BorderType etching = BorderType.stringToEnum(XmlTools.getFromMap(map, "type", "none"));
                String color = XmlTools.getFromMap(map, "color", "black");

                controller.setProperty(border, __GO_TYPE__, etching.ordinal());
                controller.setProperty(border, __GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__, color);
                controller.setProperty(border, __GO_UI_FRAME_BORDER_HIGHLIGHT_IN__, color);
                controller.setProperty(border, __GO_UI_FRAME_BORDER_SHADOW_OUT__, color);
                controller.setProperty(border, __GO_UI_FRAME_BORDER_SHADOW_IN__, color);
                break;
            }
            case COMPOUND: {
                FrameBorderType borderOut = FrameBorderType.stringToEnum(XmlTools.getFromMap(map, "border-out", "none"));
                FrameBorderType borderIn = FrameBorderType.stringToEnum(XmlTools.getFromMap(map, "border-in", "none"));

                Integer out = createBorder(controller, border, borderOut, map);
                Integer in = createBorder(controller, border, borderIn, map);

                controller.setProperty(border, __GO_UI_FRAME_BORDER_OUT_BORDER__, out);
                controller.setProperty(border, __GO_UI_FRAME_BORDER_IN_BORDER__, in);

                break;
            }
            case EMPTY: {
                Double[] position = new Double[4];
                position[0] = Double.parseDouble(XmlTools.getFromMap(map, "top", "0"));
                position[1] = Double.parseDouble(XmlTools.getFromMap(map, "left", "0"));
                position[2] = Double.parseDouble(XmlTools.getFromMap(map, "right", "0"));
                position[3] = Double.parseDouble(XmlTools.getFromMap(map, "bottom", "0"));

                controller.setProperty(border, __GO_POSITION__, position);
                break;
            }
            case ETCHED: {
                BorderType etching = BorderType.stringToEnum(XmlTools.getFromMap(map, "type", "none"));
                String color = XmlTools.getFromMap(map, "color", "black");

                controller.setProperty(border, __GO_TYPE__, etching.ordinal());
                controller.setProperty(border, __GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__, color);
                controller.setProperty(border, __GO_UI_FRAME_BORDER_HIGHLIGHT_IN__, color);
                break;
            }
            case LINE: {
                String color = XmlTools.getFromMap(map, "color", "black");
                Integer thickness = Integer.parseInt(XmlTools.getFromMap(map, "thickness", "1"));
                Boolean rounded = Boolean.parseBoolean(XmlTools.getFromMap(map, "rounded", "false"));

                controller.setProperty(border, __GO_UI_FRAME_BORDER_COLOR__, color);
                controller.setProperty(border, __GO_LINE_THICKNESS__, thickness);
                controller.setProperty(border, __GO_UI_FRAME_BORDER_ROUNDED__, rounded);
                break;
            }
            case MATTE: {
                Double[] position = new Double[4];
                position[0] = Double.parseDouble(XmlTools.getFromMap(map, "top", "0"));
                position[1] = Double.parseDouble(XmlTools.getFromMap(map, "left", "0"));
                position[2] = Double.parseDouble(XmlTools.getFromMap(map, "right", "0"));
                position[3] = Double.parseDouble(XmlTools.getFromMap(map, "bottom", "0"));
                String color = XmlTools.getFromMap(map, "color", "black");

                controller.setProperty(border, __GO_POSITION__, position);
                controller.setProperty(border, __GO_UI_FRAME_BORDER_COLOR__, color);
                break;
            }
            case TITLED: {

                FrameBorderType borderOut = FrameBorderType.stringToEnum(XmlTools.getFromMap(map, "border", "none"));
                Integer out = createBorder(controller, border, borderOut, map);
                String title = XmlTools.getFromMap(map, "title", "");
                FrameBorder.JustificationType justify = FrameBorder.JustificationType.stringToEnum(XmlTools.getFromMap(map, "justify", "leading"));
                String fontName = XmlTools.getFromMap(map, "font-name", "");
                String fontAngle = XmlTools.getFromMap(map, "italic", "false") == "false" ? "normal" : "italic";
                Integer fontSize = Integer.parseInt(XmlTools.getFromMap(map, "font-size", "0"));
                String fontWeight = XmlTools.getFromMap(map, "bold", "false") == "false" ? "normal" : "bold";

                FrameBorder.TitlePositionType position = FrameBorder.TitlePositionType.stringToEnum(XmlTools.getFromMap(map, "position", "leading"));

                String color = XmlTools.getFromMap(map, "color", "black");

                controller.setProperty(border, __GO_UI_FRAME_BORDER_OUT_BORDER__, out);

                controller.setProperty(border, __GO_UI_FRAME_BORDER_TITLE__, out);
                controller.setProperty(border, __GO_TITLE__, title);
                controller.setProperty(border, __GO_UI_FRAME_BORDER_JUSTIFICATION__, justify.ordinal());
                controller.setProperty(border, __GO_UI_FONTNAME__, fontName);
                controller.setProperty(border, __GO_UI_FONTANGLE__, fontAngle);
                controller.setProperty(border, __GO_UI_FONTSIZE__, fontSize);
                controller.setProperty(border, __GO_UI_FONTWEIGHT__, fontWeight);
                controller.setProperty(border, __GO_UI_FRAME_BORDER_POSITION__, position.ordinal());
                controller.setProperty(border, __GO_UI_FRAME_BORDER_COLOR__, color);

                break;
            }
            case NONE:
            default:
                break;
        }

        return border;
    }

    private static Double[] getColor(String str) {
        try {
            Field field = Class.forName("java.awt.Color").getField(str);
            Color color = (Color) field.get(null);
            return new Double[] {(double) color.getRed() / 255, (double) color.getGreen() / 255, (double) color.getBlue() / 255};
        } catch (Exception e) {
            return new Double[] {(double) Color.black.getRed() / 255, (double) Color.black.getGreen() / 255, (double) Color.black.getBlue() / 255};
        }
    }
}
