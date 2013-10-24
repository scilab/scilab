/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 * Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.graphic_objects.builder;

import org.scilab.modules.graphic_objects.ScilabNativeView;
import org.scilab.modules.graphic_objects.arc.Arc;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.axis.Axis;
import org.scilab.modules.graphic_objects.compound.Compound;
import org.scilab.modules.graphic_objects.fec.Fec;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicModel.GraphicModel;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.imageplot.Imageplot;
import org.scilab.modules.graphic_objects.textObject.Font;
import org.scilab.modules.graphic_objects.textObject.Text;
import org.scilab.modules.graphic_objects.utils.TicksDirection;
import org.scilab.modules.graphic_objects.utils.TicksStyle;

public final class Builder {
    public final static int buildRect(int parentSubwin, double x, double y,
                                      double height, double width, int foreground,
                                      int background, int isfilled, int isline) {
        GraphicController controller = GraphicController.getController();

        Double[] clipRegion;
        Boolean visible = false;
        Boolean clipRegionSet = false;
        Integer clipState = 0;
        Boolean iMarkMode = false;

        if (height < 0 || width < 0) {
            return 0;
        }

        Integer pobjUID = controller.askObject(GraphicObject.getTypeFromName(GraphicObjectProperties.__GO_RECTANGLE__));

        /*
         * Sets the rectangle's parent in order to initialize the former's Contoured properties
         * with the latter's values (cloneGraphicContext call below)
         */
        controller.setProperty(pobjUID, GraphicObjectProperties.__GO_UPPER_LEFT_POINT__, new Double[] {x, y, 0.0});
        controller.setProperty(pobjUID, GraphicObjectProperties.__GO_HEIGHT__, height);
        controller.setProperty(pobjUID, GraphicObjectProperties.__GO_WIDTH__, width);

        visible = (Boolean) controller.getProperty(parentSubwin, GraphicObjectProperties.__GO_VISIBLE__);
        controller.setProperty(pobjUID, GraphicObjectProperties.__GO_VISIBLE__, visible);

        /* Clip state and region */
        /* To be checked for consistency */

        clipRegion = (Double[]) controller.getProperty(parentSubwin, GraphicObjectProperties.__GO_CLIP_BOX__);
        controller.setProperty(pobjUID, GraphicObjectProperties.__GO_CLIP_BOX__, clipRegion);

        clipRegionSet = (Boolean) controller.getProperty(parentSubwin, GraphicObjectProperties.__GO_CLIP_BOX_SET__);
        controller.setProperty(pobjUID, GraphicObjectProperties.__GO_CLIP_BOX_SET__,  clipRegionSet);

        clipState = (Integer) controller.getProperty(parentSubwin, GraphicObjectProperties.__GO_CLIP_STATE__);
        controller.setProperty(pobjUID, GraphicObjectProperties.__GO_CLIP_STATE__, clipState);

        iMarkMode = (Boolean) controller.getProperty(parentSubwin, GraphicObjectProperties.__GO_MARK_MODE__);
        controller.setProperty(pobjUID, GraphicObjectProperties.__GO_MARK_MODE__, iMarkMode);

        /*
         * Initializes the contour properties (background, foreground, etc)
         * to the default values (those of the parent Axes).
         */
        cloneGraphicContext(parentSubwin, pobjUID);

        /* Contour settings */
        controller.setProperty(pobjUID, GraphicObjectProperties.__GO_LINE_MODE__, isline == 1);
        controller.setProperty(pobjUID, GraphicObjectProperties.__GO_FILL_MODE__, isfilled == 1);

        if (foreground != -1) {
            controller.setProperty(pobjUID, GraphicObjectProperties.__GO_LINE_COLOR__, foreground);
        }

        if (background != -1) {
            controller.setProperty(pobjUID, GraphicObjectProperties.__GO_BACKGROUND__, background);
        }

        /* Parent reset to the null object */
        //setGraphicObjectProperty(pobjUID, GraphicObjectProperties.__GO_PARENT__, "", jni_string, 1);

        /*
         * Sets the Axes as the rectangle's parent and adds the rectangle to
         * its parent's list of children.
         */
        //setGraphicObjectRelationship(pparentsubwinUID, pobjUID);

        return pobjUID;
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
         * All these properties are passed by value thus do not care to release them
         * and do not call releaseGraphicObjectProperty on purpose.
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

        Integer newLabel = controller.askObject(GraphicObject.getTypeFromName(GraphicObjectProperties.__GO_LABEL__));

        //Hide Label as they are non explicit children
        controller.setProperty(newLabel, GraphicObjectProperties.__GO_HIDDEN__, true);
        controller.setProperty(newLabel, GraphicObjectProperties.__GO_AUTO_POSITION__, true);
        controller.setProperty(newLabel, GraphicObjectProperties.__GO_AUTO_ROTATION__, true);

        // Sets the label's parent
        controller.setGraphicObjectRelationship(parent, newLabel);

        cloneGraphicContext(parent, newLabel);
        cloneFontContext(parent, newLabel);

        return newLabel;
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

        Integer view = (Integer) controller.getProperty(axesModel, GraphicObjectProperties.__GO_VIEW__);
        Double[] angles = (Double[]) controller.getProperty(axesModel, GraphicObjectProperties.__GO_ROTATION_ANGLES__);
        Double[] angles3d = (Double[]) controller.getProperty(axesModel, GraphicObjectProperties.__GO_ROTATION_ANGLES_3D__);

        controller.setProperty(subWin, GraphicObjectProperties.__GO_VIEW__, view);
        controller.setProperty(subWin, GraphicObjectProperties.__GO_ROTATION_ANGLES__, angles);
        controller.setProperty(subWin, GraphicObjectProperties.__GO_ROTATION_ANGLES_3D__, angles3d);

    }

