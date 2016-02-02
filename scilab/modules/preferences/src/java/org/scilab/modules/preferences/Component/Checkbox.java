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

import org.scilab.modules.preferences.XComponent;
import org.scilab.modules.preferences.XChooser;
import org.scilab.modules.preferences.XConfigManager;
import org.scilab.modules.gui.bridge.checkbox.SwingScilabCheckBox;

import org.w3c.dom.Node;

/** Implementation of Checkbox compliant with extended management.
 *
 * @author Pierre GRADIT
 *
 */
public class Checkbox extends SwingScilabCheckBox implements XComponent, XChooser {

    /** Universal identifier for serialization.
     *
     */
    private static final long serialVersionUID = -7007541669965737408L;

    private String selectedValue;
    private String unselectedValue;

    /** Define the set of actuators.
     *
     * @return array of actuator names.
     */
    public final String[] actuators() {
        String[] actuators = {"enable", "text", "checked", "selected-value", "unselected-value"};
        return actuators;
    }

    /** Constructor.
     *
     * @param peer : associated view DOM node.
     */
    public Checkbox(final Node peer) {
        super();
        setOpaque(false);
        setRequestFocusEnabled(true);
        setFocusable(true);

        refresh(peer);
    }

    /** Refresh the component by the use of actuators.
     *
     * @param peer the corresponding view DOM node
     */
    public final void refresh(final Node peer) {
        String text = XConfigManager.getAttribute(peer, "text");
        if (!text.equals(text())) {
            text(text);
        }

        String selectedValue = XConfigManager.getAttribute(peer, "selected-value");
        if (!selectedValue.equals(selected())) {
            selected(selectedValue);
        }

        String unselectedValue = XConfigManager.getAttribute(peer, "unselected-value");
        if (!unselectedValue.equals(unselected())) {
            unselected(unselectedValue);
        }

        String checked = XConfigManager.getAttribute(peer, "checked");
        if (!checked.equals(checked())) {
            checked(checked);
        }

        String enable = XConfigManager.getAttribute(peer, "enable", "true");
        setEnabled(enable.equals("true"));
    }

    /** Sensor for 'text' attribute.
     *
     * @return the attribute value.
     */
    public final String text() {
        String text = getText();
        if (text != null) {
            return text;
        } else {
            return XConfigManager.NAV;
        }
    }

    /** Actuator for 'text' attribute.
     *
     * @param text : the attribute value.
     */
    public final void text(final String text) {
        if (text != XConfigManager.NAV) {
            setText(text);
        } else {
            setText(null);
        }
    }

    /** Sensor for 'checked' attribute.
     *
     * @return the attribute value.
     */
    public final String checked() {
        boolean state = isSelected();
        if (state) {
            return "checked";
        } else {
            return "unchecked";
        }
    }

    /** Actuator for 'checked' attribute.
     *
     * @param text : the attribute value.
     */
    public final void checked(final String checked) {
        boolean state = checked.equals("checked") || checked.equals("true");
        setSelected(state);
    }

    /** Sensor for 'checked' attribute.
     *
     * @return the attribute value.
     */
    public final String selected() {
        return selectedValue;
    }

    /** Actuator for 'checked' attribute.
     *
     * @param text : the attribute value.
     */
    public final void selected(final String selected) {
        if (!selected.equals(XConfigManager.NAV)) {
            selectedValue = selected;
        } else {
            selectedValue = null;
        }
    }

    /** Sensor for 'checked' attribute.
     *
     * @return the attribute value.
     */
    public final String unselected() {
        return unselectedValue;
    }

    /** Actuator for 'checked' attribute.
     *
     * @param text : the attribute value.
     */
    public final void unselected(final String unselected) {
        if (!unselected.equals(XConfigManager.NAV)) {
            unselectedValue = unselected;
        } else {
            unselectedValue = null;
        }
    }

    /** Actual response read by the listener.
     *
     * @return response read by the listener.
     */
    public final Object choose() {
        if (isSelected()) {
            if (selectedValue != null) {
                return selectedValue;
            }
            return "checked";
        }

        if (unselectedValue != null) {
            return unselectedValue;
        }
        return "unchecked";
    }

    /** Developer serialization method.
     *
     * @return equivalent signature.
     */
    public final String toString() {
        String signature = "CHECKBOX";
        if (!text().equals(XConfigManager.NAV)) {
            signature += " text='" + text() + "'";
        }
        if (!checked().equals(XConfigManager.NAV)) {
            signature += " checked='" + checked() + "'";
        }
        return signature;
    }
}

