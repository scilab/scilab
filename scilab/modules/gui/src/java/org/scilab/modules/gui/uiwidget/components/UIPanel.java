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

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Image;
import java.awt.Insets;
import java.awt.LayoutManager;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import javax.swing.ImageIcon;
import javax.swing.JComponent;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.JTabbedPane;
import javax.swing.Timer;
import javax.swing.UIManager;
import javax.swing.border.Border;
import javax.swing.border.TitledBorder;

import org.scilab.modules.gui.uiwidget.StringConverters;
import org.scilab.modules.gui.uiwidget.UIAccessTools;
import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIComponentAnnotation;
import org.scilab.modules.gui.uiwidget.UIWidgetException;
import org.scilab.modules.gui.uiwidget.UILayoutFactory;

/**
 * JPanel wrapper
 */
public class UIPanel extends UIComponent {

    protected JPanel panel;
    protected GridBagConstraints gbc = new GridBagConstraints();
    protected ImageIcon backgroundImage;
    protected ImageFill imageStyle;
    protected List<Component> enabledComponents;
    protected String enable;

    public enum ImageFill {
        CENTER, FIT, REPEAT;
    }

    /**
     * {@inheritDoc}
     */
    public UIPanel(UIComponent parent) throws UIWidgetException {
        super(parent);
        enabledComponents = new LinkedList<Component>();
    }

    /**
     * {@inheritDoc}
     */
    public Object newInstance() {
        panel = new JPanel();

        return panel;
    }

