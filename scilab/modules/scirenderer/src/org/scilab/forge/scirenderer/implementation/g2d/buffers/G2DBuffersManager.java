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

package org.scilab.forge.scirenderer.implementation.g2d.buffers;

import org.scilab.forge.scirenderer.buffers.BuffersManager;
import org.scilab.forge.scirenderer.buffers.DataBuffer;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.buffers.IndicesBuffer;

import java.util.Collection;
import java.util.HashSet;
import java.util.Set;

/**
 * @author Pierre Lando
 */
public final class G2DBuffersManager implements BuffersManager {

    /**
     * Set off current buffers.
     */
    private final Set<DataBuffer> buffers = new HashSet<DataBuffer>();

    // todo : est ce utile d'avoir buffers... pourquoi garder une reference la-dessus
    // peut etre pr savoir qu'un buffer vient de ce manager et pas d'un autre.

    /**
     * Default constructor.
     */
    public G2DBuffersManager() {
    }

    @Override
    public ElementsBuffer createElementsBuffer() {
        G2DElementsBuffer newBuffer = new G2DElementsBuffer();
        buffers.add(newBuffer);
        return newBuffer;
    }

    @Override
    public IndicesBuffer createIndicesBuffer() {
        G2DIndicesBuffer newBuffer = new G2DIndicesBuffer();
        buffers.add(newBuffer);
        return newBuffer;
    }

    @Override
    public void dispose(DataBuffer buffer) {
        DataBuffer localBuffer = getLocalBuffer(buffer);
        if (localBuffer != null) {
            localBuffer.clear();
            buffers.remove(localBuffer);
        }
    }

    @Override
    public void dispose(Collection <? extends DataBuffer > buffers) {
        for (DataBuffer buffer : buffers) {
            dispose(buffer);
        }
    }

    /**
     * This method check buffer to be from here.
     * @param buffer the given buffer.
     * @return the G2D instance of the buffer.
     */
    private DataBuffer getLocalBuffer(DataBuffer buffer) {
        if (buffers.contains(buffer)) {
            return buffer;
        }

        return null;
    }


    /**
     * This method check buffer to be from here.
     * @param buffer the given buffer.
     * @return the G2D instance of the buffer.
     */
    private IndicesBuffer getLocalIndicesBuffer(IndicesBuffer buffer) {
        if (buffer instanceof G2DIndicesBuffer && buffers.contains(buffer)) {
            return buffer;
        }

        return null;
    }

    /**
     * This method check buffer to be from here.
     * @param buffer the given buffer.
     * @return the G2D instance of the buffer.
     */
    private ElementsBuffer getLocalElementsBuffer(ElementsBuffer buffer) {
        if (buffer instanceof G2DElementsBuffer && buffers.contains(buffer)) {
            return buffer;
        }

        return null;
    }
}
