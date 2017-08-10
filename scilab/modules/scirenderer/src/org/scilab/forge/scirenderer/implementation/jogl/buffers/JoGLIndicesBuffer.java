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

import org.scilab.forge.scirenderer.buffers.IndicesBuffer;

import javax.media.opengl.GL;
import java.nio.Buffer;
import java.nio.IntBuffer;
import java.util.Collection;

/**
 * @author Pierre Lando
 */
public class JoGLIndicesBuffer extends JoGLDataBuffer implements IndicesBuffer {

    /**
     * the data this buffer contain.
     */
    private IntBuffer data;

    /**
     * Default constructor.
     * The constructor is package : only {@link JoGLBuffersManager} can instantiate this object.
     */
    JoGLIndicesBuffer() {
        data = null;
    }


    @Override
    public void setData(int[] indices) {
        //IntBuffer buffer = BufferUtil.newIntBuffer(indices.length);
        IntBuffer buffer = IntBuffer.allocate(indices.length);
        buffer.rewind();
        buffer.put(indices);
        buffer.rewind();
        this.data = buffer;
        setDataUploaded(false);
    }

    @Override
    public void setData(Collection<Integer> indices) {
        IntBuffer buffer = IntBuffer.allocate(indices.size());
        buffer.rewind();
        for (int index : indices) {
            buffer.put(index);
        }
        buffer.rewind();
        this.data = buffer;
        setDataUploaded(false);
    }

    @Override
    public void setData(IntBuffer indexBuffer) {
        //IntBuffer buffer = BufferUtil.newIntBuffer(indexBuffer.limit());
        IntBuffer buffer = IntBuffer.allocate(indexBuffer.limit());
        buffer.rewind();
        indexBuffer.rewind();
        buffer.put(indexBuffer);
        buffer.rewind();
        indexBuffer.rewind();
        this.data = buffer;
        setDataUploaded(false);
    }

    @Override
    public int getSize() {
        if (data == null) {
            return  0;
        } else {
            return data.limit();
        }
    }

    @Override
    public IntBuffer getData() {
        return data.asReadOnlyBuffer();
    }

    @Override
    public int getByteSize() {
        if (data == null) {
            return 0;
        } else {
            return data.limit() * (Integer.SIZE / Byte.SIZE);
        }
    }

    @Override
    protected Buffer getByteBuffer() {
        if (data != null) {
            data.rewind();
        }
        return data;
    }

    @Override
    protected int getGLBindDestination() {
        return GL.GL_ELEMENT_ARRAY_BUFFER;
    }

    public void clear() {
        data.clear();
        data = null;
    }
}
