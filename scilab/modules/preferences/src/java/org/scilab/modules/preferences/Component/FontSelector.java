/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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

import java.awt.Dimension;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.GroupLayout;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JTextField;
import javax.swing.JPanel;
import javax.swing.SwingConstants;
import javax.swing.SwingUtilities;

import org.w3c.dom.Node;

import org.scilab.modules.gui.bridge.fontchooser.SwingScilabFontChooser;
import org.scilab.modules.preferences.XChooser;
import org.scilab.modules.preferences.XCommonManager;
import org.scilab.modules.preferences.XComponent;
import org.scilab.modules.preferences.XConfigManager;

/**
 * Implementation of Label compliant with extended management.
 *
 * @author Calixte DENIZET
 *
 */
public class FontSelector extends Panel implements XComponent, XChooser {

    /** Universal identifier for serialization.
     *
     */
    private static final long serialVersionUID = -4842434795956015959L;
    private ActionListener actionListener;
    private JTextField textField;
    private JButton button;
    private Font font = new Font("Monospaced", Font.PLAIN, 12);
    private int defaultSize;

    /** Define the set of actuators.
     *
     * @return array of actuator names.
     */
    public final String [] actuators() {
        return new String[] {"font-name", "font-face", "font-size", "enable"};
    }

    /** Constructor.
     *
     * @param peer : associated view DOM node.
     */
    public FontSelector(final Node peer) {
        super(peer);
        GroupLayout layout = new GroupLayout(this);
        setLayout(layout);

        textField = new JTextField();
        defaultSize = textField.getFont().getSize();
        textField.setEditable(false);

        button = new JButton("...");
        button.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                JFrame frame = (JFrame) SwingUtilities.getAncestorOfClass(JFrame.class, FontSelector.this);
                SwingScilabFontChooser fontChooser = new SwingScilabFontChooser(frame, font, false);
                fontChooser.setLocationRelativeTo(frame);
                fontChooser.displayAndWait();

                Font newFont = fontChooser.getSelectedFont();
                if (newFont != null && !newFont.equals(font)) {
                    font = newFont;
                    setTextField();
                    if (actionListener != null) {
                        actionListener.actionPerformed(new ActionEvent(FontSelector.this, 0, "Font Selector Value changed", System.currentTimeMillis(), 0));
                    }
                }
            }
        });

        layout.setHorizontalGroup(layout.createSequentialGroup().addComponent(textField).addComponent(button));
        layout.setVerticalGroup(layout.createParallelGroup().addComponent(textField).addComponent(button));

        setRequestFocusEnabled(true);
        setFocusable(true);

        String fontname = XConfigManager.getAttribute(peer , "font-name");
        fontname(fontname);

        String fontface = XConfigManager.getAttribute(peer , "font-face");
        fontface(fontface);

        String fontsize = XConfigManager.getAttribute(peer , "font-size");
        fontsize(fontsize);
    }

    private void setTextField() {
        textField.setFont(font.deriveFont((float) defaultSize));
        textField.setText(fontname() + " " + fontsize() + " " + fontface());
    }

    /** Refresh the component by the use of actuators.
     *
     * @param peer the corresponding view DOM node
     */
    public void refresh(final Node peer) {
        String fontname = XConfigManager.getAttribute(peer , "font-name");
        if (!fontname.equals(fontname())) {
            fontname(fontname);
        }

        String fontface = XConfigManager.getAttribute(peer , "font-face");
        if (!fontface.equals(fontface())) {
            fontface(fontface);
        }

        String fontsize = XConfigManager.getAttribute(peer , "font-size");
        if (!fontsize.equals(fontsize())) {
            fontsize(fontsize);
        }

        String enable = XConfigManager.getAttribute(peer , "enable", "true");
        textField.setEnabled(enable.equals("true"));
        button.setEnabled(enable.equals("true"));
    }

    public Dimension getMaximumSize() {
        return getPreferredSize();
    }

    /** Sensor for 'font-size' attribute.
     *
     * @return the attribute value.
     */
    public final String fontsize() {
        return Integer.toString(font.getSize());
    }

    /** Sensor for 'font-face' attribute.
     *
     * @return the attribute value.
     */
    public final String fontface() {
        switch (font.getStyle()) {
            case Font.PLAIN :
                return "plain";
            case Font.BOLD :
                return "bold";
            case Font.ITALIC :
                return "italic";
            case Font.BOLD | Font.ITALIC :
                return "bold italic";
            default :
                return "plain";
        }
    }

    /** Sensor for 'font-name' attribute.
     *
     * @return the attribute value.
     */
    public final String fontname() {
        return font.getName();
    }

    /** Actuator for 'font-size' attribute.
     *
     * @param text : the attribute value.
     */
    public final void fontsize(String fontsize) {
        if (!fontsize.equals(XCommonManager.NAV) && !fontsize.equals("")) {
            try {
                int size = Integer.parseInt(fontsize);
                font = font.deriveFont((float) size);
                setTextField();
            } catch (NumberFormatException e) { }
        }
    }

    /** Actuator for 'font-face' attribute.
     *
     * @param text : the attribute value.
     */
    public final void fontface(String fontface) {
        if (!fontface.equals(XCommonManager.NAV) && !fontface.equals("")) {
            int style = Font.PLAIN;
            if (fontface.equalsIgnoreCase("bold")) {
                style = Font.BOLD;
            } else if (fontface.equalsIgnoreCase("italic")) {
                style = Font.ITALIC;
            } else if (fontface.equalsIgnoreCase("bold italic")) {
                style = Font.BOLD | Font.ITALIC;
            }
            font = font.deriveFont(style);
            setTextField();
        }
    }

    /** Actuator for 'font-name' attribute.
     *
     * @param text : the attribute value.
     */
    public final void fontname(String fontname) {
        if (!fontname.equals(XCommonManager.NAV) && !fontname.equals("")) {
            font = new Font(fontname, font.getStyle(), font.getSize());
            setTextField();
        }
    }

    /** Actual response read by the listener.
     *
     * @return response read by the listener.
     */
    public final Object choose() {
        return new String[] {fontname(), fontface(), fontsize()};
    }

    public void addActionListener(ActionListener actionListener) {
        this.actionListener = actionListener;
    }

    /** Developer serialization method.
     *
     * @return equivalent signature.
     */
    public final String toString() {
        StringBuilder signature = new StringBuilder("FontSelector");

        if (!fontname().equals(XConfigManager.NAV)) {
            signature.append(" font-name='");
            signature.append(fontname());
            signature.append("'");
        }

        if (!fontface().equals(XConfigManager.NAV)) {
            signature.append(" font-face='");
            signature.append(fontface());
            signature.append("'");
        }

        if (!fontsize().equals(XConfigManager.NAV)) {
            signature.append(" font-size='");
            signature.append(fontsize());
            signature.append("'");
        }

        return signature.toString();
    }
}
