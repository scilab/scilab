package org.scilab.modules.graphic_objects.xmlloader;

import java.util.Map;
import java.util.StringTokenizer;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;

public class XmlTools {
    public static Boolean setPropAsDouble(Integer uid, Integer prop, String propString) {
        if (propString == null || propString.equals("")) {
            return false;
        }

        GraphicController.getController().setProperty(uid, prop, Double.parseDouble(propString));
        return true;
    }

    public static Boolean setPropAsDoubleVector(Integer uid, Integer prop, Map<String, String> map, String[] propString) {
        if (propString == null || propString.length == 0) {
            return false;
        }

        Double[] values = new Double[propString.length];
        for (int i = 0; i < propString.length; i++) {
            String value = map.get(propString[i]);
            if (value == null || value.equals("")) {
                values[i] = 0.0;
            } else {
                values[i] = Double.parseDouble(value);
            }
        }

        GraphicController.getController().setProperty(uid, prop, values);
        return true;
    }

    public static Boolean setPropAsDoubleVector(Integer uid, Integer prop, String propString, Integer size) {
        if (propString == null || propString.equals("")) {
            return false;
        }

        Double[] values = new Double[size];

        StringTokenizer tokens = new StringTokenizer(propString, ",");
        if (tokens.countTokens() > size) {
            return false;
        }

        int i = 0;
        while (tokens.hasMoreTokens()) {
            values[i++] = Double.parseDouble(tokens.nextToken());
        }

        GraphicController.getController().setProperty(uid, prop, values);
        return true;
    }

    public static Boolean setPropAsBoolean(Integer uid, Integer prop, String propString) {
        if (propString == null || propString.equals("")) {
            return false;
        }

        GraphicController.getController().setProperty(uid, prop, Boolean.parseBoolean(propString));
        return true;
    }

    public static Boolean setPropAsBooleanVector(Integer uid, Integer prop, String propString, Integer size) {
        if (propString == null || propString.equals("")) {
            return false;
        }

        Boolean[] values = new Boolean[size];

        StringTokenizer tokens = new StringTokenizer(propString, ",");
        if (tokens.countTokens() > size) {
            return false;
        }

        int i = 0;
        while (tokens.hasMoreTokens()) {
            values[i++] = Boolean.parseBoolean(tokens.nextToken());
        }

        GraphicController.getController().setProperty(uid, prop, values);
        return true;
    }

    public static Boolean setPropAsInteger(Integer uid, Integer prop, String propString) {
        if (propString == null || propString.equals("")) {
            return false;
        }

        GraphicController.getController().setProperty(uid, prop, Integer.parseInt(propString));
        return true;
    }

    public static Boolean setPropAsIntegerVector(Integer uid, Integer prop, String propString, Integer size) {
        if (propString == null || propString.equals("")) {
            return false;
        }

        Integer[] values = new Integer[size];

        StringTokenizer tokens = new StringTokenizer(propString, ",");
        if (tokens.countTokens() > size) {
            return false;
        }

        int i = 0;
        while (tokens.hasMoreTokens()) {
            values[i++] = Integer.parseInt(tokens.nextToken());
        }

        GraphicController.getController().setProperty(uid, prop, values);
        return true;
    }

    public static Boolean setPropAsString(Integer uid, Integer prop, String propString) {
        if (propString == null || propString.equals("")) {
            return false;
        }

        GraphicController.getController().setProperty(uid, prop, propString);
        return true;
    }

    public static Boolean setPropAsStringVector(Integer uid, Integer prop, String propString, Integer size) {
        if (propString == null || propString.equals("")) {
            return false;
        }

        String[] values = new String[size];

        StringTokenizer tokens = new StringTokenizer(propString, ",");
        if (tokens.countTokens() > size) {
            return false;
        }

        int i = 0;
        while (tokens.hasMoreTokens()) {
            values[i++] = tokens.nextToken();
        }

        GraphicController.getController().setProperty(uid, prop, values);
        return true;
    }

    public static String getFromMap(Map<String, String> map, String field, String defaultValue) {
        if (map != null) {
            String ret = map.get(field);
            if (ret != null) {
                map.remove(field);
                return ret;
            }
        }
        return defaultValue;
    }

    public static String getFromMap(Map<String, String> map, String field) {
        return getFromMap(map, field, "");
    }
}
