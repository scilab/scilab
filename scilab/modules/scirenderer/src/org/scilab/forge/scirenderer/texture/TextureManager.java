/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
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