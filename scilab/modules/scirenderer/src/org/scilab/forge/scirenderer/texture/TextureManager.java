/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.forge.scirenderer.texture;

import java.util.Collection;

/**
 * @author Pierre Lando
 */
public interface TextureManager {

    /**
     * Texture creator.
     * @return a new {@link Texture}
     */
    Texture createTexture();

    /**
     * Dispose the given textures.
     * @param textures textures to dispose.
     */
    void dispose(Collection<Texture> textures);

    void dispose(Texture texture);
}