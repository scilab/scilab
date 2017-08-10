/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
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

    public static void setGraphicObjectRelationship(int parentId, int childId) {
        GraphicController.getController().setGraphicObjectRelationship(parentId, childId);
    }

    public static void removeRelationShipAndDelete(int parentId) {
        GraphicController.getController().removeRelationShipAndDelete(parentId);
    }

    public static int cloneGraphicObject(int id) {
        return GraphicController.getController().cloneObject(id);
    }

    public static int askGraphicObject(int typeName) {
        return GraphicController.getController().askObject(GraphicObject.getTypeFromName(typeName));
    }

    public static void deleteGraphicObject(int id) {
        GraphicController.getController().deleteObject(id);
    }

    private static boolean setGraphicObjectProperty(int id, int propertyName, Object value) {
        return GraphicController.getController().setProperty(id, propertyName, value);
    }

    private static Object getGraphicObjectProperty(int id, int propertyName) {
        return GraphicController.getController().getProperty(id, propertyName);
    }

    public static boolean setGraphicObjectProperty(int id, int propertyName, String value) {
        return setGraphicObjectProperty(id, propertyName, (Object) value);
    }

    public static boolean setGraphicObjectProperty(int id, int propertyName, String[] value) {
        return setGraphicObjectProperty(id, propertyName, (Object) value);
    }

    public static boolean setGraphicObjectProperty(int id, int propertyName, double value) {
        return setGraphicObjectProperty(id, propertyName, (Object) new Double(value));
    }

    public static boolean setGraphicObjectProperty(int id, int propertyName, double[] value) {
        Double[] array = new Double[value.length];

        for (int i = 0; i < value.length; i++) {
            array[i] = value[i];
        }

        return setGraphicObjectProperty(id, propertyName, (Object) array);
    }


    public static boolean setGraphicObjectProperty(int id, int propertyName, int value) {
        return setGraphicObjectProperty(id, propertyName, (Object) new Integer(value));
    }

    public static boolean setGraphicObjectProperty(int id, int propertyName, int[] value) {
        Integer[] array = new Integer[value.length];

        for (int i = 0; i < value.length; i++) {
            array[i] = value[i];
        }

        return setGraphicObjectProperty(id, propertyName, (Object) array);
    }

    public static boolean setGraphicObjectProperty(int id, int propertyName, boolean value) {
        return setGraphicObjectProperty(id, propertyName, (Object) new Boolean(value));
    }

    public static boolean setGraphicObjectProperty(int id, int propertyName, boolean[] value) {
        Boolean[] array = new Boolean[value.length];

        for (int i = 0; i < value.length; i++) {
            array[i] = value[i];
        }

        return setGraphicObjectProperty(id, propertyName, (Object) array);
    }

    public static String getGraphicObjectPropertyAsString(int id, int propertyName) {
        return (String) getGraphicObjectProperty(id, propertyName);
    }

    public static String[] getGraphicObjectPropertyAsStringVector(int id, int propertyName) {
        return (String[]) getGraphicObjectProperty(id, propertyName);
    }

    public static double[] getGraphicObjectPropertyAsDoubleVector(int id, int propertyName) {
        Object ret = getGraphicObjectProperty(id, propertyName);
        double[] result = null;

        if (ret == null) {
            return null;
        }

        if (ret instanceof Double[]) {
            Double[] tmp = (Double[]) ret;
            result = new double[tmp.length];

            for (int i = 0; i < result.length; i++) {
                result[i] = tmp[i];
            }
        } else if (ret instanceof Double) {
            result = new double[] { (Double) ret};
        }

        return result;
    }

    public static int[] getGraphicObjectPropertyAsIntegerVector(int id, int propertyName) {
        Object ret = getGraphicObjectProperty(id, propertyName);
        int[] result = null;

        if (ret == null) {
            return null;
        }

        if (ret instanceof Integer[]) {
            Integer[] tmp = (Integer[]) ret;
            result = new int[tmp.length];

            for (int i = 0; i < result.length; i++) {
                result[i] = tmp[i] == null ? 0 : tmp[i];
            }
        } else if (ret instanceof Integer) {
            result = new int[] { (Integer) ret};
        }

        return result;
    }

    public static int[] getGraphicObjectPropertyAsBooleanVector(int id, int propertyName) {
        Object ret = getGraphicObjectProperty(id, propertyName);
        int[] result = null;

        if (ret == null) {
            return null;
        }

        if (ret instanceof Boolean[]) {
            Boolean[] tmp = (Boolean[]) getGraphicObjectProperty(id, propertyName);
            result = new int[tmp.length];
            for (int i = 0; i < result.length; i++) {
                result[i] = tmp[i] ? 1 : 0;
            }
        } else if (ret instanceof Boolean) {
            result = new int[] { ((Boolean) ret) ? 1 : 0};
        }

        return result;
    }

    public static void registerScilabView() {
        GraphicController.getController().register(ScilabView.getScilabView());
    }

    public static void unregisterScilabView() {
        GraphicController.getController().unregister(ScilabView.getScilabView());
    }

    public static int getConsoleIdentifier() {
        return Console.getConsole().getIdentifier();
    }

    public static void buildFigureMenuBar(int figureId) {
        MenuBarBuilder.buildFigureMenuBar(figureId);
    }
}
