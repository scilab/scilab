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

package org.scilab.forge.scirenderer.tranformations;

import java.util.EventListener;

/**
 *
 * The listener of a {@link org.scilab.forge.scirenderer.tranformations.TransformationManager} functions are called when events occurs in the
 * listened {@link org.scilab.forge.scirenderer.tranformations.TransformationManager}.
 *
 * @author Pierre Lando
 */
public interface TransformationManagerListener extends EventListener {

    /**
     * Called when the top transformation of the listened TransformationManager have changed.
     * @param transformationManager object where event occur.
     */
    void transformationChanged(final TransformationManager transformationManager);

}
