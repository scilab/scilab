/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.types;

import java.nio.Buffer;
import java.nio.ByteBuffer;
import java.nio.DoubleBuffer;
import java.nio.ShortBuffer;
import java.nio.IntBuffer;
import java.nio.LongBuffer;
import java.util.Arrays;

/**
 * This class contains a lot of equals methods.
 * Unfortunatelly, we cannot template it...
 */
final class ScilabTypeUtils {

    /**
     * Check equality between double data
     * @param buffer a buffer or matrix
     * @param bswaped if true the buffer is swaped
     * @param data a matrix or a buffer
     * @param dswaped if true the data is swaped
     * @return true if data contains the same values
     */
    static boolean equalsDouble(final Object buffer, final boolean bswaped, final Object data, final boolean dswaped) {
        if (buffer instanceof DoubleBuffer) {
            if (data instanceof double[][]) {
                return equals((DoubleBuffer) buffer, bswaped, (double[][]) data, dswaped);
            } else {
                return equals((DoubleBuffer) buffer, bswaped, (DoubleBuffer) data, dswaped);
            }
        } else if (buffer instanceof double[][]) {
            if (data instanceof double[][]) {
                return equals((double[][]) buffer, bswaped, (double[][]) data, dswaped);
            } else {
                return equals((double[][]) buffer, bswaped, (DoubleBuffer) data, dswaped);
            }
        }

        return false;
    }

    /**
     * Check equality between integer data
     * @param buffer a buffer or matrix
     * @param bswaped if true the buffer is swaped
     * @param data a matrix or a buffer
     * @param dswaped if true the data is swaped
     * @return true if data contains the same values
     */
    static boolean equalsInteger(final Object buffer, final boolean bswaped, final Object data, final boolean dswaped) {
        if (buffer instanceof ByteBuffer) {
            if (data instanceof byte[][]) {
                return equals((ByteBuffer) buffer, bswaped, (byte[][]) data, dswaped);
            } else {
                return equals((ByteBuffer) buffer, bswaped, (ByteBuffer) data, dswaped);
            }
        } else if (buffer instanceof byte[][]) {
            if (data instanceof byte[][]) {
                return equals((byte[][]) buffer, bswaped, (byte[][]) data, dswaped);
            } else {
                return equals((byte[][]) buffer, bswaped, (ByteBuffer) data, dswaped);
            }
        } else if (buffer instanceof ShortBuffer) {
            if (data instanceof short[][]) {
                return equals((ShortBuffer) buffer, bswaped, (short[][]) data, dswaped);
            } else {
                return equals((ShortBuffer) buffer, bswaped, (ShortBuffer) data, dswaped);
            }
        } else if (buffer instanceof short[][]) {
            if (data instanceof short[][]) {
                return equals((short[][]) buffer, bswaped, (short[][]) data, dswaped);
            } else {
                return equals((short[][]) buffer, bswaped, (ShortBuffer) data, dswaped);
            }
        } else if (buffer instanceof IntBuffer) {
            if (data instanceof int[][]) {
                return equals((IntBuffer) buffer, bswaped, (int[][]) data, dswaped);
            } else {
                return equals((IntBuffer) buffer, bswaped, (IntBuffer) data, dswaped);
            }
        } else if (buffer instanceof int[][]) {
            if (data instanceof int[][]) {
                return equals((int[][]) buffer, bswaped, (int[][]) data, dswaped);
            } else {
                return equals((int[][]) buffer, bswaped, (IntBuffer) data, dswaped);
            }
        } else if (buffer instanceof LongBuffer) {
            if (data instanceof long[][]) {
                return equals((LongBuffer) buffer, bswaped, (long[][]) data, dswaped);
            } else {
                return equals((LongBuffer) buffer, bswaped, (LongBuffer) data, dswaped);
            }
        } else if (buffer instanceof long[][]) {
            if (data instanceof long[][]) {
                return equals((long[][]) buffer, bswaped, (long[][]) data, dswaped);
            } else {
                return equals((long[][]) buffer, bswaped, (LongBuffer) data, dswaped);
            }
        }

        return false;
    }

