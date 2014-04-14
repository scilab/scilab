package org.scilab.modules.graphic_objects.xmlloader;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_LAYOUT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_AXES__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_PARENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TYPE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UICONTEXTMENU__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UICONTROL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UIMENU__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LAYER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TAB__;

import java.io.File;
import java.util.Arrays;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.modules.commons.CommonFileUtils;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.console.Console;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicModel.GraphicModel;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.uicontrol.Uicontrol;
import org.scilab.modules.graphic_objects.uicontrol.Uicontrol.UicontrolStyle;
import org.scilab.modules.graphic_objects.uicontrol.frame.border.FrameBorder;
import org.scilab.modules.graphic_objects.uicontrol.frame.border.FrameBorderType;
import org.scilab.modules.graphic_objects.uimenu.Uimenu;
import org.scilab.modules.graphic_objects.utils.LayoutType;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;


public class XmlSaver {
    static private Figure defaultFig = null;
    static private FrameBorder defaultBorder = null;

    public static String save(int figure, String filename, boolean reverseChildren) {
        try {
            //init default values
            defaultFig = GraphicModel.getFigureModel();
            defaultBorder = new FrameBorder();

            DocumentBuilderFactory docFactory = DocumentBuilderFactory.newInstance();
            DocumentBuilder docBuilder = docFactory.newDocumentBuilder();

            //root elements
            Document doc = docBuilder.newDocument();
            //standalone="yes"
            doc.setXmlStandalone(true);

            //scilab header
            Element header = doc.createElement("scilabgui");
            setAttribute(header, "version", "1.0", "");
            setAttribute(header, "usedeprecatedskin", createAttribute(Console.getConsole().getUseDeprecatedLF()), "");

            //figure
            Element figureElement = createFigure(doc, figure, reverseChildren);
            appendChild(header, figureElement);
            appendChild(doc, header);

            //write the content into xml file
            TransformerFactory transformerFactory = TransformerFactory.newInstance();
            Transformer transformer = transformerFactory.newTransformer();
            transformer.setOutputProperty(OutputKeys.INDENT, "yes");
            transformer.setOutputProperty("{http://xml.apache.org/xslt}indent-amount", "4");
            doc.getDocumentElement().normalize();
            DOMSource source = new DOMSource(doc);


            //process filename
            File f = new File(filename);
            if (f.isAbsolute() == false) {
                String initialDirectoryPath = CommonFileUtils.getCWD();
                f = new File(initialDirectoryPath + File.separator + filename);
            }

            StreamResult result =  new StreamResult(f);
            transformer.transform(source, result);
        } catch (ParserConfigurationException pce) {
            return pce.getMessage();
        } catch (TransformerException tfe) {
            return tfe.getMessage();
        }

        defaultFig = null;
        defaultBorder = null;
        return ""; //all good
    }

