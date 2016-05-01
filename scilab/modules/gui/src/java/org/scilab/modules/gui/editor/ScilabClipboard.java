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

import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.editor.PolylineHandler;

/**
* Implements clipboard between figures for polylines.
*
* @author Caio Souza <caioc2bolado@gmail.com>
* @author Pedro Souza <bygrandao@gmail.com>
*
* @since 2012-06-01
*/

public class ScilabClipboard {

    static ScilabClipboard instance = null;
    Integer objectUid = null;
    boolean needDuplication = false;
    Integer copyStyle;

    public static ScilabClipboard getInstance() {
        if (instance == null) {
            instance = new ScilabClipboard();
        }
        return instance;
    }

    /**
    * Copy a object to the clipboad.
    * @param uid object unique identifier.
    */
    public void copy(Integer uid) {
        objectUid = uid;
        needDuplication = true;
    }

    /**
    * Paste the copied object to the given axes.
    * @param figure Figure unique identifier.
    * @param position Vector with mouse position x and y.
    * @retrun The UID from pasted object
    */
    public Integer paste(Integer figure, Integer[] position) {
        Integer object = objectUid;
        /*We store only the uid, so we need check if the object exists*/
        if (!canPaste()) {
            return null;
        }

        Integer axesFrom = (new ObjectSearcher()).searchParent(object, GraphicObjectProperties.__GO_AXES__);
        if (axesFrom == null) {
            return null;
        }

        if (needDuplication == true ) {
            object = CommonHandler.duplicate(objectUid);
        } else {
            CommonHandler.cut(object);
        }

        Integer axesTo = AxesHandler.clickedAxes(figure, position);
        if (axesTo != null) { /* If there is an axes in the clicked position then adjust the bounds, make the axes visible and paste */
            AxesHandler.setAxesVisible(axesTo);
            CommonHandler.insert(axesTo, object);
            /*
             * must be set up after insertion of the object
             * else tight_limits don't are set up correctly
             */
            AxesHandler.axesBound(axesFrom, axesTo);
            if (!needDuplication) {
                objectUid = null;
            }
            return object;
        } else { /* If doesn't exists an axes will duplicate the origin axes */
            axesTo = AxesHandler.duplicateAxes(axesFrom);
            if (axesTo != null) { /* If duplicated successfull then adjust the bounds and paste */
                CommonHandler.insert(axesTo, object);
                AxesHandler.axesBound(axesFrom, axesTo);
                if (!needDuplication) {
                    objectUid = null;
                }
                return object;
            }
            return null;
        }
    }

    /**
    * Cut an object to the clipboad.
    * @param object object unique identifier.
    */
    public void cut(Integer object) {
        objectUid = object;
        needDuplication = false;
    }

    /**
    * Check if there is any polyline copied/cuted to be pasted.
    * @return True if can be pasted, false otherwise.
    */
    public boolean canPaste() {
        if (!CommonHandler.objectExists(objectUid)) {
            objectUid = null;
            return false;
        }
        return true;
    }

    /**
    * Get The current object in the clipboard
    *
    * @return Current object in the clipboard
    */
    public Integer getCurrentObject() {

        return objectUid;
    }

    /**
    * Check if the object where the style will be copied exists
    */
    public boolean canPasteStyle() {

        if (!CommonHandler.objectExists(copyStyle)) {
            copyStyle = null;
            return false;
        }
        return true;
    }

    /**
    * Copy store the object to copy style
    *
    * @param object The axes to store
    */
    public void copyStyle(Integer objectUID) {

        copyStyle = objectUID;
    }

    /**
    * Paste the Style of the object in the clipboard to a new object
    *
    * @param object The object to recieve the style
    * @return The new axes pasted
    */
    public Integer pasteStyle(Integer objectUID, boolean bounds) {

        if (!canPasteStyle()) {
            return null;
        }
        Integer newAxes = AxesHandler.cloneAxesWithStyle(copyStyle);
        Integer figureFrom = CommonHandler.getParentFigure(copyStyle);
        Integer figureTo = CommonHandler.getParentFigure(objectUID);
        CommonHandler.cloneColorMap(figureFrom, figureTo);
        CommonHandler.cloneBackgroundColor(figureFrom, figureTo);
        AxesHandler.pasteAxesStyle(newAxes, objectUID, true);
        if (bounds) {
            AxesHandler.axesBound(copyStyle, newAxes);
        }
        return newAxes;
    }

}

