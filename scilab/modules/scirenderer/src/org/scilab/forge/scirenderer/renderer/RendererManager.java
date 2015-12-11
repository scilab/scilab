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

package org.scilab.forge.scirenderer.renderer;

/**
 *
 * Interface for renderer manager.
 *
 * @author Pierre Lando
 */
public interface RendererManager {

    /**
     * Create a new renderer.
     * @return a new renderer.
     */
    Renderer createRenderer();

    /**
     * Release all resources used by the given renderer.
     * @param renderer the given renderer.
     */
    void dispose(Renderer renderer);
}