    public static boolean isAxesRedrawing(int subWin) {
        GraphicController controller = GraphicController.getController();

        Boolean autoClear = (Boolean) controller.getProperty(subWin, GraphicObjectProperties.__GO_AUTO_CLEAR__);

        if (autoClear) {
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
        Boolean autoPosition = (Boolean ) controller.getProperty(labelSource, GraphicObjectProperties.__GO_AUTO_POSITION__);
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

    public final static void cloneMenus(int model, int newParent) {
        GraphicController controller = GraphicController.getController();

        for (Integer childId : (Integer []) controller.getProperty(model, GraphicObjectProperties.__GO_CHILDREN__)) {

            Integer childType = (Integer) controller.getProperty(childId, GraphicObjectProperties.__GO_TYPE__);
            if (childType == GraphicObjectProperties.__GO_UIMENU__) {
                Integer newMenu = controller.cloneObject(childId);
                controller.setGraphicObjectRelationship(newParent, newMenu);
                cloneMenus(childId, newMenu);
            }
        }
    }

    public final static int createNewFigureWithAxes() {
        GraphicController controller = GraphicController.getController();
        Integer figModel  = GraphicModel.getFigureModel().getIdentifier();

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
        Integer[] figureSize = (Integer[]) controller.getProperty(figModel, GraphicObjectProperties.__GO_SIZE__);
        controller.setProperty(newFigure, GraphicObjectProperties.__GO_SIZE__, figureSize);
        Integer[] axesSize = (Integer[]) controller.getProperty(figModel, GraphicObjectProperties.__GO_AXES_SIZE__);
        controller.setProperty(newFigure, GraphicObjectProperties.__GO_AXES_SIZE__, axesSize);

        controller.setProperty(newFigure, GraphicObjectProperties.__GO_VALID__, true);


        return newFigure;
    }

    public final static int createSubWin(int parentFigure) {
        GraphicController controller = GraphicController.getController();

        Integer parentType = (Integer) controller.getProperty(parentFigure, GraphicObjectProperties.__GO_TYPE__);
        if (parentType != GraphicObjectProperties.__GO_FIGURE__) {
            return 0;
        }

        return cloneAxesModel(parentFigure);
    }

    public final static int allocateText(int iParentsubwinUID, String[] text, int nbRow, int nbCol,
                                         double x, double y, boolean autoSize, double[] userSize, int centerPos,
                                         int foreground, boolean isForeground, int background, boolean isBackground,
                                         boolean isBoxed, boolean isLine,
                                         boolean isFilled, int align) {

        GraphicController controller = GraphicController.getController();
        int iObj = controller.askObject(GraphicObject.getTypeFromName(GraphicObjectProperties.__GO_TEXT__));

        Axes objAxes = (Axes) controller.getObjectFromId(iParentsubwinUID);
        Text objText = (Text) controller.getObjectFromId(iObj);

        //set visible false during construction
        objText.setVisible(false);

        //clip
        objText.setClipBox(objAxes.getClipBox());
        objText.setClipBoxSet(objAxes.getClipBoxSet());
        objText.setClipState(objAxes.getClipState());

        //text
        Integer[] dimensions = new Integer[2];
        dimensions[0] = nbRow;
        dimensions[1] = nbCol;
        objText.setTextArrayDimensions(dimensions);
        objText.setTextStrings(text);

        //position
        Double[] position = new Double[3];
        position[0] = x;
        position[1] = y;
        position[2] = 0.0;
        objText.setPosition(position);

        //test box
        Double[] setUserSize = new Double[2];
        objText.setTextBoxMode(centerPos);
        objText.setAutoDimensioning(autoSize);

        if (autoSize == false || centerPos != 0) {
            setUserSize[0] = userSize[0];
            setUserSize[1] = userSize[1];
        } else {
            setUserSize[0] = 0.0;
            setUserSize[1] = 0.0;
        }
        objText.setTextBox(setUserSize);

        int alignment = align - 1;
        if (alignment < 0 || alignment > 2) {
            alignment = 0;
        }

        objText.setAlignment(alignment);

        cloneGraphicContext(iParentsubwinUID, iObj);
        cloneFontContext(iParentsubwinUID, iObj);

        objText.setBox(isBoxed);
        objText.setLineMode(isLine);
        objText.setFillMode(isFilled);

        if (isForeground) {
            objText.setLineColor(foreground);
        }

        if (isBackground) {
            objText.setBackground(background);
        }

        objText.setParent(0);
        objText.setVisible(objAxes.getVisible());
        return iObj;
    }

    public final static int createArc(int parent, double x, double y, double h, double w,
                                      double startAngle, double endAngle,
                                      int foreground, boolean isForeground, int background, boolean isBackground,
                                      boolean filled, boolean line) {

        GraphicController controller = GraphicController.getController();
        int iArc = controller.askObject(GraphicObject.getTypeFromName(GraphicObjectProperties.__GO_ARC__));

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
        controller.setGraphicObjectRelationship(parent, iArc);
        return iArc;
    }

    public final static int createAxis(int parent, int dir, int tics, double[] vx, double[] vy, int subint, String format,
                                       int fontSize, int textColor, int ticsColor, boolean seg) {

        GraphicController controller = GraphicController.getController();
        int iAxis = controller.askObject(GraphicObject.getTypeFromName(GraphicObjectProperties.__GO_AXIS__));

        Axes axes = (Axes) controller.getObjectFromId(parent);
        Axis axis = (Axis) controller.getObjectFromId(iAxis);

        //clip box
        axis.setClipBox(axes.getClipBox());
        axis.setClipBoxSet(false);

        axis.setTicsDirection(TicksDirection.stringToEnum(new String(new char[] {(char)dir})));
        axis.setTicsStyle(TicksStyle.stringToEnum(new String(new char[] {(char)tics})));
        axis.setXTicsCoord(vx);
        axis.setYTicsCoord(vy);

        if (format != null && format.equals("") == false) {
            axis.setFormatn(format);
        }

        axis.setSubtics(subint);
        axis.setTicsSegment(seg);
        cloneGraphicContext(parent, iAxis);
        cloneFontContext(parent, iAxis);

        Font font = axis.getFont();
        font.setSize(fontSize);
        font.setColor(textColor);
        axis.setTicsColor(ticsColor);

        controller.setGraphicObjectRelationship(parent, iAxis);
        return iAxis;
    }

    public static int createCompound(int parent, int[] children) {
        GraphicController controller = GraphicController.getController();
        int iCompound = controller.askObject(GraphicObject.getTypeFromName(GraphicObjectProperties.__GO_COMPOUND__));

        Axes axes = (Axes) controller.getObjectFromId(parent);
        Compound compound = (Compound) controller.getObjectFromId(iCompound);
        for (int i = 0 ; i < children.length ; i++) {
            controller.setGraphicObjectRelationship(iCompound, children[i]);
        }

        controller.setGraphicObjectRelationship(parent, iCompound);
        compound.setVisible(axes.getVisible());

        return iCompound;
    }

    public static int createCompoundSeq(int parent, int childrenCount) {
        GraphicController controller = GraphicController.getController();
        int iCompound = controller.askObject(GraphicObject.getTypeFromName(GraphicObjectProperties.__GO_COMPOUND__));
        Axes axes = (Axes) controller.getObjectFromId(parent);
        Compound compound = (Compound) controller.getObjectFromId(iCompound);

        Integer[] children = axes.getChildren();

        /*
         * Remove the last "number" created objects (located at the children list's head)
         * and add them to the compound in the same order
         */
        for (int i = 0 ; i < childrenCount ; i++) {
            /*
             * Set the parent-child relationship between the Compound and each aggregated object.
             * Children are added to the Compound from the least recent to the most recent, to
             * preserve their former ordering.
             */
            controller.setGraphicObjectRelationship(iCompound, children[childrenCount - i - 1]);
        }

        controller.setGraphicObjectRelationship(parent, iCompound);

        /*
         * visibility is obtained from the parent Figure, whereas it is retrieved from the
         * parent Axes in ConstructCompound.
         * To be made consistent.
         */
        Figure fig = (Figure)controller.getObjectFromId(axes.getParentFigure());
        compound.setVisible(fig.getVisible());
        return iCompound;
    }

    public static int createFec(int parent, double[] zminmax, int[] colminmax, int[] colout, boolean with_mesh) {

        GraphicController controller = GraphicController.getController();
        int iFec = controller.askObject(GraphicObject.getTypeFromName(GraphicObjectProperties.__GO_FEC__));
        Axes axes = (Axes) controller.getObjectFromId(parent);
        Fec fec = (Fec) controller.getObjectFromId(iFec);

        fec.setZBounds(zminmax);
        fec.setColorRange(colminmax);
        fec.setOutsideColors(colout);

        fec.setVisible(axes.getVisible());

        //clip box
        fec.setClipBox(axes.getClipBox());
        fec.setClipBoxSet(axes.getClipBoxSet());
        fec.setClipState(axes.getClipState());

        cloneGraphicContext(parent, iFec);

        fec.setLineMode(with_mesh);

        //setGraphicObjectRelationship stay in C code to do it the latest possible
        //after set on DATA_MODEL.

        //controller.setGraphicObjectRelationship(parent, iFec);

        return iFec;
    }

    public static int createGrayplot(int parent, int type, double[] pvecx, int n1, int n2) {

        int[] objectTypes = new int[] {GraphicObjectProperties.__GO_GRAYPLOT__, GraphicObjectProperties.__GO_MATPLOT__, GraphicObjectProperties.__GO_MATPLOT__};

        GraphicController controller = GraphicController.getController();
        int iPlot = controller.askObject(GraphicObject.getTypeFromName(objectTypes[type]));
        Axes axes = (Axes) controller.getObjectFromId(parent);
        Imageplot plot = (Imageplot) controller.getObjectFromId(iPlot);

        if (type == 2) { //Matplot1
            Double[] data = new Double[pvecx.length];
            for (int i = 0 ; i < pvecx.length ; i++) {
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

        return iPlot;
    }
}

