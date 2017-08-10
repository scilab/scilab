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

import org.scilab.forge.scirenderer.buffers.ElementsBuffer;

import javax.media.opengl.GL2;
import java.nio.Buffer;
import java.nio.FloatBuffer;

/**
 * @author Pierre Lando
 */
@SuppressWarnings(value = { "serial" })
public class JoGLElementsBuffer extends JoGLDataBuffer implements ElementsBuffer {

    /**
     * The current size of one element.
     */
    public static final int ELEMENT_SIZE = 4;

    /**
     * The default vertex.
     */
    private static final float[] DEFAULT_VERTEX = new float[] {0, 0, 0, 1};

    /**
     * the data this buffer contain.
     */
    private FloatBuffer data;
    private final Object mutex;

    /**
     * Default constructor.
     * The constructor is package : only {@link JoGLBuffersManager} can instantiate this object.
     */
    JoGLElementsBuffer() {
        mutex = new Object();
        data = null;
    }


    @Override
    public void setData(float[] newData, int elementSize) {
        // Check the given vertex size
        if ((elementSize < 1) || (elementSize > ELEMENT_SIZE)) {
            throw new BadElementSizeException(elementSize, 1, ELEMENT_SIZE);
        }

        int verticesNumber = newData.length / elementSize;
        //FloatBuffer buffer = BufferUtil.newFloatBuffer(ELEMENT_SIZE * verticesNumber);
        FloatBuffer buffer = FloatBuffer.allocate(ELEMENT_SIZE * verticesNumber);
        buffer.rewind();


        // Fill buffer with given data.
        // Missing coordinate are filled with the 'DEFAULT_VERTEX' ones.
        int k = 0;
        for (int i = 0; i < verticesNumber; i++) {
            for (int j = 0; j < ELEMENT_SIZE; j++) {
                if (j < elementSize) {
                    buffer.put(newData[k++]);
                } else {
                    buffer.put(DEFAULT_VERTEX[j]);
                }
            }
        }

        buffer.rewind();
        setData(buffer);
        setDataUploaded(false);
    }

    @Override
    public void setData(Float[] newData, int elementSize) {

        // Check the given vertex size
        if ((elementSize < 1) || (elementSize > ELEMENT_SIZE)) {
            throw new BadElementSizeException(elementSize, 1, ELEMENT_SIZE);
        }

        int verticesNumber = newData.length / elementSize;
        //FloatBuffer buffer = BufferUtil.newFloatBuffer(ELEMENT_SIZE * verticesNumber);
        FloatBuffer buffer = FloatBuffer.allocate(ELEMENT_SIZE * verticesNumber);
        buffer.rewind();


        // Fill buffer with given data.
        // Missing coordinate are filled with the 'DEFAULT_VERTEX' ones.
        int k = 0;
        for (int i = 0; i < verticesNumber; i++) {
            for (int j = 0; j < ELEMENT_SIZE; j++) {
                if (j < elementSize) {
                    buffer.put(newData[k++]);
                } else {
                    buffer.put(DEFAULT_VERTEX[j]);
                }
            }
        }

        buffer.rewind();
        setData(buffer);
        setDataUploaded(false);
    }

    @Override
    public void setData(FloatBuffer newData, int elementsSize) {
        // Check the given vertex size
        if ((elementsSize < 1) || (elementsSize > ELEMENT_SIZE)) {
            throw new BadElementSizeException(elementsSize, 1, ELEMENT_SIZE);
        }

        if (elementsSize == 4) {
            // No need to complete buffer.
            if (newData != null) {
                newData.rewind();
            }
            setData(newData);
            setDataUploaded(false);
            return;
        }

        int verticesNumber = newData.limit() / elementsSize;
        //FloatBuffer buffer = BufferUtil.newFloatBuffer(ELEMENT_SIZE * verticesNumber);
        FloatBuffer buffer = FloatBuffer.allocate(ELEMENT_SIZE * verticesNumber);
        buffer.rewind();

        // Fill buffer with given data.
        // Missing coordinate are filled with the 'DEFAULT_VERTEX' ones.
        newData.rewind();
        for (int i = 0; i < verticesNumber; i++) {
            for (int j = 0; j < ELEMENT_SIZE; j++) {
                if (j < elementsSize) {
                    buffer.put(newData.get());
                } else {
                    buffer.put(DEFAULT_VERTEX[j]);
                }
            }
        }

        buffer.rewind();
        setData(buffer);
        setDataUploaded(false);
    }

