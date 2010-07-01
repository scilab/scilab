/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;

/**
 * This is a static class to access all controller capabilities
 * from C/C++ code through JNI
 * See SCI/modules/graphic_objects/src/jni/graphic_objects.giws.xml for other details.
 **/
public final class CallGraphicController {

    public static String askGraphicObject(String typeName) {
        return GraphicController.getController().askObject(GraphicObject.getTypeFromName(typeName));
    }

    private static boolean setGraphicObjectProperty(String id, String propertyName, Object value) {
        return GraphicController.getController().setPropertyFast(id, propertyName, value);
    }

    private static Object getGraphicObjectProperty(String id, String propertyName) {
        return GraphicController.getController().getPropertyFast(id, propertyName);
    }

    public static boolean setGraphicObjectProperty(String id, String propertyName, String value) {
        return setGraphicObjectProperty(id, propertyName, (Object) value);
    }

    public static boolean setGraphicObjectProperty(String id, String propertyName, String[] value) {
        return setGraphicObjectProperty(id, propertyName, (Object) value);
    }

    public static boolean setGraphicObjectProperty(String id, String propertyName, double value) {
        return setGraphicObjectProperty(id, propertyName, (Object) new Double(value));
    }

    public static boolean setGraphicObjectProperty(String id, String propertyName, double[] value) {
        Double[] array = new Double[value.length];

        for (int i = 0; i < value.length; i++) {
            array[i] = value[i];
        }

        return setGraphicObjectProperty(id, propertyName, (Object) array);
    }


    public static boolean setGraphicObjectProperty(String id, String propertyName, int value) {
        return setGraphicObjectProperty(id, propertyName, (Object) new Integer(value));
    }

    public static boolean setGraphicObjectProperty(String id, String propertyName, int[] value) {
        Integer[] array = new Integer[value.length];

        for (int i = 0; i < value.length; i++) {
            array[i] = value[i];
        }

        return setGraphicObjectProperty(id, propertyName, (Object) array);
    }

    public static boolean setGraphicObjectProperty(String id, String propertyName, boolean value) {
        return setGraphicObjectProperty(id, propertyName, (Object) new Boolean(value));
    }

    public static boolean setGraphicObjectProperty(String id, String propertyName, boolean[] value) {
	Boolean[] array = new Boolean[value.length];

        for (int i = 0; i < value.length; i++) {
            array[i] = value[i];
        }

        return setGraphicObjectProperty(id, propertyName, (Object) array);
    }

    public static String getGraphicObjectPropertyAsString(String id, String propertyName) {
        return (String) getGraphicObjectProperty(id, propertyName);
    }

    public static String[] getGraphicObjectPropertyAsStringVector(String id, String propertyName) {
        return (String[]) getGraphicObjectProperty(id, propertyName);
    }

    public static double getGraphicObjectPropertyAsDouble(String id, String propertyName) {
	return (Double) getGraphicObjectProperty(id, propertyName);
    }

    public static double[] getGraphicObjectPropertyAsDoubleVector(String id, String propertyName) {
        Double[] tmp = (Double[]) getGraphicObjectProperty(id, propertyName);

        double[] result = new double[tmp.length];
        for(int i = 0; i < result.length; i++) {
            result[i] = tmp[i];
        }

        return result;
    }

    public static int getGraphicObjectPropertyAsInteger(String id, String propertyName) {
        return (Integer) getGraphicObjectProperty(id, propertyName);
    }

    public static int[] getGraphicObjectPropertyAsIntegerVector(String id, String propertyName) {
        Integer[] tmp = (Integer[]) getGraphicObjectProperty(id, propertyName);

        int[] result = new int[tmp.length];

        for (int i = 0; i < result.length; i++) {
            result[i] = tmp[i];
        }

        return result;
    }

    public static boolean getGraphicObjectPropertyAsBoolean(String id, String propertyName) {
	return (Boolean) getGraphicObjectProperty(id, propertyName);
    }

    public static boolean[] getGraphicObjectPropertyAsBooleanVector(String id, String propertyName) {
        Boolean[] tmp = (Boolean[]) getGraphicObjectProperty(id, propertyName);

        boolean[] result = new boolean[tmp.length];

        for (int i = 0; i < result.length; i++) {
            result[i] = tmp[i];
        }

        return result;
    }

}
