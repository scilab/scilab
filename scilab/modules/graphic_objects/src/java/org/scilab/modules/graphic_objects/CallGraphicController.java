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

import org.scilab.modules.graphic_objects.console.Console;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicView.ScilabView;
import org.scilab.modules.graphic_objects.utils.MenuBarBuilder;

/**
 * This is a static class to access all controller capabilities
 * from C/C++ code through JNI
 * See SCI/modules/graphic_objects/src/jni/graphic_objects.giws.xml for other details.
 **/
public final class CallGraphicController {

    public static void setGraphicObjectRelationship(String parentId, String childId) {
        GraphicController.getController().setGraphicObjectRelationship(parentId, childId);
    }

    public static void removeRelationShipAndDelete(String parentId) {
        GraphicController.getController().removeRelationShipAndDelete(parentId);
    }

    public static String cloneGraphicObject(String id) {
        return GraphicController.getController().cloneObject(id);
    }

    public static String askGraphicObject(int typeName) {
        return GraphicController.getController().askObject(GraphicObject.getTypeFromName(typeName));
    }

    public static void deleteGraphicObject(String id) {
        GraphicController.getController().deleteObject(id);
    }

    private static boolean setGraphicObjectProperty(String id, int propertyName, Object value) {
        return GraphicController.getController().setProperty(id, propertyName, value);
    }

    private static Object getGraphicObjectProperty(String id, int propertyName) {
        return GraphicController.getController().getProperty(id, propertyName);
    }

    public static boolean setGraphicObjectProperty(String id, int propertyName, String value) {
        return setGraphicObjectProperty(id, propertyName, (Object) value);
    }

    public static boolean setGraphicObjectProperty(String id, int propertyName, String[] value) {
        return setGraphicObjectProperty(id, propertyName, (Object) value);
    }

    public static boolean setGraphicObjectProperty(String id, int propertyName, double value) {
        return setGraphicObjectProperty(id, propertyName, (Object) new Double(value));
    }

    public static boolean setGraphicObjectProperty(String id, int propertyName, double[] value) {
        Double[] array = new Double[value.length];

        for (int i = 0; i < value.length; i++) {
            array[i] = value[i];
        }

        return setGraphicObjectProperty(id, propertyName, (Object) array);
    }


    public static boolean setGraphicObjectProperty(String id, int propertyName, int value) {
        return setGraphicObjectProperty(id, propertyName, (Object) new Integer(value));
    }

    public static boolean setGraphicObjectProperty(String id, int propertyName, int[] value) {
        Integer[] array = new Integer[value.length];

        for (int i = 0; i < value.length; i++) {
            array[i] = value[i];
        }

        return setGraphicObjectProperty(id, propertyName, (Object) array);
    }

    public static boolean setGraphicObjectProperty(String id, int propertyName, boolean value) {
        return setGraphicObjectProperty(id, propertyName, (Object) new Boolean(value));
    }

    public static boolean setGraphicObjectProperty(String id, int propertyName, boolean[] value) {
        Boolean[] array = new Boolean[value.length];

        for (int i = 0; i < value.length; i++) {
            array[i] = value[i];
        }

        return setGraphicObjectProperty(id, propertyName, (Object) array);
    }

    public static String getGraphicObjectPropertyAsString(String id, int propertyName) {
        return (String) getGraphicObjectProperty(id, propertyName);
    }

    public static String[] getGraphicObjectPropertyAsStringVector(String id, int propertyName) {
        return (String[]) getGraphicObjectProperty(id, propertyName);
    }

    public static double getGraphicObjectPropertyAsDouble(String id, int propertyName) {
        return (Double) getGraphicObjectProperty(id, propertyName);
    }

    public static double[] getGraphicObjectPropertyAsDoubleVector(String id, int propertyName) {
        Double[] tmp = (Double[]) getGraphicObjectProperty(id, propertyName);

        if (tmp == null) {
            return null;
        }

        double[] result = new double[tmp.length];

        for (int i = 0; i < result.length; i++) {
            result[i] = tmp[i];
        }

        return result;
    }

    public static int getGraphicObjectPropertyAsInteger(String id, int propertyName) {
        return (Integer) getGraphicObjectProperty(id, propertyName);
    }

    public static int[] getGraphicObjectPropertyAsIntegerVector(String id, int propertyName) {
        Integer[] tmp = (Integer[]) getGraphicObjectProperty(id, propertyName);

        if (tmp == null) {
            return null;
        }

        int[] result = new int[tmp.length];

        for (int i = 0; i < result.length; i++) {
            result[i] = tmp[i];
        }

        return result;
    }

    public static int getGraphicObjectPropertyAsBoolean(String id, int propertyName) {
        int result;

        Boolean tmpValue = (Boolean) getGraphicObjectProperty(id, propertyName);

        result = Boolean.TRUE.equals(tmpValue) ? 1 : 0;

        return result;
    }

    public static int[] getGraphicObjectPropertyAsBooleanVector(String id, int propertyName) {
        Boolean[] tmp = (Boolean[]) getGraphicObjectProperty(id, propertyName);

        if (tmp == null) {
            return null;
        }

        int[] result = new int[tmp.length];

        for (int i = 0; i < result.length; i++) {
            result[i] = tmp[i] ? 1 : 0;
        }

        return result;
    }

    public static void registerScilabView() {
        GraphicController.getController().register(ScilabView.getScilabView());
    }

    public static void unregisterScilabView() {
        GraphicController.getController().unregister(ScilabView.getScilabView());
    }

    public static String getConsoleIdentifier() {
        return Console.getConsole().getIdentifier();
    }

    public static void buildFigureMenuBar(String figureId) {
        MenuBarBuilder.buildFigureMenuBar(figureId);
    }
}
