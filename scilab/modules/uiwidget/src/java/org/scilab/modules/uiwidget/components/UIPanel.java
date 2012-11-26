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

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Image;
import java.awt.LayoutManager;
import java.util.HashMap;
import java.util.Map;

import javax.swing.ImageIcon;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;

import org.scilab.modules.uiwidget.StringConverters;
import org.scilab.modules.uiwidget.UIAccessTools;
import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIComponentAnnotation;
import org.scilab.modules.uiwidget.UIWidgetException;
import org.scilab.modules.uiwidget.UILayoutFactory;

public class UIPanel extends UIComponent {

    JPanel panel;
    GridBagConstraints gbc;
    ImageIcon backgroundImage;
    ImageFill imageStyle;

    public enum ImageFill {
        CENTER, FIT, REPEAT;
    }

    public UIPanel(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        panel = new JPanel();

        return panel;
    }

    @UIComponentAnnotation(attributes = {"width", "height", "background", "scrollable", "image", "image-style"})
    public Object newInstance(int width, int height, Color background, boolean scrollable, ImageIcon backgroundImage, ImageFill imageStyle) {
        this.backgroundImage = backgroundImage;
        this.imageStyle = imageStyle;
        panel = new JPanel(new NoLayout()) {
            public void paintComponent(Graphics g) {
                super.paintComponent(g);
                ImageIcon im = UIPanel.this.backgroundImage;
                if (im != null) {
                    switch (UIPanel.this.imageStyle) {
                        case CENTER:
                            g.drawImage(im.getImage(), (getWidth() - im.getIconWidth()) / 2, (getHeight() - im.getIconHeight()) / 2, this);
                            break;
                        case FIT:
                            g.drawImage(im.getImage(), 0, 0, getWidth(), getHeight(), this);
                            break;
                        case REPEAT:
                            Image img = im.getImage();
                            for (int x = 0; x <= getWidth(); x += im.getIconWidth()) {
                                for (int y = 0; y <= getHeight(); y += im.getIconHeight()) {
                                    g.drawImage(img, x, y, this);
                                }
                            }
                            break;
                    }
                }
            }

        };

        if (width != Integer.MAX_VALUE && height != Integer.MAX_VALUE) {
            panel.setSize(new Dimension(width, height));
        }

        if (background != null) {
            panel.setBackground(background);
        }

        return panel;
    }

    public ImageIcon getImage() {
        return backgroundImage;
    }

    public void setImage(ImageIcon backgroundImage) {
        this.backgroundImage = backgroundImage;
        panel.repaint();
    }

    public void setImageStyle(ImageFill imageStyle) {
        this.imageStyle = imageStyle;
        panel.repaint();
    }

    public ImageFill getImageStyle() {
        return imageStyle;
    }

    public void add(final UIComponent c) throws UIWidgetException {
        Object o = c.getComponent();
        if (o instanceof Component && !(o instanceof JPopupMenu)) {
            final Component comp = (Component) o;
            final LayoutManager layout = panel.getLayout();
            UIAccessTools.execOnEDT(new Runnable() {
                public void run() {
                    try {
                        if (layout instanceof BorderLayout) {
                            String constraint = c.getLayoutConstraint() == null ? "c" : c.getLayoutConstraint().get("position");
                            panel.add(comp, UILayoutFactory.BorderConstants.get(constraint));
                        } else if (layout instanceof GridBagLayout) {
                            if (gbc == null) {
                                gbc = new GridBagConstraints();
                            }
                            if (c.getLayoutConstraint() == null) {
                                System.err.println("Invalid layout constraint: must not be empty");
                                return;
                            }
                            add(panel, gbc, comp, c.getLayoutConstraint());
                        } else if (layout instanceof NoLayout) {
                            panel.add(comp, c.getPosition());
                        } else {
                            panel.add(comp);
                        }
                    } catch (UIWidgetException e) {
                        System.err.println(e);
                    }
                }
            });
        } else {
            super.add(c);
        }
    }

    public void finish() {
        panel.revalidate();
    }

    public void add(Component c) {
        panel.add(c);
    }

    private void add(JPanel base, GridBagConstraints gbc, Component c, Map<String, String> attrs) {
        gbc.gridx = StringConverters.getObjectFromValue(int.class, attrs.get("gridx"), 1) - 1;
        gbc.gridy = StringConverters.getObjectFromValue(int.class, attrs.get("gridy"), 1) - 1;
        gbc.gridwidth = StringConverters.getObjectFromValue(int.class, attrs.get("gridwidth"), 1);
        gbc.gridheight = StringConverters.getObjectFromValue(int.class, attrs.get("gridheight"), 1);
        gbc.weightx = StringConverters.getObjectFromValue(double.class, attrs.get("weightx"), 1d);
        gbc.weighty = StringConverters.getObjectFromValue(double.class, attrs.get("weighty"), 1d);
        gbc.ipadx = StringConverters.getObjectFromValue(int.class, attrs.get("ipadx"), 0);
        gbc.ipady = StringConverters.getObjectFromValue(int.class, attrs.get("ipady"), 0);

        String str = attrs.get("anchor");
        if (str != null) {
            Integer an = UILayoutFactory.GridBagConstants.get(str.toLowerCase());
            if (an != null) {
                gbc.anchor = an.intValue();
            }
        }

        str = attrs.get("fill");
        if (str != null) {
            Integer fi = UILayoutFactory.GridBagConstants.get(str.toLowerCase());
            if (fi != null) {
                gbc.fill = fi.intValue();
            }
        }

        base.add(c, gbc);
    }
}
