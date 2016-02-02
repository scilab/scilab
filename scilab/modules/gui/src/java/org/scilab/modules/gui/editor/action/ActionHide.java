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
package org.scilab.modules.gui.editor.action;

import org.scilab.modules.gui.editor.action.BaseAction;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

/**
* Implements the object to store hide related actions
*
* @author Caio Souza <caioc2bolado@gmail.com>
* @author Pedro Souza <bygrandao@gmail.com>
*
* @since 2012-06-01
*/
public class ActionHide extends BaseAction {


    /**
    * Constructor
    *
    * @param object the hidden Object
    */
    public ActionHide(Integer object) {

        super(object, (Integer)GraphicController.getController().getProperty(object, GraphicObjectProperties.__GO_PARENT__));
    }

    /**
    * Undo, unhide the object
    */
    public void undo() {

        if (exists()) {
            GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_VISIBLE__, true);
        } else {
            objectUID = null;
        }
    }

    /**
    * Redo, hide the object
    */
    public void redo() {

        if (exists()) {
            GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_VISIBLE__, false);
        } else {
            objectUID = null;
        }
    }

    public void dispose() {

        objectUID = null;
    }
}