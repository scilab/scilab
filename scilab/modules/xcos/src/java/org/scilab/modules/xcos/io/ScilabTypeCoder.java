/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.io;

import java.io.UnsupportedEncodingException;
import java.lang.ref.WeakReference;
import java.nio.ByteBuffer;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

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

/**
 * Encode and decode using a var2vec / vec2var compatible encoding.
 *
 * <p>
 * This encoder is used to store arbitrary data to the model. Usually some properties are hard to map to both Java and C++ STL type system, using a shared encoding let us provide an implementation
 * whatever the language is without sharing too low-level information.
 */
public class ScilabTypeCoder {

    class JavaScilabType {
        final ScilabTypeEnum type;
        final ScilabIntegerTypeEnum intType;

        public JavaScilabType(ScilabTypeEnum type, ScilabIntegerTypeEnum intType) {
            this.type = type;
            this.intType = intType;
        }
    }

    /** current position in the vec buffer */
    int position = 0;

    public ScilabTypeCoder() {
    }

    /*
     * var2vec implementation
     */

    /**
     * Encode any scilab type to a buffer
     *
     * @param var
     *            the Scilab value to encode
     * @return the encoded buffer
     */
    public VectorOfDouble var2vec(ScilabType var) {
        final ScilabType value;
        if (var == null) {
            value = new ScilabDouble();
        } else {
            value = var;
        }

        return encode(value, new VectorOfDouble());
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
     * @param var
     *            the data to encode
     * @param vec
     *            the resulting buffer
     */
    private void encode(ScilabDouble var, VectorOfDouble vec) {
        // Header
        encodeHeader(var, vec, ScilabTypeEnum.sci_matrix);

        // specific flag for managing the complex case
        if (var.isReal()) {
            vec.add(0);
        } else {
            vec.add(1);
        }

        // push the data
        for (int i = 0; i < var.getHeight(); i++)
            for (int j = 0; j < var.getWidth(); j++) {
                vec.add(var.getRealElement(i, j));
            }

        // push the complex data
        if (!var.isReal())
            for (int i = 0; i < var.getHeight(); i++)
                for (int j = 0; j < var.getWidth(); j++) {
                    vec.add(var.getImaginaryElement(i, j));
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
        final int position = vec.size();
        vec.resize(position + doubleLen);
        ByteBuffer view = vec.asByteBuffer(position, doubleLen);

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
        int position = vec.size();
        vec.resize(position + doubleLen);

        ByteBuffer buffer = vec.asByteBuffer(position, doubleLen);

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

        // encode the strings as UTF-8 and store the associated offset
        for (int i = 0; i < var.getHeight(); i++) {
            for (int j = 0; j < var.getWidth(); j++) {
                String str = var.getData()[i][j];
                byte[] bytes = str.getBytes(Charset.forName("UTF-8"));
                // append the terminal '\0'
                final int requiredBytes = ((bytes.length + 1) * Byte.BYTES);
                final int doubleLen = (requiredBytes + Double.BYTES - 1) / Double.BYTES;

                // set the offset
                vec.set(offsetTableStart++, doubleLen);

                // push the data through a temporary byte buffer
                int position = vec.size();
                vec.resize(position + doubleLen);
                vec.asByteBuffer(position, doubleLen).put(bytes);
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
     * @param var
     *            the scilab matrix type to encode
     * @param vec
     *            the raw encoded data container
     * @param as
     *            the type to encode
     * @param detected
     *            the detected type
     */
    @SuppressWarnings({ "unchecked", "fallthrough" })
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
            vec.add(matrix.getWidth());
            vec.add(matrix.getHeight());
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
     * @param vec
     *            the buffer containing encoded scilab types
     * @return the decoded scilab type
     */
    public ScilabType vec2var(VectorOfDouble vec) {
        position = 0;

        ScilabType var = decodeHeader(vec);
        return decode(vec, var);
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
        for (int i = 0; i < var.getHeight(); i++)
            for (int j = 0; j < var.getWidth(); j++) {
                realPart[i][j] = vec.get(position++);
            }

        if (!var.isReal()) {
            double[][] imaginaryPart = var.getImaginaryPart();

            for (int i = 0; i < var.getHeight(); i++)
                for (int j = 0; j < var.getWidth(); j++) {
                    imaginaryPart[i][j] = vec.get(position++);
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

        // reconstruct the offset
        int[][] offset = new int[var.getHeight()][var.getWidth()];
        for (int i = 0; i < var.getHeight(); i++) {
            for (int j = 0; j < var.getWidth(); j++) {
                offset[i][j] = (int) vec.get(position++);
            }
        }

        // reconstruct each String object
        try {
            for (int i = 0; i < var.getHeight(); i++) {
                for (int j = 0; j < var.getWidth(); j++) {
                    ByteBuffer view = vec.asByteBuffer(position, offset[i][j]);
                    byte[] bytes = new byte[offset[i][j] * Double.BYTES];

                    view.get(bytes);
                    data[i][j] = new String(bytes, "UTF-8");

                    position += offset[i][j];
                }
            }
        } catch (UnsupportedEncodingException e) {
            // ignore as UTF-8 is always available
        }
        return var;
    }

    private ScilabType decode(VectorOfDouble vec, ArrayList<ScilabType> var) {
        for (int i = 0; i < var.size(); i++) {
            var.set(i, vec2var(vec));
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
                return new ScilabDouble(new double[height][width], imagData);
            case sci_boolean:
                return new ScilabBoolean(new boolean[height][width]);
            case sci_ints:
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
            case sci_strings:
                return new ScilabString(new String[height][width]);
            case sci_list:
                return new ScilabList(Collections.nCopies(listLen, null));
            case sci_mlist:
                return new ScilabMList(new String[listLen], Collections.nCopies(listLen, null));
            case sci_tlist:
                return new ScilabTList(new String[listLen], Collections.nCopies(listLen, null));
            default:
                throw new IllegalArgumentException();

        }
    }
}
