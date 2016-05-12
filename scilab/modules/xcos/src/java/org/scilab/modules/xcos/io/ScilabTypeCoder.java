/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enterprises - Clement DAVID
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
package org.scilab.modules.xcos.io;

import java.nio.ByteBuffer;
import java.nio.DoubleBuffer;
import java.nio.IntBuffer;
import java.nio.LongBuffer;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.IllegalFormatException;
import java.util.function.Consumer;
import org.scilab.modules.types.ScilabBoolean;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabInteger;
import org.scilab.modules.types.ScilabIntegerTypeEnum;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabTList;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.types.ScilabTypeEnum;
import org.scilab.modules.xcos.VectorOfDouble;
import org.scilab.modules.xcos.VectorOfInt;
import org.scilab.modules.xcos.VectorOfScicosID;

import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Encode and decode using a var2vec / vec2var compatible encoding.
 *
 * <p>
 * This encoder is used to store arbitrary data to the model. Usually some
 * properties are hard to map to both Java and C++ STL type system, using a
 * shared encoding let us provide an implementation whatever the language is
 * without sharing too low-level information.
 */
public class ScilabTypeCoder {

    private static final Logger LOG = Logger.getLogger("org.scilab.modules.xcos.io");

    class JavaScilabType {

        final ScilabTypeEnum type;
        final ScilabIntegerTypeEnum intType;

        public JavaScilabType(ScilabTypeEnum type, ScilabIntegerTypeEnum intType) {
            this.type = type;
            this.intType = intType;
        }
    }

    /**
     * current position in the vec buffer
     */
    int position = 0;

    public ScilabTypeCoder() {
    }

    /*
     * var2vec implementation
     */
    /**
     * Encode any scilab type to a buffer
     *
     * @param var the Scilab value to encode
     * @return the encoded buffer
     */
    public VectorOfDouble var2vec(ScilabType var) {
        final ScilabType value;
        if (var == null) {
            value = new ScilabDouble();
        } else {
            value = var;
        }

        if (LOG.isLoggable(Level.FINER)) {
            LOG.entering(ScilabTypeCoder.class.getCanonicalName(), "var2vec");
        }

        VectorOfDouble vec = new VectorOfDouble();
        encode(value, vec);

        // System.err.println("var2vec:" + var.toString() + ":" + toString(vec));
        if (LOG.isLoggable(Level.FINE)) {
            LOG.log(Level.FINE, "var2vec:{0}:{1}", new Object[] {var.toString(), toString(vec)});
        }

        if (LOG.isLoggable(Level.FINER)) {
            LOG.exiting(ScilabTypeCoder.class.getCanonicalName(), "var2vec");
        }

        return vec;
    }

    @SuppressWarnings("unchecked")
    private VectorOfDouble encode(ScilabType var, VectorOfDouble vec) {
        switch (var.getType()) {
            case sci_matrix:
                encode((ScilabDouble) var, vec);
                break;
            case sci_ints:
                encode((ScilabInteger) var, vec);
                break;
            case sci_boolean:
                encode((ScilabBoolean) var, vec);
                break;
            case sci_strings:
                encode((ScilabString) var, vec);
                break;
            case sci_list:
                encode((ArrayList<ScilabType>) var, vec, var.getType());
                break;
            case sci_mlist:
                encode((ArrayList<ScilabType>) var, vec, var.getType());
                break;
            case sci_tlist:
                encode((ArrayList<ScilabType>) var, vec, var.getType());
                break;
            default:
                break;
        }

        return vec;
    }

    /**
     * Encode the double data
     *
     * @param var the data to encode
     * @param vec the resulting buffer
     */
    private void encode(ScilabDouble var, VectorOfDouble vec) {
        // Header
        encodeHeader(var, vec, ScilabTypeEnum.sci_matrix);

        // specific flag for managing the complex case
        if (var.isReal()) {
            vec.add(0f);
        } else {
            vec.add(1f);
        }

        // push the data
        for (int i = 0; i < var.getHeight(); i++) {
            for (int j = 0; j < var.getWidth(); j++) {
                vec.add(var.getRealElement(i, j));
            }
        }

        // push the complex data
        if (!var.isReal()) {
            for (int i = 0; i < var.getHeight(); i++) {
                for (int j = 0; j < var.getWidth(); j++) {
                    vec.add(var.getImaginaryElement(i, j));
                }
            }
        }
    }

