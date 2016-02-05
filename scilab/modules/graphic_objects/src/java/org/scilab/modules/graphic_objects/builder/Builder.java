/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 * Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
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

package org.scilab.modules.graphic_objects.builder;

import java.util.ArrayList;
import java.util.StringTokenizer;

import org.scilab.modules.graphic_objects.ScilabNativeView;
import org.scilab.modules.graphic_objects.arc.Arc;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.axis.Axis;
import org.scilab.modules.graphic_objects.fec.Fec;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.figure.Figure.BarType;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicModel.GraphicModel;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.Type;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.imageplot.Imageplot;
import org.scilab.modules.graphic_objects.label.Label;
import org.scilab.modules.graphic_objects.legend.Legend;
import org.scilab.modules.graphic_objects.lighting.Light;
import org.scilab.modules.graphic_objects.lighting.Light.LightType;
import org.scilab.modules.graphic_objects.polyline.Polyline;
import org.scilab.modules.graphic_objects.rectangle.Rectangle;
import org.scilab.modules.graphic_objects.surface.Surface;
import org.scilab.modules.graphic_objects.textObject.Font;
import org.scilab.modules.graphic_objects.textObject.Text;
import org.scilab.modules.graphic_objects.vectfield.Champ;
import org.scilab.modules.graphic_objects.vectfield.Segs;

public final class Builder {
    public final static int createRect(int parentSubwin, double x, double y, double height, double width, int foreground, int background, int isfilled, int isline) {
        GraphicController controller = GraphicController.getController();
        Axes axes = (Axes) controller.getObjectFromId(parentSubwin);

        if (height < 0 || width < 0) {
            return 0;
        }

        Integer iRect = controller.askObject(Type.RECTANGLE, false);

        /*
         * Sets the rectangle's parent in order to initialize the former's
         * Contoured properties with the latter's values (cloneGraphicContext
         * call below)
         */

        Rectangle rect = (Rectangle) controller.getObjectFromId(iRect);
        rect.setUpperLeftPoint(new Double[] { x, y, 0.0 });
        rect.setHeight(height);
        rect.setWidth(width);

        rect.setVisible(axes.getVisible());

        /* Clip state and region */
        /* To be checked for consistency */

        rect.setClipBox(axes.getClipBox());
        rect.setClipBoxSet(axes.getClipBoxSet());
        rect.setClipState(axes.getClipState());
        rect.setMarkMode(axes.getMarkMode());

        /*
         * Initializes the contour properties (background, foreground, etc) to
         * the default values (those of the parent Axes).
         */
        cloneGraphicContext(parentSubwin, iRect);

        /* Contour settings */
        rect.setLineMode(isline == 1);
        rect.setFillMode(isfilled == 1);

        if (foreground != -1) {
            rect.setLineColor(foreground);
        }

        if (background != -1) {
            rect.setBackground(background);
        }

        controller.objectCreated(iRect);

        /*
         * Sets the Axes as the rectangle's parent and adds the rectangle to its
         * parent's list of children.
         */
        //setGraphicObjectRelationship(pparentsubwinUID, pobjUID);

        return iRect;
    }

    public static int cloneGraphicContext(int sourceIdentifier, int destIdentifier) {
        GraphicController controller = GraphicController.getController();
        Boolean lineMode = false;
        Integer foreground = 0;
        Integer lineStyle = 0;
        Integer background = 0;
        Integer markForeground = 0;
        Integer markBackground = 0;
        Integer markStyle = 0;
        Integer markSize = 0;
        Integer markSizeUnit = 0;
        Double lineThickness = 0.;

        /*
         * All these properties are passed by value thus do not care to release
         * them and do not call releaseGraphicObjectProperty on purpose.
         */

        lineMode = (Boolean) controller.getProperty(sourceIdentifier, GraphicObjectProperties.__GO_LINE_MODE__);
        foreground = (Integer) controller.getProperty(sourceIdentifier, GraphicObjectProperties.__GO_LINE_COLOR__);
        lineThickness = (Double) controller.getProperty(sourceIdentifier, GraphicObjectProperties.__GO_LINE_THICKNESS__);
        lineStyle = (Integer) controller.getProperty(sourceIdentifier, GraphicObjectProperties.__GO_LINE_STYLE__);

        background = (Integer) controller.getProperty(sourceIdentifier, GraphicObjectProperties.__GO_BACKGROUND__);

        markForeground = (Integer) controller.getProperty(sourceIdentifier, GraphicObjectProperties.__GO_MARK_FOREGROUND__);

        markBackground = (Integer) controller.getProperty(sourceIdentifier, GraphicObjectProperties.__GO_MARK_BACKGROUND__);

        markStyle = (Integer) controller.getProperty(sourceIdentifier, GraphicObjectProperties.__GO_MARK_STYLE__);

        markSize = (Integer) controller.getProperty(sourceIdentifier, GraphicObjectProperties.__GO_MARK_SIZE__);

        markSizeUnit = (Integer) controller.getProperty(sourceIdentifier, GraphicObjectProperties.__GO_MARK_SIZE_UNIT__);

        controller.setProperty(destIdentifier, GraphicObjectProperties.__GO_LINE_MODE__, lineMode);
        controller.setProperty(destIdentifier, GraphicObjectProperties.__GO_LINE_COLOR__, foreground);
        controller.setProperty(destIdentifier, GraphicObjectProperties.__GO_LINE_THICKNESS__, lineThickness);
        controller.setProperty(destIdentifier, GraphicObjectProperties.__GO_LINE_STYLE__, lineStyle);

        controller.setProperty(destIdentifier, GraphicObjectProperties.__GO_BACKGROUND__, background);

        controller.setProperty(destIdentifier, GraphicObjectProperties.__GO_MARK_FOREGROUND__, markForeground);
        controller.setProperty(destIdentifier, GraphicObjectProperties.__GO_MARK_BACKGROUND__, markBackground);
        controller.setProperty(destIdentifier, GraphicObjectProperties.__GO_MARK_STYLE__, markStyle);
        controller.setProperty(destIdentifier, GraphicObjectProperties.__GO_MARK_SIZE__, markSize);
        controller.setProperty(destIdentifier, GraphicObjectProperties.__GO_MARK_SIZE_UNIT__, markSizeUnit);

        return 0;
    }

