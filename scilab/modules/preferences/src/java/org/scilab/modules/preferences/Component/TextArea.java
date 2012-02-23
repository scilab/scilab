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

package org.scilab.modules.preferences.Component;

import java.awt.Color;

import javax.swing.BorderFactory;
import javax.swing.JTextArea;

import org.scilab.modules.preferences.XCommonManager;
import org.scilab.modules.preferences.XComponent;
import org.scilab.modules.preferences.XConfigManager;

import org.w3c.dom.Node;

/** 
 * Implementation of a TextArea.
 *
 * @author Calixte DENIZET
 *
 */
public class TextArea extends Panel implements XComponent {

    /** Universal identifier for serialization.
     *
     */
    private static final long serialVersionUID = -7007541669965737408L;

    private JTextArea textarea;

    /** Define the set of actuators.
     *
     * @return array of actuator names.
     */
    public final String [] actuators() {
        String [] actuators = {"enable", "text", "columns", "rows", "editable"};
        return actuators;
    }

    /** Constructor.
     *
     * @param peer : associated view DOM node.
     */
    public TextArea(final Node peer) {
        super(peer);
	textarea = new JTextArea();
	textarea.setBorder(BorderFactory.createLineBorder(Color.GRAY));
	add(textarea);
        refresh(peer);
    }

    /** Refresh the component by the use of actuators.
     *
     * @param peer the corresponding view DOM node
     */
    public final void refresh(final Node peer) {
        String text = XCommonManager.getAttribute(peer, "text");
        String columns = XCommonManager.getAttribute(peer, "columns");
	String rows = XCommonManager.getAttribute(peer, "rows");
	String editable = XCommonManager.getAttribute(peer, "editable");

        if (!text.equals(text())) {
            text(text);
        }

        if (!columns.equals(columns())) {
            columns(columns);
        }

        if (!rows.equals(rows())) {
            rows(rows);
        }

        if (!editable.equals(editable())) {
            editable(editable);
        }

        boolean enable = XConfigManager.getBoolean(peer, "enable", true);
        textarea.setEnabled(enable);
    }

    /** Sensor for 'text' attribute.
     *
     * @return the attribute value.
     */
    public final String text() {
        return textarea.getText();
    }

    /** Actuator for 'text' attribute.
     *
     * @param text : the attribute value.
     */
    public final void columns(final String columns) {
        if (!columns.equals(XCommonManager.NAV)) {
            int jColumns = Integer.parseInt(columns);
            textarea.setColumns(jColumns);
        }
    }

    /** Sensor for 'columns' attribute.
     *
     * @return the attribute value.
     */
    public final String columns() {
        return Integer.toString(textarea.getColumns());
    }

    /** Actuator for 'text' attribute.
     *
     * @param text : the attribute value.
     */
    public final void rows(final String rows) {
        if (!rows.equals(XCommonManager.NAV)) {
            int jRows = Integer.parseInt(rows);
            textarea.setRows(jRows);
        }
    }

    /** Sensor for 'columns' attribute.
     *
     * @return the attribute value.
     */
    public final String rows() {
        return Integer.toString(textarea.getRows());
    }

    /** Actuator for 'text' attribute.
     *
     * @param text : the attribute value.
     */
    public final void editable(final String editable) {
	textarea.setEditable(editable.equalsIgnoreCase("true"));
    }

    /** Sensor for 'columns' attribute.
     *
     * @return the attribute value.
     */
    public final String editable() {
        return Boolean.toString(textarea.isEditable());
    }

    /** Actuator for 'text' attribute.
     *
     * @param text : the attribute value.
     */
    public final void text(final String text) {
        textarea.setText(text);
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
        String signature = "TextArea";
        if (!text().equals(XCommonManager.NAV)) {
            signature += " text='" + text() + "'";
        }
        return signature;
    }
}