    private static Element createFigure(Document doc, int figure, boolean reverseChildren) {
        GraphicController controller = GraphicController.getController();
        Figure fig = (Figure)controller.getObjectFromId(figure);
        Element elemFig = doc.createElement("figure");

        //figure_position
        setAttribute(elemFig, "figure_position", createAttribute(fig.getPosition()), createAttribute(defaultFig.getPosition()));
        //figure_size
        //setAttribute(elemFig, "figure_size", createAttribute(fig.getSize()), createAttribute(defaultFig.getSize()));
        //axes_size
        setAttribute(elemFig, "axes_size", createAttribute(fig.getAxesSize()), createAttribute(defaultFig.getAxesSize()));
        //auto_resize
        setAttribute(elemFig, "auto_resize", createAttribute(fig.getAutoResize()), createAttribute(defaultFig.getAutoResize()));
        //view_port
        setAttribute(elemFig, "view_port", createAttribute(fig.getViewport()), createAttribute(defaultFig.getViewport()));
        //figure_name
        setAttribute(elemFig, "figure_name", createAttribute(fig.getName()), createAttribute(defaultFig.getName()));
        //figure_id
        setAttribute(elemFig, "figure_id", createAttribute(fig.getId()), createAttribute(defaultFig.getId()));
        //info_message
        setAttribute(elemFig, "info_message", createAttribute(fig.getInfoMessage()), createAttribute(defaultFig.getInfoMessage()));
        //pixel_drawing_mode
        setAttribute(elemFig, "pixel_drawing_mode", createAttribute(fig.getPixelDrawingMode()), createAttribute(defaultFig.getPixelDrawingMode()));
        //anti_aliasing
        setAttribute(elemFig, "anti_aliasing", createAttribute(fig.getAntialiasing()), createAttribute(defaultFig.getAntialiasing()));
        //immediate_drawing
        setAttribute(elemFig, "immediate_drawing", createAttribute(fig.getImmediateDrawing()), createAttribute(defaultFig.getImmediateDrawing()));
        //background
        setAttribute(elemFig, "background", createAttribute(fig.getBackground()), createAttribute(defaultFig.getBackground()));
        //visible
        setAttribute(elemFig, "visible", createAttribute(fig.getVisible()), createAttribute(defaultFig.getVisible()));
        //rotation_style
        setAttribute(elemFig, "rotation_style", createAttribute(Figure.RotationType.enumToString(fig.getRotationAsEnum())), createAttribute(Figure.RotationType.enumToString(defaultFig.getRotationAsEnum())));
        //event_handler
        setAttribute(elemFig, "event_handler", createAttribute(fig.getEventHandlerString()), createAttribute(defaultFig.getEventHandlerString()));
        //event_handler_enable
        setAttribute(elemFig, "event_handler_enable", createAttribute(fig.getEventHandlerEnable()), createAttribute(defaultFig.getEventHandlerEnable()));
        //resizefcn
        setAttribute(elemFig, "resizefcn", createAttribute(fig.getResizeFcn()), createAttribute(defaultFig.getResizeFcn()));
        //closerequestfcn
        setAttribute(elemFig, "closerequestfcn", createAttribute(fig.getCloseRequestFcn()), createAttribute(defaultFig.getCloseRequestFcn()));
        //resize
        setAttribute(elemFig, "resize", createAttribute(fig.getResize()), createAttribute(defaultFig.getResize()));
        //toolbar
        setAttribute(elemFig, "toolbar", createAttribute(Figure.BarType.enumToString(fig.getToolbarAsEnum())), createAttribute(Figure.BarType.enumToString(defaultFig.getToolbarAsEnum())));
        //toolbar_visible
        setAttribute(elemFig, "toolbar_visible", createAttribute(fig.getToolbarVisible()), createAttribute(defaultFig.getToolbarVisible()));
        //menubar
        setAttribute(elemFig, "menubar", createAttribute(Figure.BarType.enumToString(fig.getMenubarAsEnum())), createAttribute(Figure.BarType.enumToString(defaultFig.getMenubarAsEnum())));
        //menubar_visible
        setAttribute(elemFig, "menubar_visible", createAttribute(fig.getMenubarVisible()), createAttribute(defaultFig.getMenubarVisible()));
        //infobar_visible
        setAttribute(elemFig, "infobar_visible", createAttribute(fig.getInfobarVisible()), createAttribute(defaultFig.getInfobarVisible()));
        //dockable
        setAttribute(elemFig, "dockable", createAttribute(fig.getDockable()), createAttribute(defaultFig.getDockable()));
        //layout
        setAttribute(elemFig, "layout", createAttribute(LayoutType.enumToString(fig.getLayoutAsEnum())), createAttribute(LayoutType.enumToString(defaultFig.getLayoutAsEnum())));
        //default_axes
        setAttribute(elemFig, "default_axes", createAttribute(fig.hasDefaultAxes()), createAttribute(defaultFig.hasDefaultAxes()));
        //icon
        setAttribute(elemFig, "icon", createAttribute(fig.getIcon()), createAttribute(defaultFig.getIcon()));
        //tag
        setAttribute(elemFig, "tag", createAttribute(fig.getTag()), createAttribute(defaultFig.getTag()));
        //layout_options
        if (fig.getLayoutAsEnum() == LayoutType.GRID) {
            setAttribute(elemFig, "grid_opt_grid", createAttribute(fig.getGridOptGrid()), createAttribute(defaultFig.getGridOptGrid()));
            setAttribute(elemFig, "grid_opt_padding", createAttribute(fig.getGridOptPadding()), createAttribute(defaultFig.getGridOptPadding()));
        } else if (fig.getLayoutAsEnum() == LayoutType.BORDER) {
            setAttribute(elemFig, "border_opt_padding", createAttribute(fig.getBorderOptPadding()), createAttribute(defaultFig.getBorderOptPadding()));
        }

        //children
        Integer[] children = fig.getChildren();
        for (int i = 0; i < children.length; i++) {
            appendChild(elemFig, createElement(doc, children[i], reverseChildren));
        }

        //colormap
        Double[] cm = fig.getColorMap().getData();
        Double[] modelcm = defaultFig.getColorMap().getData();

        if (Arrays.deepEquals(cm, modelcm) == false) {
            createColorMap(doc, elemFig, fig.getColorMap().getData());
        }

        return elemFig;
    }