    public static int cloneFontContext(int sourceIdentifier, int destIdentifier) {
        GraphicController controller = GraphicController.getController();

        Integer fontColor = (Integer) controller.getProperty(sourceIdentifier, GraphicObjectProperties.__GO_FONT_COLOR__);
        Integer fontStyle = (Integer) controller.getProperty(sourceIdentifier, GraphicObjectProperties.__GO_FONT_STYLE__);
        Double fontSize = (Double) controller.getProperty(sourceIdentifier, GraphicObjectProperties.__GO_FONT_SIZE__);
        Boolean fontFractional = (Boolean) controller.getProperty(sourceIdentifier, GraphicObjectProperties.__GO_FONT_FRACTIONAL__);

        controller.setProperty(destIdentifier, GraphicObjectProperties.__GO_FONT_COLOR__, fontColor);
        controller.setProperty(destIdentifier, GraphicObjectProperties.__GO_FONT_STYLE__, fontStyle);
        controller.setProperty(destIdentifier, GraphicObjectProperties.__GO_FONT_SIZE__, fontSize);
        controller.setProperty(destIdentifier, GraphicObjectProperties.__GO_FONT_FRACTIONAL__, fontFractional);

        return 0;
    }

    public static int createHiddenLabel(int parent) {

        GraphicController controller = GraphicController.getController();

        Integer iLabel = controller.askObject(Type.LABEL, false);
        Label label = (Label) controller.getObjectFromId(iLabel);

        //Hide Label as they are non explicit children
        label.setHidden(true);
        label.setAutoPosition(true);
        label.setAutoRotation(true);

        cloneGraphicContext(parent, iLabel);
        cloneFontContext(parent, iLabel);
        controller.objectCreated(iLabel);

        // Sets the label's parent
        controller.setGraphicObjectRelationship(parent, iLabel);

        return iLabel;
    }

    public static void initSubWinBounds(int subWin) {
        GraphicController controller = GraphicController.getController();

        controller.setProperty(subWin, GraphicObjectProperties.__GO_X_AXIS_LOG_FLAG__, false);
        controller.setProperty(subWin, GraphicObjectProperties.__GO_Y_AXIS_LOG_FLAG__, false);
        controller.setProperty(subWin, GraphicObjectProperties.__GO_Z_AXIS_LOG_FLAG__, false);
    }

    public static void reinitSubWin(int subWin) {
        GraphicController controller = GraphicController.getController();
        Integer label = 0;

        for (Integer childId : (Integer[]) controller.getProperty(subWin, GraphicObjectProperties.__GO_CHILDREN__)) {
            GraphicModel.getModel().deleteObject(childId);
        }

        label = createHiddenLabel(subWin);
        controller.setProperty(subWin, GraphicObjectProperties.__GO_TITLE__, label);

        label = createHiddenLabel(subWin);
        controller.setProperty(subWin, GraphicObjectProperties.__GO_X_AXIS_LABEL__, label);

        label = createHiddenLabel(subWin);
        controller.setProperty(subWin, GraphicObjectProperties.__GO_Y_AXIS_LABEL__, label);

        label = createHiddenLabel(subWin);
        controller.setProperty(subWin, GraphicObjectProperties.__GO_Z_AXIS_LABEL__, label);

        controller.setProperty(subWin, GraphicObjectProperties.__GO_X_AXIS_LOCATION__, 0);
        controller.setProperty(subWin, GraphicObjectProperties.__GO_Y_AXIS_LOCATION__, 4);
        controller.setProperty(subWin, GraphicObjectProperties.__GO_VISIBLE__, true);
        controller.setProperty(subWin, GraphicObjectProperties.__GO_FIRST_PLOT__, true);

        Integer axesModel = GraphicModel.getAxesModel().getIdentifier();
        Axes axes = (Axes) controller.getObjectFromId(axesModel);

        controller.setProperty(subWin, GraphicObjectProperties.__GO_VIEW__, axes.getView());
        controller.setProperty(subWin, GraphicObjectProperties.__GO_ROTATION_ANGLES__, axes.getRotationAngles());
        controller.setProperty(subWin, GraphicObjectProperties.__GO_ROTATION_ANGLES_3D__, axes.getRotationAngles3d());
    }

    public static boolean isAxesRedrawing(int subWin) {
        GraphicController controller = GraphicController.getController();
        Axes axes = (Axes) controller.getObjectFromId(subWin);

        if (axes.getAutoClear()) {
            reinitSubWin(subWin);
            return true;
        }
        return false;
    }

    public static int createLabel(int parent, int type) {
        GraphicController controller = GraphicController.getController();
        Double[] position = new Double[3];
        position[0] = 1.0;
        position[1] = 1.0;
        position[2] = 1.0;

        //check parent type
        Integer parentType = (Integer) controller.getProperty(parent, GraphicObjectProperties.__GO_TYPE__);
        if (parentType != GraphicObjectProperties.__GO_AXES__) {
            return 1;
        }

        //get axes model
        Integer axesModel = GraphicModel.getAxesModel().getIdentifier();

        //get type label from axes model
        Integer labelSource = (Integer) controller.getProperty(axesModel, type);

        //clone label
        Integer newLabel = controller.cloneObject(labelSource);
        controller.setProperty(newLabel, GraphicObjectProperties.__GO_POSITION__, position);
        // Auto position must be reset as setting the position has set it to false
        Boolean autoPosition = (Boolean) controller.getProperty(labelSource, GraphicObjectProperties.__GO_AUTO_POSITION__);
        controller.setProperty(newLabel, GraphicObjectProperties.__GO_AUTO_POSITION__, autoPosition);

        // Set relation between newLabel and parent
        controller.setProperty(parent, type, newLabel);
        controller.setGraphicObjectRelationship(parent, newLabel);
        return 0;
    }

