/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2016 - Scilab Enterprises - Antoine ELIAS
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

import org.scilab.modules.graphic_objects.ScilabNativeWebView;

public class ScilabWebView extends ScilabNativeWebView implements GraphicView {

    private static ScilabWebView me;

    public ScilabWebView() {
    }

    public static ScilabWebView getScilabWebView() {
        if (me == null) {
            me = new ScilabWebView();
        }
        return me;
    }

    public void createObject(Integer id) {
        ScilabNativeWebView__createObject(id);
    }

    public void deleteObject(Integer id) {
        ScilabNativeWebView__deleteObject(id);
    }

    public void updateObject(Integer id, int property) {
        ScilabNativeWebView__updateObject(id, property);
    }
}