    public static void appendChild(Node parent, Node child) {
        if (child != null) {
            parent.appendChild(child);
        }
    }

    private static Element createElement(Document doc, int id, boolean reverseChildren) {
        GraphicController controller = GraphicController.getController();
        Integer type = (Integer)controller.getProperty(id, __GO_TYPE__);

        switch (type) {
            case __GO_UICONTROL__ : {
                return createUicontrol(doc, id, reverseChildren);
            }
            case __GO_UIMENU__ : {
                //Uimenu uim = (Uimenu)controller.getObjectFromId(id);
                return createUimenu(doc, id, reverseChildren);
            }
            case __GO_UICONTEXTMENU__ : {
                //Uicontextmenu uicm = (Uicontextmenu)controller.getObjectFromId(id);
                return doc.createElement("contextmenu");
            }
            case __GO_AXES__ : {
                //Axes axes = (Axes)controller.getObjectFromId(id);
                return createAxes(doc, id, reverseChildren);
            }
        }

        return doc.createElement("fake");
    }

    private static Element createAxes(Document doc, Integer id, boolean reverseChildren) {
        GraphicController controller = GraphicController.getController();
        Axes axes = (Axes)controller.getObjectFromId(id);

        Element elemAxes = doc.createElement("axes");
        setAttribute(elemAxes, "tag", createAttribute(axes.getTag()), createAttribute(""));

        return elemAxes;
    }

    private static Element createBorder(Document doc, Integer id) {
        return createBorder(doc, id, "border");
    }

