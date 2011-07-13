package org.scilab.modules.gui.graphicWindow;

import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.LayoutManager;
import java.io.Serializable;

/**
 * @author Pierre Lando
 */
public class PanelLayout implements LayoutManager, Serializable{

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
        return null;
    }

    @Override
    public Dimension minimumLayoutSize(Container parent) {
        return null;
    }

    @Override
    public void layoutContainer(Container parent) {
        for (Component child : parent.getComponents()) {
            if (child.equals(canvas)) {
                child.setBounds(0, 0, parent.getWidth(), parent.getHeight());
                parent.setComponentZOrder(child, parent.getComponentCount() - 1);
            }
        }
    }
}