    /**
     * Check equality between boolean data
     * @param buffer a buffer or matrix
     * @param bswaped if true the buffer is swaped
     * @param data a matrix or a buffer
     * @param dswaped if true the data is swaped
     * @return true if data contains the same values
     */
    static boolean equalsBoolean(final Object buffer, final boolean bswaped, final Object data, final boolean dswaped) {
        if (buffer instanceof IntBuffer) {
            if (data instanceof boolean[][]) {
                return equals((IntBuffer) buffer, bswaped, (boolean[][]) data, dswaped);
            } else {
                return equals((IntBuffer) buffer, bswaped, (IntBuffer) data, dswaped);
            }
        } else if (buffer instanceof boolean[][]) {
            if (data instanceof boolean[][]) {
                return equals((boolean[][]) buffer, bswaped, (boolean[][]) data, dswaped);
            } else {
                return equals((boolean[][]) buffer, bswaped, (IntBuffer) data, dswaped);
            }
        }

        return false;
    }

    /**
     * Check equality between two buffers
     * @param buffer a buffer or matrix
     * @param bswaped if true the buffer is swaped
     * @param data a matrix or a buffer
     * @param dswaped if true the data is swaped
     * @return true if data contains the same values
     */
    static boolean equals(final Buffer buffer, final boolean bswaped, final Buffer data, final boolean dswaped) {
        buffer.clear();
        data.clear();
        return buffer.equals(data);
    }

    /**
     * Check equality between double data
     * @param buffer a matrix
     * @param bswaped if true the buffer is swaped
     * @param data a buffer
     * @param dswaped if true the data is swaped
     * @return true if data contains the same values
     */
    static boolean equals(final double[][] buffer, final boolean bswaped, final DoubleBuffer data, final boolean dswaped) {
        return equals(data, dswaped, buffer, bswaped);
    }

    /**
     * Check equality between byte data
     * @param buffer a matrix
     * @param bswaped if true the buffer is swaped
     * @param data a buffer
     * @param dswaped if true the data is swaped
     * @return true if data contains the same values
     */
    static boolean equals(final byte[][] buffer, final boolean bswaped, final ByteBuffer data, final boolean dswaped) {
        return equals(data, dswaped, buffer, bswaped);
    }

    /**
     * Check equality between short data
     * @param buffer a matrix
     * @param bswaped if true the buffer is swaped
     * @param data a buffer
     * @param dswaped if true the data is swaped
     * @return true if data contains the same values
     */
    static boolean equals(final short[][] buffer, final boolean bswaped, final ShortBuffer data, final boolean dswaped) {
        return equals(data, dswaped, buffer, bswaped);
    }

    /**
     * Check equality between int data
     * @param buffer a matrix
     * @param bswaped if true the buffer is swaped
     * @param data a buffer
     * @param dswaped if true the data is swaped
     * @return true if data contains the same values
     */
    static boolean equals(final int[][] buffer, final boolean bswaped, final IntBuffer data, final boolean dswaped) {
        return equals(data, dswaped, buffer, bswaped);
    }

    /**
     * Check equality between long data
     * @param buffer a matrix
     * @param bswaped if true the buffer is swaped
     * @param data a buffer
     * @param dswaped if true the data is swaped
     * @return true if data contains the same values
     */
    static boolean equals(final long[][] buffer, final boolean bswaped, final LongBuffer data, final boolean dswaped) {
        return equals(data, dswaped, buffer, bswaped);
    }

    /**
     * Check equality between boolean data
     * @param buffer a matrix
     * @param bswaped if true the buffer is swaped
     * @param data a buffer
     * @param dswaped if true the data is swaped
     * @return true if data contains the same values
     */
    static boolean equals(final boolean[][] buffer, final boolean bswaped, final IntBuffer data, final boolean dswaped) {
        return equals(data, dswaped, buffer, bswaped);
    }

