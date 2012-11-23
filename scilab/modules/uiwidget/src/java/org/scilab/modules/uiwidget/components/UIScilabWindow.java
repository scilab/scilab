/*
 * Uicontrol2 ( http://forge.scilab.org/index.php/p/uicontrol2/ ) - This file is a part of Uicontrol2
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.uiwidget.components;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.Map;

import javax.swing.ImageIcon;
import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JMenuBar;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIComponentAnnotation;
import org.scilab.modules.uiwidget.UIWidgetException;

import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.utils.Position;

public class UIScilabWindow extends UIComponent {

    private SwingScilabWindow win;
    private ImageIcon icon;
    private Boolean visible;

    public UIScilabWindow(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        win = new SwingScilabWindow();

        return win;
    }

    @UIComponentAnnotation(attributes = {"title", "width", "height", "posX", "posY", "background", "icon", "visible"})
    public Object newInstance(String title, int width, int height, int posX, int posY, Color background, ImageIcon icon, boolean visible) {
        this.visible = visible;
        this.icon = icon;
        win = new SwingScilabWindow();
        win.setName(title);

        if (width != Integer.MAX_VALUE && height != Integer.MAX_VALUE) {
            win.setPreferredSize(new Dimension(width, height));
        }

        if (posX != Integer.MAX_VALUE && posY != Integer.MAX_VALUE) {
            win.setPosition(new Position(posX, posY));
        }

        if (background != null) {
            win.setBackground(background);
        }

        if (icon != null) {
            win.setIconImage(icon.getImage());
        }

        win.addWindowListener(new WindowAdapter() {

            public void windowClosed(WindowEvent e) {
                win.removeWindowListener(this);
                UIScilabWindow.this.remove();
            }
        });

        return win;
    }

    public void add(JComponent c) {
        if (c instanceof JMenuBar) {
            win.setJMenuBar((JMenuBar) c);
        } else if (c instanceof SwingScilabTab) {
            win.addTab((SwingScilabTab) c);
        } else {
            win.add(c);
        }
    }

    public void finish() {
        win.pack();
        if (visible != null) {
            win.setVisible(visible);
            visible = null;
        }
    }

    public void setVisible(boolean b) {
        if (b) {
            win.pack();
        }
        win.setVisible(b);
        visible = null;
    }

    public ImageIcon getIcon() {
        return icon;
    }

    public void setTitle(String title) {
        win.setName(title);
    }

    public String getTitle() {
        return win.getName();
    }

    public void setWidth(int width) {
        win.setPreferredSize(new Dimension(width, win.getHeight()));
        if (win.isVisible()) {
            win.pack();
            win.repaint();
        }
    }

    public void setHeight(int height) {
        win.setPreferredSize(new Dimension(win.getWidth(), height));
        if (win.isVisible()) {
            win.pack();
            win.repaint();
        }
    }

    public void setSize(Dimension d) {
        win.setPreferredSize(d);
        //if (win.isVisible()) {
        win.pack();
        win.repaint();
        //}
    }

    public void setPosX(int posX) {
        win.setLocation(posX, win.getY());
    }

    public void setPosY(int posY) {
        win.setLocation(win.getX(), posY);
    }

    public int getPosX() {
        return win.getLocationOnScreen().x;
    }

    public int getPosY() {
        return win.getLocationOnScreen().y;
    }
}
