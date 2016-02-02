/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Pierre Lando
 * Copyright (C) 2010 - Scilab Enterprises - Bruno JOFRET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */
package org.scilab.modules.gui.graphicWindow;

import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.LayoutManager;
import java.io.Serializable;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.SwingViewObject;

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
            } else if (child instanceof SwingViewObject) {
                /* Here you can perform the layout of UI object. */
                Integer id = ((SwingViewObject) child).getId();
                ((SwingViewObject) child).update(GraphicObjectProperties.__GO_POSITION__, GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_POSITION__));
            }
        }
    }
}
