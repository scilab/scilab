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

package org.scilab.forge.scirenderer.buffers;

import java.util.Collection;

/**
 * Interface for a buffers manager.
 * @author Pierre Lando
 */
public interface BuffersManager {

    /**
     * Create an elements buffer.
     * @return a new elements buffer.
     */
    ElementsBuffer createElementsBuffer();

    /**
     * Create an indices buffer.
     * @return a new indices buffer.
     */
    IndicesBuffer createIndicesBuffer();

    /**
     * Release all resources used by the given buffer.
     * @param buffer the given buffer.
     */
    void dispose(DataBuffer buffer);

    /**
     * Release all resources used by the given buffers.
     * @param buffers the given buffers.
     */
    void dispose(Collection <? extends DataBuffer > buffers);
}