    private void encode(ScilabInteger var, VectorOfDouble vec) {
        // pre-processing: retrieve the raw data per type
        int sizeof;
        long[][] longData = null;
        short[][] shortData = null;
        int[][] intData = null;
        byte[][] byteData = null;
        switch (var.getPrec()) {
            case sci_int64:
            case sci_uint64:
                sizeof = Long.BYTES;
                longData = var.getDataAsLong();
                break;
            case sci_int32:
            case sci_uint32:
                sizeof = Integer.BYTES;
                intData = var.getDataAsInt();
                break;
            case sci_int16:
            case sci_uint16:
                sizeof = Short.BYTES;
                shortData = var.getDataAsShort();
                break;
            case sci_int8:
            case sci_uint8:
                sizeof = Byte.BYTES;
                byteData = var.getDataAsByte();
                break;
            default:
                throw new IllegalArgumentException();
        }

        // Header
        encodeHeader(var, vec, ScilabTypeEnum.sci_ints);

        // push the data on a pre-allocated space
        final int requiredBytes = sizeof * var.getHeight() * var.getWidth();
        final int doubleLen = (requiredBytes + Double.BYTES - 1) / Double.BYTES;
        final int index = vec.size();
        vec.resize(index + doubleLen);
        ByteBuffer view = vec.asByteBuffer(index, doubleLen);

        for (int i = 0; i < var.getHeight(); i++) {
            for (int j = 0; j < var.getWidth(); j++) {
                switch (var.getPrec()) {
                    case sci_int64:
                    case sci_uint64:
                        view.putLong(longData[i][j]);
                        break;
                    case sci_int32:
                    case sci_uint32:
                        view.putInt(intData[i][j]);
                        break;
                    case sci_int16:
                    case sci_uint16:
                        view.putShort(shortData[i][j]);
                        break;
                    case sci_int8:
                    case sci_uint8:
                        view.put(byteData[i][j]);
                        break;
                }
            }
        }
    }

    private void encode(ScilabBoolean var, VectorOfDouble vec) {
        // header
        encodeHeader(var, vec, ScilabTypeEnum.sci_boolean);

        // put all the boolean as int accordingly to Scilab 6 implementation
        final int requiredBytes = Integer.BYTES * var.getHeight() * var.getWidth();
        final int doubleLen = (requiredBytes + Double.BYTES - 1) / Double.BYTES;
        int index = vec.size();
        vec.resize(index + doubleLen);

        ByteBuffer buffer = vec.asByteBuffer(index, doubleLen);

        for (int i = 0; i < var.getHeight(); i++) {
            for (int j = 0; j < var.getWidth(); j++) {
                buffer.putInt(var.getData()[i][j] ? 1 : 0);
            }
        }
    }

    private void encode(ScilabString var, VectorOfDouble vec) {
        // header
        encodeHeader(var, vec, ScilabTypeEnum.sci_strings);

        // add the offset table which contains the offset of each UTF-8 encoded strings
        int offsetTableStart = vec.size();
        vec.resize(offsetTableStart + var.getHeight() * var.getWidth());
        int offsetTableAccumulated = 0;

        // encode the strings as UTF-8 and store the associated offset
        Charset utf8 = Charset.forName("UTF-8");
        for (int i = 0; i < var.getHeight(); i++) {
            for (int j = 0; j < var.getWidth(); j++) {
                String str = var.getData()[i][j];
                byte[] bytes = str.getBytes(utf8);
                // append the terminal '\0'
                final int requiredBytes = bytes.length + 1;
                final int doubleLen = (requiredBytes + Double.BYTES - 1) / Double.BYTES;

                // set the offset
                offsetTableAccumulated += doubleLen;
                vec.set(offsetTableStart++, offsetTableAccumulated);

                // push the data through a temporary byte buffer
                int index = vec.size();
                vec.resize(index + doubleLen);
                vec.asByteBuffer(index, doubleLen).put(bytes).put((byte) 0);

            }
        }
    }

