/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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