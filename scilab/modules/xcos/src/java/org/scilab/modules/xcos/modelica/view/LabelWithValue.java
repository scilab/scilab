/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2010 - DIGITEO - Clement DAVID <clement.david@scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.modelica.view;

import javax.swing.Icon;
import javax.swing.JPanel;
import javax.swing.SwingConstants;
import javax.swing.UIManager;

/**
 * A panel with a title and a text label
 */
@SuppressWarnings(value = { "serial" })
public final class LabelWithValue extends JPanel {
    private static final int TEXT_DEFAULT_COLUMNS = 3;
    private static final Icon ERROR_ICON = (Icon) UIManager
                                           .get("OptionPane.errorIcon");
    private static final Icon MESSAGE_ICON = (Icon) UIManager
            .get("OptionPane.informationIcon");

    private javax.swing.JLabel jLabel1;
    private javax.swing.JTextField jTextField1;

    /**
     * Default constructor
     */
    public LabelWithValue() {
        this(null, null);
    }

    /**
     * Constructor with title
     *
     * @param title
     *            the title
     */
    public LabelWithValue(String title) {
        this(title, null);
    }

    /**
     * Constructor with title and value.
     *
     * @param title
     *            the title
     * @param value
     *            the value
     */
    public LabelWithValue(String title, String value) {
        initComponents();
        setTitle(title);
        setText(value);
    }

    /**
     * Init the components of this label
     */
    private void initComponents() {

        jLabel1 = new javax.swing.JLabel();
        jTextField1 = new javax.swing.JTextField();

        jLabel1.setHorizontalAlignment(SwingConstants.TRAILING);
        jTextField1.setColumns(TEXT_DEFAULT_COLUMNS);

        setLayout(new java.awt.BorderLayout());
        add(jLabel1, java.awt.BorderLayout.CENTER);

        jTextField1.setEditable(false);
        add(jTextField1, java.awt.BorderLayout.EAST);
    }

    /**
     * @return the title
     */
    public String getTitle() {
        return jLabel1.getText();
    }

    /**
     * @param title
     *            the title to set
     */
    public void setTitle(String title) {
        jLabel1.setText(title + " :");
    }

    /**
     * @return the text
     */
    public String getText() {
        return jTextField1.getText();
    }

    /**
     * @param text
     *            the text to set
     */
    public void setText(String text) {
        jTextField1.setText(text);
    }

    /**
     * Set the number of column visible
     *
     * @param column
     *            the number of column visible
     */
    public void setColumns(int column) {
        jTextField1.setColumns(column);
    }

    /**
     * Set the value editable
     *
     * @param b
     *            the status of the value
     */
    public void setEditable(boolean b) {
        jTextField1.setEditable(b);
    }

    /**
     * Test if the value is editable
     *
     * @return the state
     */
    public boolean isEditable() {
        return jTextField1.isEditable();
    }

    /**
     * Set or clear the error
     *
     * @param error
     *            true, if the error is set, false otherwise.
     */
    public void setError(String error) {
        jLabel1.setIcon(ERROR_ICON);

        if (error != null && error.isEmpty()) {
            jLabel1.setIcon(ERROR_ICON);
        } else {
            jLabel1.setIcon(MESSAGE_ICON);
        }
    }
}
