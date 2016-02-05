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

import java.beans.MethodDescriptor;
import java.lang.reflect.Array;
import java.lang.reflect.Constructor;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * A constructor wrapper
 *
 * @author Calixte DENIZET
 */
public final class FunctionArguments {

    private static final List<Converter> converters = new ArrayList<Converter>();

    static {
        // Converter to convert a number to an int
        registerConverter(new Converter() {
            @Override
            public Object convert(Object original, Class<?> to) {
                return ((Number) original).intValue();
            }

            @Override
            public boolean canConvert(Class<?> from, Class<?> to) {
                return (to == int.class || to == Integer.class) && (Number.class.isAssignableFrom(from) || ScilabJavaObject.primTypes.containsKey(from));
            }
        });

        // Converter to convert a double to a float
        registerConverter(new Converter() {
            @Override
            public Object convert(Object original, Class<?> to) {
                return ((Number) original).floatValue();
            }

            @Override
            public boolean canConvert(Class<?> from, Class<?> to) {
                return (to == float.class || to == Float.class) && (from == double.class || from == Double.class);
            }
        });

        // Converter to convert a double[] to an int[]
        registerConverter(new Converter() {
            @Override
            public Object convert(Object original, Class<?> to) {
                return ScilabJavaArray.toIntArray((double[]) original);
            }

            @Override
            public boolean canConvert(Class<?> from, Class<?> to) {
                return (to == int[].class) && (from == double[].class);
            }
        });

        // Converter to convert a double[] to a Double[] (or an other primitive type)
        registerConverter(new Converter() {
            @Override
            public Object convert(Object original, Class<?> to) {
                return ScilabJavaArray.fromPrimitive(original);
            }

            @Override
            public boolean canConvert(Class<?> from, Class<?> to) {
                if (from.isArray() && to.isArray()) {
                    final Class _from = ScilabJavaArray.getArrayBaseType(from);
                    final Class _to = ScilabJavaArray.getArrayBaseType(to);
                    return ScilabJavaObject.primTypes.get(_from) == _to;
                }
                return false;
            }
        });

        // Converter to convert a double[] to a ArrayList<Double> (or an other primitive type)
        registerConverter(new Converter() {
            @Override
            public Object convert(Object original, Class<?> to) {
                return ScilabJavaArray.toList(original);
            }

            @Override
            public boolean canConvert(Class<?> from, Class<?> to) {
                return from.isArray() && to.isAssignableFrom(ArrayList.class);
            }
        });

        // Converter to convert a String to a Enum
        registerConverter(new Converter() {
            @Override
            public Object convert(Object original, Class<?> to) {
                return Enum.valueOf((Class) to, (String) original);
            }

            @Override
            public boolean canConvert(Class<?> from, Class<?> to) {
                return String.class.isAssignableFrom(from) && to.isEnum();
            }
        });

        // Converter to convert double[][] to double[]
        registerConverter(new Converter() {
            @Override
            public Object convert(Object original, Class<?> to) {
                return ScilabJavaArray.toOneDim(original);
            }

            @Override
            public boolean canConvert(Class<?> from, Class<?> to) {
                return to.isArray() && from.isArray() && from.getComponentType().isArray() && to.getComponentType() == from.getComponentType().getComponentType();
            }
        });

        // Converter to convert double[] to double[][]
        registerConverter(new Converter() {
            @Override
            public Object convert(Object original, Class<?> to) {
                return ScilabJavaArray.toBiDim(original);
            }

            @Override
            public boolean canConvert(Class<?> from, Class<?> to) {
                return to.isArray() && from.isArray() && to.getComponentType().isArray() && from.getComponentType() == to.getComponentType().getComponentType();
            }
        });

        // Converter to convert double to double[]
        registerConverter(new Converter() {
            @Override
            public Object convert(Object original, Class<?> to) {
                return ScilabJavaArray.singleToOneDim(to.getComponentType(), original);
            }

            @Override
            public boolean canConvert(Class<?> from, Class<?> to) {
                return to.isArray() && from == to.getComponentType();
            }
        });

        // Converter to convert Double to double[]
        registerConverter(new Converter() {
            @Override
            public Object convert(Object original, Class<?> to) {
                return ScilabJavaArray.singleToOneDim(to.getComponentType(), original);
            }

            @Override
            public boolean canConvert(Class<?> from, Class<?> to) {
                return to.isArray() && ScilabJavaArray.mappings.get(from) == to.getComponentType();
            }
        });
    }

    /**
     * Register a converter
     * @param converter the converter to register
     */
    public static final void registerConverter(Converter converter) {
        int index = converters.indexOf(converter);
        if (index == -1) {
            converters.add(converter);
        } else {
            converters.remove(index);
            converters.add(converter);
        }
    }

    /**
     * Unregister a converter
     * @param converter the converter to unregister
     */
    public static final void unregisterConverter(Converter converter) {
        int index = converters.indexOf(converter);
        if (index != -1) {
            converters.remove(index);
        }
    }

