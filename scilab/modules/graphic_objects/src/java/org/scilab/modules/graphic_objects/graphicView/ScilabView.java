/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.graphic_objects.graphicView;

import org.scilab.modules.graphic_objects.ScilabNativeView;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

public class ScilabView extends ScilabNativeView implements GraphicView {

    private static ScilabView me;

    public ScilabView() {
    }

    public static ScilabView getScilabView() {
        if (me == null) {
            me = new ScilabView();
        }
        return me;
    }

    public void createObject(String id) {
        ScilabNativeView__createObject(id);
    }

    public void deleteObject(String id) {
        ScilabNativeView__deleteObject(id);
    }

    public void updateObject(String id, int property) {
        if (property == GraphicObjectProperties.__GO_ID__) {
            ScilabNativeView__updateObject(id, property);
        }
    }
}
