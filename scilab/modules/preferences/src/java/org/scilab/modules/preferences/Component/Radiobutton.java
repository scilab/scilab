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

import org.scilab.modules.preferences.XComponent;
import org.scilab.modules.preferences.XChooser;
import org.scilab.modules.preferences.XConfigManager;
import org.scilab.modules.gui.bridge.radiobutton.SwingScilabRadioButton;

import org.w3c.dom.Node;

/** Implementation of Radiobutton compliant with extended management.
 *
 * @author Pierre GRADIT
 *
 */
public class Radiobutton extends SwingScilabRadioButton implements XComponent, XChooser {

    /** Universal identifier for serialization.
     *
     */
    private static final long serialVersionUID = -7007541669965737408L;

    private String expectedValue;

    /** Define the set of actuators.
     *
     * @return array of actuator names.
     */
    public final String [] actuators() {
        String [] actuators = {"enable", "text", "checked", "value", "expected-value"};
        return actuators;
    }

    /** Constructor.
     *
     * @param peer : associated view DOM node.
     */
    public Radiobutton(final Node peer) {
        super();
        setOpaque(false);
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

        String checked = XConfigManager.getAttribute(peer, "checked");
        if (checked.equals(XConfigManager.NAV)) {
            expectedValue = XConfigManager.getAttribute(peer, "expected-value");
            if (XConfigManager.getAttribute(peer, "value").equals(expectedValue)) {
                checked = "checked";
            } else {
                checked = "unchecked";
            }
        } else {
            expectedValue = null;
        }

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
        boolean state = checked.equals("checked");
        setSelected(state);
    }

    /** Actual response read by the listener.
     *
     * @return response read by the listener.
     */
    public final Object choose() {
        if (expectedValue != null) {
            return expectedValue;
        }

        if (isSelected()) {
            return "checked";
        }
        return "unchecked";
    }

    /** Developer serialization method.
     *
     * @return equivalent signature.
     */
    public final String toString() {
        String signature = "RadioButton";
        if (!text().equals(XConfigManager.NAV)) {
            signature += " text='" + text() + "'";
        }
        if (!checked().equals(XConfigManager.NAV)) {
            signature += " checked='" + checked() + "'";
        }
        return signature;
    }
}
