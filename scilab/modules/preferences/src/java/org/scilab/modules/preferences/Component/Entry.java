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

import javax.swing.JPasswordField;

import org.scilab.modules.preferences.XCommonManager;
import org.scilab.modules.preferences.XComponent;
import org.scilab.modules.preferences.XChooser;
import org.scilab.modules.preferences.XConfigManager;
import org.scilab.modules.gui.bridge.checkbox.SwingScilabCheckBox;

import org.w3c.dom.Node;

/** Implementation of Entry compliant with extended management.
 *
 * @author Pierre GRADIT
 *
 */
public class Entry extends JPasswordField implements XComponent, XChooser {

    /** Universal identifier for serialization.
     *
     */
    private static final long serialVersionUID = -7007541669965737408L;

    /** Define the set of actuators.
     *
     * @return array of actuator names.
     */
    public final String [] actuators() {
        String [] actuators = {"enable", "text", "columns", "lines", "editable"};
        return actuators;
    }

    /** Constructor.
     *
     * @param peer : associated view DOM node.
     */
    public Entry(final Node peer) {
        super();
        refresh(peer);
    }

    /** Refresh the component by the use of actuators.
     *
     * @param peer the corresponding view DOM node
     */
    public final void refresh(final Node peer) {
        String text = XCommonManager.getAttribute(peer, "text");
        String columns = XCommonManager.getAttribute(peer, "columns");
        if (!text.equals(text())) {
            text(text);
        }
        if (!columns.equals(columns())) {
            columns(columns);
        }
        String enable = XConfigManager.getAttribute(peer, "enable", "true");
        setEnabled(enable.equals("true"));

        String passwd = XConfigManager.getAttribute(peer, "password", "false");
        if (!"true".equals(passwd)) {
            setEchoChar('\0');
        }
    }

    /** Sensor for 'text' attribute.
     *
     * @return the attribute value.
     */
    public final String text() {
        return new String(getPassword());
    }

    /** Actuator for 'text' attribute.
     *
     * @param text : the attribute value.
     */
    public final void columns(final String columns) {
        if (! (columns.equals(XCommonManager.NAV))) {
            int jColumns = Integer.parseInt(columns);
            setColumns(jColumns);
        }
    }

    /** Sensor for 'columns' attribute.
     *
     * @return the attribute value.
     */
    public final String columns() {
        return "" + getColumns();
    }

    /** Actuator for 'text' attribute.
     *
     * @param text : the attribute value.
     */
    public final void text(final String text) {
        setText(text);
    }


    /** Actual response read by the listener.
     *
     * @return response read by the listener.
     */
    public final Object choose() {
        return text();
    }

    /** Developer serialization method.
     *
     * @return equivalent signature.
     */
    public final String toString() {
        String signature = "Entry";
        if (!text().equals(XCommonManager.NAV)) {
            signature += " text='" + text() + "'";
        }
        return signature;
    }
}
