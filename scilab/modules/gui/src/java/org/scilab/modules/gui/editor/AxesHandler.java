/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.editor;

import java.lang.Math;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.axes.Axes;

import org.scilab.modules.gui.editor.ObjectSearcher;
import org.scilab.modules.gui.editor.PolylineHandler;




/**
* Implements all axes manipulation functions for the editor.
*
* @author Caio Souza <caioc2bolado@gmail.com>
* @author Pedro Souza <bygrandao@gmail.com>
*
* @since 2012-06-01
*/
public class AxesHandler {

    public enum axisTo { __X__, __Y__, __Z__ };

    private static String[] searchAxes(String uid) {
        return (new ObjectSearcher()).search(uid, GraphicObjectProperties.__GO_AXES__);
    }


    public static Axes getAxesFromUid(String uid) {
        return (Axes)GraphicController.getController().getObjectFromId(uid);
    }


    /**
    * Given a mouse coordinate point (x, y) in pixels
    * retrieves the axes wich the point belongs.
    *
    * @param figure Figure unique identifier.
    * @param position Vector with mouse position x and y.
    * @return Retrieved axes or null if there isn't an axes.
    */
    public static String clickedAxes(String figure, Integer[] position) {

        String[] axes = searchAxes(figure);
        if (axes == null) {
            return null;
        }
        Integer[] figureSize = (Integer[])GraphicController.getController().getProperty(figure, GraphicObjectProperties.__GO_AXES_SIZE__);

        for (Integer i = 0; i < axes.length; i++) {
            Double[] axesBound = (Double[])GraphicController.getController().getProperty(axes[i], GraphicObjectProperties.__GO_AXES_BOUNDS__);
            Double xi, xf, yi, yf;

            xi = (figureSize[0] * axesBound[0]);
            xf = (figureSize[0] * (axesBound[0] + axesBound[2]));
            yi = (figureSize[1] * axesBound[1]);
            yf = (figureSize[1] * (axesBound[1] + axesBound[3]));

            if (position[0] > xi && position[0] < xf && position[1] > yi && position[1] < yf) {
                return axes[i];
            }
        }
        return null;
    }



    /**
     * Change the status from the axes to visible and the axis too
     *
     * @param axes Axes to set visible
     */
    public static void setAxesVisible(String axes) {

        GraphicController.getController().setProperty(axes, GraphicObjectProperties.__GO_X_AXIS_VISIBLE__, true);
        GraphicController.getController().setProperty(axes, GraphicObjectProperties.__GO_Y_AXIS_VISIBLE__, true);
        GraphicController.getController().setProperty(axes, GraphicObjectProperties.__GO_Z_AXIS_VISIBLE__, true);
        GraphicController.getController().setProperty(axes, GraphicObjectProperties.__GO_VISIBLE__, true);
        GraphicController.getController().setProperty(axes, GraphicObjectProperties.__GO_CLIP_STATE__, 1);
        GraphicController.getController().setProperty(axes, GraphicObjectProperties.__GO_TIGHT_LIMITS__, true);
        GraphicController.getController().setProperty(axes, GraphicObjectProperties.__GO_BOX_TYPE__, 1);
        GraphicController.getController().setProperty(axes, GraphicObjectProperties.__GO_FILLED__, true);
    }

    /**
     * Clone an axes and delete all their children's, change the labels from axes to empty
     *
     * @param axes The axes to duplicate
     * @return Return a duplicated axes without childrens
     */
    public static String duplicateAxes(String axes) {

        String newAxes = GraphicController.getController().cloneObject(axes);
        setAxesVisible(newAxes);
        PolylineHandler.getInstance().deleteAll(newAxes);
        return newAxes;
    }

