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
