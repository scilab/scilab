/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Pierre GRADIT
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

import java.awt.Color;
import java.awt.Font;

import javax.swing.JLabel;
import javax.swing.SwingConstants;

import org.w3c.dom.Node;

import org.scilab.modules.preferences.XCommonManager;
import org.scilab.modules.preferences.XComponent;
import org.scilab.modules.preferences.XConfigManager;

/** Implementation of Label compliant with extended management.
 *
 * @author Pierre GRADIT
 *
 */
public class Label extends JLabel implements XComponent {

    /** Universal identifier for serialization.
     *
     */
    private static final long serialVersionUID = -4842434795956015957L;

    /** Define the set of actuators.
     *
     * @return array of actuator names.
     */
    public final String [] actuators() {
        String [] actuators = {"text", "foreground", "background", "tooltip", "font-family", "font-face", "font-size", "enable"};
        return actuators;
    }

    /** Constructor.
     *
     * @param peer : associated view DOM node.
     */
    public Label(final Node peer) {
        super();

        String text = XConfigManager.getAttribute(peer, "text");
        text(text);

        String tooltip = XConfigManager.getAttribute(peer, "tooltip");
        tooltip(tooltip);

        String background = XConfigManager.getAttribute(peer, "background");
        background(background);

        String foreground = XConfigManager.getAttribute(peer, "foreground");
        foreground(foreground);

        String fontFamily = XConfigManager.getAttribute(peer, "font-family", fontFamily());
        fontFamily(fontFamily);

        String fontFace = XConfigManager.getAttribute(peer, "font-face", fontFace());
        fontFace(fontFace);

        String fontSize = XConfigManager.getAttribute(peer, "font-size", fontSize());
        fontSize(fontSize);

        String enable = XConfigManager.getAttribute(peer, "enable", "true");
        setEnabled(enable.equals("true"));
    }

    /** Refresh the component by the use of actuators.
     *
     * @param peer the corresponding view DOM node
     */
    public void refresh(final Node peer) {
        String text = XConfigManager.getAttribute(peer, "text");
        if (!text.equals(text())) {
            text(text);
        }

        String tooltip = XConfigManager.getAttribute(peer, "tooltip");
        if (!tooltip.equals(tooltip())) {
            tooltip(tooltip);
        }

        String background = XConfigManager.getAttribute(peer, "background");
        if (!background.equals(background())) {
            background(background);
        }

        String foreground = XConfigManager.getAttribute(peer, "foreground");
        if (!foreground.equals(foreground())) {
            foreground(foreground);
        }

        String fontFamily = XConfigManager.getAttribute(peer, "font-family", fontFamily());
        if (!fontFamily.equals(fontFamily())) {
            fontFamily(fontFamily);
        }

        String fontFace = XConfigManager.getAttribute(peer, "font-face", fontFace());
        if (!fontFace.equals(fontFace())) {
            fontFace(fontFace);
        }

        String fontSize = XConfigManager.getAttribute(peer, "font-size", fontSize());
        if (!fontSize.equals(fontSize())) {
            fontSize(fontSize);
        }

        String halign = XConfigManager.getAttribute(peer, "halign", "left");
        if (halign.equals("left")) {
            setHorizontalAlignment(SwingConstants.LEFT);
        }
        if (halign.equals("right")) {
            setHorizontalAlignment(SwingConstants.RIGHT);
        }

        String enable = XConfigManager.getAttribute(peer, "enable", "true");
        setEnabled(enable.equals("true"));
    }

    /** Sensor for 'text' attribute.
     *
     * @return the attribute value.
     */
    public final String text() {
        return getText();
    }

    /** Sensor for 'tooltip' attribute.
     *
     * @return the attribute value.
     */
    public final String tooltip() {
        String tooltip = getToolTipText();
        if (tooltip == null) {
            return "";
        }
        return tooltip;
    }

    /** Sensor for 'foreground' attribute.
     *
     * @return the attribute value.
     */
    public final String foreground() {
        return XConfigManager.getColor(getForeground());
    }

    /** Sensor for 'background' attribute.
     *
     * @return the attribute value.
     */
    public final String background() {
        Color fg = getForeground();
        if (fg != null && fg.equals(getParent().getBackground())) {
            return XConfigManager.NAV;
        } else {
            return XConfigManager.getColor(getBackground());
        }
    }