    /**
     * Check equality between matrix data
     * @param buffer a matrix
     * @param bswaped if true the buffer is swaped
     * @param data a matrix
     * @param dswaped if true the data is swaped
     * @return true if data contains the same values
     */
    static boolean equals(final Object[] buffer, final boolean bswaped, final Object[] data, final boolean dswaped) {
        return Arrays.deepEquals(buffer, data);
    }

    /**
     * Check equality between boolean data
     * @param buffer a buffer
     * @param bswaped if true the buffer is swaped
     * @param data a matrix
     * @param dswaped if true the data is swaped
     * @return true if data contains the same values
     */
    static boolean equals(final IntBuffer buffer, final boolean bswaped, final boolean[][] data, final boolean dswaped) {
        final int r = data.length;
        final int c = r > 0 ? data[0].length : 0;
        if (!dswaped) {
            for (int i = 0; i < r; i++) {
                for (int j = 0; j < c; j++) {
                    if ((buffer.get(i + r * j) != 0) != data[i][j]) {
                        return false;
                    }
                }
            }
        } else {
            for (int i = 0; i < r; i++) {
                for (int j = 0; j < c; j++) {
                    if ((buffer.get(j + c * i) != 0) != data[i][j]) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    /**
     * Check equality between double data
     * @param buffer a buffer
     * @param bswaped if true the buffer is swaped
     * @param data a matrix
     * @param dswaped if true the data is swaped
     * @return true if data contains the same values
     */
    static boolean equals(final DoubleBuffer buffer, final boolean bswaped, final double[][] data, final boolean dswaped) {
        if (!dswaped) {
            final int r = data.length;
            final int c = r > 0 ? data[0].length : 0;
            for (int i = 0; i < r; i++) {
                for (int j = 0; j < c; j++) {
                    if (buffer.get(i + r * j) != data[i][j]) {
                        return false;
                    }
                }
            }
        } else {
            final int c = data.length;
            final int r = c > 0 ? data[0].length : 0;
            buffer.clear();
            for (int i = 0; i < c; i++) {
                buffer.position(r * i);
                buffer.limit(r * i + r);
                if (!buffer.equals(DoubleBuffer.wrap(data[i]))) {
                    buffer.clear();
                    return false;
                }
            }
            buffer.clear();
        }
        return true;
    }

    /**
     * Check equality between byte data
     * @param buffer a buffer
     * @param bswaped if true the buffer is swaped
     * @param data a matrix
     * @param dswaped if true the data is swaped
     * @return true if data contains the same values
     */
    static boolean equals(final ByteBuffer buffer, final boolean bswaped, final byte[][] data, final boolean dswaped) {
        if (!dswaped) {
            final int r = data.length;
            final int c = r > 0 ? data[0].length : 0;
            for (int i = 0; i < r; i++) {
                for (int j = 0; j < c; j++) {
                    if (buffer.get(i + r * j) != data[i][j]) {
                        return false;
                    }
                }
            }
        } else {
            final int c = data.length;
            final int r = c > 0 ? data[0].length : 0;
            buffer.clear();
            for (int i = 0; i < c; i++) {
                buffer.position(r * i);
                buffer.limit(r * i + r);
                if (!buffer.equals(ByteBuffer.wrap(data[i]))) {
                    buffer.clear();
                    return false;
                }
            }
            buffer.clear();
        }
        return true;
    }

    /**
     * Check equality between short data
     * @param buffer a buffer
     * @param bswaped if true the buffer is swaped
     * @param data a matrix
     * @param dswaped if true the data is swaped
     * @return true if data contains the same values
     */
    static boolean equals(final ShortBuffer buffer, final boolean bswaped, final short[][] data, final boolean dswaped) {
        if (!dswaped) {
            final int r = data.length;
            final int c = r > 0 ? data[0].length : 0;
            for (int i = 0; i < r; i++) {
                for (int j = 0; j < c; j++) {
                    if (buffer.get(i + r * j) != data[i][j]) {
                        return false;
                    }
                }
            }
        } else {
            final int c = data.length;
            final int r = c > 0 ? data[0].length : 0;
            buffer.clear();
            for (int i = 0; i < c; i++) {
                buffer.position(r * i);
                buffer.limit(r * i + r);
                if (!buffer.equals(ShortBuffer.wrap(data[i]))) {
                    buffer.clear();
                    return false;
                }
            }
            buffer.clear();
        }
        return true;
    }

    /**
     * Check equality between int data
     * @param buffer a buffer
     * @param bswaped if true the buffer is swaped
     * @param data a matrix
     * @param dswaped if true the data is swaped
     * @return true if data contains the same values
     */
    static boolean equals(final IntBuffer buffer, final boolean bswaped, final int[][] data, final boolean dswaped) {
        if (!dswaped) {
            final int r = data.length;
            final int c = r > 0 ? data[0].length : 0;
            for (int i = 0; i < r; i++) {
                for (int j = 0; j < c; j++) {
                    if (buffer.get(i + r * j) != data[i][j]) {
                        return false;
                    }
                }
            }
        } else {
            final int c = data.length;
            final int r = c > 0 ? data[0].length : 0;
            buffer.clear();
            for (int i = 0; i < c; i++) {
                buffer.position(r * i);
                buffer.limit(r * i + r);
                if (!buffer.equals(IntBuffer.wrap(data[i]))) {
                    buffer.clear();
                    return false;
                }
            }
            buffer.clear();
        }
        return true;
    }

    /**
     * Check equality between long data
     * @param buffer a buffer
     * @param bswaped if true the buffer is swaped
     * @param data a matrix
     * @param dswaped if true the data is swaped
     * @return true if data contains the same values
     */
    static final boolean equals(final LongBuffer buffer, final boolean bswaped, final long[][] data, final boolean dswaped) {
        if (!dswaped) {
            final int r = data.length;
            final int c = r > 0 ? data[0].length : 0;
            for (int i = 0; i < r; i++) {
                for (int j = 0; j < c; j++) {
                    if (buffer.get(i + r * j) != data[i][j]) {
                        return false;
                    }
                }
            }
        } else {
            final int c = data.length;
            final int r = c > 0 ? data[0].length : 0;
            buffer.clear();
            for (int i = 0; i < c; i++) {
                buffer.position(r * i);
                buffer.limit(r * i + r);
                if (!buffer.equals(LongBuffer.wrap(data[i]))) {
                    buffer.clear();
                    return false;
                }
            }
            buffer.clear();
        }
        return true;
    }

    /**
     * Set the real part of the data.
     *
     * @param realPart
     *            the real part.
     */
    public static final void setBuffer(final double[][] part, final DoubleBuffer buffer) {
        final int c = part.length;
        final int r = c > 0 ? part[0].length : 0;
        if (r * c <= buffer.capacity()) {
            buffer.clear();
            for (int i = 0; i < c; i++) {
                buffer.get(part[i]);
            }
        }
    }

    /**
     * Set the part of the data.
     */
    public static final void setPart(final DoubleBuffer buffer, final double[][] part) {
        final int c = part.length;
        final int r = c > 0 ? part[0].length : 0;
        if (r * c <= buffer.capacity()) {
            buffer.clear();
            for (int i = 0; i < c; i++) {
                buffer.put(part[i]);
            }
        }
    }

    /**
     * Set the real part of the data.
     *
     * @param realPart
     *            the real part.
     */
    public static final void setBuffer(final byte[][] part, final ByteBuffer buffer) {
        final int c = part.length;
        final int r = c > 0 ? part[0].length : 0;
        if (r * c <= buffer.capacity()) {
            buffer.clear();
            for (int i = 0; i < c; i++) {
                buffer.get(part[i]);
            }
        }
    }

    /**
     * Set the part of the data.
     */
    public static final void setPart(final ByteBuffer buffer, final byte[][] part) {
        final int c = part.length;
        final int r = c > 0 ? part[0].length : 0;
        if (r * c <= buffer.capacity()) {
            buffer.clear();
            for (int i = 0; i < c; i++) {
                buffer.put(part[i]);
            }
        }
    }

    /**
     * Set the real part of the data.
     *
     * @param realPart
     *            the real part.
     */
    public static final void setBuffer(final short[][] part, final ShortBuffer buffer) {
        final int c = part.length;
        final int r = c > 0 ? part[0].length : 0;
        if (r * c <= buffer.capacity()) {
            buffer.clear();
            for (int i = 0; i < c; i++) {
                buffer.get(part[i]);
            }
        }
    }

    /**
     * Set the part of the data.
     */
    public static final void setPart(final ShortBuffer buffer, final short[][] part) {
        final int c = part.length;
        final int r = c > 0 ? part[0].length : 0;
        if (r * c <= buffer.capacity()) {
            buffer.clear();
            for (int i = 0; i < c; i++) {
                buffer.put(part[i]);
            }
        }
    }

    /**
     * Set the real part of the data.
     *
     * @param realPart
     *            the real part.
     */
    public static final void setBuffer(final int[][] part, final IntBuffer buffer) {
        final int c = part.length;
        final int r = c > 0 ? part[0].length : 0;
        if (r * c <= buffer.capacity()) {
            buffer.clear();
            for (int i = 0; i < c; i++) {
                buffer.get(part[i]);
            }
        }
    }

    /**
     * Set the part of the data.
     */
    public static final void setPart(final IntBuffer buffer, final int[][] part) {
        final int c = part.length;
        final int r = c > 0 ? part[0].length : 0;
        if (r * c <= buffer.capacity()) {
            buffer.clear();
            for (int i = 0; i < c; i++) {
                buffer.put(part[i]);
            }
        }
    }

    /**
     * Set the real part of the data.
     *
     * @param realPart
     *            the real part.
     */
    public static final void setBuffer(final long[][] part, final LongBuffer buffer) {
        final int c = part.length;
        final int r = c > 0 ? part[0].length : 0;
        if (r * c <= buffer.capacity()) {
            buffer.clear();
            for (int i = 0; i < c; i++) {
                buffer.get(part[i]);
            }
        }
    }

    /**
     * Set the part of the data.
     */
    public static final void setPart(final LongBuffer buffer, final long[][] part) {
        final int c = part.length;
        final int r = c > 0 ? part[0].length : 0;
        if (r * c <= buffer.capacity()) {
            buffer.clear();
            for (int i = 0; i < c; i++) {
                buffer.put(part[i]);
            }
        }
    }

    /**
     * Set the part of the data.
     */
    public static final void setPart(final IntBuffer buffer, final boolean[][] part) {
        final int c = part.length;
        final int r = c > 0 ? part[0].length : 0;
        if (r * c == buffer.capacity()) {
            buffer.clear();
            for (int i = 0; i < c; i++) {
                for (int j = 0; j < r; j++) {
                    buffer.put(i + c * j, part[i][j] ? 1 : 0);
                }
            }
        }
    }

    /**
     * Set the real part of the data.
     *
     * @param realPart
     *            the real part.
     */
    public static final void setBuffer(final boolean[][] part, final IntBuffer buffer) {
        final int c = part.length;
        final int r = c > 0 ? part[0].length : 0;
        if (r * c <= buffer.capacity()) {
            buffer.clear();
            for (int i = 0; i < c; i++) {
                for (int j = 0; j < r; j++) {
                    part[i][j] = buffer.get(i + c * j) != 0;
                }
            }
        }
    }
}
