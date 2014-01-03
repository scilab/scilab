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

/**
* Implements the object to store delete related actions
*
* @author Caio Souza <caioc2bolado@gmail.com>
* @author Pedro Souza <bygrandao@gmail.com>
*
* @since 2012-06-01
*/
public class ActionDelete extends BaseAction {


    /**
    * Constructor
    *
    * @param object the Deleted Object
    * @param parentUID The parent from object
    */
    public ActionDelete(Integer object, Integer parentUID) {

        super(object, parentUID);
    }

    /**
    * Undo a delete action setting back the relation of the object
    */
    public void undo() {

        if (exists()) {
            setRelation(objectUID, parentUID);
        } else {
            objectUID = null;
        }
    }

    /**
    * Redo a delete action removing the relation of the object
    */
    public void redo() {

        if (exists()) {
            removeRelation(objectUID);
        } else {
            objectUID = null;
        }
    }

    public void dispose() {

        objectUID = null;
    }
}