    private void encode(ArrayList<ScilabType> var, VectorOfDouble vec, ScilabTypeEnum as) {
        // header
        encodeHeader(var, vec, as);

        // encode list content
        for (ScilabType localVar : var) {
            encode(localVar, vec);
        }
    }

    /**
     * Helper method to add an header of the detected type
     *
     * @param var the scilab matrix type to encode
     * @param vec the raw encoded data container
     * @param as the type to encode
     * @param detected the detected type
     */
    @SuppressWarnings({"unchecked", "fallthrough"})
    private void encodeHeader(Object var, VectorOfDouble vec, final ScilabTypeEnum as) {
        ScilabType matrix = null;
        ArrayList<ScilabType> list = null;

        // defensive programming
        switch (as) {
            case sci_boolean:
            case sci_ints:
            case sci_matrix:
            case sci_strings:
                matrix = (ScilabType) var;
                break;
            case sci_list:
            case sci_mlist:
            case sci_tlist:
                list = (ArrayList<ScilabType>) var;
                break;
            default:
                throw new IllegalArgumentException();
        }

        vec.add(as.swigValue());
        if (matrix instanceof ScilabInteger) {
            vec.add(((ScilabInteger) matrix).getPrec().swigValue());
        }
        if (matrix != null) {
            vec.add(2);
            vec.add(matrix.getHeight());
            vec.add(matrix.getWidth());
        } else if (list != null) {
            vec.add(list.size());
        } else {
            throw new IllegalArgumentException();
        }
    }

    /*
     * vec2var implementation
     */
    /**
     * Decode a scilab type from a buffer
     *
     * @param vec the buffer containing encoded scilab types
     * @return the decoded scilab type
     */
    public ScilabType vec2var(VectorOfDouble vec) {
        position = 0;

        if (LOG.isLoggable(Level.FINER)) {
            LOG.entering(ScilabTypeCoder.class.getName(), "vec2var", vec);
        }

        ScilabType var = decodeHeader(vec);
        decode(vec, var);

        // System.err.println("vec2var:" + toString(vec) + ":" + var.toString());
        if (LOG.isLoggable(Level.FINE)) {
            LOG.log(Level.FINE, "vec2var:{0}:{1}", new Object[] {toString(vec), var.toString()});
        }

        if (LOG.isLoggable(Level.FINER)) {
            LOG.exiting(ScilabTypeCoder.class.getName(), "vec2var");
        }

        return var;
    }

    @FunctionalInterface
    private static interface StoreFunction<R, C, V> {
        public void apply(R r, C c, V v);
    }

    @SuppressWarnings("unchecked")
    private ScilabType decode(VectorOfDouble vec, ScilabType var) {
        switch (var.getType()) {
            case sci_matrix:
                decode(vec, (ScilabDouble) var);
                break;
            case sci_ints:
                decode(vec, (ScilabInteger) var);
                break;
            case sci_boolean:
                decode(vec, (ScilabBoolean) var);
                break;
            case sci_strings:
                decode(vec, (ScilabString) var);
                break;
            case sci_list:
                decode(vec, (ArrayList<ScilabType>) var);
                break;
            case sci_mlist:
                decode(vec, (ArrayList<ScilabType>) var);
                break;
            case sci_tlist:
                decode(vec, (ArrayList<ScilabType>) var);
                break;
            default:
                break;
        }
        return var;
    }

    private ScilabType decode(VectorOfDouble vec, ScilabDouble var) {
        double[][] realPart = var.getRealPart();
        for (int i = 0; i < var.getHeight(); i++) {
            for (int j = 0; j < var.getWidth(); j++) {
                realPart[i][j] = vec.get(position++);
            }
        }

        if (!var.isReal()) {
            double[][] imaginaryPart = var.getImaginaryPart();

            for (int i = 0; i < var.getHeight(); i++) {
                for (int j = 0; j < var.getWidth(); j++) {
                    imaginaryPart[i][j] = vec.get(position++);
                }
            }
        }
        return var;
    }