    private static Element createBorder(Document doc, Integer id, String borderName) {
        GraphicController controller = GraphicController.getController();
        FrameBorder border = (FrameBorder)controller.getObjectFromId(id);

        if (border == null || border.getStyleAsEnum() == FrameBorderType.NONE) {
            return null;
        }

        Element elemBorder = doc.createElement(borderName);

        setAttribute(elemBorder, "style", createAttribute(border.getStyleAsEnum().toString().toLowerCase()), "");

        switch (border.getStyleAsEnum()) {
            case BEVEL: {
                setAttribute(elemBorder, "type", createAttribute(border.getBorderType()), createAttribute(defaultBorder.getBorderType()));
                setAttribute(elemBorder, "hlout", createAttribute(border.getHlOut()), createAttribute(defaultBorder.getHlOut()));
                setAttribute(elemBorder, "hlin", createAttribute(border.getHlIn()), createAttribute(defaultBorder.getHlIn()));
                setAttribute(elemBorder, "shadowout", createAttribute(border.getShadowOut()), createAttribute(defaultBorder.getShadowOut()));
                setAttribute(elemBorder, "shadowin", createAttribute(border.getShadowIn()), createAttribute(defaultBorder.getShadowIn()));
                break;
            }
            case COMPOUND: {
                Element out = createBorder(doc, border.getOutBorder(), "out");
                if (out != null) {
                    appendChild(elemBorder, out);
                }

                Element in = createBorder(doc, border.getInBorder(), "in");
                if (in != null) {
                    appendChild(elemBorder, in);
                }
                break;
            }
            case EMPTY: {
                setAttribute(elemBorder, "position", createAttribute(border.getPosition()), createAttribute(defaultBorder.getPosition()));
                break;
            }
            case ETCHED: {
                setAttribute(elemBorder, "type", createAttribute(border.getBorderType()), createAttribute(defaultBorder.getBorderType()));
                setAttribute(elemBorder, "hl", createAttribute(border.getHlOut()), createAttribute(defaultBorder.getHlOut()));
                setAttribute(elemBorder, "shadow", createAttribute(border.getShadowOut()), createAttribute(defaultBorder.getShadowOut()));
                break;
            }
            case LINE: {
                setAttribute(elemBorder, "color", createAttribute(border.getColor()), createAttribute(defaultBorder.getColor()));
                setAttribute(elemBorder, "thickness", createAttribute(border.getThickness()), createAttribute(defaultBorder.getThickness()));
                setAttribute(elemBorder, "rounded", createAttribute(border.getRounded()), createAttribute(defaultBorder.getRounded()));
                break;
            }
            case MATTE: {
                setAttribute(elemBorder, "position", createAttribute(border.getPosition()), createAttribute(defaultBorder.getPosition()));
                setAttribute(elemBorder, "color", createAttribute(border.getColor()), createAttribute(defaultBorder.getColor()));
                break;
            }
            case SOFTBEVEL: {
                setAttribute(elemBorder, "type", createAttribute(border.getBorderType()), createAttribute(defaultBorder.getBorderType()));
                setAttribute(elemBorder, "hlout", createAttribute(border.getHlOut()), createAttribute(defaultBorder.getHlOut()));
                setAttribute(elemBorder, "hlin", createAttribute(border.getHlIn()), createAttribute(defaultBorder.getHlIn()));
                setAttribute(elemBorder, "shadowout", createAttribute(border.getShadowOut()), createAttribute(defaultBorder.getShadowOut()));
                setAttribute(elemBorder, "shadowin", createAttribute(border.getShadowIn()), createAttribute(defaultBorder.getShadowIn()));
                break;
            }
            case TITLED: {
                setAttribute(elemBorder, "title", createAttribute(border.getTitle()), createAttribute(defaultBorder.getTitle()));
                setAttribute(elemBorder, "justification", createAttribute(border.getJustification()), createAttribute(defaultBorder.getJustification()));
                setAttribute(elemBorder, "titleposition", createAttribute(border.getTitlePosition()), createAttribute(defaultBorder.getTitlePosition()));
                setAttribute(elemBorder, "fontname", createAttribute(border.getFontName()), createAttribute(defaultBorder.getFontName()));
                setAttribute(elemBorder, "fontangle", createAttribute(border.getFontAngle()), createAttribute(defaultBorder.getFontAngle()));
                setAttribute(elemBorder, "fontsize", createAttribute(border.getFontSize()), createAttribute(defaultBorder.getFontSize()));
                setAttribute(elemBorder, "fontweight", createAttribute(border.getFontWeight()), createAttribute(defaultBorder.getFontWeight()));
                setAttribute(elemBorder, "color", createAttribute(border.getColor()), createAttribute(defaultBorder.getColor()));
                Element title = createBorder(doc, border.getTitleBorder(), "title");
                if (title != null) {
                    appendChild(elemBorder, title);
                }
                break;
            }
            case NONE:
            default: {
                break;
            }
        }
        return elemBorder;
    }

