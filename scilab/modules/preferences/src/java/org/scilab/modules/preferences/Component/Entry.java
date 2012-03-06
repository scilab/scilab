/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Pierre GRADIT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.preferences.Component;

import javax.swing.JTextField;

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
public class Entry extends JTextField implements XComponent, XChooser {

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
    }

    /** Sensor for 'text' attribute.
     *
     * @return the attribute value.
     */
    public final String text() {
        return getText();
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
