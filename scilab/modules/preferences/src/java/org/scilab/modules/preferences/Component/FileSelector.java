/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Pierre GRADIT
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

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;

import javax.swing.GroupLayout;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.SwingUtilities;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;

import org.scilab.modules.commons.ScilabCommonsUtils;
import org.scilab.modules.preferences.XCommonManager;
import org.scilab.modules.preferences.XComponent;
import org.scilab.modules.preferences.XChooser;
import org.scilab.modules.preferences.XConfigManager;
import org.scilab.modules.gui.bridge.filechooser.SwingScilabFileChooser;

import org.w3c.dom.Node;

/** Implementation of Entry compliant with extended management.
 *
 * @author Calixte DENIZET
 *
 */
public class FileSelector extends Panel implements XComponent, XChooser, DocumentListener {

    /** Universal identifier for serialization.
     *
     */
    private static final long serialVersionUID = -7007541669965737408L;

    private static final Color NORMALCOLOR = Color.BLACK;
    private static final Color ERRORCOLOR = Color.RED;

    private ActionListener actionListener;
    private JTextField textField;
    private JButton button;
    private String currentDir = System.getProperty("user.home");
    private boolean dirSelection;
    private boolean checkEntry;
    private String previousPath;

    /** Constructor.
     *
     * @param peer : associated view DOM node.
     */
    public FileSelector(final Node peer) {
        super(peer);
        setLayout(new GridBagLayout());

        textField = new JTextField();
        textField.setEditable(true);
        textField.setColumns(10);
        textField.getDocument().addDocumentListener(this);

        button = new JButton("...");
        button.setPreferredSize(new Dimension(button.getPreferredSize().width, textField.getPreferredSize().height));
        button.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                JFrame frame = (JFrame) SwingUtilities.getAncestorOfClass(JFrame.class, FileSelector.this);
                SwingScilabFileChooser fileChooser = new SwingScilabFileChooser();
                fileChooser.setInitialDirectory(currentDir);
                fileChooser.setParentFrame(frame);
                fileChooser.setMultipleSelection(false);
                fileChooser.setFileSelectionMode(dirSelection ? SwingScilabFileChooser.DIRECTORIES_ONLY : SwingScilabFileChooser.FILES_ONLY);
                String mask = XCommonManager.getAttribute(peer, "mask");
                String desc = XCommonManager.getAttribute(peer, "desc");
                if (!mask.equals(XCommonManager.NAV) && !desc.equals(XCommonManager.NAV)) {
                    fileChooser.addMask(new String[] {mask}, new String[] {desc});
                }
                fileChooser.displayAndWait();

                String[] paths = fileChooser.getSelection();
                if (paths != null && paths.length != 0 && !paths[0].isEmpty()) {
                    File file = new File(paths[0]);
                    if (dirSelection) {
                        currentDir = file.getAbsolutePath();
                    } else {
                        currentDir = file.getParentFile().getAbsolutePath();
                    }

                    textField.setText(file.getAbsolutePath());
                }
            }
        });

        GridBagConstraints gbc = new GridBagConstraints();
        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.weightx = 1;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        add(textField, gbc);
        gbc.gridx = 1;
        gbc.weightx = 0;
        add(button, gbc);

        textField.setRequestFocusEnabled(true);
        textField.setFocusable(true);
        button.setRequestFocusEnabled(true);
        button.setFocusable(true);

        String href = XConfigManager.getAttribute(peer, "href");
        href(href);
        textField.setCaretPosition(0);

        String dirsel = XCommonManager.getAttribute(peer, "dir-selection", "false");
        dirSelection = dirsel.equals("true");

        String checkentry = XConfigManager.getAttribute(peer, "check-entry", "true");
        checkEntry = checkentry.equals("true");
    }

    public void addDocumentListener(DocumentListener listener) {
        textField.getDocument().addDocumentListener(listener);
    }

    /** Define the set of actuators.
     *
     * @return array of actuator names.
     */
    public final String[] actuators() {
        String[] actuators = {"enable", "href", "desc", "mask", "dir-selection", "check-entry"};
        return actuators;
    }

    /** Refresh the component by the use of actuators.
     *
     * @param peer the corresponding view DOM node
     */
    public final void refresh(final Node peer) {
        String href = XCommonManager.getAttribute(peer, "href");
        if (!href.equals(href())) {
            href(href);
        }

        String dirsel = XCommonManager.getAttribute(peer, "dir-selection", "false");
        dirSelection = dirsel.equals("true");

        String enable = XConfigManager.getAttribute(peer, "enable", "true");
        textField.setEnabled(enable.equals("true"));
        button.setEnabled(enable.equals("true"));

        String checkentry = XConfigManager.getAttribute(peer, "check-entry", "true");
        checkEntry = checkentry.equals("true");
    }

    /** Sensor for 'href' attribute.
     *
     * @return the attribute value.
     */
    public final String href() {
        if (checkPath()) {
            return textField.getText();
        }

        return null;
    }

    /** Actuator for 'href' attribute.
     *
     * @param text : the attribute value.
     */
    public final void href(final String href) {
        if (!href.equals(XCommonManager.NAV)) {
            textField.setText(href);
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
        if (checkPath() && actionListener != null) {
            ActionEvent transmit = new ActionEvent(this, 0, "File change", System.currentTimeMillis(), 0);
            actionListener.actionPerformed(transmit);
        }
    }

    private boolean checkPath() {
        String path = textField.getText();
        if (path.isEmpty()) {
            return false;
        }

        path = ScilabCommonsUtils.getCorrectedPath(path);

        File file = new File(path);
        boolean ok = false;
        if (file.exists()) {
            if (dirSelection) {
                ok = file.isDirectory();
            } else {
                ok = file.isFile();
            }
        }

        if (!checkEntry || ok) {
            textField.setForeground(NORMALCOLOR);
        } else {
            textField.setForeground(ERRORCOLOR);
        }

        return checkEntry ? ok : true;
    }

    /** Actual response read by the listener.
     *
     * @return response read by the listener.
     */
    public final Object choose() {
        return href();
    }

    /** Developer serialization method.
     *
     * @return equivalent signature.
     */
    public final String toString() {
        String signature = "FileSelector";
        String href = href();
        if (href != null && !href.equals(XCommonManager.NAV)) {
            signature += " href='" + href + "'";
        }

        return signature;
    }

    /** Registration of a single listener.
     * @param listener for the unique actionListener.
     */
    public final void addActionListener(final ActionListener listener) {
        actionListener = listener;
    }

}

