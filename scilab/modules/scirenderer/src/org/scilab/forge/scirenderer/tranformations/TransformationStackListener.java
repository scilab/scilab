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
 * @author Pierre Lando
 */
public interface TransformationStackListener extends EventListener {

    /**
     * Called when an event occur in the listened transformation stack.
     * @param transformationStack the transformation stack where the event occur.
     * @param event the event.
     * @param top the new peek transformation of the stack.
     */
    void changed(final TransformationStack transformationStack, final TransformationStack.TransformationStackEvent event, final Transformation top);
}
