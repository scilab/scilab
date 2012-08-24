/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Pierre Lando
 * Copyright (C) 2010 - Scilab Enterprises - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */
package org.scilab.modules.gui.graphicWindow;

import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.LayoutManager;
import java.io.Serializable;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;

/**
 * @author Pierre Lando
 */
@SuppressWarnings(value = { "serial" })
public class PanelLayout implements LayoutManager, Serializable {

    public static final String GL_CANVAS = "GL_CANVAS";
    public static final String UI_CONTROL = "UI_CONTROL";
    private Component canvas;

    @Override
    public void addLayoutComponent(String name, Component comp) {
        if (GL_CANVAS.equals(name)) {
            canvas = comp;
        }
    }

    @Override
    public void removeLayoutComponent(Component comp) {
    }

    @Override
    public Dimension preferredLayoutSize(Container parent) {
        return new Dimension(0, 0);
    }

    @Override
    public Dimension minimumLayoutSize(Container parent) {
        return new Dimension(0, 0);
    }

    @Override
    public void layoutContainer(Container parent) {
        for (Component child : parent.getComponents()) {
            if (child.equals(canvas)) {
                canvas.setBounds(0, 0, parent.getWidth(), parent.getHeight());
                parent.setComponentZOrder(child, parent.getComponentCount() - 1);
            }

            String figureIdentifier = ((SwingScilabCanvas) parent).getFigure().getIdentifier();
            String resizeFcn = (String) GraphicController.getController().getProperty(figureIdentifier, GraphicObjectProperties.__GO_RESIZEFCN__);
            if (resizeFcn != null && !resizeFcn.equals("")) {
                String resizeCommand = "if exists(\"gcbo\") then %oldgcbo = gcbo; end;"
                                       + "gcbo = getcallbackobject(\"" + figureIdentifier + "\");"
                                       + resizeFcn
                                       + ";if exists(\"%oldgcbo\") then gcbo = %oldgcbo; else clear gcbo; end;";
                InterpreterManagement.requestScilabExec(resizeCommand);
            }

            /* Here you can perform the layout of UI object. */
            if (child instanceof SwingViewObject) {
                String id = ((SwingViewObject) child).getId();
                ((SwingViewObject) child).update(GraphicObjectProperties.__GO_POSITION__,
                                                 GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_POSITION__));
            }
        }
    }
}
