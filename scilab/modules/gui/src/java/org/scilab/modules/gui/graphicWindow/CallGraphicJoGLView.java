/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.gui.graphicWindow;

import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.gui.SwingView;
import org.scilab.modules.jvm.LoadClassPath;

/**
 * @author Pierre Lando
 */
public final class CallGraphicJoGLView {
    private static boolean notLoaded = true;
    private static void load() {
        if(notLoaded) {
            LoadClassPath.loadOnUse("graphics");
            notLoaded = false;
        }
    }

    public static void  createJoGLView(String id) {
        load();
        GraphicObject controller = GraphicController.getController().getObjectFromId(id);
        if (controller != null && controller instanceof Figure) {
            FigureBridge.createFigure(id);
        }
    }
    
    public static void createSwingView() {
        load();
        SwingView.registerSwingView();
    }
}