    private static Element createUicontrol(Document doc, Integer id, boolean reverseChildren) {
        GraphicController controller = GraphicController.getController();
        Uicontrol uic = (Uicontrol)controller.getObjectFromId(id);
        Uicontrol defaultUi = initDefaultui(uic.getStyle());
        Element elemUi = doc.createElement(uic.getStyleAsEnum().toString().toLowerCase());

        //BackgroundColor
        setAttribute(elemUi, "backgroundcolor", createAttribute(uic.getBackgroundColor()), createAttribute(defaultUi.getBackgroundColor()));
        //Enable
        setAttribute(elemUi, "enable", createAttribute(uic.getEnable()), createAttribute(defaultUi.getEnable()));
        //FontAngle
        setAttribute(elemUi, "fontangle", createAttribute(uic.getFontAngle()), createAttribute(defaultUi.getFontAngle()));
        //FontName
        setAttribute(elemUi, "fontname", createAttribute(uic.getFontName()), createAttribute(defaultUi.getFontName()));
        //FontSize
        setAttribute(elemUi, "fontsize", createAttribute(uic.getFontSize()), createAttribute(defaultUi.getFontSize()));
        //FontUnits
        setAttribute(elemUi, "fontunits", createAttribute(uic.getFontUnits()), createAttribute(defaultUi.getFontUnits()));
        //FontWeight
        setAttribute(elemUi, "fontweight", createAttribute(uic.getFontWeight()), createAttribute(defaultUi.getFontWeight()));
        //ForegroundColor
        setAttribute(elemUi, "foregroundcolor", createAttribute(uic.getForegroundColor()), createAttribute(defaultUi.getForegroundColor()));
        //HorizontalAlignment
        setAttribute(elemUi, "horizontalalignment", createAttribute(uic.getHorizontalAlignment()), createAttribute(defaultUi.getHorizontalAlignment()));
        //ListboxTop
        setAttribute(elemUi, "listboxtop", createAttribute(uic.getListboxTop()), createAttribute(defaultUi.getListboxTop()));
        //Max
        setAttribute(elemUi, "max", createAttribute(uic.getMax()), createAttribute(defaultUi.getMax()));
        //Min
        setAttribute(elemUi, "min", createAttribute(uic.getMin()), createAttribute(defaultUi.getMin()));
        //Relief
        setAttribute(elemUi, "relief", createAttribute(uic.getRelief()), createAttribute(defaultUi.getRelief()));
        //SliderStep
        setAttribute(elemUi, "sliderstep", createAttribute(uic.getSliderStep()), createAttribute(defaultUi.getSliderStep()));
        //String
        //do not save string and value of tab/layer to avoid setting bad index during loading
        if (uic.getStyle() != __GO_UI_TAB__ && uic.getStyle() != __GO_UI_LAYER__) {
            String[] uicSstr = uic.getString();
            String[] modelStr = defaultUi.getString();
            if (Arrays.deepEquals(uicSstr, modelStr) == false) {
                createStringArray(doc, elemUi, "string", uicSstr, uic.getStringColNb());
            }
        }
        //TooltipString
        String[] uicToolTip = uic.getTooltipString();
        String[] modelToolTip = defaultUi.getTooltipString();
        if (Arrays.deepEquals(uicToolTip, modelToolTip) == false) {
            createStringArray(doc, elemUi, "tooltipstring", uicToolTip, 1);
        }
        //Units
        setAttribute(elemUi, "units", createAttribute(uic.getUnits()), createAttribute(defaultUi.getUnits()));
        //Value
        //do not save string and value of tab/layer to avoid setting bad index during loading
        if (uic.getStyle() != __GO_UI_TAB__ && uic.getStyle() != __GO_UI_LAYER__) {
            setAttribute(elemUi, "value", createAttribute(uic.getUiValue()), createAttribute(defaultUi.getUiValue()));
        }
        //VerticalAlignment
        setAttribute(elemUi, "verticalalignment", createAttribute(uic.getVerticalAlignment()), createAttribute(defaultUi.getVerticalAlignment()));
        //Visible
        setAttribute(elemUi, "visible", createAttribute(uic.getVisible()), createAttribute(!defaultUi.getVisible()));
        //Callback
        setAttribute(elemUi, "callback", createAttribute(uic.getCallbackString()), createAttribute(defaultUi.getCallbackString()));
        //Callback_Type
        setAttribute(elemUi, "callback_type", createAttribute(uic.getCallbackType()), createAttribute(defaultUi.getCallbackType()));

        //Constraints
        Integer parent = (Integer)controller.getProperty(uic.getIdentifier(), __GO_PARENT__);
        Integer iLayout = (Integer)controller.getProperty(parent, __GO_LAYOUT__);
        LayoutType layout = LayoutType.intToEnum(iLayout);

        if (layout == LayoutType.GRIDBAG) {
            setAttribute(elemUi, "gridbaggrid", createAttribute(uic.getGridBagGrid()), createAttribute(defaultUi.getGridBagGrid()));
            setAttribute(elemUi, "gridbagweight", createAttribute(uic.getGridBagWeight()), createAttribute(defaultUi.getGridBagWeight()));
            setAttribute(elemUi, "gridbagfill", createAttribute(Uicontrol.FillType.enumToString(uic.getGridBagFillAsEnum())), createAttribute(Uicontrol.FillType.enumToString(defaultUi.getGridBagFillAsEnum())));
            setAttribute(elemUi, "gridbaganchor", createAttribute(Uicontrol.AnchorType.enumToString(uic.getGridBagAnchorAsEnum())), createAttribute(Uicontrol.AnchorType.enumToString(defaultUi.getGridBagAnchorAsEnum())));
            setAttribute(elemUi, "gridbagpadding", createAttribute(uic.getGridBagPadding()), createAttribute(defaultUi.getGridBagPadding()));
            setAttribute(elemUi, "gridbagpreferredsize", createAttribute(uic.getGridBagPreferredSize()), createAttribute(defaultUi.getGridBagPreferredSize()));
        } else if (layout == LayoutType.BORDER) {
            setAttribute(elemUi, "borderposition", createAttribute(Uicontrol.BorderLayoutType.enumToString(uic.getBorderPositionAsEnum())), createAttribute(Uicontrol.BorderLayoutType.enumToString(defaultUi.getBorderPositionAsEnum())));
            setAttribute(elemUi, "borderpreferredsize", createAttribute(uic.getBorderPreferredSize()), createAttribute(defaultUi.getBorderPreferredSize()));
        } else if (layout == LayoutType.NONE) {
            //Position
            setAttribute(elemUi, "position", createAttribute(uic.getUiPosition()), createAttribute(defaultUi.getUiPosition()));
        }

        //Margins
        setAttribute(elemUi, "margins", createAttribute(uic.getMargins()), createAttribute(defaultUi.getMargins()));
        //Groupname
        setAttribute(elemUi, "groupname", createAttribute(uic.getGroupName()), createAttribute(defaultUi.getGroupName()));

        //icon
        setAttribute(elemUi, "icon", createAttribute(uic.getIcon()), createAttribute(defaultUi.getIcon()));
        //Tag
        setAttribute(elemUi, "tag", createAttribute(uic.getTag()), createAttribute(defaultUi.getTag()));

        //only for frame type
        if (uic.getStyleAsEnum() == UicontrolStyle.FRAME) {
            //Border
            Integer iBorder = uic.getFrameBorder();
            Element border = createBorder(doc, iBorder);
            if (border != null) {
                appendChild(elemUi, createBorder(doc, iBorder));
            }

            //Layout
            setAttribute(elemUi, "layout", createAttribute(LayoutType.enumToString(uic.getLayoutAsEnum())), createAttribute(LayoutType.enumToString(defaultUi.getLayoutAsEnum())));
            //Layout_options
            if (uic.getLayoutAsEnum() == LayoutType.GRID) {
                setAttribute(elemUi, "gridoptgrid", createAttribute(uic.getGridOptGrid()), createAttribute(defaultUi.getGridOptGrid()));
                setAttribute(elemUi, "gridoptpadding", createAttribute(uic.getGridOptPadding()), createAttribute(defaultUi.getGridOptPadding()));
            } else if (uic.getLayoutAsEnum() == LayoutType.BORDER) {
                setAttribute(elemUi, "borderoptpadding", createAttribute(uic.getBorderOptPadding()), createAttribute(defaultUi.getBorderOptPadding()));
            }
        }

        if (uic.getStyleAsEnum() == UicontrolStyle.FRAME || uic.getStyleAsEnum() == UicontrolStyle.EDIT) {
            //Scrollable
            setAttribute(elemUi, "scrollable", createAttribute(uic.getScrollable()), createAttribute(defaultUi.getScrollable()));
        }

        controller.deleteObject(defaultUi.getIdentifier());
        //children
        Integer[] children = uic.getChildren();
        if (reverseChildren) {
            for (int i = children.length - 1 ; i >= 0 ; i--) {
                if ((Integer)controller.getProperty(children[i], __GO_TYPE__) != __GO_UI_FRAME_BORDER__) {
                    appendChild(elemUi, createElement(doc, children[i], reverseChildren));
                }
            }
        } else {
            for (int i = 0 ; i < children.length ; i++) {
                if ((Integer)controller.getProperty(children[i], __GO_TYPE__) != __GO_UI_FRAME_BORDER__) {
                    appendChild(elemUi, createElement(doc, children[i], reverseChildren));
                }
            }
        }
        return elemUi;
    }

