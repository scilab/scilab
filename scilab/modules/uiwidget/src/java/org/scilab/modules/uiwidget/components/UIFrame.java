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
import java.awt.Image;
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

public class UIFrame extends UIComponent {

    private JFrame win;
    private ImageIcon icon;
    private boolean visible;

    public UIFrame(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        win = new JFrame();

        return win;
    }

    @UIComponentAnnotation(attributes = {"title", "width", "height", "posX", "posY", "background", "icon", "visible"})
    public Object newInstance(String title, int width, int height, int posX, int posY, Color background, ImageIcon icon, boolean visible) {
        this.visible = visible;
        this.icon = icon;
        win = new JFrame();

        win.setName(title);
        win.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);

        if (width != Integer.MAX_VALUE && height != Integer.MAX_VALUE) {
            win.setPreferredSize(new Dimension(width, height));
        }

        if (posX != Integer.MAX_VALUE && posY != Integer.MAX_VALUE) {
            win.setLocation(posX, posY);
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
                UIFrame.this.remove();
            }
        });

        return win;
    }

    public void add(JComponent c) {
        if (c instanceof JMenuBar) {
            win.setJMenuBar((JMenuBar) c);
        } else {
            win.add(c);
        }
    }

    public void finish() {
        win.validate();
        win.pack();
        win.setVisible(visible);
    }

    public ImageIcon getIcon() {
        return icon;
    }

    public String getTitle() {
        return win.getName();
    }

    public void setTitle(String title) {
        win.setName(title);
    }

    public void setWidth(int width) {
        win.setSize(new Dimension(width, win.getHeight()));
        win.validate();
    }

    public void setHeight(int height) {
        win.setSize(new Dimension(win.getWidth(), height));
        win.validate();
    }

    public void setPosX(int posX) {
        win.setLocation(posX, win.getY());
    }

    public void setPosY(int posY) {
        win.setLocation(win.getX(), posY);
    }
}
