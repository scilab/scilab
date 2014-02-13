/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
package org.scilab.modules.graphic_objects.graphicView;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_ID__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CHILDREN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TAG__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_PARENT__;

import org.scilab.modules.graphic_objects.ScilabNativeView;

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

    public void createObject(Integer id) {
        ScilabNativeView__createObject(id);
    }

    public void deleteObject(Integer id) {
        ScilabNativeView__deleteObject(id);
    }

    public void updateObject(Integer id, int property) {
        switch (property) {
            case __GO_ID__ :
            case __GO_CHILDREN__ :
            case __GO_PARENT__ :
            case __GO_TAG__ : {
                ScilabNativeView__updateObject(id, property);
                break;
            }
            default : {
                break;
            }
        }
    }
}