    /** Sensor for 'font-family' attribute.
     *
     * @return the attribute value.
     */
    public final String fontFamily() {
        return getFont().getFamily();
    }

    /** Sensor for 'font-size' attribute.
     *
     * @return the attribute value.
     */
    public final String fontSize() {
        return "" + getFont().getSize();
    }

    /** Sensor for 'font-face' attribute.
     *
     * @return the attribute value.
     */
    public final String fontFace() {
        switch (getFont().getStyle()) {
            case Font.PLAIN :
                return "plain";
            case Font.ITALIC :
                return "italic";
            case Font.BOLD :
                return "bold";
            case 3 :
                return "bold italic";
        }
        return XCommonManager.NAV;
    }

    /** Actuator for 'font-family' attribute.
     *
     * @return the attribute value.
     */
    public final void fontFamily(String fontFamily) {
        Font font = new Font(
            fontFamily,
            getFont().getStyle(),
            getFont().getSize());
        setFont(font);
    }

    /** Actuator for 'font-size' attribute.
     *
     * @return the attribute value.
     */
    public final void fontSize(String fontSize) {
        Font font = new Font(
            getFont().getFamily(),
            getFont().getStyle(),
            Integer.parseInt(fontSize));
        setFont(font);
    }

    /** Accelerator for 'font-face' actuator.
     *
     * @param xFace
     * @return
     */
    private int getStyle(String xFace) {
        if (xFace.equals("plain")) {
            return Font.PLAIN;
        }
        if (xFace.equals("italic")) {
            return Font.ITALIC;
        }
        if (xFace.equals("bold")) {
            return Font.BOLD;
        }
        if (xFace.equals("bold italic")) {
            return Font.BOLD + Font.ITALIC;
        }
        return -1;
    }
    /** Actuator for 'font-face' attribute.
     *
     * @return the attribute value.
     */
    public final void fontFace(String fontFace) {
        Font font = new Font(
            getFont().getFamily(),
            getStyle(fontFace),
            getFont().getSize());
        setFont(font);
    }

    /** Actuator for 'text' attribute.
     *
     * @param text : the attribute value.
     */
    public final void text(final String text) {
        setText(text);
    }

    /** Actuator for 'tooltip' attribute.
     *
     * @param text : the attribute value.
     */
    public final void tooltip(String tooltip) {
        if (tooltip.equals(XCommonManager.NAV)
                || tooltip.equals("")
           ) {
            tooltip = null;
        }
        setToolTipText(tooltip);
    }

    /** Actuator for 'foreground' attribute.
     *
     * @param foreground : the attribute value.
     */
    public final void foreground(final String foreground) {
        if (foreground.equals(XConfigManager.NAV)) {
            setForeground(null);
        } else {
            Color color = XConfigManager.getColor(foreground);
            //System.err.println("foreground:" + actuator + " = " + color);
            setForeground(color);
        }
    }

    /** Actuator for 'background' attribute.
     *
     * @param background : the attribute value.
     */
    public final void background(final String background) {
        if (background.equals(XConfigManager.NAV)) {
            setOpaque(false);
            setBackground(null);
        } else {
            Color color = XConfigManager.getColor(background);
            //System.err.println("background:" + actuator + " = " + color);
            setOpaque(true);
            setBackground(color);
        }
    }

    /** Developer serialization method.
     *
     * @return equivalent signature.
     */
    public final String toString() {
        String signature = "Label";

        if (!text().equals(XConfigManager.NAV)) {
            signature += " text='" + text() + "'";
        }
        if (!background().equals(XConfigManager.NAV)) {
            signature += " background='" + background() + "'";
        }
        if (!foreground().equals(XConfigManager.NAV)) {
            signature += " foreground='" + foreground() + "'";
        }
        if (!fontFamily().equals(XConfigManager.NAV)) {
            signature += " font-family='" + fontFamily() + "'";
        }
        if (!fontFace().equals(XConfigManager.NAV)) {
            signature += " font-face='" + fontFace() + "'";
        }
        if (!fontSize().equals(XConfigManager.NAV)) {
            signature += " font-size='" + fontSize() + "'";
        }
        return signature;
    }
}

