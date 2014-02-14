package org.scilab.modules.graphic_objects.xmlloader;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_AXES__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_LAYOUT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_PARENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TYPE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UICONTEXTMENU__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UICONTROL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UIMENU__;

import java.io.File;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.scilab.modules.graphic_objects.console.Console;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicModel.GraphicModel;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.uicontrol.Uicontrol;
import org.scilab.modules.graphic_objects.uicontrol.Uicontrol.UicontrolStyle;
import org.scilab.modules.graphic_objects.uicontrol.frame.border.FrameBorder;
import org.scilab.modules.graphic_objects.uicontrol.frame.border.FrameBorderType;
import org.scilab.modules.graphic_objects.utils.LayoutType;
import org.w3c.dom.Document;
import org.w3c.dom.Element;


public class XmlSaver {
    static private Uicontrol defaultUi = null;
    static private Figure defaultFig = null;
    static private FrameBorder defaultBorder = null;

    public static String save(int figure, String filename) {
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
            Element figureElement = createFigure(doc, figure);
            header.appendChild(figureElement);
            doc.appendChild(header);

            //write the content into xml file
            TransformerFactory transformerFactory = TransformerFactory.newInstance();
            Transformer transformer = transformerFactory.newTransformer();
            transformer.setOutputProperty(OutputKeys.INDENT, "yes");
            transformer.setOutputProperty("{http://xml.apache.org/xslt}indent-amount", "4");
            doc.getDocumentElement().normalize();
            DOMSource source = new DOMSource(doc);

            StreamResult result =  new StreamResult(new File(filename));
            transformer.transform(source, result);
        } catch (ParserConfigurationException pce) {
            return pce.getMessage();
        } catch (TransformerException tfe) {
            return tfe.getMessage();
        }

