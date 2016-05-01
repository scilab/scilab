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

import java.awt.Color;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.regex.Pattern;

import javax.swing.BorderFactory;
import javax.swing.JScrollBar;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.ScrollPaneConstants;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;

import org.scilab.modules.preferences.XChooser;
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
public class TextArea extends Panel implements XComponent, XChooser, DocumentListener {

    /** Universal identifier for serialization.
     *
     */
    private static final long serialVersionUID = -7007541669965737408L;

    private JTextArea textarea;
    private JScrollPane scrollPane;
    private ActionListener actionListener;

    /** Define the set of actuators.
     *
     * @return array of actuator names.
     */
    public final String [] actuators() {
        String [] actuators = {"enable", "text", "columns", "rows", "editable", "scroll"};
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
        scrollPane = new JScrollPane(textarea);
        add(scrollPane);
        textarea.setFocusable(true);
        textarea.setColumns(10);
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
        String scroll = XCommonManager.getAttribute(peer, "scroll");

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

        if (!scroll.equals(scroll())) {
            scroll(scroll);
        }

        boolean enable = XConfigManager.getBoolean(peer, "enable", true);
        textarea.setEnabled(enable);
        JScrollBar bar = scrollPane.getVerticalScrollBar();
        if (bar != null) {
            bar.setEnabled(enable);
        }

        bar = scrollPane.getHorizontalScrollBar();
        if (bar != null) {
            bar.setEnabled(enable);
        }
    }

    /**
     * {@inheritDoc}
     */
    public void changedUpdate(DocumentEvent e) { }

    /**
     * {@inheritDoc}
     */
    public void insertUpdate(DocumentEvent e) {
        update();
    }

    /**
     * {@inheritDoc}
     */
    public void removeUpdate(DocumentEvent e) {
        update();
    }

    private void update() {
        if (actionListener != null) {
            ActionEvent transmit = new ActionEvent(this, 0, "TextArea change", System.currentTimeMillis(), 0);
            actionListener.actionPerformed(transmit);
        }
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
    public final void scroll(final String scroll) {
        scrollPane.setVerticalScrollBarPolicy(scroll.equalsIgnoreCase("true") ? ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED : ScrollPaneConstants.VERTICAL_SCROLLBAR_NEVER);
        scrollPane.setHorizontalScrollBarPolicy(scroll.equalsIgnoreCase("true") ? ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED : ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
    }

    /** Sensor for 'columns' attribute.
     *
     * @return the attribute value.
     */
    public final String scroll() {
        return Boolean.toString(scrollPane.getVerticalScrollBarPolicy() == ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED);
    }

    /** Actuator for 'text' attribute.
     *
     * @param text : the attribute value.
     */
    public final void text(final String text) {
        textarea.setText(text);
    }

    public void addActionListener(ActionListener actionListener) {
        this.actionListener = actionListener;
    }

    public void addDocumentListener(DocumentListener listener) {
        textarea.getDocument().addDocumentListener(listener);
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