    private ScilabType decode(VectorOfDouble vec, ScilabInteger var) {
        final int sizeof;
        long[][] longData = null;
        short[][] shortData = null;
        int[][] intData = null;
        byte[][] byteData = null;

        switch (var.getPrec()) {
            case sci_int64:
            case sci_uint64:
                sizeof = Long.BYTES;
                longData = var.getDataAsLong();
                break;
            case sci_int32:
            case sci_uint32:
                sizeof = Integer.BYTES;
                intData = var.getDataAsInt();
                break;
            case sci_int16:
            case sci_uint16:
                sizeof = Short.BYTES;
                shortData = var.getDataAsShort();
                break;
            case sci_int8:
            case sci_uint8:
                sizeof = Byte.BYTES;
                byteData = var.getDataAsByte();
                break;
            default:
                throw new IllegalArgumentException();
        }

        final int doubleLen = (sizeof * var.getHeight() * var.getWidth()) / Double.BYTES + 1;
        ByteBuffer view = vec.asByteBuffer(position, doubleLen);

        for (int i = 0; i < var.getHeight(); i++) {
            for (int j = 0; j < var.getWidth(); j++) {
                switch (var.getPrec()) {
                    case sci_int64:
                    case sci_uint64:
                        longData[i][j] = view.getLong();
                        break;
                    case sci_int32:
                    case sci_uint32:
                        intData[i][j] = view.getInt();
                        break;
                    case sci_int16:
                    case sci_uint16:
                        shortData[i][j] = view.getShort();
                        break;
                    case sci_int8:
                    case sci_uint8:
                        byteData[i][j] = view.get();
                        break;
                }
            }
        }
        position += doubleLen;

        return var;
    }

    private ScilabType decode(VectorOfDouble vec, ScilabBoolean var) {
        final boolean[][] data = var.getData();

        final int doubleLen = (Integer.BYTES * var.getHeight() * var.getWidth()) / Double.BYTES + 1;
        ByteBuffer view = vec.asByteBuffer(position, doubleLen);

        for (int i = 0; i < var.getHeight(); i++) {
            for (int j = 0; j < var.getWidth(); j++) {
                data[i][j] = view.getInt() != 0;
            }
        }
        position += doubleLen;
        return var;
    }

    private ScilabType decode(VectorOfDouble vec, ScilabString var) {
        final String[][] data = var.getData();
        decodeString(var.getHeight(), var.getWidth(), vec, (i, j, str) -> data[i][j] = str);
        return var;
    }