    /**
     * Set the AxesTo bounds with the max bounds between axesFrom and axesTo(used when paste new objects to make all object visible)
     *
     * @param axesFrom Axes from you want to merge the bounds
     * @param axesTo Axes that will be set the max bounds between the two axes
     */
    public static void axesBound(String axesFrom, String axesTo) {

        Double[] axesFB = (Double[])GraphicController.getController().getProperty(axesFrom, GraphicObjectProperties.__GO_DATA_BOUNDS__);
        Double[] axesTB = (Double[])GraphicController.getController().getProperty(axesTo, GraphicObjectProperties.__GO_DATA_BOUNDS__);
        Double[] newBounds = {0., 0., 0., 0., 0., 0.};

        newBounds[0] = Math.min(axesFB[0], axesTB[0]);
        newBounds[1] = Math.max(axesFB[1], axesTB[1]);
        newBounds[2] = Math.min(axesFB[2], axesTB[2]);
        newBounds[3] = Math.max(axesFB[3], axesTB[3]);
        newBounds[4] = Math.min(axesFB[4], axesTB[4]);
        newBounds[5] = Math.max(axesFB[5], axesTB[5]);

        GraphicController.getController().setProperty(axesTo, GraphicObjectProperties.__GO_DATA_BOUNDS__, newBounds);
    }

    /**
     * Checks if zoombox is enebled in the given axes.
     *
     * @param uid axes unique identifier.
     * @return True if enabled, false otherwise.
     */
    public static Boolean isZoomBoxEnabled(String uid) {
        return (Boolean)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_ZOOM_ENABLED__);
    }

    /**
     * Checks if the given point (x,y) is in zoombox bounds.
     *
     * @param uid Axes unique identifier.
     * @param x position on x axis.
     * @param y position on y axis.
     * @return True if the point is within the bounds, false otherwise.
     */
    public static Boolean isInZoomBoxBounds(String uid, double x, double y) {
        Double[] bounds = (Double[])GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_ZOOM_BOX__);

        if (x >= bounds[0] && x <= bounds[1]) {
            if (y >= bounds[2] && y <= bounds[3]) {
                return true;
            }
        }
        return false;
    }

    /**
     * Checks if the given figure have any object visible.
     *
     * @param figure Figure unique identifier.
     * @return True if there is any object visible, false otherwise.
     */
    public static boolean isAxesNotBlank(String figure) {

        String[] axes = searchAxes(figure);
        if (axes == null) {
            return false;
        }
        boolean flag = false;
        for ( Integer j = 0; j < axes.length; j++) {

            Integer childCount = (Integer)GraphicController.getController().getProperty(axes[j], GraphicObjectProperties.__GO_CHILDREN_COUNT__);
            String[] child = (String[])GraphicController.getController().getProperty(axes[j], GraphicObjectProperties.__GO_CHILDREN__);
            for (Integer i = 0; i < childCount; i++) {
                flag = isBlank(child[i]);
                if (flag) {
                    return flag;
                }
            }
        }
        return false;
    }

    /**
     * Checks if the given object is visible or if it has child check its children.
     *
     * @param objectID Object unique identifier.
     * @return True if there is any object visible, false otherwise.
     */
    private static boolean isBlank(String objectID) {

        String type = (String)GraphicController.getController().getProperty(objectID, GraphicObjectProperties.__GO_TYPE__);
        boolean flag = (Boolean) GraphicController.getController().getProperty(objectID, GraphicObjectProperties.__GO_VISIBLE__);
        if (flag) {

            if (type != GraphicObjectProperties.__GO_LABEL__ && type != GraphicObjectProperties.__GO_COMPOUND__) {
                return flag;
            } else if (type == GraphicObjectProperties.__GO_COMPOUND__) {
                Integer childCount = (Integer)GraphicController.getController().getProperty(objectID, GraphicObjectProperties.__GO_CHILDREN_COUNT__);
                String[] child = (String[])GraphicController.getController().getProperty(objectID, GraphicObjectProperties.__GO_CHILDREN__);
                for (Integer i = 0; i < childCount; i++) {
                    flag = (Boolean) GraphicController.getController().getProperty(child[i], GraphicObjectProperties.__GO_VISIBLE__);
                    if (flag) {
                        return flag;
                    }
                }
            }
        }
        return false;
    }

}