        defaultFig = null;
        defaultUi = null;
        defaultBorder = null;
        return ""; //all good
    }

    private static Element createFigure(Document doc, int figure) {
        GraphicController controller = GraphicController.getController();
        Figure fig = (Figure)controller.getObjectFromId(figure);
        Element elemFig = doc.createElement("figure");

        //figure_position
        setAttribute(elemFig, "figure_position", createAttribute(fig.getPosition()), createAttribute(defaultFig.getPosition()));
        //figure_size
        setAttribute(elemFig, "figure_size", createAttribute(fig.getSize()), createAttribute(defaultFig.getSize()));
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
        setAttribute(elemFig, "rotation_style", createAttribute(fig.getRotation()), createAttribute(defaultFig.getRotation()));
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
        setAttribute(elemFig, "toolbar", createAttribute(fig.getToolbar()), createAttribute(defaultFig.getToolbar()));
        //toolbar_visible
        setAttribute(elemFig, "toolbar_visible", createAttribute(fig.getToolbarVisible()), createAttribute(defaultFig.getToolbarVisible()));
        //menubar
        setAttribute(elemFig, "menubar", createAttribute(fig.getMenubar()), createAttribute(defaultFig.getMenubar()));
        //menubar_visible
        setAttribute(elemFig, "menubar_visible", createAttribute(fig.getMenubarVisible()), createAttribute(defaultFig.getMenubarVisible()));
        //infobar_visible
        setAttribute(elemFig, "infobar_visible", createAttribute(fig.getInfobarVisible()), createAttribute(defaultFig.getInfobarVisible()));
        //dockable
        setAttribute(elemFig, "dockable", createAttribute(fig.getDockable()), createAttribute(defaultFig.getDockable()));
        //layout
        setAttribute(elemFig, "layout", createAttribute(fig.getLayout()), createAttribute(defaultFig.getLayout()));

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
        for (int i = 0 ; i < children.length ; i++) {
            elemFig.appendChild(createElement(doc, children[i]));
        }

        return elemFig;
    }

    private static Element createElement(Document doc, int id) {
        GraphicController controller = GraphicController.getController();
        Integer type = (Integer)controller.getProperty(id, __GO_TYPE__);

        switch (type) {
            case __GO_UICONTROL__ : {
                return createUicontrol(doc, id);
            }
            case __GO_UIMENU__ : {
                //Uimenu uim = (Uimenu)controller.getObjectFromId(id);
                return doc.createElement("menu");
            }
            case __GO_UICONTEXTMENU__ : {
                //Uicontextmenu uicm = (Uicontextmenu)controller.getObjectFromId(id);
                return doc.createElement("contextmenu");
            }
            case __GO_AXES__ : {
                //Axes axes = (Axes)controller.getObjectFromId(id);
                return doc.createElement("axes");
            }
        }

        return doc.createElement("fake");
    }

    private static Element createBorders(Document doc, Integer id) {
        return createBorders(doc, id, "borders");
    }

    private static Element createBorders(Document doc, Integer id, String borderName) {
        GraphicController controller = GraphicController.getController();
        FrameBorder border = (FrameBorder)controller.getObjectFromId(id);

        if (border == null || border.getStyleAsEnum() == FrameBorderType.NONE) {
            return null;
        }

        Element elemBorders = doc.createElement(borderName);

        setAttribute(elemBorders, "style", createAttribute(border.getStyleAsEnum().toString().toLowerCase()), "");

        switch (border.getStyleAsEnum()) {
            case BEVEL: {
                setAttribute(elemBorders, "type", createAttribute(border.getBorderType()), createAttribute(defaultBorder.getBorderType()));
                setAttribute(elemBorders, "hlout", createAttribute(border.getHlOut()), createAttribute(defaultBorder.getHlOut()));
                setAttribute(elemBorders, "hlin", createAttribute(border.getHlIn()), createAttribute(defaultBorder.getHlIn()));
                setAttribute(elemBorders, "shadowout", createAttribute(border.getShadowOut()), createAttribute(defaultBorder.getShadowOut()));
                setAttribute(elemBorders, "shadowin", createAttribute(border.getShadowIn()), createAttribute(defaultBorder.getShadowIn()));
                break;
            }
            case COMPOUND: {
                Element out = createBorders(doc, border.getOutBorder(), "out");
                if (out != null) {
                    elemBorders.appendChild(out);
                }

                Element in = createBorders(doc, border.getInBorder(), "in");
                if (in != null) {
                    elemBorders.appendChild(in);
                }
                break;
            }
            case EMPTY: {
                setAttribute(elemBorders, "position", createAttribute(border.getPosition()), createAttribute(defaultBorder.getPosition()));
                break;
            }
            case ETCHED: {
                setAttribute(elemBorders, "type", createAttribute(border.getBorderType()), createAttribute(defaultBorder.getBorderType()));
                setAttribute(elemBorders, "hl", createAttribute(border.getHlOut()), createAttribute(defaultBorder.getHlOut()));
                setAttribute(elemBorders, "shadow", createAttribute(border.getShadowOut()), createAttribute(defaultBorder.getShadowOut()));
                break;
            }
            case LINE: {
                setAttribute(elemBorders, "color", createAttribute(border.getColor()), createAttribute(defaultBorder.getColor()));
                setAttribute(elemBorders, "thickness", createAttribute(border.getThickness()), createAttribute(defaultBorder.getThickness()));
                setAttribute(elemBorders, "rounded", createAttribute(border.getRounded()), createAttribute(defaultBorder.getRounded()));
                break;
            }
            case MATTE: {
                setAttribute(elemBorders, "position", createAttribute(border.getPosition()), createAttribute(defaultBorder.getPosition()));
                setAttribute(elemBorders, "color", createAttribute(border.getColor()), createAttribute(defaultBorder.getColor()));
                break;
            }
            case SOFTBEVEL: {
                setAttribute(elemBorders, "type", createAttribute(border.getBorderType()), createAttribute(defaultBorder.getBorderType()));
                setAttribute(elemBorders, "hlout", createAttribute(border.getHlOut()), createAttribute(defaultBorder.getHlOut()));
                setAttribute(elemBorders, "hlin", createAttribute(border.getHlIn()), createAttribute(defaultBorder.getHlIn()));
                setAttribute(elemBorders, "shadowout", createAttribute(border.getShadowOut()), createAttribute(defaultBorder.getShadowOut()));
                setAttribute(elemBorders, "shadowin", createAttribute(border.getShadowIn()), createAttribute(defaultBorder.getShadowIn()));
                break;
            }
            case TITLED: {
                setAttribute(elemBorders, "title", createAttribute(border.getTitle()), createAttribute(defaultBorder.getTitle()));
                setAttribute(elemBorders, "justification", createAttribute(border.getJustification()), createAttribute(defaultBorder.getJustification()));
                setAttribute(elemBorders, "titleposition", createAttribute(border.getTitlePosition()), createAttribute(defaultBorder.getTitlePosition()));
                setAttribute(elemBorders, "fontname", createAttribute(border.getFontName()), createAttribute(defaultBorder.getFontName()));
                setAttribute(elemBorders, "fontangle", createAttribute(border.getFontAngle()), createAttribute(defaultBorder.getFontAngle()));
                setAttribute(elemBorders, "fontsize", createAttribute(border.getFontSize()), createAttribute(defaultBorder.getFontSize()));
                setAttribute(elemBorders, "fontweight", createAttribute(border.getFontWeight()), createAttribute(defaultBorder.getFontWeight()));
                setAttribute(elemBorders, "color", createAttribute(border.getColor()), createAttribute(defaultBorder.getColor()));
                Element title = createBorders(doc, border.getTitleBorder(), "title");
                if (title != null) {
                    elemBorders.appendChild(title);
                }
                break;
            }
            case NONE:
            default: {
                break;
            }
        }
        return elemBorders;
    }

    private static Element createUicontrol(Document doc, Integer id) {
        GraphicController controller = GraphicController.getController();
        Uicontrol uic = (Uicontrol)controller.getObjectFromId(id);
        initDefaultui(uic.getStyle());
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
        setAttribute(elemUi, "string", createAttribute(uic.getString()), createAttribute(defaultUi.getString()));
        //TooltipString
        setAttribute(elemUi, "tooltipstring", createAttribute(uic.getTooltipString()), createAttribute(defaultUi.getTooltipString()));
        //Units
        setAttribute(elemUi, "units", createAttribute(uic.getUnits()), createAttribute(defaultUi.getUnits()));
        //Value
        setAttribute(elemUi, "value", createAttribute(uic.getUiValue()), createAttribute(defaultUi.getUiValue()));
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
            setAttribute(elemUi, "gridbagfill", createAttribute(uic.getGridBagFill()), createAttribute(defaultUi.getGridBagFill()));
            setAttribute(elemUi, "gridbaganchor", createAttribute(uic.getGridBagAnchor()), createAttribute(defaultUi.getGridBagAnchor()));
            setAttribute(elemUi, "gridbagpadding", createAttribute(uic.getGridBagPadding()), createAttribute(defaultUi.getGridBagPadding()));
            setAttribute(elemUi, "gridbagpreferredsize", createAttribute(uic.getGridBagPreferredSize()), createAttribute(defaultUi.getGridBagPreferredSize()));
        } else if (layout == LayoutType.BORDER) {
            setAttribute(elemUi, "borderposition", createAttribute(uic.getBorderPosition()), createAttribute(defaultUi.getBorderPosition()));
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
            //Borders
            Integer iBorders = uic.getFrameBorder();
            Element borders = createBorders(doc, iBorders);
            if (borders != null) {
                elemUi.appendChild(createBorders(doc, iBorders));
            }

            //Scrollable
            setAttribute(elemUi, "scrollable", createAttribute(uic.getScrollable()), createAttribute(defaultUi.getScrollable()));

            //Layout
            setAttribute(elemUi, "layout", createAttribute(uic.getLayout()), createAttribute(defaultUi.getLayout()));
            //Layout_options
            if (uic.getLayoutAsEnum() == LayoutType.GRID) {
                setAttribute(elemUi, "gridoptgrid", createAttribute(uic.getGridOptGrid()), createAttribute(defaultUi.getGridOptGrid()));
                setAttribute(elemUi, "gridoptpadding", createAttribute(uic.getGridOptPadding()), createAttribute(defaultUi.getGridOptPadding()));
            } else if (uic.getLayoutAsEnum() == LayoutType.BORDER) {
                setAttribute(elemUi, "borderoptpadding", createAttribute(uic.getBorderOptPadding()), createAttribute(defaultUi.getBorderOptPadding()));
            }
        }

        //children
        Integer[] children = uic.getChildren();
        for (int i = 0 ; i < children.length ; i++) {
            elemUi.appendChild(createElement(doc, children[i]));
        }

        return elemUi;
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
    //        StringBuilder onoff = new StringBuilder();
    //
    //        onoff.append(createAttribute(val[0]));
    //
    //        for(int i = 1 ; i < val.length; i++)
    //        {
    //            onoff.append(",");
    //            onoff.append(createAttribute(val[i]));
    //        }
    //
    //        return onoff.toString();
    //    }


    private static String createAttribute(String[] val) {
        if (val == null || val.length == 0) {
            return null;
        }

        StringBuilder onoff = new StringBuilder();

        onoff.append(createAttribute(val[0]));

        for (int i = 1 ; i < val.length; i++) {
            onoff.append(",");
            onoff.append(createAttribute(val[i]));
        }

        return onoff.toString();
    }

    private static String createAttribute(Double[] val) {
        if (val == null || val.length == 0) {
            return null;
        }

        StringBuilder onoff = new StringBuilder();

        onoff.append(createAttribute(val[0]));

        for (int i = 1 ; i < val.length; i++) {
            onoff.append(",");
            onoff.append(createAttribute(val[i]));
        }

        return onoff.toString();
    }

    private static String createAttribute(Integer[] val) {
        if (val == null || val.length == 0) {
            return null;
        }

        StringBuilder onoff = new StringBuilder();

        onoff.append(createAttribute(val[0]));

        for (int i = 1 ; i < val.length; i++) {
            onoff.append(",");
            onoff.append(createAttribute(val[i]));
        }

        return onoff.toString();
    }

    private static void setAttribute(Element elem, String property, String value, String ref) {
        if (value == null || property == null || elem == null || (ref != null && value.equals(ref))) {
            return;
        }

        elem.setAttribute(property, value);
    }

    private static void initDefaultui(Integer uicontrolStyle) {
        Integer uic = GraphicController.getController().askObject(GraphicObject.getTypeFromName(uicontrolStyle));
        defaultUi = (Uicontrol) GraphicController.getController().getObjectFromId(uic);
    }

}
