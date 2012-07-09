/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Gustavo Barbosa Libotte
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.datatip;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.Type;
import org.scilab.modules.graphic_objects.graphicObject.*;

import org.scilab.modules.gui.editor.EditorEventListener;

/**
 * Manage the datatip mode activation
 * @author Gustavo Barbosa Libotte
 */
public class DatatipManagerMode {

    public static boolean datatipManagerModeStatus = false;
    public static String datatipMessage = "Left click on the curve creates a datatip and right click on the datatip removes it.";

    /**
    * Set the datatip manager mode between on/off
    * for graphics toolbar's toggle button
    *
    * @return Datatip manager mode status.
    */
    public static boolean setDatatipManagerModeToggle() {
        if(datatipManagerModeStatus) {
            datatipManagerModeStatus = false;
            GraphicController.getController().setProperty(EditorEventListener.windowUid, GraphicObjectProperties.__GO_INFO_MESSAGE__, "");
        } else {
            datatipManagerModeStatus = true;
            GraphicController.getController().setProperty(EditorEventListener.windowUid, GraphicObjectProperties.__GO_INFO_MESSAGE__, datatipMessage);
        }
        return datatipManagerModeStatus;
    }

    /**
    * Set the datatip manager mode on
    * for graphics menubar's button
    *
    * @return Datatip manager mode status on.
    */
    public static boolean setDatatipManagerModeButtonOn() {
        if(!datatipManagerModeStatus) {
            datatipManagerModeStatus = true;
            GraphicController.getController().setProperty(EditorEventListener.windowUid, GraphicObjectProperties.__GO_INFO_MESSAGE__, datatipMessage);
        }
        return datatipManagerModeStatus;
    }

    /**
    * Set the datatip manager mode off
    * for graphics menubar's button
    *
    * @return Datatip manager mode status off.
    */
    public static boolean setDatatipManagerModeButtonOff() {
        if(datatipManagerModeStatus) {
            datatipManagerModeStatus = false;
            GraphicController.getController().setProperty(EditorEventListener.windowUid, GraphicObjectProperties.__GO_INFO_MESSAGE__, "");
        }
        return datatipManagerModeStatus;
    }

    /**
    * Get the datatip's actual manager mode
    *
    * @return Datatip manager mode status.
    */
    public static boolean getDatatipManagerMode() {
        if(datatipManagerModeStatus) {
            return true;
        } else {
            return false;
        }
    }
}
