/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.tests.modules.xcos.graph;

import java.awt.GraphicsEnvironment;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.beans.PropertyVetoException;
import java.beans.VetoableChangeListener;
import java.lang.reflect.Array;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.ArrayList;
import java.util.List;

import org.junit.Before;
import org.junit.Test;
import org.scilab.modules.xcos.graph.ScicosParameters;

/**
 * Test the behavior of the {@link ScicosParameters} class.
 *
 * We are checking the class behavior by using introspection.
 */
public class ScicosParametersTest {
    private static final String[] FIELDS;
    private static final Field[] DEFAULT_VALUES;
    private static final Method[] GETTERS;
    private static final Method[] SETTERS;

    @Before
    public void loadLibrary() {
        System.loadLibrary("scilab");
    }

    /**
     * Initialize FIELDS, GETTERS and SETTERS and DEFAULT_VALUES.
     */
    static {
        ArrayList<String> fields = new ArrayList<String>();
        ArrayList<Method> setters = new ArrayList<Method>();
        ArrayList<Method> getters = new ArrayList<Method>();
        ArrayList<Field> default_values = new ArrayList<Field>();
        /*
         * First pass for getting SETTERS and FIELDS
         */
        for (Method m : ScicosParameters.class.getMethods()) {
            if (m.getName().startsWith("set")) {
                char[] field = m.getName().replaceFirst("set", "").toCharArray();
                char firstChar = field[0];
                field[0] = Character.toLowerCase(firstChar);

                fields.add(new String(field));
                setters.add(m);
            }
        }

        /*
         * Second pass for getting GETTERS and DEFAULT_VALUES
         */
        for (String f : fields) {
            /*
             * Getters
             */
            StringBuilder getter = new StringBuilder(f);
            char firstChar = f.charAt(0);
            getter.delete(0, 1);
            getter.insert(0, Character.toUpperCase(firstChar));
            getter.insert(0, "get");
            String name = getter.toString();

            for (Method m : ScicosParameters.class.getMethods()) {
                if (m.getName().equals(name)) {
                    getters.add(m);
                    break;
                }
            }

            /*
             * Default values
             */
            StringBuilder constant = new StringBuilder(f);
            for (int i = 0; i < constant.length(); i++) {
                char c = constant.charAt(i);
                if (Character.isUpperCase(c)) {
                    constant.insert(i, "_");
                    i++;
                }
            }
            String field = constant.toString().toUpperCase();
            try {
                Field defaultValue = ScicosParameters.class.getDeclaredField(field);
                default_values.add(defaultValue);
            } catch (SecurityException e) {
                throw new IllegalAccessError(e.toString());
            } catch (NoSuchFieldException e) {
                throw new IllegalAccessError(e.toString());
            }
        }

        FIELDS = fields.toArray(new String[fields.size()]);
        SETTERS = setters.toArray(new Method[setters.size()]);
        GETTERS = getters.toArray(new Method[getters.size()]);
        DEFAULT_VALUES = default_values.toArray(new Field[default_values.size()]);
    }

    @Test
    public void checkNumberOfField() {
        assert FIELDS.length == SETTERS.length;
        assert FIELDS.length == GETTERS.length;
        assert FIELDS.length == DEFAULT_VALUES.length;
    }

    @Test
    public void checkInitValues() throws IllegalArgumentException, IllegalAccessException, InvocationTargetException {
        if (GraphicsEnvironment.isHeadless()) {
            return;
        }

        ScicosParameters obj = new ScicosParameters();

        for (int i = 0; i < DEFAULT_VALUES.length; i++) {
            Field f = DEFAULT_VALUES[i];
            Method m = GETTERS[i];

            final Object result = m.invoke(obj, (Object[]) null);
            final Object reference = f.get(null);
            assert result.equals(reference);
        }
    }

    @Test
    public void checkBeansNamedProperties() throws SecurityException, NoSuchFieldException, IllegalArgumentException, IllegalAccessException {
        if (GraphicsEnvironment.isHeadless()) {
            return;
        }

        for (int i = 0; i < DEFAULT_VALUES.length; i++) {
            Field f = DEFAULT_VALUES[i];
            String field = FIELDS[i];

            // Check existence (will throw an exception)
            Field change = ScicosParameters.class.getDeclaredField(f.getName() + "_CHANGE");

            // Check value (field)
            assert change.get(null).equals(field);
        }
    }

