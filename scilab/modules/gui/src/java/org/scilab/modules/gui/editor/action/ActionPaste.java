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
* Implements the object to store Paste related actions
*
* @author Caio Souza <caioc2bolado@gmail.com>
* @author Pedro Souza <bygrandao@gmail.com>
*
* @since 2012-06-01
*/
public class ActionPaste extends BaseAction {

    /* Store the Parent where the object was pasted */
    Integer pastedParent;

    /* True if the object is a dup, false otherwise */
    boolean isDuplicated;

    /**
    * Constructor
    *
    * @param object The pasted object
    * @param previosParent The parent from the object was copied
    * @param pastedParent The parent that the object was pasted
    * @param isDuplicated If the object was cutted it is false, if was duplicated it is true
    */
    public ActionPaste(Integer object, Integer previousParent, Integer pastedParent, boolean isDuplicated) {

        super(object, previousParent);
        this.pastedParent = pastedParent;
        this.isDuplicated = isDuplicated;
    }

    /**
    * Undo, if the object is a dup then remove it else put it back to the original parent
    */
    public void undo() {

        if (exists()) {
            if (isDuplicated) {
                removeRelation(objectUID);
            } else {
                setRelation(objectUID, parentUID);
            }
        } else {
            objectUID = null;
        }
    }

    /**
    * Redo, set the relation of the object with the last Parent
    */
    public void redo() {

        if (exists()) {
            setRelation(objectUID, pastedParent);
        } else {
            objectUID = null;
        }
    }

    public void dispose() {

        objectUID = null;
    }
}