    public final static int createFigureFromModel() {
        GraphicController controller = GraphicController.getController();
        return controller.cloneObject(GraphicModel.getFigureModel().getIdentifier());
    }

    public final static int cloneAxesModel(int parent) {
        GraphicController controller = GraphicController.getController();

        Integer newAxes = controller.cloneObject(GraphicModel.getAxesModel().getIdentifier());
        createLabel(newAxes, GraphicObjectProperties.__GO_X_AXIS_LABEL__);
        createLabel(newAxes, GraphicObjectProperties.__GO_Y_AXIS_LABEL__);
        createLabel(newAxes, GraphicObjectProperties.__GO_Z_AXIS_LABEL__);
        createLabel(newAxes, GraphicObjectProperties.__GO_TITLE__);

        controller.setGraphicObjectRelationship(parent, newAxes);
        controller.setProperty(parent, GraphicObjectProperties.__GO_SELECTED_CHILD__, newAxes);

        ScilabNativeView.ScilabNativeView__setCurrentSubWin(newAxes);
        ScilabNativeView.ScilabNativeView__setCurrentObject(newAxes);
        return newAxes;
    }

    public final static void cloneMenus(int iModel, int iParent) {
        GraphicController controller = GraphicController.getController();
        GraphicObject model = controller.getObjectFromId(iModel);

        Integer[] children = model.getChildren();

        for (int i = children.length - 1; i >= 0; i--) {
            GraphicObject child = controller.getObjectFromId(children[i]);
            if (child.getType() == GraphicObjectProperties.__GO_UIMENU__) {
                Integer newMenu = controller.cloneObject(children[i]);
                controller.setGraphicObjectRelationship(iParent, newMenu);

                cloneMenus(children[i], newMenu);
            }
        }
    }

    public final static int createFigure(boolean dockable, int menubarType, int toolbarType, boolean defaultAxes, boolean visible) {
        GraphicController controller = GraphicController.getController();
        Integer figModel = GraphicModel.getFigureModel().getIdentifier();
        Integer figId = controller.cloneObject(figModel, false);
        Figure figure = (Figure) controller.getObjectFromId(figId);
        figure.setDockable(dockable);
        figure.setMenubar(menubarType);
        figure.setToolbar(toolbarType);
        figure.setVisible(visible);
        figure.setDefaultAxes(defaultAxes);

        controller.objectCreated(figId);

        ScilabNativeView.ScilabNativeView__setCurrentFigure(figId);

        if (menubarType == BarType.FIGURE.ordinal()) {
            cloneMenus(figModel, figId);
        }

        if (defaultAxes) {
            //clone default axes
            cloneAxesModel(figId);
        }

        return figId;
    }

    public final static int createNewFigureWithAxes() {
        GraphicController controller = GraphicController.getController();
        Integer figModel = GraphicModel.getFigureModel().getIdentifier();

        //clone default figure
        Integer newFigure = createFigureFromModel();

        //Clone the default menus
        cloneMenus(figModel, newFigure);

        //set ID to 0
        controller.setProperty(newFigure, GraphicObjectProperties.__GO_ID__, 0);

        //clone default axes
        cloneAxesModel(newFigure);
        ScilabNativeView.ScilabNativeView__setCurrentFigure(newFigure);

        //Force axes size after window creation
        Integer[] axesSize = (Integer[]) controller.getProperty(figModel, GraphicObjectProperties.__GO_AXES_SIZE__);
        controller.setProperty(newFigure, GraphicObjectProperties.__GO_AXES_SIZE__, axesSize);

        controller.setProperty(newFigure, GraphicObjectProperties.__GO_VALID__, true);

        return newFigure;
    }

    public final static int createSubWin(int parentFigure) {
        //GraphicController controller = GraphicController.getController();
        //GraphicObject parent = controller.getObjectFromId(parentFigure);
        //if (parent.getType() != GraphicObjectProperties.__GO_FIGURE__) {
        //    return 0;
        //}

        return cloneAxesModel(parentFigure);
    }

    public final static int createText(int iParentsubwinUID, String[] str, int nbRow, int nbCol, double x, double y, boolean autoSize, double[] userSize, int centerPos, int foreground,
                                       boolean isForeground, int background, boolean isBackground, boolean isBoxed, boolean isLine, boolean isFilled, int align) {

        GraphicController controller = GraphicController.getController();
        int iText = controller.askObject(Type.TEXT, false);

        Axes axes = (Axes) controller.getObjectFromId(iParentsubwinUID);
        Text text = (Text) controller.getObjectFromId(iText);

        //clip
        text.setClipBox(axes.getClipBox());
        text.setClipBoxSet(axes.getClipBoxSet());
        text.setClipState(axes.getClipState());

        //text
        Integer[] dimensions = new Integer[2];
        dimensions[0] = nbRow;
        dimensions[1] = nbCol;
        text.setTextArrayDimensions(dimensions);
        text.setTextStrings(str);

        //position
        Double[] position = new Double[3];
        position[0] = x;
        position[1] = y;
        position[2] = 0.0;
        text.setPosition(position);

        //test box
        Double[] setUserSize = new Double[2];
        text.setTextBoxMode(centerPos);
        text.setAutoDimensioning(autoSize);

        if (autoSize == false || centerPos != 0) {
            setUserSize[0] = userSize[0];
            setUserSize[1] = userSize[1];
        } else {
            setUserSize[0] = 0.0;
            setUserSize[1] = 0.0;
        }
        text.setTextBox(setUserSize);

        int alignment = align - 1;
        if (alignment < 0 || alignment > 2) {
            alignment = 0;
        }

        text.setAlignment(alignment);

        cloneGraphicContext(iParentsubwinUID, iText);
        cloneFontContext(iParentsubwinUID, iText);

        text.setBox(isBoxed);
        text.setLineMode(isLine);
        text.setFillMode(isFilled);

        if (isForeground) {
            text.setLineColor(foreground);
        }

        if (isBackground) {
            text.setBackground(background);
        }

        text.setVisible(axes.getVisible());
        controller.objectCreated(iText);
        return iText;
    }

