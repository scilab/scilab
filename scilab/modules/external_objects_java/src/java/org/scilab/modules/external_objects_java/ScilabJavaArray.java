/*
 * JIMS ( http://forge.scilab.org/index.php/p/JIMS/ ) - This file is a part of JIMS
 * Copyright (C) 2010 - 2011 - Calixte DENIZET <calixte@contrib.scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.external_objects_java;

import java.lang.reflect.Array;
import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.Map;
import java.util.logging.Level;

/**
 * Tools for arrays
 *
 * @author Calixte DENIZET
 */
@SuppressWarnings("serial")
public class ScilabJavaArray {

    private static final Map<Class, Class> mappings = new HashMap<Class, Class>();
    private static final Map<Class, Method> mappingsMethods = new HashMap<Class, Method>();

    private static final Class[] EMPTYC = new Class[0];
    private static final Object[] EMPTYO = new Object[0];

    static {
        mappings.put(Double.class, double.class);
        mappings.put(Integer.class, int.class);
        mappings.put(Boolean.class, boolean.class);
        mappings.put(Short.class, short.class);
        mappings.put(Byte.class, byte.class);
        mappings.put(Character.class, char.class);
        mappings.put(Long.class, long.class);
        mappings.put(Float.class, float.class);

        try {
            mappingsMethods.put(Double.class, Double.class.getMethod("doubleValue", EMPTYC));
            mappingsMethods.put(Integer.class, Integer.class.getMethod("intValue", EMPTYC));
            mappingsMethods.put(Boolean.class, Boolean.class.getMethod("booleanValue", EMPTYC));
            mappingsMethods.put(Short.class, Short.class.getMethod("shortValue", EMPTYC));
            mappingsMethods.put(Byte.class, Byte.class.getMethod("byteValue", EMPTYC));
            mappingsMethods.put(Character.class, Character.class.getMethod("charValue", EMPTYC));
            mappingsMethods.put(Long.class, Long.class.getMethod("longValue", EMPTYC));
            mappingsMethods.put(Float.class, Float.class.getMethod("floatValue", EMPTYC));
        } catch (Exception e) { }
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
                if (index[i] < Array.getLength(obj)) {
                    obj = Array.get(obj, index[i]);
                } else {
                    throw new ScilabJavaException("Problem in retrieving " + i + "-th element: " + index[i] + ">" + (Array.getLength(obj) - 1));
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
                if (index[i] < Array.getLength(obj)) {
                    obj = Array.get(obj, index[i]);
                } else {
                    throw new ScilabJavaException("Problem in retrieving " + i + "-th element: " + index[i] + ">" + (Array.getLength(obj) - 1));
                }
            } else {
                throw new ScilabJavaException("Problem in retrieving " + i + "-th element: it is not an array");
            }
        }

        if (obj != null && obj.getClass().isArray()) {
            if (index[i] < Array.getLength(obj)) {
                try {
                    Array.set(obj, index[i], x);
                } catch (IllegalArgumentException e) {
                    throw new ScilabJavaException("Array " + obj + " cannot contain object which is an instance of " + x.getClass());
                }
            } else {
                throw new ScilabJavaException("Problem in setting " + index[i] + "-th element: " + index[i] + ">" + (Array.getLength(obj) - 1));
            }
        } else {
            throw new ScilabJavaException("Problem in retrieving " + i + "-th element: it is not an array");
        }
    }

    /**
     * Convert a Double (or other type of the same kind) multiarray into a primitive double multiarray.
     * (Take care: it is not a high performance function !!! if you have a better implementation, send it to me...)
     * @param the array to convert, allowed types are: Double, Float, Integer, Character, Byte, Boolean, Short, Long
     * @return the primitive array.
     */
    public static Object toPrimitive(Object array) {
        /* TODO:
           1) Verifier qu'on peut faire mieux avec des templates
           2) Faire des tests (pas sur que ca passe...)
        */
        String info[] = getBasicType(array);
        if (info == null) {
            return null;
        }
        Class clazz;
        try {
            clazz = Class.forName(info[1]);
        } catch (ClassNotFoundException e) {
            return null;
        }

        Method method = mappingsMethods.get(clazz);
        clazz = mappings.get(clazz);
        if (clazz == null) {
            return null;
        }

        int[] dims = new int[info[0].length()];
        Object obj = array;
        for (int i = 0; i < dims.length; i++) {
            dims[i] = Array.getLength(obj);
            obj = Array.get(obj, 0);
        }

        Object ret = Array.newInstance(clazz, dims);

        int[] index = new int[dims.length - 1];
        int last = dims.length - 1;
        int pos = dims.length - 1;
        try {
            while (true) {
                Object arrG = array;
                Object arrS = ret;
                for (int i = 0; i < index.length; i++) {
                    arrG = Array.get(arrG, index[i]);
                    arrS = Array.get(arrS, index[i]);
                }
                for (int i = 0; i < dims[last]; i++) {
                    Array.set(arrS, i, method.invoke(Array.get(arrG, i), EMPTYO));
                }
                pos = dims.length - 1;
                while (--pos >= 0 && index[pos] == dims[pos]) {
                    ;
                }
                if (pos < 0) {
                    break;
                }
                index[pos]++;
                for (int i = pos + 1; i < index.length; i++) {
                    index[pos] = 0;
                }
            }
        } catch (Exception e) { }

        return ret;
    }

    /**
     * @param array an array
     * @return the Object base type or null if it is not an Object
     */
    private static final String[] getBasicType(Object array) {
        if (array != null && array.getClass().isArray()) {
            String signature = array.toString();
            String[] ret = new String[2];
            int pos = 0;
            while (signature.charAt(pos++) == '[') {
                ;
            }
            ret[0] = signature.substring(0, pos - 1);
            int semicolon = signature.indexOf(';');

            if (signature.charAt(pos - 1) == 'L') {
                ret[1] = signature.substring(pos, semicolon);
                return ret;
            }
        }

        return null;
    }
}
