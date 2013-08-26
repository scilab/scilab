/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Pierre GRADIT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.preferences.Component;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;

import javax.swing.BorderFactory;
import javax.swing.border.TitledBorder;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.UIManager;

import org.w3c.dom.Node;

import org.scilab.modules.preferences.XComponent;
import org.scilab.modules.preferences.XConfigManager;

/**
 * Implementation of Title compliant with extended management.
 *
 * @author Pierre GRADIT
 *
 */
@SuppressWarnings(value = { "serial" })
public class Title extends Panel implements XComponent {

    /** Universal identifier for serialization.
     *
     */
    private static final long serialVersionUID = 6183280975436648612L;

    /** Define the set of actuators.
    *
    * @return array of actuator names.
    */
    public String[] actuators() {
        return new String[] {};
    }

    /** Constructor.
    *
    * @param peer : associated view DOM node.
    */
    public Title(final Node peer) {
        super(peer);
        String text = XConfigManager.getAttribute(peer, "text");
        TitledBorder title = new TitledBorder(text) {
            public void paintBorder(Component c, Graphics g, int x, int y, int width, int height) {
                super.paintBorder(c, g, x, y - TitledBorder.EDGE_SPACING - 1, width, height + 2 * TitledBorder.EDGE_SPACING + 2);
            }
        };

        Font font = title.getTitleFont();
        if (font == null) {
            font = UIManager.getDefaults().getFont("TitledBorder.font");
            if (font == null) {
                JLabel label = new JLabel("a");
                font = label.getFont();
                if (font == null) {
                    font = new Font("Sans Serif", Font.PLAIN, 10);
                }
            }
        }
        title.setTitleFont(font.deriveFont(Font.BOLD));
        setBorder(title);
        XConfigManager.setDimension(this, peer);

        String background = XConfigManager.getAttribute(peer, "background");
        if (!(background.equals(XConfigManager.NAV))) {
            Color color = XConfigManager.getColor(background);
            setOpaque(true);
            setBackground(color);
        }
    }

    /** Refresh the component by the use of actuators.
    *
    * @param peer the corresponding view DOM node
    */
    public void refresh(final Node peer) { }

    /** Developer serialization method.
    *
    * @return equivalent signature.
    */
    public final String toString() {
        return "Title";
    }
}

