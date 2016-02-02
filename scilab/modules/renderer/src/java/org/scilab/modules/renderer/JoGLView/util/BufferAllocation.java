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

package org.scilab.modules.renderer.JoGLView.util;

import com.jogamp.opengl.util.GLBuffers;

import java.nio.ByteBuffer;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;

/**
 * @author Pierre Lando
 */
public final class BufferAllocation {

    /** keep 32 Mo of free memory */
    private static final int REMAINING_MEMORY = 32 * 1024 * 1024;

    /** Hidden constructor */
    private BufferAllocation() {
    }

    /**
     * Allocate a new direct byte buffer of given length.
     * @param length the given length.
     * @return a new direct byte buffer of given length.
     * @throws OutOfMemoryException if java heap space is to small.
     */
    public static ByteBuffer newByteBuffer(int length) throws OutOfMemoryException {
        haveFreeMemory(length);
        ByteBuffer buffer = GLBuffers.newDirectByteBuffer(length);
        if (buffer.limit() != length) {
            throw new OutOfMemoryException();
        }
        return buffer;
    }

    /**
     * Allocate a new direct integer buffer of given length.
     * @param length the given length.
     * @return a new direct integer buffer of given length.
     * @throws OutOfMemoryException if java heap space is to small.
     */
    public static IntBuffer newIntBuffer(int length) throws OutOfMemoryException {
        haveFreeMemory(length * Integer.SIZE / Byte.SIZE);
        IntBuffer buffer = GLBuffers.newDirectIntBuffer(length);
        if (buffer.limit() != length) {
            throw new OutOfMemoryException();
        }
        return buffer;
    }

    /**
     * Allocate a new direct float buffer of given length.
     * @param length the given length.
     * @return a new direct float buffer of given length.
     * @throws OutOfMemoryException if java heap space is to small.
     */
    public static FloatBuffer newFloatBuffer(int length) throws OutOfMemoryException {
        haveFreeMemory(length * Float.SIZE / Byte.SIZE);
        FloatBuffer buffer = GLBuffers.newDirectFloatBuffer(length);
        if (buffer.limit() != length) {
            throw new OutOfMemoryException();
        }
        return buffer;
    }

    /**
     * Check if the java heap space have enough memory.
     * @param length the memory needed (in bytes).
     * @throws OutOfMemoryException if there was not enough memory.
     * @return a byte buffer of given length.
     */
    private static void haveFreeMemory(int length) throws OutOfMemoryException {
        final long max = Runtime.getRuntime().maxMemory();
        long total = Runtime.getRuntime().totalMemory();
        long free = Runtime.getRuntime().freeMemory();

        long realFree = max - total + free;
        if (realFree < length + REMAINING_MEMORY) {
            Runtime.getRuntime().gc();
            total = Runtime.getRuntime().totalMemory();
            free = Runtime.getRuntime().freeMemory();
        }

        realFree = max - total + free;
        if (realFree < length + REMAINING_MEMORY) {
            throw new OutOfMemoryException();
        }
    }
}