    @Test
    public void checkBeansPropertyChangeEvent() throws IllegalArgumentException, IllegalAccessException, InvocationTargetException, InstantiationException {
        if (GraphicsEnvironment.isHeadless()) {
            return;
        }

        ScicosParameters obj = new ScicosParameters();
        final List<PropertyChangeEvent> receivedEvents = new ArrayList<PropertyChangeEvent>();

        obj.addPropertyChangeListener(new PropertyChangeListener() {
            @Override
            public void propertyChange(PropertyChangeEvent evt) {
                receivedEvents.add(evt);
            }
        });

        /*
         * Update fields
         */
        for (int i = 0; i < DEFAULT_VALUES.length; i++) {
            final Method set = SETTERS[i];
            final Field defaultValue = DEFAULT_VALUES[i];

            Object newValue = null;
            if (defaultValue.getType().equals(double.class)) {
                newValue = defaultValue.getDouble(null) + 1;
            } else if (defaultValue.getType().equals(int.class)) {
                newValue = defaultValue.getInt(null) + 1;
            } else if (defaultValue.getType().equals(String.class)) {
                newValue = ((String) defaultValue.get(null)).concat("1");
            } else if (defaultValue.getType().isArray()) {
                Object array = defaultValue.get(null);
                final Class <? extends Object > type = defaultValue.getType().getComponentType();
                final Object value;

                final int arrayLength = Array.getLength(array);
                value = Array.newInstance(type, arrayLength + 1);

                /*
                 * Copying values
                 */
                for (int j = 0; j < arrayLength; j++) {
                    Array.set(value, j, Array.get(array, j));
                }

                /*
                 * The latest value must be equal to the previous one or to the
                 * type's default value
                 */
                final Object o;
                if (arrayLength > 0) {
                    o = Array.get(array, arrayLength - 1);
                } else {
                    o = type.newInstance();
                }
                Array.set(value, arrayLength, o);

                newValue = value;
            }

            set.invoke(obj, newValue);
        }

        /*
         * Check for modification events
         */
        for (int i = 0; i < DEFAULT_VALUES.length; i++) {
            final Method get = GETTERS[i];
            final PropertyChangeEvent evt = receivedEvents.get(i);

            Object newValue = get.invoke(obj);
            assert newValue.equals(evt.getNewValue());
        }
    }

    @Test
    public void checkBeansVetoableChangeEvent() throws IllegalArgumentException, IllegalAccessException, InstantiationException {
        if (GraphicsEnvironment.isHeadless()) {
            return;
        }

        ScicosParameters obj = new ScicosParameters();
        final List<PropertyVetoException> receivedExceptions = new ArrayList<PropertyVetoException>();

        obj.addVetoableChangeListener(new VetoableChangeListener() {
            @Override
            public void vetoableChange(PropertyChangeEvent evt) throws PropertyVetoException {
                // always throw exception
                throw new PropertyVetoException(evt.getPropertyName(), evt);
            }
        });

        /*
         * Update fields
         */
        for (int i = 0; i < DEFAULT_VALUES.length; i++) {
            final Method set = SETTERS[i];
            final Field defaultValue = DEFAULT_VALUES[i];

            Object newValue = null;
            if (defaultValue.getType().equals(double.class)) {
                newValue = defaultValue.getDouble(null) + 1;
            } else if (defaultValue.getType().equals(int.class)) {
                newValue = defaultValue.getInt(null) + 1;
            } else if (defaultValue.getType().equals(String.class)) {
                newValue = ((String) defaultValue.get(null)).concat("1");
            } else if (defaultValue.getType().isArray()) {
                Object array = defaultValue.get(null);
                final Class <? extends Object > type = defaultValue.getType().getComponentType();
                final Object value;

                final int arrayLength = Array.getLength(array);
                value = Array.newInstance(type, arrayLength + 1);

                /*
                 * Copying values
                 */
                for (int j = 0; j < arrayLength; j++) {
                    Array.set(value, j, Array.get(array, j));
                }

                /*
                 * The latest value must be equal to the previous one or to the
                 * type's default value
                 */
                final Object o;
                if (arrayLength > 0) {
                    o = Array.get(array, arrayLength - 1);
                } else {
                    o = type.newInstance();
                }
                Array.set(value, arrayLength, o);

                newValue = value;
            }

            try {
                set.invoke(obj, newValue);
            } catch (InvocationTargetException e) {
                receivedExceptions.add((PropertyVetoException) e.getTargetException());
            }
        }

        /*
         * Check for received PropertyVetoException
         */
        assert receivedExceptions.size() == DEFAULT_VALUES.length;
        for (int i = 0; i < DEFAULT_VALUES.length; i++) {
            final String field = FIELDS[i];
            final PropertyVetoException e = receivedExceptions.get(i);

            assert e.getMessage().equals(field);
        }
    }

    /**
     * Call all public methods through introspection
     *
     * @param args
     *            not used
     * @throws InvocationTargetException
     * @throws IllegalAccessException
     * @throws IllegalArgumentException
     */
    public static void main(String[] args) throws IllegalArgumentException, IllegalAccessException, InvocationTargetException {
        Object obj = new ScicosParametersTest();
        Method[] tests = ScicosParametersTest.class.getDeclaredMethods();
        for (Method method : tests) {
            int modifiers = method.getModifiers();
            if ((modifiers | Modifier.STATIC) != modifiers) {
                method.invoke(obj, (Object[]) null);
            }
        }
    }
}
