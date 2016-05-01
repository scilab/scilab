/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Pierre Lando
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
