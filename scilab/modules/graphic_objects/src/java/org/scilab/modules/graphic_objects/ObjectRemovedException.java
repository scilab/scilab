/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - ScilabEnterprises - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.graphic_objects;

public class ObjectRemovedException extends Exception {
    private static final long serialVersionUID = -5447310699942928356L;

    private String objectId;

    public ObjectRemovedException(String objectId) {
        this.objectId = objectId;
    }

    public String getMessage() {
        return "ObjectRemoved Exception: Object " + objectId + "has been deleted from model";
    }
}