    private static Element createUimenu(Document doc, Integer id, boolean reverseChildren) {
        GraphicController controller = GraphicController.getController();
        Uimenu uim = (Uimenu)controller.getObjectFromId(id);
        //bypass hidden menu
        if (uim.isHidden()) {
            return null;
        }

        Uimenu defaultMenu = initDefaultMenu();
        Element elemMenu = doc.createElement("menu");

        //enable
        setAttribute(elemMenu, "enable", createAttribute(uim.getEnable()), createAttribute(defaultMenu.getEnable()));
        //foregroundcolor
        setAttribute(elemMenu, "foregroundcolor", createAttribute(uim.getForegroundColor()), createAttribute(defaultMenu.getForegroundColor()));
        //label
        setAttribute(elemMenu, "label", createAttribute(uim.getLabel()), createAttribute(defaultMenu.getLabel()));
        //handle_visible
        setAttribute(elemMenu, "hidden", createAttribute(uim.isHidden()), createAttribute(defaultMenu.isHidden()));
        //visible
        setAttribute(elemMenu, "visible", createAttribute(uim.getVisible()), createAttribute(defaultMenu.getVisible()));
        //callback
        setAttribute(elemMenu, "callback", createAttribute(uim.getCallbackString()), createAttribute(defaultMenu.getCallbackString()));
        //callback_type
        setAttribute(elemMenu, "callback_type", createAttribute(uim.getCallbackType()), createAttribute(defaultMenu.getCallbackType()));
        //checked
        setAttribute(elemMenu, "checked", createAttribute(uim.getChecked()), createAttribute(defaultMenu.getChecked()));
        //icon
        setAttribute(elemMenu, "icon", createAttribute(uim.getIcon()), createAttribute(defaultMenu.getIcon()));
        //tag
        setAttribute(elemMenu, "tag", createAttribute(uim.getTag()), createAttribute(defaultMenu.getTag()));

        //children
        Integer[] children = uim.getChildren();
        for (int i = 0; i < children.length; i++) {
            appendChild(elemMenu, createElement(doc, children[i], reverseChildren));
        }

        return elemMenu;
    }

