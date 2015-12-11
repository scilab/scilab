/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.forge.scirenderer.picking;

/**
 * Interface for a picking task.
 *
 * @author Pierre Lando
 */
public interface PickingTask {

    /**
     * Call to perform the task.
     * @param pickingTools the picking tools to use.
     */
    void perform(PickingTools pickingTools);
}
