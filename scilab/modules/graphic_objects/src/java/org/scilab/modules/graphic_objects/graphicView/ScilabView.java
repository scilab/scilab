/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
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
