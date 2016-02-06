/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
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

package org.scilab.modules.graphic_objects.graphicView;

import java.util.EventListener;

public interface GraphicView extends EventListener {

    /**
     * Notify an object with ID : id
     * has been updated.
     * @param id
     */
    void updateObject(Integer id, int property);

    /**
     * Notify an object with ID : id
     * has been created.
     * @param id
     */
    void createObject(Integer id);

    /**
     * Notify an object with ID : id
     * has been deleted.
     * @param id
     */
    void deleteObject(Integer id);
}
