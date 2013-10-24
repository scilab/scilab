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
        Integer figureModelUID = GraphicModel.getFigureModel().getIdentifier();
        Integer axesModelUID = GraphicModel.getAxesModel().getIdentifier();
        Integer newFID = controller.createUID();
        Integer newAID = controller.createUID();
        GraphicModel.getModel().cloneObject(figureModelUID, newFID);
        GraphicModel.getModel().cloneObject(axesModelUID, newAID);

        controller.setProperty(newFID, GraphicObjectProperties.__GO_ID__, id);
        controller.setGraphicObjectRelationship(newFID, newAID);
        controller.setProperty(newFID, GraphicObjectProperties.__GO_SELECTED_CHILD__, newAID);

        for (Integer type : props) {
            final double[] position = new double[] {1, 1, 1};
            Integer modelLabelUID = (Integer) controller.getProperty(axesModelUID, type);
            Integer pobjUID = controller.createUID();
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
}
