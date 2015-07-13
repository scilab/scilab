/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.palette.view;

import javax.swing.BoxLayout;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;

/**
 * Implement a view to show the search results.
 * @author Marcos Cardinot <mcardinot@gmail.com>
 */
@SuppressWarnings(value = { "serial" })
public class PaletteSearchView extends JScrollPane {

    private JPanel panel;

    /**
     * Default constructor
     */
    public PaletteSearchView() {
        this.panel = new JPanel();
        initComponents();
    }

    /** Setup component */
    private void initComponents() {
        panel.setLayout(new BoxLayout(panel, BoxLayout.PAGE_AXIS));
        panel.add(new JLabel("Searching..."));
        setViewportView(panel);
    }
}
