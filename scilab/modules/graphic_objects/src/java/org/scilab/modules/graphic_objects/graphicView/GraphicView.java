/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
