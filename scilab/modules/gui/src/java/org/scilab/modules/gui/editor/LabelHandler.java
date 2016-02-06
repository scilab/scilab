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

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.editor.AxesHandler;
import org.scilab.modules.gui.editor.CommonHandler;

import org.scilab.modules.renderer.CallRenderer;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.renderer.JoGLView.axes.AxesDrawer;
import java.lang.Math;

/**
* Implements all Label manipulation functions for the editor.
*
* @author Caio Souza <caioc2bolado@gmail.com>
* @author Pedro Souza <bygrandao@gmail.com>
*
* @since 2012-06-01
*/
public class LabelHandler {

    static AxesHandler.axisTo axis;
    static Integer axes;

    /**
     * Set the text of label x, y or z.
     *
     * @param axes The axes that will recieve the label.
     * @param text An array of text to set.
     * @param axis The axis to set x, y or z.
     * @return The Label that was setted
     */

    public static Integer setLabel(Integer axes, String[] text, AxesHandler.axisTo axis) {

        Integer label = null;

        switch (axis) {
            case __X__:
                label = (Integer)GraphicController.getController().getProperty(axes, GraphicObjectProperties.__GO_X_AXIS_LABEL__);
                break;
            case __Y__:
                label = (Integer)GraphicController.getController().getProperty(axes, GraphicObjectProperties.__GO_Y_AXIS_LABEL__);
                break;
            case __Z__:
                label = (Integer)GraphicController.getController().getProperty(axes, GraphicObjectProperties.__GO_Z_AXIS_LABEL__);
                break;
            case __TITLE__:
                label = (Integer)GraphicController.getController().getProperty(axes, GraphicObjectProperties.__GO_TITLE__);
                break;
            default:
                return null;
        }
        if (label != null) {
            GraphicController.getController().setProperty(label, GraphicObjectProperties.__GO_TEXT_STRINGS__, text);
        }
        return label;
    }

    /**
     * Get the text of the label x, y or z.
     *
     * @param axes The axes where is the label.
     * @param axis The axis of the label (x, y or z).
     * @return The text of the label.
     */

    public static String getLabelText(Integer axes, AxesHandler.axisTo axis) {

        Integer label = null;
        switch (axis) {
            case __X__:
                label = (Integer)GraphicController.getController().getProperty(axes, GraphicObjectProperties.__GO_X_AXIS_LABEL__);
                break;
            case __Y__:
                label = (Integer)GraphicController.getController().getProperty(axes, GraphicObjectProperties.__GO_Y_AXIS_LABEL__);
                break;
            case __Z__:
                label = (Integer)GraphicController.getController().getProperty(axes, GraphicObjectProperties.__GO_Z_AXIS_LABEL__);
                break;
            case __TITLE__:
                label = (Integer)GraphicController.getController().getProperty(axes, GraphicObjectProperties.__GO_TITLE__);
                break;
            default:
                return null;
        }
        String[] text = (String[])GraphicController.getController().getProperty(label, GraphicObjectProperties.__GO_TEXT_STRINGS__);
        if (text != null) {
            return text[0];
        }
        return null;
    }

