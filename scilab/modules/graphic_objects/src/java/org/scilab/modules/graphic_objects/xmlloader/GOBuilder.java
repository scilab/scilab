package org.scilab.modules.graphic_objects.xmlloader;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_BORDER_OPT_PADDING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DOCKABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_GRID_OPT_GRID__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_GRID_OPT_PADDING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_INFOBAR_VISIBLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_LAYOUT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_MENUBAR_VISIBLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_NAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_RESIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TAG__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TOOLBAR_VISIBLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_BORDER_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_VISIBLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_GRIDBAG_GRID__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_GRIDBAG_WEIGHT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_GRIDBAG_FILL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_GRIDBAG_ANCHOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_GRIDBAG_PADDING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_MARGINS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_STYLE__;


import java.util.Map;

import org.scilab.modules.graphic_objects.builder.Builder;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.uicontrol.Uicontrol;
import org.scilab.modules.graphic_objects.uicontrol.Uicontrol.AnchorType;
import org.scilab.modules.graphic_objects.uicontrol.uitext.UiText;
import org.scilab.modules.graphic_objects.utils.LayoutType;
import org.xml.sax.Attributes;

public class GOBuilder {
    public static Integer figureBuilder(GraphicController controller, Attributes attributes) {
        Integer fig = Builder.createNewFigureWithAxes();
        String item = null;

        // hide toolbar
        controller.setProperty(fig, __GO_TOOLBAR_VISIBLE__, false);

        // hide menubar
        controller.setProperty(fig, __GO_MENUBAR_VISIBLE__, false);

        // hide infobar
        controller.setProperty(fig, __GO_INFOBAR_VISIBLE__, false);

        // remove dock
        controller.setProperty(fig, __GO_DOCKABLE__, false);

        // id
        XmlTools.setPropAsString(fig, __GO_TAG__, attributes.getValue("id"));

        // visible
        XmlTools.setPropAsBoolean(fig, __GO_VISIBLE__,
                                  attributes.getValue("visible"));

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
        setLayoutProperty(controller, fig, attributes.getValue("layout"));

        return fig;
    }

    public static Integer uicontrolBuilder(GraphicController controller, int type, Attributes attributes, int parent) {
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
            //Common attributes

            //id
            XmlTools.setPropAsString(uic, __GO_TAG__, attributes.getValue("id"));


            // layout and layout_options
            item = attributes.getValue("layout");
            if (item == null && fromModel != null) {
                item = fromModel.get("layout");
            }
            setLayoutProperty(controller, uic, item);

            //constraints

            //get parent layout
            LayoutType layout = LayoutType.NONE;
            if (parent != 0) {
                layout = LayoutType.intToEnum((Integer)controller.getProperty(parent, __GO_LAYOUT__));
            }

            if (layout != LayoutType.NONE) {
                item = attributes.getValue("constraint");
                if (item == null && fromModel != null) {
                    //use value from model if property is not in "declaration"
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
                        Integer[] grid = new Integer[] {0, 0, 0, 0};
                        grid[0] = Integer.parseInt(XmlTools.getFromMap(map, "gridx", "0"));
                        grid[1] = Integer.parseInt(XmlTools.getFromMap(map, "gridy", "0"));
                        grid[2] = Integer.parseInt(XmlTools.getFromMap(map, "gridwidth", "0"));
                        grid[3] = Integer.parseInt(XmlTools.getFromMap(map, "gridheight", "0"));

                        Double[] weight = new Double[] {0.0, 0.0};
                        weight[0] = Double.parseDouble(XmlTools.getFromMap(map, "weightx", "0.0"));
                        weight[1] = Double.parseDouble(XmlTools.getFromMap(map, "weighty", "0.0"));

                        Integer fill = Uicontrol.FillType.stringToEnum(XmlTools.getFromMap(map, "fill", "none")).ordinal();
                        Integer anchor = Uicontrol.AnchorType.stringToEnum(XmlTools.getFromMap(map, "anchor", "center")).ordinal();

                        Integer[] padding = new Integer[] {0, 0};
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

                //inserts -> marging
                String insets = XmlTools.getFromMap(map, "insets", "0,0,0,0");
                Double[] margins = new Double[] {0.0, 0.0, 0.0, 0.0};
                String[] insert = insets.split(",");
                for (int i = 0; i < insert.length && i < 4; i++) {
                    margins[i] = Double.parseDouble(insert[i]);
                }

                controller.setProperty(uic, __GO_MARGINS__, margins);
            }


            //specific attributes
            Integer type = (Integer)controller.getProperty(uic, __GO_STYLE__);
            switch (type) {
            }

        } catch (CSSParserException e) {
            e.printStackTrace();
        }

        return uic;
    }

    private static void setLayoutProperty(GraphicController controller, Integer uid, String item) {
        try {
            if (item != null) {
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
            }
        } catch (CSSParserException e) {
            e.printStackTrace();
        }

    }
}
