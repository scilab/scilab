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
 * This is a static class to acces all controller capabilities
 * from C/C++ code through JNI
 * See SCI/modules/graphic_objects/src/jni/graphic_objects.giws.xml for other details.
 **/
public final class CallGraphicController {

    public static String askGraphicObject(String typeName) {
        return GraphicController.getController().askObject(GraphicObject.getTypeFromName(typeName));
    }

    private static void setGraphicObjectProperty(String id, String propertyName, Object value) {
        GraphicController.getController().setPropertyFast(id, propertyName, value);
    }

    private static Object getGraphicObjectProperty(String id, String propertyName) {
        return GraphicController.getController().getPropertyFast(id, propertyName);
    }

    public static void setGraphicObjectProperty(String id, String propertyName, String value) {
        setGraphicObjectProperty(id, propertyName, (Object) value);
    }

    public static String getGraphicObjectPropertyAsString(String id, String propertyName) {
        return (String) getGraphicObjectProperty(id, propertyName);
    }
}
