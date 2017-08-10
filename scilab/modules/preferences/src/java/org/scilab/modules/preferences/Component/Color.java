/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Pierre GRADIT
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.preferences.Component;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.Icon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.SwingUtilities;

import org.w3c.dom.Node;

import org.scilab.modules.gui.bridge.colorchooser.SwingScilabColorChooser;
import org.scilab.modules.preferences.XChooser;
import org.scilab.modules.preferences.XComponent;
import org.scilab.modules.preferences.XCommonManager;
import org.scilab.modules.preferences.XConfigManager;
import org.scilab.modules.preferences.XSentinel;

/**
 * Implementation of Label compliant with extended management.
 *
 * @author Pierre GRADIT
 * @author Calixte DENIZET
 */
public class Color extends JButton implements XComponent, XChooser {

    /**
     *
     */
    private static final long serialVersionUID = 5598263085800128888L;
    private static final int ICONDIM = 16;

    private ActionListener actionListener = null;
    private SwingScilabColorChooser colorChooser;

    /**
     * Define the set of actuators.
     * @return array of actuator names.
     */
    public final String[] actuators() {
        String[] actuators = {"enable", "color"};
        return actuators;
    }

    /**
     * Constructor.
     * @param peer : associated view DOM node.
     */
    public Color(final Node peer) {
        super(new Icon() {
            public final int getIconHeight() {
                return ICONDIM;
            }

            public final int getIconWidth() {
                return ICONDIM * 2;
            }

            public void paintIcon(Component c, Graphics g, int x, int y) {
                if (c.isEnabled()) {
                    g.setColor(c.getForeground());
                    g.fillRect(x, y, getIconWidth() - 1, getIconHeight() - 1);
                    g.setColor(java.awt.Color.BLACK);
                    g.drawRect(x, y, getIconWidth() - 1, getIconHeight() - 1);
                } else {
                    java.awt.Color color = c.getForeground();
                    float hsb[] = new float[3];
                    java.awt.Color.RGBtoHSB(color.getRed(), color.getGreen(), color.getBlue(), hsb);
                    g.setColor(java.awt.Color.getHSBColor(hsb[0], hsb[1] * 0.2f, hsb[2] * 0.95f));
                    g.fillRect(x, y, getIconWidth(), getIconHeight());
                }
            }
        });

        String color = XCommonManager.getAttribute(peer , "color", "000000");
        color(color);
        setOpaque(true);
        addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                java.awt.Color jColor = XCommonManager.getColor(color());
                ActionEvent transmit  = new ActionEvent(Color.this, e.getID(), "Color change", e.getWhen() + 1, e.getModifiers());
                colorChooser = new SwingScilabColorChooser(jColor);
                JFrame frame = (JFrame) SwingUtilities.getAncestorOfClass(JFrame.class, Color.this);
                colorChooser.setLocationRelativeTo(frame);
                colorChooser.displayAndWait();
                if (actionListener != null) {
                    actionListener.actionPerformed(transmit);
                }
            }
        });

        setRequestFocusEnabled(true);
        setFocusable(true);

        String enable = XConfigManager.getAttribute(peer , "enable", "true");
        setEnabled(enable.equals("true"));
    }

    /**
     * Refresh the component by the use of actuators.
     * @param peer the corresponding view DOM node
     */
    public final void refresh(final Node peer) {
        String color = XCommonManager.getAttribute(peer , "color", "000000");
        if (!color.equals(color())) {
            color(color);
        }

        String enable = XConfigManager.getAttribute(peer , "enable", "true");
        setEnabled(enable.equals("true"));
    }

    /**
     * Sensor for 'color' attribute.
     * @return the attribute value.
     */
    public final String color() {
        java.awt.Color color = getForeground();
        return XCommonManager.getColor(color);
    }

    /**
     * Actuator for 'color' attribute.
     * @param text : the attribute value.
     */
    public final void color(final String color) {
        java.awt.Color jColor = XCommonManager.getColor(color);
        setForeground(jColor);
    }

    /**
     * Registration of a single listener.
     * @param listener
     */
    public void addActionListener(ActionListener listener) {
        if (listener instanceof XSentinel) {
            actionListener = listener;
        } else {
            super.addActionListener(listener);
        }
    }

    /**
     * External consultation
     */
    public Object choose() {
        java.awt.Color jColor = colorChooser.getSelectedColor();
        if (jColor != null) {
            return XCommonManager.getColor(jColor);
        } else {
            return color();
        }
    }

    /**
     * Developer serialization method.
     * @return equivalent signature.
     */
    public final String toString() {
        String signature = "Color";
        signature += " color='" + color() + "'";
        return signature;
    }
}

