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

package org.scilab.modules.gui.uiwidget.components;

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

import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIComponentAnnotation;
import org.scilab.modules.gui.uiwidget.UIWidgetException;
import org.scilab.modules.gui.uiwidget.UIWidgetTools;
import org.scilab.modules.gui.uiwidget.callback.UICallback;

/**
 * JFrame wrapper
 */
public class UIFrame extends UIComponent {

    // TODO: regarder un peu ce qui ce fait ds UIScilabWindow qui est plus finalisee que celle ci

    private JFrame win;
    private ImageIcon icon;
    private boolean visible;
    protected UICallback oncloseAction;
    protected boolean oncloseEnable = true;

    /**
     * {@inheritDoc}
     */
    public UIFrame(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    /**
     * {@inheritDoc}
     */
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

            public void windowClosing(WindowEvent e) {
                if (oncloseEnable && oncloseAction != null) {
                    UIWidgetTools.execAction(UIFrame.this.oncloseAction);
                }
            }

            public void windowClosed(WindowEvent e) {
                win.removeWindowListener(this);
                UIFrame.this.remove();
            }
        });

        return win;
    }

    /**
     * {@inheritDoc}
     */
    public void add(JComponent c) {
        if (c instanceof JMenuBar) {
            win.setJMenuBar((JMenuBar) c);
        } else {
            win.setContentPane(c);
        }
    }

    /**
     * {@inheritDoc}
     */
    public void finish() {
        win.invalidate();
        win.pack();
        win.setVisible(visible);
    }

    /**
     * {@inheritDoc}
     */
    public void setVisible(boolean b) {
        if (!win.isVisible() && b) {
            win.invalidate();
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
     * Set the frame icon
     * @param icon the frame icon
     */
    public void setIcon(ImageIcon icon) {
        if (icon != null) {
            win.setIconImage(icon.getImage());
        } else {
            win.setIconImage(null);
        }

        this.icon = icon;
    }

    /**
     * Get the frame icon
     * @return the frame icon
     */
    public ImageIcon getIcon() {
        return icon;
    }

    /**
     * Get the frame title
     * @return the frame title
     */
    public String getTitle() {
        return win.getName();
    }

    /**
     * Set the frame title
     * @param title the frame title
     */
    public void setTitle(String title) {
        win.setName(title);
    }

    /**
     * Set the frame width
     * @param width the frame width
     */
    public void setWidth(int width) {
        Dimension prev = win.getSize();
        if (prev.width != width) {
            win.setPreferredSize(new Dimension(width, win.getHeight()));
            if (win.isVisible()) {
                win.pack();
                win.repaint();
            }
        }
    }

    /**
     * Set the frame height
     * @param height the frame height
     */
    public void setHeight(int height) {
        Dimension prev = win.getSize();
        if (prev.height != height) {
            win.setPreferredSize(new Dimension(win.getWidth(), height));
            if (win.isVisible()) {
                win.pack();
                win.repaint();
            }
        }
    }

    /**
     * Set the frame size
     * @param d the frame size
     */
    public void setSize(Dimension d) {
        Dimension prev = win.getSize();
        if (!prev.equals(d)) {
            win.setPreferredSize(d);
            if (win.isVisible()) {
                win.pack();
                win.repaint();
            }
        }
    }

    /**
     * Set the frame posX
     * @param posX the frame posX
     */
    public void setPosX(int posX) {
        win.setLocation(posX, win.getY());
    }

    /**
     * Set the frame posY
     * @param posY the frame posY
     */
    public void setPosY(int posY) {
        win.setLocation(win.getX(), posY);
    }

    /**
     * Get the frame posX
     * @return the frame posX
     */
    public int getPosX() {
        return win.getX();
    }

    /**
     * Get the frame posY
     * @return the frame posY
     */
    public int getPosY() {
        return win.getY();
    }

    /**
     * Check if onclose event is enabled
     * @return true if the event is enabled
     */
    public boolean getOncloseEnable() {
        return oncloseEnable;
    }

    /**
     * Enable onclose event
     * @param b if true enable onclose event
     */
    public void setOncloseEnable(boolean b) {
        if (oncloseEnable != b) {
            oncloseEnable = b;
            if (oncloseEnable && oncloseAction != null) {
                win.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
            } else {
                win.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
            }
        }
    }

    /**
     * Get the callback
     * @return the callback
     */
    public UICallback getOnclose() {
        return oncloseAction;
    }

    /**
     * Set the onclose callback
     * @param oncloseAction the callback string
     */
    public void setOnclose(final String oncloseAction) {
        this.oncloseAction = UICallback.newInstance(this, oncloseAction);
        if (oncloseEnable && this.oncloseAction != null) {
            win.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
        } else {
            win.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        }
    }
}
