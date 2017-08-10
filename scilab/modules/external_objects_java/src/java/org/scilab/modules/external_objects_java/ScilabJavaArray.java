/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - 2011 - Calixte DENIZET <calixte@contrib.scilab.org>
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

package org.scilab.modules.external_objects_java;

import java.lang.reflect.Array;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.logging.Level;

/**
 * Tools for arrays
 *
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = {"unchecked", "serial"})
public final class ScilabJavaArray {

    static final Map<Class, Class> mappings = new HashMap<Class, Class>(8);
    private static final Map<String, Class> baseType = new HashMap<String, Class>(8);

    static {
        mappings.put(Double.class, double.class);
        mappings.put(Integer.class, int.class);
        mappings.put(Boolean.class, boolean.class);
        mappings.put(Short.class, short.class);
        mappings.put(Byte.class, byte.class);
        mappings.put(Character.class, char.class);
        mappings.put(Long.class, long.class);
        mappings.put(Float.class, float.class);

        baseType.put("double", double.class);
        baseType.put("int", int.class);
        baseType.put("boolean", boolean.class);
        baseType.put("short", short.class);
        baseType.put("long", long.class);
        baseType.put("float", float.class);
        baseType.put("char", char.class);
        baseType.put("byte", byte.class);
    }

    /**
     * Create a wrapper for a Java Array object
     * @param className the class to use
     * @param dims the dimensions of the resulting array
     * @param the id of the array
     */
    public static int newInstance(String className, int[] dims) throws ScilabJavaException {
        Class cl = null;

        if (ScilabJavaObject.debug) {
            StringBuffer buf = new StringBuffer();
            buf.append("(");
            if (dims.length > 0) {
                int i = 0;
                for (; i < dims.length - 1; i++) {
                    buf.append(Integer.toString(dims[i]));
                    buf.append(",");
                }
                buf.append(Integer.toString(dims[i]));
            }
            buf.append(")");
            ScilabJavaObject.logger.log(Level.INFO, "Array creation: base class is \'" + className + "\' with dims=" + buf.toString());
        }

        cl = baseType.get(className);
        if (cl == null) {
            try {
                int id = ScilabClassLoader.loadJavaClass(className, false);
                if (id == 0) {
                    cl = (Class) ScilabJavaObject.arraySJO[id].object;
                } else {
                    cl = Class.forName(className);
                }
            } catch (ClassNotFoundException e) {
                throw new ScilabJavaException("Cannot find the class " + className);
            }
        }

        return new ScilabJavaObject(Array.newInstance(cl, dims)).id;
    }

    /**
     * Get an element in an array
     * @param array the array
     * @param index the index of the element to get
     * @return the element
     */
    public static Object get(Object array, int[] index) throws ScilabJavaException {
        Object obj = array;
        for (int i = 0; i < index.length; i++) {
            if (obj != null && obj.getClass().isArray()) {
                if (index[i] >= 0 && index[i] < Array.getLength(obj)) {
                    obj = Array.get(obj, index[i]);
                } else {
                    throw new ScilabJavaException("Problem in retrieving " + i + "-th element: " + index[i] + ">" + (Array.getLength(obj) - 1));
                }
            } else if (obj instanceof List) {
                List list = (List) obj;
                if (index[i] >= 0 && index[i] < list.size()) {
                    obj = list.get(index[i]);
                } else {
                    throw new ScilabJavaException("Problem in retrieving " + i + "-th element: " + index[i] + ">" + (list.size() - 1));
                }
            } else {
                throw new ScilabJavaException("Problem in retrieving " + i + "-th element: it is not an array");
            }
        }

        return obj;
    }

    /**
     * Set an element in an array
     * @param array the array
     * @param index the index of the element to set
     * @param x the element
     */
    public static void set(Object array, int[] index, Object x) throws ScilabJavaException {
        Object obj = array;
        int i = 0;
        for (; i < index.length - 1; i++) {
            if (obj != null && obj.getClass().isArray()) {
                if (index[i] >= 0 && index[i] < Array.getLength(obj)) {
                    obj = Array.get(obj, index[i]);
                } else {
                    throw new ScilabJavaException("Problem in retrieving " + i + "-th element: " + index[i] + ">" + (Array.getLength(obj) - 1));
                }
            } else if (obj instanceof List) {
                List list = (List) obj;
                if (index[i] >= 0 && index[i] < list.size()) {
                    obj = list.get(index[i]);
                } else {
                    throw new ScilabJavaException("Problem in retrieving " + i + "-th element: " + index[i] + ">" + (list.size() - 1));
                }
            } else {
                throw new ScilabJavaException("Problem in retrieving " + i + "-th element: it is not an array");
            }
        }

        if (obj != null && obj.getClass().isArray()) {
            if (index[i] >= 0 && index[i] < Array.getLength(obj)) {
                try {
                    Array.set(obj, index[i], FunctionArguments.convert(x, obj.getClass().getComponentType()));
                } catch (IllegalArgumentException e) {
                    throw new ScilabJavaException("Array " + obj + " cannot contain object which is an instance of " + x.getClass());
                }
            } else {
                throw new ScilabJavaException("Problem in setting " + index[i] + "-th element: " + index[i] + ">" + (Array.getLength(obj) - 1));
            }
        } else if (obj instanceof List) {
            List list = (List) obj;
            if (index[i] >= 0 && index[i] < list.size()) {
                list.set(index[i], x);
            } else if (index[i] == list.size()) {
                list.add(x);
            } else {
                throw new ScilabJavaException("Problem in retrieving " + i + "-th element: " + index[i] + ">" + (list.size() - 1));
            }
        } else {
            throw new ScilabJavaException("Problem in retrieving " + i + "-th element: it is not an array");
        }
    }

    /**
     * Unbox a Double array
     * @param a an array
     * @return an unboxed array
     */
    public static double[] toPrimitive(final Double[] a) {
        double[] arr = new double[a.length];
        for (int i = 0; i < a.length; i++) {
            arr[i] = a[i];
        }

        return arr;
    }

    /**
     * Unbox a Float array
     * @param a an array
     * @return an unboxed array
     */
    public static float[] toPrimitive(final Float[] a) {
        float[] arr = new float[a.length];
        for (int i = 0; i < a.length; i++) {
            arr[i] = a[i];
        }

        return arr;
    }

    /**
     * Unbox an Integer array
     * @param a an array
     * @return an unboxed array
     */
    public static int[] toPrimitive(final Integer[] a) {
        int[] arr = new int[a.length];
        for (int i = 0; i < a.length; i++) {
            arr[i] = a[i];
        }

        return arr;
    }

    /**
     * Unbox a Character array
     * @param a an array
     * @return an unboxed array
     */
    public static char[] toPrimitive(final Character[] a) {
        char[] arr = new char[a.length];
        for (int i = 0; i < a.length; i++) {
            arr[i] = a[i];
        }

        return arr;
    }

    /**
     * Unbox a Double array
     * @param a an array
     * @return an unboxed array
     */
    public static byte[] toPrimitive(final Byte[] a) {
        byte[] arr = new byte[a.length];
        for (int i = 0; i < a.length; i++) {
            arr[i] = a[i];
        }

        return arr;
    }

    /**
     * Unbox a Short array
     * @param a an array
     * @return an unboxed array
     */
    public static short[] toPrimitive(final Short[] a) {
        short[] arr = new short[a.length];
        for (int i = 0; i < a.length; i++) {
            arr[i] = a[i];
        }

        return arr;
    }

    /**
     * Unbox a Long array
     * @param a an array
     * @return an unboxed array
     */
    public static long[] toPrimitive(final Long[] a) {
        long[] arr = new long[a.length];
        for (int i = 0; i < a.length; i++) {
            arr[i] = a[i];
        }

        return arr;
    }

    /**
     * Unbox a Boolean array
     * @param a an array
     * @return an unboxed array
     */
    public static boolean[] toPrimitive(final Boolean[] a) {
        boolean[] arr = new boolean[a.length];
        for (int i = 0; i < a.length; i++) {
            arr[i] = a[i];
        }

        return arr;
    }

    /**
     * Convert a Double (or other type of the same kind) multiarray into a primitive double multiarray.
     * (Take care: it is not a high performance function !!! if you have a better implementation, send it to me...)
     * @param the array to convert, allowed types are: Double, Float, Integer, Character, Byte, Boolean, Short, Long
     * @return the primitive array.
     */
    public static Object toPrimitive(final Object a) {
        Class base = a.getClass();
        if (base.isArray()) {
            Class[] types = getPrimClasses(base);
            if (types == null) {
                return a;
            }

            if (types.length == 0) {
                try {
                    Method m = ScilabJavaArray.class.getDeclaredMethod("toPrimitive", base);
                    return m.invoke(null, a);
                } catch (Exception e) {
                    return null;
                }
            }

            return toPrimitive(a, types.length - 1, types);
        }

        return a;
    }

    /**
     * Recursively convert an array Double[][][] into an array double[][][]
     * @param a the array
     * @param pos the position in the array types
     * @param types an array containing the intermediate array class (see getPrimClass)
     * @return the converted array
     */
    private static Object toPrimitive(final Object a, int pos, Class[] types) {
        Object[] arr = (Object[]) a;
        Object[] o = (Object[]) Array.newInstance(types[pos], arr.length);

        if (pos != 0) {
            for (int i = 0; i < arr.length; i++) {
                o[i] = toPrimitive(arr[i], pos - 1, types);
            }
        } else {
            try {
                Method m = ScilabJavaArray.class.getDeclaredMethod("toPrimitive", a.getClass().getComponentType());
                for (int i = 0; i < arr.length; i++) {
                    o[i] = m.invoke(null, arr[i]);
                }
            } catch (Exception e) {
                return null;
            }
        }

        return o;
    }

    public static int[] toIntArray(double[] o) {
        int[] ret = new int[o.length];
        for (int i = 0; i < o.length; i++) {
            ret[i] = (int) o[i];
        }

        return ret;
    }

    /**
     * Box a double array
     * @param a an array
     * @return a boxed array
     */
    public static Double[] fromPrimitive(final double[] a) {
        Double[] arr = new Double[a.length];
        for (int i = 0; i < a.length; i++) {
            arr[i] = a[i];
        }

        return arr;
    }

    /**
     * Box a float array
     * @param a an array
     * @return a boxed array
     */
    public static Float[] fromPrimitive(final float[] a) {
        Float[] arr = new Float[a.length];
        for (int i = 0; i < a.length; i++) {
            arr[i] = a[i];
        }

        return arr;
    }

    /**
     * Box an int array
     * @param a an array
     * @return a boxed array
     */
    public static Integer[] fromPrimitive(final int[] a) {
        Integer[] arr = new Integer[a.length];
        for (int i = 0; i < a.length; i++) {
            arr[i] = a[i];
        }

        return arr;
    }

    /**
     * Box a char array
     * @param a an array
     * @return a boxed array
     */
    public static Character[] fromPrimitive(final char[] a) {
        Character[] arr = new Character[a.length];
        for (int i = 0; i < a.length; i++) {
            arr[i] = a[i];
        }

        return arr;
    }

    /**
     * Box a double array
     * @param a an array
     * @return a boxed array
     */
    public static Byte[] fromPrimitive(final byte[] a) {
        Byte[] arr = new Byte[a.length];
        for (int i = 0; i < a.length; i++) {
            arr[i] = a[i];
        }

        return arr;
    }

    /**
     * Box a short array
     * @param a an array
     * @return a boxed array
     */
    public static Short[] fromPrimitive(final short[] a) {
        Short[] arr = new Short[a.length];
        for (int i = 0; i < a.length; i++) {
            arr[i] = a[i];
        }

        return arr;
    }

    /**
     * Box a long array
     * @param a an array
     * @return a boxed array
     */
    public static Long[] fromPrimitive(final long[] a) {
        Long[] arr = new Long[a.length];
        for (int i = 0; i < a.length; i++) {
            arr[i] = a[i];
        }

        return arr;
    }

    /**
     * Box a boolean array
     * @param a an array
     * @return a boxed array
     */
    public static Boolean[] fromPrimitive(final boolean[] a) {
        Boolean[] arr = new Boolean[a.length];
        for (int i = 0; i < a.length; i++) {
            arr[i] = a[i];
        }

        return arr;
    }

    /**
     * Convert a double (or other type of the same kind) multiarray into a primitive Double multiarray.
     * (Take care: it is not a high performance function !!! if you have a better implementation, send it to me...)
     * @param the array to convert, allowed types are: double, float, int, char, byte, boolean, short, long
     * @return the primitive array.
     */
    public static Object fromPrimitive(final Object a) {
        Class base = a.getClass();
        if (base.isArray()) {
            Class[] types = getFromPrimClasses(base);
            if (types == null) {
                return a;
            }

            if (types.length == 0) {
                try {
                    Method m = ScilabJavaArray.class.getDeclaredMethod("fromPrimitive", base);
                    return m.invoke(null, a);
                } catch (Exception e) {
                    return null;
                }
            }

            return fromPrimitive(a, types.length - 1, types);
        }

        return a;
    }

    /**
     * Recursively convert an array double[][][] into an array Double[][][]
     * @param a the array
     * @param pos the position in the array types
     * @param types an array containing the intermediate array class (see getPrimClass)
     * @return the converted array
     */
    private static Object fromPrimitive(final Object a, int pos, Class[] types) {
        Object[] arr = (Object[]) a;
        Object[] o = (Object[]) Array.newInstance(types[pos], arr.length);

        if (pos != 0) {
            for (int i = 0; i < arr.length; i++) {
                o[i] = fromPrimitive(arr[i], pos - 1, types);
            }
        } else {
            try {
                Method m = ScilabJavaArray.class.getDeclaredMethod("fromPrimitive", a.getClass().getComponentType());
                for (int i = 0; i < arr.length; i++) {
                    o[i] = m.invoke(null, arr[i]);
                }
            } catch (Exception e) {
                return null;
            }
        }

        return o;
    }

    /**
     * Get array base type
     * @param c the Class
     * @return the base
     */
    public static Class getArrayBaseType(Class c) {
        Class base = c;
        while (base.isArray()) {
            base = base.getComponentType();
        }

        return base;
    }

    /**
     * Get array info: base Class and number of dimensions
     * @param c the Class
     * @return info
     */
    public static Object[] getArrayInfo(Class c) {
        Class base = c;
        int dims = 0;
        while (base.isArray()) {
            base = base.getComponentType();
            dims++;
        }

        return new Object[] {base, new Integer(dims)};
    }

    /**
     * Get an array of primitive arrays classes. For example, double[][][][] will give {Double[], Double[][], Double[][][]}.
     * @param c the base class
     * @return an array of classes
     */
    private static Class[] getFromPrimClasses(Class c) {
        int nbDims = 0;
        Class base = c;
        while (base.isArray()) {
            base = base.getComponentType();
            nbDims++;
        }

        base = ScilabJavaObject.primTypes.get(base);
        if (base == null) {
            return null;
        }

        Class[] cl = new Class[nbDims - 1];
        if (cl.length != 0) {
            cl[0] = Array.newInstance(base, 0).getClass();
            for (int i = 1; i < nbDims - 1; i++) {
                cl[i] = Array.newInstance(cl[i - 1], 0).getClass();
            }
        }

        return cl;
    }

    /**
     * Get an array of primitive arrays classes. For example, Double[][][][] will give {double[], double[][], double[][][]}.
     * @param c the base class
     * @return an array of classes
     */
    private static Class[] getPrimClasses(Class c) {
        int nbDims = 0;
        Class base = c;
        while (base.isArray()) {
            base = base.getComponentType();
            nbDims++;
        }

        base = mappings.get(base);
        if (base == null) {
            return null;
        }

        Class[] cl = new Class[nbDims - 1];
        if (cl.length != 0) {
            cl[0] = Array.newInstance(base, 0).getClass();
            for (int i = 1; i < nbDims - 1; i++) {
                cl[i] = Array.newInstance(cl[i - 1], 0).getClass();
            }
        }

        return cl;
    }

    /**
     * Convert a bi-dimensional array into one-dim one
     * @param x the bi-dim array
     * @return a one-dim array
     */
    public static byte[] toOneDim(byte[][] x) {
        if (x.length == 0 || x[0].length == 0) {
            return new byte[0];
        }

        final int len = x[0].length;
        final byte[] ret = new byte[x.length * len];
        for (int i = 0, s = 0; i < x.length; i++, s += len) {
            System.arraycopy(x[i], 0, ret, s, len);
        }

        return ret;
    }

    /**
     * Convert a bi-dimensional array into one-dim one
     * @param x the bi-dim array
     * @return a one-dim array
     */
    public static short[] toOneDim(short[][] x) {
        if (x.length == 0 || x[0].length == 0) {
            return new short[0];
        }

        final int len = x[0].length;
        final short[] ret = new short[x.length * len];
        for (int i = 0, s = 0; i < x.length; i++, s += len) {
            System.arraycopy(x[i], 0, ret, s, len);
        }

        return ret;
    }

    /**
     * Convert a bi-dimensional array into one-dim one
     * @param x the bi-dim array
     * @return a one-dim array
     */
    public static int[] toOneDim(int[][] x) {
        if (x.length == 0 || x[0].length == 0) {
            return new int[0];
        }

        final int len = x[0].length;
        final int[] ret = new int[x.length * len];
        for (int i = 0, s = 0; i < x.length; i++, s += len) {
            System.arraycopy(x[i], 0, ret, s, len);
        }

        return ret;
    }

    /**
     * Convert a bi-dimensional array into one-dim one
     * @param x the bi-dim array
     * @return a one-dim array
     */
    public static long[] toOneDim(long[][] x) {
        if (x.length == 0 || x[0].length == 0) {
            return new long[0];
        }

        final int len = x[0].length;
        final long[] ret = new long[x.length * len];
        for (int i = 0, s = 0; i < x.length; i++, s += len) {
            System.arraycopy(x[i], 0, ret, s, len);
        }

        return ret;
    }

    /**
     * Convert a bi-dimensional array into one-dim one
     * @param x the bi-dim array
     * @return a one-dim array
     */
    public static float[] toOneDim(float[][] x) {
        if (x.length == 0 || x[0].length == 0) {
            return new float[0];
        }

        final int len = x[0].length;
        final float[] ret = new float[x.length * len];
        for (int i = 0, s = 0; i < x.length; i++, s += len) {
            System.arraycopy(x[i], 0, ret, s, len);
        }

        return ret;
    }

    /**
     * Convert a bi-dimensional array into one-dim one
     * @param x the bi-dim array
     * @return a one-dim array
     */
    public static double[] toOneDim(double[][] x) {
        if (x.length == 0 || x[0].length == 0) {
            return new double[0];
        }

        final int len = x[0].length;
        final double[] ret = new double[x.length * len];
        for (int i = 0, s = 0; i < x.length; i++, s += len) {
            System.arraycopy(x[i], 0, ret, s, len);
        }

        return ret;
    }

    /**
     * Convert a bi-dimensional array into one-dim one
     * @param x the bi-dim array
     * @return a one-dim array
     */
    public static char[] toOneDim(char[][] x) {
        if (x.length == 0 || x[0].length == 0) {
            return new char[0];
        }

        final int len = x[0].length;
        final char[] ret = new char[x.length * len];
        for (int i = 0, s = 0; i < x.length; i++, s += len) {
            System.arraycopy(x[i], 0, ret, s, len);
        }

        return ret;
    }

    /**
     * Convert a bi-dimensional array into one-dim one
     * @param x the bi-dim array
     * @return a one-dim array
     */
    static Object toOneDim(Object x) {
        Class base = getArrayBaseType(x.getClass());

        if (base.isPrimitive()) {
            try {
                Method m = ScilabJavaArray.class.getDeclaredMethod("toOneDim", x.getClass());
                return m.invoke(null, x);
            } catch (Exception e) {
                return null;
            }
        } else {
            Object[][] xx = (Object[][]) x;

            if (xx.length == 0 || xx[0].length == 0) {
                return Array.newInstance(base, 0);
            }

            final int len = xx[0].length;
            final Object ret = Array.newInstance(base, xx.length * len);
            for (int i = 0, s = 0; i < xx.length; i++, s += len) {
                System.arraycopy(xx[i], 0, ret, s, len);
            }

            return ret;
        }
    }

    /**
     * Convert a one-dimensional array into bi-dim one
     * @param x the one-dim array
     * @return a bi-dim array
     */
    static Object toBiDim(Object x) {
        Object ret = Array.newInstance(x.getClass(), 1);
        Array.set(ret, 0, x);

        return ret;
    }

    /**
     * Convert a one-dimensional array into bi-dim one
     * @param x the one-dim array
     * @return a bi-dim array
     */
    static Object singleToOneDim(Class base, Object x) {
        Object ret = Array.newInstance(base, 1);
        Array.set(ret, 0, x);

        return ret;
    }

    /**
     * Convert a list to an array of primitive type
     * @return an array
     */
    public static double[] toDoubleArray(List<Double> l) {
        double[] arr = new double[l.size()];
        int i = 0;
        for (Double d : l) {
            arr[i++] = d;
        }

        return arr;
    }

    /**
     * Convert a list to an array of primitive type
     * @return an array
     */
    public static float[] toFloatArray(List<Float> l) {
        float[] arr = new float[l.size()];
        int i = 0;
        for (Float f : l) {
            arr[i++] = f;
        }

        return arr;
    }

    /**
     * Convert a list to an array of primitive type
     * @return an array
     */
    public static byte[] toByteArray(List<Byte> l) {
        byte[] arr = new byte[l.size()];
        int i = 0;
        for (Byte b : l) {
            arr[i++] = b;
        }

        return arr;
    }

    /**
     * Convert a list to an array of primitive type
     * @return an array
     */
    public static short[] toShortArray(List<Short> l) {
        short[] arr = new short[l.size()];
        int i = 0;
        for (Short s : l) {
            arr[i++] = s;
        }

        return arr;
    }

    /**
     * Convert a list to an array of primitive type
     * @return an array
     */
    public static int[] toIntArray(List<Integer> l) {
        int[] arr = new int[l.size()];
        int i = 0;
        for (Integer in : l) {
            arr[i++] = in;
        }

        return arr;
    }

    /**
     * Convert a list to an array of primitive type
     * @return an array
     */
    public static long[] toLongArray(List<Long> l) {
        long[] arr = new long[l.size()];
        int i = 0;
        for (Long lo : l) {
            arr[i++] = lo;
        }

        return arr;
    }

    /**
     * Convert a list to an array of primitive type
     * @return an array
     */
    public static String[] toStringArray(List<String> l) {
        return l.toArray(new String[l.size()]);
    }

    /**
     * Convert a list to an array of primitive type
     * @return an array
     */
    public static boolean[] toBooleanArray(List<Boolean> l) {
        boolean[] arr = new boolean[l.size()];
        int i = 0;
        for (Boolean b : l) {
            arr[i++] = b;
        }

        return arr;
    }

    /**
     * Convert a list to an array of primitive type
     * @return an array
     */
    public static char[] toCharArray(List<Character> l) {
        char[] arr = new char[l.size()];
        int i = 0;
        for (Character c : l) {
            arr[i++] = c;
        }

        return arr;
    }

    /**
     * Convert a list to an array of primitive type
     * @return an array
     */
    public static List toList(Object obj) {
        if (obj.getClass().isArray()) {
            Class base = obj.getClass().getComponentType();
            if (ScilabJavaObject.primTypes.containsKey(base)) {
                obj = fromPrimitive(obj);
            }
            ArrayList list = new ArrayList();
            try {
                Field _elementData = ArrayList.class.getDeclaredField("elementData");
                Field _size = ArrayList.class.getDeclaredField("size");
                _elementData.setAccessible(true);
                _size.setAccessible(true);
                _elementData.set(list, obj);
                _size.set(list, ((Object[]) obj).length);
            } catch (Exception e) {
                e.printStackTrace();
            }

            return list;
        }

        List l = new LinkedList();
        l.add(obj);

        return l;
    }
}
