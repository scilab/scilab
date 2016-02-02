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