    /**
    * Given a figure, a initial mouse position and a final mouse position, it gets the clicked label and moves it to the final position
    * PS: It is not a free move, it slides the label over the current axis
    *
    * @param figure The figure to drag the label
    * @param pos The initial click position
    * @param nextPos The final click position
    * @param update Control if need update the label / Axes, false if the mouse still pressed/dragging, true otherwise
    */
    public static void dragLabel(Integer figure, Integer[] pos, Integer[] nextPos, boolean update) {


        if (update) {
            axis = EntityPicker.pickLabel(figure, pos);
            axes = AxesHandler.clickedAxes(figure, pos);
        }
        if (axes == null || axis == null) {
            return;
        }

        Integer label = null;
        Axes ax = AxesHandler.getAxesFromUid(axes);
        boolean scale;
        Double[] labelPos;
        double[] pixBotton  = { 0., 0., 0.}, pixTop = { 0., 0., 0.};
        Double[] bounds = ax.getDataBounds();
        Double  width, height, length, delta, min, max, temp;
        int flag;

        switch (axis) {
            case __X__:
                label = ax.getXAxisLabel();
                scale = ax.getXAxisLogFlag();
                break;
            case __Y__:
                label = ax.getYAxisLabel();
                scale = ax.getYAxisLogFlag();
                break;
            case __Z__:
                label = ax.getZAxisLabel();
                scale = ax.getZAxisLogFlag();
                break;
            case __TITLE__:
                return;
            default:
                return;
        }

        labelPos  = (Double[])GraphicController.getController().getProperty(label, GraphicObjectProperties.__GO_POSITION__);

        switch (axis) {
            case __X__:
                pixBotton[0] = bounds[0];
                pixBotton[1] = labelPos[1];
                pixBotton[2] = labelPos[2];
                pixTop[0] = bounds[1];
                pixTop[1] = labelPos[1];
                pixTop[2] = labelPos[2];
                min = bounds[0];
                max = bounds[1];
                break;
            case __Y__:
                pixBotton[0] = labelPos[0];
                pixBotton[1] = bounds[2];
                pixBotton[2] = labelPos[2];
                pixTop[0] = labelPos[0];
                pixTop[1] = bounds[3];
                pixTop[2] = labelPos[2];
                min = bounds[2];
                max = bounds[3];
                break;
            case __Z__:
                pixBotton[0] = labelPos[0];
                pixBotton[1] = labelPos[1];
                pixBotton[2] = bounds[4];
                pixTop[0] = labelPos[0];
                pixTop[1] = labelPos[1];
                pixTop[2] = bounds[5];
                min = bounds[4];
                max = bounds[5];
                break;
            default:
                return;
        }

        pixBotton = AxesDrawer.computePixelFrom3dCoordinates(ax, pixBotton);
        pixTop = AxesDrawer.computePixelFrom3dCoordinates(ax, pixTop);

        width = Math.abs(pixBotton[0] - pixTop[0]);
        height = Math.abs(pixTop[1] - pixBotton[1]);

        if (width > height) {
            length = width;
            delta = 1.0 * (nextPos[0] - pos[0]);
            if (pixTop[0] >= pixBotton[0]) {
                flag = 1;
            } else {
                flag = -1;
            }
        } else {
            length = height;
            delta = 1.0 * (nextPos[1] - pos[1]);
            if (pixTop[1] > pixBotton[1]) {
                flag = 1;
            } else {
                flag = -1;
            }
        }

        switch (axis) {
            case __X__:
                temp = CommonHandler.logScale(labelPos[0], scale) +
                       (delta * ((CommonHandler.logScale(max, scale) - CommonHandler.logScale(min, scale)) / length) * flag);
                labelPos[0] = CommonHandler.InverseLogScale(temp, scale);
                if (labelPos[0] < min) {
                    labelPos[0] = min;
                } else if (labelPos[0] > max) {
                    labelPos[0] = max;
                }
                break;
            case __Y__:
                temp = CommonHandler.logScale(labelPos[1], scale) +
                       (delta * ((CommonHandler.logScale(max, scale) - CommonHandler.logScale(min, scale)) / length) * flag);
                labelPos[1] = CommonHandler.InverseLogScale(temp, scale);
                if (labelPos[1] < min) {
                    labelPos[1] = min;
                } else if (labelPos[1] > max) {
                    labelPos[1] = max;
                }
                break;
            case __Z__:
                temp = CommonHandler.logScale(labelPos[2], scale) +
                       (delta * ((CommonHandler.logScale(max, scale) - CommonHandler.logScale(min, scale)) / length) * flag);
                labelPos[2] = CommonHandler.InverseLogScale(temp, scale);
                if (labelPos[2] < min) {
                    labelPos[2] = min;
                } else if (labelPos[2] > max) {
                    labelPos[2] = max;
                }
                break;
            default:
                return;
        }
        GraphicController.getController().setProperty(label, GraphicObjectProperties.__GO_POSITION__, labelPos);
    }
}