    /**
     * Convert an object x into another one according to base Class
     * @param x the object to convert
     * @param base the base Class
     * @return the converted object
     */
    public static final Object convert(Object x, Class base) {
        if (x == null) {
            return null;
        }
        final Class clazz = x.getClass();
        if (base.isAssignableFrom(clazz)) {
            return x;
        }
        for (Converter converter : converters) {
            if (converter.canConvert(clazz, base)) {
                return converter.convert(x, base);
            }
        }

        return x;
    }

    /**
     * To find the "correct" method we proceed as follow:
     * i) We calculate the distance between the Class of the arguments.
     *    For a Class B which derivates from A (A.isAssignableFrom(B) == true), the distance between A and B is the number of
     *    derivations of A to obtain B. For example, if we have C extends B extends A, dist(C,A)=2.
     *    So the square of the distance between two Class array is defined as the sum of the square of the distance between
     *    Class elements.
     *    In the particular case where a int.class is required and the argument is double.class, if the double value is an integer
     *    the argument is converted into an int. In this case, dist(int.class, double.class) is equal to 2048. This value has been
     *    chosen to be "sure" that another method with double.class will be considered better than the previous one.
     *    If the method has variable arguments foo(A a, B b, Object...), the Java reflection only shows that Class arguments are
     *    A.class, B.class and Object[].class. So such a method will give a very high distance (more than 2^40).
     *    When a method is not correct, the distance will be Long.MIN_VALUE.
     *
     * ii) For each Method we calculate the distance and we try to find the method with the minimal distance.
     * iii) If a method has variable arguments, then we transform the arguments and return the new ones in the second element of
     *      the returned array.
     */

    /**
     * Find a valid method. An argument can be null (thanks to Fabien Viale).
     * @param name the method name
     * @param functions the set of the valid functions in the class
     * @param argsClass the class of the arguments
     * @param args the arguments
     * @return a valid method wrapped in an array or a method and the modified args (variable args) wrapped in an array
     */
    public static final Object[] findMethod(String name, MethodDescriptor[] descriptors, Class[] argsClass, Object[] args) throws NoSuchMethodException {
        String internName = name.intern();
        Method better = null;
        boolean mustConv = false;
        boolean isVarArgs = false;
        long sqd = Long.MAX_VALUE;
        boolean[] refBools = new boolean[1];
        Map<Integer, Converter> toConvert = null;
        Map<Integer, Converter> toConv = new HashMap<Integer, Converter>();
        for (MethodDescriptor desc : descriptors) {
            if (desc.getName() == internName) {
                Method f = desc.getMethod();
                Class[] types = f.getParameterTypes();
                refBools[0] = false;
                long d = compareClassArgs(types, argsClass, args, refBools, toConv);
                if (d != Long.MIN_VALUE && d < sqd) {
                    // The method is valid and the distance is lesser than the previous found one.
                    sqd = d;
                    better = f;
                    isVarArgs = refBools[0];
                    toConvert = toConv;
                    if (d == 0) {
                        break;
                    }

                    toConv = new HashMap<Integer, Converter>();
                } else {
                    toConv.clear();
                }
            }
        }

        if (better != null) {
            if (!toConvert.isEmpty()) {
                // Contains int.class arguments and we passed double.class args
                Class[] types = better.getParameterTypes();
                Class base = types[types.length - 1].getComponentType();
                for (Map.Entry<Integer, Converter> entry : toConvert.entrySet()) {
                    int i = entry.getKey();
                    if (i >= 0) {
                        argsClass[i] = types[i];
                        args[i] = entry.getValue().convert(args[i], types[i]);
                    } else {
                        i = -i - 1;
                        args[i] = entry.getValue().convert(args[i], base);
                    }
                }
            }

            if (isVarArgs) {
                // Variable arguments
                Class[] types = better.getParameterTypes();
                Object o;
                if (args.length == 1 && types.length == 1 && args[0] == null) {
                    o = null;
                } else {
                    Class base = types[types.length - 1].getComponentType();
                    o = Array.newInstance(base, args.length - types.length + 1);

                    // Don't use System.arraycopy since it does not handle unboxing
                    for (int i = 0; i < args.length - types.length + 1; i++) {
                        Array.set(o, i, args[i + types.length - 1]);
                    }
                }

                Object[] newArgs = new Object[types.length];
                System.arraycopy(args, 0, newArgs, 0, types.length - 1);
                newArgs[types.length - 1] = o;

                return new Object[] {better, newArgs};
            }

            return new Object[] {better};
        }

        throw new NoSuchMethodException("");
    }