    public final static int createArc(int parent, double x, double y, double h, double w, double startAngle, double endAngle, int foreground, boolean isForeground, int background,
                                      boolean isBackground, boolean filled, boolean line) {

        GraphicController controller = GraphicController.getController();
        int iArc = controller.askObject(Type.ARC, false);

        Axes axes = (Axes) controller.getObjectFromId(parent);
        Arc arc = (Arc) controller.getObjectFromId(iArc);

        //set visible false during construction
        arc.setVisible(false);

        Double[] upperLeftPoint = new Double[3];
        upperLeftPoint[0] = x;
        upperLeftPoint[1] = y;
        upperLeftPoint[2] = 0.0;

        arc.setUpperLeftPoint(upperLeftPoint);
        arc.setHeight(h);
        arc.setWidth(w);
        arc.setStartAngle(startAngle);
        arc.setEndAngle(endAngle);
        arc.setArcDrawingMethod(axes.getArcDrawingMethod());

        //clip
        arc.setClipBox(axes.getClipBox());
        arc.setClipBoxSet(axes.getClipBoxSet());
        arc.setClipState(axes.getClipState());

        cloneGraphicContext(parent, iArc);

        arc.setLineMode(line);
        arc.setFillMode(filled);

        if (isForeground) {
            arc.setLineColor(foreground);
        }

        if (isBackground) {
            arc.setBackground(background);
        }

        arc.setVisible(axes.getVisible());
        controller.objectCreated(iArc);
        controller.setGraphicObjectRelationship(parent, iArc);
        return iArc;
    }

    public final static int createAxis(int parent, int dir, int tics, double[] vx, double[] vy, int subint, String format, int fontSize, int textColor, int ticsColor, boolean seg) {

        GraphicController controller = GraphicController.getController();
        int iAxis = controller.askObject(Type.AXIS, false);

        Axes axes = (Axes) controller.getObjectFromId(parent);
        Axis axis = (Axis) controller.getObjectFromId(iAxis);

        //clip box
        axis.setClipBox(axes.getClipBox());
        axis.setClipBoxSet(false);

        axis.setTicksDirection(dir);
        axis.setTicksStyle(tics);

        axis.setXTicksCoords(toDouble(vx));
        axis.setYTicksCoords(toDouble(vy));

        if (format != null && format.equals("") == false) {
            axis.setFormatn(format);
        }

        axis.setSubticks(subint);
        axis.setTicksSegment(seg);
        cloneGraphicContext(parent, iAxis);
        cloneFontContext(parent, iAxis);

        Font font = axis.getFont();
        if (fontSize == -1) {
            font.setSize(axes.getFontSize());
        } else {
            font.setSize(new Double(fontSize));
        }
        if (textColor == -1) {
            font.setColor(axes.getFontColor());
        } else {
            font.setColor(textColor);
        }
        font.setStyle(axes.getFontStyle());

        axis.setTicksColor(ticsColor);

        controller.objectCreated(iAxis);
        controller.setGraphicObjectRelationship(parent, iAxis);
        return iAxis;
    }

    public static int createCompound(int parent, int[] children) {
        GraphicController controller = GraphicController.getController();
        int iCompound = controller.askObject(Type.COMPOUND);

        GraphicObject obj = controller.getObjectFromId(parent);
        for (int i = 0; i < children.length; i++) {
            controller.setGraphicObjectRelationship(iCompound, children[i]);
        }

        controller.setGraphicObjectRelationship(parent, iCompound);
        controller.setProperty(iCompound, GraphicObjectProperties.__GO_VISIBLE__, obj.getVisible());
        return iCompound;
    }

    public static int createCompoundSeq(int parent, int childrenCount) {
        GraphicController controller = GraphicController.getController();
        int iCompound = controller.askObject(Type.COMPOUND);
        GraphicObject axes = controller.getObjectFromId(parent);

        Integer[] children = axes.getChildren();

        /*
         * Remove the last "number" created objects (located at the children
         * list's head) and add them to the compound in the same order
         */
        for (int i = 0; i < childrenCount; i++) {
            /*
             * Set the parent-child relationship between the Compound and each
             * aggregated object. Children are added to the Compound from the
             * least recent to the most recent, to preserve their former
             * ordering.
             */
            controller.setGraphicObjectRelationship(iCompound, children[childrenCount - i - 1]);
        }

        controller.setGraphicObjectRelationship(parent, iCompound);

        /*
         * visibility is obtained from the parent Figure, whereas it is
         * retrieved from the parent Axes in ConstructCompound. To be made
         * consistent.
         */
        Figure fig = (Figure) controller.getObjectFromId(axes.getParentFigure());
        controller.setProperty(iCompound, GraphicObjectProperties.__GO_VISIBLE__, fig.getVisible());
        return iCompound;
    }