    private static String createAttribute(Boolean val) {
        if (val == null) {
            return "off";
        }

        if (val) {
            return "on";
        } else {
            return "off";
        }
    }

    private static String createAttribute(Integer val) {
        if (val == null) {
            return null;
        }

        StringBuilder builder = new StringBuilder();
        builder.append(val.intValue());
        return builder.toString();
    }

    private static String createAttribute(String val) {
        if (val == null) {
            return null;
        }
        return val;
    }

    private static String createAttribute(Double val) {
        if (val == null) {
            return null;
        }

        StringBuilder builder = new StringBuilder();
        builder.append(val.doubleValue());
        return builder.toString();
    }

    //    private static String createAttribute(Boolean[] val) {
    //        if(val == null || val.length == 0) {
    //            return null;
    //        }
    //
    //        StringBuilder str = new StringBuilder();
    //
    //        str.append(createAttribute(val[0]));
    //
    //        for(int i = 1 ; i < val.length; i++)
    //        {
    //            str.append(",");
    //            str.append(createAttribute(val[i]));
    //        }
    //
    //        return str.toString();
    //    }


    private static String createAttribute(String[] val) {
        if (val == null || val.length == 0) {
            return null;
        }

        StringBuilder str = new StringBuilder();

        str.append(createAttribute(val[0]));

        for (int i = 1 ; i < val.length; i++) {
            str.append(",");
            str.append(createAttribute(val[i]));
        }

        return str.toString();
    }

