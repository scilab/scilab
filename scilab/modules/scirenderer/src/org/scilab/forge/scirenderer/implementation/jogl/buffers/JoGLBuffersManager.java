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

package org.scilab.forge.scirenderer.implementation.jogl.buffers;

import org.scilab.forge.scirenderer.buffers.BuffersManager;
import org.scilab.forge.scirenderer.buffers.DataBuffer;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.buffers.IndicesBuffer;

import javax.media.opengl.GL2;
import java.util.Collection;
import java.util.HashSet;
import java.util.Set;
import java.util.Stack;

/**
 * @author Pierre Lando
 */
public final class JoGLBuffersManager implements BuffersManager {

    /**
     * Set off current buffers.
     */
    private final Set<JoGLDataBuffer> buffers = new HashSet<JoGLDataBuffer>();

    /**
     * Set of dead buffers.
     */
    private final Stack<JoGLDataBuffer> deadBuffers = new Stack<JoGLDataBuffer>();

    /**
     * Default constructor.
     */
    public JoGLBuffersManager() {
    }

    @Override
    public synchronized ElementsBuffer createElementsBuffer() {
        JoGLElementsBuffer newBuffer = new JoGLElementsBuffer();
        buffers.add(newBuffer);
        return newBuffer;
    }

    @Override
    public synchronized IndicesBuffer createIndicesBuffer() {
        JoGLIndicesBuffer newBuffer = new JoGLIndicesBuffer();
        buffers.add(newBuffer);
        return newBuffer;
    }

    @Override
    public synchronized void dispose(DataBuffer buffer) {
        JoGLDataBuffer localBuffer = getLocalBuffer(buffer);
        if (localBuffer != null) {
            buffers.remove(localBuffer);
            localBuffer.clear();
            deadBuffers.push(localBuffer);
        }
    }

    @Override
    public synchronized void dispose(Collection <? extends DataBuffer > buffers) {
        for (DataBuffer buffer : buffers) {
            dispose(buffer);
        }
    }

    /**
     * Called when previous OpenGl context is gone.
     */
    public synchronized void glReload() {
        for (JoGLDataBuffer buffer : buffers) {
            buffer.reload();
        }
    }

    /**
     * Called before rendering for synchronisation.
     * Clean dead buffers.
     * @param gl the OpenGl context.
     */
    public synchronized void glSynchronize(GL2 gl) {
        int[] names = new int[deadBuffers.size()];
        int i = 0;
        while (!deadBuffers.isEmpty()) {
            int n = deadBuffers.pop().disposeWithoutDelete(gl);
            if (n != -1) {
                names[i++] = n;
            }
        }

        if (i != 0) {
            gl.glDeleteBuffers(i, names, 0);
        }
    }

    /**
     * Bind the given buffer as vertex buffer.
     * @param gl the OpenGl context where the buffer is bind.
     * @param buffer the buffer to bind.
     * @return the number of element actually bind.
     */
    public int bindVertexBuffer(GL2 gl, ElementsBuffer buffer) {
        JoGLElementsBuffer localBuffer = getLocalElementsBuffer(buffer);
        if (localBuffer != null) {
            return localBuffer.bindAsVertexBuffer(gl);
        } else {
            return 0;
        }
    }

    /**
     * Bind the given buffer as normal buffer.
     * @param gl the OpenGl context where the buffer is bind.
     * @param buffer the buffer to bind.
     * @return the number of element actually bind.
     */
    public int bindNormalsBuffer(GL2 gl, ElementsBuffer buffer) {
        JoGLElementsBuffer localBuffer = getLocalElementsBuffer(buffer);
        if (localBuffer != null) {
            return localBuffer.bindAsNormalsBuffer(gl);
        } else {
            return 0;
        }
    }

    /**
     * Bind the given buffer as texture coordinate buffer.
     * @param gl the OpenGl context where the buffer is bind.
     * @param buffer the buffer to bind.
     * @return the number of element actually bind.
     */
    public int bindTextureCoordinatesBuffer(GL2 gl, ElementsBuffer buffer) {
        JoGLElementsBuffer localBuffer = getLocalElementsBuffer(buffer);
        if (localBuffer != null) {
            return localBuffer.bindAsTextureCoordinatesBuffer(gl);
        } else {
            return 0;
        }
    }

    /**
     * Bind the given buffer as color buffer.
     * @param gl the OpenGl context where the buffer is bind.
     * @param buffer the buffer to bind.
     * @return the number of element actually bind.
     */
    public int bindColorsBuffer(GL2 gl, ElementsBuffer buffer) {
        JoGLElementsBuffer localBuffer = getLocalElementsBuffer(buffer);
        if (localBuffer != null) {
            return localBuffer.bindAsColorsBuffer(gl);
        } else {
            return 0;
        }
    }

    /**
     * Bind the given buffer as indices buffer.
     * @param gl the OpenGl context where the buffer is bind.
     * @param buffer the buffer to bind.
     * @return the number of element actually bind.
     */
    public int bindIndicesBuffer(GL2 gl, IndicesBuffer buffer) {
        JoGLDataBuffer localBuffer = getLocalBuffer(buffer);
        if (localBuffer != null) {
            gl.glBindBuffer(GL2.GL_ELEMENT_ARRAY_BUFFER, localBuffer.getGlName(gl));
            return buffer.getSize();
        } else {
            return 0;
        }
    }

    /**
     * This method check buffer to be from here.
     * @param buffer the given buffer.
     * @return the JoGL instance of the buffer.
     */
    private synchronized JoGLDataBuffer getLocalBuffer(DataBuffer buffer) {
        if (buffer instanceof JoGLDataBuffer) {
            JoGLDataBuffer localBuffer = (JoGLDataBuffer) buffer;
            if (buffers.contains(localBuffer)) {
                return localBuffer;
            }
        }
        return null;
    }

    /**
     * This method check buffer to be from here.
     * @param buffer the given buffer.
     * @return the JoGL instance of the buffer.
     */
    private synchronized JoGLIndicesBuffer getLocalIndicesBuffer(IndicesBuffer buffer) {
        if (buffer instanceof JoGLIndicesBuffer) {
            JoGLIndicesBuffer localBuffer = (JoGLIndicesBuffer) buffer;
            if (buffers.contains(localBuffer)) {
                return localBuffer;
            }
        }
        return null;
    }

    /**
     * This method check buffer to be from here.
     * @param buffer the given buffer.
     * @return the JoGL instance of the buffer.
     */
    private synchronized JoGLElementsBuffer getLocalElementsBuffer(ElementsBuffer buffer) {
        if (buffer instanceof JoGLElementsBuffer) {
            JoGLElementsBuffer localBuffer = (JoGLElementsBuffer) buffer;
            if (buffers.contains(localBuffer)) {
                return localBuffer;
            }
        }
        return null;
    }
}