    /**
     * Find a valid method. An argument can be null (thanks to Fabien Viale).
     * @param functions the set of the valid constructors in the class
     * @param argsClass the class of the arguments
     * @param args the arguments
     * @return a valid method wrapped in an array or a method and the modified args (variable args) wrapped in an array
     */
    public static final Object[] findConstructor(Constructor[] functions, Class[] argsClass, Object[] args) throws NoSuchMethodException {
        // Essentially a clone of findMethod.
        Constructor better = null;
        boolean mustConv = false;
        boolean isVarArgs = false;
        long sqd = Long.MAX_VALUE;
        boolean[] refBools = new boolean[1];
        Map<Integer, Converter> toConvert = null;
        Map<Integer, Converter> toConv = new HashMap<Integer, Converter>();
        for (Constructor f : functions) {
            if (Modifier.isPublic(f.getModifiers())) {
                Class[] types = f.getParameterTypes();
                refBools[0] = false;
                long d = compareClassArgs(types, argsClass, args, refBools, toConv);
                if (d != Long.MIN_VALUE && d < sqd) {
                    // The constructor is valid and the distance is lesser than the previous found one.
                    sqd = d;
                    better = f;
                    isVarArgs = refBools[0];
                    toConvert = toConv;
                    if (d == 0) {
                        break;
                    }

                    toConv = new HashMap<Integer, Converter>();
                } else {
                    toConv.clear();
                }
            }
        }

        if (better != null) {
            if (!toConvert.isEmpty()) {
                // Contains int.class arguments and we passed double.class args
                Class[] types = better.getParameterTypes();
                Class base = types[types.length - 1].getComponentType();
                for (Map.Entry<Integer, Converter> entry : toConvert.entrySet()) {
                    int i = entry.getKey();
                    if (i >= 0) {
                        argsClass[i] = types[i];
                        args[i] = entry.getValue().convert(args[i], types[i]);
                    } else {
                        i = -i - 1;
                        args[i] = entry.getValue().convert(args[i], base);
                    }
                }
            }
            if (isVarArgs) {
                // Variable arguments
                Class[] types = better.getParameterTypes();
                Object o;
                if (args.length == 1 && types.length == 1 && args[0] == null) {
                    o = null;
                } else {
                    Class base = types[types.length - 1].getComponentType();
                    o = Array.newInstance(base, args.length - types.length + 1);
                    // Don't use System.arraycopy since it does not handle unboxing
                    for (int i = 0; i < args.length - types.length + 1; i++) {
                        Array.set(o, i, args[i + types.length - 1]);
                    }
                }

                Object[] newArgs = new Object[types.length];
                System.arraycopy(args, 0, newArgs, 0, types.length - 1);
                newArgs[types.length - 1] = o;

                return new Object[] {better, newArgs};
            }

            return new Object[] {better};
        }

        throw new NoSuchMethodException("");
    }

    /**
     * This function calculates the distance between the method signatures
     * @param A the method signature
     * @param B the class of the passed arguments
     * @param arr array of arguments (used to transform double in int)
     * @param bools references on boolean
     * @return the distance
     */
    private static final long compareClassArgs(Class[] A, Class[] B, Object[] arr, boolean[] bools, Map<Integer, Converter> toConvert) {
        if (A.length > B.length) {
            return Long.MIN_VALUE;
        }

        long s = 0;
        int end = A.length;
        if (A.length > 0 && A[A.length - 1].isArray() && (A.length < B.length || (A.length == 1 && B.length == 1 && (B[0] == null || !B[0].isArray())))) {
            Class base = A[A.length - 1].getComponentType();
            // this is a variable arguments method
            bools[0] = true;
            end--;
            s = 1 << 40;

            for (int i = A.length - 1; i < B.length; i++) {
                long d = dist(base, B[i]);
                if (d == -1) {
                    for (Converter converter : converters) {
                        if (converter.canConvert(B[i], base)) {
                            d = 2048;
                            toConvert.put(-i - 1, converter);
                            break;
                        }
                    }

                    if (d != 2048) {
                        return Long.MIN_VALUE;
                    }
                }
                // s is the sum of the square of the distance
                s += d * d;
            }
        } else if (A.length < B.length) {
            return Long.MIN_VALUE;
        }

        for (int i = 0; i < end; i++) {
            long d = dist(A[i], B[i]);
            if (d == -1) {
                for (Converter converter : converters) {
                    if (converter.canConvert(B[i], A[i])) {
                        d = 2048;
                        toConvert.put(i, converter);
                        break;
                    }
                }

                if (d != 2048) {
                    return Long.MIN_VALUE;
                }
            }
            // s is the sum of the square of the distance
            s += d * d;
        }

        return s;
    }

    /**
     * Calculate the distance between two classes.
     * If B derivate from A, then the distance is the number of derivations.
     * @param A a class
     * @param B another class
     * @return the distance
     */
    private static final long dist(Class<?> A, Class<?> B) {
        if (B == null) {
            return 0;
        }

        if (!A.isPrimitive() && B.isPrimitive()) {
            // Autounboxing
            B = ScilabJavaObject.primTypes.get(B);
        }

        if (A == B) {
            return 0;
        }

        if (A.isAssignableFrom(B)) {
            long i = 0;
            do {
                i++;
                B = B.getSuperclass();
            } while (B != null && A.isAssignableFrom(B));

            return i;
        }

        return -1;
    }
}