    public static int createFec(int parent, double[] zminmax, int[] colminmax, int[] colout, boolean with_mesh) {

        GraphicController controller = GraphicController.getController();
        int iFec = controller.askObject(Type.FEC, false);
        Axes axes = (Axes) controller.getObjectFromId(parent);
        Fec fec = (Fec) controller.getObjectFromId(iFec);

        fec.setZBounds(toDouble(zminmax));
        fec.setColorRange(toInteger(colminmax));
        fec.setOutsideColor(toInteger(colout));

        fec.setVisible(axes.getVisible());

        //clip box
        fec.setClipBox(axes.getClipBox());
        fec.setClipBoxSet(axes.getClipBoxSet());
        fec.setClipState(axes.getClipState());

        cloneGraphicContext(parent, iFec);

        fec.setLineMode(with_mesh);

        controller.objectCreated(iFec);
        return iFec;
    }

    public static int createGrayplot(int parent, int type, double[] pvecx, int n1, int n2) {

        int[] objectTypes = new int[] { GraphicObjectProperties.__GO_GRAYPLOT__, GraphicObjectProperties.__GO_MATPLOT__, GraphicObjectProperties.__GO_MATPLOT__ };

        GraphicController controller = GraphicController.getController();
        int iPlot = controller.askObject(GraphicObject.getTypeFromName(objectTypes[type]), false);
        Axes axes = (Axes) controller.getObjectFromId(parent);
        Imageplot plot = (Imageplot) controller.getObjectFromId(iPlot);

        if (type == 2) { //Matplot1
            Double[] data = new Double[pvecx.length];
            for (int i = 0; i < pvecx.length; i++) {
                data[i] = pvecx[i];
            }
            plot.setTranslate(data);

            Double[] scale = new Double[2];
            scale[0] = (pvecx[2] - pvecx[0]) / (n2 - 1.0);
            scale[1] = (pvecx[3] - pvecx[1]) / (n1 - 1.0);
            plot.setScale(scale);
        }

        plot.setVisible(axes.getVisible());

        //clip box
        plot.setClipBox(axes.getClipBox());
        plot.setClipBoxSet(axes.getClipBoxSet());
        plot.setClipState(axes.getClipState());

        cloneGraphicContext(parent, iPlot);
        controller.objectCreated(iPlot);
        return iPlot;
    }

    public static int createPolyline(int parent, boolean closed, int plot, int foreground, boolean isForeground, int[] background, int mark_style, boolean isMarkStyle, int mark_foreground,
                                     boolean isMarkForeground, int mark_background, boolean isMarkBackground, boolean isline, boolean isfilled, boolean ismark, boolean isinterp) {

        GraphicController controller = GraphicController.getController();
        int iPoly = controller.askObject(Type.POLYLINE, false);
        Axes axes = (Axes) controller.getObjectFromId(parent);
        Polyline poly = (Polyline) controller.getObjectFromId(iPoly);

        poly.setVisible(false);

        //clip box
        poly.setClipBox(axes.getClipBox());
        poly.setClipBoxSet(axes.getClipBoxSet());
        poly.setClipState(axes.getClipState());

        poly.setClosed(closed);
        poly.setPolylineStyle(plot);

        cloneGraphicContext(parent, iPoly);

        poly.setMarkMode(ismark);
        poly.setLineMode(isline);
        poly.setFillMode(isfilled);
        poly.setInterpColorMode(isinterp);

        if (isForeground) {
            poly.setLineColor(foreground);
        }

        if (background.length != 0) {
            if (isinterp) {
                /* 3 or 4 values to store */
                Integer[] color = new Integer[background.length];
                for (int i = 0; i < background.length; i++) {
                    color[i] = background[i];
                }

                poly.setInterpColorVector(color);
            } else {
                poly.setBackground(background[0]);
            }
        }

        if (isMarkStyle) {
            poly.setMarkStyle(mark_style);
        }

        if (isMarkForeground) {
            poly.setMarkForeground(mark_foreground);
        }

        if (isMarkBackground) {
            poly.setMarkBackground(mark_background);
        }

        poly.setVisible(true);
        controller.objectCreated(iPoly);
        return iPoly;
    }

    public static int createLegend(int parent, String[] text, int[] handles) {
        GraphicController controller = GraphicController.getController();
        Axes axes = (Axes) controller.getObjectFromId(parent);

        if (axes.getHasLegendChild()) {
            controller.removeRelationShipAndDelete(axes.getLegendChild());
        }

        int iLeg = controller.askObject(Type.LEGEND, false);
        Legend leg = (Legend) controller.getObjectFromId(iLeg);

        leg.setParent(parent);
        leg.setVisible(axes.getVisible());

        int count = handles.length;
        Integer[] textDims = new Integer[2];
        textDims[0] = count;
        textDims[1] = 1;

        leg.setTextArrayDimensions(textDims);
        leg.setTextStrings(text);

        /*
         * Links are ordered from most recent to least recent, as their
         * referred-to Polylines in the latter's parent Compound object.
         */

        ArrayList<Integer> links = new ArrayList<Integer>();
        for (int i = count - 1; i >= 0; i--) {
            links.add(count - i - 1, handles[i]);
        }

        leg.setLinks(links);

        leg.setClipBoxSet(false);
        leg.setClipState(0); //OFF
        leg.setClipBox(axes.getClipBox());

        cloneGraphicContext(parent, iLeg);
        cloneFontContext(parent, iLeg);

        leg.setFillMode(true);

        controller.objectCreated(iLeg);
        leg.setParent(0);
        controller.setGraphicObjectRelationship(parent, iLeg);
        return iLeg;
    }