    private void decodeString(int height, int width, VectorOfDouble vec, StoreFunction<Integer, Integer, String> store) {
        // reconstruct the offset
        int[][] offset = new int[height][width];
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                offset[i][j] = (int) vec.get(position++);
            }
        }

        Charset utf8 = Charset.forName("UTF-8");
        int accumulatedOffset = 0;
        // reconstruct each String object
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                final int nbOfDouble = offset[i][j] - accumulatedOffset;
                ByteBuffer view = vec.asByteBuffer(position, nbOfDouble);
                byte[] bytes = new byte[nbOfDouble * Double.BYTES];

                view.get(bytes);

                // to avoid mis-decoding we have to pass the len of bytes
                int strlen = 0;
                while (bytes[strlen] != 0) {
                    strlen++;
                }

                store.apply(i, j, new String(bytes, 0, strlen, utf8));

                accumulatedOffset += nbOfDouble;
                position += nbOfDouble;
            }
        }
    }

    private ScilabType decode(VectorOfDouble vec, ArrayList<ScilabType> var) {
        for (int i = 0; i < var.size(); i++) {
            ScilabType sub = decodeHeader(vec);
            decode(vec, sub);

            var.set(i, sub);
        }
        return (ScilabType) var;
    }

    @SuppressWarnings("fallthrough")
    private ScilabType decodeHeader(VectorOfDouble vec) {
        int nativeScilabType = (int) vec.get(position++);

        // specific integer sub-type
        int precision = 0;

        // for data[][]-based type
        int height = 0;
        int width = 0;

        // for ArrayList-based type
        int listLen = 0;

        final ScilabTypeEnum type = ScilabTypeEnum.swigToEnum(nativeScilabType);
        switch (type) {
            case sci_ints:
                // special case for integer precision
                precision = (int) vec.get(position++);
            case sci_matrix:
            case sci_boolean:
            case sci_strings:
                position++; // n-Dims not managed
                height = (int) vec.get(position++);
                width = (int) vec.get(position++);
                break;
            case sci_list:
            case sci_mlist:
            case sci_tlist:
                listLen = (int) vec.get(position++);
                break;
            default:
                throw new IllegalArgumentException();
        }

        // special case for complex double matrix
        double[][] imagData = null;
        if (type == ScilabTypeEnum.sci_matrix) {
            boolean isComplex = vec.get(position++) != 0;

            if (isComplex) {
                imagData = new double[height][width];
            }
        }

        // allocate the right type with the decoded properties
        switch (type) {
            case sci_matrix:
                if (height * width == 0) {
                    return new ScilabDouble();
                } else {
                    return new ScilabDouble(new double[height][width], imagData);
                }
            case sci_boolean:
                if (height * width == 0) {
                    return new ScilabBoolean();
                } else {
                    return new ScilabBoolean(new boolean[height][width]);
                }
            case sci_ints:
                if (height * width == 0) {
                    return new ScilabInteger();
                } else {
                    switch (ScilabIntegerTypeEnum.swigToEnum(precision)) {
                        case sci_int8:
                            return new ScilabInteger(new byte[height][width], false);
                        case sci_int16:
                            return new ScilabInteger(new short[height][width], false);
                        case sci_int32:
                            return new ScilabInteger(new int[height][width], false);
                        case sci_int64:
                            return new ScilabInteger(new long[height][width], false);
                        case sci_uint8:
                            return new ScilabInteger(new byte[height][width], true);
                        case sci_uint16:
                            return new ScilabInteger(new short[height][width], true);
                        case sci_uint32:
                            return new ScilabInteger(new int[height][width], true);
                        case sci_uint64:
                            return new ScilabInteger(new long[height][width], true);

                    }
                }
            case sci_strings:
                if (height * width == 0) {
                    return new ScilabString();
                } else {
                    return new ScilabString(new String[height][width]);
                }
            case sci_list:
                return new ScilabList(Collections.nCopies(listLen, null));
            case sci_mlist:
                return new ScilabMList(new String[listLen - 1], Collections.nCopies(listLen - 1, null));
            case sci_tlist:
                return new ScilabTList(new String[listLen - 1], Collections.nCopies(listLen - 1, null));
            default:
                throw new IllegalArgumentException();

        }
    }

    /**
     * Format the encoded value accordingly to the Java format string
     *
     * @param format a
     * {@link String#format(java.lang.String, java.lang.Object...)} compatible
     * argument
     * @param vec the buffer containing encoded scilab types
     * @throws IllegalFormatException when the underlying formatting failed
     * @throws IllegalArgumentException when the vec argument is wrongly encoded
     * @return A formatted string
     */
    public String format(String format, VectorOfDouble vec) throws IllegalFormatException, IllegalArgumentException {
        ArrayList<Object> arguments = new ArrayList<>();
        decodeToJava(vec, arguments);
        return String.format(format, arguments.toArray());
    }

    private void decodeToJava(VectorOfDouble vec, ArrayList<Object> arguments) {
        int nativeScilabType = (int) vec.get(position++);

        // specific integer sub-type
        int precision = 0;

        // for data[][]-based type
        int height = 0;
        int width = 0;

        // for ArrayList-based type
        int listLen = 0;

        final ScilabTypeEnum type = ScilabTypeEnum.swigToEnum(nativeScilabType);
        switch (type) {
            case sci_ints:
                // special case for integer precision
                precision = (int) vec.get(position++);
            case sci_matrix:
            case sci_boolean:
            case sci_strings:
                position++; // n-Dims not managed
                height = (int) vec.get(position++);
                width = (int) vec.get(position++);
                break;
            case sci_list:
            case sci_mlist:
            case sci_tlist:
                listLen = (int) vec.get(position++);
                break;
            default:
                throw new IllegalArgumentException();
        }

        // special case for complex double matrix
        boolean isComplex = false;
        if (type == ScilabTypeEnum.sci_matrix) {
            isComplex = vec.get(position++) != 0;
        }

        // allocate the right type with the decoded properties
        switch (type) {
            case sci_matrix:
                for (int i = 0; i < height; i++) {
                    for (int j = 0; j < width; j++) {
                        arguments.add(vec.get(position++));
                    }
                }

                if (isComplex) {
                    for (int i = 0; i < height; i++) {
                        for (int j = 0; j < width; j++) {
                            arguments.add(vec.get(position++));
                        }
                    }
                }
                break;
            case sci_boolean: {
                final int doubleLen = (Integer.BYTES * height * width) / Double.BYTES + 1;
                ByteBuffer view = vec.asByteBuffer(position, doubleLen);
                for (int i = 0; i < height; i++) {
                    for (int j = 0; j < width; j++) {
                        arguments.add(view.getInt() != 0);
                    }
                }
                position += doubleLen;
            }
            break;
            case sci_ints:
                switch (ScilabIntegerTypeEnum.swigToEnum(precision)) {
                    case sci_int8:
                    case sci_uint8: {
                        final int sizeof = Byte.BYTES;
                        final int doubleLen = (sizeof * height * width) / Double.BYTES + 1;
                        ByteBuffer view = vec.asByteBuffer(position, doubleLen);
                        for (int i = 0; i < height; i++) {
                            for (int j = 0; j < width; j++) {
                                arguments.add(view.get());
                            }
                        }
                    }
                    break;
                    case sci_int16:
                    case sci_uint16: {
                        final int sizeof = Short.BYTES;
                        final int doubleLen = (sizeof * height * width) / Double.BYTES + 1;
                        ByteBuffer view = vec.asByteBuffer(position, doubleLen);
                        for (int i = 0; i < height; i++) {
                            for (int j = 0; j < width; j++) {
                                arguments.add(view.getShort());
                            }
                        }
                    }
                    break;
                    case sci_int32:
                    case sci_uint32: {
                        final int sizeof = Integer.BYTES;
                        final int doubleLen = (sizeof * height * width) / Double.BYTES + 1;
                        ByteBuffer view = vec.asByteBuffer(position, doubleLen);
                        for (int i = 0; i < height; i++) {
                            for (int j = 0; j < width; j++) {
                                arguments.add(view.getInt());
                            }
                        }
                    }
                    break;
                    case sci_int64:
                    case sci_uint64: {
                        final int sizeof = Long.BYTES;
                        final int doubleLen = (sizeof * height * width) / Double.BYTES + 1;
                        ByteBuffer view = vec.asByteBuffer(position, doubleLen);
                        for (int i = 0; i < height; i++) {
                            for (int j = 0; j < width; j++) {
                                arguments.add(view.getLong());
                            }
                        }
                    }
                    break;
                }
                break;
            case sci_strings:
                decodeString(height, width, vec, (i, j, v) -> arguments.add(v));
                break;
            case sci_list:
            case sci_mlist:
            case sci_tlist: {
                for (int i = 0; i < listLen; i++) {
                    decodeToJava(vec, arguments);
                }
            }
            break;
            default:
                throw new IllegalArgumentException();
        }
    }

    /**
     * Utility to display a vec on debug
     *
     * @param vec the vector to convert
     * @return a representative string
     */
    public static String toString(VectorOfDouble vec) {
        int len = vec.size();
        double[] copy = new double[len];
        DoubleBuffer.wrap(copy).put(vec.asByteBuffer(0, len).asDoubleBuffer());
        return Arrays.toString(copy);
    }

    /**
     * Utility to display a vec on debug
     *
     * @param vec the vector to convert
     * @return a representative string
     */
    public static String toString(VectorOfScicosID vec) {
        int len = vec.size();
        long[] copy = new long[len];
        LongBuffer.wrap(copy).put(vec.asByteBuffer(0, len).asLongBuffer());
        return Arrays.toString(copy);
    }

    /**
     * Utility to display a vec on debug
     *
     * @param vec the vector to convert
     * @return a representative string
     */
    public static String toString(VectorOfInt vec) {
        int len = vec.size();
        int[] copy = new int[len];
        IntBuffer.wrap(copy).put(vec.asByteBuffer(0, len).asIntBuffer());
        return Arrays.toString(copy);
    }
}