    @UIComponentAnnotation(attributes = {"width", "height", "background", "image", "image-style", "enable"})
    public Object newInstance(int width, int height, Color background, ImageIcon backgroundImage, ImageFill imageStyle, String enable) {
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
                            final Image img = im.getImage();
                            final int w = im.getIconWidth();
                            final int h = im.getIconHeight();
                            for (int x = 0; x <= getWidth(); x += w) {
                                for (int y = 0; y <= getHeight(); y += h) {
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

        this.enable = enable;

        return panel;
    }

    /**
     * Enable the component or not
     * @param enable a boolean
     */
    public void setEnable(boolean enable) {
        setEnable(panel, enable, enabledComponents);
    }

    /**
     * Enable a JPanel and all its descendants. The list enabledComponents will contains all the enabled children components.
     * @param panel the JPanel
     * @param enable the enable status
     * @param enabledComponents when enable is false, the list will contains enabled components
     */
    public static void setEnable(JPanel panel, boolean enable, List<Component> enabledComponents) {
        if (panel.isEnabled() != enable) {
            if (enable) {
                for (Component c : enabledComponents) {
                    c.setEnabled(true);
                    changeBorderColor(c, (Color) UIManager.get("Label.enabledForeground"));
                }
                enabledComponents.clear();
            } else {
                changeBorderColor(panel, panel.getBackground().darker());
                enabledComponents.add(panel);
                disableDescendants(panel, enabledComponents);
            }
            if (panel.getParent() instanceof JTabbedPane) {
                JTabbedPane tab = (JTabbedPane) panel.getParent();
                int index = tab.indexOfComponent(panel);
                if (index != -1) {
                    tab.setEnabledAt(index, enable);
                    if (!enable) {
                        int nb = tab.getTabCount();
                        if (nb >= 2) {
                            for (int i = (index + 1) % nb; i != index; i = (i + 1) % nb) {
                                if (tab.isEnabledAt(i)) {
                                    tab.setSelectedIndex(i);
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            panel.setEnabled(enable);
        }
    }

    /**
     * Disable the descendants of the given container
     * @param container the container
     */
    private static void disableDescendants(Container container, List<Component> enabledComponents) {
        for (Component c : container.getComponents()) {
            if (c.isEnabled()) {
                enabledComponents.add(c);
                c.setEnabled(false);
                changeBorderColor(c, c.getBackground().darker());
            }
            if (c instanceof Container) {
                disableDescendants((Container) c, enabledComponents);
            }
        }
    }

    /**
     * Change the border of a component
     * @param comp the component
     * @param c the new color
     */
    private static void changeBorderColor(Component comp, Color c) {
        if (comp instanceof JComponent) {
            Border border = ((JComponent) comp).getBorder();
            if (border instanceof TitledBorder) {
                TitledBorder titled = (TitledBorder) border;
                titled.setTitleColor(c);
            }
        }
    }

    /**
     * Get the background image of this panel
     * @return the image
     */
    public ImageIcon getImage() {
        return backgroundImage;
    }

    /**
     * Set the background image of this panel
     * @param backgroundImage the image
     */
    public void setImage(ImageIcon backgroundImage) {
        this.backgroundImage = backgroundImage;
        panel.repaint();
    }

    /**
     * Set the background image style of this panel
     * @param imageStyle one of the constants CENTER, FIT or REPEAT
     */
    public void setImageStyle(ImageFill imageStyle) {
        this.imageStyle = imageStyle;
        panel.repaint();
    }

    /**
     * Get the background image style of this panel
     * @return the imageStyle
     */
    public ImageFill getImageStyle() {
        return imageStyle;
    }

    public static void addToPanel(final Container panel, final UIComponent c, final GridBagConstraints gbc) throws UIWidgetException {
        if (panel != null && c != null && c.isValid()) {
            final Component comp = (Component) c.getComponent();
            final LayoutManager layout = panel.getLayout();
            UIAccessTools.execOnEDT(new Runnable() {
                public void run() {
                    if (layout instanceof BorderLayout) {
                        String constraint = c.getLayoutConstraint() == null ? "c" : c.getLayoutConstraint().get("position");
                        panel.add(comp, UILayoutFactory.BorderConstants.get(constraint));
                    } else if (layout instanceof GridBagLayout) {
                        if (c.getLayoutConstraint() == null) {
                            System.err.println("Invalid layout constraint: must not be empty" + c);
                            return;
                        }
                        add(panel, gbc, comp, c.getLayoutConstraint());
                    } else if (layout instanceof NoLayout) {
                        // In Swing (see JComponent::paintChildren), children are painted in reverse order
                        // to keep uicontrol compatibility, first added is first painted !
                        panel.add(comp, c.getNoLayoutConstraint(), 0);
                    } else {
                        panel.add(comp);
                    }
                }
            });
        }
    }

    /**
     * {@inheritDoc}
     */
    public void add(final UIComponent c) throws UIWidgetException {
        Object o = c.getComponent();
        if (o instanceof Component && !(o instanceof JPopupMenu)) {
            addToPanel(panel, c, gbc);
        } else {
            super.add(c);
        }
    }

    /**
     * {@inheritDoc}
     */
    public void finish() {
        if (enable != null && !enable.isEmpty() && !StringConverters.getObjectFromValue(boolean.class, enable)) {
            setEnable(false);
        }
        panel.revalidate();
    }

    /**
     * Add a component to this panel
     * @param c the component to add
     */
    public void add(Component c) {
        panel.add(c);
    }

    /**
     * Add a component to the base panel which have a GridBagLayout
     * @param base the target panel
     * @param gbc the constraints
     * @param c the component to add
     * @param attrs the cnosstraints attributes
     */
    private static final void add(Container base, GridBagConstraints gbc, Component c, Map<String, String> attrs) {
        gbc.gridx = StringConverters.getObjectFromValue(int.class, attrs.get("gridx"), 1) - 1;
        gbc.gridy = StringConverters.getObjectFromValue(int.class, attrs.get("gridy"), 1) - 1;
        gbc.gridwidth = StringConverters.getObjectFromValue(int.class, attrs.get("gridwidth"), 1);
        gbc.gridheight = StringConverters.getObjectFromValue(int.class, attrs.get("gridheight"), 1);
        gbc.weightx = StringConverters.getObjectFromValue(double.class, attrs.get("weightx"), 1d);
        gbc.weighty = StringConverters.getObjectFromValue(double.class, attrs.get("weighty"), 1d);
        gbc.ipadx = StringConverters.getObjectFromValue(int.class, attrs.get("ipadx"), 0);
        gbc.ipady = StringConverters.getObjectFromValue(int.class, attrs.get("ipady"), 0);
        gbc.insets = StringConverters.getObjectFromValue(Insets.class, attrs.get("insets"), new Insets(0, 0, 0, 0));

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
/*


*/