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

package org.scilab.modules.graphic_objects.graphicView;

import java.lang.reflect.Field;
import java.util.HashMap;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TYPE__;

public class LogView implements GraphicView {

    private static LogView me = null;

    private LogView() {}

    public static void createLogView() {
        if (me == null) {
            me = new LogView();
            GraphicController.getController().register(me);
        }
    }

    public static void deleteLogView() {
        if (me != null) {
            GraphicController.getController().unregister(me);
            me = null;
        }
    }

    public static HashMap<Integer, String> pierreDeRosette = new HashMap<Integer, String>();

    static {
        Field[] fields = GraphicObjectProperties.class.getFields();
        for (int i = 0 ; i < fields.length ; ++i) {
            try {
                pierreDeRosette.put(fields[i].getInt(null), fields[i].getName());
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    public void createObject(Integer id) {
        int objectType = (Integer) GraphicController.getController().getProperty(id, __GO_TYPE__);
        System.out.println("+++ " + id + " Object Created type is " + pierreDeRosette.get(objectType) + ".");
    }

    public void deleteObject(Integer id) {
        System.out.println("--- " + id + " Object Deleted.");

    }

    public void updateObject(Integer id, int property) {
        System.out.println("=== " + id + " Object Property " + pierreDeRosette.get(property) + " Updated.");
    }

}
