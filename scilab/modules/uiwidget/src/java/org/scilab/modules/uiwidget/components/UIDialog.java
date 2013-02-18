/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
import java.awt.Component;
import java.awt.Dialog;
import java.awt.Dimension;
import java.awt.Frame;
import java.awt.Image;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.swing.ImageIcon;
import javax.swing.JComponent;
import javax.swing.JDialog;
import javax.swing.JMenuBar;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIComponentAnnotation;
import org.scilab.modules.uiwidget.UIWidgetException;

/**
 * JDialog wrapper
 */
public class UIDialog extends UIComponent {

    private JDialog win;
    private ImageIcon icon;
    private Boolean visible;
    private UIComponent parentWindow;

    /**
     * {@inheritDoc}
     */
    public UIDialog(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    /**
     * {@inheritDoc}
     */
    public Object newInstance() {
        win = new JDialog();

        return win;
    }

    @UIComponentAnnotation(attributes = {"title", "width", "height", "posX", "posY", "background", "icon", "visible", "parent", "modal"})
    public Object newInstance(String title, int width, int height, int posX, int posY, Color background, ImageIcon icon, boolean visible, UIComponent parent, boolean modal) {
        this.visible = visible;
        this.icon = icon;
        this.parentWindow = parent;

        if (parent == null) {
            win = new JDialog();
        } else {
            Object c = parent.getComponent();
            if (c instanceof Frame) {
                win = new JDialog((Frame) c, modal);
            } else if (c instanceof Dialog) {
                win = new JDialog((Dialog) c, modal);
            } else {
                win = new JDialog();
            }

            if (c instanceof Component) {
                win.setLocationRelativeTo((Component) c);
            }
        }

        win.setName(title);
        win.setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);

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
                UIDialog.this.remove();
            }
        });

        return win;
    }

    /**
     * Add a component
     * @param c the component to add
     */
    public void add(JComponent c) {
        if (c instanceof JMenuBar) {
            win.setJMenuBar((JMenuBar) c);
        } else {
            win.add(c);
        }
    }

    /**
     * {@inheritDoc}
     */
    public void finish() {
        win.pack();
        if (visible != null) {
            setVisible(visible);
            visible = null;
        }
    }

    /**
     * Set the dialog visibility
     * @param b if true the dialog is made visible
     */
    public void setVisible(boolean b) {
        if (parentWindow != null && parentWindow.getComponent() instanceof Component) {
            ScreenInfo.center(win, (Component) parentWindow.getComponent());
        } else {
            ScreenInfo.center(win, null);
        }

        if (!win.isVisible() && b) {
            win.pack();
        }

        win.setVisible(b);
    }

    /**
     * {@inheritDoc}
     */
    public void remove() {
        win.dispose();
        super.remove();
    }

    /**
     * Set the dialog icon
     * @param icon the dialog icon
     */
    public void setIcon(ImageIcon icon) {
        if (icon != null) {
            win.setIconImage(icon.getImage());
        } else {
            win.setIconImage(null);
        }
    }

    /**
     * Get the dialog icon
     * @return the dialog icon
     */
    public ImageIcon getIcon() {
        return icon;
    }

    /**
     * Get the dialog title
     * @return the dialog title
     */
    public String getTitle() {
        return win.getName();
    }

    /**
     * Set the dialog title
     * @param title the dialog title
     */
    public void setTitle(String title) {
        win.setName(title);
    }

    /**
     * Set the dialog width
     * @param width the dialog width
     */
    public void setWidth(int width) {
        win.setSize(new Dimension(width, win.getHeight()));
        win.validate();
    }

    /**
     * Set the dialog height
     * @param height the dialog height
     */
    public void setHeight(int height) {
        win.setSize(new Dimension(win.getWidth(), height));
        win.validate();
    }

    /**
     * Set the dialog posX
     * @param posX the dialog posX
     */
    public void setPosX(int posX) {
        win.setLocation(posX, win.getY());
    }

    /**
     * Set the dialog posY
     * @param posY the dialog posY
     */
    public void setPosY(int posY) {
        win.setLocation(win.getX(), posY);
    }

    /**
     * Get the dialog posX
     * @return the dialog posX
     */
    public int getPosX() {
        return win.getX();
    }

    /**
     * Get the dialog posY
     * @return the dialog posY
     */
    public int getPosY() {
        return win.getY();
    }
}

