/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.ui_data.filebrowser;

import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;

import javax.swing.JButton;
import javax.swing.JPanel;

import org.scilab.modules.ui_data.filebrowser.actions.ChangeCWDAction;

/**
 * The JPanel containing the combobox used to set the path and the two buttons
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class ScilabFileSelectorPanel extends JPanel {

    private SwingScilabTreeTable table;

    /**
     * Default constructor
     * @param table the table linked with the combobox
     */
    public ScilabFileSelectorPanel(SwingScilabTreeTable table) {
        super();
        this.table = table;
        init();
    }

    /**
     * Init the panel contents
     */
    private void init() {
        setLayout(new GridBagLayout());
        JButton changeDirButton = ((ChangeCWDAction) table.getActionMap().get("cwd")).createButton();
        JButton upDirButton = ((ChangeCWDAction) table.getActionMap().get("cwd")).createParentDirButton();

        GridBagConstraints gbc = new GridBagConstraints();

        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.gridwidth = 1;
        gbc.gridheight = 1;
        gbc.weightx = 0;
        gbc.anchor = gbc.LINE_START;
        add(changeDirButton, gbc);

        gbc.gridx = 1;
        gbc.gridwidth = gbc.gridheight = 1;
        gbc.weightx = 1;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.anchor = gbc.LINE_START;
        add(table.getComboBox(), gbc);

        gbc.gridx = 2;
        gbc.gridwidth = gbc.gridheight = 1;
        gbc.weightx = 0;
        gbc.anchor = gbc.LINE_START;
        add(upDirButton, gbc);
    }
}
