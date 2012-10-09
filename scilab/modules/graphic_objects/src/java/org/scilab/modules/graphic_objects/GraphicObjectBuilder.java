/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

/**
 * This is a static class to access all controller capabilities
 * from C/C++ code through JNI
 * See SCI/modules/graphic_objects/src/jni/graphic_objects.giws.xml for other details.
 **/
public final class GraphicObjectBuilder {

    public static String constructRectangles(String pparentsubwinUID, double x, double y, double height, double width, int foreground, int background, int isfilled, int isline) {
        double[] clipRegion;
        int visible = 0;
        int clipRegionSet = 0;
        int clipState = 0;
        int iMarkMode = 0;

        if (height < 0 || width < 0)
        {
            return null;
        }

        String pobjUID = CallGraphicController.askGraphicObject(GraphicObjectProperties.__GO_RECTANGLE__);

        /*
         * Sets the rectangle's parent in order to initialize the former's Contoured properties
         * with the latter's values (cloneGraphicContext call below)
         */
        //setGraphicObjectProperty(pobjUID, __GO_PARENT__, pparentsubwinUID, jni_string, 1);

        CallGraphicController.setGraphicObjectProperty(pobjUID, GraphicObjectProperties.__GO_UPPER_LEFT_POINT__, new double[]{x, y, 0});
        CallGraphicController.setGraphicObjectProperty(pobjUID, GraphicObjectProperties.__GO_HEIGHT__, height);
        CallGraphicController.setGraphicObjectProperty(pobjUID, GraphicObjectProperties.__GO_WIDTH__, width);

        visible = CallGraphicController.getGraphicObjectPropertyAsBoolean(pparentsubwinUID, GraphicObjectProperties.__GO_VISIBLE__);
        CallGraphicController.setGraphicObjectProperty(pobjUID, GraphicObjectProperties.__GO_VISIBLE__, visible == 1);

        /* Clip state and region */
        /* To be checked for consistency */

        clipRegion = CallGraphicController.getGraphicObjectPropertyAsDoubleVector(pparentsubwinUID, GraphicObjectProperties.__GO_CLIP_BOX__);
        CallGraphicController.setGraphicObjectProperty(pobjUID, GraphicObjectProperties.__GO_CLIP_BOX__, clipRegion);

        clipRegionSet = CallGraphicController.getGraphicObjectPropertyAsBoolean(pparentsubwinUID, GraphicObjectProperties.__GO_CLIP_BOX_SET__);
        CallGraphicController.setGraphicObjectProperty(pobjUID, GraphicObjectProperties.__GO_CLIP_BOX_SET__,  clipRegionSet == 1);

        clipState = CallGraphicController.getGraphicObjectPropertyAsInteger(pparentsubwinUID, GraphicObjectProperties.__GO_CLIP_STATE__);
        CallGraphicController.setGraphicObjectProperty(pobjUID, GraphicObjectProperties.__GO_CLIP_STATE__, clipState);

        iMarkMode = CallGraphicController.getGraphicObjectPropertyAsBoolean(pparentsubwinUID, GraphicObjectProperties.__GO_MARK_MODE__);
        CallGraphicController.setGraphicObjectProperty(pobjUID, GraphicObjectProperties.__GO_MARK_MODE__, iMarkMode == 1);

        /*
         * Initializes the contour properties (background, foreground, etc)
         * to the default values (those of the parent Axes).
         */
        cloneGraphicContext(pparentsubwinUID, pobjUID);

        /* Contour settings */
        CallGraphicController.setGraphicObjectProperty(pobjUID, GraphicObjectProperties.__GO_LINE_MODE__, isline == 1);
        CallGraphicController.setGraphicObjectProperty(pobjUID, GraphicObjectProperties.__GO_FILL_MODE__, isfilled == 1);

        if (foreground != -1)
        {
            CallGraphicController.setGraphicObjectProperty(pobjUID, GraphicObjectProperties.__GO_LINE_COLOR__, foreground);
        }

        if (background != -1)
        {
            CallGraphicController.setGraphicObjectProperty(pobjUID, GraphicObjectProperties.__GO_BACKGROUND__, background);
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
        int lineMode = 0;
        int foreground = 0;
        int lineStyle = 0;
        int background = 0;
        int markForeground = 0;
        int markBackground = 0;
        int markStyle = 0;
        int markSize = 0;
        int markSizeUnit = 0;
        double lineThickness = 0.;

        /*
         * All these properties are passed by value thus do not care to release them
         * and do not call releaseGraphicObjectProperty on purpose.
         */

        lineMode = CallGraphicController.getGraphicObjectPropertyAsBoolean(sourceIdentifier, GraphicObjectProperties.__GO_LINE_MODE__);
        foreground = CallGraphicController.getGraphicObjectPropertyAsInteger(sourceIdentifier, GraphicObjectProperties.__GO_LINE_COLOR__);
        lineThickness = CallGraphicController.getGraphicObjectPropertyAsDouble(sourceIdentifier, GraphicObjectProperties.__GO_LINE_THICKNESS__);
        lineStyle = CallGraphicController.getGraphicObjectPropertyAsInteger(sourceIdentifier, GraphicObjectProperties.__GO_LINE_STYLE__);

        background = CallGraphicController.getGraphicObjectPropertyAsInteger(sourceIdentifier, GraphicObjectProperties.__GO_BACKGROUND__);

        markForeground = CallGraphicController.getGraphicObjectPropertyAsInteger(sourceIdentifier, GraphicObjectProperties.__GO_MARK_FOREGROUND__);

        markBackground = CallGraphicController.getGraphicObjectPropertyAsInteger(sourceIdentifier, GraphicObjectProperties.__GO_MARK_BACKGROUND__);

        markStyle = CallGraphicController.getGraphicObjectPropertyAsInteger(sourceIdentifier, GraphicObjectProperties.__GO_MARK_STYLE__);

        markSize = CallGraphicController.getGraphicObjectPropertyAsInteger(sourceIdentifier, GraphicObjectProperties.__GO_MARK_SIZE__);

        markSizeUnit = CallGraphicController.getGraphicObjectPropertyAsInteger(sourceIdentifier, GraphicObjectProperties.__GO_MARK_SIZE_UNIT__);

        CallGraphicController.setGraphicObjectProperty(destIdentifier, GraphicObjectProperties.__GO_LINE_MODE__, lineMode == 1);
        CallGraphicController.setGraphicObjectProperty(destIdentifier, GraphicObjectProperties.__GO_LINE_COLOR__, foreground);
        CallGraphicController.setGraphicObjectProperty(destIdentifier, GraphicObjectProperties.__GO_LINE_THICKNESS__, lineThickness);
        CallGraphicController.setGraphicObjectProperty(destIdentifier, GraphicObjectProperties.__GO_LINE_STYLE__, lineStyle);

        CallGraphicController.setGraphicObjectProperty(destIdentifier, GraphicObjectProperties.__GO_BACKGROUND__, background);

        CallGraphicController.setGraphicObjectProperty(destIdentifier, GraphicObjectProperties.__GO_MARK_FOREGROUND__, markForeground);
        CallGraphicController.setGraphicObjectProperty(destIdentifier, GraphicObjectProperties.__GO_MARK_BACKGROUND__, markBackground);
        CallGraphicController.setGraphicObjectProperty(destIdentifier, GraphicObjectProperties.__GO_MARK_STYLE__, markStyle);
        CallGraphicController.setGraphicObjectProperty(destIdentifier, GraphicObjectProperties.__GO_MARK_SIZE__, markSize);
        CallGraphicController.setGraphicObjectProperty(destIdentifier, GraphicObjectProperties.__GO_MARK_SIZE_UNIT__, markSizeUnit);

        return 0;
    }
}
