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