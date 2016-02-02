/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - ScilabEnterprises - Bruno JOFRET
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
package org.scilab.modules.graphic_objects;

public class ObjectRemovedException extends Exception {
    private static final long serialVersionUID = -5447310699942928356L;

    private Integer objectId;

    public ObjectRemovedException(Integer objectId) {
        this.objectId = objectId;
    }

    public String getMessage() {
        return "ObjectRemoved Exception: Object " + objectId.toString() + "has been deleted from model";
    }
}
