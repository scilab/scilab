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

import org.scilab.modules.gui.editor.action.Action;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

/**
* Implementes the Base for the all actions
*
* @author Caio Souza <caioc2bolado@gmail.com>
* @author Pedro Souza <bygrandao@gmail.com>
*
* @since 2012-06-01
*/
public abstract class BaseAction implements Action {

    Integer objectUID;
    Integer parentUID;

    /**
    * Constructor
    *
    * @param objectUID The object of the action
    * @param parentUID The parent from the object
    */
    public BaseAction(Integer objectUID, Integer parentUID) {

        this.objectUID = objectUID;
        this.parentUID = parentUID;
    }

    /**
    * Check if the Object and his parent exists
    */
    public boolean exists() {

        if (GraphicController.getController().getObjectFromId(objectUID) != null && GraphicController.getController().getObjectFromId(parentUID) != null) {
            return true;
        } else {
            return false;
        }
    }

    /**
    * Set a relation between two objects
    */
    public void setRelation(Integer object, Integer parent) {

        if (exists()) {
            GraphicController.getController().setGraphicObjectRelationship(parent, object);
        } else {
            GraphicController.getController().removeRelationShipAndDelete(object);
            objectUID = null;
            parentUID = null;
        }
    }

    /**
    * Removes the relation of an object
    */
    public void removeRelation(Integer object) {

        GraphicController.getController().setGraphicObjectRelationship(0, object);
    }
}