    public static int createSegs(int parent, double[] vx, double[] vy, double[] vz, boolean isVZ, int[] style, double arsize) {
        GraphicController controller = GraphicController.getController();
        Axes axes = (Axes) controller.getObjectFromId(parent);
        int iSegs = controller.askObject(Type.SEGS, false);
        Segs segs = (Segs) controller.getObjectFromId(iSegs);

        segs.setVisible(axes.getVisible());

        //clip box
        segs.setClipBox(axes.getClipBox());
        segs.setClipBoxSet(axes.getClipBoxSet());
        segs.setClipState(axes.getClipState());

        /* Segs: Nbr1/2 arrows, Nbr1 is the number of endpoints */
        int numberArrows = vx.length / 2;

        segs.setNumberArrows(numberArrows);
        segs.setArrowSize(arsize);

        Double[] arrowCoords = new Double[3 * numberArrows];
        for (int i = 0; i < numberArrows; i++) {
            arrowCoords[3 * i] = vx[2 * i];
            arrowCoords[3 * i + 1] = vy[2 * i];
            if (isVZ) {
                arrowCoords[3 * i + 2] = vz[2 * i];
            } else {
                arrowCoords[3 * i + 2] = 0.0;
            }
        }

        segs.setBase(arrowCoords);

        for (int i = 0; i < numberArrows; i++) {
            arrowCoords[3 * i] = vx[2 * i + 1];
            arrowCoords[3 * i + 1] = vy[2 * i + 1];
            if (isVZ) {
                arrowCoords[3 * i + 2] = vz[2 * i + 1];
            } else {
                arrowCoords[3 * i + 2] = 0.0;
            }
        }

        segs.setDirection(arrowCoords);
        segs.setColors(toInteger(style));

        cloneGraphicContext(parent, iSegs);
        controller.objectCreated(iSegs);
        controller.setGraphicObjectRelationship(parent, iSegs);
        return iSegs;
    }

    public static int createChamp(int parent, double[] vx, double[] vy, double[] vfx, double[] vfy, double arsize, boolean typeofchamp) {

        GraphicController controller = GraphicController.getController();
        Axes axes = (Axes) controller.getObjectFromId(parent);

        int iChamp = controller.askObject(Type.CHAMP, false);
        Champ champ = (Champ) controller.getObjectFromId(iChamp);

        champ.setVisible(axes.getVisible());

        //clip box
        champ.setClipBox(axes.getClipBox());
        champ.setClipBoxSet(axes.getClipBoxSet());
        champ.setClipState(axes.getClipState());

        int numberArrows = vx.length * vy.length;
        champ.setNumberArrows(numberArrows);

        Integer[] dimensions = new Integer[] { vx.length, vy.length };
        champ.setDimensions(dimensions);

        champ.setArrowSize(arsize);

        Double[] arrowCoords = new Double[3 * numberArrows];

        //convert vx Double
        Double[] temp = new Double[vx.length];
        for (int i = 0; i < vx.length; i++) {
            temp[i] = vx[i];
        }
        champ.setBaseX(temp);

        //convert vy Double
        temp = new Double[vy.length];
        for (int i = 0; i < vy.length; i++) {
            temp[i] = vy[i];
        }
        champ.setBaseY(temp);

        for (int i = 0; i < numberArrows; i++) {
            arrowCoords[3 * i] = vfx[i];
            arrowCoords[3 * i + 1] = vfy[i];
            arrowCoords[3 * i + 2] = 0.0;
        }

        champ.setDirection(arrowCoords);
        champ.setColored(typeofchamp);

        cloneGraphicContext(parent, iChamp);

        controller.objectCreated(iChamp);
        controller.setGraphicObjectRelationship(parent, iChamp);

        return iChamp;
    }

    public static int createSurface(int parent, int typeof3d, int colorFlag, int colorMode) {
        GraphicController controller = GraphicController.getController();
        Axes axes = (Axes) controller.getObjectFromId(parent);

        int iSurf = controller.askObject(GraphicObject.getTypeFromName(typeof3d), false);
        Surface surf = (Surface) controller.getObjectFromId(iSurf);

        surf.setVisible(axes.getVisible());

        //clip box
        surf.setClipBox(axes.getClipBox());
        surf.setClipBoxSet(axes.getClipBoxSet());
        surf.setClipState(axes.getClipState());

        surf.setColorFlag(colorFlag);
        surf.setColorMode(colorMode);

        surf.setHiddenColor(axes.getHiddenColor());

        surf.setSurfaceMode(true);
        cloneGraphicContext(parent, iSurf);
        controller.objectCreated(iSurf);
        controller.setGraphicObjectRelationship(parent, iSurf);

        return iSurf;
    }