    @Override
    public int getSize() {
        synchronized (mutex) {
            if (data == null) {
                return 0;
            } else {
                return data.limit() / ELEMENT_SIZE;
            }
        }
    }

    @Override
    public FloatBuffer getData() {
        synchronized (mutex) {
            if (data != null) {
                return data;
            } else {
                return null;
            }
        }
    }

    @Override
    public int getElementsSize() {
        return ELEMENT_SIZE;
    }

    @Override
    public int getByteSize() {
        synchronized (mutex) {
            if (data == null) {
                return 0;
            } else {
                return data.limit() * (Float.SIZE / Byte.SIZE);
            }
        }
    }

    @Override
    protected Buffer getByteBuffer() {
        synchronized (mutex) {
            if (data != null) {
                data.rewind();
            }
            return data;
        }
    }

    @Override
    protected int getGLBindDestination() {
        return GL2.GL_ARRAY_BUFFER;
    }

    public int bindAsVertexBuffer(GL2 gl) {
        synchronized (mutex) {
            if (getSize() != 0) {
                gl.glEnableClientState(GL2.GL_VERTEX_ARRAY);
                gl.glBindBuffer(GL2.GL_ARRAY_BUFFER, getGlName(gl));
                gl.glVertexPointer(getElementsSize(), GL2.GL_FLOAT, 0, 0);
                gl.glBindBuffer(GL2.GL_ARRAY_BUFFER, 0);
                if (data == null) {
                    return 0;
                } else {
                    return data.limit() * (Float.SIZE / Byte.SIZE);
                }
            } else {
                return 0;
            }
        }
    }

    public int bindAsNormalsBuffer(GL2 gl) {
        synchronized (mutex) {
            if (getSize() != 0) {
                gl.glEnableClientState(GL2.GL_NORMAL_ARRAY);
                gl.glBindBuffer(GL2.GL_ARRAY_BUFFER, getGlName(gl));
                gl.glNormalPointer(GL2.GL_FLOAT, getElementsSize() * Float.SIZE / Byte.SIZE, 0);
                gl.glBindBuffer(GL2.GL_ARRAY_BUFFER, 0);
                if (data == null) {
                    return 0;
                } else {
                    return data.limit() * (Float.SIZE / Byte.SIZE);
                }
            } else {
                return 0;
            }
        }
    }

    public int bindAsColorsBuffer(GL2 gl) {
        synchronized (mutex) {
            if (getSize() != 0) {
                gl.glEnableClientState(GL2.GL_COLOR_ARRAY);
                gl.glBindBuffer(GL2.GL_ARRAY_BUFFER, getGlName(gl));
                gl.glColorPointer(getElementsSize(), GL2.GL_FLOAT, 0, 0);
                gl.glBindBuffer(GL2.GL_ARRAY_BUFFER, 0);
                if (data == null) {
                    return 0;
                } else {
                    return data.limit() * (Float.SIZE / Byte.SIZE);
                }
            } else {
                return 0;
            }
        }
    }

    public int bindAsTextureCoordinatesBuffer(GL2 gl) {
        synchronized (mutex) {
            if (getSize() != 0) {
                gl.glEnableClientState(GL2.GL_TEXTURE_COORD_ARRAY);
                gl.glBindBuffer(GL2.GL_ARRAY_BUFFER, getGlName(gl));
                gl.glTexCoordPointer(getElementsSize(), GL2.GL_FLOAT, 0, 0);
                gl.glBindBuffer(GL2.GL_ARRAY_BUFFER, 0);
                if (data == null) {
                    return 0;
                } else {
                    return data.limit() * (Float.SIZE / Byte.SIZE);
                }
            } else {
                return 0;
            }
        }
    }

    /**
     * Really set the data.
     * @param data the new data.
     */
    private void setData(FloatBuffer data) {
        synchronized (mutex) {
            this.data = data;
        }
    }

    /**
     * A specific runtime exception for bad elements size.
     */
    private static class BadElementSizeException extends RuntimeException {

        /**
         * Default constructor.
         * @param size the size given for elements.
         * @param min  the minimum possible size.
         * @param max  the upper bound of possible size (excluded of possible size).
         */
        public BadElementSizeException(int size, int min, int max) {
            super("Bad vertex elements  size : " + size + ". Should be in [" + min + ", " + (max - 1) + "]");
        }
    }

    @Override
    public void clear() {
        if (data != null) {
            data.clear();
        }
        data = null;
    }
}