    private static String createAttribute(Double[] val) {
        if (val == null || val.length == 0) {
            return null;
        }

        StringBuilder str = new StringBuilder();

        str.append(createAttribute(val[0]));

        for (int i = 1 ; i < val.length; i++) {
            str.append(",");
            str.append(createAttribute(val[i]));
        }

        return str.toString();
    }

    private static String createAttribute(Integer[] val) {
        if (val == null || val.length == 0) {
            return null;
        }

        StringBuilder str = new StringBuilder();

        str.append(createAttribute(val[0]));

        for (int i = 1 ; i < val.length; i++) {
            str.append(",");
            str.append(createAttribute(val[i]));
        }

        return str.toString();
    }

    private static void setAttribute(Element elem, String property, String value, String ref) {
        if (value == null || property == null || elem == null || (ref != null && value.equals(ref))) {
            return;
        }

        elem.setAttribute(property, value);
    }

    private static Uicontrol initDefaultui(Integer uicontrolStyle) {
        Integer uic = GraphicController.getController().askObject(GraphicObject.getTypeFromName(uicontrolStyle));
        return (Uicontrol) GraphicController.getController().getObjectFromId(uic);
    }

    private static Uimenu initDefaultMenu() {
        Integer uic = GraphicController.getController().askObject(GraphicObject.getTypeFromName(__GO_UIMENU__));
        return (Uimenu) GraphicController.getController().getObjectFromId(uic);
    }

    private static void createStringArray(Document doc, Element parent, String property, String[] value, Integer cols) {
        //create a new Node to store string
        Element elemString = doc.createElement("string");
        Integer rows = value.length / cols;

        setAttribute(elemString, "property", property, "");
        setAttribute(elemString, "rows", rows.toString(), "");
        setAttribute(elemString, "cols", cols.toString(), "");

        for (int i = 0 ; i < rows * cols ; i++) {
            Element elemSub = doc.createElement("stringitem");
            setAttribute(elemSub, "value", value[i], null);
            elemString.appendChild(elemSub);
        }

        parent.appendChild(elemString);
    }

    private static void createColorMap(Document doc, Element parent, Double[] value) {
        //create a new Node to store string
        Element elemString = doc.createElement("colormap");
        Integer rows = value.length / 3;

        setAttribute(elemString, "size", rows.toString(), "");

        for (int i = 0 ; i < rows ; i++) {
            Element elemSub = doc.createElement("colormapitem");
            Color c = new Color(value[i].floatValue(), value[i + rows].floatValue(), value[i + 2 * rows].floatValue());
            Integer color = c.getRGB();
            setAttribute(elemSub, "color", color.toString(), null);
            elemString.appendChild(elemSub);
        }

        parent.appendChild(elemString);
    }

}
