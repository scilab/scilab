/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
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

import org.scilab.modules.gui.bridge.slider.SwingScilabSlider;
import org.scilab.modules.gui.widget.Widget;
import org.scilab.modules.preferences.XCommonManager;
import org.scilab.modules.preferences.XComponent;
import org.scilab.modules.preferences.XComponentAttributes;
import org.scilab.modules.preferences.XConfigManager;
import org.w3c.dom.Node;

/**
 * Implementation of Label compliant with extended management.
 * @author Vincent COUVERT
 *
 */
public class Slider extends SwingScilabSlider implements XComponent, Widget {

    /** Universal identifier for serialization.
     *
     */
    private static final long serialVersionUID = -4842434795956015957L;

    /** Constructor.
     *
     * @param peer : associated view DOM node.
     */
    public Slider(final Node peer) {
        super();
        setHorizontal();

        String tooltip = XConfigManager.getAttribute(peer , XComponentAttributes.TOOLTIP);
        tooltip(tooltip);

        String background = XConfigManager.getAttribute(peer , XComponentAttributes.BACKGROUND);
        background(background);

        String foreground = XConfigManager.getAttribute(peer , XComponentAttributes.FOREGROUND);
        foreground(foreground);
    }

    /**
     * Define the set of actuators.
     * @return array of actuator names.
     */
    public final String [] actuators() {
        String [] actuators = {XComponentAttributes.FOREGROUND,
                               XComponentAttributes.BACKGROUND,
                               XComponentAttributes.TOOLTIP
                              };
        return actuators;
    }

    /**
     * Refresh the component by the use of actuators.
     * @param peer the corresponding view DOM node
     */
    public void refresh(final Node peer) {

        String tooltip = XConfigManager.getAttribute(peer , "tooltip");
        if (!tooltip.equals(tooltip())) {
            tooltip(tooltip);
        }

        String background = XConfigManager.getAttribute(peer , "background");
        if (!background.equals(background())) {
            background(background);
        }

        String foreground = XConfigManager.getAttribute(peer , "foreground");
        if (!foreground.equals(foreground())) {
            foreground(foreground);
        }

        setRequestFocusEnabled(true);
        setFocusable(true);
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
        if (getForeground().equals(getParent().getForeground())) {
            return XConfigManager.NAV;
        } else {
            return XConfigManager.getColor(getForeground());
        }
    }

    /** Sensor for 'background' attribute.
     *
     * @return the attribute value.
     */
    public final String background() {
        if (getForeground().equals(getParent().getBackground())) {
            return XConfigManager.NAV;
        } else {
            return XConfigManager.getColor(getBackground());
        }
    }


    /**
     * Actuator for 'tooltip' attribute.
     * @param tooltip : the attribute value.
     */
    public final void tooltip(String tooltip) {
        if (tooltip.equals(XCommonManager.NAV) || tooltip.equals("")) {
            setToolTipText(null);
        } else {
            setToolTipText(tooltip);
        }
    }

    /**
     * Actuator for 'foreground' attribute.
     * @param foreground : the attribute value.
     */
    public final void foreground(final String foreground) {
        if (foreground.equals(XConfigManager.NAV)) {
            setForeground(null);
        } else {
            Color color = XConfigManager.getColor(foreground);
            setForeground(color);
        }
    }

    /**
     * Actuator for 'background' attribute.
     * @param background : the attribute value.
     */
    public final void background(final String background) {
        if (background.equals(XConfigManager.NAV)) {
            setOpaque(false);
            setBackground(null);
        } else {
            Color color = XConfigManager.getColor(background);
            setOpaque(true);
            setBackground(color);
        }
    }

    /**
     * Developer serialization method.
     * @return equivalent signature.
     */
    public final String toString() {
        String signature = "Label";

        if (!background().equals(XConfigManager.NAV)) {
            signature += " background='" + background() + "'";
        }
        if (!foreground().equals(XConfigManager.NAV)) {
            signature += " foreground='" + foreground() + "'";
        }
        return signature;
    }
}