    public static void initSubWinTo3d(int iSubwin, String legend, boolean isLegend, int[] flag, double alpha, double theta, double[] ebox, double[] x, double[] y, double[] z) {
        GraphicController controller = GraphicController.getController();

        // Force 3d view
        controller.setProperty(iSubwin, GraphicObjectProperties.__GO_VIEW__, 1);

        if (isLegend) {
            StringTokenizer strTok = new StringTokenizer(legend, "@");
            int iToken = strTok.countTokens();
            if (iToken > 0) {
                //X
                String str[] = new String[] { strTok.nextToken() };
                Integer label = (Integer) controller.getProperty(iSubwin, GraphicObjectProperties.__GO_X_AXIS_LABEL__);
                Integer[] dims = new Integer[] { 1, 1 };
                controller.setProperty(label, GraphicObjectProperties.__GO_TEXT_ARRAY_DIMENSIONS__, dims);
                controller.setProperty(label, GraphicObjectProperties.__GO_TEXT_STRINGS__, str);
            }

            if (iToken > 1) {
                //Y
                String str[] = new String[] { strTok.nextToken() };
                Integer label = (Integer) controller.getProperty(iSubwin, GraphicObjectProperties.__GO_Y_AXIS_LABEL__);
                Integer[] dims = new Integer[] { 1, 1 };
                controller.setProperty(label, GraphicObjectProperties.__GO_TEXT_ARRAY_DIMENSIONS__, dims);
                controller.setProperty(label, GraphicObjectProperties.__GO_TEXT_STRINGS__, str);
            }

            if (iToken > 2) {
                //Z
                String str[] = new String[] { strTok.nextToken() };
                Integer label = (Integer) controller.getProperty(iSubwin, GraphicObjectProperties.__GO_Z_AXIS_LABEL__);
                Integer[] dims = new Integer[] { 1, 1 };
                controller.setProperty(label, GraphicObjectProperties.__GO_TEXT_ARRAY_DIMENSIONS__, dims);
                controller.setProperty(label, GraphicObjectProperties.__GO_TEXT_STRINGS__, str);
            }
        }

        // Force psubwin->logflags to linear
        controller.setProperty(iSubwin, GraphicObjectProperties.__GO_X_AXIS_LOG_FLAG__, false);
        controller.setProperty(iSubwin, GraphicObjectProperties.__GO_Y_AXIS_LOG_FLAG__, false);
        controller.setProperty(iSubwin, GraphicObjectProperties.__GO_Z_AXIS_LOG_FLAG__, false);

        Boolean firstPlot = (Boolean) controller.getProperty(iSubwin, GraphicObjectProperties.__GO_FIRST_PLOT__);

        if (firstPlot && (flag[2] == 0 || flag[2] == 1)) {
            /* Nothing to do */
        } else {
            if (flag[2] == 0 || flag[2] == 1) {
                if (firstPlot) {
                    controller.setProperty(iSubwin, GraphicObjectProperties.__GO_X_AXIS_VISIBLE__, false);
                    controller.setProperty(iSubwin, GraphicObjectProperties.__GO_Y_AXIS_VISIBLE__, false);
                    controller.setProperty(iSubwin, GraphicObjectProperties.__GO_Z_AXIS_VISIBLE__, false);

                    // 0: OFF
                    controller.setProperty(iSubwin, GraphicObjectProperties.__GO_BOX_TYPE__, 0);

                    Integer label = 0;
                    label = (Integer) controller.getProperty(iSubwin, GraphicObjectProperties.__GO_X_AXIS_LABEL__);
                    controller.setProperty(label, GraphicObjectProperties.__GO_VISIBLE__, false);
                    label = (Integer) controller.getProperty(iSubwin, GraphicObjectProperties.__GO_Y_AXIS_LABEL__);
                    controller.setProperty(label, GraphicObjectProperties.__GO_VISIBLE__, false);
                    label = (Integer) controller.getProperty(iSubwin, GraphicObjectProperties.__GO_Z_AXIS_LABEL__);
                    controller.setProperty(label, GraphicObjectProperties.__GO_VISIBLE__, false);
                }
            } else if (flag[2] == 2) {
                controller.setProperty(iSubwin, GraphicObjectProperties.__GO_X_AXIS_VISIBLE__, false);
                controller.setProperty(iSubwin, GraphicObjectProperties.__GO_Y_AXIS_VISIBLE__, false);
                controller.setProperty(iSubwin, GraphicObjectProperties.__GO_Z_AXIS_VISIBLE__, false);

                //2: HIDDEN_AXES
                controller.setProperty(iSubwin, GraphicObjectProperties.__GO_BOX_TYPE__, 2);

                Integer label = 0;
                label = (Integer) controller.getProperty(iSubwin, GraphicObjectProperties.__GO_X_AXIS_LABEL__);
                controller.setProperty(label, GraphicObjectProperties.__GO_VISIBLE__, false);
                label = (Integer) controller.getProperty(iSubwin, GraphicObjectProperties.__GO_Y_AXIS_LABEL__);
                controller.setProperty(label, GraphicObjectProperties.__GO_VISIBLE__, false);
                label = (Integer) controller.getProperty(iSubwin, GraphicObjectProperties.__GO_Z_AXIS_LABEL__);
                controller.setProperty(label, GraphicObjectProperties.__GO_VISIBLE__, false);

            } else if (flag[2] == 3) {
                controller.setProperty(iSubwin, GraphicObjectProperties.__GO_X_AXIS_VISIBLE__, false);
                controller.setProperty(iSubwin, GraphicObjectProperties.__GO_Y_AXIS_VISIBLE__, false);
                controller.setProperty(iSubwin, GraphicObjectProperties.__GO_Z_AXIS_VISIBLE__, false);

                //1: ON
                controller.setProperty(iSubwin, GraphicObjectProperties.__GO_BOX_TYPE__, 1);

                Integer label = 0;
                label = (Integer) controller.getProperty(iSubwin, GraphicObjectProperties.__GO_X_AXIS_LABEL__);
                controller.setProperty(label, GraphicObjectProperties.__GO_VISIBLE__, true);
                label = (Integer) controller.getProperty(iSubwin, GraphicObjectProperties.__GO_Y_AXIS_LABEL__);
                controller.setProperty(label, GraphicObjectProperties.__GO_VISIBLE__, true);
                label = (Integer) controller.getProperty(iSubwin, GraphicObjectProperties.__GO_Z_AXIS_LABEL__);
                controller.setProperty(label, GraphicObjectProperties.__GO_VISIBLE__, true);

            } else if (flag[2] == 4) {
                controller.setProperty(iSubwin, GraphicObjectProperties.__GO_X_AXIS_VISIBLE__, true);
                controller.setProperty(iSubwin, GraphicObjectProperties.__GO_Y_AXIS_VISIBLE__, true);
                controller.setProperty(iSubwin, GraphicObjectProperties.__GO_Z_AXIS_VISIBLE__, true);

                //1: ON
                controller.setProperty(iSubwin, GraphicObjectProperties.__GO_BOX_TYPE__, 1);

                Integer label = 0;
                label = (Integer) controller.getProperty(iSubwin, GraphicObjectProperties.__GO_X_AXIS_LABEL__);
                controller.setProperty(label, GraphicObjectProperties.__GO_VISIBLE__, true);
                label = (Integer) controller.getProperty(iSubwin, GraphicObjectProperties.__GO_Y_AXIS_LABEL__);
                controller.setProperty(label, GraphicObjectProperties.__GO_VISIBLE__, true);
                label = (Integer) controller.getProperty(iSubwin, GraphicObjectProperties.__GO_Z_AXIS_LABEL__);
                controller.setProperty(label, GraphicObjectProperties.__GO_VISIBLE__, true);
            }
        }

        Double[] rotationAngles = new Double[] { alpha, theta };
        controller.setProperty(iSubwin, GraphicObjectProperties.__GO_ROTATION_ANGLES__, rotationAngles);

        Double[] dataBounds = (Double[]) controller.getProperty(iSubwin, GraphicObjectProperties.__GO_DATA_BOUNDS__);
        Boolean autoScale = (Boolean) controller.getProperty(iSubwin, GraphicObjectProperties.__GO_AUTO_SCALE__);

        Double rect[] = new Double[] { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
        if (autoScale) {
            // compute and merge new specified bounds with data bounds
            switch (flag[1]) {
                case 0:
                    break;
                case 1:
                case 3:
                case 5:
                case 7:
                    rect[0] = ebox[0]; // xmin
                    rect[1] = ebox[1]; // xmax
                    rect[2] = ebox[2]; // ymin
                    rect[3] = ebox[3]; // ymax
                    rect[4] = ebox[4]; // zmin
                    rect[5] = ebox[5]; // zmax
                    break;
                case 2:
                case 4:
                case 6:
                case 8:
                    double[] res = getDrect(x, rect[0], rect[1], dataBounds[0], dataBounds[1]);
                    rect[0] = res[0];
                    rect[1] = res[1];

                    res = getDrect(y, rect[2], rect[3], dataBounds[2], dataBounds[3]);
                    rect[2] = res[0];
                    rect[3] = res[1];

                    res = getDrect(z, rect[4], rect[5], dataBounds[4], dataBounds[5]);
                    rect[4] = res[0];
                    rect[5] = res[1];
                    break;
            }

            if (firstPlot == false) {
                rect[0] = Math.min(dataBounds[0], rect[0]); // xmin
                rect[1] = Math.max(dataBounds[1], rect[1]); // xmax
                rect[2] = Math.min(dataBounds[2], rect[2]); // ymin
                rect[3] = Math.max(dataBounds[3], rect[3]); // ymax
                rect[4] = Math.min(dataBounds[4], rect[4]); // zmin
                rect[5] = Math.max(dataBounds[5], rect[5]); // zmax
            }

            if (flag[1] != 0) {
                controller.setProperty(iSubwin, GraphicObjectProperties.__GO_DATA_BOUNDS__, rect);
            }
        }

        if (flag[1] != 0) {
            boolean isoview = (flag[1] == 3 || flag[1] == 4 || flag[1] == 5 || flag[1] == 6);
            controller.setProperty(iSubwin, GraphicObjectProperties.__GO_ISOVIEW__, isoview);
        }
    }

    private static double[] getDrect(double[] x, double min, double max, double defaultMin, double defaultMax) {
        double refMax = Double.NEGATIVE_INFINITY;
        double refMin = Double.POSITIVE_INFINITY;
        boolean isInfinite = true;

        for (int i = 0; i < x.length; i++) {
            Double tmp = x[i];
            if (tmp.isInfinite() == false && tmp.isNaN() == false) {
                refMin = Math.min(refMin, tmp);
                refMax = Math.max(refMax, tmp);
                isInfinite = false;
            }
        }

        if (isInfinite) {
            refMin = defaultMin;
            refMax = defaultMax;
        }

        return new double[] { refMin, refMax };
    }

    private static boolean isValidType(int type) {
        return type >= 0 && type <= 1;
    }

    private static boolean isValidColor(double[] color) {
        return (color[0] >= 0.0 && color[0] <= 1.0) && (color[1] >= 0.0 && color[1] <= 1.0) && (color[2] >= 0.0 && color[2] <= 1.0);
    }

    public static int createLight(int parent, int type, boolean visible, double[] pos, double[] dir, double[] ambient, double[] diffuse, double[] specular) {
        GraphicController controller = GraphicController.getController();

        int iLight = controller.askObject(Type.LIGHT, false);
        Light light = (Light) controller.getObjectFromId(iLight);

        light.setVisible(visible);
        if (isValidType(type)) {
            light.setLightType(LightType.intToEnum(type));
        }

        if (pos.length == 3) {
            light.setPosition(toDouble(pos));
        }

        if (dir.length == 3) {
            light.setDirection(toDouble(dir));
        }
        if (ambient.length == 3) {
            if (isValidColor(ambient)) {
                light.setAmbientColor(toDouble(ambient));
            }
        }
        if (diffuse.length == 3) {
            if (isValidColor(diffuse)) {
                light.setDiffuseColor(toDouble(diffuse));
            }
        }
        if (specular.length == 3) {
            if (isValidColor(specular)) {
                light.setSpecularColor(toDouble(specular));
            }
        }

        controller.objectCreated(iLight);
        // Set light's parent
        controller.setGraphicObjectRelationship(parent, iLight);
        return iLight;
    }

    private static Double[] toDouble(double[] var) {
        Double[] ret = new Double[var.length];
        for (int i = 0; i < var.length; i++) {
            ret[i] = var[i];
        }

        return ret;
    }

    private static Integer[] toInteger(int[] var) {
        Integer[] ret = new Integer[var.length];
        for (int i = 0; i < var.length; i++) {
            ret[i] = var[i];
        }

        return ret;
    }
}
