/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
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

package org.scilab.modules.gui.editor;

import java.lang.Math;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicModel.GraphicModel;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.axes.AxesContainer;
import org.scilab.modules.graphic_objects.ScilabNativeView;

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

    public enum axisTo { __X__, __Y__, __Z__, __TITLE__ };

    private static Integer lastAxes;

    private static Integer[] searchAxes(Integer uid) {
        return (new ObjectSearcher()).search(uid, GraphicObjectProperties.__GO_AXES__);
    }

    public static Axes getAxesFromUid(Integer uid) {
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
    public static Integer clickedAxes(Integer figure, Integer[] position) {
        Integer[] axes = searchAxes(figure);
        if (axes == null) {
            return lastAxes;
        }

        AxesContainer container = (AxesContainer) GraphicController.getController().getObjectFromId(figure);
        Integer[] figureSize = container.getAxesSize();

        for (Integer i = 0; i < axes.length; i++) {
            Double[] axesBound = (Double[])GraphicController.getController().getProperty(axes[i], GraphicObjectProperties.__GO_AXES_BOUNDS__);
            Double xi, xf, yi, yf;

            xi = (figureSize[0] * axesBound[0]);
            xf = (figureSize[0] * (axesBound[0] + axesBound[2]));
            yi = (figureSize[1] * axesBound[1]);
            yf = (figureSize[1] * (axesBound[1] + axesBound[3]));

            if (position[0] > xi && position[0] < xf && position[1] > yi && position[1] < yf) {
                lastAxes = axes[i];
                return axes[i];
            }
        }
        return lastAxes;
    }



    /**
     * Change the status from the axes to visible and the axis too
     *
     * @param axes Axes to set visible
     */
    public static void setAxesVisible(Integer axes) {

        GraphicController.getController().setProperty(axes, GraphicObjectProperties.__GO_X_AXIS_VISIBLE__, true);
        GraphicController.getController().setProperty(axes, GraphicObjectProperties.__GO_Y_AXIS_VISIBLE__, true);
        GraphicController.getController().setProperty(axes, GraphicObjectProperties.__GO_Z_AXIS_VISIBLE__, true);
        GraphicController.getController().setProperty(axes, GraphicObjectProperties.__GO_VISIBLE__, true);
        GraphicController.getController().setProperty(axes, GraphicObjectProperties.__GO_CLIP_STATE__, 1);
        GraphicController.getController().setProperty(axes, GraphicObjectProperties.__GO_X_TIGHT_LIMITS__, true);
        GraphicController.getController().setProperty(axes, GraphicObjectProperties.__GO_Y_TIGHT_LIMITS__, true);
        GraphicController.getController().setProperty(axes, GraphicObjectProperties.__GO_Z_TIGHT_LIMITS__, true);
        GraphicController.getController().setProperty(axes, GraphicObjectProperties.__GO_BOX_TYPE__, 1);
        GraphicController.getController().setProperty(axes, GraphicObjectProperties.__GO_FILLED__, true);
    }

    /**
     * Clone an axes and delete all their children's, change the labels from axes to empty
     *
     * @param axes The axes to duplicate
     * @return Return a duplicated axes without childrens
     */
    public static Integer duplicateAxes(Integer axes) {

        Integer newAxes = GraphicController.getController().cloneObject(axes);
        setAxesVisible(newAxes);
        PolylineHandler.getInstance().deleteAll(newAxes);
        return newAxes;
    }

    /**
     * Set the AxesTo bounds with the max bounds between axesFrom
     * and axesTo(used when paste new objects to make all object visible)
     *
     * @param axesFrom Axes from you want to merge the bounds
     * @param axesTo Axes that will be set the max bounds between the two axes
     */
    public static void axesBound(Integer axesFrom, Integer axesTo) {

        if (axesFrom == axesTo) {
            return;
        }

        Double[] axesFB = (Double[])GraphicController.getController().getProperty(axesFrom, GraphicObjectProperties.__GO_DATA_BOUNDS__);
        Double[] axesTB = (Double[])GraphicController.getController().getProperty(axesTo, GraphicObjectProperties.__GO_REAL_DATA_BOUNDS__);
        Double[] newBounds = {0., 0., 0., 0., 0., 0.};

        newBounds[0] = Math.min(axesFB[0], axesTB[0]);
        newBounds[1] = Math.max(axesFB[1], axesTB[1]);
        newBounds[2] = Math.min(axesFB[2], axesTB[2]);
        newBounds[3] = Math.max(axesFB[3], axesTB[3]);
        newBounds[4] = Math.min(axesFB[4], axesTB[4]);
        newBounds[5] = Math.max(axesFB[5], axesTB[5]);

        GraphicController.getController().setProperty(axesTo, GraphicObjectProperties.__GO_DATA_BOUNDS__, newBounds);

        Boolean x_tight_lim = (Boolean)GraphicController.getController().getProperty(axesFrom, GraphicObjectProperties.__GO_X_TIGHT_LIMITS__);
        GraphicController.getController().setProperty(axesTo, GraphicObjectProperties.__GO_X_TIGHT_LIMITS__, x_tight_lim);
        Boolean y_tight_lim = (Boolean)GraphicController.getController().getProperty(axesFrom, GraphicObjectProperties.__GO_Y_TIGHT_LIMITS__);
        GraphicController.getController().setProperty(axesTo, GraphicObjectProperties.__GO_Y_TIGHT_LIMITS__, y_tight_lim);
        Boolean z_tight_lim = (Boolean)GraphicController.getController().getProperty(axesFrom, GraphicObjectProperties.__GO_Z_TIGHT_LIMITS__);
        GraphicController.getController().setProperty(axesTo, GraphicObjectProperties.__GO_Z_TIGHT_LIMITS__, z_tight_lim);
    }

    /**
     * Checks if zoombox is enebled in the given axes.
     *
     * @param uid axes unique identifier.
     * @return True if enabled, false otherwise.
     */
    public static Boolean isZoomBoxEnabled(Integer uid) {
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
    public static Boolean isInZoomBoxBounds(Integer uid, double x, double y) {
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
    public static boolean isAxesNotBlank(Integer figure) {

        Integer[] axes = searchAxes(figure);
        if (axes == null) {
            return false;
        }
        boolean flag = false;
        for ( Integer j = 0; j < axes.length; j++) {

            Integer childCount = (Integer)GraphicController.getController().getProperty(axes[j], GraphicObjectProperties.__GO_CHILDREN_COUNT__);
            Integer[] child = (Integer[])GraphicController.getController().getProperty(axes[j], GraphicObjectProperties.__GO_CHILDREN__);
            for (Integer i = 0; i < childCount; i++) {
                flag = isBlank(child[i]);
                if (flag) {
                    return flag;
                }
            }
        }
        return false;
    }

    public static boolean isAxesEmpty(Integer axes) {

        Integer childCount = (Integer)GraphicController.getController().getProperty(axes, GraphicObjectProperties.__GO_CHILDREN_COUNT__);
        Integer[] child = (Integer[])GraphicController.getController().getProperty(axes, GraphicObjectProperties.__GO_CHILDREN__);
        for (Integer i = 0; i < childCount; i++) {
            Integer type = (Integer)GraphicController.getController().getProperty(child[i], GraphicObjectProperties.__GO_TYPE__);
            if (type != GraphicObjectProperties.__GO_LABEL__) {
                return false;
            }
        }
        return true;
    }

    /**
     * Checks if the given object is visible or if it has child check its children.
     *
     * @param objectID Object unique identifier.
     * @return True if there is any object visible, false otherwise.
     */
    private static boolean isBlank(Integer objectID) {

        Integer type = (Integer)GraphicController.getController().getProperty(objectID, GraphicObjectProperties.__GO_TYPE__);
        boolean flag = (Boolean) GraphicController.getController().getProperty(objectID, GraphicObjectProperties.__GO_VISIBLE__);
        if (flag) {
            if (type != GraphicObjectProperties.__GO_LABEL__ && type != GraphicObjectProperties.__GO_COMPOUND__) {
                return flag;
            } else if (type == GraphicObjectProperties.__GO_COMPOUND__) {
                Integer childCount = (Integer)GraphicController.getController().getProperty(objectID, GraphicObjectProperties.__GO_CHILDREN_COUNT__);
                Integer[] child = (Integer[])GraphicController.getController().getProperty(objectID, GraphicObjectProperties.__GO_CHILDREN__);
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

    public static void pasteRotationAngles(Integer obj, Integer figure, Integer[] pos) {

        Integer axesFrom = (new ObjectSearcher()).searchParent(obj, GraphicObjectProperties.__GO_AXES__);
        Integer axesTo = AxesHandler.clickedAxes(figure, pos);

        Double[] angles = (Double[])GraphicController.getController().getProperty(axesFrom, GraphicObjectProperties.__GO_ROTATION_ANGLES__);
        GraphicController.getController().setProperty(axesTo, GraphicObjectProperties.__GO_ROTATION_ANGLES__, angles);
    }

    /**
    * Clone an axes with the same properties from the original
    *
    * @param axesFromUID The uid from the axes to clone
    * @return The cloned axes UID
    */
    public static Integer cloneAxesWithStyle(Integer axesFromUID) {

        Integer cloneUID = GraphicController.getController().cloneObject(axesFromUID);
        Axes clone = (Axes)GraphicController.getController().getObjectFromId(cloneUID);
        Axes axesFrom = (Axes)GraphicController.getController().getObjectFromId(axesFromUID);

        //Cloning the Axes does not clone the labels/title, so we need do it manually

        Integer xLabelUID = GraphicController.getController().cloneObject(axesFrom.getXAxisLabel());
        Integer yLabelUID = GraphicController.getController().cloneObject(axesFrom.getYAxisLabel());
        Integer zLabelUID = GraphicController.getController().cloneObject(axesFrom.getZAxisLabel());
        Integer titleUID = GraphicController.getController().cloneObject(axesFrom.getTitle());

        GraphicController.getController().setGraphicObjectRelationship(cloneUID, xLabelUID);
        GraphicController.getController().setGraphicObjectRelationship(cloneUID, yLabelUID);
        GraphicController.getController().setGraphicObjectRelationship(cloneUID, zLabelUID);
        GraphicController.getController().setGraphicObjectRelationship(cloneUID, titleUID);

        //Set the correct position to the labels/title in the new Axes

        GraphicController.getController().setProperty(xLabelUID, GraphicObjectProperties.__GO_AUTO_POSITION__, true);
        GraphicController.getController().setProperty(yLabelUID, GraphicObjectProperties.__GO_AUTO_POSITION__, true);
        GraphicController.getController().setProperty(zLabelUID, GraphicObjectProperties.__GO_AUTO_POSITION__, true);
        GraphicController.getController().setProperty(titleUID, GraphicObjectProperties.__GO_AUTO_POSITION__, true);

        Double[] margins = axesFrom.getMargins();
        Integer boxType = axesFrom.getBoxType();
        boolean markMode = axesFrom.getMarkMode();
        Integer clipState = axesFrom.getClipState();
        Integer markSize = axesFrom.getMarkSize();
        Integer markStyle = axesFrom.getMarkStyle();
        Integer markForeground = axesFrom.getMarkForeground();
        Integer markBackground = axesFrom.getMarkBackground();
        boolean lineMode = axesFrom.getLineMode();
        Integer lineStyle = axesFrom.getLineStyle();
        Double lineThickness = axesFrom.getLineThickness();
        Integer lineColor = axesFrom.getLineColor();
        Double[] rotationAngles = axesFrom.getRotationAngles();
        boolean XAxisVisible = axesFrom.getXAxisVisible();
        boolean YAxisVisible = axesFrom.getYAxisVisible();
        boolean ZAxisVisible = axesFrom.getZAxisVisible();
        boolean XAxisReverse = axesFrom.getXAxisReverse();
        boolean YAxisReverse = axesFrom.getYAxisReverse();
        boolean ZAxisReverse = axesFrom.getZAxisReverse();
        Integer XAxisGridColor = axesFrom.getXAxisGridColor();
        Integer YAxisGridColor = axesFrom.getYAxisGridColor();
        Integer ZAxisGridColor = axesFrom.getZAxisGridColor();
        Integer fontStyle = axesFrom.getFontStyle();
        Double fontSize = axesFrom.getFontSize();
        Integer fontColor = axesFrom.getFontColor();
        boolean fontFractional = axesFrom.getFontFractional();
        Integer hiddenColor = axesFrom.getHiddenColor();
        boolean XTightLimits = axesFrom.getXTightLimits();
        boolean YTightLimits = axesFrom.getYTightLimits();
        boolean ZTightLimits = axesFrom.getZTightLimits();

        clone.setXAxisLabel(xLabelUID);
        clone.setYAxisLabel(yLabelUID);
        clone.setZAxisLabel(zLabelUID);
        clone.setTitle(titleUID);

        clone.setMargins(margins);
        clone.setBoxType(boxType);
        clone.setClipState(clipState);
        clone.setMarkMode(markMode);
        clone.setMarkStyle(markStyle);
        clone.setMarkSize(markSize);
        clone.setMarkBackground(markBackground);
        clone.setMarkForeground(markForeground);
        clone.setLineMode(lineMode);
        clone.setLineStyle(lineStyle);
        clone.setLineThickness(lineThickness);
        clone.setLineColor(lineColor);


        clone.setRotationAngles(rotationAngles);
        clone.setXAxisVisible(XAxisVisible);
        clone.setYAxisVisible(YAxisVisible);
        clone.setZAxisVisible(ZAxisVisible);
        clone.setXAxisReverse(XAxisReverse);
        clone.setYAxisReverse(YAxisReverse);
        clone.setZAxisReverse(ZAxisReverse);
        clone.setXAxisGridColor(XAxisGridColor);
        clone.setYAxisGridColor(YAxisGridColor);
        clone.setZAxisGridColor(ZAxisGridColor);
        clone.setFontStyle(fontStyle);
        clone.setFontSize(fontSize);
        clone.setFontColor(fontColor);
        clone.setFontFractional(fontFractional);
        clone.setHiddenColor(hiddenColor);
        clone.setXTightLimits(XTightLimits);
        clone.setYTightLimits(YTightLimits);
        clone.setZTightLimits(ZTightLimits);

        return cloneUID;
    }

    /**
    * Cuts the original from the figure, paste the new axes in the figure and copy
    * the childrens, data bounds and title from the original axes to the new one
    *
    * @param newAxesUID The uid from the new axes to paste
    * @param axesToUID The uid from the axes to be cut
    */
    public static void pasteAxesStyle(Integer newAxesUID, Integer axesToUID, boolean bounds) {

        if (newAxesUID == null || axesToUID == null) {
            return;
        }
        Axes newAxes = (Axes)GraphicController.getController().getObjectFromId(newAxesUID);
        Axes axesTo = (Axes)GraphicController.getController().getObjectFromId(axesToUID);

        Integer[] children = axesTo.getChildren();
        Integer parentUID = axesTo.getParent();
        String[] titleText = (String[])GraphicController.getController().getProperty(axesTo.getTitle(), GraphicObjectProperties.__GO_TEXT_STRINGS__);
        String[] xLabelText = (String[])GraphicController.getController().getProperty(axesTo.getXAxisLabel(), GraphicObjectProperties.__GO_TEXT_STRINGS__);
        String[] yLabelText = (String[])GraphicController.getController().getProperty(axesTo.getYAxisLabel(), GraphicObjectProperties.__GO_TEXT_STRINGS__);
        String[] zLabelText = (String[])GraphicController.getController().getProperty(axesTo.getZAxisLabel(), GraphicObjectProperties.__GO_TEXT_STRINGS__);

        GraphicController.getController().setProperty(newAxes.getTitle(), GraphicObjectProperties.__GO_TEXT_STRINGS__, titleText);
        GraphicController.getController().setProperty(newAxes.getXAxisLabel(), GraphicObjectProperties.__GO_TEXT_STRINGS__, xLabelText);
        GraphicController.getController().setProperty(newAxes.getYAxisLabel(), GraphicObjectProperties.__GO_TEXT_STRINGS__, yLabelText);
        GraphicController.getController().setProperty(newAxes.getZAxisLabel(), GraphicObjectProperties.__GO_TEXT_STRINGS__, zLabelText);

        if (bounds) {
            Double[] dataBounds = axesTo.getDataBounds();
            Double[] realDataBounds = axesTo.getRealDataBounds();
            newAxes.setDataBounds(dataBounds);
            newAxes.setRealDataBounds(realDataBounds);
        }

        for (Integer i = 0; i < children.length; i++) {
            GraphicController.getController().setGraphicObjectRelationship(newAxesUID, children[i]);
        }
        GraphicController.getController().setGraphicObjectRelationship(parentUID, newAxesUID);
        GraphicController.getController().setGraphicObjectRelationship(0, axesToUID);
        ScilabNativeView.ScilabNativeView__setCurrentSubWin(newAxesUID);
    }
}

