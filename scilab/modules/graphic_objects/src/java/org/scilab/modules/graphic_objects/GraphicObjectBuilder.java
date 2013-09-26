/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 * Copyright (C) 2013 - Scilab Enterprises - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.graphic_objects;

import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicModel.GraphicModel;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

/**
 * This is a static class to access all controller capabilities
 * from C/C++ code through JNI
 * See SCI/modules/graphic_objects/src/jni/graphic_objects.giws.xml for other details.
 **/
public final class GraphicObjectBuilder {

    public static Figure createNewFigureWithAxes(Integer id) {
        GraphicController controller = GraphicController.getController();
        
        final int[] props = new int[] {GraphicObjectProperties.__GO_X_AXIS_LABEL__, GraphicObjectProperties.__GO_Y_AXIS_LABEL__, GraphicObjectProperties.__GO_Z_AXIS_LABEL__, GraphicObjectProperties.__GO_TITLE__};
        String figureModelUID = GraphicModel.getModel().getFigureModel().getIdentifier();
        String axesModelUID = GraphicModel.getModel().getAxesModel().getIdentifier();
        String newFID = controller.createUID().toString();
        String newAID = controller.createUID().toString();
        GraphicModel.getModel().cloneObject(figureModelUID, newFID);
        GraphicModel.getModel().cloneObject(axesModelUID, newAID);

        controller.setProperty(newFID, GraphicObjectProperties.__GO_ID__, id);
        controller.setGraphicObjectRelationship(newFID, newAID);
        controller.setProperty(newFID, GraphicObjectProperties.__GO_SELECTED_CHILD__, newAID);

        for (Integer type : props) {
            final double[] position = new double[] {1, 1, 1};
            String modelLabelUID = (String) controller.getProperty(axesModelUID, type);
            String pobjUID = controller.createUID().toString();
            GraphicModel.getModel().cloneObject(modelLabelUID, pobjUID);
            controller.setProperty(pobjUID, GraphicObjectProperties.__GO_POSITION__, position);

            Boolean autoPosition = (Boolean ) controller.getProperty(modelLabelUID, GraphicObjectProperties.__GO_AUTO_POSITION__);
            controller.setProperty(pobjUID, GraphicObjectProperties.__GO_AUTO_POSITION__, autoPosition);

            controller.setProperty(newAID, type, pobjUID);
            CallGraphicController.setGraphicObjectRelationship(newAID, pobjUID);
        }

        ScilabNativeView.ScilabNativeView__createObject(newFID);
        ScilabNativeView.ScilabNativeView__updateObject(newFID, GraphicObjectProperties.__GO_ID__);
        ScilabNativeView.ScilabNativeView__setCurrentObject(newAID);
        ScilabNativeView.ScilabNativeView__setCurrentSubWin(newAID);
        ScilabNativeView.ScilabNativeView__setCurrentFigure(newFID);

        Integer[] axesSize = (Integer[]) controller.getProperty(figureModelUID, GraphicObjectProperties.__GO_AXES_SIZE__);
        controller.setProperty(newFID, GraphicObjectProperties.__GO_AXES_SIZE__, axesSize);

        return (Figure) controller.getObjectFromId(newFID);
    }

    public static String constructRectangles(String pparentsubwinUID, double x, double y, double height, double width, int foreground, int background, int isfilled, int isline) {
        GraphicController controller = GraphicController.getController();
        
        Double[] clipRegion;
        Boolean visible = false;
        Boolean clipRegionSet = false;
        Integer clipState = 0;
        Boolean iMarkMode = false;

        if (height < 0 || width < 0) {
            return null;
        }

        String pobjUID = CallGraphicController.askGraphicObject(GraphicObjectProperties.__GO_RECTANGLE__);

        /*
         * Sets the rectangle's parent in order to initialize the former's Contoured properties
         * with the latter's values (cloneGraphicContext call below)
         */
        //setGraphicObjectProperty(pobjUID, __GO_PARENT__, pparentsubwinUID, jni_string, 1);

        controller.setProperty(pobjUID, GraphicObjectProperties.__GO_UPPER_LEFT_POINT__, new Double[] {x, y, 0.0});
        controller.setProperty(pobjUID, GraphicObjectProperties.__GO_HEIGHT__, height);
        controller.setProperty(pobjUID, GraphicObjectProperties.__GO_WIDTH__, width);

        visible = (Boolean) controller.getProperty(pparentsubwinUID, GraphicObjectProperties.__GO_VISIBLE__);
        controller.setProperty(pobjUID, GraphicObjectProperties.__GO_VISIBLE__, visible);

        /* Clip state and region */
        /* To be checked for consistency */

        clipRegion = (Double[]) controller.getProperty(pparentsubwinUID, GraphicObjectProperties.__GO_CLIP_BOX__);
        controller.setProperty(pobjUID, GraphicObjectProperties.__GO_CLIP_BOX__, clipRegion);

        clipRegionSet = (Boolean) controller.getProperty(pparentsubwinUID, GraphicObjectProperties.__GO_CLIP_BOX_SET__);
        controller.setProperty(pobjUID, GraphicObjectProperties.__GO_CLIP_BOX_SET__,  clipRegionSet);

        clipState = (Integer) controller.getProperty(pparentsubwinUID, GraphicObjectProperties.__GO_CLIP_STATE__);
        controller.setProperty(pobjUID, GraphicObjectProperties.__GO_CLIP_STATE__, clipState);

        iMarkMode = (Boolean) controller.getProperty(pparentsubwinUID, GraphicObjectProperties.__GO_MARK_MODE__);
        controller.setProperty(pobjUID, GraphicObjectProperties.__GO_MARK_MODE__, iMarkMode);

        /*
         * Initializes the contour properties (background, foreground, etc)
         * to the default values (those of the parent Axes).
         */
        cloneGraphicContext(pparentsubwinUID, pobjUID);

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

    public static int cloneGraphicContext(String sourceIdentifier, String destIdentifier) {
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
}
